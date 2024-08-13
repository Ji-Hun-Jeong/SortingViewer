#include "pch.h"
#include "InsertSort.h"

InsertSort::InsertSort(string strOfSortType)
	: Sort(strOfSortType)
{
}

void InsertSort::DoSort(vector<shared_ptr<Mesh>>& vec)
{
	int n = vec.size();

	for (int i = 0; i < n; i++)
	{
		int j = i - 1;
		vec[i]->GetMeshConst().findPos = true;
		for (; j >= 0; j--)
		{
			if (m_destroy)
				break;

			if (!m_oneTimeFinish)
				OneFrameRenderingPreparation(vec[j], vec[j + 1], m_nullPtr);
			

			if (vec[j + 1]->GetScale().y > vec[j]->GetScale().y)
				break;

			SwapMeshData(vec[j + 1], vec[j]);

			if (m_destroy)
				break;

			if (m_oneTimeFinish)
				continue;

			OneFrameRenderingPreparation(vec[j], vec[j + 1], m_nullPtr);
		}
	}
	UnlockFindPos(vec);
}
