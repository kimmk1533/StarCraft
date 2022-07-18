#pragma once
#include <Animator.h>
#include <Timer.h>
#include <BoxCollider.h>

#include "UnitInfo.h"

namespace Game
{
	enum class E_Direction : unsigned char;
	enum class E_UnitState : unsigned char;
}

namespace Game
{
	using namespace CoreEngine;

	class C_Unit : public C_FrameWork, public IUpdatable, public IRenderable
	{
	private:
		static WORD m_UnitCount;

	protected:
		D3DXVECTOR3* m_pPosition;
		D3DXVECTOR3* m_pTargetPos;
		D3DXVECTOR3* m_pScale;

		S_UnitInfo* m_Info;
		C_BoxCollider* m_pBoxCollider;

	protected: // Animation
		C_Animator* m_pAnimator;

		WORD m_AnimIndex;
		C_Timer* m_pGameFrameTimer;
		E_UnitState m_UnitState;
		E_Direction m_Direction;
		E_Direction m_TargetDir;

	public:
		READONLY_PROPERTY(C_BoxCollider*, collider);
		GET(collider) { return m_pBoxCollider; }

	public:
		C_Unit();
		virtual ~C_Unit();

	public:
		virtual HRESULT	Create() override;
		virtual void	Destroy() override;

		virtual HRESULT	Update(const FLOAT& _deltaTime) override;
		virtual HRESULT	Render() override;

	};

	enum class E_UnitState : unsigned char
	{
		// 생성, 사망 관련
		Init,						// 첫 생성(유닛 태어남, 건물 1단계) 상태
		Death,						// 유닛 사망, 건물 파괴 상태

		// 아이들 관련
		Idle,						// 아이들 상태
		Other1,						// 한동안 아무것도 안할 때 랜덤하게 나타나는 상태1
		Other2,						// 한동안 아무것도 안할 때 랜덤하게 나타나는 상태2

		// 이동 관련
		Walking,					// 이동 상태

		// 공격 관련
		GroundAttackInit,			// 지상 공격 시작 상태
		AirAttackInit,				// 공중 공격 시작 상태

		GroundAttackRepeat,			// 지상 공격 반복 상태
		AirAttackRepeat,			// 공중 공격 반복 상태

		GroundAttackToIdle,			// 지상 공격 종료 상태
		AirAttackToIdle,			// 공중 공격 종료 상태

		// 기술 관련
		CastSpell,					// 마법 시전 상태

		// 일꾼 관련
		WorkingToIdle,				// 일꾼 작업 종료 상태

		// 건물 관련
		Built,						// 건물 건설 2단계 상태
		AlmostBuilt,				// 건물 건설 3단계 상태
		IsWorking,					// 생산 or 업그레이드 상태
		Landing,					// 테란 건물 랜딩 시작 상태
		LiftOff,					// 테란 건물 랜딩 종료 상태

		// 저그 버로우 관련
		BurrowInit,					// 버로우 할 때의 상태
		Burrow,						// 버로우 상태
		UnBurrow,					// 언버로우 상태

		Max
	};
}