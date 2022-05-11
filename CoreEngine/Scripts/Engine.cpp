#include "stdafx.h"

namespace CoreEngine
{
	//const LPCWSTR	C_Engine::m_Title = LPCWSTR(L"DirectX StarCraft");
	//HINSTANCE		C_Engine::m_hInst = nullptr;
	//HWND			C_Engine::m_hWnd = nullptr;
	//DWORD			C_Engine::m_dWinStyle = WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
	//DWORD			C_Engine::m_dScnX = 800;						// Screen Width
	//DWORD			C_Engine::m_dScnY = 600;						// Screen Height
	//bool			C_Engine::m_bWindow = TRUE;					// WindowMode
	//bool			C_Engine::m_bShowCusor = TRUE;				// Show Cusor

	C_Engine* g_pApp;

	C_Engine::C_Engine() : m_Title(LPCWSTR(L"DirectX StarCraft"))
	{
		m_hInst = nullptr;
		m_hWnd = nullptr;
		m_dWinStyle = WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
		m_dScnX = 800;						// Screen Width
		m_dScnY = 600;						// Screen Height
		m_bWindow = TRUE;					// WindowMode
		m_bShowCusor = TRUE;				// Show Cusor
	}

	HRESULT C_Engine::Init(HINSTANCE _hInst)
	{
		m_hInst = _hInst;

		WNDCLASS wc =								// Register the window class
		{
			CS_CLASSDC
			, WndProc
			, 0L
			, 0L
			, m_hInst
			, NULL
			, LoadCursor(NULL,IDC_ARROW)
			, (HBRUSH)GetStockObject(WHITE_BRUSH)
			, NULL
			, m_Title
		};

		RegisterClass(&wc);

		RECT rc;									//Create the application's window

		SetRect(&rc, 0, 0, m_dScnX, m_dScnY);
		AdjustWindowRect(&rc, m_dWinStyle, FALSE);

		int iScnSysW = ::GetSystemMetrics(SM_CXSCREEN);
		int iScnSysH = ::GetSystemMetrics(SM_CYSCREEN);

		m_hWnd = CreateWindow(m_Title
			, m_Title
			, m_dWinStyle
			, (iScnSysW - (rc.right - rc.left)) / 2
			, (iScnSysH - (rc.bottom - rc.top)) / 2
			, (rc.right - rc.left)
			, (rc.bottom - rc.top)
			, GetDesktopWindow()
			, NULL
			, m_hInst
			, NULL);

		// DirectX 초기화
		////////////////////////////////////////////////////////////////////////////
		//

		// D3D생성
		if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
			return E_FAIL;

		// D3D를 통해서 화면 모드를 찾는다.
		D3DDISPLAYMODE d3ddm;
		if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
			return E_FAIL;

		// 디바이스를 생성하기 위해서는 프리젠트 파라메터 구조체가 필요
		// 전부 0으로 셋팅한다음 일부만 변수세팅
		ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
		m_d3dpp.Windowed = m_bWindow;
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_d3dpp.BackBufferFormat = d3ddm.Format;
		m_d3dpp.BackBufferCount = 2;
		m_d3dpp.BackBufferWidth = m_dScnX;
		m_d3dpp.BackBufferHeight = m_dScnY;
		m_d3dpp.EnableAutoDepthStencil = TRUE;
		m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		// Device
		// D3DADAPTER_DEFAULT: 대부분의 그래픽카드는 모노. 듀얼일 경우 디폴트
		// D3DDEVTYPE_HAL : 하드웨어 가속(가장 큰 속도)을 받을 것인가. 하드웨어 지원이 없을 경우 D3D는 소프트웨어로 이를 대체 할 수 있다.
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			m_hWnd, D3DCREATE_MIXED_VERTEXPROCESSING, &m_d3dpp, &m_pd3dDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
				m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dpp, &m_pd3dDevice)))
			{
				SAFE_RELEASE(m_pd3dDevice);
				SAFE_RELEASE(m_pD3D);

				return E_FAIL;
			}
		}

		// Sprite
		// DX의 스프라이트는 디바이스가 생성된 후에 만들어야 한다.
		if (FAILED(D3DXCreateSprite(m_pd3dDevice, &m_pd3dSprite)))
		{
			SAFE_RELEASE(m_pd3dDevice);
			SAFE_RELEASE(m_pD3D);

			return E_FAIL;
		}

		//
		////////////////////////////////////////////////////////////////////////////

		ShowWindow(m_hWnd, SW_SHOW);
		UpdateWindow(m_hWnd);
		::ShowCursor(m_bShowCusor);

		if (FAILED(this->Create()))
			return E_FAIL;

		return S_OK;
	}
	HRESULT C_Engine::Run()
	{
		MSG msg;
		memset(&msg, 0, sizeof(msg));

		m_pTime = new C_Time();

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				m_pTime->Update();

				if (FAILED(m_pInput->Update()))
					break;

				if (FAILED(RPR()))
					break;

				// 후면버퍼 전면버퍼 교체( flipping)
				m_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
			}
		}

		UnregisterClass(m_Title, m_hInst);

		return S_OK;
	}
	void C_Engine::Cleanup()
	{
		// 게임 데이터 해제
		Destroy();

		// DX 해제
		SAFE_RELEASE(m_pd3dSprite);
		SAFE_RELEASE(m_pd3dDevice);
		SAFE_RELEASE(m_pD3D);
	}
	HRESULT C_Engine::RPR()
	{
		if (NULL == m_pd3dDevice)
			return E_FAIL;

		if (FAILED(Update(m_pTime->DeltaTime())))
			return E_FAIL;

		if (FAILED(Render()))
			return E_FAIL;

		return S_OK;
	}

	HRESULT C_Engine::Create()
	{
		m_pSprite = new C_Sprite();
		if (FAILED(m_pSprite->Create(m_pd3dSprite)))
			return E_FAIL;

		m_pInput = new C_Input();
		if (FAILED(m_pInput->Create(m_hWnd)))
			return E_FAIL;

		return S_OK;
	}

	void C_Engine::Destroy()
	{
		SAFE_DELETE(m_pInput);
		SAFE_DELETE(m_pSprite);
	}

	HRESULT C_Engine::Update(float deltaTime)
	{
		return S_OK;
	}

	HRESULT C_Engine::Render()
	{
		m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 120, 160), 1.0f, 0);

		/*if (FAILED(m_pd3dDevice->BeginScene()))
			return E_FAIL;

		D3DXVECTOR2			vcPos(50, 50);
		D3DXVECTOR2			vcScale(1, 1);
		RECT				rc1 { 0, 0, 64, 64 };
		LPDIRECT3DTEXTURE9	pTex = m_Marine->GetTexture()->GetTexture();

		pTex = marine->GetTexture()->GetTexture();

		m_TestTexture->GetImageRect(&rc1);

		m_pSprite->Draw(pTex, &rc1, &vcScale, NULL, 0, &vcPos, D3DXCOLOR(1, 1, 1, 1));

		vcPos.x += 100;
		vcPos.y += 100;

		m_pSprite->Draw(pTex, &rc1, &vcScale, NULL, 0, &vcPos, D3DXCOLOR(1, 1, 1, 1));

		// EndScene
		m_pd3dDevice->EndScene();*/

		return S_OK;
	}


	LRESULT C_Engine::MsgProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
	{
		switch (_msg)
		{
		case WM_KEYDOWN:
		{
			switch (_wParam)
			{
			case VK_ESCAPE:
			{
				SendMessage(_hWnd, WM_DESTROY, 0, 0);
				break;
			}
			}

			return 0;

		}
		case WM_DESTROY:
		{
			Cleanup();
			PostQuitMessage(0);
			return 0;
		}
		}

		return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
	}
	LRESULT WINAPI C_Engine::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return g_pApp->MsgProc(hWnd, msg, wParam, lParam);
	}
}