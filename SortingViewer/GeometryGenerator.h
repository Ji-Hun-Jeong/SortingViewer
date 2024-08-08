#pragma once
class GeometryGenerator
{
public:
	static MeshData MakeSphere(float radius, UINT numOfStack, UINT numOfSlice);
	static MeshData MakeTriangle();
	static MeshData MakeSquare();
	static MeshData MakeBox();
private:
	GeometryGenerator() {}
};

