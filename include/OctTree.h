#ifndef OCT_TREE
#define OCT_TREE

#include "GameEntity.h"
#include "EntityManager.h"
#include <ResourceManager.h>
#include <DirectXMath.h>
#include "CollisionSphere.h"
#include <Octant.h>

// STD
#include <vector>
#include <cstdint>

class OctTree
{
private:
    std::uint32_t maxEntities;								    // Max number of entities per partition
	std::vector<std::vector<GameEntity>> partitionedEntities;   // List holding sub-lists of entities in their respective partitions

public:
	OctTree();
	OctTree(std::uint32_t mE);

	void Update(EntityManager* eManager, std::vector<GameEntity> collidables);

	/* Getters */
    std::uint32_t GetMaxEntities() const;
	std::vector<std::vector<GameEntity>> GetPartitionedEntities();

private:
	/*
	* Remove a Component of type T from the provided GameEntity.
	* @param   pManager			GameEntity Manager
	* @param   entities			List containing entities within partition
	* @param   o				Octant to partition
	*/
	void PartitionSpace(EntityManager* pManager, std::vector<GameEntity> entities, Octant o);
};

#endif
