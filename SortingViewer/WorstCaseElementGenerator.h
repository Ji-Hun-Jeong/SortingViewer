#pragma once
#include "ElementGenerator.h"
#include "BoxElementGenerator.h"
class WorstCaseElementGenerator 
	: public BoxElementGenerator
{
public:
	WorstCaseElementGenerator() = default;
	WorstCaseElementGenerator(const WorstCaseElementGenerator&) = delete;
	WorstCaseElementGenerator(WorstCaseElementGenerator&&) = delete;
	virtual ~WorstCaseElementGenerator() = default;

public:
	void GenerateElement(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context
		, vector<shared_ptr<Mesh>>& vec, int numOfElement) override;
	void GlobalConstSetData(GlobalConst& globalConst) override;
};

