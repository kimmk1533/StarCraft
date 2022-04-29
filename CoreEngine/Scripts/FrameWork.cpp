#include "stdafx.h"

D3DPRESENT_PARAMETERS	CFrameWork::m_d3dpp = D3DPRESENT_PARAMETERS();
LPDIRECT3D9				CFrameWork::m_pD3D = nullptr;
LPDIRECT3DDEVICE9		CFrameWork::m_pd3dDevice = nullptr;
LPD3DXSPRITE			CFrameWork::m_pd3dSprite = nullptr;

CTime*					CFrameWork::m_Time = nullptr;
CLcSprite*				CFrameWork::m_pLcSprite = nullptr;
CLcInput*				CFrameWork::m_pLcInput = nullptr;