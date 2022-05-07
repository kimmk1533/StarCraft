#pragma once
#include "UnitInfo.h"

#include "Animator.h"

namespace Game
{
	using namespace CoreEngine;

	class C_Unit : public C_FrameWork, public IUpdatable, public IRenderable
	{
	protected:
		S_UnitInfo* m_Info;

	protected:
		D3DXVECTOR2 m_Position;
		D3DXVECTOR2 m_TargetPos;

		C_Animator* m_Animator;

	public:
		C_Unit();
		virtual ~C_Unit();

		virtual HRESULT	Create() override;
		virtual HRESULT	Update(const float _deltaTime) override = 0;
		virtual HRESULT	Render() override = 0;
		virtual void	Destroy() override;

	};
}