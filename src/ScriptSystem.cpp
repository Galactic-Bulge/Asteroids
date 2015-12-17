#include "ScriptSystem.h"

#include "EntityManager.h" // Need to to include this to access EntityManager::* functions.

// Components
#include "ScriptComponent.h"

// STD
#include <iostream>

void ScriptSystem::Update(EntityManager* pManager, float dt, float tt )
{
    for(auto& entity : pManager->EntitiesWithComponents<ScriptComponent>())
    {
        ScriptComponent* script = pManager->GetComponent<ScriptComponent>(entity);
		script->Update(entity, dt, tt);
    }
}
