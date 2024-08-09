#pragma once
#include "Sort.h"
class HeapSort :
    public Sort
{
public:
	virtual ~HeapSort() {}

public:
	virtual void StartSort(vector<shared_ptr<Mesh>>& vec) override;
private:
	static bool LessValue(float f1, float f2);
};

