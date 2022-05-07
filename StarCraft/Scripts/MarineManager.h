#pragma once
#include "UnitManager.cpp"

namespace Game
{
	using namespace CoreEngine;

	class C_MarineManager final : public C_UnitManager<C_MarineManager>
	{
		friend class C_Singleton;

	private:
		C_MarineManager();
		virtual ~C_MarineManager();
		
		C_MarineManager(const C_MarineManager& _other) = delete;

	public:
		virtual HRESULT Create() override;
		virtual void Destroy() override;
	};
}
