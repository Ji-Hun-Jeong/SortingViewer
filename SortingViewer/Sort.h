#pragma once
#include "Mesh.h"
class Sort
{
public:
	Sort(string strOfSortType);
	Sort(const Sort&) = delete;
	Sort(Sort&&) = delete;
	virtual ~Sort();

public:
	virtual void DoSort(vector<shared_ptr<Mesh>>& vec) = 0;
	virtual void UpdateContinue(vector<shared_ptr<Mesh>>& vec, bool* permitSort);
	virtual void UpdateOneStep(vector<shared_ptr<Mesh>>& vec);

	void StartSort(vector<shared_ptr<Mesh>>& vec);
	void OneFrameRenderingPreparation(shared_ptr<Mesh>& stdMesh, shared_ptr<Mesh>& cmpMesh1
		, shared_ptr<Mesh>& cmpMesh2);
	bool DoingSort() { return m_doingSort; }
	bool IsSleep() { return m_sleep; }
	void WaitToSort() { m_sortDone = false; }
	void OneTimeFinish();
	float GetTimeTakenToSort() { return m_timeTakenToSort; }
	const string& GetSortTypeByString() { return m_strOfSortType; }

protected:
	void WakeUp();
	void Destroy();
	void ClearState();
	void UnlockFindPos(vector<shared_ptr<Mesh>>& vec);
	void CopyHeight(shared_ptr<Mesh>& mesh, float height);
	void SwapMeshData(shared_ptr<Mesh>& mesh1, shared_ptr<Mesh>& mesh2);
	void Print(vector<shared_ptr<Mesh>>& vec);
	void SetMeshConst(shared_ptr<Mesh>& stdMesh, shared_ptr<Mesh>& cmpMesh1
		, shared_ptr<Mesh>& cmpMesh2, bool onOff);

	bool m_startSort = false;
	bool m_oneTimeFinish = false;
	bool m_destroy = false;
	bool m_sortDone = false;
	bool m_doingSort = false;
	bool m_sleep = false;
	string m_strOfSortType;

	thread m_sortThread;
	mutex m_mtx;
	condition_variable m_cv;
	shared_ptr<Mesh> m_nullPtr;

private:
	float m_timeTakenToSort = 0.0f;
};

