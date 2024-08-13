#include "pch.h"
#include "Camera.h"
#include "KeyMgr.h"

Camera::Camera(float fov, float aspect, float nearZ, float farZ)
	: m_fov(fov* XM_PI / 180.0f), m_aspect(aspect)
	, m_nearZ(nearZ), m_farZ(farZ), m_speed(2.0f), m_toBeActive(false)
	, m_pitch(0.0f), m_yaw(0.0f)
{
}

void Camera::Update(float dt)
{
	if (KEYCHECK(F, TAP))
		m_toBeActive = !m_toBeActive;
	if (m_toBeActive)
	{
		CalcDir();
		if (KEYCHECK(A, HOLD))
			m_pos += -m_rightDir * m_speed * dt;
		if (KEYCHECK(D, HOLD))
			m_pos += m_rightDir * m_speed * dt;
		if (KEYCHECK(W, HOLD))
			m_pos += m_viewDir * m_speed * dt;
		if (KEYCHECK(S, HOLD))
			m_pos += -m_viewDir * m_speed * dt;
		if (KEYCHECK(E, HOLD))
			m_pos += m_upDir * m_speed * dt;
		if (KEYCHECK(Q, HOLD))
			m_pos += -m_upDir * m_speed * dt;
	}
	m_view = Matrix::CreateTranslation(-m_pos)
		* Matrix::CreateRotationY(-m_yaw)
		* Matrix::CreateRotationX(m_pitch);
	
	m_proj = DirectX::XMMatrixPerspectiveFovLH(m_fov, m_aspect, m_nearZ, m_farZ);
}

void Camera::UpdateGlobalConst(GlobalConst& globalConst)
{
	globalConst.view = m_view;
	globalConst.proj = m_proj;
	globalConst.viewProj = globalConst.view * globalConst.proj;
	globalConst.invViewProj = globalConst.viewProj.Invert();
	globalConst.eyePos = m_pos;

	Vector3 viewTrans = globalConst.view.Translation();
	globalConst.skyBoxView = globalConst.view * Matrix::CreateTranslation(-viewTrans);

}

void Camera::FinalUpdate(ComPtr<ID3D11DeviceContext>& context, GlobalConst& globalConst)
{
	globalConst.view = globalConst.view.Transpose();
	globalConst.proj = globalConst.proj.Transpose();
	globalConst.viewProj = globalConst.viewProj.Transpose();
	globalConst.invViewProj = globalConst.invViewProj.Transpose();
	globalConst.skyBoxView = globalConst.skyBoxView.Transpose();
}

void Camera::CalcDir()
{
	Vector2 mouseNDCPos = KeyMgr::GetMouseNDCPos();
	m_yaw = mouseNDCPos.x * XM_PI;
	m_pitch = mouseNDCPos.y * XM_PI / 2.0f;

	m_viewDir = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f),
		Matrix::CreateRotationX(-m_pitch)
		* Matrix::CreateRotationY(m_yaw));
	m_viewDir.Normalize();

	m_upDir = Vector3::Transform(Vector3(0.0f, 1.0f, 0.0f),
		Matrix::CreateRotationX(-m_pitch)
		* Matrix::CreateRotationY(m_yaw));
	m_upDir.Normalize();

	m_rightDir = m_upDir.Cross(m_viewDir);
	m_rightDir.Normalize();
}
