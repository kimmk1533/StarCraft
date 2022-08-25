#include "stdafx_Core.h"
#include "Bounds.h"

namespace CoreEngine
{
	S_Bounds::S_Bounds()
	{
		m_pSize = nullptr;
		m_pCenter = nullptr;
	}
	S_Bounds::~S_Bounds()
	{
		Destroy();
	}
	HRESULT S_Bounds::Create()
	{
		m_pSize = std::make_shared<D3DXVECTOR3>(0.0f, 0.0f, 0.0f);
		m_pCenter = std::make_shared<D3DXVECTOR3>(0.0f, 0.0f, 0.0f);

		return S_OK;
	}
	void S_Bounds::Destroy()
	{
		m_pSize = nullptr;
		m_pCenter = nullptr;
	}

	std::ostream& operator<<(std::ostream& _out, const S_Bounds& _bounds)
	{
		_out << " size(" << _bounds.c_size.x << ", " << _bounds.c_size.y << ")\n";
		_out << " center(" << _bounds.c_center.x << ", " << _bounds.c_center.y << ")\n";
		_out << " min(" << _bounds.c_min.x << ", " << _bounds.c_min.y << ")\n";
		_out << " max(" << _bounds.c_max.x << ", " << _bounds.c_max.y << ")\n";

		return _out;
	}
}