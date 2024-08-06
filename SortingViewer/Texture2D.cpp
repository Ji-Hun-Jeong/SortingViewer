#include "pch.h"
#include "Texture2D.h"

void Texture2D::Init(ComPtr<ID3D11Device>& device, UINT width, UINT height, DXGI_FORMAT pixelFormat
	, UINT viewType, UINT msaaLevel)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = pixelFormat;
	desc.Usage = D3D11_USAGE_DEFAULT;
	if (viewType & VIEW_SRV)
		desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	if (viewType & VIEW_RTV)
		desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	if (viewType & VIEW_UAV)
		desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	if (msaaLevel > 0)
	{
		desc.SampleDesc.Count = 4;
		desc.SampleDesc.Quality = msaaLevel - 1;
	}
	else
	{
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
	}
	CHECKRESULT(device->CreateTexture2D(&desc, nullptr, m_texture.GetAddressOf()));
	CreateView(device, viewType);
}

void Texture2D::Init(ComPtr<ID3D11Device>& device, DXGI_FORMAT pixelFormat
	, UINT viewType, void* initData)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width;
	desc.Height;
	desc.MipLevels;
	desc.ArraySize;
	desc.Format;
	desc.SampleDesc;
	desc.Usage;
	desc.BindFlags;
	desc.CPUAccessFlags;
	desc.MiscFlags;

}

void Texture2D::Init(ComPtr<ID3D11Device>& device, ComPtr<ID3D11Texture2D>& texture, UINT viewType)
{
	m_texture = texture;
	CreateView(device, viewType);
}

void Texture2D::CreateView(ComPtr<ID3D11Device>& device, UINT viewType)
{
	if (viewType & VIEW_SRV)
		CHECKRESULT(device->CreateShaderResourceView(m_texture.Get(), nullptr, m_srv.GetAddressOf()));
	if (viewType & VIEW_RTV)
		CHECKRESULT(device->CreateRenderTargetView(m_texture.Get(), nullptr, m_rtv.GetAddressOf()));
	if (viewType & VIEW_UAV)
		CHECKRESULT(device->CreateUnorderedAccessView(m_texture.Get(), nullptr, m_uav.GetAddressOf()));
}
