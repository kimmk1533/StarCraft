#pragma once
#include "FrameWork.h"

namespace CoreEngine
{
	class C_Engine;

	extern C_Engine* g_pApp;
}

namespace CoreEngine
{
	class C_Engine : public C_FrameWork, public IUpdatable, public IRenderable
	{
	private:
#ifdef UNICODE
		static const LPCWSTR		m_Title;
#else
		static const LPCSTR			m_Title;
#endif // UNICODE

		static HINSTANCE			m_hInst;
		static HWND					m_hWnd;
		static DWORD				m_dwWinStyle;
		static DWORD				m_dwScreenX;		// Screen Width
		static DWORD				m_dwScreenY;		// Screen Height
		static bool					m_bWindow;			// WindowMode
		static bool					m_bShowCusor;		// Show Cusor

		static LPD3DXSPRITE			m_pd3dSprite;		// 2D Sprite
		static LPD3DXLINE			m_pd3dLine;			// 2D Line

		static C_Time*				m_pTime;			// System Time

	public:
		// Window+Device관련 함수들
		virtual HRESULT	Init(HINSTANCE _hInst);
		HRESULT	Run();
		void	Cleanup();
		HRESULT RPR();	// 렌더링 파이프라인

		virtual LRESULT MsgProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
		static LRESULT WINAPI WndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

	public:
		static RECT GetScreenRect();

	public:
		C_Engine();

	public:
		virtual HRESULT	Create() override;
		virtual void	Destroy() override;

	public:
		virtual HRESULT	Update(const FLOAT& _deltaTime) override;
		virtual HRESULT	Render() override;

	};
}