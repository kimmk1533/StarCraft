#pragma once
#include <Singleton.h>

namespace Game
{
	using namespace CoreEngine;

	class C_MapManager : public C_Singleton<C_MapManager>, public IFrameWork, public IUpdatable, public IRenderable
	{
		friend class C_Singleton<C_MapManager>;
		static const std::string Path;

	private:
		std::string m_MapName;
		std::string m_TileSetName;

		uint16_t m_MapWidth;
		uint16_t m_MapHeight;
		uint16_t m_WidthPixel;
		uint16_t m_HeightPixel;

		RECT m_TileSetSize;

		std::vector<std::string>* m_pTileSetNameList;
		std::unordered_map<std::string, uint16_t>* m_pMegaTileNumber;
		std::unordered_map<std::string, std::shared_ptr<C_Texture>>* m_pTileSetList;
		std::vector<RECT>* m_pTileSetRect;
		std::vector<uint16_t>* m_pTileSetIndex;
		
		std::vector<uint8_t>* m_pMapBuffer;

	private:
		enum class E_WalkAbilityRes : uint8_t
		{
			None = 0,

			High = 1,
			Middle = 2,
			Low = 4,

			Max = 4,

		};

		E_WalkAbilityRes m_WalkAbility_Res;
		bool** m_pWalkAbility_HighRes;
		bool** m_pWalkAbility_MidRes;
		bool** m_pWalkAbility_LowRes;
		bool** m_pWalkAbility;

	public:
		READONLY_PROPERTY(uint16_t, width);
		GET(width) { return m_MapWidth; }

		READONLY_PROPERTY(uint16_t, height);
		GET(height) { return m_MapHeight; }

	private:
		int FindOffset(const std::string& _item) const;

		HRESULT Open_chk_File();
		HRESULT SetMapSize();

	public:
		HRESULT LoadMap(const std::string& _mapName);
		HRESULT CreateTerrain();

		std::vector<D3DXVECTOR3> GetPath(const D3DXVECTOR3& _start, const D3DXVECTOR3& _end);

	private:
		C_MapManager();
		virtual ~C_MapManager();

		C_MapManager(const C_MapManager& _other) = delete;
		void operator=(const C_MapManager& _other) = delete;

	public:
		virtual HRESULT Create() override;
		virtual void Destroy() override;

		virtual HRESULT Update(const FLOAT& _deltaTime) override;
		virtual HRESULT Render() override;

	};
}