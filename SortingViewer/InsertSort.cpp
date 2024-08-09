#include "pch.h"
#include "InsertSort.h"

void InsertSort::StartSort(vector<shared_ptr<Mesh>>& vec)
{
	m_doingSort = true;
	int n = vec.size();
	for (int i = 1; i < n; i++)
	{
		float key = vec[i]->GetScale().y;

		int j = i - 1;
		for (; j >= 0; j--)
		{
			if (vec[j + 1]->GetScale().y > vec[j]->GetScale().y)
				break;

			SwapMeshData(vec[j + 1], vec[j]);

			if (m_destroy)
				break;

			if (m_oneTimeFinish)
				continue;

			SetMeshConst(vec[j + 1], vec[j], m_nullPtr, true);

			std::unique_lock<mutex> lock(m_mtx);
			m_sleep = true;
			m_cv.wait(lock, [this] {return !m_sleep; });

			SetMeshConst(vec[j + 1], vec[j], m_nullPtr, false);
		}
	}
	m_sortDone = true;
	m_doingSort = false;
}
