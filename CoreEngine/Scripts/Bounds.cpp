#include "stdafx_Core.h"
#include "Bounds.h"

namespace CoreEngine
{
	S_Bounds::S_Bounds()
	{
		m_pSize = new D3DXVECTOR3();
		m_pCenter = new D3DXVECTOR3();
	}
	S_Bounds::~S_Bounds()
	{
		SAFE_DELETE(m_pSize);
		SAFE_DELETE(m_pCenter);
	}
}