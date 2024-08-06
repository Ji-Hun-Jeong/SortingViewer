#include "pch.h"
#include "Mesh.h"
#include "D3DUtils.h"

void Mesh::Init(ComPtr<ID3D11Device>& device, const MeshData& meshData)
{
	D3DUtils::CreateVertexBuffer<Vertex>(device, meshData.vertices, m_vertexBuffer);

	D3DUtils::CreateIndexBuffer<uint16_t>(device, meshData.indices, m_indexBuffer);

	m_indexCount = meshData.indices.size();
}

void Mesh::Render(ComPtr<ID3D11DeviceContext>& context)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	context->DrawIndexed(m_indexCount, 0, 0);
}
