#include "stdafx.h"
#include "Unit.h"

namespace Game
{
	WORD C_Unit::m_UnitCount = 0;

	C_Unit::C_Unit()
	{
		m_Info = nullptr;

		m_pPosition = nullptr;
		m_pTargetPos = nullptr;
		m_pScale = nullptr;

		m_pAnimator = nullptr;

		m_AnimIndex = 0;
		m_pGameFrameTimer = nullptr;
		m_UnitState = E_UnitState::Init;
		m_Direction = E_Direction::Right_Down_Down;
		m_TargetDir = E_Direction::Right_Down_Down;
	}
	C_Unit::~C_Unit()
	{
		Destroy();
	}

	HRESULT C_Unit::Create()
	{
		m_Info = new S_UnitInfo();

		m_pPosition = new D3DXVECTOR2(100.0f, 100.0f);
		m_pTargetPos = new D3DXVECTOR2(100.0f, 100.0f);
		m_pScale = new D3DXVECTOR2(1.0f, 1.0f);

		m_pAnimator = new C_Animator();
		SAFE_CREATE(m_pAnimator);
		m_pAnimator->SetSampleRate(g_InGameFPS);
		m_pAnimator->AddFunc(0, [this]() -> void
			{
				++m_AnimIndex;
			});

		m_pGameFrameTimer = new C_Timer();
		m_pGameFrameTimer->SetTime(g_InGameFPS);
		m_pGameFrameTimer->Play();

		return S_OK;
	}
	void C_Unit::Destroy()
	{
		SAFE_DELETE(m_pGameFrameTimer);

		SAFE_DELETE(m_pScale);
		SAFE_DELETE(m_pTargetPos);
		SAFE_DELETE(m_pPosition);

		SAFE_DELETE(m_pAnimator);

		SAFE_DELETE(m_Info);
	}

	HRESULT C_Unit::Update(const FLOAT& _deltaTime)
	{
		SAFE_UPDATE(m_pAnimator);

		return S_OK;
	}
	
}