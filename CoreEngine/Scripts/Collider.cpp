#include "stdafx_Core.h"
#include "Collider.h"

namespace CoreEngine
{
	C_Collider::C_Collider()
	{
		m_pBounds = nullptr;
	}
	C_Collider::~C_Collider()
	{
		Destroy();
	}

	HRESULT C_Collider::Create()
	{
		m_pBounds = new S_Bounds();
		SAFE_CREATE(m_pBounds);

		return S_OK;
	}
	void	C_Collider::Destroy()
	{
		SAFE_DELETE(m_pBounds);
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
	D3DXVECTOR3 C_Collider::GetPosition()
	{
		return m_pBounds->center;
	}
	C_Collider C_Collider::GetCollider()
	{
		return (*this);
	}
	S_Bounds C_Collider::GetBounds()
	{
		return *m_pBounds;
	}
}