#pragma once
#include "Sort.h"
class MergeSort
	: public Sort
{
public:
	MergeSort(string strOfSortType);
	MergeSort(const MergeSort&) = delete;
	MergeSort(MergeSort&&) = delete;
	virtual ~MergeSort() = default;

public:
	virtual void DoSort(vector<shared_ptr<Mesh>>& vec) override;

private:
	void Merge(vector<shared_ptr<Mesh>>& vec, int left, int right);
	void Merge_Sort(vector<shared_ptr<Mesh>>& vec, int left, int mid, int right);
	static vector<float> m_vecTemp;

};

