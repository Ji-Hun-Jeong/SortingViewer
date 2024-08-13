#pragma once
class ConstBuffer;
class Texture2D;
class Mesh
{
public:
	Mesh();
public:
	void Init(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context
		, const MeshData& meshData);
	void Update(const GlobalConst globalConst, float dt);
	void FinalUpdate(ComPtr<ID3D11DeviceContext>& context, float dt);
	void Render(ComPtr<ID3D11DeviceContext>& context);
	void ReadImage(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context
		, const string& filePath, bool useSRGB = false);
	void FrustumCulling(const GlobalConst& globalConst);

	Vector3& GetTrans() { return m_translation; }
	Vector3& GetRotation() { return m_rotation; }
	Vector3& GetScale() { return m_scale; }
	MeshConst& GetMeshConst() { return m_meshConst; }

protected:
	MeshData m_meshData;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	UINT m_indexCount = 0;

	MeshConst m_meshConst;
	shared_ptr<ConstBuffer> m_meshConstBuffer;

	shared_ptr<Texture2D> m_texture;

	Vector3 m_translation;
	Vector3 m_rotation;
	Vector3 m_scale;

	bool m_drawMe = true;
};

