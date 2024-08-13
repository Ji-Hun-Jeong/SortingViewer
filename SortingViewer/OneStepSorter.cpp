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
	if (m_curPressSortType == m_curSortType)	// �״�� ����
	{
		if (m_arrSortAlgorithm[(UINT)m_curSortType]->DoingSort())	// �������̸�
			m_oneTimeFinishSort = true;		// �ٷ� �Ϸ�
	}
	else								// �������ʹ� �ٸ� �˰��� ����Ұž�
	{
		// ���� ���� �˰������� �������� �ƴҶ��� �ٲ�
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
