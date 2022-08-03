#include "stdafx_Core.h"

namespace CoreEngine
{
	C_Input::C_Input()
	{
		m_hWnd = NULL; // for Mouse

		ZeroMemory(m_KeyOld, sizeof(m_KeyOld));
		ZeroMemory(m_KeyCur, sizeof(m_KeyCur));
		ZeroMemory(m_KeyMap, sizeof(m_KeyMap));

		ZeroMemory(m_BtnCur, sizeof(m_BtnCur));
		ZeroMemory(m_BtnOld, sizeof(m_BtnOld));
		ZeroMemory(m_BtnMap, sizeof(m_BtnMap));

		m_vcCur = D3DXVECTOR3(0, 0, 0);
		m_vcOld = D3DXVECTOR3(0, 0, 0);
		m_vcEps = D3DXVECTOR3(0, 0, 0);

		m_dTimeDC = 0;
		ZeroMemory(m_dBtnBgn, sizeof(m_dBtnBgn));
		ZeroMemory(m_dBtnCnt, sizeof(m_dBtnCnt));

		SetKeyboardState(m_KeyCur);
	}
	C_Input::~C_Input()
	{

	}

	HRESULT	C_Input::Create(HWND hWnd)
	{
		m_hWnd = hWnd;

		ZeroMemory(m_KeyOld, sizeof(m_KeyOld));
		ZeroMemory(m_KeyCur, sizeof(m_KeyCur));
		ZeroMemory(m_KeyMap, sizeof(m_KeyMap));

		ZeroMemory(m_BtnCur, sizeof(m_BtnCur));
		ZeroMemory(m_BtnOld, sizeof(m_BtnOld));
		ZeroMemory(m_BtnMap, sizeof(m_BtnMap));

		m_vcCur = D3DXVECTOR3(0, 0, 0);
		m_vcOld = D3DXVECTOR3(0, 0, 0);
		m_vcEps = D3DXVECTOR3(0, 0, 0);

		m_dTimeDC = GetDoubleClickTime();

		ZeroMemory(m_dBtnBgn, sizeof(m_dBtnBgn));
		ZeroMemory(m_dBtnCnt, sizeof(m_dBtnCnt));

		m_dBtnBgn[0] = timeGetTime();

		for (int i = 1; i < MAX_INPUT_BTN; ++i)
		{
			m_dBtnBgn[i] = m_dBtnBgn[0];
		}

		ZeroMemory(m_dBtnCnt, sizeof(m_dBtnCnt));

		//UINT ucNumLines=3;  // 3 is the default
		UINT ucNumLines = 0;  // 3 is the default
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ucNumLines, 0);

		return S_OK;
	}
	HRESULT C_Input::Update(const FLOAT& _deltaTime)	// Mouse
	{
		INT i = 0;

		// 1. current 값을 old에 복사, 이전 상태를 저장한다.
		memcpy(m_KeyOld, m_KeyCur, sizeof(m_KeyOld));
		memcpy(m_BtnOld, m_BtnCur, sizeof(m_BtnOld));		// Mouse

		// 2. Current 와 Map의 값을 0으로 초기화
		ZeroMemory(m_KeyCur, sizeof(m_KeyCur));
		ZeroMemory(m_KeyMap, sizeof(m_KeyMap));

		ZeroMemory(m_BtnCur, sizeof(m_BtnCur));				// Mouse
		ZeroMemory(m_BtnMap, sizeof(m_BtnMap));

		// 3. current 이벤트 를 가져옴
		if (!GetKeyboardState(m_KeyCur))
			return E_FAIL;

		// Keyboard
		for (i = 0; i < MAX_INPUT_KEY; ++i)
		{
			BYTE	vKey = m_KeyCur[i] & 0x80;					// 현재의 키보드 상태를 읽어온다.
			m_KeyCur[i] = (vKey) ? 1 : 0;						// 키보드 상태를 0과 1로 정한다.

			INT nOld = m_KeyOld[i];
			INT nCur = m_KeyCur[i];

			if (0 == nOld && 1 == nCur)
				m_KeyMap[i] = E_InputState::INPUT_DOWN;	// Down
			else if (1 == nOld && 0 == nCur)
				m_KeyMap[i] = E_InputState::INPUT_UP;		// UP
			else if (1 == nOld && 1 == nCur)
				m_KeyMap[i] = E_InputState::INPUT_PRESS;	// PRESS
			else
				m_KeyMap[i] = E_InputState::INPUT_NONE;	// NONE
		}

		// Mouse button
		m_BtnCur[0] = m_KeyCur[VK_LBUTTON];	// Left Button
		m_BtnCur[1] = m_KeyCur[VK_RBUTTON];	// Right Button
		m_BtnCur[2] = m_KeyCur[VK_MBUTTON];	// Middle Button

		for (i = 0; i < MAX_INPUT_BTN; ++i)
		{
			INT nOld = m_BtnOld[i];
			INT nCur = m_BtnCur[i];

			if (0 == nOld && 1 == nCur)
				m_BtnMap[i] = E_InputState::INPUT_DOWN;	// Down
			else if (1 == nOld && 0 == nCur)
				m_BtnMap[i] = E_InputState::INPUT_UP;		// UP
			else if (1 == nOld && 1 == nCur)
				m_BtnMap[i] = E_InputState::INPUT_PRESS;	// PRESS
			else
				m_BtnMap[i] = E_InputState::INPUT_NONE;	// NONE
		}

		// 4. Update Mouse Position
		POINT pt;
		::GetCursorPos(&pt);
		::ScreenToClient(m_hWnd, &pt);

		m_vcCur.x = FLOAT(pt.x);
		m_vcCur.y = FLOAT(pt.y);

		m_vcEps = m_vcCur - m_vcOld;

		m_vcOld = m_vcCur;

		// 5. Double Check
		DWORD dBtnCur = timeGetTime();

		for (i = 0; i < MAX_INPUT_BTN; ++i)
		{
			if (E_InputState::INPUT_DOWN == m_BtnMap[i])
			{
				if (1 == m_dBtnCnt[i])
				{
					if ((dBtnCur - m_dBtnBgn[i]) >= m_dTimeDC)
					{
						m_dBtnCnt[i] = 0;
					}
				}

				++m_dBtnCnt[i];

				if (1 == m_dBtnCnt[i])
				{
					m_dBtnBgn[i] = dBtnCur;
				}
				else if (2 == m_dBtnCnt[i])
				{
					m_BtnMap[i] = E_InputState::INPUT_DBCLC;
				}
			}

			if (E_InputState::INPUT_UP == m_BtnMap[i])
			{
				if (1 == m_dBtnCnt[i])
				{
					if ((dBtnCur - m_dBtnBgn[i]) >= m_dTimeDC)
					{
						m_dBtnCnt[i] = 0;
					}
				}
				else if (2 == m_dBtnCnt[i])
				{
					if ((dBtnCur - m_dBtnBgn[i]) <= m_dTimeDC)
					{
						m_BtnMap[i] = E_InputState::INPUT_DBCLC_UP;
					}

					m_dBtnCnt[i] = 0;
				}
			}
		}

		//
		//	mouse_event(MOUSEEVENTF_MOVE, -1, 0, 0,0);

		return S_OK;
	}

	LRESULT C_Input::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_MOUSEWHEEL:
		{
			INT c = HIWORD(wParam);
			INT d = LOWORD(wParam);

			c = short(c);
			m_vcCur.z += FLOAT(c);
			break;
		}
		}

		return 0;
	}

	// 마우스
	bool C_Input::GetMouseDown(E_MouseCode nBtn)
	{
		if (nBtn <= E_MouseCode::None || nBtn >= E_MouseCode::Max)
		{
			ErrorMessageBox("Mouse Down Error");

			return FALSE;
		}

		bool condition1 = m_BtnMap[static_cast<INT>(nBtn)] == E_InputState::INPUT_DOWN;
		bool condition2 = m_BtnMap[static_cast<INT>(nBtn)] == E_InputState::INPUT_DBCLC;

		return condition1 || condition2;
	}
	bool C_Input::GetMouseUp(E_MouseCode nBtn)
	{
		if (nBtn <= E_MouseCode::None || nBtn >= E_MouseCode::Max)
		{
			ErrorMessageBox("Mouse Up Error");

			return FALSE;
		}

		bool condition1 = m_BtnMap[static_cast<INT>(nBtn)] == E_InputState::INPUT_UP;
		bool condition2 = m_BtnMap[static_cast<INT>(nBtn)] == E_InputState::INPUT_DBCLC_UP;

		return condition1 || condition2;
	}
	bool C_Input::GetMouse(E_MouseCode nBtn)
	{
		if (nBtn <= E_MouseCode::None || nBtn >= E_MouseCode::Max)
		{
			ErrorMessageBox("Mouse Pressed Error");

			return FALSE;
		}

		return m_BtnMap[static_cast<INT>(nBtn)] == E_InputState::INPUT_PRESS;
	}
	C_Input::E_InputState C_Input::GetMouseState(E_MouseCode nBtn)
	{
		if (nBtn <= E_MouseCode::None || nBtn >= E_MouseCode::Max)
		{
			ErrorMessageBox("Mouse State Error");

			return E_InputState::INPUT_ERROR;
		}

		return m_BtnMap[static_cast<INT>(nBtn)];
	}

	D3DXVECTOR3 C_Input::GetMousePos()
	{
		return m_vcCur;
	}
	D3DXVECTOR3 C_Input::GetMouseEps()
	{
		return m_vcEps;
	}

	// 키보드
	bool C_Input::GetKeyDown(E_KeyCode nKey)
	{
		if (nKey < E_KeyCode::Backspace || nKey >= E_KeyCode::MAX)
		{
			ErrorMessageBox("Key Down Error");

			return FALSE;
		}

		return m_KeyMap[static_cast<INT>(nKey)] == E_InputState::INPUT_DOWN;
	}
	bool C_Input::GetKeyUp(E_KeyCode nKey)
	{
		if (nKey < E_KeyCode::Backspace || nKey >= E_KeyCode::MAX)
		{
			ErrorMessageBox("Key Up Error");

			return FALSE;
		}

		return m_KeyMap[static_cast<INT>(nKey)] == E_InputState::INPUT_UP;
	}
	bool C_Input::GetKey(E_KeyCode nKey)
	{
		if (nKey < E_KeyCode::Backspace || nKey >= E_KeyCode::MAX)
		{
			ErrorMessageBox("Key Pressed Error");

			return FALSE;
		}

		return m_KeyMap[static_cast<INT>(nKey)] == E_InputState::INPUT_PRESS;
	}
	C_Input::E_InputState C_Input::GetKeyState(E_KeyCode nKey)
	{
		if (nKey < E_KeyCode::Backspace || nKey >= E_KeyCode::MAX)
		{
			ErrorMessageBox("Key State Error");

			return E_InputState::INPUT_ERROR;
		}

		return m_KeyMap[static_cast<INT>(nKey)];
	}
}