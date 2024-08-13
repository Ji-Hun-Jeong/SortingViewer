#pragma once
#include "GraphicsPSO.h"
namespace Graphics
{
	extern ComPtr<ID3D11SamplerState> linearSampler;

	extern ComPtr<ID3D11InputLayout> basicInputLayout;

	extern ComPtr<ID3D11VertexShader> basicVS;
	extern ComPtr<ID3D11VertexShader> skyBoxVS;

	extern ComPtr<ID3D11PixelShader> basicPS;
	extern ComPtr<ID3D11PixelShader> skyBoxPS;

	extern ComPtr<ID3D11RasterizerState> solidCWRS;
	extern ComPtr<ID3D11RasterizerState> solidCCWRS;

	extern ComPtr<ID3D11DepthStencilState> basicDSS;

	extern GraphicsPSO basicSolidPSO;
	extern GraphicsPSO skyBoxSolidPSO;

	void InitCommons(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
	void InitSampler(ComPtr<ID3D11Device>& device);
	void InitShader(ComPtr<ID3D11Device>& device);
	void InitRasterizerState(ComPtr<ID3D11Device>& device);
	void InitDepthStencilState(ComPtr<ID3D11Device>& device);
	void InitPSO();
}