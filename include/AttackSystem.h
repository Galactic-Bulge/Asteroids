#ifndef ATTACK_SYSTEM
#define ATTACK_SYSTEM

#include "System.h"
#include "CollisionSphere.h"

class AttackSystem : public System<AttackSystem>
{
private:
	CollisionSphere projectileCollider;

public:
    AttackSystem(void);
	virtual ~AttackSystem(void) {}
	virtual void Update(EntityManager* pManager, float dt, float tt) override;

	void FireProjectile(EntityManager* pManager);
};

#endif
