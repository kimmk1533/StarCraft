#include "stdafx_Core.h"

namespace CoreEngine
{
	D3DPRESENT_PARAMETERS	C_FrameWork::m_d3dpp = D3DPRESENT_PARAMETERS();		// D3D Parameter
	LPDIRECT3D9				C_FrameWork::m_pD3D = nullptr;						// D3D
	LPDIRECT3DDEVICE9		C_FrameWork::m_pd3dDevice = nullptr;				// Device

	HWND					C_FrameWork::m_hWnd = nullptr;
}