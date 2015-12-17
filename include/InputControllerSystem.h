#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

// ECS
#include "System.h"

// Forward Declarations
class InputManager;
class CameraManager;

class InputControllerSystem : public System<InputControllerSystem>
{
private:
	CameraManager* m_pCamera;
	InputManager* m_pInput;
public:
	InputControllerSystem(void);
	virtual ~InputControllerSystem(void);

	/* System interface. */
	virtual void Update(EntityManager* pManager, float dt, float tt) override;
};

#endif