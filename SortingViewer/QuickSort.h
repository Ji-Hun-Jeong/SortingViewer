#pragma once
#include "Sort.h"
class QuickSort :
    public Sort
{
public:
    QuickSort(string strOfSortType);
    QuickSort(const QuickSort&) = delete;
    QuickSort(QuickSort&&) = delete;
    virtual ~QuickSort() = default;
public:
    virtual void DoSort(vector<shared_ptr<Mesh>>& vec) override;

private:

    void Quick_Sort(vector<shared_ptr<Mesh>>& vec, int left, int right);
    int Partition(vector<shared_ptr<Mesh>>& vec, int left, int right);
};

