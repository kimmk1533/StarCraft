#pragma once
#include "..\..\CoreEngine\Scripts\Singleton.cpp"

namespace Game
{
	using namespace CoreEngine;

	template <class T>
	class C_UnitManager : public C_Singleton<T>, public IFrameWork
	{
	protected:
		C_UnitManager();
		virtual ~C_UnitManager();

		C_UnitManager(const C_UnitManager& _other) = delete;

	protected:
		std::shared_ptr<C_Texture> m_pUnitTexture;

	public:
		virtual HRESULT Create() override = 0;
		virtual void	Destroy() override = 0;

	public:
		std::shared_ptr<C_Texture> GetUnitTexture();

	};
}