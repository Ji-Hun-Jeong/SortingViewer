#include "pch.h"
#include "Sort.h"

void Sort::Update(vector<shared_ptr<Mesh>>& vec, bool& permitSortUpdate)
{
	if (m_sortDone)
	{
		m_sortThread.join();
		m_sortDone = false;
		permitSortUpdate = false;
	}
	else if (!m_doingSort)
	{
		m_sortThread = thread(&Sort::StartSort, this
			, std::ref(vec));
	}
	else if (m_doingSort)
	{
		WakeUp();
	}
}
