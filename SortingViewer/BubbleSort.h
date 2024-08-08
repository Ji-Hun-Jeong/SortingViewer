#pragma once
#include "Sort.h"
class BubbleSort :
    public Sort
{
public:
    virtual void StartSort(vector<shared_ptr<Mesh>>& vec) override;
    virtual ~BubbleSort() {}
};

