#pragma once
class Mesh;
class Sort;
class Sorter
{
public:
	void Init(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
	void GenerateRandomElements(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);

	void Update(ComPtr<ID3D11DeviceContext>& context, float dt);
	void Render(ComPtr<ID3D11DeviceContext>& context);

	float m_maxHeight = 0.0f;
private:
	bool m_permitSortUpdate = false;
	vector<shared_ptr<Mesh>> m_vecMeshes;
	shared_ptr<Sort> m_sortAlgorithm;
};

