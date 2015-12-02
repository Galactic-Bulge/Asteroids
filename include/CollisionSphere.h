#ifndef COLLISION_SPHERE
#define COLLISION_SPHERE

// Mesh
#include "Mesh.h"

// DirectX
#include <DirectXMath.h>

class CollisionSphere
{
private:
	DirectX::XMFLOAT3 position;
	float radius;
	bool isColliding;

public:
	CollisionSphere(void) = default;
	CollisionSphere(Mesh& m, DirectX::XMFLOAT3& p, float scale);
	CollisionSphere(float r, DirectX::XMFLOAT3& p);

    // Getters
	DirectX::XMFLOAT3   GetPosition(void) const;
	float               GetRadius(void) const;
	bool                GetIsColliding(void) const;

    // Setters
	bool IsColliding(bool value);
    DirectX::XMFLOAT3 SetPosition(DirectX::XMFLOAT3& value);

	bool CollidesWith(CollisionSphere& other);
};

#endif
