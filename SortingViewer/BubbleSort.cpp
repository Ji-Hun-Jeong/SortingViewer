#include "pch.h"
#include "BubbleSort.h"

BubbleSort::BubbleSort(string strOfSortType)
	: Sort(strOfSortType)
{
}

void BubbleSort::DoSort(vector<shared_ptr<Mesh>>& vec)
{
	int n = vec.size();
	shared_ptr<Mesh> nullMesh(nullptr);
	for (int i = n - 1; i > 0; i--)
	{
		SetMeshConst(vec[i], nullMesh, m_nullPtr, true);
		for (int j = 0; j < i; j++)
		{
			if (vec[j]->GetScale().y > vec[j + 1]->GetScale().y)
				SwapMeshData(vec[j], vec[j + 1]);

			if (m_destroy)
				break;

			if (m_oneTimeFinish)
				continue;

			OneFrameRenderingPreparation(m_nullPtr, vec[j], vec[j + 1]);
		}
		SetMeshConst(vec[i], nullMesh, m_nullPtr, false);
		vec[i]->GetMeshConst().findPos = true;
	}
	UnlockFindPos(vec);
}
