#include "stdafx.h"

D3DPRESENT_PARAMETERS	C_FrameWork::m_d3dpp = D3DPRESENT_PARAMETERS();
LPDIRECT3D9				C_FrameWork::m_pD3D = nullptr;
LPDIRECT3DDEVICE9		C_FrameWork::m_pd3dDevice = nullptr;
LPD3DXSPRITE			C_FrameWork::m_pd3dSprite = nullptr;

C_Time*					C_FrameWork::m_Time = nullptr;
C_Sprite*				C_FrameWork::m_pLcSprite = nullptr;
C_Input*				C_FrameWork::m_pLcInput = nullptr;