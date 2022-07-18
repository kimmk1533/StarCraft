#include "stdafx.h"
#include "Unit.h"

namespace Game
{
	WORD C_Unit::m_UnitCount = 0;

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
		m_pPosition = new D3DXVECTOR3(100.0f, 100.0f, 0.0f);
		m_pTargetPos = new D3DXVECTOR3(100.0f, 100.0f, 0.0f);
		m_pScale = new D3DXVECTOR3(1.0f, 1.0f, 1.0f);

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

		SAFE_DELETE(m_pScale);
		SAFE_DELETE(m_pTargetPos);
		SAFE_DELETE(m_pPosition);
	}

	HRESULT C_Unit::Update(const FLOAT& _deltaTime)
	{
		SAFE_UPDATE(m_pAnimator);

		return S_OK;
	}
	HRESULT C_Unit::Render()
	{
		/*S_Bounds* bounds = m_pBoxCollider->bounds;

		float width = bounds->size.x * 0.8f;
		float height_half = bounds->size.y * 0.3f;

		FAILED_CHECK(m_pSprite->DrawEllipse(
			m_pPosition->x, m_pPosition->y + height_half + 5,
			width, height_half,
			30,
			1.0f,
			D3DCOLOR_XRGB(0, 255, 0)
		)
		);*/

//#define TEST
#ifdef TEST
		D3DXVECTOR2 min = D3DXVECTOR2(bounds->min);
		D3DXVECTOR2 max = D3DXVECTOR2(bounds->max);

		D3DXVECTOR2 vertex[5];

		vertex[4] = vertex[0] = min;
		vertex[1] = min + D3DXVECTOR2(bounds->size.x, 0.0f);
		vertex[2] = max;
		vertex[3] = min + D3DXVECTOR2(0.0f, bounds->size.y);

		FAILED_CHECK(m_pSprite->DrawLine(
			vertex,
			5,
			1,
			false,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
		)
		);
#endif // TEST

		return S_OK;
	}
}