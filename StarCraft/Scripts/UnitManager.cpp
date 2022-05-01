#include "UnitManager.h"

C_UnitManager::C_UnitManager()
{

}

const std::shared_ptr<C_Texture*>& C_UnitManager::GetUnitTexture()
{
	return m_pUnitTexture;
}
