#pragma once

namespace Game
{
	using namespace CoreEngine;

	enum class E_RaceType : unsigned char
	{
		Neutral,
		Protoss,
		Terran,
		Zerg,

		Max
	};
	enum class E_UnitAtkType : unsigned char
	{
		None,
		NormalType,
		ExplosiveType,
		ConcussiveType,
		SpellType,

		Max
	};
	enum class E_UnitDefType : unsigned char
	{
		None,
		SmallType,
		MediumType,
		LargeType,

		Max
	};

	struct S_UnitInfo
	{
	public:
		// 종족
		E_RaceType m_RaceType;				// 종족

		// 생산
		WORD m_wMineral;					// 필요 미네랄
		WORD m_wVespeneGas;					// 필요 가스
		WORD m_wPopulation;					// 필요 인구수
		WORD m_wProductionTime;				// 필요 생산 시간(ms단위)
		E_KeyCode m_ShortcutKey;			// 생산 단축키

		// 이동
		bool m_bIsMovable;					// 이동 가능 여부
		FLOAT m_fMoveSpeed;					// 이동 속도
		bool m_bIsAir;						// 공중 유닛인지

		// 공격
		bool m_bIsAttackable;				// 공격 가능 여부
		E_UnitAtkType m_AttackType;			// 공격 타입(일반형, 폭발형, 진동형, 방어무시형)
		WORD m_wAttackPoint;				// 공격력
		FLOAT m_fAttackSpeed;				// 공격 속도
		FLOAT m_fAttackRange;				// 공격 사거리

		// 방어
		bool m_bIsHitable;					// 타격 가능 여부(타겟팅 가능 여부)
		E_UnitDefType m_DeffenseType;		// 방어 타입(소형, 중형, 대형)
		FLOAT m_fMaxHP;						// 최대 체력
		FLOAT m_fHP;						// 체력

		// 쉴드
		bool m_bUseShield;					// 쉴드 사용 여부
		FLOAT m_fMaxShield;					// 최대 쉴드
		FLOAT m_fShield;					// 쉴드
		WORD m_wDeffensePoint;				// 방어력

		// 에너지(마나)
		bool m_bUseEnergy;					// 에너지 사용 여부
		FLOAT m_fMaxEnergy;					// 최대 에너지
		FLOAT m_fEnergy;					// 에너지

		// 시야
		FLOAT m_fVisualRange;				// 시야 거리

		// 충돌 크기
		POINT m_PixelSize;					// 충돌 크기

	public:
		S_UnitInfo();
		S_UnitInfo(const S_UnitInfo& s);
		S_UnitInfo& operator=(const S_UnitInfo& s);

	};
}