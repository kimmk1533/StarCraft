#pragma once
#include "UnitManager.cpp"

namespace Game
{
	using namespace CoreEngine;

	class C_Marine;

	class C_MarineManager final : public C_UnitManager<C_MarineManager, C_Marine>
	{
		friend class C_Singleton<C_MarineManager>;

	private:
		C_MarineManager();
		virtual ~C_MarineManager();
		
		C_MarineManager(const C_MarineManager& _other) = delete;
		void operator=(const C_MarineManager& _other) = delete;

	public:
		virtual HRESULT Create() override;
		virtual void Destroy() override;

		virtual HRESULT Update(const FLOAT& _deltaTime) override;
		virtual HRESULT Render() override;

	};
}
