#include "stdafx.h"

#include "Main.h"

#include "Cursor.h"

#pragma region include Manager
#include "MarineManager.h"
#include "SelectManager.h"
#pragma endregion

#pragma region include Unit
#include "Marine.h"
#pragma endregion

namespace Game
{
	HRESULT C_Main::Create()
	{
		if (FAILED(C_Engine::Create()))
			return E_FAIL;

		SAFE_CREATE(C_SelectManager::GetI());

		SAFE_CREATE(C_MarineManager::GetI());

		for (int i = 0; i < 1; ++i)
			C_MarineManager::GetI()->SpawnUnit();

		return S_OK;
	}
	void C_Main::Destroy()
	{

	}

	HRESULT C_Main::Update(const FLOAT& _deltaTime)
	{
		if (FAILED(C_Engine::Update(_deltaTime)))
			return E_FAIL;

		SAFE_UPDATE(C_SelectManager::GetI());

		SAFE_UPDATE(C_MarineManager::GetI());

		return S_OK;
	}
	HRESULT C_Main::Render()
	{
		FAILED_CHECK(C_Engine::Render());

		FAILED_CHECK(m_pd3dDevice->BeginScene());

		SAFE_RENDER(C_MarineManager::GetI());
		SAFE_RENDER(C_SelectManager::GetI());

		// EndScene
		m_pd3dDevice->EndScene();

		return S_OK;
	}

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
}