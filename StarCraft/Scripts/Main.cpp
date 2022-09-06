#include "stdafx.h"

#include "Main.h"

#include "Cursor.h"

#pragma region include Manager
#include <TextureManager.h>
#include "MapManager.h"
#include "MarineManager.h"
#include "SelectManager.h"
#pragma endregion

#pragma region include Unit
#include "Marine.h"
#pragma endregion

namespace Game
{
	void C_Main::ClipCursor(HWND _hWnd)
	{
		RECT rc;
		POINT lt, rb;

		// 클라이언트 크기 받아오기
		GetClientRect(_hWnd, &rc);

		// 좌표 입력
		lt.x = rc.left; lt.y = rc.top;
		rb.x = rc.right; rb.y = rc.bottom;

		// 윈도우상 좌표로 변환
		ClientToScreen(_hWnd, &lt);
		ClientToScreen(_hWnd, &rb);

		// 좌표 적용
		SetRect(&rc, lt.x, lt.y, rb.x, rb.y);

		// 마우스 클리핑
		::ClipCursor(&rc);
	}

	HRESULT C_Main::Create()
	{
		if (FAILED(C_Engine::Create()))
			return E_FAIL;

		SAFE_CREATE(C_TextureManager::GetI());
		SAFE_CREATE(C_MapManager::GetI());
		SAFE_CREATE(C_SelectManager::GetI());
		SAFE_CREATE(C_MarineManager::GetI());

		C_TextureManager::GetI()->AddTexture("Cursor.png", "Cursor");
		C_TextureManager::GetI()->AddTexture("Marine.png", "Marine");

		C_MapManager::GetI()->LoadMap("Lost Temple");
		C_MapManager::GetI()->CreateTerrain();
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

		SAFE_UPDATE(C_TextureManager::GetI());
		SAFE_UPDATE(C_MapManager::GetI());
		SAFE_UPDATE(C_MarineManager::GetI());
		SAFE_UPDATE(C_SelectManager::GetI());

		return S_OK;
	}
	HRESULT C_Main::Render()
	{
		FAILED_CHECK_RETURN(C_Engine::Render());

		// Begine Scene
		FAILED_CHECK_RETURN(m_pd3dDevice->BeginScene());

		SAFE_RENDER(C_MapManager::GetI());
		SAFE_RENDER(C_MarineManager::GetI());
		SAFE_RENDER(C_SelectManager::GetI());

		// End Scene
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
		case WM_SIZE:
			ClipCursor(_hWnd);
			break;
		case WM_SETFOCUS:
			ClipCursor(_hWnd);
			break;
		case WM_SETCURSOR:
			ShowCursor(false);
			break;
		case WM_DESTROY:
			ShowCursor(true);
			::ClipCursor(nullptr);
			break;
		}

		return C_Engine::MsgProc(_hWnd, _msg, _wParam, _lParam);
	}
}