#pragma once

namespace Game
{
	using namespace CoreEngine;

	class C_Main : public C_Engine
	{
	public:
		virtual HRESULT	Create() override;
		virtual void	Destroy() override;

		virtual HRESULT	Update(const FLOAT& _deltaTime) override;
		virtual HRESULT	Render() override;

	public:
		// Window+Device 관련 함수들
		virtual HRESULT	Init(HINSTANCE _hInst) override;

		virtual LRESULT MsgProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam) override;

	};
}