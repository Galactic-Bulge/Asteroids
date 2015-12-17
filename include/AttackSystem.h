#ifndef ATTACK_SYSTEM
#define ATTACK_SYSTEM

#include "System.h"
#include "CollisionSphere.h"
#include "ParticleGenerator.h"

class AttackSystem : public System<AttackSystem>
{
private:
	CollisionSphere projectileCollider;
	ParticleGenerator* projectileGenerator;

public:
    AttackSystem();
	virtual ~AttackSystem(void) {}
	virtual void Update(EntityManager* pManager, float dt, float tt) override;

	void FireProjectile(EntityManager* pManager);
};

#endif
