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
	if (m_curPressSortType == m_curSortType)	// �״�� ����
	{
		if (m_arrSortAlgorithm[(UINT)m_curSortType]->DoingSort())	// �������̸�
		{
			m_oneTimeFinishSort = true;		// �ٷ� �Ϸ�
			m_permitToSort = false;
		}
		else
			m_permitToSort = true;
	}
	else								// �������ʹ� �ٸ� �˰��� ����Ұž�
	{
		// ���� ���� �˰������� �������� �ƴҶ��� �ٲ�
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
