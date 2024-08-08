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
enum class IBL_TYPE
{
	SPECULAR,
	IRRADIANCE,
	LUT,
	END,
};

enum class SORT_TYPE
{
	SELECT,
	BUBBLE,
	INSERT,
	MERGE,
	QUICK,
	END,
};