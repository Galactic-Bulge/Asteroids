#ifndef UI_UPDATE_SYSTEM_H
#define UI_UPDATE_SYSTEM_H

#include "System.h"

// Forward Declarations.
class InputManager;
class EventManager;

class UIUpdateSystem : public System<UIUpdateSystem>
{
private:
    InputManager* m_pInput;
    EventManager* m_pEvent;

public:
    /*
     * Construct an instance of the UIUpdateSystem.
     */
    UIUpdateSystem(void);

    /*
     * Destruct the UIUpdateSystem instance.
     */
    virtual ~UIUpdateSystem(void) override;

    /*
     * System Interface.
     */
    virtual void Update(EntityManager* pManager, float dt, float tt) override;
};

#endif
