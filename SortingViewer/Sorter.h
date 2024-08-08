#pragma once
class Mesh;
class Sort;
class Sorter
{
public:
	void Init(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
	void GenerateRandomElements(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);

	void ChooseSortAlgorithm(SORT_TYPE sortType);
	void Update(ComPtr<ID3D11DeviceContext>& context, float dt);
	void Render(ComPtr<ID3D11DeviceContext>& context);

	float m_maxHeight = 0.0f;
private:
	bool m_permitSort = false;
	bool m_changeSortType = false;
	bool m_oneTimeFinishSort = false;
	vector<shared_ptr<Mesh>> m_vecMeshes;
	shared_ptr<Sort> m_arrSortAlgorithm[(UINT)SORT_TYPE::END];
	SORT_TYPE m_nextSortType = SORT_TYPE::SELECT;
	SORT_TYPE m_sortType = SORT_TYPE::SELECT;

};

