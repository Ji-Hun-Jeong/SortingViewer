#pragma once
#include "CoreBase.h"
class Mesh;
class Camera;
class ConstBuffer;
class Sorter;
#include "Texture2D.h"
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
	shared_ptr<Mesh> m_skyBox;
	shared_ptr<Camera> m_camera;
	shared_ptr<Sorter> m_sorter;
	GlobalConst m_globalConst;
	shared_ptr<ConstBuffer> m_globalConstBuffer;

	Texture2D m_arrIBL[(UINT)IBL_TYPE::END];
};

