#include "Mesh.h"
#include <DirectXMath.h>
#include <vector>
#include <fstream>

using namespace DirectX;

Mesh::Mesh(Vertex* vertArray, int numVerts, unsigned int* indexArray, int nIndices, ID3D11Device* device)
{
	vertices = std::vector<Vertex>(numVerts);
	for (int i = 0; i < numVerts; i++) {
		vertices.push_back(vertArray[i]);
	}
	numVertices = numVerts;
	numIndices = nIndices;

	CreateBuffers(vertArray, numVerts, indexArray, numIndices, device);
}

Mesh::Mesh(const std::string& filename, ID3D11Device* device)
{
	// String to hold a single line
	char chars[512];

	// Vector to hold the verts
	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT3> normals;
	std::vector<XMFLOAT2> uvs;
	std::vector<OBJTriangle> triangles;

	// Amounts
	int numVerts = 0;
	int numNormals = 0;
	int numUVs = 0;
	int numTriangles = 0;

	// File input
	std::ifstream obj(filename);

	// Scan the file for info
	if (obj.is_open())
	{
		while (obj.good())
		{
			// Get the first 2 characters of a line
			obj.getline(chars, 512);
			if (chars[0] == 'v' && chars[1] == 'n') numNormals++;
			else if (chars[0] == 'v' && chars[1] == 't') numUVs++;
			else if (chars[0] == 'v') numVerts++;
			else if (chars[0] == 'f') numTriangles++;
		}
	}

	// Reset position
	obj.clear();
	obj.seekg(0, obj.beg);

	positions.resize(numVerts);
	normals.resize(numNormals);
	uvs.resize(numUVs);
	triangles.resize(numTriangles * 3);

	// Set up counts
	int vertCounter = 0;
	int normalCounter = 0;
	int uvCounter = 0;
	int triangleCounter = 0;

	// Check for successful open
	if (obj.is_open())
	{
		// Still good?
		while (obj.good())
		{
			// Get the line
			obj.getline(chars, 512);

			// Check the type of line
			if (chars[0] == 'v' && chars[1] == 'n')
			{
				sscanf_s(
					chars,
					"vn %f %f %f",
					&normals[normalCounter].x,
					&normals[normalCounter].y,
					&normals[normalCounter].z);
				normalCounter++;
			}
			else if (chars[0] == 'v' && chars[1] == 't')
			{
				sscanf_s(
					chars,
					"vt %f %f",
					&uvs[uvCounter].x,
					&uvs[uvCounter].y);
				uvCounter++;
			}
			else if (chars[0] == 'v')
			{
				sscanf_s(
					chars,
					"v %f %f %f",
					&positions[vertCounter].x,
					&positions[vertCounter].y,
					&positions[vertCounter].z);
				vertCounter++;
			}
			else if (chars[0] == 'f')
			{
				sscanf_s(
					chars,
					"f %d/%d/%d %d/%d/%d %d/%d/%d",
					&triangles[triangleCounter].Position[0],
					&triangles[triangleCounter].UV[0],
					&triangles[triangleCounter].Normal[0],
					&triangles[triangleCounter].Position[1],
					&triangles[triangleCounter].UV[1],
					&triangles[triangleCounter].Normal[1],
					&triangles[triangleCounter].Position[2],
					&triangles[triangleCounter].UV[2],
					&triangles[triangleCounter].Normal[2]);
				triangleCounter++;
			}
		}

		// Close
		obj.close();

		vertices = std::vector<Vertex>(triangleCounter * 3);

		// Make a vector for the verts
		std::vector<Vertex> verts(triangleCounter * 3);
		std::vector<UINT> indices(triangleCounter * 3);

		// Get the verts ready (OBJ indices are 1-based!)
		UINT indexCounter = 0;
		for (int t = 0; t < triangleCounter; t++)
		{
			for (int i = 0; i < 3; i++)
			{
				// Set up verts
				int index = t * 3 + i;
				vertices[index].Position = positions[triangles[t].Position[i] - 1];
				vertices[index].Normal = normals[triangles[t].Normal[i] - 1];
				vertices[index].UV = uvs[triangles[t].UV[i] - 1];

				// Set up indices too
				indices[index] = indexCounter;
				indexCounter++;
			}
		}

		//memcpy(vertices, &verts[0], sizeof(verts));

		numVertices = numVerts;

		// Create the buffers
		CreateBuffers(&vertices[0], triangleCounter * 3, &indices[0], triangleCounter * 3, device);
	}
}


Mesh::~Mesh(void)
{
	vb->Release(); vb = 0;
	ib->Release(); ib = 0;
}


void Mesh::CreateBuffers(Vertex* vertArray, int numVerts, unsigned int* indexArray, int numIndices, ID3D11Device* device)
{
	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * numVerts; // Number of vertices
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertArray;
	device->CreateBuffer(&vbd, &initialVertexData, &vb);


	// Create the index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int) * numIndices; // Number of indices
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indexArray;
	device->CreateBuffer(&ibd, &initialIndexData, &ib);

	// Save the indices
	this->numIndices = numIndices;
}
