#include "stdafx.h"

#include "Unit.h"

CUnit::CUnit()
{
	m_Info = nullptr;

	m_Animator = nullptr;
}
CUnit::~CUnit()
{
	Destroy();
}

HRESULT CUnit::Create()
{
	m_Info = new S_UnitInfo();

	m_Animator = new CAnimator();

	return S_OK;
}

void CUnit::Destroy()
{
	SAFE_DELETE(m_Animator);

	SAFE_DELETE(m_Info);
}
