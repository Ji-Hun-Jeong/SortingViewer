#include "pch.h"
#include "D3DUtils.h"

void D3DUtils::CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device>& device
	, const wstring& vsPrefix, ComPtr<ID3D11VertexShader>& vs
	, const vector<D3D11_INPUT_ELEMENT_DESC>& desc
	, ComPtr<ID3D11InputLayout>& inputLayout)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	wstring fileName = vsPrefix + L"VertexShader.hlsl";

	ComPtr<ID3DBlob> shaderBlob;
	ComPtr<ID3DBlob> errorBlob;

	CHECKRESULT(D3DCompileFromFile(fileName.c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0",
		flags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf()));

	CHECKRESULT(device->CreateVertexShader(shaderBlob->GetBufferPointer()
		, shaderBlob->GetBufferSize(), nullptr, vs.GetAddressOf()));

	CHECKRESULT(device->CreateInputLayout(desc.data(), UINT(desc.size()), shaderBlob->GetBufferPointer()
		, shaderBlob->GetBufferSize(), inputLayout.GetAddressOf()));
}

void D3DUtils::CreatePixelShader(ComPtr<ID3D11Device>& device, const wstring& psPrefix
	, ComPtr<ID3D11PixelShader>& ps)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	wstring fileName = psPrefix + L"PixelShader.hlsl";

	ComPtr<ID3DBlob> shaderBlob;
	ComPtr<ID3DBlob> errorBlob;

	CHECKRESULT(D3DCompileFromFile(fileName.c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0",
		flags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf()));

	CHECKRESULT(device->CreatePixelShader(shaderBlob->GetBufferPointer()
		, shaderBlob->GetBufferSize(), nullptr, ps.GetAddressOf()));
}

void D3DUtils::CreateConstBuffer(ComPtr<ID3D11Device>& device
	, UINT elementSize, ComPtr<ID3D11Buffer>& constBuffer)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = elementSize;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.StructureByteStride = elementSize;

	CHECKRESULT(device->CreateBuffer(&bufferDesc, nullptr, constBuffer.GetAddressOf()));
}

void D3DUtils::CreateStagingBuffer(ComPtr<ID3D11Device>& device, UINT elementSize
	, UINT numElement, void* initData, ComPtr<ID3D11Buffer>& stagingBuffer)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = elementSize * numElement;
	bufferDesc.Usage = D3D11_USAGE_STAGING;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	bufferDesc.StructureByteStride = elementSize;

	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = initData;

	CHECKRESULT(device->CreateBuffer(&bufferDesc, &sd, stagingBuffer.GetAddressOf()));
}

void D3DUtils::UpdateStagingBuffer(ComPtr<ID3D11DeviceContext>& context, UINT elementSize
	, UINT numElement, void* pData, ComPtr<ID3D11Buffer>& stagingBuffer)
{
	D3D11_MAPPED_SUBRESOURCE ms;
	context->Map(stagingBuffer.Get(), 0, D3D11_MAP_WRITE, 0, &ms);
	for (int i = 0; i < numElement; ++i)
		memcpy(ms.pData, pData, elementSize);
	context->Unmap(stagingBuffer.Get(), 0);
}

