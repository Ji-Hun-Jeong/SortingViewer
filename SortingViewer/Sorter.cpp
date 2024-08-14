#include "pch.h"
#include "Sorter.h"
#include "Mesh.h"
#include "GeometryGenerator.h"
#include "KeyMgr.h"
#include "SelectSort.h"
#include "MergeSort.h"
#include "BubbleSort.h"
#include "InsertSort.h"
#include "QuickSort.h"
#include "HeapSort.h"
#include "RandomElementGenerator.h"
#include "WorstCaseElementGenerator.h"

int Sorter::m_numOfElement = 10;
int Sorter::m_elementGenerateMode = 0;

vector<shared_ptr<Mesh>> Sorter::m_vecMeshes;
std::array<unique_ptr<IElementGenerator>, 2> Sorter::m_arrElementGenerator =
{ make_unique<RandomElementGenerator>(), make_unique<WorstCaseElementGenerator>() };

Sorter::~Sorter()
{
	for (auto& thread : m_vecMeshUpdateThread)
	{
		if (thread.joinable())
			thread.join();
	}
	for (auto& sort : m_arrSortAlgorithm)
	{
		sort.reset();
	}
}

void Sorter::Init(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	m_arrSortAlgorithm[(UINT)SORT_TYPE::SELECT] = make_shared<SelectSort>("Select Sort");
	m_arrSortAlgorithm[(UINT)SORT_TYPE::BUBBLE] = make_shared<BubbleSort>("Bubble Sort");
	m_arrSortAlgorithm[(UINT)SORT_TYPE::INSERT] = make_shared<InsertSort>("InsertSort");
	m_arrSortAlgorithm[(UINT)SORT_TYPE::MERGE] = make_shared<MergeSort>("MergeSort");
	m_arrSortAlgorithm[(UINT)SORT_TYPE::QUICK] = make_shared<QuickSort>("QuickSort");
	m_arrSortAlgorithm[(UINT)SORT_TYPE::HEAP] = make_shared<HeapSort>("HeapSort");

	m_vecMeshUpdateThread.resize(4);
	this->GenerateElement(device, context);
}

void Sorter::GenerateElement(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	if (m_arrSortAlgorithm[(UINT)m_curSortType]->DoingSort())
		return;

	m_arrElementGenerator[m_elementGenerateMode]->GenerateElement(device, context
		, m_vecMeshes, m_numOfElement);
}

void Sorter::Update(ComPtr<ID3D11DeviceContext>& context, const GlobalConst& globalConst
	, float dt)
{
	if (KEYCHECK(B1, TAP))
		ChooseSortAlgorithm(SORT_TYPE::SELECT);
	else if (KEYCHECK(B2, TAP))
		ChooseSortAlgorithm(SORT_TYPE::BUBBLE);
	else if (KEYCHECK(B3, TAP))
		ChooseSortAlgorithm(SORT_TYPE::INSERT);
	else if (KEYCHECK(B4, TAP))
		ChooseSortAlgorithm(SORT_TYPE::MERGE);
	else if (KEYCHECK(B5, TAP))
		ChooseSortAlgorithm(SORT_TYPE::QUICK);
	else if (KEYCHECK(B6, TAP))
		ChooseSortAlgorithm(SORT_TYPE::HEAP);
	
	if (m_changeSortType)
	{
		m_curSortType = m_curPressSortType;
		m_changeSortType = false;
	}

	if (m_oneTimeFinishSort)
	{
		m_arrSortAlgorithm[(UINT)m_curSortType]->OneTimeFinish();
		m_oneTimeFinishSort = false;
	}
	else
		OperateSortAlgorithm(dt);

	// 만약 현재 정렬스레드가 아직 멈추지 않았을 경우(mesh들에 접근해야하기 때문에)
	while (m_arrSortAlgorithm[(UINT)m_curSortType]->DoingSort() &&
		m_arrSortAlgorithm[(UINT)m_curSortType]->IsSleep() == false);

	for (int i = 0; i < m_vecMeshUpdateThread.size(); ++i)
	{
		int startIdx = m_vecMeshes.size() * i / m_vecMeshUpdateThread.size();
		int finishIdx = m_vecMeshes.size() * (i + 1) / m_vecMeshUpdateThread.size();
		m_vecMeshUpdateThread[i] = thread(&Sorter::DivisionThreadUpdate, this, globalConst
			, dt, startIdx, finishIdx);
	}
	
	for (int i = 0; i < m_vecMeshUpdateThread.size(); ++i)
		m_vecMeshUpdateThread[i].join();
}

void Sorter::UpdateGlobalConst(GlobalConst& globalConst)
{
	m_arrElementGenerator[m_elementGenerateMode]->GlobalConstSetData(globalConst);
}

void Sorter::UpdateGUI()
{
	ImGui::RadioButton("Generate Random Case Elements", &m_elementGenerateMode, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Generate Worst Case Elements", &m_elementGenerateMode, 1);

	ImGui::InputInt("Num Of Element", &m_numOfElement);
	ImGui::Text("Max Element : 1000");
}

void Sorter::FinalUpdate(ComPtr<ID3D11DeviceContext>& context, float dt)
{
	for (auto& mesh : m_vecMeshes)
		mesh->FinalUpdate(context, dt);
}

void Sorter::DivisionThreadUpdate(const GlobalConst& globalConst, float dt, UINT startIdx
	, UINT finishIdx)
{
	for (int i = startIdx; i < finishIdx; ++i)
		m_vecMeshes[i]->Update(globalConst, dt);
}

void Sorter::Render(ComPtr<ID3D11DeviceContext>& context)
{
	for (auto& mesh : m_vecMeshes)
		mesh->Render(context);
}

