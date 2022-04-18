#pragma once
#include "stdafx.h"

enum class E_RaceType
{
	Neutral,
	Protoss,
	Terran,
	Zerg
};
enum class E_UnitAtkType
{
	None,
	NormalType,
	ExplosiveType,
	ConcussiveType,
	SpellType,
};
enum class E_UnitDefType
{
	None,
	SmallType,
	MediumType,
	LargeType,
};

struct S_UnitInfo
{
public:
	// 종족
	E_RaceType RaceType;			// 종족

	// 생산
	INT iReqMineral;				// 필요 미네랄
	INT iReqVespeneGas;				// 필요 가스
	INT iReqPopulation;				// 필요 인구수
	INT iReqTime;					// 필요 생산 시간(ms단위)
	E_KeyCode ShortcutKey;			// 생산 단축키

	// 이동
	bool bIsMovable;				// 이동 가능 여부
	FLOAT fMoveSpeed;				// 이동 속도
	bool bIsAir;					// 공중 유닛인지

	// 공격
	bool bIsAttackable;				// 공격 가능 여부
	E_UnitAtkType AttackType;		// 공격 타입(일반형, 폭발형, 진동형, 방어무시형)
	INT iAttackPoint;				// 공격력
	FLOAT fAttackSpeed;				// 공격 속도
	FLOAT fAttackRange;				// 공격 사거리

	// 방어
	bool bIsDefensible;				// 방어 가능 여부(타겟팅 가능 여부)
	E_UnitDefType DeffenseType;		// 방어 타입(소형, 중형, 대형)
	FLOAT fMaxHP;					// 최대 체력
	FLOAT fHP;						// 체력
	FLOAT fMaxShield;				// 최대 쉴드
	FLOAT fShield;					// 쉴드
	INT iDeffensePoint;				// 방어력

	// 에너지(마나)
	bool bCanUseEnergy;				// 에너지 사용 여부
	FLOAT fMaxEnergy;				// 최대 에너지
	FLOAT fEnergy;					// 에너지
	
	// 시야
	FLOAT fVisualRange;				// 시야 거리

	// 충돌 크기
	Point ptPixelSize;		// 충돌 크기

public:
	S_UnitInfo();
	S_UnitInfo(const S_UnitInfo& s);
	S_UnitInfo& operator=(const S_UnitInfo& s);
};