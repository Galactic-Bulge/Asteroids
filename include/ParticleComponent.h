#ifndef PARTICLE_COMPONENT
#define PARTICLE_COMPONENT

#include "Component.h"
#include "ParticleGenerator.h"

class ParticleComponent : public Component<ParticleComponent>
{
private:
	std::vector<ParticleGenerator*> generators;

public:
	virtual ~ParticleComponent() override
	{
		for (ParticleGenerator* pGen : generators) {
			delete pGen;
			pGen = 0;
		}
	}

	std::vector<ParticleGenerator*> GetGenerators() {
		return generators;
	}

	void AddGenerator(ParticleGenerator* generator) {
		generators.push_back(generator);
	}
};

#endif