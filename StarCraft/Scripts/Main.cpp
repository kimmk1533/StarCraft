#include "..\..\CoreEngine\Scripts\stdafx.h"

#include "Main.h"
#include "Cursor.h"
#include "Marine.h"
#include "MarineManager.h"
#include "SelectManager.h"

namespace Game
{
	using namespace CoreEngine;

	const int Count = 1;

	HRESULT C_Main::Init(HINSTANCE _hInst)
	{
		if (FAILED(C_Engine::Init(_hInst)))
			return E_FAIL;

		return S_OK;
	}
	LRESULT C_Main::MsgProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
	{
		switch (_msg)
		{
		case WM_CREATE:
			ShowCursor(false);
			break;
		case WM_SETCURSOR:
			ShowCursor(false);
			break;
		case WM_DESTROY:
			ShowCursor(true);
			break;
		}

		return C_Engine::MsgProc(_hWnd, _msg, _wParam, _lParam);
	}
	HRESULT C_Main::Create()
	{
		if (FAILED(C_Engine::Create()))
			return E_FAIL;

		SAFE_CREATE(C_SelectManager::GetI());

		m_pCursor = new C_Cursor();
		SAFE_CREATE_POINTER(m_pCursor);

		SAFE_CREATE(C_MarineManager::GetI());

		m_Marine = new C_Marine * [Count];
		for (int i = 0; i < Count; ++i)
		{
			m_Marine[i] = new C_Marine();

			SAFE_CREATE_POINTER(m_Marine[i]);
		}

		return S_OK;
	}
	void C_Main::Destroy()
	{
		for (int i = 0; i < Count; ++i)
		{
			SAFE_DELETE(m_Marine[i]);
		}
		SAFE_DELETE_ARRAY(m_Marine);
	}

	HRESULT C_Main::Update(const FLOAT& _deltaTime)
	{
		if (FAILED(C_Engine::Update(_deltaTime)))
			return E_FAIL;

		SAFE_UPDATE_POINTER(m_pCursor);

		for (int i = 0; i < Count; ++i)
		{
			SAFE_UPDATE_POINTER(m_Marine[i]);
		}

		return S_OK;
	}

	HRESULT C_Main::Render()
	{
		if (FAILED(C_Engine::Render()))
			return E_FAIL;

		if (FAILED(m_pd3dDevice->BeginScene()))
			return E_FAIL;

		for (int i = 0; i < Count; ++i)
		{
			SAFE_RENDER_POINTER(m_Marine[i]);
		}

		SAFE_RENDER_POINTER(m_pCursor);

		// EndScene
		m_pd3dDevice->EndScene();

		return S_OK;
	}
}