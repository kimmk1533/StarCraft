#pragma once
#include <stdafx_Core.h>

namespace Game
{
#pragma region Define DEBUG

#define DEBUG
#define DEBUG_Game_Speed 24

//#define DEBUG_Unit_Draw_HitBox
#define DEBUG_Unit_Draw_MovingPath

//#define DEBUG_MapManager_SaveImage_WalkAbility
//#define DEBUG_MapManager_SingleTexture

#pragma endregion

	static const FLOAT g_InGameFPS = 1.0f / DEBUG_Game_Speed;
	static const FLOAT g_FPS = 1.0f / 20;

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

	static const FLOAT g_DegreeUnit = 360.0f / static_cast<int>(E_Direction::Max);
	static const FLOAT g_RadianUnit = D3DXToRadian(g_DegreeUnit);

}