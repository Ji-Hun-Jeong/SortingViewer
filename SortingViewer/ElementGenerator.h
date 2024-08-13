#pragma once
class Mesh;
class IElementGenerator
{
public:
	IElementGenerator() = default;
	IElementGenerator(const IElementGenerator&) = delete;
	IElementGenerator(IElementGenerator&&) = delete;
	virtual ~IElementGenerator() = default;

public:
	virtual void GenerateElement(ComPtr<ID3D11Device>& device
		, ComPtr<ID3D11DeviceContext>& context, vector<shared_ptr<Mesh>>& vec
		, int numOfElement) = 0;	
	virtual void GlobalConstSetData(GlobalConst& globalConst) = 0;
};

