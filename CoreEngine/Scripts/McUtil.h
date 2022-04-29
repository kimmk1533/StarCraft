#pragma once

#ifdef UNICODE
#define Text(text) text
#else
#define Text(text) TEXT(text)
#endif

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p) = NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p) = NULL; } }

enum class E_Direction : unsigned char
{
	Up,

	Up_Right,
	Up_Right_Right,
	Right_Up,
	Right,
	Right_Down,
	Right_Down_Down,
	Down_Right,

	Down,

	Down_Left,
	Down_Left_Left,
	Left_Down,
	Left,
	Left_Up,
	Left_Up_Up,
	Up_Left,

	Max
};