#pragma once
class Texture2D;
class D3DUtils
{
public:
	static void CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device>& device, const wstring& vsPrefix, ComPtr<ID3D11VertexShader>& vs,
		const vector<D3D11_INPUT_ELEMENT_DESC>& desc, ComPtr<ID3D11InputLayout>& inputLayout);
	static void CreatePixelShader(ComPtr<ID3D11Device>& device, const wstring& psPrefix, ComPtr<ID3D11PixelShader>& ps);
	static void CreateComputeShader(ComPtr<ID3D11Device>& device, const wstring& csPrefix, ComPtr<ID3D11ComputeShader>& cs);

	template <typename T_Vertex>
	static void CreateVertexBuffer(ComPtr<ID3D11Device>& device, const vector<T_Vertex>& vertices
		, ComPtr<ID3D11Buffer>& vertexBuffer);

	template <typename T_Int>
	static void CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<T_Int>& indices
		, ComPtr<ID3D11Buffer>& indexBuffer);

	static void CreateConstBuffer(ComPtr<ID3D11Device>& device
		, UINT elementSize, ComPtr<ID3D11Buffer>& constBuffer);

	static void CreateStagingBuffer(ComPtr<ID3D11Device>& device, UINT elementSize
		, UINT numElement, void* initData, ComPtr<ID3D11Buffer>& stagingBuffer);

	static void UpdateStagingBuffer(ComPtr<ID3D11DeviceContext>& context, UINT elementSize
		, UINT numElement, void* pData, ComPtr<ID3D11Buffer>& stagingBuffer);

	
};

template<typename T_Vertex>
inline void D3DUtils::CreateVertexBuffer(ComPtr<ID3D11Device>& device, const vector<T_Vertex>& vertices
	, ComPtr<ID3D11Buffer>& vertexBuffer)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = vertices.size() * sizeof(T_Vertex);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride= sizeof(T_Vertex);

	D3D11_SUBRESOURCE_DATA dataDesc = { 0 };
	dataDesc.pSysMem = vertices.data();	// 초기화 데이터에 대한 포인터
	dataDesc.SysMemPitch = 0;			// 텍스쳐 한줄 시작부터 다음줄까지의 바이트 (텍스쳐에만 쓰임)
	dataDesc.SysMemSlicePitch = 0;		// 3D 텍스쳐에만 사용

	CHECKRESULT(device->CreateBuffer(&bufferDesc, &dataDesc, vertexBuffer.GetAddressOf()));
}

template<typename T_Int>
inline void D3DUtils::CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<T_Int>& indices
	, ComPtr<ID3D11Buffer>& indexBuffer)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = (UINT)indices.size() * sizeof(T_Int);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.StructureByteStride = sizeof(T_Int);

	D3D11_SUBRESOURCE_DATA dataDesc = { 0 };
	dataDesc.pSysMem = indices.data();
	dataDesc.SysMemPitch = 0;
	dataDesc.SysMemSlicePitch = 0;
	CHECKRESULT(device->CreateBuffer(&bufferDesc, &dataDesc, indexBuffer.GetAddressOf()));
}
