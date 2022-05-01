#pragma once
#include "..\..\CoreEngine\Scripts\Singleton.cpp"

class C_UnitManager :  public C_Singleton<C_UnitManager>
{
private:
	std::shared_ptr<C_Texture*> m_pUnitTexture;

protected:
	C_UnitManager();

	C_UnitManager(const C_UnitManager& other) = delete;

public:
	const std::shared_ptr<C_Texture*>& GetUnitTexture();

};
