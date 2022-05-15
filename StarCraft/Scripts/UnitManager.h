#pragma once

namespace Game
{
	using namespace CoreEngine;

#pragma region extern
	enum class E_UnitState : unsigned char;
	enum class E_Direction : unsigned char;
	struct S_UnitInfo;
	class CoreEngine::C_Texture;
#pragma endregion

	template <class T>
	class C_UnitManager : public C_Singleton<T>, public IFrameWork
	{
	protected:
		C_UnitManager();
		virtual ~C_UnitManager();

		C_UnitManager(const C_UnitManager& _other) = delete;
		void operator=(const C_UnitManager& _other) = delete;

	protected:
		S_UnitInfo* m_pUnitInfo;
		std::shared_ptr<C_Texture> m_pUnitTexture;
		RECT m_rcUnitSize;
		std::unordered_map<std::pair<E_UnitState, E_Direction>, RECT, Pair_Hash>* m_pUnitTextureRect;

	protected:
		void SetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const RECT& _rect);
		void SetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const RECT& _rect);

	public:
		virtual HRESULT Create() override;
		virtual void	Destroy() override;

	public:
		std::shared_ptr<C_Texture> GetTexture();
		const RECT GetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const int& _index);
		const RECT GetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const int& _index);
		const int GetTextureMaxIndex(const E_UnitState& _state, const E_Direction& _direction);
		const int GetTextureMaxIndex(const std::pair<E_UnitState, E_Direction>& _state);
	};
}