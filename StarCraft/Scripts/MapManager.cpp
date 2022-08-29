#include "stdafx.h"
#include "MapManager.h"

#include <lodepng.h>
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

		/*if (!m_pMapBuffer)
			return -1;

		std::vector<std::string> temp;
		for (size_t i = 0; i < m_pMapBuffer->size(); ++i)
		{
			std::string str;
			str += (*m_pMapBuffer)[i];
			temp.push_back(str);
		}

		auto offset_iter = std::find(temp.begin(), temp.end(), "DIM ");
		if (offset_iter == temp.end())
			return -1;

		return offset_iter - temp.begin();*/
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
		int offset = FindOffset("DIM ");
		if (offset == -1)
			return E_FAIL;

		m_MapWidth = (*m_pMapBuffer)[offset + 8];
		m_MapHeight = (*m_pMapBuffer)[offset + 10];

		m_WidthPixel = m_MapWidth * 32;
		m_HeightPixel = m_MapHeight * 32;

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
					uint32_t width = texture->GetImageWidth() / 32;

					uint16_t real_x = real_megatile_index % width;
					uint16_t real_y = real_megatile_index / width;

					RECT rc{ real_x * 32, real_y * 32, (real_x + 1) * 32, (real_y + 1) * 32 };

					m_pTileSetRect->push_back(rc);
					m_pTileSetIndex->push_back(megatile_length);
					check[real_megatile_index] = megatile_length++;
				}

				buffer_index += 2;
			}
		}

		m_pMapBuffer->clear();
		delete[] cv5_buffer;
		fin_cv5.close();

		return S_OK;
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
		SAFE_DELETE(m_pMapBuffer);
		SAFE_DELETE(m_pTileSetIndex);
		SAFE_DELETE(m_pTileSetRect);
		m_pTileSetRect->clear();
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