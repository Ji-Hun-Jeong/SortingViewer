#pragma once
class Mesh
{
public:
	void Init(ComPtr<ID3D11Device>& device, const MeshData& meshData);

	void Render(ComPtr<ID3D11DeviceContext>& context);
private:
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	UINT m_indexCount = 0;
};

