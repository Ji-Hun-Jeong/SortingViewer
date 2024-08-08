#include "pch.h"
#include "Core.h"
#include "GraphicsCommons.h"
#include "GeometryGenerator.h"
#include "Mesh.h"
#include "Camera.h"
#include "ConstBuffer.h"
#include "KeyMgr.h"
#include "Sorter.h"
Core::Core()
	: CoreBase()
{

}

void Core::Init(HWND hWnd, UINT width, UINT height)
{
	CoreBase::Init(hWnd, width, height);
	Graphics::InitCommons(m_device, m_context);
	KeyMgr::Init(hWnd, m_width, m_height);

	MeshData sphere = GeometryGenerator::MakeSphere(1.0f, 30, 30);
	m_skyBox = make_shared<Mesh>();
	m_skyBox->GetScale() = Vector3(30.0f);
	m_skyBox->Init(m_device, m_context, sphere);

	m_sorter = make_shared<Sorter>();
	m_sorter->Init(m_device, m_context);
	m_globalConst.maxHeight = m_sorter->m_maxHeight;

	m_camera = make_shared<Camera>(70.0f, float(m_width) / m_height, 0.01f, 100.0f);
	m_camera->SetPos(Vector3(0.0f, 0.0f, -1.0f));

	m_globalConstBuffer = make_shared<ConstBuffer>();
	m_globalConstBuffer->Init(m_device, sizeof(m_globalConst), 1, &m_globalConst);

	m_arrIBL[(UINT)IBL_TYPE::SPECULAR].ReadDDSImage(m_device, L"Image/SkyBox/SunSetSpecularHDR.dds");
	m_arrIBL[(UINT)IBL_TYPE::IRRADIANCE].ReadDDSImage(m_device, L"Image/SkyBox/SunSetDiffuseHDR.dds");
	m_arrIBL[(UINT)IBL_TYPE::LUT].ReadDDSImage(m_device, L"Image/SkyBox/SunSetBrdf.dds");

	m_context->VSSetSamplers(0, 1, Graphics::linearSampler.GetAddressOf());
	m_context->PSSetSamplers(0, 1, Graphics::linearSampler.GetAddressOf());

	m_context->VSSetConstantBuffers(0, 1, m_globalConstBuffer->GetBufferAddress());
	m_context->PSSetConstantBuffers(0, 1, m_globalConstBuffer->GetBufferAddress());

	vector<ID3D11ShaderResourceView*> arrSRV =
	{
		m_arrIBL[(UINT)IBL_TYPE::SPECULAR].GetSRV(),
		m_arrIBL[(UINT)IBL_TYPE::IRRADIANCE].GetSRV(),
		m_arrIBL[(UINT)IBL_TYPE::LUT].GetSRV()
	};
	m_context->PSSetShaderResources(10, 1, arrSRV.data());
}

void Core::Update()
{
	static const float dt = 0.01f;
	KeyMgr::Update();

	m_camera->Update(0.01f);

	UpdateGlobalConst();

	if (KEYCHECK(SPACE, TAP))
		m_sorter->GenerateRandomElements(m_device, m_context);
	m_sorter->Update(m_context, dt);
	m_skyBox->Update(m_context, dt);
}

void Core::Render()
{
	float clearColor[4] = { 0.3f,0.3f,1.0f,1.0f };
	m_context->ClearRenderTargetView(m_msaaBuffer.GetRTV(), clearColor);
	m_context->ClearDepthStencilView(m_dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_context->OMSetRenderTargets(1, m_msaaBuffer.GetRTVAddress(), m_dsv.Get());

	m_context->RSSetViewports(1, &m_viewPort);
	CoreBase::SetPSO(Graphics::basicSolidPSO);

	m_sorter->Render(m_context);

	CoreBase::SetPSO(Graphics::skyBoxSolidPSO);
	m_skyBox->Render(m_context);

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
