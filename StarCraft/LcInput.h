#pragma once
#define MAX_INPUT_KEY	256
#define MAX_INPUT_BTN	8

enum class E_KeyCode
{
	None = 0,
	MouseLeftButton = 1,
	MouseRightButton = 2,
	MouseMiddleButton = 4,
	Backspace = 8,
	Tab = 9,
	Clear = 12,
	Return = 13,
	Pause = 19,
	Escape = 27,
	Space = 32,
	LeftArrow = 37,
	UpArrow = 38,
	RightArrow = 39,
	DownArrow = 40,
	A = 97,
	B = 98,
	C = 99,
	D = 100,
	E = 101,
	F = 102,
	G = 103,
	H = 104,
	I = 105,
	J = 106,
	K = 107,
	L = 108,
	M = 109,
	N = 110,
	O = 111,
	P = 112,
	Q = 113,
	R = 114,
	S = 115,
	T = 116,
	U = 117,
	V = 118,
	W = 119,
	X = 120,
	Y = 121,
	Z = 122,

	MAX = MAX_INPUT_KEY
};

class CLcInput
{
public:
	enum class E_InputState
	{
		INPUT_ERROR = -1,					// 에러
		INPUT_NONE = 0,						// 누르지 않은 상태
		INPUT_DOWN = 1,						// 누른 상태
		INPUT_UP = 2,						// 뗀 상태
		INPUT_PRESS = 3,					// 누르고 있는 상태
		INPUT_DBCLC = 4,					// 더블클릭한 상태
	};

protected:
	HWND								m_hWnd;

	BYTE								m_KeyCur[MAX_INPUT_KEY];	// 키보드 현재 상태
	BYTE								m_KeyOld[MAX_INPUT_KEY];	// 키보드 이전 상태
	E_InputState						m_KeyMap[MAX_INPUT_KEY];	// 키보드 맵

	BYTE								m_BtnCur[MAX_INPUT_BTN];	// 마우스 현재 상태
	BYTE								m_BtnOld[MAX_INPUT_BTN];	// 마우스 이전 상태
	E_InputState						m_BtnMap[MAX_INPUT_BTN];	// 마우스 맵

	D3DXVECTOR3							m_vcCur;					// 휠 마우스 Z
	D3DXVECTOR3							m_vcOld;
	D3DXVECTOR3							m_vcEps;

	DWORD								m_dTimeDC;					// Double Click Time Interval
	DWORD								m_dBtnBgn[MAX_INPUT_BTN];	// Double Click Start
	INT									m_dBtnCnt[MAX_INPUT_BTN];	// Double Click Count

public:
	HRESULT		Create(HWND	hWnd);
	HRESULT		Update();
	LRESULT		MsgProc(HWND, UINT, WPARAM, LPARAM);

public:
	bool			KeyDown(E_KeyCode nKey);
	bool			KeyUp(E_KeyCode nKey);
	bool			KeyPress(E_KeyCode nKey);
	E_InputState	KeyState(E_KeyCode nKey);

	bool			BtnDown(E_KeyCode nBtn);
	bool			BtnUp(E_KeyCode nBtn);
	bool			BtnPress(E_KeyCode nBtn);
	E_InputState	BtnState(E_KeyCode nBtn);

	D3DXVECTOR3 GetMousePos();
	D3DXVECTOR3 GetMouseEps();

public:
	CLcInput();
	virtual ~CLcInput();

};
