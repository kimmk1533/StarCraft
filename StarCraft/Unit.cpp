#include "stdafx.h"

#include "Unit.h"
#include "Animation.h"

CUnit::CUnit()
{
	m_Info = nullptr;

	m_Animation = nullptr;
}
CUnit::~CUnit()
{
	Destroy();
}

HRESULT CUnit::Create()
{
	m_Info = new S_UnitInfo();

	m_Animation = new CAnimation();

	return S_OK;
}

void CUnit::Destroy()
{
	SAFE_DELETE(m_Animation);

	SAFE_DELETE(m_Info);
}
