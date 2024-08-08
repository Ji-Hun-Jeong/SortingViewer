#include "pch.h"
#include "MergeSort.h"

vector<float> MergeSort::m_vecTemp;
void MergeSort::StartSort(vector<shared_ptr<Mesh>>& vec)
{
	m_doingSort = true;
	m_vecTemp.resize(vec.size());

	Merge(vec, 0, vec.size() - 1);

	m_sortDone = true;
	m_doingSort = false;
}

void MergeSort::Merge_Sort(vector<shared_ptr<Mesh>>& vec, int left, int mid, int right)
{
	int l = left;
	int m = mid;
	int r = right;

	for (int i = 0; i <= right; ++i)
		m_vecTemp[i] = vec[i]->GetScale().y;

	int idx = 0;
	for (int i = left; i <= right; ++i)
	{
		if (l >= mid)
			idx = m++;
		else if (m > right)
			idx = l++;
		else if (m_vecTemp[l] < m_vecTemp[m])
			idx = l++;
		else
			idx = m++;

		CopyHeight(vec[i], m_vecTemp[idx]);

		if (m_destroy)
			break;

		if (m_oneTimeFinish)
			continue;

		std::unique_lock<mutex> lock(m_mtx);
		m_sleep = true;
		m_cv.wait(lock, [this] {return !m_sleep; });
	}
}

void MergeSort::CopyHeight(shared_ptr<Mesh>& mesh, float height)
{
	mesh->GetScale().y = height;
	mesh->GetTrans().y = mesh->GetScale().y;
}

void MergeSort::Merge(vector<shared_ptr<Mesh>>& vec, int left, int right)
{
	if (left >= right)
		return;
	int mid = (left + right) / 2;
	Merge(vec, left, mid);
	Merge(vec, mid + 1, right);
	Merge_Sort(vec, left, mid + 1, right);
}


