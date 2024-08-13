#pragma once
#include "Sort.h"
class InsertSort :
    public Sort
{
public:
    InsertSort(string strOfSortType);
    InsertSort(const InsertSort&) = delete;
    InsertSort(InsertSort&&) = delete;
    virtual ~InsertSort() = default;

public:
    virtual void DoSort(vector<shared_ptr<Mesh>>& vec) override;

};

