#pragma once
struct Vertex
{
	Vector3 pos;
	Vector3 normal;
	Vector2 uv;
	Vector3 color;
};
struct MeshData
{
	vector<Vertex> vertices;
	vector<uint16_t> indices;
};