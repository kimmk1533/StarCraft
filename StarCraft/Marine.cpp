#include "stdafx.h"

#include "Marine.h"

//CLcTexture* CMarine::m_pTex = nullptr;

CMarine::CMarine()
{
	m_Info = NULL;

	m_pTex = nullptr;
}

CMarine::~CMarine()
{
	Destroy();
}

HRESULT CMarine::Create()
{
	// 부모 클래스 Create
	if (FAILED(CUnit::Create()))
		return E_FAIL;

	// 매니저에서 관리하도록 변경해야함
	if (!m_pTex)
	{
		m_pTex = std::make_shared<CLcTexture*>();
		(*m_pTex) = new CLcTexture();
		if (FAILED((*m_pTex)->Create(m_pd3dDevice, L"Marine.png")))
		{
			SAFE_DELETE(*m_pTex);

			MessageBox(NULL, L"마린 텍스쳐 불러오기 실패", L"오류", MB_OK);

			return E_FAIL;
		}
	}
	//

#pragma region Animation
	if (FAILED(m_Animator->Create()))
		return E_FAIL;

	int max = static_cast<int>(E_Direction::Max);
	float fps = 1.0f / 18;
	RECT rect{ 0, 0, 64, 64 };
	std::pair<E_AnimState, E_Direction> animState;

	/*
	*  1: 0
	*  2: 64
	*  3: 128
	*  4: 194
	*  5: 256
	*  6: 320
	*  7: 384
	*  8: 448
	*  9: 512
	* 10: 576
	* 11: 640
	* 12: 704
	* 13: 768
	*/
	for (int i = 0; i < max; ++i)
	{
		if (i < 9)
		{
			rect.top = i * 64;
			rect.bottom = (i + 1) * 64;
		}
		else
		{
			rect.top = (16 - i) * 64;
			rect.bottom = (16 - i + 1) * 64;
		}

		E_Direction direction;
		animState.second = direction = static_cast<E_Direction>(i);

#pragma region Init
		animState.first = E_AnimState::Init;
		rect.left = 256; rect.right = 320;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);
#pragma endregion

#pragma region Idle
		animState.first = E_AnimState::Idle;
		rect.left = 256; rect.right = 320;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);
#pragma endregion

#pragma region Other
		m_Animator->SetAnimMode({ E_AnimState::Other1, direction }, E_AnimMode::Repeat_Back);
		animState.first = E_AnimState::Other1;

		rect.left = 0; rect.right = 64;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);

		rect.left = 64; rect.right = 128;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);

		rect.left = 128; rect.right = 192;
		m_Animator->AddFrame(animState, m_pTex, rect, fps * 4);

		/*rect.top = (i + 1) * 64; rect.bottom = (i + 2) * 64;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);

		rect.top = (i + 2) * 64; rect.bottom = (i + 3) * 64;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);

		rect.top = (i + 2) * 64; rect.bottom = (i + 4) * 64;
		m_Animator->AddFrame(animState, m_pTex, rect, fps * 4);

		rect.top = (i + 2) * 64; rect.bottom = (i + 3) * 64;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);

		rect.top = (i + 1) * 64; rect.bottom = (i + 2) * 64;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);

		rect.top = i * 64; rect.bottom = (i + 1) * 64;
		rect.left = 128; rect.right = 192;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);

		rect.left = 64; rect.right = 128;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);

		rect.left = 0; rect.right = 64;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);*/
#pragma endregion

#pragma region GroundAttack
		animState.first = E_AnimState::GroundAttackInit;
		rect.left = 0; rect.right = 64;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);
		rect.left = 64; rect.right = 128;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);
		m_Animator->AddFunc(animState, [&] {
			m_Animator->SetAnimState(E_AnimState::GroundAttackRepeat);
			});

		animState.first = E_AnimState::GroundAttackRepeat;
		rect.left = 128; rect.right = 192;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);
		rect.left = 192; rect.right = 256;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);

		animState.first = E_AnimState::GroundAttackToIdle;
		rect.left = 256; rect.right = 320;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);
#pragma endregion

#pragma region AirAttack
		animState.first = E_AnimState::AirAttackInit;
		rect.left = 0; rect.right = 64;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);
		rect.left = 64; rect.right = 128;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);

		animState.first = E_AnimState::AirAttackRepeat;
		rect.left = 128; rect.right = 192;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);
		rect.left = 192; rect.right = 256;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);

		animState.first = E_AnimState::AirAttackToIdle;
		rect.left = 256; rect.right = 320;
		m_Animator->AddFrame(animState, m_pTex, rect, fps);
#pragma endregion

#pragma region Walking
		animState.first = E_AnimState::Walking;
		for (int j = 4; j < 13; ++j)
		{
			rect.left = j * 64; rect.right = (j + 1) * 64;
			m_Animator->AddFrame(animState, m_pTex, rect, fps);
		}
#pragma endregion

#pragma region Death
		m_Animator->SetAnimMode({ E_AnimState::Death, direction }, E_AnimMode::Once);
		animState.first = E_AnimState::Death;

		rect.top = 576; rect.bottom = 640;
		for (int j = 0; j < 8; ++j)
		{
			rect.left = j * 64; rect.right = (j + 1) * 64;
			m_Animator->AddFrame(animState, m_pTex, rect, fps);
		}
#pragma endregion
	}

	/*m_Animator->AddFunc(E_AnimState::GroundAttackInit,
		1,
		[&] {
			m_Animator->SetAnimState(E_AnimState::GroundAttackRepeat);
		});*/

	m_Animator->SetOffset(32, 32);
	m_Animator->SetAnimState(E_AnimState::GroundAttackInit);
#pragma endregion

#pragma region UnitInfo

	// 종족
	m_Info->RaceType = E_RaceType::Terran;				// 종족

	// 생산
	m_Info->iReqMineral = 50;							// 필요 미네랄
	m_Info->iReqVespeneGas = 0;							// 필요 가스
	m_Info->iReqPopulation = 1;							// 필요 인구수
	m_Info->iReqTime = 2400;							// 필요 생산 시간(ms단위)
	m_Info->ShortcutKey = E_KeyCode::M;					// 생산 단축키

	// 이동
	m_Info->bIsMovable = true;							// 이동 가능 여부
	m_Info->fMoveSpeed = 4.0f;							// 이동 속도
	m_Info->bIsAir = FALSE;								// 공중 유닛인지

	// 공격
	m_Info->bIsAttackable = true;						// 공격 가능 여부
	m_Info->AttackType = E_UnitAtkType::NormalType;		// 공격 타입(일반형, 폭발형, 진동형, 방어무시형)
	m_Info->iAttackPoint = 6;							// 공격력
	m_Info->fAttackSpeed = 15.0F;						// 공격 속도
	m_Info->fAttackRange = 4.0F;						// 공격 사거리

	// 방어
	m_Info->bIsDefensible = true;						// 방어 가능 여부(타겟팅 가능 여부)
	m_Info->DeffenseType = E_UnitDefType::SmallType;	// 방어 타입(무적, 소형, 중형, 대형)
	m_Info->fMaxHP = 50.0F;								// 최대 체력
	m_Info->fHP = m_Info->fMaxHP;						// 체력
	m_Info->fMaxShield = 0.0F;							// 최대 쉴드
	m_Info->fShield = 0.0F;								// 쉴드
	m_Info->iDeffensePoint = 0;							// 방어력

	// 에너지(마나)
	m_Info->bCanUseEnergy = FALSE;						// 에너지 사용 여부
	m_Info->fMaxEnergy = 0.0F;							// 최대 에너지
	m_Info->fEnergy = 0.0F;								// 에너지

	// 시야
	m_Info->fVisualRange = 7.0F;						// 시야 거리

	// 충돌 크기
	m_Info->ptPixelSize = Point(17, 20);				// 충돌 크기

#pragma endregion

	return S_OK;
}
HRESULT CMarine::Update(const float _deltaTime)
{
	m_Animator->Update(_deltaTime);

	if (m_pLcInput->BtnDown(E_KeyCode::MouseRightButton))
	{
		//m_Animator->SetPosition(32, 32);

		D3DXVECTOR3 mousePos = m_pLcInput->GetMousePos();
		m_TargetPos = D3DXVECTOR2(mousePos.x, mousePos.y);
	}

	D3DXVECTOR2 d = m_TargetPos - m_Position;
	float length = D3DXVec2Length(&d);

	if (length > 0.01f)
	{
		float dx = d.x;
		float dy = d.y;

		float radian = atan2f(dx, dy);
		float speed = m_Info->fMoveSpeed * 32 * _deltaTime;

		D3DXVECTOR2 vcMove(1, 1);

		float move = fminf(length, speed);

		vcMove.x *= move * sinf(radian);
		vcMove.y *= move * cosf(radian);

		m_Animator->AddPosition(vcMove);
		m_Position = m_Animator->GetPosition();

		static const float degree_unit = 360.0f / static_cast<int>(E_Direction::Max);
		float theta = 180.0f - D3DXToDegree(radian);
		std::cout << "theta:" << theta << "\n";
		E_Direction dir = static_cast<E_Direction>(theta / degree_unit);

		m_Animator->SetAnimState(E_AnimState::Walking, dir);
	}
	else
	{
		m_Animator->SetAnimState(E_AnimState::Idle);
	}

	/*if (m_pLcInput->KeyDown(E_KeyCode::A))
	{
		m_Animator->SetAnimState(E_AnimState::Walking);
	}
	if (m_pLcInput->KeyPress(E_KeyCode::A))
	{
		m_Animator->AddPosition(D3DXVECTOR2(-m_Info->fMoveSpeed * _deltaTime, 0.0f));
	}
	if (m_pLcInput->KeyUp(E_KeyCode::A))
	{
		m_Animator->SetAnimState(E_AnimState::Idle);
	}

	if (m_pLcInput->KeyDown(E_KeyCode::D))
	{
		m_Animator->SetAnimState(E_AnimState::Walking);
	}
	if (m_pLcInput->KeyPress(E_KeyCode::D))
	{
		m_Animator->AddPosition(D3DXVECTOR2(m_Info->fMoveSpeed * _deltaTime, 0.0f));
	}
	if (m_pLcInput->KeyUp(E_KeyCode::D))
	{
		m_Animator->SetAnimState(E_AnimState::Idle);
	}*/

	return S_OK;
}
HRESULT CMarine::Render()
{
	return m_Animator->Render();
}
void CMarine::Destroy()
{
	SAFE_DELETE(*m_pTex);
}

CLcTexture* CMarine::GetTexture() const
{
	return *m_pTex;
}
