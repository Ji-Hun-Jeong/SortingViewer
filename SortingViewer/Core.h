#pragma once
#include "CoreBase.h"
class Mesh;
class Camera;
class ConstBuffer;
#include "Sorter.h"
#include "Texture2D.h"
class Core
	: public CoreBase
{
public:
	Core();
	~Core();
public:
	void Init(HWND hWnd, UINT width, UINT height);
	void Progress();

private:
	void UpdateGUI();
	void Update(float dt);
	void FinalUpdate(float dt);
	void Render();
	void UpdateGlobalConst();
	void CreateSorter(SORTMODE_TYPE sortType);

private:
	shared_ptr<Mesh> m_skyBox;
	shared_ptr<Camera> m_camera;

	unique_ptr<Sorter> m_sorter;

	GlobalConst m_globalConst;
	shared_ptr<ConstBuffer> m_globalConstBuffer;

	Texture2D m_arrIBL[(UINT)IBL_TYPE::END];

	SORTMODE_TYPE m_sortMode;
};

