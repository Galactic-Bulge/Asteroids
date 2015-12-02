#ifndef COLLISION_SYSTEM
#define COLLISION_SYSTEM

#include "System.h"
#include "OctTree.h"
#include "ResourceManager.h"

class CollisionSystem : public System<CollisionSystem>
{
	OctTree* octTree;

	virtual ~CollisionSystem() { delete octTree; }
	virtual void Update(EntityManager* pManager, float dt, float tt) override;
};

#endif

