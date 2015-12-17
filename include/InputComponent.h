#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

// Components
#include "Component.h"

struct InputComponent : public Component<InputComponent>
{
    float movementSpeed;

    InputComponent(float speed)
        : movementSpeed(speed)
    {
        /* Nothing to do. */
    }

    virtual ~InputComponent(void){}
};

#endif
