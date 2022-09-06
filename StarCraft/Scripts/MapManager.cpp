#include "stdafx.h"
#include "MapManager.h"

#include <lodepng.h>
#include <AStar.h>

namespace Game
{
	const std::string C_MapManager::Path = ".//Resources/Map/";

	int C_MapManager::FindOffset(const std::string& _item) const
	{
		if (nullptr == m_pMapBuffer || m_pMapBuffer->empty())
			return -1;

		bool flag;
		size_t item_size = _item.size();
		size_t buffer_size = m_pMapBuffer->size() - item_size;

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
		if (nullptr == m_pMapBuffer)
			return E_FAIL;

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
		if (nullptr == m_pMapBuffer || m_pMapBuffer->empty())
			return E_FAIL;

		int offset = FindOffset("DIM ");
		if (offset == -1)
			return E_FAIL;

		m_MapWidth = (*m_pMapBuffer)[offset + 8];
		m_MapHeight = (*m_pMapBuffer)[offset + 10];

		m_WidthPixel = m_MapWidth * 32;
		m_HeightPixel = m_MapHeight * 32;

#pragma region WalkAbility

		uint16_t w_low = m_MapWidth * ((uint8_t)E_WalkAbilityRes::Max / (uint8_t)E_WalkAbilityRes::Low);
		uint16_t h_low = m_MapHeight * ((uint8_t)E_WalkAbilityRes::Max / (uint8_t)E_WalkAbilityRes::Low);
		uint16_t w_mid = m_MapWidth * ((uint8_t)E_WalkAbilityRes::Max / (uint8_t)E_WalkAbilityRes::Middle);
		uint16_t h_mid = m_MapHeight * ((uint8_t)E_WalkAbilityRes::Max / (uint8_t)E_WalkAbilityRes::Middle);
		uint16_t w_high = m_MapWidth * ((uint8_t)E_WalkAbilityRes::Max / (uint8_t)E_WalkAbilityRes::High);
		uint16_t h_high = m_MapHeight * ((uint8_t)E_WalkAbilityRes::Max / (uint8_t)E_WalkAbilityRes::High);

		for (uint16_t i = 0; i < h_high; ++i)
		{
			if (nullptr != m_pWalkAbility_LowRes && i < h_low)
				SAFE_DELETE_ARRAY(m_pWalkAbility_LowRes[i]);

			if (nullptr != m_pWalkAbility_MidRes && i < h_mid)
				SAFE_DELETE_ARRAY(m_pWalkAbility_MidRes[i]);

			if (nullptr != m_pWalkAbility_HighRes)
				SAFE_DELETE_ARRAY(m_pWalkAbility_HighRes[i]);
		}
		SAFE_DELETE_ARRAY(m_pWalkAbility_LowRes);
		SAFE_DELETE_ARRAY(m_pWalkAbility_MidRes);
		SAFE_DELETE_ARRAY(m_pWalkAbility_HighRes);
		m_pWalkAbility = nullptr;

		m_pWalkAbility_LowRes = new bool* [h_low];
		m_pWalkAbility_MidRes = new bool* [h_mid];
		m_pWalkAbility_HighRes = new bool* [h_high];
		for (uint16_t i = 0; i < h_high; ++i)
		{
			if (i < h_low)
				m_pWalkAbility_LowRes[i] = new bool[w_low];

			if (i < h_mid)
				m_pWalkAbility_MidRes[i] = new bool[w_mid];

			m_pWalkAbility_HighRes[i] = new bool[w_high];
		}

		switch (m_WalkAbility_Res)
		{
		case E_WalkAbilityRes::Low:
			m_pWalkAbility = m_pWalkAbility_LowRes;
			break;
		case E_WalkAbilityRes::Middle:
			m_pWalkAbility = m_pWalkAbility_MidRes;
			break;
		case E_WalkAbilityRes::High:
			m_pWalkAbility = m_pWalkAbility_HighRes;
			break;
		}

#pragma endregion

		RECT clipingRect = RECT{ 0, m_HeightPixel, m_WidthPixel - C_Engine::GetScreenRect().right, C_Engine::GetScreenRect().bottom };
		Camera->SetClipingRect(clipingRect);

		return S_OK;
	}
	HRESULT C_MapManager::CreateTerrain()
	{
		if (nullptr == m_pMapBuffer || m_pMapBuffer->empty())
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
#pragma endregion

#ifdef DEBUG_MapManager_SaveImage_WalkAbility
		std::vector<uint8_t> walkability(m_MapHeight * 4 * m_MapWidth * 4 * 4, 0xff);
#endif // DEBUG_MapManager_SaveImage_WalkAbility

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

#ifdef DEBUG_MapManager_SingleTexture
		std::shared_ptr<C_Texture> texture = (*this->m_pTileSetList)[m_TileSetName];
		uint32_t image_width = texture->GetImageWidth() / 32;
#else
		uint32_t image_width = 32;
#endif // DEBUG_MapManager_SingleTexture

		for (uint16_t y = 0; y < m_MapHeight; ++y)
		{
			for (uint16_t x = 0; x < m_MapWidth; ++x)
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
#ifdef DEBUG_MapManager_SingleTexture
					uint16_t real_x = real_megatile_index % image_width;
					uint16_t real_y = real_megatile_index / image_width;

					RECT rc{ real_x * 32, real_y * 32, (real_x + 1) * 32, (real_y + 1) * 32 };

					m_pTileSetRect->push_back(rc);
#else
					std::shared_ptr<C_Texture> texture = std::make_shared<C_Texture>();

					std::string filepath = Path + "TileSet/" + m_TileSetName + "/png/" +
						m_TileSetName + " (" + std::to_string(real_megatile_index) + ").png";
					texture->Init(filepath);
					texture->Create();
					m_pTileSetList->push_back(texture);
#endif // DEBUG_MapManager_SingleTexture
					m_pTileSetIndex->push_back(megatile_length);
					check[real_megatile_index] = megatile_length++;
				}

#pragma region WalkAbility
				uint8_t tileCount = 0;

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
							m_pWalkAbility_HighRes[index_y][index_x] = true;
						}
						else
						{
							m_pWalkAbility_HighRes[index_y][index_x] = false;
							++tileCount;

#ifdef DEBUG_MapManager_SaveImage_WalkAbility
							walkability[(index_y * m_MapWidth * 4 + index_x) * 4 + 0] = 0xff; // r
							walkability[(index_y * m_MapWidth * 4 + index_x) * 4 + 1] = 0x00; // g
							walkability[(index_y * m_MapWidth * 4 + index_x) * 4 + 2] = 0x00; // b
							walkability[(index_y * m_MapWidth * 4 + index_x) * 4 + 3] = 0xff; // a
#endif // DEBUG_MapManager_SaveImage_WalkAbility
						}
					}
				}

				if (tileCount > 8)
				{
					m_pWalkAbility_LowRes[y][x] = false;
				}
				else
				{
					m_pWalkAbility_LowRes[y][x] = true;
				}
#pragma endregion

				buffer_index += 2;
			}
		}

#pragma region WalkAbility
		for (uint16_t y = 0; y < m_MapHeight * 2; ++y)
		{
			for (uint16_t x = 0; x < m_MapWidth * 2; ++x)
			{
				uint8_t tileCount = 0;

				for (uint8_t i = 0; i < 2; ++i)
				{
					for (uint8_t j = 0; j < 2; ++j)
					{
						if (!(m_pWalkAbility_HighRes[y * 2 + i][x * 2 + j]))
							++tileCount;
					}
				}

				if (tileCount > 2)
					m_pWalkAbility_MidRes[y][x] = false;
				else
					m_pWalkAbility_MidRes[y][x] = true;
			}
		}
#pragma endregion

#ifdef DEBUG_MapManager_SaveImage_WalkAbility
		std::vector<uint8_t> imageBuffer;
		lodepng::encode(imageBuffer, walkability, m_MapWidth * 4, m_MapHeight * 4, LodePNGColorType::LCT_RGBA, 8);
		lodepng::save_file(imageBuffer, Path + m_MapName + "_walkAbility" + ".png");
#endif // DEBUG_MapManager_SaveImage_WalkAbility

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
		uint32_t width = m_MapWidth * ((uint8_t)E_WalkAbilityRes::Max / (uint8_t)m_WalkAbility_Res);
		uint32_t height = m_MapHeight * ((uint8_t)E_WalkAbilityRes::Max / (uint8_t)m_WalkAbility_Res);

		uint8_t m = 8 * (uint8_t)m_WalkAbility_Res;

		int32_t startX = (int32_t)roundf(_start.x / m);
		int32_t startY = (height - 1) - (int32_t)roundf(_start.y / m);
		int32_t endX = (int32_t)roundf(_end.x / m);
		int32_t endY = (height - 1) - (int32_t)roundf(_end.y / m);

		auto path = AStar::AStarFindPath(m_pWalkAbility, width, height, startX, startY, endX, endY);

		for (size_t i = 0; i < path.size(); ++i)
		{
			path[i].x *= m;
			path[i].y = (height - 1) - path[i].y;
			path[i].y *= m;
		}

		return path;
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
#ifdef DEBUG_MapManager_SingleTexture
		m_pTileSetRect = nullptr;
#endif // DEBUG_MapManager_SingleTexture
		m_pTileSetIndex = nullptr;

		m_pMapBuffer = nullptr;

		m_WalkAbility_Res = E_WalkAbilityRes::None;
		m_pWalkAbility_HighRes = nullptr;
		m_pWalkAbility_MidRes = nullptr;
		m_pWalkAbility_LowRes = nullptr;
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

#ifdef DEBUG_MapManager_SingleTexture
		m_pTileSetList = new std::unordered_map<std::string, std::shared_ptr<C_Texture>>();

		for (size_t i = 0; i < (*m_pTileSetNameList).size(); ++i)
		{
			std::shared_ptr<C_Texture> texture = std::make_shared<C_Texture>();

			std::string filepath = Path + "TileSet/" + (*m_pTileSetNameList)[i] + "/" +	// Folder Path
				(*m_pTileSetNameList)[i] + "_sheet.png";								// File Name

			texture->Init(filepath);
			texture->Create();

			(*m_pTileSetList)[(*m_pTileSetNameList)[i]] = texture;
		}

		m_pTileSetRect = new std::vector<RECT>();
#else
		m_pTileSetList = new std::vector<std::shared_ptr<C_Texture>>();
#endif // DEBUG_MapManager_SingleTexture

		m_pTileSetIndex = new std::vector<uint16_t>();

		m_pMapBuffer = new std::vector<uint8_t>();

		m_WalkAbility_Res = E_WalkAbilityRes::High;

		return S_OK;
	}
	void C_MapManager::Destroy()
	{
		uint16_t low = m_MapHeight * (uint8_t)E_WalkAbilityRes::Max / (uint8_t)E_WalkAbilityRes::Low;
		uint16_t mid = m_MapHeight * (uint8_t)E_WalkAbilityRes::Max / (uint8_t)E_WalkAbilityRes::Middle;
		uint16_t high = m_MapHeight * (uint8_t)E_WalkAbilityRes::Max / (uint8_t)E_WalkAbilityRes::High;
		for (uint16_t i = 0; i < high; ++i)
		{
			if (i < low)
				SAFE_DELETE_ARRAY(m_pWalkAbility_LowRes[i]);

			if (i < mid)
				SAFE_DELETE_ARRAY(m_pWalkAbility_MidRes[i]);

			SAFE_DELETE_ARRAY(m_pWalkAbility_HighRes[i]);
		}
		SAFE_DELETE_ARRAY(m_pWalkAbility_LowRes);
		SAFE_DELETE_ARRAY(m_pWalkAbility_MidRes);
		SAFE_DELETE_ARRAY(m_pWalkAbility_HighRes);
		m_pWalkAbility = nullptr;

		SAFE_DELETE(m_pMapBuffer);
		SAFE_DELETE(m_pTileSetIndex);
#ifdef DEBUG_MapManager_SingleTexture
		SAFE_DELETE(m_pTileSetRect);
#endif // DEBUG_MapManager_SingleTexture
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
		Sprite->SetScale(nullptr);

#ifdef DEBUG_MapManager_SingleTexture
		std::shared_ptr<C_Texture> texture = (*this->m_pTileSetList)[m_TileSetName];
#endif // DEBUG_MapManager_SingleTexture

		static const D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);

		Sprite->Begin();

		for (size_t y = 0; y < m_MapHeight; ++y)
		{
			for (size_t x = 0; x < m_MapWidth; ++x)
			{
				size_t index = y * m_MapWidth + x;
#ifdef DEBUG_MapManager_SingleTexture
				uint16_t rect_index = (*m_pTileSetIndex)[index];
#else
				std::shared_ptr<C_Texture> tile = (*m_pTileSetList)[(*m_pTileSetIndex)[index]];
#endif // DEBUG_MapManager_SingleTexture

				Sprite->SetTranslation(x * 32, (m_MapHeight - y) * 32, 0.0f);

#ifdef DEBUG_MapManager_SingleTexture
				Sprite->Draw(texture->GetTexture(), &(*m_pTileSetRect)[rect_index], &center, D3DCOLOR_XRGB(255, 255, 255));
#else
				Sprite->Draw(tile->GetTexture(), &m_TileSetSize, &center, D3DCOLOR_XRGB(255, 255, 255));
#endif // DEBUG_MapManager_SingleTexture
			}
		}

		Sprite->End();

		return S_OK;
	}
}