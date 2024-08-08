#include "pch.h"
#include "Sorter.h"
#include "Mesh.h"
#include "GeometryGenerator.h"
#include "KeyMgr.h"
#include "SelectSort.h"

void Sorter::Init(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	GenerateRandomElements(device, context);
}

void Sorter::GenerateRandomElements(ComPtr<ID3D11Device>& device
	, ComPtr<ID3D11DeviceContext>& context)
{
	m_vecMeshes.clear();
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> randomSize(30, 60);
	uniform_real_distribution<float> randomHeight(0.1f, 2.0f);
	m_vecMeshes.resize(randomSize(gen));

	MeshData box = GeometryGenerator::MakeBox();
	float xzScale = 0.1f;
	for (int i = 0; i < m_vecMeshes.size(); ++i)
	{
		auto& mesh = m_vecMeshes[i];
		mesh = make_shared<Mesh>();
		mesh->GetScale() = Vector3(xzScale, randomHeight(gen), xzScale);
		mesh->GetTrans() = Vector3(i * xzScale * 2.1f , mesh->GetScale().y, 0.0f);
		mesh->Init(device, context, box);
		if (m_maxHeight < mesh->GetScale().y)
			m_maxHeight = mesh->GetTrans().y + 2.0f;
	}
}

void Sorter::Update(ComPtr<ID3D11DeviceContext>& context, float dt)
{
	if(KEYCHECK(B1, TAP))
	{
		m_sorter = make_shared<SelectSort>();
		m_sorter->StartSort(m_vecMeshes);
	}
	for (auto& mesh : m_vecMeshes)
		mesh->Update(context, dt);
}

void Sorter::Render(ComPtr<ID3D11DeviceContext>& context)
{
	for (auto& mesh : m_vecMeshes)
		mesh->Render(context);
}
