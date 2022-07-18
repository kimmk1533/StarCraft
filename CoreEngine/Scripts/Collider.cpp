#include "stdafx_Core.h"
#include "Collider.h"

namespace CoreEngine
{
	C_Collider::C_Collider()
	{
		m_pBoxCollider = nullptr;
	}
	C_Collider::~C_Collider()
	{
		Destroy();
	}

	HRESULT C_Collider::Create()
	{
		m_pBoxCollider = new S_Bounds();

		return S_OK;
	}
	void	C_Collider::Destroy()
	{
		SAFE_DELETE(m_pBoxCollider);
	}

	void C_Collider::OnCollisionBegin(const C_Collider& _other)
	{
	}
	void C_Collider::OnCollisionStay(const C_Collider& _other)
	{
	}
	void C_Collider::OnCollisionEnd(const C_Collider& _other)
	{
	}
}