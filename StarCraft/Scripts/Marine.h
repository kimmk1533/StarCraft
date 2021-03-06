#pragma once
#include "Unit.h"

namespace Game
{
	using namespace CoreEngine;

	class C_Marine : public C_Unit
	{
	public:
		C_Marine();
		virtual ~C_Marine() override;

	public:
		virtual HRESULT	Create() override;
		virtual void	Destroy() override;

		virtual HRESULT	Update(const FLOAT& _deltaTime) override;
		virtual HRESULT	Render() override;

	};
}