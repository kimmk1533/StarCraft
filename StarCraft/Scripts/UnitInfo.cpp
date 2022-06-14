#include "stdafx.h"
#include "UnitInfo.h"

namespace Game
{
	S_UnitInfo::S_UnitInfo()
	{
		// 종족
		m_RaceType = E_RaceType::Neutral;		// 종족

		// 생산
		m_wMineral = 0;							// 필요 미네랄
		m_wVespeneGas = 0;						// 필요 가스
		m_wPopulation = 0;						// 필요 인구수
		m_wProductionTime = 0;					// 필요 생산 시간(ms단위)
		m_ShortcutKey = E_KeyCode::None;		// 생산 단축키

		// 이동
		m_bIsMovable = false;					// 이동 가능 여부
		m_fMoveSpeed = 0.0f;					// 이동 속도
		m_bIsAir = false;						// 공중 유닛인지

		// 공격
		m_bIsAttackable = false;				// 공격 가능 여부
		m_AttackType = E_UnitAtkType::None;		// 공격 타입(일반형, 폭발형, 진동형, 방어무시형)
		m_wAttackPoint = 0;						// 공격력
		m_fAttackSpeed = 0.0f;					// 공격 속도
		m_fAttackRange = 0.0f;					// 공격 사거리

		// 방어
		m_bIsHitable = false;					// 방어 가능 여부(타겟팅 가능 여부)
		m_DeffenseType = E_UnitDefType::None;	// 방어 타입(소형, 중형, 대형)
		m_fMaxHP = 0.0f;						// 최대 체력
		m_fHP = 0.0f;							// 체력
		m_wDeffensePoint = 0;					// 방어력

		// 쉴드
		m_bUseShield = false;					// 쉴드 사용 여부
		m_fMaxShield = 0.0f;					// 최대 쉴드
		m_fShield = 0.0f;						// 쉴드

		// 에너지(마나)
		m_bUseEnergy = false;					// 에너지 사용 여부
		m_fMaxEnergy = 0.0f;					// 최대 에너지
		m_fEnergy = 0.0f;						// 에너지

		// 시야
		m_fVisualRange = 0.0f;					// 시야 거리

		// 충돌 크기
		m_PixelSize.x = 0;						// 충돌 크기 x
		m_PixelSize.y = 0;						// 충돌 크기 y
	}
	S_UnitInfo::S_UnitInfo(const S_UnitInfo& s)
	{
		*this = s;
	}
	S_UnitInfo& S_UnitInfo::operator=(const S_UnitInfo& s)
	{
		// 종족
		m_RaceType = s.m_RaceType;						// 종족

		// 생산
		m_wMineral = s.m_wMineral;						// 필요 미네랄
		m_wVespeneGas = s.m_wVespeneGas;				// 필요 가스
		m_wPopulation = s.m_wPopulation;				// 필요 인구수
		m_wProductionTime = s.m_wProductionTime;		// 필요 생산 시간(ms단위)
		m_ShortcutKey = s.m_ShortcutKey;				// 생산 단축키

		// 이동
		m_bIsMovable = s.m_bIsMovable;					// 이동 가능 여부
		m_fMoveSpeed = s.m_fMoveSpeed;					// 이동 속도
		m_bIsAir = s.m_bIsAir;							// 공중 유닛인지

		// 공격
		m_bIsAttackable = s.m_bIsAttackable;			// 공격 가능 여부
		m_AttackType = s.m_AttackType;					// 공격 타입(일반형, 폭발형, 진동형, 방어무시형)
		m_wAttackPoint = s.m_wAttackPoint;				// 공격력
		m_fAttackSpeed = s.m_fAttackSpeed;				// 공격 속도
		m_fAttackRange = s.m_fAttackRange;				// 공격 사거리

		// 방어
		m_bIsHitable = s.m_bIsHitable;					// 방어 가능 여부(타겟팅 가능 여부)
		m_DeffenseType = s.m_DeffenseType;				// 방어 타입(소형, 중형, 대형)
		m_fMaxHP = s.m_fMaxHP;							// 최대 체력
		m_fHP = s.m_fHP;								// 체력
		m_wDeffensePoint = s.m_wDeffensePoint;			// 방어력

		// 쉴드
		m_bUseShield = s.m_bUseShield;					// 쉴드 사용 여부
		m_fMaxShield = s.m_fMaxShield;					// 최대 쉴드
		m_fShield = s.m_fShield;						// 쉴드

		// 에너지(마나)
		m_bUseEnergy = s.m_bUseEnergy;					// 에너지 사용 여부
		m_fMaxEnergy = s.m_fMaxEnergy;					// 최대 에너지
		m_fEnergy = s.m_fEnergy;						// 에너지

		// 시야
		m_fVisualRange = s.m_fVisualRange;				// 시야 거리

		// 충돌 크기
		m_PixelSize = s.m_PixelSize;					// 충돌 크기

		return *this;
	}
}