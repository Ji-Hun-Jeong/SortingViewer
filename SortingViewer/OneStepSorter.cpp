#include "pch.h"
#include "OneStepSorter.h"
#include "KeyMgr.h"
#include "Sort.h"

void OneStepSorter::OperateSortAlgorithm(float dt)
{
	ImVec2 guiPosMin = ImGui::GetWindowPos();
	ImVec2 guiSize = ImGui::GetWindowSize();
	ImVec2 guiPosmax = ImVec2(guiPosMin.x + guiSize.x, guiPosMin.y + guiSize.y);
	bool hoverd = ImGui::IsMouseHoveringRect(guiPosMin, guiPosmax);

	if (KEYCHECK(LBUTTON, TAP) && !hoverd)
		m_arrSortAlgorithm[(UINT)m_curSortType]->UpdateOneStep(m_vecMeshes);
}

void OneStepSorter::ChooseSortAlgorithm(SORT_TYPE sortType)
{
	m_curPressSortType = sortType;
	if (m_curPressSortType == m_curSortType)	// 그대로 진행
	{
		if (m_arrSortAlgorithm[(UINT)m_curSortType]->DoingSort())	// 정렬중이면
			m_oneTimeFinishSort = true;		// 바로 완료
	}
	else								// 다음부터는 다른 알고리즘 사용할거야
	{
		// 만약 현재 알고리즘으로 정렬중이 아닐때만 바꿔
		if (!m_arrSortAlgorithm[(UINT)m_curSortType]->DoingSort())
			m_changeSortType = true;
	}
}

void OneStepSorter::UpdateGUI()
{
	Sorter::UpdateGUI();
	string sortType = m_arrSortAlgorithm[(UINT)m_curSortType]->GetSortTypeByString();
	string sentenceToPrint = "Current SortType Is " + sortType;
	ImGui::Text(sentenceToPrint.c_str());
}
