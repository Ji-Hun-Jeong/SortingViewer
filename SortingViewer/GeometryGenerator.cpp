#include "pch.h"
#include "GeometryGenerator.h"

MeshData GeometryGenerator::MakeSphere(float radius, UINT numOfStack, UINT numOfSlice)
{
	MeshData meshData;
	auto& vertices = meshData.vertices;
	Vector3 startPos = Vector3{ 0.0f, -radius, 0.0f };
	const float xTheta = -2 * XM_PI / float(numOfSlice);
	const float yTheta = -XM_PI / float(numOfStack);
	for (UINT i = 0; i <= numOfStack; ++i)
	{
		Vector3 startOfRotation =
			Vector3::Transform(startPos, Matrix::CreateRotationZ(yTheta * i));
		for (UINT j = 0; j <= numOfSlice; ++j)
		{
			Vertex v;
			v.pos =
				Vector3::Transform(startOfRotation, Matrix::CreateRotationY(xTheta * float(j)));

			v.normal = v.pos;
			v.normal.Normalize();
			Vector3 biTangent = Vector3(0.0f, 1.0f, 0.0f);

			Vector3 normalOrth =
				v.normal - biTangent.Dot(v.normal) * v.normal;
			normalOrth.Normalize();

			//v.tangent = biTangent.Cross(normalOrth);
			//v.tangent.Normalize();
			v.uv = Vector2(float(j) / numOfSlice, 1.0f - float(i) / numOfStack) * 2;
			vertices.push_back(v);
		}
	}
	auto& indices = meshData.indices;
	int yOffSet = numOfSlice + 1;
	for (UINT i = 0; i < numOfStack; ++i)
	{
		int startPoint = i * yOffSet;
		for (UINT j = 0; j < numOfSlice; ++j)
		{
			indices.push_back(startPoint + j);
			indices.push_back(startPoint + yOffSet + j);
			indices.push_back(startPoint + yOffSet + j + 1);

			indices.push_back(startPoint + j);
			indices.push_back(startPoint + yOffSet + j + 1);
			indices.push_back(startPoint + j + 1);
		}
	}
	return meshData;
}
MeshData GeometryGenerator::MakeTriangle()
{
	MeshData returnData;
	vector<Vertex>& vertices = returnData.vertices;
	Vertex v;
	v.pos = Vector3(-0.5f, -0.5f, 0.0f);
	v.normal = Vector3(0.0f, 0.0f, -1.0f);
	v.uv = Vector2(0.0f, 0.0f);
	vertices.push_back(v);

	v.pos = Vector3(0.0f, 0.5f, 0.0f);
	v.normal = Vector3(0.0f, 0.0f, -1.0f);
	v.uv = Vector2(0.0f, 0.0f);
	vertices.push_back(v);

	v.pos = Vector3(0.5f, -0.5f, 0.0f);
	v.normal = Vector3(0.0f, 0.0f, -1.0f);
	v.uv = Vector2(0.0f, 0.0f);
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

MeshData GeometryGenerator::MakeBox()
{
	vector<Vector3> positions;
	vector<Vector3> normals;
	vector<Vector2> texcoords; // ÅØ½ºÃç ÁÂÇ¥

	// À­¸é
	positions.push_back(Vector3(-1.0f, 1.0f, -1.0f));
	positions.push_back(Vector3(-1.0f, 1.0f, 1.0f));
	positions.push_back(Vector3(1.0f, 1.0f, 1.0f));
	positions.push_back(Vector3(1.0f, 1.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));


	// ¾Æ·§¸é
	positions.push_back(Vector3(-1.0f, -1.0f, 1.0f));
	positions.push_back(Vector3(-1.0f, -1.0f, -1.0f));
	positions.push_back(Vector3(1.0f, -1.0f, -1.0f));
	positions.push_back(Vector3(1.0f, -1.0f, 1.0f));
	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));

	// ¾Õ¸é
	positions.push_back(Vector3(-1.0f, -1.0f, -1.0f));
	positions.push_back(Vector3(-1.0f, 1.0f, -1.0f));
	positions.push_back(Vector3(1.0f, 1.0f, -1.0f));
	positions.push_back(Vector3(1.0f, -1.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));

	// µÞ¸é
	positions.push_back(Vector3(1.0f, -1.0f, 1.0f));
	positions.push_back(Vector3(1.0f, 1.0f, 1.0f));
	positions.push_back(Vector3(-1.0f, 1.0f, 1.0f));
	positions.push_back(Vector3(-1.0f, -1.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));

	// ¿ÞÂÊ
	positions.push_back(Vector3(-1.0f, -1.0f, 1.0f));
	positions.push_back(Vector3(-1.0f, 1.0f, 1.0f));
	positions.push_back(Vector3(-1.0f, 1.0f, -1.0f));
	positions.push_back(Vector3(-1.0f, -1.0f, -1.0f));
	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));

	// ¿À¸¥ÂÊ
	positions.push_back(Vector3(1.0f, -1.0f, -1.0f));
	positions.push_back(Vector3(1.0f, 1.0f, -1.0f));
	positions.push_back(Vector3(1.0f, 1.0f, 1.0f));
	positions.push_back(Vector3(1.0f, -1.0f, 1.0f));
	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));

	MeshData meshData;
	for (size_t i = 0; i < positions.size(); i++) 
	{
		Vertex v;
		v.pos = positions[i];
		v.normal = normals[i];
		v.uv = texcoords[i];
		meshData.vertices.push_back(v);
	}

	meshData.indices = {
		0,  1,  2,  0,  2,  3,  // À­¸é
		4,  5,  6,  4,  6,  7,  // ¾Æ·§¸é
		8,  9,  10, 8,  10, 11, // ¾Õ¸é
		12, 13, 14, 12, 14, 15, // µÞ¸é
		16, 17, 18, 16, 18, 19, // ¿ÞÂÊ
		20, 21, 22, 20, 22, 23  // ¿À¸¥ÂÊ
	};

	return meshData;
}
