#include "UnitManager.h"
#include "UnitInfo.h"

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
	const RECT C_UnitManager<T>::GetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const int& _index)
	{
		return this->GetTextureRect({ _state, _direction }, _index);
	}
	template<class T>
	const RECT C_UnitManager<T>::GetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const int& _index)
	{
		int maxIndex = this->GetTextureMaxIndex(_state);
		if (maxIndex <= _index)
		{
#ifdef DEBUG
			std::cout << "maxIndex: " << maxIndex << "\n";
			std::cout << "_index: " << _index << "\n";
#else
			MessageBox(nullptr, TEXT("Error: UnitTexture Index"), TEXT("Error!"), MB_OK);
#endif // DEBUG

			return RECT();
		}

		RECT rect = (*m_pUnitTextureRect)[_state];
		int size = m_rcUnitSize.right - m_rcUnitSize.left;

		if (maxIndex > 1)
		{
			rect.left += _index * size;
			rect.right = rect.left + size;
		}

		return rect;
	}
	template<class T>
	const int C_UnitManager<T>::GetTextureMaxIndex(const E_UnitState& _state, const E_Direction& _direction)
	{
		return this->GetTextureMaxIndex({ _state, _direction });
	}
	template<class T>
	const int C_UnitManager<T>::GetTextureMaxIndex(const std::pair<E_UnitState, E_Direction>& _state)
	{
		if (m_pUnitTextureRect == nullptr)
		{
			MessageBox(nullptr, TEXT("Error: UnitTexture Rect is nullptr"), TEXT("Error!"), MB_OK);
			return -1;
		}

		RECT rect = (*m_pUnitTextureRect)[_state];
		int width = rect.right - rect.left;
		int size = m_rcUnitSize.right - m_rcUnitSize.left;

		if (width <= 0)
		{
			MessageBox(nullptr, TEXT("Error: UnitTexture Rect Width"), TEXT("Error!"), MB_OK);
			return -1;
		}

		return width / size;
	}
	template<class T>
	void C_UnitManager<T>::SetTextureRect(const E_UnitState& _state, const E_Direction& _direction, const RECT& _rect)
	{
		this->SetTextureRect({ _state, _direction }, _rect);
	}
	template<class T>
	void C_UnitManager<T>::SetTextureRect(const std::pair<E_UnitState, E_Direction>& _state, const RECT& _rect)
	{
		if (m_pUnitTextureRect == nullptr)
			return;

		(*m_pUnitTextureRect)[_state] = _rect;
	}

	template<class T>
	HRESULT C_UnitManager<T>::Create()
	{
		m_pUnitTextureRect = new std::unordered_map<std::pair<E_UnitState, E_Direction>, RECT, Pair_Hash>();

		return S_OK;
	}
	template<class T>
	void C_UnitManager<T>::Destroy()
	{
		SAFE_DELETE(m_pUnitTextureRect);
	}

	template <class T>
	std::shared_ptr<C_Texture> C_UnitManager<T>::GetTexture()
	{
		return m_pUnitTexture;
	}
}