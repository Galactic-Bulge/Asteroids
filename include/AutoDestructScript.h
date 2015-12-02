#ifndef AUTO_DESTRUCT_SCRIPT_H
#define AUTO_DESTRUCT_SCRIPT_H

// Managers
#include "EventManager.h"
#include "EntityManager.h"

// Components
#include "ScriptComponent.h"
#include "Transform.h"
#include "TransformComponent.h"

#include "GameEntity.h"

class AutoDestructScript : public IScript
{
private :
	float destroyAtZ;

public:
	AutoDestructScript(float destroyAtZ)
        : destroyAtZ(destroyAtZ)
    {
        /* Nothing to do. */
    }

	virtual ~AutoDestructScript(void) {}

	virtual void Update(GameEntity entity, float dt, float tt) override
    {
		EntityManager* pEntity = EntityManager::Instance();

		TransformComponent* pTransform = pEntity->GetComponent<TransformComponent>(entity);
		Transform& t = pTransform->transform;

		if (t.GetTranslation().z <= destroyAtZ && entity.GetTag() == "Asteroid")
        {
			pEntity->Destroy(entity);
			EventManager::Instance()->Fire("AsteroidDestroyed", nullptr);
		}
		else if (t.GetTranslation().z >= destroyAtZ && entity.GetTag() == "Projectile") {
			pEntity->Destroy(entity);
			EventManager::Instance()->Fire("ProjectileDestroyed", nullptr);
		}
	}
};

#endif
