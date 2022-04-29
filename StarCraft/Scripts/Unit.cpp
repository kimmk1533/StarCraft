
#include "Unit.h"

CUnit::CUnit()
{
	m_Info = nullptr;

	m_TargetPos = m_Position = D3DXVECTOR2(100, 100);	

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
	m_Animator->SetPosition(m_Position);

	return S_OK;
}

void CUnit::Destroy()
{
	SAFE_DELETE(m_Animator);

	SAFE_DELETE(m_Info);
}
