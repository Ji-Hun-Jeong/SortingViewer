#include "pch.h"
#include "SelectSort.h"


void SelectSort::StartSort(vector<shared_ptr<Mesh>>& vec)
{
	m_doingSort = true;
	shared_ptr<Mesh> nullMesh(nullptr);
	for (int i = 0; i < vec.size() - 1; ++i)
	{
		int minIdx = i;
		SetMeshConst(vec[i], m_nullPtr, nullMesh, true);
		for (int j = i + 1; j < vec.size(); ++j)
		{
			if (vec[j]->GetScale().y < vec[minIdx]->GetScale().y)
				minIdx = j;

			if (m_destroy)
				break;

			if (m_oneTimeFinish)
				continue;

			SetMeshConst(m_nullPtr, vec[j], vec[minIdx],true);

			std::unique_lock<mutex> lock(m_mtx);
			m_sleep = true;
			m_cv.wait(lock, [this] {return !m_sleep; });

			SetMeshConst(m_nullPtr, vec[j], vec[minIdx], false);
		}

		SwapMeshData(vec[i], vec[minIdx]);
		SetMeshConst(vec[i], m_nullPtr, nullMesh, false);
		vec[i]->GetMeshConst().findPos = true;
	}
	UnlockFindPos(vec);
	m_sortDone = true;
	m_doingSort = false;
}
