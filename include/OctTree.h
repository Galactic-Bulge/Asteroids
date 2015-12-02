#ifndef OCT_TREE
#define OCT_TREE

#include "GameEntity.h"
#include "EntityManager.h"
#include <DirectXMath.h>
#include "CollisionSphere.h"
#include <vector>
#include <Octant.h>

class OctTree
{
private:
	int maxEntities;										  // Max number of entities per partition
	std::vector<std::vector<GameEntity>> partitionedEntities; // List holding sub-lists of entities in their respective partitions

public:
	OctTree();
	OctTree(int mE);

	void Update(EntityManager* eManager, std::vector<GameEntity> collidables);

	/* Getters */
	int GetMaxEntities();
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