#pragma once
#define MAX_INPUT_KEY	256
#define MAX_INPUT_BTN	8

enum class E_KeyCode
{
	None = -1,
	MouseLeftButton = 0,
	MouseRightButton = 1,
	MouseMiddleButton = 2,
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
	A = 65,// | 97,
	B = 66,// | 98,
	C = 67,// | 99,
	D = 68,// | 100,
	E = 69,// | 101,
	F = 70,// | 102,
	G = 71,// | 103,
	H = 72,// | 104,
	I = 73,// | 105,
	J = 74,// | 106,
	K = 75,// | 107,
	L = 76,// | 108,
	M = 77,// | 109,
	N = 78,// | 110,
	O = 79,// | 111,
	P = 80,// | 112,
	Q = 81,// | 113,
	R = 82,// | 114,
	S = 83,// | 115,
	T = 84,// | 116,
	U = 85,// | 117,
	V = 86,// | 118,
	W = 87,// | 119,
	X = 88,// | 120,
	Y = 89,// | 121,
	Z = 90,// | 122,

	MAX = MAX_INPUT_KEY
};

class C_Input
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
	C_Input();
	virtual ~C_Input();

};
