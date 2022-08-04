#pragma once
#include <Physics.h>
#include "Unit.h"

namespace Game
{
	enum class E_UnitState : unsigned char;
	enum class E_Direction : unsigned char;
}

namespace Game
{
	using namespace CoreEngine;

	template <class TManager, class TUnit>
	class C_UnitManager : public C_Singleton<TManager>, public IFrameWork, public IUpdatable, public IRenderable
	{
	protected: // Unit
		RECT m_rcUnitSize;
		std::shared_ptr<C_Texture> m_pUnitTexture;
		std::unordered_map<std::pair<E_UnitState, E_Direction>, std::pair<WORD, RECT>, Pair_Hash>* m_pUnitTextureRect;

		C_ObjectPool<TUnit>* m_pUnitPool;

	protected:
		void SetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const RECT& _rect)
		{
			this->SetTextureRect({ _state, _direction }, _rect);
		}
		void SetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const WORD& _maxIndex, const RECT& _rect)
		{
			this->SetTextureRect({ _state, _direction }, { _maxIndex, _rect });
		}
		void SetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const std::pair<WORD, RECT>& _condition)
		{
			this->SetTextureRect({ _state, _direction }, _condition);
		}
		void SetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const RECT& _rect)
		{
			int width = _rect.right - _rect.left;
			int size = m_rcUnitSize.right - m_rcUnitSize.left;

			if (width <= 0)
			{
				MessageBox(nullptr, TEXT("Error: UnitTexture Rect Width"), TEXT("Error!"), MB_OK);
				return;
			}

			this->SetTextureRect(_state, { width / size, _rect });
		}
		void SetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const WORD& _maxIndex, const RECT& _rect)
		{
			this->SetTextureRect(_state, { _maxIndex, _rect });
		}
		void SetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const std::pair<WORD, RECT>& _condition)
		{
			if (m_pUnitTextureRect == nullptr)
				return;

			(*m_pUnitTextureRect)[_state] = _condition;
		}

	public:
		std::shared_ptr<C_Texture> GetTexture()
		{
			return m_pUnitTexture;
		}
		const RECT GetTextureRect(const E_UnitState& _state, const E_Direction& _direction, WORD& _index)
		{
			return this->GetTextureRect({ _state, _direction }, _index);
		}
		const RECT GetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, WORD& _index)
		{
			std::pair<WORD, RECT> index_rect = (*m_pUnitTextureRect)[_state];
			RECT rect = index_rect.second;

			if (index_rect.first <= _index)
			{
				_index = 0;
			}

			if (index_rect.first > 1)
			{
				int size = m_rcUnitSize.right - m_rcUnitSize.left;

				rect.left += _index * size;
				rect.right = rect.left + size;
			}

			return rect;
		}
		const WORD& GetTextureMaxIndex(const E_UnitState& _state, const E_Direction& _direction)
		{
			return this->GetTextureMaxIndex({ _state, _direction });
		}
		const WORD& GetTextureMaxIndex(const std::pair<E_UnitState, E_Direction> _state)
		{
			return (*m_pUnitTextureRect)[_state].first;
		}

	public:
		std::shared_ptr<TUnit> SpawnUnit()
		{
			static_assert(std::is_base_of_v<C_Unit, TUnit>, "TUnit of UnitManagerT> is not base of C_Unit class.");

			std::shared_ptr<TUnit> unit_smtptr = m_pUnitPool->Spawn();
			C_Unit* unit_ptr = static_cast<C_Unit*>(unit_smtptr.get());

			Physics::AddCollision(unit_ptr->collider);

			return unit_smtptr;
		}
		const std::vector<std::shared_ptr<TUnit>>* GetSpawnedObjList() { return m_pUnitPool->GetSpawnedObjList(); }

	protected:
		C_UnitManager()
		{
			m_pUnitTexture = nullptr;
			m_pUnitTextureRect = nullptr;
			m_pUnitPool = nullptr;
		}
		virtual ~C_UnitManager()
		{
			Destroy();
		}

	public:
		virtual HRESULT Create() override
		{
			if (!std::is_base_of_v<C_Unit, TUnit>)
				return E_FAIL;

			m_pUnitPool = new C_ObjectPool<TUnit>(100);
			if (FAILED(m_pUnitPool->Create()))
				return E_FAIL;

			m_pUnitTextureRect = new std::unordered_map<std::pair<E_UnitState, E_Direction>, std::pair<WORD, RECT>, Pair_Hash>();

			return S_OK;
		}
		virtual void	Destroy() override
		{
			SAFE_DELETE(m_pUnitPool);
			SAFE_DELETE(m_pUnitTextureRect);
			m_pUnitTexture.reset();
		}

		virtual HRESULT Update(const FLOAT& _deltaTime) override
		{
			static_assert(std::is_base_of_v<IUpdatable, TUnit>, "UnitManager TUnit is not base of IUpdatable");

			const std::vector<std::shared_ptr<TUnit>>* list = m_pUnitPool->GetSpawnedObjList();
			for (unsigned int i = 0; i < list->size(); ++i)
			{
				(*list)[i]->Update(_deltaTime);
			}

			return S_OK;
		}
		virtual HRESULT Render() override
		{
			static_assert(std::is_base_of_v<IRenderable, TUnit>, "UnitManager TUnit is not base of IRenderable");

			const std::vector<std::shared_ptr<TUnit>>* list = m_pUnitPool->GetSpawnedObjList();
			for (unsigned int i = 0; i < list->size(); ++i)
			{
				(*list)[i]->Render();
			}

			return S_OK;
		}

	};
}