#include "pch.h"
#include "MergeSort.h"

vector<float> MergeSort::m_vecTemp;
MergeSort::MergeSort(string strOfSortType)
	: Sort(strOfSortType)
{
}

void MergeSort::DoSort(vector<shared_ptr<Mesh>>& vec)
{
	m_vecTemp.resize(vec.size());
	Merge(vec, 0, vec.size() - 1);
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

		int idxL = l;
		int idxM = m;

		if (l >= mid)
			idxL = mid - 1;
		if (m > right)
			idxM = right;

		OneFrameRenderingPreparation(vec[i], vec[idxL], vec[idxM]);
		vec[i]->GetMeshConst().findPos = true;
	}
	UnlockFindPos(vec);
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


