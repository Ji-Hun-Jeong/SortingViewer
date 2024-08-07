#pragma once
class ConstBuffer;
class Texture2D;
class Mesh
{
public:
	explicit Mesh();
public:
	void Init(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context
		, const MeshData& meshData);
	void Update(ComPtr<ID3D11DeviceContext>& context);
	void Render(ComPtr<ID3D11DeviceContext>& context);

	Vector3& GetTrans() { return m_translation; }
	Vector3& GetRotation() { return m_rotation; }
	Vector3& GetScale() { return m_scale; }

protected:
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	UINT m_indexCount = 0;

	MeshConst m_constData;
	shared_ptr<ConstBuffer> m_constBuffer;

	shared_ptr<Texture2D> m_texture;

	Vector3 m_translation;
	Vector3 m_rotation;
	Vector3 m_scale;
};

