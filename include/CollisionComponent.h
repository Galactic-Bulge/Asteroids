#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "Component.h"
#include "CollisionSphere.h"

struct CollisionComponent : public Component<CollisionComponent>
{
	CollisionSphere collider;

	CollisionComponent(Mesh& m, DirectX::XMFLOAT3& p, float s)
		: collider(m, p, s)
	{
        /* Nothing to do */
	}

	CollisionComponent(float r, DirectX::XMFLOAT3& p)
		: collider(r, p)
	{
        /* Nothing to do */
	}

	virtual ~CollisionComponent(void) {}
};

#endif
