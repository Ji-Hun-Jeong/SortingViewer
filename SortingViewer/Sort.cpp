#include "pch.h"
#include "Sort.h"

void Sort::Update(vector<shared_ptr<Mesh>>& vec, bool& permitSort)
{
	if (m_sortDone)
	{
		m_sortThread.join();
		m_sortDone = false;
		permitSort = false;
	}
	else if (!m_doingSort)
	{
		m_sortThread =
			thread(&Sort::StartSort, this, std::ref(vec));
	}
	else if (m_doingSort)
	{
		WakeUp();
	}
}

void Sort::OneTimeFinish()
{
	m_oneTimeFinish = true;
	WakeUp();
	m_sortThread.join();

	m_oneTimeFinish = false;
	m_sortDone = false;
	m_doingSort = false;
	m_sleep = false;
	m_destroy = false;
}

void Sort::WakeUp()
{
	while (!m_sleep);
	m_sleep = false;
	m_cv.notify_all();
	while (m_sleep == false && m_sortDone == false);
}

void Sort::Destroy()
{
	if (m_sortThread.joinable())
	{
		m_destroy = true;
		WakeUp();
		m_sortThread.join();

		m_oneTimeFinish = false;
		m_sortDone = false;
		m_doingSort = false;
		m_sleep = false;
		m_destroy = false;
	}
}

void Sort::CopyHeight(shared_ptr<Mesh>& mesh, float height)
{
	mesh->GetScale().y = height;
	mesh->GetTrans().y = mesh->GetScale().y;
}

void Sort::SwapMeshData(shared_ptr<Mesh>& mesh1, shared_ptr<Mesh>& mesh2)
{
	const float tempY = mesh1->GetTrans().y;
	mesh1->GetTrans().y = mesh2->GetTrans().y;
	mesh2->GetTrans().y = tempY;

	const float tempHeight = mesh1->GetScale().y;
	mesh1->GetScale().y = mesh2->GetScale().y;
	mesh2->GetScale().y = tempHeight;
}

void Sort::Print(vector<shared_ptr<Mesh>>& vec)
{
	static int cnt = 0;
	cout << cnt++ << endl;
	for (int i = 0; i < vec.size(); ++i)
	{
		cout << i << " Pos, X : " << vec[i]->GetTrans().x << " Y : " <<
			vec[i]->GetTrans().y << " Z : " << vec[i]->GetTrans().z << endl;
	}
}