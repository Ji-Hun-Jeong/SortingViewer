#include "pch.h"
#include "RandomElementGenerator.h"
#include "Mesh.h"

void RandomElementGenerator::GenerateElement(ComPtr<ID3D11Device>& device
	, ComPtr<ID3D11DeviceContext>& context, vector<shared_ptr<Mesh>>& vec, int numOfElement)
{
	if (numOfElement > 1000)
		numOfElement = 1000;
	vector<bool> arrChecking(numOfElement + 1, false);
	vec.clear();
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> randomHeight(1, numOfElement);
	uniform_real_distribution<float> randomMetallic(0.001f, 0.99f);
	uniform_real_distribution<float> randomRoughness(0.001f, 0.99f);
	vec.resize(numOfElement);

	float xzScale = 0.01f;
	if (numOfElement >= 1000)
		xzScale = 0.005f;
	else if (numOfElement >= 10000)
		xzScale = 0.001f;

	m_sumOfBoxWidth = xzScale * vec.size() * 2.3f;
	m_maxOfBoxHeight = numOfElement * xzScale;
	for (size_t i = 0; i < vec.size(); ++i)
	{
		auto& mesh = vec[i];
		mesh = make_shared<Mesh>();

		int height = 0;
		while (true)
		{
			height = randomHeight(gen);
			if (arrChecking[height] == false)
			{
				arrChecking[height] = true;
				break;
			}
		}

		mesh->GetScale() = Vector3(xzScale, height * xzScale, xzScale);
		mesh->GetTrans() = Vector3(i * xzScale * 2.3f, mesh->GetScale().y, 0.0f);
		mesh->GetMeshConst().metallic = randomMetallic(gen);
		mesh->GetMeshConst().roughness = randomRoughness(gen);
		mesh->Init(device, context, m_box);
	}
}

void RandomElementGenerator::GlobalConstSetData(GlobalConst& globalConst)
{
	BoxElementGenerator::GlobalConstSetData(globalConst);
}
