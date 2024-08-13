#include "pch.h"
#include "WorstCaseElementGenerator.h"
#include "Mesh.h"

void WorstCaseElementGenerator::GenerateElement(ComPtr<ID3D11Device>& device
	, ComPtr<ID3D11DeviceContext>& context, vector<shared_ptr<Mesh>>& vec, int numOfElement)
{
	if (numOfElement > 1000)
		numOfElement = 1000;
	vec.clear();
	vec.resize(numOfElement);
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> randomMetallic(0.001f, 0.99f);
	uniform_real_distribution<float> randomRoughness(0.001f, 0.99f);
	float xzScale = 0.01f;
	if (numOfElement >= 1000)
		xzScale = 0.005f;
	else if (numOfElement >= 10000)
		xzScale = 0.001f;

	m_sumOfBoxWidth = xzScale * vec.size() * 2.0f;
	m_maxOfBoxHeight = numOfElement * xzScale;

	for (size_t i = 0; i < vec.size(); ++i)
	{
		auto& mesh = vec[i];
		mesh = make_shared<Mesh>();
		mesh->GetScale() = Vector3(xzScale, (numOfElement - i) * xzScale, xzScale);
		mesh->GetTrans() = Vector3(i * xzScale * 2.3f, mesh->GetScale().y, 0.0f);
		mesh->GetMeshConst().metallic = randomMetallic(gen);
		mesh->GetMeshConst().roughness = randomRoughness(gen);
		mesh->Init(device, context, m_box);
	}
}

void WorstCaseElementGenerator::GlobalConstSetData(GlobalConst& globalConst)
{
	BoxElementGenerator::GlobalConstSetData(globalConst);
}
