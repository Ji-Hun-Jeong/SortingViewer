#pragma once
#include "Sort.h"
class HeapSort :
    public Sort
{
public:
	HeapSort(string strOfSortType);
	HeapSort(const HeapSort&) = delete;
	HeapSort(HeapSort&&) = delete;
	virtual ~HeapSort() = default;

public:
	virtual void DoSort(vector<shared_ptr<Mesh>>& vec) override;
	virtual void UpdateContinue(vector<shared_ptr<Mesh>>& vec, bool* permitSort) override;
	virtual void UpdateOneStep(vector<shared_ptr<Mesh>>& vec) override;
private:
	UINT m_frame = 0;
	bool m_pushHeap = false;
};

