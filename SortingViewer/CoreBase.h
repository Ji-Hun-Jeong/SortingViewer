#pragma once
#include "GraphicsPSO.h"
#include "Texture2D.h"
class CoreBase
{
public:
	CoreBase();
public:
	void Init(HWND hWnd, UINT width, UINT height);
	void InitDirect3D();
	void CreateDeviceAndSwapChain();
	void CreateViewPort();
	void CreateBackBufferView();
	void CreateDepthBuffer();

protected:
	void SetPSO(GraphicsPSO& pso);
	void ComputeShaderBarrier();

protected:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_context;
	ComPtr<IDXGISwapChain> m_swapChain;

	D3D11_VIEWPORT m_viewPort;

	Texture2D m_msaaBuffer;
	Texture2D m_resolveBuffer;
	Texture2D m_backBuffer;

	ComPtr<ID3D11Texture2D> m_depthBuffer;
	ComPtr<ID3D11DepthStencilView> m_dsv;

	UINT m_msaaLevel;
	

protected:
	HWND m_hWnd;
	UINT m_width;
	UINT m_height;

};

