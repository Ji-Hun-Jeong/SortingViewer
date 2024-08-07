#pragma once
class Camera
{
public:
	Camera(float fov, float aspect, float nearZ, float farZ);
	void Update(float dt);
	const Matrix& GetViewRow() { return m_view; }
	const Matrix& GetProjRow() { return m_proj; }
	void SetPos(const Vector3& pos) { m_pos = pos; }
	void CalcDir();

private:
	float m_speed;
	Vector3 m_pos;

	Vector3 m_viewDir;
	Vector3 m_upDir;
	Vector3 m_rightDir;
	float m_pitch;
	float m_yaw;

	Matrix m_view;
	Matrix m_proj;

	float m_fov;
	float m_aspect;
	float m_nearZ;
	float m_farZ;

	bool m_toBeActive;
};

