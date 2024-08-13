#pragma once
#include "Sorter.h"
class ContinueSorter :
    public Sorter
{
public:
    ContinueSorter() = default;
    ContinueSorter(const ContinueSorter&) = delete;
    ContinueSorter(ContinueSorter&&) = delete;
    virtual ~ContinueSorter() = default;

public:
    void OperateSortAlgorithm(float dt) override;
    void ChooseSortAlgorithm(SORT_TYPE sortType) override;
    void UpdateGUI() override;
    void SetAnimateSec(float animateSec) { m_animateSec = animateSec; }

private:
    float m_animateSec = 0.0f;
    bool m_permitToSort = false;

};

