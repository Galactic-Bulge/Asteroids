#pragma once

#include <d3d11.h>

#include "Vertex.h"

struct OBJTriangle
{
	int Position[3];
	int Normal[3];
	int UV[3];
};

class Mesh
{
public:
	Mesh(Vertex* vertArray, int numVerts, unsigned int* indexArray, int numIndices, ID3D11Device* device);
	Mesh(char* objFile, ID3D11Device* device);
	~Mesh(void);

	ID3D11Buffer* GetVertexBuffer() { return vb; }
	ID3D11Buffer* GetIndexBuffer() { return ib; }
	int GetIndexCount() { return numIndices; }

private:
	ID3D11Buffer* vb;
	ID3D11Buffer* ib;
	int numIndices;

	void CreateBuffers(Vertex* vertArray, int numVerts, unsigned int* indexArray, int numIndices, ID3D11Device* device);
};

