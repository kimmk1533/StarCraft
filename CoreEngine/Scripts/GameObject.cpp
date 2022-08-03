#include "stdafx_Core.h"
#include "GameObject.h"

#include "Transform.h"

namespace CoreEngine
{
	C_GameObject::C_GameObject()
	{
		m_pTransform = nullptr;
	}
	C_GameObject::~C_GameObject()
	{
		Destroy();
	}

	HRESULT C_GameObject::Create()
	{
		m_pTransform = std::make_unique<C_Transform>();
		SAFE_CREATE(m_pTransform);

		return S_OK;
	}
	void C_GameObject::Destroy()
	{
		m_pTransform = nullptr;
	}
}