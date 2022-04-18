#pragma once
#include "FrameWork.h"

class CAnimation;
class CMarine;

class CMain : public CFrameWork
{
private:
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

private:
	CMarine* m_Marine;

public:
	// Window+Device관련 함수들
	HRESULT	Create(HINSTANCE hInst);
	HRESULT	Run();
	void	Cleanup();
	HRESULT RPR();						// 렌더링 파이프라인

	LPDIRECT3DDEVICE9 GetDevice();

public:
	virtual HRESULT	Create() override;
	virtual HRESULT	Update(const float deltaTime) override;
	virtual HRESULT	Render() override;
	virtual void	Destroy() override;

	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	CMain();
};

extern CMain* g_pApp;
