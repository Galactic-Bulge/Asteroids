#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "System.h"

class InputControllerSystem : public System<InputControllerSystem>
{
public:
	virtual ~InputControllerSystem(void) {}

	/* System interface. */
	virtual void Update(EntityManager* pManager, float dt, float tt) override;
};

#endif