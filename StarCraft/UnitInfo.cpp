#include "UnitInfo.h"

S_UnitInfo::S_UnitInfo()
{
	// 종족
	RaceType = E_RaceType::Neutral;			// 종족

	// 생산
	iReqMineral = 0;						// 필요 미네랄
	iReqVespeneGas = 0;						// 필요 가스
	iReqPopulation = 0;						// 필요 인구수
	iReqTime = 0;							// 필요 생산 시간(ms단위)
	ShortcutKey = E_KeyCode::None;			// 생산 단축키

	// 이동
	bIsMovable = FALSE;						// 이동 가능 여부
	fMoveSpeed = 0.0F;						// 이동 속도
	bIsAir = FALSE;							// 공중 유닛인지

	// 공격
	bIsAttackable = FALSE;					// 공격 가능 여부
	AttackType = E_UnitAtkType::None;		// 공격 타입(일반형, 폭발형, 진동형, 방어무시형)
	iAttackPoint = 0;						// 공격력
	fAttackSpeed = 0.0F;					// 공격 속도
	fAttackRange = 0.0F;					// 공격 사거리

	// 방어
	bIsDefensible = FALSE;					// 방어 가능 여부(타겟팅 가능 여부)
	DeffenseType = E_UnitDefType::None;		// 방어 타입(소형, 중형, 대형)
	fMaxHP = 0.0F;							// 최대 체력
	fHP = 0.0F;								// 체력
	fMaxShield = 0.0F;						// 최대 쉴드
	fShield = 0.0F;							// 쉴드
	iDeffensePoint = 0;						// 방어력

	// 에너지(마나)
	bCanUseEnergy = FALSE;					// 에너지 사용 여부
	fMaxEnergy = 0.0F;						// 최대 에너지
	fEnergy = 0.0F;							// 에너지

	// 시야
	fVisualRange = 0.0F;					// 시야 거리

	// 충돌 크기
	ptPixelSize = Point(0, 0);				// 충돌 크기
}
S_UnitInfo::S_UnitInfo(const S_UnitInfo& s)
{
	*this = s;
}
S_UnitInfo& S_UnitInfo::operator=(const S_UnitInfo& s)
{
	// 종족
	RaceType = s.RaceType;					// 종족

	// 생산
	iReqMineral = s.iReqMineral;			// 필요 미네랄
	iReqVespeneGas = s.iReqVespeneGas;		// 필요 가스
	iReqPopulation = s.iReqPopulation;		// 필요 인구수
	iReqTime = s.iReqTime;					// 필요 생산 시간(ms단위)
	ShortcutKey = s.ShortcutKey;			// 생산 단축키

	// 이동
	bIsMovable = s.bIsMovable;				// 이동 가능 여부
	fMoveSpeed = s.fMoveSpeed;				// 이동 속도
	bIsAir = FALSE;							// 공중 유닛인지

	// 공격
	bIsAttackable = s.bIsAttackable;		// 공격 가능 여부
	AttackType = s.AttackType;				// 공격 타입(일반형, 폭발형, 진동형, 방어무시형)
	iAttackPoint = s.iAttackPoint;			// 공격력
	fAttackSpeed = s.fAttackSpeed;			// 공격 속도
	fAttackRange = s.fAttackRange;			// 공격 사거리

	// 방어
	bIsDefensible = s.bIsDefensible;		// 방어 가능 여부(타겟팅 가능 여부)
	DeffenseType = s.DeffenseType;			// 방어 타입(소형, 중형, 대형)
	fMaxHP = s.fMaxHP;						// 최대 체력
	fHP = s.fHP;							// 체력
	fMaxShield = s.fMaxShield;				// 최대 쉴드
	fShield = s.fShield;					// 쉴드
	iDeffensePoint = s.iDeffensePoint;		// 방어력

	// 에너지(마나)
	bCanUseEnergy = s.bCanUseEnergy;		// 에너지 사용 여부
	fMaxEnergy = s.fMaxEnergy;				// 최대 에너지
	fEnergy = s.fEnergy;					// 에너지

	// 시야
	fVisualRange = s.fVisualRange;			// 시야 거리

	// 충돌 크기
	ptPixelSize = s.ptPixelSize;			// 충돌 크기

	return *this;
}

bool S_UnitInfo::IsMovable()
{
	return this->bIsMovable;
}
