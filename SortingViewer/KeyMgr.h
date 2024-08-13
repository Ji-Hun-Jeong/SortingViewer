#pragma once
struct KeyInfo
{
	bool isPrev;
	KEY_STATE keyState;
};
class KeyMgr
{
	// Default
public:
	static void Init(HWND hWnd, UINT screenWidth, UINT screenHeight);
	static void Update();

private:
	static void KeyUpdate();
	static void MouseUpdate();

public:
	static bool KeyCheck(KEY_TYPE keyType, KEY_STATE keyState) 
	{ 
		return m_arrKey[(UINT)keyType].keyState == keyState; 
	}
	static Vector2 GetMouseNDCPos();

private:
	static KeyInfo m_arrKey[(UINT)KEY_TYPE::END];

	static POINT m_cursorPos;
	static Vector2 m_prevCursorPos;
	static Vector2 m_curCursorPos;

	static UINT m_screenWidth;
	static UINT m_screenHeight;
	static UINT m_aspect;
	static HWND m_hWnd;

};

