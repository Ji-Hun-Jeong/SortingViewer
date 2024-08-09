#include "pch.h"
#include "SelectSort.h"


void SelectSort::StartSort(vector<shared_ptr<Mesh>>& vec)
{
	m_doingSort = true;
	for (int i = 0; i < vec.size() - 1; ++i)
	{
		int minIdx = i;
		for (int j = i + 1; j < vec.size(); ++j)
		{
			if (vec[j]->GetScale().y < vec[minIdx]->GetScale().y)
				minIdx = j;

			if (m_destroy)
				break;

			if (m_oneTimeFinish)
				continue;

			SetMeshConst(vec[minIdx], vec[j], m_nullPtr, true);

			std::unique_lock<mutex> lock(m_mtx);
			m_sleep = true;
			m_cv.wait(lock, [this] {return !m_sleep; });

			SetMeshConst(vec[minIdx], vec[j], m_nullPtr, false);
		}

		SwapMeshData(vec[i], vec[minIdx]);
	}
	m_sortDone = true;
	m_doingSort = false;
}
