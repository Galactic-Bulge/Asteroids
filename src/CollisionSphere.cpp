#include "CollisionSphere.h"

CollisionSphere::CollisionSphere(Mesh& m, DirectX::XMFLOAT3& p, float scale)
    : position(p), radius(0), isColliding(false)
{
	// Loop through vertices to find farthest from centroid
	DirectX::XMFLOAT3 current;
	DirectX::XMVECTOR container;
	for (int i = 0; i < m.GetVertexCount(); i += 100)
    {
		current = m.GetVertices()[i].Position;
		container = DirectX::XMLoadFloat3(&current);
		DirectX::XMVectorScale(container, scale);
		container = DirectX::XMVector3Length(container);
		XMStoreFloat3(&current, container); // Stores magnitude in each index

		if (current.x > radius)
        {
			radius = current.x;
		}
	}

	radius *= scale;
}

CollisionSphere::CollisionSphere(float r, DirectX::XMFLOAT3& p)
    : position(p), radius(r), isColliding(false)
{
}

#pragma region Getters

DirectX::XMFLOAT3 CollisionSphere::GetPosition(void) const
{
	return position;
}

float CollisionSphere::GetRadius(void) const
{
	return radius;
}

bool CollisionSphere::GetIsColliding(void) const
{
	return isColliding;
}

#pragma endregion

#pragma region Setters
bool CollisionSphere::IsColliding(bool val)
{
	return isColliding = val;
}

DirectX::XMFLOAT3 CollisionSphere::SetPosition(DirectX::XMFLOAT3& p)
{
	return position = p;
}
#pragma endregion

bool CollisionSphere::CollidesWith(CollisionSphere& other)
{
    DirectX::XMFLOAT3 otherPos = other.GetPosition();
	float xDiff = position.x - otherPos.x;
	float yDiff = position.y - otherPos.y;
	float zDiff = position.z - otherPos.z;
	float distance = sqrtf(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);

    return (distance < radius + other.GetRadius());
}
