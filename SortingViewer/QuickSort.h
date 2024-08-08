#pragma once
#include "Sort.h"
class QuickSort :
    public Sort
{
public:
    virtual void StartSort(vector<shared_ptr<Mesh>>& vec) override;
    virtual ~QuickSort() {}

private:
    void Quick_Sort(vector<shared_ptr<Mesh>>& vec, int left, int right);
    int Partition(vector<shared_ptr<Mesh>>& vec, int left, int right);
};

