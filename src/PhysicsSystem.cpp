#include "PhysicsSystem.h"

#include "EntityManager.h" // Need to to include this to access EntityManager::* functions.

// Components
#include "TransformComponent.h"
#include "PhysicsComponent.h"

// DirectX
#include <DirectXMath.h>

void PhysicsSystem::Update(EntityManager* pManager, float dt, float tt )
{
    using namespace DirectX;

    for(auto& entity : pManager->EntitiesWithComponents<PhysicsComponent, TransformComponent>())
    {
        TransformComponent* pTransform = pManager->GetComponent<TransformComponent>(entity);
        Transform& t = pTransform->transform;

        PhysicsComponent* pPhysics = pManager->GetComponent<PhysicsComponent>(entity);

		//Translation
		XMVECTOR velocity = XMLoadFloat3(&pPhysics->velocity);
		XMVECTOR acceleration = XMLoadFloat3(&pPhysics->acceleration);

		float drag = (1 - pPhysics->drag);
		XMVECTOR velocityLoss = XMLoadFloat3(& XMFLOAT3(pPhysics->velocity.x * drag, pPhysics->velocity.y * drag, pPhysics->velocity.z * drag));

		velocity = XMVectorAdd(velocity, XMVectorScale(acceleration, dt));
		velocity = XMVectorSubtract(velocity, XMVectorScale(velocityLoss,dt));
		velocity = XMVectorScale(velocity, pPhysics->drag);
		t.Translate(XMVectorScale(velocity, dt));

		//Rotation
		XMVECTOR rotVelocity = XMLoadFloat3(&pPhysics->rotationalVelocity);
		XMVECTOR rotAcceleration = XMLoadFloat3(&pPhysics->rotationalAcceleration);
		float rotDrag = (1 - pPhysics->rotationalDrag);
		XMVECTOR rotVelocityLoss = XMLoadFloat3(&XMFLOAT3(pPhysics->rotationalVelocity.x * rotDrag, pPhysics->rotationalVelocity.y * rotDrag, pPhysics->rotationalVelocity.z * rotDrag));

		rotVelocity = XMVectorAdd(rotVelocity, XMVectorScale(rotAcceleration, dt));
		rotVelocity = XMVectorSubtract(rotVelocity, XMVectorScale(rotVelocityLoss, dt));
		rotVelocity = XMVectorScale(rotVelocity, pPhysics->rotationalDrag);
		t.Rotate(XMVectorScale(rotVelocity, dt));

		//Saving
        XMStoreFloat3(&pPhysics->velocity, velocity);
		XMStoreFloat3(&pPhysics->acceleration, acceleration);
		XMStoreFloat3(&pPhysics->rotationalVelocity, rotVelocity);
		XMStoreFloat3(&pPhysics->rotationalAcceleration, rotAcceleration);
    }
}
