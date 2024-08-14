#include "pch.h"
#include "Core.h"
#include "GraphicsCommons.h"
#include "GeometryGenerator.h"
#include "Mesh.h"
#include "Camera.h"
#include "ConstBuffer.h"
#include "KeyMgr.h"
#include "ContinueSorter.h"
#include "OneStepSorter.h"
Core::Core()
	: CoreBase()
	, m_sortMode(SORTMODE_TYPE::CONTINUE)
{

}

Core::~Core()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Core::Init(HWND hWnd, UINT width, UINT height)
{
	CoreBase::Init(hWnd, width, height);

	Graphics::InitCommons(m_device, m_context);

	KeyMgr::Init(hWnd, m_width, m_height);

	MeshData sphere = GeometryGenerator::MakeSphere(10000.0f, 30, 30);
	m_skyBox = make_shared<Mesh>();
	m_skyBox->Init(m_device, m_context, sphere);

	CreateSorter(m_sortMode);

	MeshData square = GeometryGenerator::MakeSquare();
	m_ground = make_shared<Mesh>();
	m_ground->Init(m_device, m_context, square);
	m_ground->ReadImage(m_device, m_context, "Image/Ground.png", true);
	m_ground->GetMeshConst().metallic = 0.99f;
	m_ground->GetMeshConst().roughness = 0.46f;
	//m_ground->GetTrans().y = -1.0f;
	m_ground->GetScale() = Vector3(100.0f);
	m_ground->GetRotation().x = 90.0f * XM_PI / 180.0f;

	m_camera = make_shared<Camera>(70.0f, float(m_width) / m_height, 0.001f, 100000.0f);
	m_camera->SetPos(Vector3(0.0f, 0.0f, -1.0f));

	m_context->VSSetSamplers(0, 1, Graphics::linearSampler.GetAddressOf());
	m_context->CSSetSamplers(0, 1, Graphics::linearSampler.GetAddressOf());
	m_context->PSSetSamplers(0, 1, Graphics::linearSampler.GetAddressOf());

	m_arrIBL[(UINT)IBL_TYPE::SPECULAR].ReadDDSImage(m_device, L"Image/SkyBox/DaySky29SpecularHDR.dds");
	m_arrIBL[(UINT)IBL_TYPE::IRRADIANCE].ReadDDSImage(m_device, L"Image/SkyBox/DaySky29DiffuseHDR.dds");
	m_arrIBL[(UINT)IBL_TYPE::LUT].ReadDDSImage(m_device, L"Image/SkyBox/DaySky29Brdf.dds");

	vector<ID3D11ShaderResourceView*> arrSRV =
	{
		m_arrIBL[(UINT)IBL_TYPE::SPECULAR].GetSRV(),
		m_arrIBL[(UINT)IBL_TYPE::IRRADIANCE].GetSRV(),
		m_arrIBL[(UINT)IBL_TYPE::LUT].GetSRV()
	};
	m_context->PSSetShaderResources(10, UINT(arrSRV.size()), arrSRV.data());

	m_globalConst.eyePos = m_camera->GetPos();
	m_globalConst.light.pos = Vector3(0.0f, 0.0f, -1.0f);
	m_globalConst.light.strength = Vector3(1.0f);

	m_globalConstBuffer = make_shared<ConstBuffer>();
	m_globalConstBuffer->Init(m_device, sizeof(m_globalConst), 1, &m_globalConst);

	m_context->VSSetConstantBuffers(0, 1, m_globalConstBuffer->GetBufferAddress());
	m_context->PSSetConstantBuffers(0, 1, m_globalConstBuffer->GetBufferAddress());

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = m_width;
	desc.Height = m_height;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	CHECKRESULT(m_device->CreateTexture2D(&desc, nullptr
		, m_lightViewDepthBuffer.GetAddressOf()));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;  
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	CHECKRESULT(m_device->CreateDepthStencilView(m_lightViewDepthBuffer.Get(), &dsvDesc
		, m_lightViewDSV.GetAddressOf()));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;  // Shader resource view format
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	CHECKRESULT(m_device->CreateShaderResourceView(m_lightViewDepthBuffer.Get(), &srvDesc
		, m_lightViewSRV.GetAddressOf()));

	//m_context->PSSetShaderResources(1, 1, m_lightViewSRV.GetAddressOf());
}

void Core::UpdateGUI()
{
	ImGui::SliderFloat3("LightPos", &m_globalConst.light.pos.x, -100.0f, 100.0f);
	ImGui::SliderFloat("Metallic", &m_ground->GetMeshConst().metallic, 0.0f, 1.0f);
	ImGui::SliderFloat("Roughness", &m_ground->GetMeshConst().roughness, 0.0f, 1.0f);
	ImGui::SliderFloat("SpotFactor", &m_globalConst.light.spotFactor, 0.0f, 10.0f);
	ImGui::Text("Red is Standard, Blue And Green is Comparison, Matenta is Partition");
	ImGui::Text("Button Space : Generate Random Element");
	ImGui::Text("Button 1 : Select Sort");
	ImGui::Text("Button 2 : Bubble Sort");
	ImGui::Text("Button 3 : Insert Sort");
	ImGui::Text("Button 4 : Merge Sort");
	ImGui::Text("Button 5 : Quick Sort");
	ImGui::Text("Button 6 : Heap Sort");
	ImGui::SliderFloat("Camera Speed", &m_camera->m_speed, 1.0f, 20.0f);

	static SORTMODE_TYPE prevMode = m_sortMode;
	prevMode = m_sortMode;
	ImGui::RadioButton("Continue Mode", (int*)&m_sortMode, 0);
	ImGui::SameLine();
	ImGui::RadioButton("One Step Mode", (int*)&m_sortMode, 1);
	if (m_sortMode != prevMode)
	{
		m_sorter.reset();
		CreateSorter(m_sortMode);
	}

	if (m_sortMode == SORTMODE_TYPE::CONTINUE && prevMode == SORTMODE_TYPE::ONESTEP)
	{
		ContinueSorter* pContinueSorter = dynamic_cast<ContinueSorter*>(m_sorter.get());
		assert(pContinueSorter);
		pContinueSorter->SetAnimateSec(1.0f);
	}

	m_sorter->UpdateGUI();
}

void Core::Update(float dt)
{
	KeyMgr::Update();

	if (KEYCHECK(ESC, TAP))
	{
		m_sorter.reset();
		exit(0);
	}

	m_camera->Update(dt);

	if (KEYCHECK(SPACE, TAP))
		m_sorter->GenerateElement(m_device, m_context);

	this->UpdateGlobalConst();
		
	m_sorter->Update(m_context, m_globalConst, dt);

	m_ground->Update(m_globalConst, dt);
	m_skyBox->Update(m_globalConst, dt);

}

void Core::FinalUpdate(float dt)
{
	m_sorter->FinalUpdate(m_context, dt);
	m_skyBox->FinalUpdate(m_context, dt);
	m_ground->FinalUpdate(m_context, dt);
	m_camera->FinalUpdate(m_context, m_globalConst);

	m_globalConst.light.lightView = m_globalConst.light.lightView.Transpose();
	m_globalConst.light.lightProj = m_globalConst.light.lightProj.Transpose();
	m_globalConstBuffer->Update(m_context, sizeof(m_globalConst), 1, &m_globalConst);
}

void Core::Render()
{
	float clearColor[4] = { 0.3f,0.3f,1.0f,1.0f };
	m_context->ClearRenderTargetView(m_msaaBuffer.GetRTV(), clearColor);
	m_context->ClearDepthStencilView(m_dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_context->ClearDepthStencilView(m_lightViewDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_context->RSSetViewports(1, &m_viewPort);
	m_context->OMSetRenderTargets(0, nullptr, m_lightViewDSV.Get());
	// m_context->OMSetRenderTargets(1, m_resolveBuffer.GetRTVAddress(), m_lightViewDSV.Get());
	
	CoreBase::SetPSO(Graphics::depthOnlyPSO);
	m_sorter->Render(m_context);
	m_ground->Render(m_context);
	
	m_context->OMSetRenderTargets(1, m_msaaBuffer.GetRTVAddress(), m_dsv.Get());
	m_context->PSSetShaderResources(1, 1, m_lightViewSRV.GetAddressOf());
	CoreBase::SetPSO(Graphics::basicSolidPSO);
	m_sorter->Render(m_context);
	m_ground->Render(m_context);
	
	CoreBase::SetPSO(Graphics::skyBoxSolidPSO);
	m_skyBox->Render(m_context);
	
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_context->ResolveSubresource(m_resolveBuffer.GetTexture(), 0, m_msaaBuffer.GetTexture()
		, 0, DXGI_FORMAT_R16G16B16A16_FLOAT);
	
	m_context->CopyResource(m_backBuffer.GetTexture(), m_resolveBuffer.GetTexture());
}

void Core::UpdateGlobalConst()
{
	m_camera->UpdateGlobalConst(m_globalConst);
	
	m_sorter->UpdateGlobalConst(m_globalConst);

	static Vector3 lightFocusPos = Vector3(0.0f);
	m_globalConst.light.lightDir = lightFocusPos - m_globalConst.light.pos;
	m_globalConst.light.lightDir.Normalize();
	Vector3 firstViewDir = Vector3{ 0.0f,0.0f,1.0f };

	Quaternion rotation = Quaternion::FromToRotation(firstViewDir
		, m_globalConst.light.lightDir);
	m_globalConst.light.lightView = Matrix::CreateTranslation(-m_globalConst.light.pos)
		* Matrix::CreateFromQuaternion(rotation).Invert();
	m_globalConst.light.lightProj = m_globalConst.proj;

}

void Core::CreateSorter(SORTMODE_TYPE sortType)
{
	m_sorter.reset();

	if (sortType == SORTMODE_TYPE::CONTINUE)
		m_sorter = make_unique<ContinueSorter>();
	else if (sortType == SORTMODE_TYPE::ONESTEP)
		m_sorter = make_unique<OneStepSorter>();

	m_sorter->Init(m_device, m_context);
}

void Core::Progress()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Create a window called "Hello, world!" and append into it.
	ImGui::Begin("How To Use");                          
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	this->UpdateGUI();
	this->Update(ImGui::GetIO().DeltaTime);
	this->FinalUpdate(ImGui::GetIO().DeltaTime);
	
	ImGui::End();
	ImGui::Render();
	this->Render();

	m_swapChain->Present(1, 0);
}
