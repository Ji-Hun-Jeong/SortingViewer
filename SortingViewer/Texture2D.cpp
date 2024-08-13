#include "pch.h"
#include "Texture2D.h"
#include "D3DUtils.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void Texture2D::Init(ComPtr<ID3D11Device>& device, UINT width, UINT height, DXGI_FORMAT pixelFormat
	, UINT viewType, UINT msaaLevel)
{
	m_width = width;
	m_height = height;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = m_width;
	desc.Height = m_height;
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

void Texture2D::ReadImage(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context
	, const string& filePath, bool useSRGB)
{
	int channel = 0;
	uint8_t* img = stbi_load(filePath.c_str(), &m_width, &m_height, &channel, 0);

	DXGI_FORMAT pixelFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (useSRGB)
		pixelFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	this->CreateStagingTexture(device, m_width, m_height, pixelFormat);

	size_t pixelSize = sizeof(uint8_t) * 4;
	this->CopyToStagingTexture(context, m_width, m_height, pixelSize, img);

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = m_width;
	desc.Height = m_height;
	desc.MipLevels = 0;
	desc.ArraySize = 1;
	desc.Format = pixelFormat;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	CHECKRESULT(device->CreateTexture2D(&desc, nullptr, m_texture.GetAddressOf()));

	// texture를 밉맵으로 설정해두었기 때문에 copyresource는 안됌
	context->CopySubresourceRegion(m_texture.Get(), 0, 0, 0, 0, m_stagingTexture.Get(), 0, 0);
	//context->CopyResource(m_texture.Get(), m_stagingTexture.Get());

	this->CreateView(device, VIEW_SRV);

	context->GenerateMips(m_srv.Get());
}

void Texture2D::ReadDDSImage(ComPtr<ID3D11Device>& device, const wstring& filePath)
{
	CHECKRESULT(DirectX::CreateDDSTextureFromFileEx(device.Get(), filePath.c_str(), 0
		, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0
		, D3D11_RESOURCE_MISC_TEXTURECUBE, DirectX::DDS_LOADER_FLAGS(false)
		, (ID3D11Resource**)m_texture.GetAddressOf()
		, m_srv.GetAddressOf(), nullptr));

	D3D11_TEXTURE2D_DESC desc;
	m_texture->GetDesc(&desc);
	m_width = desc.Width;
	m_height = desc.Height;
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

void Texture2D::CreateStagingTexture(ComPtr<ID3D11Device>& device, UINT width
	, UINT height, DXGI_FORMAT pixelFormat)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = pixelFormat;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	CHECKRESULT(device->CreateTexture2D(&desc, nullptr, m_stagingTexture.GetAddressOf()));
}

void Texture2D::CopyToStagingTexture(ComPtr<ID3D11DeviceContext>& context, UINT width
	, UINT height, size_t pixelSize, uint8_t* pData)
{
	D3D11_MAPPED_SUBRESOURCE ms;
	// 먼저 map으로 ms를 초기화
	context->Map(m_stagingTexture.Get(), 0, D3D11_MAP_WRITE, 0, &ms);
	// 텍스쳐를 읽으면 거기있는 픽셀포맷과 width를 계산하여 한줄이 얼마인지를 rowpitch에 가지고있음
	uint8_t* gpuMem = (uint8_t*)ms.pData;
	for (int i = 0; i < height; ++i)
		memcpy(&gpuMem[i * ms.RowPitch], &pData[i * ms.RowPitch], ms.RowPitch);

	context->Unmap(m_stagingTexture.Get(), 0);
}
