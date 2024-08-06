#pragma once
class Texture2D
{
public:
	void Init(ComPtr<ID3D11Device>& device, UINT width, UINT height, DXGI_FORMAT pixelFormat
		, UINT viewType, UINT msaaLevel);
	void Init(ComPtr<ID3D11Device>& device, DXGI_FORMAT pixelFormat
		, UINT viewType, void* initData);
	void Init(ComPtr<ID3D11Device>& device, ComPtr<ID3D11Texture2D>& texture
		, UINT viewType);
	void CreateView(ComPtr<ID3D11Device>& device, UINT viewType);

public:
	ID3D11Texture2D* GetTexture() { return m_texture.Get(); }
	ID3D11Texture2D** GetTextureAddress() { return m_texture.GetAddressOf(); }

	ID3D11ShaderResourceView* GetSRV() { return m_srv.Get(); }
	ID3D11ShaderResourceView** GetSRVAddress() { return m_srv.GetAddressOf(); }

	ID3D11RenderTargetView* GetRTV() { return m_rtv.Get(); }
	ID3D11RenderTargetView** GetRTVAddress() { return m_rtv.GetAddressOf(); }

	ID3D11UnorderedAccessView* GetUAV() { return m_uav.Get(); }
	ID3D11UnorderedAccessView** GetUAVAddress() { return m_uav.GetAddressOf(); }

private:
	ComPtr<ID3D11Texture2D> m_texture;
	ComPtr<ID3D11Texture2D> m_stagingTexture;
	
	ComPtr<ID3D11ShaderResourceView> m_srv;
	ComPtr<ID3D11RenderTargetView> m_rtv;
	ComPtr<ID3D11UnorderedAccessView> m_uav;
};

