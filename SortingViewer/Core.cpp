#include "pch.h"
#include "Core.h"
#include "GraphicsCommons.h"
#include "GeometryGenerator.h"
#include "Mesh.h"
#include "Camera.h"
#include "ConstBuffer.h"
#include "KeyMgr.h"
Core::Core()
	: CoreBase()
{

}

void Core::Init(HWND hWnd, UINT width, UINT height)
{
	CoreBase::Init(hWnd, width, height);
	Graphics::InitCommons(m_device, m_context);
	KeyMgr::Init(hWnd, m_width, m_height);

	MeshData triangle = GeometryGenerator::MakeSquare();
	m_mesh = make_shared<Mesh>();
	m_mesh->GetScale() = Vector3(0.3f);
	m_mesh->GetRotation().z = 90 * XM_PI / 180.0f;
	m_mesh->Init(m_device, m_context, triangle);

	m_camera = make_shared<Camera>(70.0f, float(m_width) / m_height, 0.01f, 100.0f);
	m_camera->SetPos(Vector3(0.0f, 0.0f, -1.0f));

	m_globalConstBuffer = make_shared<ConstBuffer>();
	m_globalConstBuffer->Init(m_device, sizeof(m_globalConst), 1, &m_globalConst);

	m_context->VSSetSamplers(0, 1, Graphics::linearSampler.GetAddressOf());
	m_context->PSSetSamplers(0, 1, Graphics::linearSampler.GetAddressOf());

	m_context->VSSetConstantBuffers(0, 1, m_globalConstBuffer->GetBufferAddress());
	m_context->PSSetConstantBuffers(0, 1, m_globalConstBuffer->GetBufferAddress());
}

void Core::Update()
{
	static const float dt = 0.01f;
	KeyMgr::Update();

	m_camera->Update(0.01f);

	UpdateGlobalConst();

	m_mesh->Update(m_context, dt);
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

void Core::UpdateGlobalConst()
{
	m_globalConst.view = m_camera->GetViewRow();
	m_globalConst.proj = m_camera->GetProjRow();
	m_globalConst.view = m_globalConst.view.Transpose();
	m_globalConst.proj = m_globalConst.proj.Transpose();
	m_globalConstBuffer->Update(m_context, sizeof(m_globalConst), 1, &m_globalConst);
}

void Core::Progress()
{
	this->Update();
	this->Render();
	m_swapChain->Present(1, 0);
}
