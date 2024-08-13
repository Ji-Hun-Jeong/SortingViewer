#include "pch.h"
#include "ContinueSorter.h"
#include "Sort.h"

void ContinueSorter::OperateSortAlgorithm(float dt)
{
	static float time = 0.0f;
	time += dt;

	if (m_permitToSort && time > m_animateSec)
	{
		m_arrSortAlgorithm[(UINT)m_curSortType]->UpdateContinue(m_vecMeshes, &m_permitToSort);
		time = 0.0f;
	}
}

void ContinueSorter::ChooseSortAlgorithm(SORT_TYPE sortType)
{
	m_curPressSortType = sortType;
	if (m_curPressSortType == m_curSortType)	// 그대로 진행
	{
		if (m_arrSortAlgorithm[(UINT)m_curSortType]->DoingSort())	// 정렬중이면
		{
			m_oneTimeFinishSort = true;		// 바로 완료
			m_permitToSort = false;
		}
		else
			m_permitToSort = true;
	}
	else								// 다음부터는 다른 알고리즘 사용할거야
	{
		// 만약 현재 알고리즘으로 정렬중이 아닐때만 바꿔
		if (m_arrSortAlgorithm[(UINT)m_curSortType]->DoingSort() == false)
		{
			m_changeSortType = true;
			m_permitToSort = true;
		}
	}
}

void ContinueSorter::UpdateGUI()
{
	Sorter::UpdateGUI();
	ImGui::SliderFloat("Animate Second", &m_animateSec, 0.0f, 1.0f);
	ImGui::Text("Sort Time : %f", m_arrSortAlgorithm[(UINT)m_curSortType]->GetTimeTakenToSort());
}
