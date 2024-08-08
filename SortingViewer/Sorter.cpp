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

void Sorter::Init(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	m_arrSortAlgorithm[(UINT)SORT_TYPE::SELECT] = make_shared<SelectSort>();
	m_arrSortAlgorithm[(UINT)SORT_TYPE::BUBBLE] = make_shared<BubbleSort>();
	m_arrSortAlgorithm[(UINT)SORT_TYPE::INSERT] = make_shared<InsertSort>();
	m_arrSortAlgorithm[(UINT)SORT_TYPE::MERGE] = make_shared<MergeSort>();
	m_arrSortAlgorithm[(UINT)SORT_TYPE::QUICK] = make_shared<QuickSort>();
	//m_arrSortAlgorithm[(UINT)SORT_TYPE::HEAP] = make_shared<HeapSort>();
	GenerateRandomElements(device, context);
}

void Sorter::GenerateRandomElements(ComPtr<ID3D11Device>& device
	, ComPtr<ID3D11DeviceContext>& context)
{
	if (m_arrSortAlgorithm[(UINT)m_sortType]->DoingSort())
		return;

	m_vecMeshes.clear();
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> randomSize(30, 60);
	uniform_real_distribution<float> randomHeight(0.01f, 2.0f);
	m_vecMeshes.resize(randomSize(gen));

	MeshData box = GeometryGenerator::MakeBox();
	float xzScale = 0.01f;
	for (int i = 0; i < m_vecMeshes.size(); ++i)
	{
		auto& mesh = m_vecMeshes[i];
		mesh = make_shared<Mesh>();
		mesh->GetScale() = Vector3(xzScale, randomHeight(gen), xzScale);
		mesh->GetTrans() = Vector3(i * xzScale * 2.3f, mesh->GetScale().y, 0.0f);
		mesh->Init(device, context, box);
		if (m_maxHeight < mesh->GetScale().y)
			m_maxHeight = mesh->GetTrans().y;
	}
	m_maxHeight += 2.0f;
}

void Sorter::ChooseSortAlgorithm(SORT_TYPE sortType)
{
	m_nextSortType = sortType;
	if (m_nextSortType == m_sortType)	// 그대로 진행
	{
		if (m_arrSortAlgorithm[(UINT)m_sortType]->DoingSort())	// 정렬중이면
			m_oneTimeFinishSort = true;		// 바로 완료
		m_permitSort = true;
	}
	else								// 다음과 달라
	{
		if (!m_arrSortAlgorithm[(UINT)m_sortType]->DoingSort())
			m_changeSortType = true;
	}
}

void Sorter::Update(ComPtr<ID3D11DeviceContext>& context, float dt)
{
	// 1 -> 2 -> 2 고치기	

	if (m_oneTimeFinishSort)
	{
		m_arrSortAlgorithm[(UINT)m_sortType]->OneTimeFinish();
		m_permitSort = false;
		m_oneTimeFinishSort = false;
	}

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

	if (m_permitSort)
		m_arrSortAlgorithm[(UINT)m_sortType]->Update(m_vecMeshes, m_permitSort);

	if (m_changeSortType)
	{
		m_sortType = m_nextSortType;
		m_changeSortType = false;
		m_permitSort = true;
	}
	else
		m_nextSortType = m_sortType;

	for (auto& mesh : m_vecMeshes)
		mesh->Update(context, dt);

}

void Sorter::Render(ComPtr<ID3D11DeviceContext>& context)
{
	for (auto& mesh : m_vecMeshes)
		mesh->Render(context);
}
