#include "pch.h"
#include "GeometryGenerator.h"

MeshData GeometryGenerator::MakeTriangle()
{
    MeshData returnData;
    vector<Vertex>& vertices = returnData.vertices;
    Vertex v;
    v.pos = Vector3(-0.5f, -0.5f, 0.0f);
    v.normal = Vector3(0.0f, 0.0f, -1.0f);
    v.uv = Vector2(0.0f, 0.0f);
    v.color = Vector3(1.0f, 0.0f, 0.0f);
    vertices.push_back(v);

    v.pos = Vector3(0.0f, 0.5f, 0.0f);
    v.normal = Vector3(0.0f, 0.0f, -1.0f);
    v.uv = Vector2(0.0f, 0.0f);
    v.color = Vector3(0.0f, 1.0f, 0.0f);
    vertices.push_back(v);

    v.pos = Vector3(0.5f, -0.5f, 0.0f);
    v.normal = Vector3(0.0f, 0.0f, -1.0f);
    v.uv = Vector2(0.0f, 0.0f);
    v.color = Vector3(0.0f, 0.0f, 1.0f);
    vertices.push_back(v);

    vector<uint16_t>& indices = returnData.indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    return returnData;
}

MeshData GeometryGenerator::MakeSquare()
{
	MeshData returnData;
	vector<Vertex>& vertices = returnData.vertices;
	Vertex v;
	v.pos = Vector3{ -1.0f,-1.0f,0.0f };
	v.normal = Vector3{ 0.0f,0.0f,-1.0f };
	v.uv = Vector2{ 0.0f,1.0f };
	v.color = Vector3(1.0f, 0.0f, 0.0f);
	//vertex.tangent = Vector3{ 1.0f,0.0f,0.0f };
	vertices.push_back(v);

	v.pos = Vector3{ -1.0f,1.0f,0.0f };
	v.normal = Vector3{ 0.0f,0.0f,-1.0f };
	v.uv = Vector2{ 0.0f,0.0f };
	//vertex.tangent = Vector3{ 1.0f,0.0f,0.0f };
	vertices.push_back(v);

	v.pos = Vector3{ 1.0f,1.0f,0.0f };
	v.normal = Vector3{ 0.0f,0.0f,-1.0f };
	v.uv = Vector2{ 1.0f,0.0f };
	//vertex.tangent = Vector3{ 1.0f,0.0f,0.0f };
	vertices.push_back(v);

	v.pos = Vector3{ 1.0f,-1.0f,0.0f };
	v.normal = Vector3{ 0.0f,0.0f,-1.0f };
	v.uv = Vector2{ 1.0f,1.0f };
	//vertex.tangent = Vector3{ 1.0f,0.0f,0.0f };
	vertices.push_back(v);

	vector<uint16_t>& indices = returnData.indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	return returnData;
}
