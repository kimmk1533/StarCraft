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

		m_pAnimator->AddFunc(0, [&]() -> void
			{
#pragma region Variables
				D3DXVECTOR3 d = (*m_pTargetPos) - (*m_pPosition);
				FLOAT dx = d.x;
				FLOAT dy = d.y;
				d.z = 0.0f;

				FLOAT length = D3DXVec3Length(&d);

				FLOAT radian = atan2f(dx, dy);
				FLOAT theta = g_DegreeUnit * 0.5f + D3DXToDegree(radian);

				// % 연산자는 성능을 많이 잡아 먹으니 비교 연산이 더 나을 것.
				if (theta >= 360.0f) theta -= 360.0f;
				if (theta <= 0.0f) theta += 360.0f;

				FLOAT target = theta / g_DegreeUnit;
#pragma endregion

#pragma region Rotation
				if (m_Direction != m_TargetDir)
				{
					int direction = static_cast<int>(m_Direction);

					D3DXVECTOR2 target_dir(sinf(target * g_RadianUnit), cosf(target * g_RadianUnit));
					D3DXVECTOR2 dir(sinf(direction * g_RadianUnit), cosf(direction * g_RadianUnit));

					D3DXVECTOR2 target_normal, dir_normal;

					D3DXVec2Normalize(&target_normal, &target_dir);
					D3DXVec2Normalize(&dir_normal, &dir);

					// 회전 방향 계산
					FLOAT ccw = D3DXVec2CCW(&dir_normal, &target_normal);

					// 1칸 차이 나는 경우
					if (abs((int)target - direction) == 1)
						// 1칸 이동
						ccw < 0 ? ++direction : --direction;
					// 2칸 이상 차이 나는 경우
					else
						// 2칸 이동
						ccw < 0 ? direction += 2 : direction -= 2;

					direction = (static_cast<int>(E_Direction::Max) + direction) % static_cast<int>(E_Direction::Max);
					m_Direction = static_cast<E_Direction>(direction);
				}
#pragma endregion

#pragma region Move
				if (m_Info->m_bIsMovable == false &&
					m_Direction == m_TargetDir)
				{
					m_Info->m_bIsMovable = true;
				}

				if (m_Info->m_bIsMovable && length > 0.01f)
				{
					// 최종 속도 = 이동 속도
					FLOAT speed = m_Info->m_fMoveSpeed;

					D3DXVECTOR3 vcMove;

					FLOAT move = fminf(length, speed);

					vcMove.x = move * sinf(radian);
					vcMove.y = move * cosf(radian);
					vcMove.z = 0.0f;

					(*m_pPosition) += vcMove;

					m_UnitState = E_UnitState::Walking;
				}
#pragma endregion

#pragma region UnitState
				if (length <= 1e-5f)
				{
					m_UnitState = E_UnitState::Idle;
				}
#pragma endregion
			});

		return S_OK;
	}
	void C_Marine::Destroy()
	{

	}

	HRESULT C_Marine::Update(const FLOAT& _deltaTime)
	{
		C_Unit::Update(_deltaTime);

		if (Input->GetMouseDown(E_MouseCode::Right))
		{
			(*m_pTargetPos) = Camera->WorldToScreenPoint(Input->GetMousePos());

			D3DXVECTOR3 d = (*m_pTargetPos) - (*m_pPosition);
			FLOAT dx = d.x;
			FLOAT dy = d.y;
			d.z = 0.0f;

			// 명령 방향: 현재 위치에서 우클릭한 위치로 에 해당하는 방향
			// 현재 방향과 명령 방향의 각도(라디안) 구하기
			FLOAT radian = atan2f(dx, dy);
			// 현재 방향과 명령 방향의 각도(세타) 구하기
			FLOAT theta = g_DegreeUnit * 0.5f + D3DXToDegree(radian);

			// % 연산자는 성능을 많이 잡아 먹으니 비교 연산이 더 나을 것.
			// 360도 회전 예외 처리
			if (theta >= 360.0f) theta -= 360.0f;
			if (theta <= 0.0f) theta += 360.0f;

			// 목표 방향에 해당하는 Enum 값 구하기
			FLOAT target = theta / g_DegreeUnit;

			// 목표 방향 설정
			m_TargetDir = static_cast<E_Direction>(target);

			// 회전하는 도중 이동 금지
			m_Info->m_bIsMovable = false;
		}

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

		Sprite->Begin();

		Sprite->SetTranslation(*m_pPosition);
		Sprite->SetRotation(nullptr);
		Sprite->SetScale(*m_pScale);

		Sprite->Draw(texture->GetTexture(), &rect, C_MarineManager::GetI()->offset, D3DCOLOR_XRGB(255, 255, 255));

		Sprite->End();

		return S_OK;
	}
}