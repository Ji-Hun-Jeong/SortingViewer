#include "pch.h"
#include "SelectSort.h"


SelectSort::SelectSort(string strOfSortType)
	: Sort(strOfSortType)
{
}

void SelectSort::DoSort(vector<shared_ptr<Mesh>>& vec)
{
	shared_ptr<Mesh> nullMesh(nullptr);
	for (int i = 0; i < vec.size() - 1; ++i)
	{
		int minIdx = i;
		SetMeshConst(vec[i], m_nullPtr, nullMesh, true);
		for (int j = i + 1; j < vec.size(); ++j)
		{
			if (vec[j]->GetScale().y < vec[minIdx]->GetScale().y)
			{
				minIdx = j;
			}
			

			if (m_destroy)
				break;

			if (m_oneTimeFinish)
				continue;

			OneFrameRenderingPreparation(m_nullPtr, vec[j], vec[minIdx]);
		}

		SwapMeshData(vec[i], vec[minIdx]);
		SetMeshConst(vec[i], m_nullPtr, nullMesh, false);
		vec[i]->GetMeshConst().findPos = true;
	}
	UnlockFindPos(vec);
}
