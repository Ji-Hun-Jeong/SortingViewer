#pragma once
#include "Sort.h"
class InsertSort :
    public Sort
{
public:
    virtual void StartSort(vector<shared_ptr<Mesh>>& vec) override;
    virtual ~InsertSort() {}
};

