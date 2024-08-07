#pragma once
enum class KEY_TYPE
{
	Q, W, E, A, S, D, F,
	LBUTTON, RBUTTON, ESC,
	B1, B2, B3, B4, SPACE,
	END,
};
enum class KEY_STATE
{
	NONE,
	TAP,
	AWAY,
	HOLD,
};