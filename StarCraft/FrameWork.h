#pragma once

class CFrameWork
{
protected:
	LPCWSTR					m_Title;
	HINSTANCE				m_hInst;
	HWND					m_hWnd;
	DWORD					m_dWinStyle;
	DWORD					m_dScnX;				// Screen Width
	DWORD					m_dScnY;				// Screen Height
	bool					m_bWindow;				// WindowMode
	bool					m_bShowCusor;			// Show Cusor

	// 항상 포인터들은 NULL로 초기화
	static D3DPRESENT_PARAMETERS	m_d3dpp;
	static LPDIRECT3D9				m_pD3D;			// D3D
	static LPDIRECT3DDEVICE9		m_pd3dDevice;	// Device
	static LPD3DXSPRITE				m_pd3dSprite;	// 2D Sprite

	CTime* m_Time;									// System Time
	CLcSprite* m_pLcSprite;							// 2D를 출력하기 위한 객체

public:
	// 게임 관련 함수들
	virtual HRESULT Create() = 0;
	virtual HRESULT	Update(const float deltaTime) = 0;
	virtual HRESULT	Render() = 0;
	virtual void	Destroy() = 0;

};