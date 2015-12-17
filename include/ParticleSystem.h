#ifndef PARTICLE_SYSTEM
#define PARTICLE_SYSTEM

#include <vector>
#include <random>
#include <time.h>
#include "System.h"
#include "EntityManager.h"
#include "CameraManager.h"
#include "ParticleComponent.h"
#include "TransformComponent.h"

class ParticleSystem : public System<ParticleSystem>
{
public:
	std::vector<ParticleGenerator> generators;
	SimpleVertexShader*		  particleVS;
	SimpleGeometryShader*	  particleGS;
	SimplePixelShader*		  particlePS;
	ID3D11Texture1D*		  randomTexture;
	ID3D11ShaderResourceView* randomSRV;

	ParticleSystem(void);
	~ParticleSystem(void);

	virtual void Update(EntityManager* p, float dt, float tt) override;
};

#endif