#include "stdafx.h"
#include "Marine.h"

#include "MarineManager.h"

namespace Game
{
	C_Marine::C_Marine()
	{

	}
	C_Marine::~C_Marine()
	{
		Destroy();
	}

	HRESULT C_Marine::Create()
	{
		// 부모 클래스 Create
		if (FAILED(C_Unit::Create()))
			return E_FAIL;

#pragma region UnitInfo
		// 종족
		m_Info->m_RaceType = E_RaceType::Terran;				// 종족

		// 생산
		m_Info->m_wMineral = 50;								// 필요 미네랄
		m_Info->m_wVespeneGas = 0;								// 필요 가스
		m_Info->m_wPopulation = 1;								// 필요 인구수
		m_Info->m_wProductionTime = 2400;						// 필요 생산 시간(ms단위)
		m_Info->m_ShortcutKey = E_KeyCode::M;					// 생산 단축키

		// 이동
		m_Info->m_bIsMovable = true;							// 이동 가능 여부
		m_Info->m_fMoveSpeed = 4.0f;							// 이동 속도
		m_Info->m_bIsAir = false;								// 공중 유닛인지

		// 공격
		m_Info->m_bIsAttackable = true;							// 공격 가능 여부
		m_Info->m_AttackType = E_UnitAtkType::NormalType;		// 공격 타입(일반형, 폭발형, 진동형, 방어무시형)
		m_Info->m_wAttackPoint = 6;								// 공격력
		m_Info->m_fAttackSpeed = 15.0f;							// 공격 속도
		m_Info->m_fAttackRange = 128.0f;						// 공격 사거리

		// 방어
		m_Info->m_bIsHitable = true;							// 방어 가능 여부(타겟팅 가능 여부)
		m_Info->m_DeffenseType = E_UnitDefType::SmallType;		// 방어 타입(무적, 소형, 중형, 대형)
		m_Info->m_fMaxHP = 50.0f;								// 최대 체력
		m_Info->m_fHP = m_Info->m_fMaxHP;						// 체력
		m_Info->m_wDeffensePoint = 0;							// 방어력

		// 쉴드
		m_Info->m_bUseShield = false;							// 쉴드 사용 여부
		m_Info->m_fMaxShield = 0.0f;							// 최대 쉴드
		m_Info->m_fShield = 0.0f;								// 쉴드

		// 에너지(마나)
		m_Info->m_bUseEnergy = false;							// 에너지 사용 여부
		m_Info->m_fMaxEnergy = 0.0f;							// 최대 에너지
		m_Info->m_fEnergy = 0.0f;								// 에너지

		// 시야
		m_Info->m_fVisualRange = 7.0f;							// 시야 거리

		// 바운딩 박스
		m_pBoxCollider->bounds->size = D3DXVECTOR3(17.0f, 20.0f, 0.0f);		// 충돌 크기
		m_pBoxCollider->bounds->SetSharedCenter(m_pPosition);
#pragma endregion

		return S_OK;
	}
	void C_Marine::Destroy()
	{

	}

	HRESULT C_Marine::Update(const FLOAT& _deltaTime)
	{
		C_Unit::Update(_deltaTime);

		// 임시 코드
		/*if (Input->GetKeyDown(E_KeyCode::S))
		{
			(*m_pTargetPos) = (*m_pPosition);
		}*/

		return S_OK;
	}
	HRESULT C_Marine::Render()
	{
		FAILED_CHECK_RETURN(C_Unit::Render());

		std::shared_ptr<C_Texture> texture = C_MarineManager::GetI()->GetTexture();
		RECT rect = C_MarineManager::GetI()->GetTextureRect({ m_UnitState, m_Direction }, m_AnimIndex);

		if (m_Direction > E_Direction::Down)
			m_pScale->x = -1.0f;
		else
			m_pScale->x = 1.0f;

		Sprite->SetTranslation(*m_pPosition);
		Sprite->SetRotation(nullptr);
		Sprite->SetScale(*m_pScale);

		Sprite->Draw(texture->GetTexture(), &rect, C_MarineManager::GetI()->offset, D3DCOLOR_XRGB(255, 255, 255));

		return S_OK;
	}
}