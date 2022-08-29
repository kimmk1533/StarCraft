#include "stdafx.h"
#include "MapManager.h"

#include <lodepng.h>
#include <AStar.h>

namespace Game
{
	const std::string C_MapManager::Path = ".//Resources/Map/";

	int C_MapManager::FindOffset(const std::string& _item) const
	{
		if (!m_pMapBuffer)
			return -1;

		size_t item_size = _item.size();
		size_t buffer_size = m_pMapBuffer->size() - item_size;
		bool flag = true;
		for (size_t i = 0; i < buffer_size; ++i)
		{
			flag = true;
			for (size_t j = 0; j < item_size; ++j)
			{
				if ((*m_pMapBuffer)[i + j] != _item[j])
				{
					flag = false;
					break;
				}
			}

			if (flag)
				return i;
		}

		return -1;
	}
	HRESULT C_MapManager::Open_chk_File()
	{
		if (!m_pMapBuffer)
		{
			m_pMapBuffer = new std::vector<uint8_t>();
		}

		std::ifstream fin_chk;
		fin_chk.open(Path + m_MapName + ".chk", std::ios::binary);
		if (!fin_chk)
		{
			fin_chk.close();
			return E_FAIL;
		}

		// 파일 크기 구하기
		fin_chk.seekg(0, fin_chk.end);
		int length = (int)fin_chk.tellg();
		fin_chk.seekg(0, fin_chk.beg);

		// 전체 파일 읽기
		m_pMapBuffer->clear();
		char* chk_buffer = new char[length];
		fin_chk.read((char*)chk_buffer, length);
		for (int i = 0; i < length; ++i)
		{
			m_pMapBuffer->push_back(chk_buffer[i]);
		}
		delete[] chk_buffer;

		fin_chk.close();

		return S_OK;
	}
	HRESULT C_MapManager::SetMapSize()
	{
		if (!m_pMapBuffer)
			return E_FAIL;

		int offset = FindOffset("DIM ");
		if (offset == -1)
			return E_FAIL;

		m_MapWidth = (*m_pMapBuffer)[offset + 8];
		m_MapHeight = (*m_pMapBuffer)[offset + 10];

		m_WidthPixel = m_MapWidth * 32;
		m_HeightPixel = m_MapHeight * 32;

		RECT clipingRect = RECT{ 0, m_HeightPixel, m_WidthPixel - C_Engine::GetScreenRect().right, C_Engine::GetScreenRect().bottom };
		Camera->SetClipingRect(clipingRect);

		return S_OK;
	}
	HRESULT C_MapManager::CreateTerrain()
	{
		if (!m_pMapBuffer)
			return E_FAIL;

		//
		// 1. ERA - tileset type->prepare megatile set
		//
		int offset = FindOffset("ERA ");
		if (offset == -1)
			return E_FAIL;

		uint16_t tileset_offset = (*m_pMapBuffer)[offset + 8] & 0x7; // 뒤의 3비트만 읽으면 됨
		m_TileSetName = (*m_pTileSetNameList)[tileset_offset];

		std::string tileSetPath = Path + "TileSet/" + m_TileSetName + "/" + m_TileSetName;

#pragma region cv5
		// 해당하는 CV5 파일 열기
		std::ifstream fin_cv5;
		fin_cv5.open(tileSetPath + ".cv5", std::ios::binary);
		if (!fin_cv5)
		{
			fin_cv5.close();
			return E_FAIL;
		}

		// 파일 크기 구하기
		fin_cv5.seekg(0, fin_cv5.end);
		int length = (int)fin_cv5.tellg();
		fin_cv5.seekg(0, fin_cv5.beg);

		// 전체 파일 읽기
		char* cv5_buffer = new char[length];
		fin_cv5.read((char*)cv5_buffer, length);
#pragma endregion

#pragma region vf4
		std::ifstream fin_vf4;
		fin_vf4.open(tileSetPath + ".vf4", std::ios::binary);
		if (!fin_vf4)
		{
			fin_vf4.close();
			return E_FAIL;
		}

		// 파일 크기 구하기
		fin_vf4.seekg(0, fin_vf4.end);
		length = (int)fin_vf4.tellg();
		fin_vf4.seekg(0, fin_vf4.beg);

		// 전체 파일 읽기
		char* vf4_buffer = new char[length];
		fin_vf4.read((char*)vf4_buffer, length);

		if (nullptr == m_pWalkAbility)
		{
			m_pWalkAbility = new bool* [m_MapHeight * 4];
			for (uint16_t i = 0; i < m_MapHeight * 4; ++i)
			{
				m_pWalkAbility[i] = new bool[m_MapWidth * 4];
			}
		}
#pragma endregion
#ifdef DEBUG_MapManager_SAVE_WALKABILITY_IMAGE
		std::vector<uint8_t> walkability(m_MapHeight * 4 * m_MapWidth * 4 * 4, 0xff);
#endif // DEBUG_MapManager_SAVE_WALKABILITY_IMAGE

		// 사용한 mega tile index 저장할 배열
		// -1 -> 사용 X
		std::vector<int16_t> check((*m_pMegaTileNumber)[this->m_TileSetName], -1);

		//
		// 2. MTXM - 좌표별 mega tile 정보
		//           CV5에서 몇번째 group이고
		//           group 내에서 몇 번째인지
		//
		int MTXM_offset = FindOffset("MTXM");

		int buffer_index = MTXM_offset + 8;
		int megatile_length = 0;

		std::shared_ptr<C_Texture> texture = (*this->m_pTileSetList)[m_TileSetName];
		uint32_t width = texture->GetImageWidth() / 32;

		for (int y = 0; y < m_MapHeight; ++y)
		{
			for (int x = 0; x < m_MapWidth; ++x)
			{
				uint8_t int_1 = (*m_pMapBuffer)[buffer_index];
				uint8_t int_2 = (*m_pMapBuffer)[buffer_index + 1];
				uint16_t megatile_info = int_2 << 8 ^ int_1;

				// CV5 내에서 group과 group 내에서의 index
				uint16_t group = megatile_info >> 4; // 상위 12 bit
				uint16_t group_index = megatile_info & 0xf; // 하위 4 bit

				// 실제 mega tile index
				uint8_t index_1 = cv5_buffer[group * 52 + 20 + 2 * group_index];
				uint8_t index_2 = cv5_buffer[group * 52 + 20 + 2 * group_index + 1];
				uint16_t real_megatile_index = index_2 << 8 ^ index_1;

				int16_t checking_index = check[real_megatile_index];

				// 이미 사용된 경우
				if (checking_index != -1)
				{
					m_pTileSetIndex->push_back(checking_index);
				}
				// 아닌 경우 지정 위치에서 mega tile 을 읽어와 리스트에 추가
				else
				{
					uint16_t real_x = real_megatile_index % width;
					uint16_t real_y = real_megatile_index / width;

					RECT rc{ real_x * 32, real_y * 32, (real_x + 1) * 32, (real_y + 1) * 32 };

					m_pTileSetRect->push_back(rc);
					m_pTileSetIndex->push_back(megatile_length);
					check[real_megatile_index] = megatile_length++;
				}

#pragma region WalkAbility
				// 하나의 megatile을 이루는 4 * 4
				// 총 16개의 mini tile 들의 flag 확인
				for (size_t i = 0; i < 4; ++i)
				{
					for (size_t j = 0; j < 4; ++j)
					{
						uint16_t flag = vf4_buffer[(real_megatile_index << 5) ^ (i << 3) ^ (j << 1)];

						uint16_t index_x = x * 4 + j;
						uint16_t index_y = y * 4 + i;

						if (flag % 2 == 1)
						{
							m_pWalkAbility[index_y][index_x] = true;
						}
						else
						{
							m_pWalkAbility[index_y][index_x] = false;

#ifdef DEBUG_MapManager_SAVE_WALKABILITY_IMAGE
							walkability[(index_y * 512 + index_x) * 4 + 0] = 0xff; // r
							walkability[(index_y * 512 + index_x) * 4 + 1] = 0x00; // g
							walkability[(index_y * 512 + index_x) * 4 + 2] = 0x00; // b
							walkability[(index_y * 512 + index_x) * 4 + 3] = 0xff; // a
#endif // DEBUG_MapManager_SAVE_WALKABILITY_IMAGE
						}
					}
				}
#pragma endregion

				buffer_index += 2;
			}
		}

#ifdef DEBUG_MapManager_SAVE_WALKABILITY_IMAGE
		std::vector<uint8_t> imageBuffer;
		lodepng::encode(imageBuffer, walkability, m_MapWidth * 4, m_MapHeight * 4, LodePNGColorType::LCT_RGBA, 8);
		lodepng::save_file(imageBuffer, Path + m_MapName + "_walkAbility" + ".png");
#endif // DEBUG_MapManager_SAVE_WALKABILITY_IMAGE

		m_pMapBuffer->clear();
		SAFE_DELETE(m_pMapBuffer);

		delete[] cv5_buffer;
		fin_cv5.close();

		delete[] vf4_buffer;
		fin_vf4.close();

		return S_OK;
	}
	std::vector<D3DXVECTOR3> C_MapManager::GetPath(const D3DXVECTOR3& _start, const D3DXVECTOR3& _end)
	{
		uint32_t width = m_MapWidth * 4;
		uint32_t height = m_MapHeight * 4;

		uint32_t startX = (int32_t)_start.x >> 3;
		uint32_t startY = (height - 1) - ((int32_t)_start.y >> 3);
		uint32_t endX = (int32_t)_end.x >> 3;
		uint32_t endY = (height - 1) - ((int32_t)_end.y >> 3);

		return AStar::AStarFindPath(m_pWalkAbility, width, height, startX, startY, endX, endY);
	}
	HRESULT C_MapManager::LoadMap(const std::string& _mapName)
	{
		m_MapName = _mapName;

		FAILED_CHECK_RETURN(Open_chk_File());
		FAILED_CHECK_RETURN(SetMapSize());

		return S_OK;
	}

	C_MapManager::C_MapManager()
	{
		m_MapName = "";
		m_TileSetName = "";

		m_MapWidth = 0;
		m_MapHeight = 0;
		m_WidthPixel = 0;
		m_HeightPixel = 0;

		m_pTileSetNameList = nullptr;
		m_pMegaTileNumber = nullptr;
		m_pTileSetList = nullptr;
		m_pTileSetRect = nullptr;
		m_pTileSetIndex = nullptr;

		m_pMapBuffer = nullptr;

		m_pWalkAbility = nullptr;
	}
	C_MapManager::~C_MapManager()
	{
		Destroy();
	}

	HRESULT C_MapManager::Create()
	{
		SetRect(&m_TileSetSize, 0, 0, 32, 32);

		m_pTileSetNameList = new std::vector<std::string>();
		m_pTileSetNameList->push_back("badlands");
		m_pTileSetNameList->push_back("platform");
		m_pTileSetNameList->push_back("install");
		m_pTileSetNameList->push_back("ashworld");
		m_pTileSetNameList->push_back("jungle");

		m_pMegaTileNumber = new std::unordered_map<std::string, uint16_t>();
		(*m_pMegaTileNumber)["badlands"] = 4844;
		(*m_pMegaTileNumber)["platform"] = 3055;
		(*m_pMegaTileNumber)["install"] = 1431;
		(*m_pMegaTileNumber)["ashworld"] = 3497;
		(*m_pMegaTileNumber)["jungle"] = 5046;

		m_pTileSetList = new std::unordered_map<std::string, std::shared_ptr<C_Texture>>();

		for (size_t i = 0; i < (*m_pTileSetNameList).size(); ++i)
		{
			std::shared_ptr<C_Texture> texture = std::make_shared<C_Texture>();

			std::string filepath = Path + "TileSet/" + (*m_pTileSetNameList)[i] + "/" +	// Folder Path
				(*m_pTileSetNameList)[i] + "_sheet.png";								// File Name

			texture->Init(filepath);
			texture->Create();

			//std::cout << (*m_pTileSetNameList)[i] << " (" << texture->GetImageWidth() << ", " << texture->GetImageHeight() << ")\n";

			(*m_pTileSetList)[(*m_pTileSetNameList)[i]] = texture;
		}


		m_pTileSetRect = new std::vector<RECT>();
		m_pTileSetIndex = new std::vector<uint16_t>();

		return S_OK;
	}
	void C_MapManager::Destroy()
	{
		for (uint16_t i = 0; i < m_MapHeight * 4; ++i)
		{
			SAFE_DELETE_ARRAY(m_pWalkAbility[i]);
		}
		SAFE_DELETE_ARRAY(m_pWalkAbility);
		SAFE_DELETE(m_pMapBuffer);
		SAFE_DELETE(m_pTileSetIndex);
		SAFE_DELETE(m_pTileSetRect);
		SAFE_DELETE(m_pTileSetList);
		SAFE_DELETE(m_pMegaTileNumber);
		SAFE_DELETE(m_pTileSetNameList);
	}

	HRESULT C_MapManager::Update(const FLOAT& _deltaTime)
	{
		return S_OK;
	}
	HRESULT C_MapManager::Render()
	{
		Sprite->SetRotation(nullptr);
		Sprite->SetScale(1.0f, 1.0f, 1.0f);

		std::shared_ptr<C_Texture> texture = (*this->m_pTileSetList)[m_TileSetName];

		static const D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);

		Sprite->Begin();

		for (size_t y = 0; y < m_MapHeight; ++y)
		{
			for (size_t x = 0; x < m_MapWidth; ++x)
			{
				size_t index = y * m_MapWidth + x;
				uint16_t rect_index = (*m_pTileSetIndex)[index];

				Sprite->SetTranslation(x * 32, (m_MapHeight - y) * 32, 0.0f);

				Sprite->Draw(texture->GetTexture(), &(*m_pTileSetRect)[rect_index], &center, D3DCOLOR_XRGB(255, 255, 255));
			}
		}

		Sprite->End();

		return S_OK;
	}
}