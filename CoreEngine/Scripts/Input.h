#pragma once

namespace CoreEngine
{
#define MAX_INPUT_BTN	3
#define MAX_INPUT_KEY	256

	enum class E_MouseCode : char;
	enum class E_KeyCode : short;

	class C_Input final
	{
	public:
		enum class E_InputState
		{
			INPUT_ERROR = -1,					// 에러
			INPUT_NONE = 0,						// 누르지 않은	상태
			INPUT_DOWN = 1,						// 누른			상태
			INPUT_UP = 2,						// 뗀			상태
			INPUT_PRESS = 3,					// 누르고 있는	상태
			INPUT_DBCLC = 4,					// 더블클릭한		상태
			INPUT_DBCLC_UP = 5,					// 더블클릭 뗀	상태
		};

	public:
		C_Input();
		~C_Input();

	public:
		HRESULT		Create(HWND	hWnd);
		HRESULT		Update(const FLOAT& _deltaTime);

	public:
		LRESULT		MsgProc(HWND, UINT, WPARAM, LPARAM);

	private: // 마우스
		HWND								m_hWnd;

		BYTE								m_BtnCur[MAX_INPUT_BTN];	// 마우스 현재 상태
		BYTE								m_BtnOld[MAX_INPUT_BTN];	// 마우스 이전 상태
		E_InputState						m_BtnMap[MAX_INPUT_BTN];	// 마우스 맵

		D3DXVECTOR3							m_vcCur;					// 휠 마우스 Z
		D3DXVECTOR3							m_vcOld;
		D3DXVECTOR3							m_vcEps;

		DWORD								m_dTimeDC;					// Double Click Time Interval
		DWORD								m_dBtnBgn[MAX_INPUT_BTN];	// Double Click Start
		INT									m_dBtnCnt[MAX_INPUT_BTN];	// Double Click Count

	private: // 키보드
		BYTE								m_KeyCur[MAX_INPUT_KEY];	// 키보드 현재 상태
		BYTE								m_KeyOld[MAX_INPUT_KEY];	// 키보드 이전 상태
		E_InputState						m_KeyMap[MAX_INPUT_KEY];	// 키보드 맵

	public: // 마우스
		bool			GetMouseDown(E_MouseCode nBtn);
		bool			GetMouseUp(E_MouseCode nBtn);
		bool			GetMouse(E_MouseCode nBtn);
		E_InputState	GetMouseState(E_MouseCode nBtn);

		D3DXVECTOR3		GetMousePos();
		D3DXVECTOR3		GetMouseEps();

	public: // 키보드
		bool			GetKeyDown(E_KeyCode nKey);
		bool			GetKeyUp(E_KeyCode nKey);
		bool			GetKey(E_KeyCode nKey);
		E_InputState	GetKeyState(E_KeyCode nKey);

	};

	enum class E_MouseCode : char
	{
		None = -1,

		Left = 0,
		Right = 1,
		Middle = 2,

		Max = MAX_INPUT_BTN
	};
	enum class E_KeyCode : short
	{
		None = -1,
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
}