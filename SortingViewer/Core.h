#pragma once
#include "CoreBase.h"
class Mesh;
class Camera;
class ConstBuffer;
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
private:
	void UpdateGlobalConst();
private:
	shared_ptr<Mesh> m_mesh;
	shared_ptr<Camera> m_camera;
	GlobalConst m_globalConst;
	shared_ptr<ConstBuffer> m_globalConstBuffer;
};

