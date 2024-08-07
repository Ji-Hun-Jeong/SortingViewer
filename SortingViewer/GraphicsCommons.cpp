#include "pch.h"
#include "GraphicsCommons.h"
#include "D3DUtils.h"

namespace Graphics
{
	ComPtr<ID3D11SamplerState> linearSampler;

	ComPtr<ID3D11InputLayout> basicInputLayout;

	ComPtr<ID3D11VertexShader> basicVS;

	ComPtr<ID3D11PixelShader> basicPS;

	ComPtr<ID3D11RasterizerState> solidCWRS;

	ComPtr<ID3D11DepthStencilState> basicDSS;

	GraphicsPSO basicSolidPSO;

	void InitCommons(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
	{
		InitSampler(device);
		InitShader(device);
		InitRasterizerState(device);
		InitDepthStencilState(device);
		InitPSO();
	}
	void InitSampler(ComPtr<ID3D11Device>& device)
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;
		CHECKRESULT(device->CreateSamplerState(&desc, linearSampler.GetAddressOf()));
	}
	void InitShader(ComPtr<ID3D11Device>& device)
	{
		vector<D3D11_INPUT_ELEMENT_DESC> desc =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA , 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA , 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA , 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA , 0 }
		};
		D3DUtils::CreateVertexShaderAndInputLayout(device, L"Basic", basicVS, desc, basicInputLayout);

		D3DUtils::CreatePixelShader(device, L"Basic", basicPS);
	}
	void InitRasterizerState(ComPtr<ID3D11Device>& device)
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = false;
		desc.DepthClipEnable = true;
		desc.MultisampleEnable = true;

		device->CreateRasterizerState(&desc, solidCWRS.GetAddressOf());
	}
	void InitDepthStencilState(ComPtr<ID3D11Device>& device)
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS;
		desc.StencilEnable = false;
		desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;	
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;	
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		device->CreateDepthStencilState(&desc, basicDSS.GetAddressOf());
	}
	void InitPSO()
	{
		basicSolidPSO.SetInputLayout(basicInputLayout);
		basicSolidPSO.SetVS(basicVS);
		basicSolidPSO.SetPS(basicPS);
		basicSolidPSO.SetRS(solidCWRS);
		basicSolidPSO.SetDSS(basicDSS);
	}
}