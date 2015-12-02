#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "System.h"

class PhysicsSystem : public System<PhysicsSystem>
{
private:
	bool paused = false;
public:
    virtual ~PhysicsSystem(void){}

    /* System interface. */
    virtual void Update(EntityManager* pManager, float dt, float tt ) override;

};

#endif
