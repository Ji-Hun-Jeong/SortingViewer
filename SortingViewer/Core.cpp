#include "pch.h"
#include "Core.h"
#include "GraphicsCommons.h"
#include "GeometryGenerator.h"
#include "Mesh.h"
Core::Core()
	: CoreBase()
{

}

void Core::Init(HWND hWnd, UINT width, UINT height)
{
	CoreBase::Init(hWnd, width, height);
	Graphics::InitCommons(m_device, m_context);

	m_context->VSSetSamplers(0, 1, Graphics::linearSampler.GetAddressOf());
	m_context->PSSetSamplers(0, 1, Graphics::linearSampler.GetAddressOf());

	MeshData triangle = GeometryGenerator::MakeSquare();
	m_mesh = make_shared<Mesh>();
	m_mesh->GetScale() = Vector3(0.3f);
	m_mesh->GetRotation().z = 90 * XM_PI / 180.0f;
	m_mesh->Init(m_device, m_context, triangle);
}

void Core::Update()
{
	m_mesh->Update(m_context);
}

void Core::Render()
{
	float clearColor[4] = { 0.3f,0.3f,1.0f,1.0f };
	m_context->ClearRenderTargetView(m_msaaBuffer.GetRTV(), clearColor);
	m_context->ClearDepthStencilView(m_dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_context->OMSetRenderTargets(1, m_msaaBuffer.GetRTVAddress(), m_dsv.Get());

	m_context->RSSetViewports(1, &m_viewPort);
	CoreBase::SetPSO(Graphics::basicSolidPSO);

	m_mesh->Render(m_context);

	m_context->ResolveSubresource(m_resolveBuffer.GetTexture(), 0, m_msaaBuffer.GetTexture()
		, 0, DXGI_FORMAT_R16G16B16A16_FLOAT);

	m_context->CopyResource(m_backBuffer.GetTexture(), m_resolveBuffer.GetTexture());
}

void Core::Progress()
{
	this->Update();
	this->Render();
	m_swapChain->Present(1, 0);
}
