#pragma once
#include "Sort.h"
class MergeSort
	: public Sort
{
public:
	virtual ~MergeSort() {}
public:
	virtual void StartSort(vector<shared_ptr<Mesh>>& vec) override;
};

