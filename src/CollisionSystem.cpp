#include "CollisionSystem.h"

// Manager
#include "EntityManager.h"
#include "EventManager.h"

// Component
#include "CollisionComponent.h"
#include "TransformComponent.h"

void CollisionSystem::Update(EntityManager* pManager, float dt, float tt)
{
	// Get entities with collision components
	std::vector<GameEntity> collidables = pManager->EntitiesWithComponents<CollisionComponent>();

	// Loop through entities and set 'isColliding' to false before next frame
	for (auto& ge : collidables)
    {
        CollisionComponent* pCollider = pManager->GetComponent<CollisionComponent>(ge);
        TransformComponent* pTransform = pManager->GetComponent<TransformComponent>(ge);

		// Update spheres position to be the same as the entities' it surrounds
		pCollider->collider.SetPosition(pTransform->transform.GetTranslation());
		pCollider->collider.IsColliding(false);
	}

	// Create our oct tree if we havent already
	if (!octTree) octTree = new OctTree(3);

	octTree->Update(pManager, collidables);
	std::vector<std::vector<GameEntity>> partitionedEntities = octTree->GetPartitionedEntities();

	std::vector<GameEntity> toDestroy;
	for (std::size_t i = 0; i < partitionedEntities.size(); i++)
    {
		for (std::size_t j =  0; j < partitionedEntities[i].size(); j++)
        {
			for (std::size_t k = j + 1; k < partitionedEntities[i].size(); k++) {
				// Get current entities and their colliders
				GameEntity e1 = partitionedEntities[i][j];
				GameEntity e2 = partitionedEntities[i][k];
				CollisionComponent* pColliderOne = pManager->GetComponent<CollisionComponent>(e1);
				CollisionComponent* pColliderTwo = pManager->GetComponent<CollisionComponent>(e2);

				if (pColliderOne->collider.CollidesWith(pColliderTwo->collider))
				{
					pColliderOne->collider.IsColliding(true);
					pColliderTwo->collider.IsColliding(true);

					// If it's a projectile hitting an asteroid, delete both
					if ((e1.GetTag() == "Projectile" && e2.GetTag() == "Asteroid") || (e1.GetTag() == "Asteroid" && e2.GetTag() == "Projectile"))
					{
						toDestroy.push_back(e1);
						toDestroy.push_back(e2);
					}
				}
			}
		}
	}

    EventManager* pEvent = EventManager::Instance();
    for(auto& entity : toDestroy)
    {
		pManager->Destroy(entity);

        std::string tag = entity.GetTag();
        if(tag == "Asteroid")
        {
            pEvent->Fire("AsteroidDestroyed", nullptr);
        }
    }
}
