#pragma once
struct Vertex
{
	Vector3 pos;
	Vector3 normal;
	Vector2 uv;
};

struct MeshData
{
	vector<Vertex> vertices;
	vector<uint16_t> indices;
};

struct Light
{
	Vector3 pos;
	float fallOfStart = 1.0f;
	Vector3 strength;
	float fallOfEnd = 100.0f;
	Matrix lightView;
	Matrix lightProj;
};

struct MeshConst
{
	Matrix world;
	int stdElement = false;
	int cmpElement1 = false;
	int cmpElement2 = false;
	int findPos = false;
	int partition = false;
	float metallic = 0.0f;
	float roughness = 0.0f;
	float dummy = 0.0f;
};

struct GlobalConst
{
	Matrix view;
	Matrix proj;
	Matrix viewProj;
	Matrix invViewProj;
	Matrix skyBoxView;

	float sumOfBoxWidth;
	Vector3 eyePos;
	float maxOfBoxHeight;
	Vector3 dummy;

	Light light;
};

static_assert(sizeof(MeshConst) % 16 == 0, "MeshConst Size Check");
static_assert(sizeof(GlobalConst) % 16 == 0, "GlobalConst Size Check");