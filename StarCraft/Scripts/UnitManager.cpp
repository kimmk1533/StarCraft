#include "UnitManager.h"

namespace Game
{
	template <class T>
	C_UnitManager<T>::C_UnitManager()
	{
		m_pUnitTexture = nullptr;
	}
	template<class T>
	C_UnitManager<T>::~C_UnitManager()
	{
		
	}

	template <class T>
	std::shared_ptr<C_Texture> C_UnitManager<T>::GetUnitTexture()
	{
		return m_pUnitTexture;
	}
}