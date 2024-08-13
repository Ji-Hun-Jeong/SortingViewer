#include "pch.h"
#include "QuickSort.h"

QuickSort::QuickSort(string strOfSortType)
	: Sort(strOfSortType)
{
}

void QuickSort::DoSort(vector<shared_ptr<Mesh>>& vec)
{
	Quick_Sort(vec, 0, vec.size() - 1);
	for (auto& mesh : vec)
		mesh->GetMeshConst().partition = false;
}

void QuickSort::Quick_Sort(vector<shared_ptr<Mesh>>& vec, int left, int right)
{
	if (left >= right)
		return;
	int q = Partition(vec, left, right);
	vec[q]->GetMeshConst().partition = true;
	Quick_Sort(vec, left, q - 1);
	Quick_Sort(vec, q + 1, right);

}

int QuickSort::Partition(vector<shared_ptr<Mesh>>& vec, int left, int right)
{
	float pivot = vec[left]->GetScale().y;
	int l = left + 1;
	int r = right;
	if (!m_destroy && !m_oneTimeFinish)
		OneFrameRenderingPreparation(vec[left], vec[l], vec[r]);
	do
	{
		/*if (m_destroy)
			break;

		if (m_oneTimeFinish)
			continue;

		OneFrameRenderingPreparation(vec[left], vec[l], vec[r]);*/
		while (vec[l]->GetScale().y <= pivot && l < right)
		{
			++l;
			if (m_destroy)
				break;

			if (m_oneTimeFinish)
				continue;

			OneFrameRenderingPreparation(vec[left], vec[l], vec[r]);
		}
		while (vec[r]->GetScale().y > pivot && r > 0)
		{
			--r;
			if (m_destroy)
				break;

			if (m_oneTimeFinish)
				continue;

			OneFrameRenderingPreparation(vec[left], vec[l], vec[r]);
		}

		if (l >= r)	break;

		SwapMeshData(vec[l], vec[r]);

		if (m_destroy)
			break;

		if (m_oneTimeFinish)
			continue;

		OneFrameRenderingPreparation(vec[left], vec[l], vec[r]);
	} while (true);

	SwapMeshData(vec[left], vec[r]);
	if (!m_destroy && !m_oneTimeFinish)
		OneFrameRenderingPreparation(vec[left], vec[l], vec[r]);
	return r;
}
