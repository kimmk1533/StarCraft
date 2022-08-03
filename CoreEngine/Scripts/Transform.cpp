#include "stdafx_Core.h"
#include "Transform.h"

#include "GameObject.h"

namespace CoreEngine
{
	C_Transform::C_Transform()
	{
		m_pPosition = nullptr;
		m_pRotation = nullptr;
		m_pScale = nullptr;

		m_pGameObject = nullptr;
	}
	C_Transform::~C_Transform()
	{
		Destroy();
	}

	HRESULT C_Transform::Create()
	{
		m_pPosition = new D3DXVECTOR3();
		m_pRotation = new D3DXVECTOR3();
		m_pScale = new D3DXVECTOR3();

		return S_OK;
	}
	void C_Transform::Destroy()
	{
		SAFE_DELETE(m_pPosition);
		SAFE_DELETE(m_pRotation);
		SAFE_DELETE(m_pScale);

		m_pGameObject = nullptr;
	}
}