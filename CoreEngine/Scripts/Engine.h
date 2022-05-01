#pragma once
#include "FrameWork.h"

class C_Engine;

extern C_Engine* g_pApp;

class C_Engine : public C_FrameWork
{
private:
	static const LPCWSTR			m_Title;

	static HINSTANCE				m_hInst;
	static HWND						m_hWnd;
	static DWORD					m_dWinStyle;
	static DWORD					m_dScnX;			// Screen Width
	static DWORD					m_dScnY;			// Screen Height
	static bool						m_bWindow;			// WindowMode
	static bool						m_bShowCusor;		// Show Cusor

public:
	// Window+Device관련 함수들
	virtual HRESULT	Create(HINSTANCE hInst);
	HRESULT	Run();
	void	Cleanup();
	HRESULT RPR();						// 렌더링 파이프라인

	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	virtual HRESULT	Create() override;
	virtual HRESULT	Update(const float deltaTime) override;
	virtual HRESULT	Render() override;
	virtual void	Destroy() override;

public:
	C_Engine();
};