#include "pch.h"
#include "Sort.h"

Sort::Sort(string strOfSortType)
	: m_startSort(false)
	, m_oneTimeFinish(false)
	, m_destroy(false)
	, m_sortDone(false)
	, m_doingSort(false)
	, m_sleep(false)
	, m_strOfSortType(std::move(strOfSortType))
{
}

Sort::~Sort()
{
	Destroy();
}

void Sort::StartSort(vector<shared_ptr<Mesh>>& vec)
{
	m_doingSort = true;
	float startTime = ImGui::GetTime();
	DoSort(vec);
	float endTime = ImGui::GetTime();
	m_timeTakenToSort = endTime - startTime;
	m_sleep = true;
	m_sortDone = true;
	m_doingSort = false;
}

void Sort::UpdateContinue(vector<shared_ptr<Mesh>>& vec, bool* permitSort)
{
	if (m_sortDone)
	{
		m_sortThread.join();
		m_sortDone = false;
		*permitSort = false;
	}

	else if (!m_doingSort)
		m_sortThread = thread(&Sort::StartSort, this, std::ref(vec));

	else if (m_doingSort)
		WakeUp();
}

void Sort::UpdateOneStep(vector<shared_ptr<Mesh>>& vec)
{
	if (m_sortDone)
	{
		m_sortThread.join();
		m_sortDone = false;
	}

	else if (!m_doingSort)
		m_sortThread = thread(&Sort::StartSort, this, std::ref(vec));

	else if (m_doingSort)
		WakeUp();
}

void Sort::OneFrameRenderingPreparation(shared_ptr<Mesh>& stdMesh
	, shared_ptr<Mesh>& cmpMesh1, shared_ptr<Mesh>& cmpMesh2)
{
	SetMeshConst(stdMesh, cmpMesh1, cmpMesh2, true);

	std::unique_lock<mutex> lock(m_mtx);
	m_sleep = true;
	m_cv.wait(lock, [this] {return !m_sleep; });

	SetMeshConst(stdMesh, cmpMesh1, cmpMesh2, false);
}

void Sort::OneTimeFinish()
{
	m_oneTimeFinish = true;
	WakeUp();
	if (m_sortThread.joinable())
		m_sortThread.join();

	this->ClearState();
}

void Sort::WakeUp()
{
	while (!m_sleep);	// 아직 스레드가 멈추지 않았을 수 있기 때문에
	m_sleep = false;
	m_cv.notify_all();	// 다시 스레드를 깨운다.

	// 스레드가 다시 깨거나 정렬이 끝나면 
	// while (m_sleep == false && m_sortDone == false);	
}

void Sort::Destroy()
{
	if (m_sortThread.joinable())
	{
		m_destroy = true;
		WakeUp();
		m_sortThread.join();
		ClearState();
	}
}

void Sort::ClearState()
{
	m_oneTimeFinish = false;
	m_sortDone = false;
	m_doingSort = false;
	m_sleep = false;
	m_destroy = false;
}

void Sort::UnlockFindPos(vector<shared_ptr<Mesh>>& vec)
{
	for (auto& mesh : vec)
		mesh->GetMeshConst().findPos = false;
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

void Sort::SetMeshConst(shared_ptr<Mesh>& stdMesh, shared_ptr<Mesh>& cmpMesh1
	, shared_ptr<Mesh>& cmpMesh2, bool onOff)
{
	if (stdMesh)
		stdMesh->GetMeshConst().stdElement = onOff;
	if(cmpMesh1)
		cmpMesh1->GetMeshConst().cmpElement1 = onOff;
	if (cmpMesh2)
		cmpMesh2->GetMeshConst().cmpElement2 = onOff;
}
