#include "stdafx.h"

namespace CoreEngine
{
	D3DPRESENT_PARAMETERS	C_FrameWork::m_d3dpp = D3DPRESENT_PARAMETERS();		// D3D Parameter
	LPDIRECT3D9				C_FrameWork::m_pD3D = nullptr;						// D3D
	LPDIRECT3DDEVICE9		C_FrameWork::m_pd3dDevice = nullptr;				// Device

	C_Time*					C_FrameWork::m_pTime = nullptr;						// System Time
	C_Sprite*				C_FrameWork::m_pSprite = nullptr;					// 2D를 출력하기 위한 객체
	C_Input*				C_FrameWork::m_pInput = nullptr;					// 입력을 위한 객체
}