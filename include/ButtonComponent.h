#ifndef BUTTON_COMPONENT_H
#define BUTTON_COMPONENT_H

// ECS
#include "Component.h"

// STD
#include <string>

struct ButtonComponent : public Component<ButtonComponent>
{
    bool isEnabled;
    std::string event;

    /*
     * Construct an instance of the ButtonComponent.
     * @param   isEnabled   Boolean indicating if the button is enabled.
     * @param   event       The event to fire when clicked.
     */
    ButtonComponent(bool isEnabled, std::string event)
        : isEnabled(isEnabled), event(event)
    {
        /* Nothing to do */
    }

    virtual ~ButtonComponent(void) override {}
};

#endif
