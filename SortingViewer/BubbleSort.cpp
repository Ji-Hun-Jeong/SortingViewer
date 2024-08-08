#include "pch.h"
#include "BubbleSort.h"

void BubbleSort::StartSort(vector<shared_ptr<Mesh>>& vec)
{
	m_doingSort = true;
	int n = vec.size();
	for (int i = n - 1; i > 0; i--) 
	{
		for (int j = 0; j < i; j++) 
		{
			if (vec[j]->GetScale().y > vec[j + 1]->GetScale().y)
				SwapMeshData(vec[j], vec[j + 1]);

			if (m_destroy)
				break;

			if (m_oneTimeFinish)
				continue;

			std::unique_lock<mutex> lock(m_mtx);
			m_sleep = true;
			m_cv.wait(lock, [this] {return !m_sleep; });
		}
	}
	m_sortDone = true;
	m_doingSort = false;
}
