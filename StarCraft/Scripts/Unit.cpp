#include "stdafx.h"
#include "Unit.h"

#include "MapManager.h"

namespace Game
{
	void C_Unit::Move()
	{
//		if (!m_pMovingPosList->empty())
//		{
//			D3DXVECTOR3 d = (*m_pMovingPosList)[0] - (*m_pPosition);
//			FLOAT dx = d.x;
//			FLOAT dy = d.y;
//			d.z = 0.0f;
//
//			FLOAT length = D3DXVec3Length(&d);
//
//			FLOAT radian = atan2f(dx, dy);
//			FLOAT theta = g_DegreeUnit * 0.5f + D3DXToDegree(radian);
//
//			// % 연산자는 성능을 많이 잡아 먹으니 비교 연산이 더 나을 것.
//			if (theta >= 360.0f) theta -= 360.0f;
//			if (theta <= 0.0f) theta += 360.0f;
//
//			FLOAT target = theta / g_DegreeUnit;
//
//			if (m_Info->m_bIsMovable == false &&
//				m_Direction == m_TargetDir)
//			{
//				m_Info->m_bIsMovable = true;
//			}
//
//#pragma region Move
//			if (m_Info->m_bIsMovable == false &&
//				m_Direction == m_TargetDir)
//			{
//				m_Info->m_bIsMovable = true;
//			}
//
//			if (m_Info->m_bIsMovable && length > 0.01f)
//			{
//				// 최종 속도 = 이동 속도
//				FLOAT speed = m_Info->m_fMoveSpeed;
//
//				D3DXVECTOR3 vcMove;
//
//				FLOAT move = fminf(length, speed);
//
//				vcMove.x = move * sinf(radian);
//				vcMove.y = move * cosf(radian);
//				vcMove.z = 0.0f;
//
//				(*m_pPosition) += vcMove;
//
//				m_UnitState = E_UnitState::Walking;
//			}
//#pragma endregion
//		}
	}

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
		m_pPosition = std::make_shared<D3DXVECTOR3>(500.0f, 100.0f, 0.0f);
		m_pTargetPos = std::make_shared<D3DXVECTOR3>(500.0f, 100.0f, 0.0f);
		m_pMovingPosList = new std::list<D3DXVECTOR3>();
		m_pScale = std::make_shared<D3DXVECTOR3>(1.0f, 1.0f, 1.0f);

		m_Info = new S_UnitInfo();
		m_pBoxCollider = new C_BoxCollider();
		SAFE_CREATE(m_pBoxCollider);

		m_pAnimator = new C_Animator();
		SAFE_CREATE(m_pAnimator);
		m_pAnimator->SetSampleRate(g_InGameFPS);
		m_pAnimator->AddFunc(0, [this]() -> void
			{
#pragma region Animation
				++m_AnimIndex;
#pragma endregion

				if (!m_pMovingPosList->empty())
				{
#pragma region Variables
					D3DXVECTOR3 d = (*m_pMovingPosList).back() - (*m_pPosition);
					//D3DXVECTOR3 d = (*m_pTargetPos) - (*m_pPosition);
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

					if (length <= 1e-5f)
					{
						m_pMovingPosList->pop_back();

						if (!m_pMovingPosList->empty())
						{
							D3DXVECTOR3 d = (*m_pMovingPosList).back() - (*m_pPosition);
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
						else
						{
							m_UnitState = E_UnitState::Idle;
						}
					}
#pragma endregion
				}
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
		SAFE_DELETE(m_pMovingPosList);
		m_pTargetPos = nullptr;
		m_pPosition = nullptr;
	}

	HRESULT C_Unit::Update(const FLOAT& _deltaTime)
	{
		static C_MapManager* M_Map = C_MapManager::GetI();

		SAFE_UPDATE(m_pAnimator);

		if (Input->GetMouseDown(E_MouseCode::Right))
		{
			(*m_pTargetPos) = Camera->WorldToScreenPoint(Input->GetMousePos());

			m_pMovingPosList->clear();
			std::vector<D3DXVECTOR3> path = M_Map->GetPath(*m_pPosition, *m_pTargetPos);
			m_pMovingPosList->insert(m_pMovingPosList->end(), path.begin(), path.end());

			m_Test.clear();
			m_Test.insert(m_Test.end(), path.begin(), path.end());
			for (size_t i = 0; i < m_Test.size(); ++i)
			{
				D3DXVECTOR3 item = D3DXVECTOR3(m_Test[i].x, m_Test[i].y, 0.0f);
				item -= *m_pPosition;
				m_Test[i] = D3DXVECTOR2(item.x, item.y);
			}

			if (m_pMovingPosList->empty())
			{
				m_TargetDir = m_Direction;

				m_Info->m_bIsMovable = true;

				m_UnitState = E_UnitState::Idle;

				return S_OK;
			}

			D3DXVECTOR3 d = (*m_pMovingPosList).back() - (*m_pPosition);
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

		return S_OK;
	}
	HRESULT C_Unit::Render()
	{
		S_Bounds* bounds = m_pBoxCollider->bounds;

		float width = bounds->size.x * 0.8f;
		float height_half = bounds->size.y * 0.3f;

		Sprite->SetTranslation(*m_pPosition);
		Sprite->SetRotation(nullptr);
		Sprite->SetScale(nullptr);

		FAILED_CHECK_RETURN(Sprite->DrawEllipse(
			0, height_half + 5,
			width, height_half,
			30,
			1.0f,
			D3DCOLOR_XRGB(0, 255, 0)
		));

		if (!m_pMovingPosList->empty())
		{
			Sprite->SetTranslation(nullptr);
			Sprite->DrawLine(m_Test.data(), m_Test.size(), 1.0f, false, D3DCOLOR_XRGB(255, 0, 0));
		}

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