#pragma once
#include "UnitInfo.h"

namespace Game
{
	using namespace CoreEngine;

	enum class E_UnitState : unsigned char;
	enum class E_Direction : unsigned char;

	class C_Unit : public C_FrameWork, public IUpdatable, public IRenderable
	{
	protected:
		S_UnitInfo* m_Info;

		D3DXVECTOR2* m_pPosition;
		D3DXVECTOR2* m_pTargetPos;
		D3DXVECTOR2* m_pScale;

	protected: // Animation
		unsigned short m_AnimIndex;
		float m_AnimIndexTimer;
		E_UnitState m_UnitState;
		E_Direction m_Direction;
		E_Direction m_TargetDir;

	public:
		C_Unit();
		virtual ~C_Unit();

		virtual HRESULT	Create() override;
		virtual HRESULT	Update(const float _deltaTime) override = 0;
		virtual HRESULT	Render() override = 0;
		virtual void	Destroy() override;

	};

	enum class E_UnitState : unsigned char
	{
		Init,						// 첫 생성(유닛 태어남, 건물 1단계) 애니메이션
		Death,						// 유닛 사망, 건물 파괴 애니메이션

		Idle,						// 아이들 애니메이션
		Other1,						// 한동안 아무것도 안할 때 랜덤하게 나타나는 애니메이션1
		Other2,						// 한동안 아무것도 안할 때 랜덤하게 나타나는 애니메이션2

		GroundAttackInit,			// 지상 공격 시작 애니메이션
		AirAttackInit,				// 공중 공격 시작 애니메이션

		GroundAttackRepeat,			// 지상 공격 반복 애니메이션
		AirAttackRepeat,			// 공중 공격 반복 애니메이션

		GroundAttackToIdle,			// 지상 공격 종료 애니메이션
		AirAttackToIdle,			// 공중 공격 종료 애니메이션

		CastSpell,					// 마법 시전 애니메이션

		Walking,					// 이동 애니메이션

		// 일꾼 애니메이션
		WorkingToIdle,				// 일꾼 작업 종료 애니메이션

		// 건물 애니메이션
		Built,						// 건물 건설 2단계 애니메이션
		AlmostBuilt,				// 건물 건설 3단계 애니메이션
		IsWorking,					// 생산 or 업그레이드 애니메이션
		Landing,					// 테란 건물 랜딩 시작 애니메이션
		LiftOff,					// 테란 건물 랜딩 종료 애니메이션

		// 저그 버로우 애니메이션
		BurrowInit,					// 버로우 할 때의 애니메이션
		Burrow,						// 버로우 애니메이션
		UnBurrow,					// 언버로우 애니메이션

		Max
	};

}