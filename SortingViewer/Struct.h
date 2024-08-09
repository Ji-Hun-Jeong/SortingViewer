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

struct MeshConst
{
	Matrix world;
	int stdElement = false;
	int cmpElement1 = false;
	int cmpElement2 = false;
	int findPos = false;
};

struct GlobalConst
{
	Matrix view;
	Matrix proj;
	Matrix viewProj;
	float maxHeight;
	Vector3 eyePos;
	float maxWidth;
	Vector3 dummy;
};

static_assert(sizeof(MeshConst) % 16 == 0, "MeshConst Size Check");
static_assert(sizeof(GlobalConst) % 16 == 0, "GlobalConst Size Check");