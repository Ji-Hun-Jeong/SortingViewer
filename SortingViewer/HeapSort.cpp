#include "pch.h"
#include "HeapSort.h"
#include <queue>
#include "Sorter.h"

using std::priority_queue;

HeapSort::HeapSort(string strOfSortType)
	: Sort(strOfSortType)
{
}

void HeapSort::DoSort(vector<shared_ptr<Mesh>>& vec)
{
	shared_ptr<Mesh> nullMesh;
	priority_queue<float, vector<float>, std::greater<float>> q;
	m_pushHeap = true;
	for (auto& mesh : vec)
	{
		q.push(mesh->GetScale().y);

		if (m_destroy)
			break;

		if (m_oneTimeFinish)
			continue;

		OneFrameRenderingPreparation(nullMesh, m_nullPtr, mesh);
	}
	m_pushHeap = false;

	int i = 0;
	while (!q.empty())
	{
		CopyHeight(vec[i], q.top());
		q.pop();
		int idx = i++;

		if (m_destroy)
			break;

		if (m_oneTimeFinish)
			continue;
		OneFrameRenderingPreparation(vec[idx], m_nullPtr, nullMesh);

		vec[idx]->GetMeshConst().findPos = true;
	}
	UnlockFindPos(vec);
}

void HeapSort::UpdateContinue(vector<shared_ptr<Mesh>>& vec, bool* permitSort)
{
	++m_frame;
	if (m_frame < 5 && m_pushHeap)
		return;
	m_frame = 0;
	Sort::UpdateContinue(vec, permitSort);
}

void HeapSort::UpdateOneStep(vector<shared_ptr<Mesh>>& vec)
{
	Sort::UpdateOneStep(vec);
}
