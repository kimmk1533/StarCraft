#pragma once
#include "FrameWork.h"

namespace CoreEngine
{
	class C_Engine;

	extern C_Engine* g_pApp;

	class C_Engine : public C_FrameWork, public IUpdatable, public IRenderable
	{
	public:
		C_Engine();

	public:
		virtual HRESULT	Create() override;
		virtual void	Destroy() override;

		virtual HRESULT	Update(const FLOAT& deltaTime) override;
		virtual HRESULT	Render() override;

	private:
#ifdef UNICODE
		static const LPCWSTR		m_Title;
#else
		static const LPCSTR			m_Title;
#endif // UNICODE

		static HINSTANCE			m_hInst;
		static HWND					m_hWnd;
		static DWORD				m_dWinStyle;
		static DWORD				m_dScnX;			// Screen Width
		static DWORD				m_dScnY;			// Screen Height
		static bool					m_bWindow;			// WindowMode
		static bool					m_bShowCusor;		// Show Cusor

		static LPD3DXSPRITE			m_pd3dSprite;		// 2D Sprite
		static C_Time*				m_pTime;			// System Time

	public:
		// Window+Device관련 함수들
		virtual HRESULT	Init(HINSTANCE _hInst);
		HRESULT	Run();
		void	Cleanup();
		HRESULT RPR();	// 렌더링 파이프라인

		virtual LRESULT MsgProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
		static LRESULT WINAPI WndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

	};
}