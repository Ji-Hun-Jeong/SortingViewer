#include "pch.h"
#include "CoreBase.h"

CoreBase::CoreBase()
	: m_hWnd(nullptr)
	, m_width(0)
	, m_height(0)
	, m_viewPort{}
	, m_msaaLevel(0)
{
}

void CoreBase::Init(HWND hWnd, UINT width, UINT height)
{
	m_hWnd = hWnd;
	m_width = width;
	m_height = height;
	InitDirect3D();

	m_msaaBuffer.Init(m_device, m_width, m_height, DXGI_FORMAT_R16G16B16A16_FLOAT
		, VIEW_RTV, m_msaaLevel);

	m_resolveBuffer.Init(m_device, m_width, m_height, DXGI_FORMAT_R16G16B16A16_FLOAT
		, VIEW_SRV | VIEW_RTV, 0);
}

void CoreBase::InitDirect3D()
{
	CreateDeviceAndSwapChain();
	CreateViewPort();
	CreateBackBufferView();
	CreateDepthBuffer();
}

void CoreBase::CreateDeviceAndSwapChain()
{
	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE;
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_0 ,D3D_FEATURE_LEVEL_9_3 };
	D3D_FEATURE_LEVEL outputLevel;

	CHECKRESULT(D3D11CreateDevice(nullptr, driverType, 0, createDeviceFlags, featureLevel,
		ARRAYSIZE(featureLevel), D3D11_SDK_VERSION, m_device.GetAddressOf(),
		&outputLevel, m_context.GetAddressOf()));

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferDesc.Width = m_width;
	swapChainDesc.BufferDesc.Height = m_height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT
		| DXGI_USAGE_UNORDERED_ACCESS;
	swapChainDesc.OutputWindow = m_hWnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	CHECKRESULT(m_device->CheckMultisampleQualityLevels(swapChainDesc.BufferDesc.Format,
		4, &m_msaaLevel));

	CHECKRESULT(D3D11CreateDeviceAndSwapChain(nullptr, driverType, 0
		, createDeviceFlags, featureLevel, 1
		, D3D11_SDK_VERSION, &swapChainDesc
		, m_swapChain.GetAddressOf(), m_device.GetAddressOf()
		, &outputLevel, m_context.GetAddressOf()));
}

void CoreBase::CreateViewPort()
{
	m_viewPort.TopLeftX = 0.0f;
	m_viewPort.TopLeftY = 0.0f;
	m_viewPort.Width = float(m_width);
	m_viewPort.Height = float(m_height);
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
}

void CoreBase::CreateBackBufferView()
{
	ComPtr<ID3D11Texture2D> backBuffer;
	CHECKRESULT(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));
	m_backBuffer.Init(m_device, backBuffer, VIEW_RTV | VIEW_UAV);
}

void CoreBase::CreateDepthBuffer()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = m_width;
	desc.Height = m_height;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (m_msaaLevel > 0)
	{
		desc.SampleDesc.Count = 4;
		desc.SampleDesc.Quality = m_msaaLevel - 1;
	}
	else
	{
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
	}
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	CHECKRESULT(m_device->CreateTexture2D(&desc, nullptr, m_depthBuffer.GetAddressOf()));

	CHECKRESULT(m_device->CreateDepthStencilView(m_depthBuffer.Get(), nullptr, m_dsv.GetAddressOf()));
}

void CoreBase::SetPSO(GraphicsPSO& pso)
{
	m_context->IASetInputLayout(pso.GetInputLayout().Get());
	m_context->IASetPrimitiveTopology(pso.GetTopology());
	m_context->VSSetShader(pso.GetVS().Get(), nullptr, 0);
	m_context->CSSetShader(pso.GetCS().Get(), nullptr, 0);
	m_context->PSSetShader(pso.GetPS().Get(), nullptr, 0);
	m_context->RSSetState(pso.GetRS().Get());
	m_context->OMSetDepthStencilState(pso.GetDSS().Get(), pso.m_stencilRef);
}

void CoreBase::ComputeShaderBarrier()
{
	ID3D11ShaderResourceView* srv[6] = { nullptr };
	m_context->CSSetShaderResources(0, 6, srv);
	ID3D11UnorderedAccessView* uav[6] = { nullptr };
	m_context->CSSetUnorderedAccessViews(0, 6, uav, nullptr);
}
