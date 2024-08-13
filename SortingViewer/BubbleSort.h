#pragma once
#include "Sort.h"
class BubbleSort :
    public Sort
{
public:
    BubbleSort(string strOfSortType);
    BubbleSort(const BubbleSort&) = delete;
    BubbleSort(BubbleSort&&) = delete;
    virtual ~BubbleSort() = default;

public:
    virtual void DoSort(vector<shared_ptr<Mesh>>& vec) override;

};

