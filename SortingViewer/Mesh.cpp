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
	D3DUtils::CreateVertexBuffer<Vertex>(device, meshData.vertices, m_vertexBuffer);

	D3DUtils::CreateIndexBuffer<uint16_t>(device, meshData.indices, m_indexBuffer);

	m_indexCount = meshData.indices.size();

	m_constBuffer = make_shared<ConstBuffer>();
	m_constBuffer->Init(device, sizeof(m_constData), 1, &m_constData);

	m_texture = make_shared<Texture2D>();
	m_texture->ReadImage(device, context, "Image/Ground.png");
}

void Mesh::Update(ComPtr<ID3D11DeviceContext>& context)
{
	m_constData.world =
		Matrix::CreateScale(m_scale)
		* Matrix::CreateRotationX(m_rotation.x)
		* Matrix::CreateRotationY(m_rotation.y)
		* Matrix::CreateRotationZ(m_rotation.z)
		* Matrix::CreateTranslation(m_translation);
	m_constData.world = m_constData.world.Transpose();

	m_constBuffer->Update(context, sizeof(m_constData), 1, &m_constData);
}

void Mesh::Render(ComPtr<ID3D11DeviceContext>& context)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	context->VSSetConstantBuffers(1, 1, m_constBuffer->GetBufferAddress());

	context->PSSetShaderResources(0, 1, m_texture->GetSRVAddress());

	context->DrawIndexed(m_indexCount, 0, 0);
}
