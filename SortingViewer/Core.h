#pragma once
#include "CoreBase.h"
class Mesh;
class Core
	: public CoreBase
{
public:
	Core();
public:
	void Init(HWND hWnd, UINT width, UINT height);
	void Progress();
	void Update();
	void Render();
public:

private:
	shared_ptr<Mesh> m_mesh;
	GlobalConst m_globalConst;
};

