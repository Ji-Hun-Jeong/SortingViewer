#pragma once
class Mesh;
class Sort;
#include "ElementGenerator.h"

class Sorter
{
public:
	Sorter() = default;
	Sorter(const Sorter&) = delete;
	Sorter(Sorter&&) = delete;
	virtual ~Sorter();

public:
	virtual void UpdateGUI();

	void Init(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
	void Update(ComPtr<ID3D11DeviceContext>& context, const GlobalConst& globalConst
		, float dt);
	void UpdateGlobalConst(GlobalConst& globalConst);
	void FinalUpdate(ComPtr<ID3D11DeviceContext>& context, float dt);
	void Render(ComPtr<ID3D11DeviceContext>& context);
	void GenerateElement(ComPtr<ID3D11Device>& device
		, ComPtr<ID3D11DeviceContext>& context);

protected:
	virtual void OperateSortAlgorithm(float dt) = 0;
	virtual void ChooseSortAlgorithm(SORT_TYPE sortType) = 0;

	void DivisionThreadUpdate(const GlobalConst& globalConst, float dt
		, UINT startIdx, UINT finishIdx);

protected:
	static int m_numOfElement;
	static int m_elementGenerateMode;
	static std::array<unique_ptr<IElementGenerator>, 2> m_arrElementGenerator;

	bool m_changeSortType = false;
	bool m_oneTimeFinishSort = false;

	static vector<shared_ptr<Mesh>> m_vecMeshes;
	shared_ptr<Sort> m_arrSortAlgorithm[(UINT)SORT_TYPE::END];
	SORT_TYPE m_curPressSortType = SORT_TYPE::SELECT;
	SORT_TYPE m_curSortType = SORT_TYPE::SELECT;

	vector<thread> m_vecMeshUpdateThread;
};

