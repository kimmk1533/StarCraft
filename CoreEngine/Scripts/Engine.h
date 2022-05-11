#pragma once
#include "FrameWork.h"

namespace CoreEngine
{
	class C_Engine;

	extern C_Engine* g_pApp;

	class C_Engine : public C_FrameWork, public IUpdatable, public IRenderable
	{
	private:
#ifdef UNICODE
		const LPCWSTR			m_Title;
#else
		const LPCSTR			m_Title;
#endif // UNICODE

		HINSTANCE				m_hInst;
		HWND					m_hWnd;
		DWORD					m_dWinStyle;
		DWORD					m_dScnX;			// Screen Width
		DWORD					m_dScnY;			// Screen Height
		bool					m_bWindow;			// WindowMode
		bool					m_bShowCusor;		// Show Cusor

		LPD3DXSPRITE			m_pd3dSprite;		// 2D Sprite

	public:
		// Window+Device관련 함수들
		virtual HRESULT	Init(HINSTANCE _hInst);
		HRESULT	Run();
		void	Cleanup();
		HRESULT RPR();						// 렌더링 파이프라인

		virtual LRESULT MsgProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
		static LRESULT WINAPI WndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

	public:
		virtual HRESULT	Create() override;
		virtual HRESULT	Update(const float deltaTime) override;
		virtual HRESULT	Render() override;
		virtual void	Destroy() override;

	public:
		C_Engine();
	};
}