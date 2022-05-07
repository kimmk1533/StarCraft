#pragma once

namespace Game
{
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