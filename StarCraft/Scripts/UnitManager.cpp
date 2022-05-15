#include "..\..\CoreEngine\Scripts\Singleton.cpp"

#include "UnitManager.h"
#include "UnitInfo.h"
#include "..\..\CoreEngine\Scripts\Texture.h"

namespace Game
{
	template <class T>
	C_UnitManager<T>::C_UnitManager()
	{
		m_pUnitTexture = nullptr;
		m_pUnitTextureRect = nullptr;
	}
	template<class T>
	C_UnitManager<T>::~C_UnitManager()
	{
		Destroy();
	}

	template<class T>
	void C_UnitManager<T>::SetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const RECT& _rect)
	{
		this->SetTextureRect({ _state, _direction }, _rect);
	}
	template<class T>
	void C_UnitManager<T>::SetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const WORD& _maxIndex, const RECT& _rect)
	{
		this->SetTextureRect({ _state, _direction }, { _maxIndex, _rect });
	}
	template<class T>
	void C_UnitManager<T>::SetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const std::pair<WORD, RECT>& _condition)
	{
		this->SetTextureRect({ _state, _direction }, _condition);
	}
	template<class T>
	void C_UnitManager<T>::SetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const RECT& _rect)
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
	template<class T>
	void C_UnitManager<T>::SetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const WORD& _maxIndex, const RECT& _rect)
	{
		this->SetTextureRect(_state, { _maxIndex, _rect });
	}
	template<class T>
	void C_UnitManager<T>::SetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const std::pair<WORD, RECT>& _condition)
	{
		if (m_pUnitTextureRect == nullptr)
			return;

		(*m_pUnitTextureRect)[_state] = _condition;
	}
	
	template<class T>
	HRESULT C_UnitManager<T>::Create()
	{
		m_pUnitTextureRect = new std::unordered_map<std::pair<E_UnitState, E_Direction>, std::pair<WORD, RECT>, Pair_Hash>();

		return S_OK;
	}
	template<class T>
	void C_UnitManager<T>::Destroy()
	{
		SAFE_DELETE(m_pUnitTextureRect);
		m_pUnitTexture.reset();
	}

	template <class T>
	std::shared_ptr<C_Texture> C_UnitManager<T>::GetTexture()
	{
		return m_pUnitTexture;
	}
	template<class T>
	const RECT C_UnitManager<T>::GetTextureRect(const E_UnitState& _state, const E_Direction& _direction, WORD& _index)
	{
		return this->GetTextureRect({ _state, _direction }, _index);
	}
	template<class T>
	const RECT C_UnitManager<T>::GetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, WORD& _index)
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
}