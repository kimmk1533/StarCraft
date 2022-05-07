//#include <stdafx.h>
#include "..\..\CoreEngine\Scripts\stdafx.h"

#include "Unit.h"
#include "Utility.h"

namespace Game
{
	C_Unit::C_Unit()
	{
		m_Info = nullptr;

		m_TargetPos = m_Position = D3DXVECTOR2(100, 100);

		m_Animator = nullptr;
	}
	C_Unit::~C_Unit()
	{
		Destroy();
	}

	HRESULT C_Unit::Create()
	{
		m_Info = new S_UnitInfo();

		m_Animator = new C_Animator();
		m_Animator->SetPosition(m_Position);

		return S_OK;
	}

	void C_Unit::Destroy()
	{
		SAFE_DELETE(m_Animator);

		SAFE_DELETE(m_Info);
	}
}