#include "pch.h"
#include "HeapSort.h"
#include <queue>

using std::priority_queue;

void HeapSort::StartSort(vector<shared_ptr<Mesh>>& vec)
{
	m_doingSort = true;

	priority_queue<float, vector<float>, std::greater<float>> q;
	for (auto& mesh : vec)
		q.push(mesh->GetScale().y);
	
	int i = 0;
	shared_ptr<Mesh> nullMesh;
	while (!q.empty())
	{
		CopyHeight(vec[i], q.top());
		q.pop();

		if (m_destroy)
			break;

		if (m_oneTimeFinish)
			continue;

		SetMeshConst(vec[i], m_nullPtr, nullMesh, true);

		std::unique_lock<mutex> lock(m_mtx);
		m_sleep = true;
		m_cv.wait(lock, [this] {return !m_sleep; });

		SetMeshConst(vec[i], m_nullPtr, nullMesh, false);
		++i;
	}
	
	m_sortDone = true;
	m_doingSort = false;
}
