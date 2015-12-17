#pragma once

#include <string>

#include <d3d11.h>
#include <vector>

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
	Mesh(const std::string& filename, ID3D11Device* device);
	~Mesh(void);

	ID3D11Buffer* GetVertexBuffer() { return vb; }
	ID3D11Buffer* GetIndexBuffer() { return ib; }
	std::vector<Vertex> GetVertices() { return vertices; }
	int GetVertexCount() { return numVertices; }
	int GetIndexCount() { return numIndices; }

private:
	ID3D11Buffer* vb;
	ID3D11Buffer* ib;

	std::vector<Vertex> vertices;
	int numVertices;
	int numIndices;

	void CreateBuffers(Vertex* vertArray, int numVerts, unsigned int* indexArray, int numIndices, ID3D11Device* device);
};

