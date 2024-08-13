#pragma once
class Buffer
{
public:
	virtual void Init(ComPtr<ID3D11Device>& device, UINT elementSize
		, UINT numElement, void* pData) = 0;

	void Update(ComPtr<ID3D11DeviceContext>& context, UINT elementSize
		, UINT numElement, void* pData);

	ID3D11Buffer* GetBuffer() { return m_buffer.Get(); }
	ID3D11Buffer** GetBufferAddress() { return m_buffer.GetAddressOf(); }
protected:
	ComPtr<ID3D11Buffer> m_buffer;
	ComPtr<ID3D11Buffer> m_stagingBuffer;
};

