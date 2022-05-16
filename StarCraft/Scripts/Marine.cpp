//#include <stdafx.h>
#include "..\..\CoreEngine\Scripts\stdafx.h"

#include "Marine.h"
#include "MarineManager.h"
#include "Utility.h"

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
		m_Info->bIsAir = false;								// 공중 유닛인지

		// 공격
		m_Info->bIsAttackable = true;						// 공격 가능 여부
		m_Info->AttackType = E_UnitAtkType::NormalType;		// 공격 타입(일반형, 폭발형, 진동형, 방어무시형)
		m_Info->iAttackPoint = 6;							// 공격력
		m_Info->fAttackSpeed = 15.0f;						// 공격 속도
		m_Info->fAttackRange = 4.0f;						// 공격 사거리

		// 방어
		m_Info->bIsDefensible = true;						// 방어 가능 여부(타겟팅 가능 여부)
		m_Info->DeffenseType = E_UnitDefType::SmallType;	// 방어 타입(무적, 소형, 중형, 대형)
		m_Info->fMaxHP = 50.0f;								// 최대 체력
		m_Info->fHP = m_Info->fMaxHP;						// 체력
		m_Info->fMaxShield = 0.0f;							// 최대 쉴드
		m_Info->fShield = 0.0f;								// 쉴드
		m_Info->iDeffensePoint = 0;							// 방어력

		// 에너지(마나)
		m_Info->bCanUseEnergy = false;						// 에너지 사용 여부
		m_Info->fMaxEnergy = 0.0f;							// 최대 에너지
		m_Info->fEnergy = 0.0f;								// 에너지

		// 시야
		m_Info->fVisualRange = 7.0f;						// 시야 거리

		// 충돌 크기
		m_Info->ptPixelSize.x = 17;							// 충돌 크기 x
		m_Info->ptPixelSize.y = 20;							// 충돌 크기 y
#pragma endregion

		return S_OK;
	}
	void C_Marine::Destroy()
	{

	}

	HRESULT C_Marine::Update(const FLOAT& _deltaTime)
	{
#pragma region 1 프레임
		if (m_pInput->BtnDown(E_KeyCode::MouseRightButton))
		{
			D3DXVECTOR3 mousePos = m_pInput->GetMousePos();
			m_pTargetPos->x = mousePos.x;
			m_pTargetPos->y = mousePos.y;
		}

		static const FLOAT degree_unit = 360.0f / static_cast<int>(E_Direction::Max);
		static const FLOAT radian_unit = D3DXToRadian(degree_unit);

		D3DXVECTOR2 d = (*m_pTargetPos) - (*m_pPosition);
		FLOAT length = D3DXVec2Length(&d);

		FLOAT dx = d.x;
		FLOAT dy = d.y;

		FLOAT radian = atan2f(dx, dy);
		FLOAT theta = 180.0f + degree_unit * 0.5f - D3DXToDegree(radian);

		// % 연산자는 성능을 많이 잡아 먹으니 비교 연산이 더 나을 것.
		if (theta >= 360.0f)
			theta -= 360.0f;

		FLOAT target = theta / degree_unit;
		if (m_pInput->BtnDown(E_KeyCode::MouseRightButton))
		{
			m_TargetDir = static_cast<E_Direction>(target);

			m_Info->bIsMovable = false;
		}
#pragma endregion

#pragma region 1 게임 프레임 (Normal 기준 15프레임)
		// 1 게임 프레임 대기
		if (FAILED(m_pGameFrameTimer->Update(_deltaTime)))
			return S_OK;

#pragma region AnimIndex
		++m_AnimIndex;
#pragma endregion

#pragma region Rotation
		if (m_Direction != m_TargetDir)
		{
			int direction = static_cast<int>(m_Direction);

			D3DXVECTOR2 target_dir(sinf(target * radian_unit), cosf(target * radian_unit));
			D3DXVECTOR2 dir(sinf(direction * radian_unit), cosf(direction * radian_unit));

			D3DXVECTOR2 target_normal, dir_normal;

			D3DXVec2Normalize(&target_normal, &target_dir);
			D3DXVec2Normalize(&dir_normal, &dir);

			FLOAT ccw = D3DXVec2CCW(&dir_normal, &target_normal);

			if (abs((int)target - direction) == 1)
				ccw < 0 ? ++direction : --direction;
			else
				ccw < 0 ? direction += 2 : direction -= 2;

			direction = (static_cast<int>(E_Direction::Max) + direction) % static_cast<int>(E_Direction::Max);
			m_Direction = static_cast<E_Direction>(direction);
		}
#pragma endregion

#pragma region Move
		if (m_Direction == m_TargetDir)
		{
			m_Info->bIsMovable = true;
		}

		if (m_Info->bIsMovable && length > 0.01f)
		{
			// 최종 속도 = 이동 속도 * 1 게임 프레임 * 델타타임(1 게임 프레임이 진행되는 동안 지난 시간) 
			FLOAT speed = m_Info->fMoveSpeed * DEBUG_GAME_SPEED * InGame_FPS;

			D3DXVECTOR2 vcMove;

			FLOAT move = fminf(length, speed);

			vcMove.x = move * sinf(radian);
			vcMove.y = move * cosf(radian);

			(*m_pPosition) += vcMove;

			m_UnitState = E_UnitState::Walking;
		}
#pragma endregion

#pragma endregion

		if (length <= 0.0f)
		{
			m_UnitState = E_UnitState::Idle;
		}

		return S_OK;
	}
	HRESULT C_Marine::Render()
	{
		static const D3DXVECTOR3 Offset(32, 32, 0);

		std::shared_ptr<C_Texture> texture = C_MarineManager::GetI().GetTexture();
		RECT rect = C_MarineManager::GetI().GetTextureRect({ m_UnitState, m_Direction }, m_AnimIndex);

		if (m_Direction > E_Direction::Down)
			m_pScale->x = -1.0f;
		else
			m_pScale->x = 1.0f;

		return m_pSprite->Draw(texture->GetTexture(), &rect, m_pScale, nullptr, m_pPosition, &Offset, D3DXCOLOR(1, 1, 1, 1));
	}
}