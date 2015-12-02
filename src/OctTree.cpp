#include "OctTree.h"
#include "EntityManager.h"
#include <TransformComponent.h>
#include <CollisionComponent.h>



OctTree::OctTree() {
	maxEntities = 0;
	partitionedEntities = std::vector<std::vector<GameEntity>>();
}

OctTree::OctTree(int mE)
	: maxEntities(mE)
{
	partitionedEntities = std::vector<std::vector<GameEntity>>();
}

void OctTree::Update(EntityManager* eManager, std::vector<GameEntity> collidables) {
	partitionedEntities.clear();

	DirectX::XMFLOAT3 centroid(0, 0, 10);
	Octant initial(centroid, 10, 10, 20);
	eManager->AddOctant(initial);

	PartitionSpace(eManager, collidables, initial);
}

int OctTree::GetMaxEntities() {
	return maxEntities;
}

std::vector<std::vector<GameEntity>> OctTree::GetPartitionedEntities() {
	return partitionedEntities;
}

void OctTree::PartitionSpace(EntityManager* pManager, std::vector<GameEntity> entities, Octant o) {
	std::vector<GameEntity> containing;

	for (auto& gE : entities) {
		CollisionSphere entityColSphere = pManager->GetComponent<CollisionComponent>(gE)->collider;

		if (o.Contains(entityColSphere)) {
			containing.push_back(gE);
		}
	}


	if (containing.size() > maxEntities) {
		float halfW = o.halfW;
		float halfH = o.halfH;
		float halfD = o.halfD;
		Octant LUF(DirectX::XMFLOAT3(o.position.x - (halfW / 2), o.position.y + (halfH / 2), o.position.z + (halfD / 2)), halfW, halfH, halfD); // Left-Upper-Front Octant
		Octant RUF(DirectX::XMFLOAT3(o.position.x + (halfW / 2), o.position.y + (halfH / 2), o.position.z + (halfD / 2)), halfW, halfH, halfD); // Right-Upper-Front Octant
		Octant LLF(DirectX::XMFLOAT3(o.position.x - (halfW / 2), o.position.y - (halfH / 2), o.position.z + (halfD / 2)), halfW, halfH, halfD); // Left-Lower-Front Octant
		Octant RLF(DirectX::XMFLOAT3(o.position.x + (halfW / 2), o.position.y - (halfH / 2), o.position.z + (halfD / 2)), halfW, halfH, halfD); // Right-Lower-Front Octant
		Octant LUB(DirectX::XMFLOAT3(o.position.x - (halfW / 2), o.position.y + (halfH / 2), o.position.z - (halfD / 2)), halfW, halfH, halfD); // Left-Upper-Back Octant
		Octant RUB(DirectX::XMFLOAT3(o.position.x + (halfW / 2), o.position.y + (halfH / 2), o.position.z - (halfD / 2)), halfW, halfH, halfD); // Right-Upper-Back Octant
		Octant LLB(DirectX::XMFLOAT3(o.position.x - (halfW / 2), o.position.y - (halfH / 2), o.position.z - (halfD / 2)), halfW, halfH, halfD); // Left-Lower-Back Octant
		Octant RLB(DirectX::XMFLOAT3(o.position.x + (halfW / 2), o.position.y - (halfH / 2), o.position.z - (halfD / 2)), halfW, halfH, halfD); // Right-Lower-Back Octant

		PartitionSpace(pManager, containing, LUF);
		PartitionSpace(pManager, containing, RUF);
		PartitionSpace(pManager, containing, LLF);
		PartitionSpace(pManager, containing, RLF);
		PartitionSpace(pManager, containing, LUB);
		PartitionSpace(pManager, containing, RUB);
		PartitionSpace(pManager, containing, LLB);
		PartitionSpace(pManager, containing, RLB);
	}
	else if (containing.size() > 1) {
		partitionedEntities.push_back(containing);
		pManager->AddOctant(o);
	}
}
