#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "Component.h"

// DirectX
#include <DirectXMath.h>

struct PhysicsComponent : public Component<PhysicsComponent>
{
    DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 acceleration;
	float drag;
	DirectX::XMFLOAT3 rotationalVelocity;
	DirectX::XMFLOAT3 rotationalAcceleration;
	float rotationalDrag;

    PhysicsComponent(float vx, float vy, float vz, float ax, float ay, float az)
    {
        velocity = DirectX::XMFLOAT3( vx, vy, vz );
        acceleration = DirectX::XMFLOAT3( ax, ay, az );
		drag = 1;
		rotationalVelocity = DirectX::XMFLOAT3(0, 0, 0);
		rotationalAcceleration = DirectX::XMFLOAT3(0, 0, 0);
		rotationalDrag = 1;
    }

    PhysicsComponent(DirectX::XMVECTOR& v, DirectX::XMVECTOR& a)
    {
        DirectX::XMStoreFloat3(&velocity, v);
        DirectX::XMStoreFloat3(&acceleration, a);
		drag = 1;
		rotationalVelocity = DirectX::XMFLOAT3(0, 0, 0);
		rotationalAcceleration = DirectX::XMFLOAT3(0, 0, 0);
		rotationalDrag = 1;
    }

    PhysicsComponent(DirectX::XMFLOAT3 v, DirectX::XMFLOAT3 a)
    {
        velocity = v;
        acceleration = a;
		drag = 1;
		rotationalVelocity = DirectX::XMFLOAT3(0, 0, 0);
		rotationalAcceleration = DirectX::XMFLOAT3(0, 0, 0);
		rotationalDrag = 1;
    }

    ~PhysicsComponent(void){}
};

#endif
