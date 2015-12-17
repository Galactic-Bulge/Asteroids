#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "System.h"

// This system is for running small scripts that don't need a whole system on a per-entity basis.

class ScriptSystem : public System<ScriptSystem>
{
public:
    ~ScriptSystem(void){}

    /* System interface. */
    void Update(EntityManager* pManager, float dt, float tt ) override;
};

#endif
