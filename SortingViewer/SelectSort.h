#pragma once
#include "Sort.h"
class SelectSort
	: public Sort
{
public:
	virtual ~SelectSort() {}
public:
	virtual void StartSort(vector<shared_ptr<Mesh>>& vec) override;
};

