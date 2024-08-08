#pragma once
#include "Mesh.h"
class Sort
{
public:
	virtual void StartSort(vector<shared_ptr<Mesh>>& vec) = 0;
	void Update(vector<shared_ptr<Mesh>>& vec, bool& permitSort);
	
	bool Done() { return m_sortDone; }
	void WaitToSort() { m_sortDone = false; }

	bool DoingSort() { return m_doingSort; }

	bool IsSleep() { return m_sleep; }

	void OneTimeFinish();

	void WakeUp();
	void Destroy();
	virtual ~Sort()
	{
		Destroy();
	}
protected:
	void CopyHeight(shared_ptr<Mesh>& mesh, float height);
	void SwapMeshData(shared_ptr<Mesh>& mesh1, shared_ptr<Mesh>& mesh2);
	void Print(vector<shared_ptr<Mesh>>& vec);

	bool m_startSort = false;
	bool m_oneTimeFinish = false;
	bool m_destroy = false;
	bool m_sortDone = false;
	bool m_doingSort = false;
	bool m_sleep = false;

	thread m_sortThread;
	mutex m_mtx;
	condition_variable m_cv;
};

