#pragma once
#include "Sort.h"
class SelectSort
	: public Sort
{
public:
	SelectSort(string strOfSortType);
	SelectSort(const SelectSort&) = delete;
	SelectSort(SelectSort&&) = delete;
	virtual ~SelectSort() = default;
public:
	virtual void DoSort(vector<shared_ptr<Mesh>>& vec) override;
	
};

