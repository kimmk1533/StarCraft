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
	if (FAILED(CUnit::Create()))
		return E_FAIL;

	if (FAILED(m_Animator->Create()))
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
	m_Info->fMoveSpeed = 1.875F;						// 이동 속도
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

	m_Animator->AddFrame(E_AnimState::Walking, m_pTex, RECT{ 256, 256, 320, 256 + 64 }, 1.0f / 15);
	m_Animator->AddFrame(E_AnimState::Walking, m_pTex, RECT{ 320, 256, 384, 256 + 64 }, 1.0f / 15);
	m_Animator->AddFrame(E_AnimState::Walking, m_pTex, RECT{ 384, 256, 448, 256 + 64 }, 1.0f / 15);
	m_Animator->AddFrame(E_AnimState::Walking, m_pTex, RECT{ 448, 256, 512, 256 + 64 }, 1.0f / 15);
	m_Animator->AddFrame(E_AnimState::Walking, m_pTex, RECT{ 512, 256, 576, 256 + 64 }, 1.0f / 15);
	m_Animator->AddFrame(E_AnimState::Walking, m_pTex, RECT{ 576, 256, 640, 256 + 64 }, 1.0f / 15);
	m_Animator->AddFrame(E_AnimState::Walking, m_pTex, RECT{ 640, 256, 704, 256 + 64 }, 1.0f / 15);
	m_Animator->AddFrame(E_AnimState::Walking, m_pTex, RECT{ 704, 256, 768, 256 + 64 }, 1.0f / 15);
	m_Animator->AddFrame(E_AnimState::Walking, m_pTex, RECT{ 768, 256, 832, 256 + 64 }, 1.0f / 15);

	return S_OK;
}
HRESULT CMarine::Update(const float deltaTime)
{
	m_Animator->Update(deltaTime);

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
