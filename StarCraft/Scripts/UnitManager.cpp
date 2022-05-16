#include "..\..\CoreEngine\Scripts\Singleton.cpp"

#include "UnitManager.h"
#include "UnitInfo.h"

namespace Game
{
	template <class Manager, class Unit>
	C_UnitManager<Manager, Unit>::C_UnitManager()
	{
		m_pUnitInfo = nullptr;
		m_pUnitTexture = nullptr;
		m_pUnitTextureRect = nullptr;
		m_pUnitPool = nullptr;
	}
	template<class Manager, class Unit>
	C_UnitManager<Manager, Unit>::~C_UnitManager()
	{
		Destroy();
	}
	
	template<class Manager, class Unit>
	HRESULT C_UnitManager<Manager, Unit>::Create()
	{
		m_pUnitTextureRect = new std::unordered_map<std::pair<E_UnitState, E_Direction>, std::pair<WORD, RECT>, Pair_Hash>();
		m_pUnitPool = new C_ObjectPool<Unit>(100);
		if (FAILED(m_pUnitPool->Create()))
			return E_FAIL;

		return S_OK;
	}
	template<class Manager, class Unit>
	void C_UnitManager<Manager, Unit>::Destroy()
	{
		SAFE_DELETE(m_pUnitPool);
		SAFE_DELETE(m_pUnitTextureRect);
		m_pUnitTexture.reset();
		SAFE_DELETE(m_pUnitInfo);
	}

	template<class Manager, class Unit>
	HRESULT C_UnitManager<Manager, Unit>::Update(const FLOAT& _deltaTime)
	{
		if (!std::is_base_of<IUpdatable, Unit>::value)
			return S_OK;

		const std::vector<std::shared_ptr<Unit>>* list = m_pUnitPool->GetSpawnedObjList();
		for (int i = 0; i < list->size(); ++i)
		{
			(*list)[i]->Update(_deltaTime);
		}

		return S_OK;
	}
	template<class Manager, class Unit>
	HRESULT C_UnitManager<Manager, Unit>::Render()
	{
		if (!std::is_base_of<IRenderable, Unit>::value)
			return S_OK;

		const std::vector<std::shared_ptr<Unit>>* list = m_pUnitPool->GetSpawnedObjList();
		for (int i = 0; i < list->size(); ++i)
		{
			(*list)[i]->Render();
		}

		return S_OK;
	}

	template<class Manager, class Unit>
	void C_UnitManager<Manager, Unit>::SetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const RECT& _rect)
	{
		this->SetTextureRect({ _state, _direction }, _rect);
	}
	template<class Manager, class Unit>
	void C_UnitManager<Manager, Unit>::SetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const WORD& _maxIndex, const RECT& _rect)
	{
		this->SetTextureRect({ _state, _direction }, { _maxIndex, _rect });
	}
	template<class Manager, class Unit>
	void C_UnitManager<Manager, Unit>::SetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const std::pair<WORD, RECT>& _condition)
	{
		this->SetTextureRect({ _state, _direction }, _condition);
	}
	template<class Manager, class Unit>
	void C_UnitManager<Manager, Unit>::SetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const RECT& _rect)
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
	template<class Manager, class Unit>
	void C_UnitManager<Manager, Unit>::SetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const WORD& _maxIndex, const RECT& _rect)
	{
		this->SetTextureRect(_state, { _maxIndex, _rect });
	}
	template<class Manager, class Unit>
	void C_UnitManager<Manager, Unit>::SetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const std::pair<WORD, RECT>& _condition)
	{
		if (m_pUnitTextureRect == nullptr)
			return;

		(*m_pUnitTextureRect)[_state] = _condition;
	}

	template <class Manager, class Unit>
	std::shared_ptr<C_Texture> C_UnitManager<Manager, Unit>::GetTexture()
	{
		return m_pUnitTexture;
	}
	template<class Manager, class Unit>
	const RECT C_UnitManager<Manager, Unit>::GetTextureRect(const E_UnitState& _state, const E_Direction& _direction, WORD& _index)
	{
		return this->GetTextureRect({ _state, _direction }, _index);
	}
	template<class Manager, class Unit>
	const RECT C_UnitManager<Manager, Unit>::GetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, WORD& _index)
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

	template<class Manager, class Unit>
	std::shared_ptr<Unit> C_UnitManager<Manager, Unit>::SpawnUnit()
	{
		return m_pUnitPool->Spawn();
	}
}