#pragma once

namespace Game
{
#define DEBUG_GAME_SPEED 24
	static const FLOAT InGame_FPS = 1.0f / DEBUG_GAME_SPEED;
	static const FLOAT FPS = 1.0f / 20;

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
}