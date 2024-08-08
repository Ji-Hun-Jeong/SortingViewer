#pragma once
#include "Mesh.h"
class Sort
{
public:
	virtual void StartSort(vector<shared_ptr<Mesh>>& vec) = 0;
	void Update(vector<shared_ptr<Mesh>>& vec, bool& permitSortUpdate);
	
	bool Done() { return m_sortDone; }
	void WaitToSort() { m_sortDone = false; }

	bool DoingSort() { return m_doingSort; }

	bool IsSleep() { return m_sleep; }

	void WakeUp() 
	{
		while (!m_sleep);
		m_sleep = false;
		m_cv.notify_one();
		while (m_sleep == false && m_sortDone == false);
	}
	void Destroy()
	{
		if (m_sortThread.joinable())
		{
			m_destroy = true;
			WakeUp();
			m_sortThread.join();

			m_sortDone = false;
			m_doingSort = false;
			m_sleep = false;
			m_destroy = false;
		}
	}
	virtual ~Sort()
	{
		Destroy();
	}
protected:
	bool m_destroy = false;
	bool m_sortDone = false;
	bool m_doingSort = false;
	bool m_sleep = false;

	thread m_sortThread;
	mutex m_mtx;
	condition_variable m_cv;
};

