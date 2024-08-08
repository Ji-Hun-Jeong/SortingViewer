#pragma once
#include "Sort.h"
class MergeSort
	: public Sort
{
public:
	virtual ~MergeSort() {}

public:
	virtual void StartSort(vector<shared_ptr<Mesh>>& vec) override;

private:
	void CopyHeight(shared_ptr<Mesh>& mesh, float height);
	void Merge(vector<shared_ptr<Mesh>>& vec, int left, int right);
	void Merge_Sort(vector<shared_ptr<Mesh>>& vec, int left, int mid, int right);
	static vector<float> m_vecTemp;

};

