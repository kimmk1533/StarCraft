#include "stdafx.h"
#include "Unit.h"

namespace Game
{
	C_Unit::C_Unit()
	{
		m_pPosition = nullptr;
		m_pTargetPos = nullptr;
		m_pScale = nullptr;

		m_Info = nullptr;
		m_pBoxCollider = nullptr;

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
		m_pPosition = std::make_shared<D3DXVECTOR3>(0.0f, 0.0f, 0.0f);
		m_pTargetPos = std::make_shared<D3DXVECTOR3>(0.0f, 0.0f, 0.0f);
		m_pScale = std::make_shared<D3DXVECTOR3>(1.0f, 1.0f, 1.0f);

		m_Info = new S_UnitInfo();
		m_pBoxCollider = new C_BoxCollider();
		SAFE_CREATE(m_pBoxCollider);

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

		SAFE_DELETE(m_pAnimator);

		SAFE_DELETE(m_pBoxCollider);
		SAFE_DELETE(m_Info);

		m_pScale = nullptr;
		m_pTargetPos = nullptr;
		m_pPosition = nullptr;
	}

	HRESULT C_Unit::Update(const FLOAT& _deltaTime)
	{
		SAFE_UPDATE(m_pAnimator);

		return S_OK;
	}
	HRESULT C_Unit::Render()
	{
		S_Bounds* bounds = m_pBoxCollider->bounds;

		float width = bounds->size.x * 0.8f;
		float height_half = bounds->size.y * 0.3f;

		Sprite->SetTranslation(*m_pPosition);
		Sprite->SetRotation(nullptr);
		Sprite->SetScale(*m_pScale);

		FAILED_CHECK_RETURN(Sprite->DrawEllipse(
			0, height_half + 5,
			width, height_half,
			30,
			1.0f,
			D3DCOLOR_XRGB(0, 255, 0)
		));

#ifdef DEBUG_Unit_HITBOX
		RECT rc;

		rc.left = lroundf(-bounds->size.x * 0.5f);
		rc.bottom = lroundf(-bounds->size.y * 0.5f);
		rc.right = bounds->size.x * 0.5f;
		rc.top = bounds->size.y * 0.5f;

		FAILED_CHECK_RETURN(Sprite->DrawRect(
			rc,
			1.0f,
			false,
			D3DCOLOR_XRGB(255, 255, 255)
		));
#endif // DEBUG_Unit_HITBOX

		return S_OK;
	}
}