#pragma once
#include "ElementGenerator.h"
#include "BoxElementGenerator.h"
class RandomElementGenerator 
	: public BoxElementGenerator
{
public:
	RandomElementGenerator() = default;
	RandomElementGenerator(const RandomElementGenerator&) = delete;
	RandomElementGenerator(RandomElementGenerator&&) = delete;
	virtual ~RandomElementGenerator() = default;

public:
	void GenerateElement(ComPtr<ID3D11Device>& device
		, ComPtr<ID3D11DeviceContext>& context, vector<shared_ptr<Mesh>>& vec
		, int numOfElement) override;
	void GlobalConstSetData(GlobalConst& globalConst) override;
};

