#ifndef PARTICLE_MANAGER
#define PARTICLE_MANAGER

#include "Singleton.h"
#include "ResourceManager.h"
#include "ParticleGenerator.h"
#include "CameraManager.h"
#include <vector>


class ParticleManager : public Singleton<ParticleManager>
{
private:
	ID3D11Texture1D*			randomTexture;
	ID3D11ShaderResourceView*	randomSRV;
	SimpleGeometryShader*		generatorGS;

public:
	ParticleManager() {
		ResourceManager* rManager = ResourceManager::Instance();
		ID3D11Device* device = rManager->GetDevice();
		ID3D11DeviceContext* deviceContext = rManager->GetDeviceContext();

		
	}

	~ParticleManager() {
		randomTexture->Release();
		randomSRV->Release();
	}

	void Update(float dt, float tt) {
		
	}

	void AddGenerator(ParticleGenerator generator) 
	{ 
		generators.push_back(generator); 
	}
	std::vector<ParticleGenerator> GetGenerators() { return generators; }
	ID3D11Texture1D* GetRandomTexture() { return randomTexture; }
	ID3D11ShaderResourceView* GetRandomSRV() { return randomSRV; }
};

#endif