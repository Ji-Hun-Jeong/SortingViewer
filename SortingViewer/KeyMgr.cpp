#include "pch.h"
#include "KeyMgr.h"
#include "Core.h"

KeyInfo KeyMgr::m_arrKey[(UINT)KEY_TYPE::END];
POINT KeyMgr::m_cursorPos;
Vector2 KeyMgr::m_prevCursorPos;
Vector2 KeyMgr::m_curCursorPos;
UINT KeyMgr::m_screenWidth;
UINT KeyMgr::m_screenHeight;
HWND KeyMgr::m_hWnd;
UINT KeyMgr::m_aspect;
UINT g_keyValue[(UINT)KEY_TYPE::END] =
{
	0x51, 0x57, 0x45, 0x41, 0x53, 0x44, 0x46,
	MK_LBUTTON, MK_RBUTTON, VK_ESCAPE,
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, VK_SPACE
};

void KeyMgr::Init(HWND hWnd, UINT screenWidth, UINT screenHeight)
{
	m_hWnd = hWnd;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_aspect = float(m_screenWidth) / m_screenHeight;
}

void KeyMgr::Update()
{
	KeyUpdate();
	if (GetFocus())
		MouseUpdate();
}

Vector2 KeyMgr::GetMouseNDCPos()
{
	float x = (float)m_cursorPos.x;
	float y = (float)m_cursorPos.y;
	x = (x * 2 * m_aspect) / m_screenWidth - m_aspect;
	y = -(y * 2.0f) / m_screenHeight + 1.0f;
	x /= m_aspect;
	return Vector2(x, y);
}

void KeyMgr::KeyUpdate()
{
	for (UINT i = 0; i < (UINT)KEY_TYPE::END; ++i)
	{
		if (GetAsyncKeyState(g_keyValue[i]) & 0x8000)	// 눌림
		{
			if (m_arrKey[i].isPrev)	// 이전에도 눌림
				m_arrKey[i].keyState = KEY_STATE::HOLD;
			else
				m_arrKey[i].keyState = KEY_STATE::TAP;
			m_arrKey[i].isPrev = true;
		}
		else
		{
			if (m_arrKey[i].isPrev)	// 이전에 눌림
				m_arrKey[i].keyState = KEY_STATE::AWAY;
			else
				m_arrKey[i].keyState = KEY_STATE::NONE;
			m_arrKey[i].isPrev = false;
		}
	}
}

void KeyMgr::MouseUpdate()
{
	GetCursorPos(&m_cursorPos);
	ScreenToClient(m_hWnd, &m_cursorPos);
}
