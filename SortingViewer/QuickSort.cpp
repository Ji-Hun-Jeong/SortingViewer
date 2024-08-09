#include "pch.h"
#include "QuickSort.h"

void QuickSort::StartSort(vector<shared_ptr<Mesh>>& vec)
{
	m_doingSort = true;

	Quick_Sort(vec, 0, vec.size() - 1);

	m_sortDone = true;
	m_doingSort = false;
}

void QuickSort::Quick_Sort(vector<shared_ptr<Mesh>>& vec, int left, int right)
{
	if (left >= right)
		return;
	int q = Partition(vec, left, right);
	Quick_Sort(vec, left, q - 1);
	Quick_Sort(vec, q + 1, right);
}

int QuickSort::Partition(vector<shared_ptr<Mesh>>& vec, int left, int right)
{
	float pivot = vec[left]->GetScale().y;
	int l = left + 1;
	int r = right;
	do
	{
		while (vec[l]->GetScale().y <= pivot && l < right)
		{
			++l;
			if (m_destroy)
				break;

			if (m_oneTimeFinish)
				continue;

			SetMeshConst(vec[left], vec[l], vec[r], true);

			std::unique_lock<mutex> lock(m_mtx);
			m_sleep = true;
			m_cv.wait(lock, [this] {return !m_sleep; });

			SetMeshConst(vec[left], vec[l], vec[r], false);
		}

		while (vec[r]->GetScale().y > pivot && r > 0)
		{
			--r;
			if (m_destroy)
				break;

			if (m_oneTimeFinish)
				continue;

			SetMeshConst(vec[left], vec[l], vec[r], true);

			std::unique_lock<mutex> lock(m_mtx);
			m_sleep = true;
			m_cv.wait(lock, [this] {return !m_sleep; });

			SetMeshConst(vec[left], vec[l], vec[r], false);
		}
			
		if (l >= r) break;

		SwapMeshData(vec[l++], vec[r--]);
		if (m_destroy)
			break;

		if (m_oneTimeFinish)
			continue;

		std::unique_lock<mutex> lock(m_mtx);
		m_sleep = true;
		m_cv.wait(lock, [this] {return !m_sleep; });
		
	} while (l <= r);

	SwapMeshData(vec[left], vec[r]);
	return r;
}
