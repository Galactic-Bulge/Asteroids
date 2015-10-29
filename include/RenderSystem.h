#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "System.h"

class RenderSystem : public System<RenderSystem>
{
    virtual ~RenderSystem(void) {}

    /* System interface. */
    virtual void Update(EntityManager* pManager, float dt, float tt ) override;
};

#endif
