#include "pch.h"
#include "D3DUtils.h"

void D3DUtils::CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device>& device, const wstring& vsPrefix
	, ComPtr<ID3D11VertexShader>& vs, const vector<D3D11_INPUT_ELEMENT_DESC>& desc
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

void D3DUtils::CreatePixelShader(ComPtr<ID3D11Device>& device, const wstring& psPrefix, ComPtr<ID3D11PixelShader>& ps)
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

