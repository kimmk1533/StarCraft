#pragma once
#include "..\..\CoreEngine\Scripts\ObjectPool.cpp"

namespace Game
{
	using namespace CoreEngine;

#pragma region extern
	enum class E_UnitState : unsigned char;
	enum class E_Direction : unsigned char;
	struct S_UnitInfo;
	class CoreEngine::C_Texture;
#pragma endregion

	template <class Manager, class Unit>
	class C_UnitManager : public C_Singleton<Manager>, public IFrameWork, public IUpdatable, public IRenderable
	{
	protected:
		C_UnitManager();
		virtual ~C_UnitManager();

		C_UnitManager(const C_UnitManager& _other) = delete;
		void operator=(const C_UnitManager& _other) = delete;

	public:
		virtual HRESULT Create() override;
		virtual void	Destroy() override;

	public:
		virtual HRESULT Update(const FLOAT& _deltaTime) override;
		virtual HRESULT Render() override;

	protected:
		S_UnitInfo* m_pUnitInfo;
		std::shared_ptr<C_Texture> m_pUnitTexture;
		RECT m_rcUnitSize;
		std::unordered_map<std::pair<E_UnitState, E_Direction>, std::pair<WORD, RECT>, Pair_Hash>* m_pUnitTextureRect;
		C_ObjectPool<Unit>* m_pUnitPool;

	protected:
		void SetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const RECT& _rect);
		void SetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const WORD& _maxIndex, const RECT& _rect);
		void SetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const std::pair<WORD, RECT>& _condition);
		void SetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const RECT& _rect);
		void SetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const WORD& _maxIndex, const RECT& _rect);
		void SetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const std::pair<WORD, RECT>& _condition);

	public:
		std::shared_ptr<C_Texture> GetTexture();
		const RECT GetTextureRect(const E_UnitState& _state, const E_Direction& _direction, WORD& _index);
		const RECT GetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, WORD& _index);

	public:
		std::shared_ptr<Unit> SpawnUnit();

	};
}