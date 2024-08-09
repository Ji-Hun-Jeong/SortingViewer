#include "pch.h"
#include "Mesh.h"
#include "D3DUtils.h"
#include "ConstBuffer.h"
#include "Texture2D.h"

Mesh::Mesh()
	: m_scale(1.0f)
{
}

void Mesh::Init(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context
	, const MeshData& meshData)
{
	m_meshData = meshData;
	D3DUtils::CreateVertexBuffer<Vertex>(device, meshData.vertices, m_vertexBuffer);

	D3DUtils::CreateIndexBuffer<uint16_t>(device, meshData.indices, m_indexBuffer);

	m_indexCount = meshData.indices.size();

	m_meshConstBuffer = make_shared<ConstBuffer>();
	m_meshConstBuffer->Init(device, sizeof(m_meshConst), 1, &m_meshConst);
	m_meshConstBuffer->Update(context, sizeof(m_meshConst), 1, &m_meshConst);

	m_texture = make_shared<Texture2D>();
}

void Mesh::Update(const GlobalConst globalConst, float dt)
{
	m_meshConst.world =
		Matrix::CreateScale(m_scale)
		* Matrix::CreateRotationX(m_rotation.x)
		* Matrix::CreateRotationY(m_rotation.y)
		* Matrix::CreateRotationZ(m_rotation.z)
		* Matrix::CreateTranslation(m_translation);
	FrustumCulling(globalConst);
}

void Mesh::FinalUpdate(ComPtr<ID3D11DeviceContext>& context, float dt)
{
	m_meshConst.world = m_meshConst.world.Transpose();
	m_meshConstBuffer->Update(context, sizeof(m_meshConst), 1, &m_meshConst);
}

void Mesh::Render(ComPtr<ID3D11DeviceContext>& context)
{
	if (!m_drawMe)
		return;
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	context->VSSetConstantBuffers(1, 1, m_meshConstBuffer->GetBufferAddress());

	context->PSSetConstantBuffers(1, 1, m_meshConstBuffer->GetBufferAddress());
	context->PSSetShaderResources(0, 1, m_texture->GetSRVAddress());

	context->DrawIndexed(m_indexCount, 0, 0);
}

void Mesh::ReadImage(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context
	, const string& filePath, bool useSRGB)
{
	m_texture->ReadImage(device, context, filePath, useSRGB);
}

void Mesh::FrustumCulling(const GlobalConst& globalConst)
{
	Matrix mvp = m_meshConst.world * globalConst.viewProj;
	for (auto& v : m_meshData.vertices)
	{
		Vector4 pos = Vector4(v.pos.x, v.pos.y, v.pos.z, 1.0f);
		pos = Vector4::Transform(pos, mvp);
		if (pos.w != 0)
			pos /= pos.w;
		// Box의 Scale이 너무 커서 화면에 있는게 확실함에도 Vertex들이 화면에서 빠져나가서
		// 안그려짐
		if (pos.z >= 0.0f && pos.z <= 1.0f)
		{
			m_drawMe = true;
			return;
		}
	}
	m_drawMe = false;
}
