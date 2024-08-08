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
};

struct GlobalConst
{
	Matrix view;
	Matrix proj;
	float maxHeight;
	Vector3 dummy;
};

static_assert(sizeof(MeshConst) % 16 == 0, "MeshConst Size Check");
static_assert(sizeof(GlobalConst) % 16 == 0, "GlobalConst Size Check");