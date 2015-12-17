#ifndef PARTICLE_GENERATOR
#define PARTICLE_GENERATOR

#include <d3d11.h>
#include "Particle.h"
#include "ResourceManager.h"
#include "Transform.h"

struct ParticleGenerator
{
private:
	bool init;

public:
	ID3D11Buffer* particleBuff;
	ID3D11Buffer* readBuff;
	ID3D11Buffer* writeBuff;
	SimpleVertexShader* generatorVS;
	SimpleGeometryShader* generatorGS;

	DirectX::XMFLOAT3 position;
	float lifeTime;
	float spawnRate;

	ParticleGenerator(Particle p, DirectX::XMFLOAT3 pos, float lt, float sr, float numRoots);
	~ParticleGenerator();

	void Update(Transform eTransform, float dt, float tt);
	void SwapBuffers();

	ID3D11Buffer* GetParticles() { return readBuff; }
	void SetPosition(DirectX::XMFLOAT3 pos) { position = pos; }
};

#endif