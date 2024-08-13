#pragma once
#include "Sorter.h"
class OneStepSorter :
    public Sorter
{
public:
    OneStepSorter() = default;
    OneStepSorter(const OneStepSorter&) = delete;
    OneStepSorter(OneStepSorter&&) = delete;
    virtual ~OneStepSorter() = default;

public:
    void OperateSortAlgorithm(float dt) override;
    void ChooseSortAlgorithm(SORT_TYPE sortType) override;
    void UpdateGUI() override;

};

