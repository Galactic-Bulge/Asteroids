#pragma once

#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include "SimpleShader.h"

#include "Mesh.h"
#include "DebugCamera.h"
#include "GameEntity.h"

#include <vector>

// Include run-time memory checking in debug builds, so 
// we can be notified of memory leaks
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// --------------------------------------------------------
// Game class which extends the base DirectXGameCore class
// --------------------------------------------------------
class MyDemoGame : public DirectXGameCore
{
public:
	MyDemoGame(HINSTANCE hInstance);
	~MyDemoGame();

	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float deltaTime, float totalTime);
	void DrawScene(float deltaTime, float totalTime);

private:
    // Input and mesh swapping
    bool prevSpaceBar;
    unsigned int currentEntity;

    // Keep track of "stuff"
    std::vector<Mesh*> meshes;
    std::vector<GameEntity*> entities;

	// Initialization for our "game" demo - Feel free to
	// expand, alter, rename or remove these once you
	// start doing something more advanced!
	void CreateGeometry();
	void LoadShaders();
	void CreateMatrices();

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

    // Textures
    ID3D11ShaderResourceView* diffuseTexture;
    ID3D11ShaderResourceView* rustTexture;
    ID3D11ShaderResourceView* specMapTexture;

    ID3D11SamplerState* samplerState;

	// Basic debug camera
	Camera* camera;
};
