#include "InputControllerSystem.h"

// Managers
#include "EntityManager.h" // Need to to include this to access EntityManager::* functions.
#include "InputManager.h"
#include "CameraManager.h"

// ECS
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "InputComponent.h"

// DirectX
#include <DirectXMath.h>
#include <SimpleMath.h>

InputControllerSystem::InputControllerSystem(void)
{
	m_pCamera = CameraManager::Instance();
	m_pInput = InputManager::Instance();
}

InputControllerSystem::~InputControllerSystem(void)
{
	/* Nothing to do. */
}

void InputControllerSystem::Update(EntityManager* pManager, float dt, float tt)
{
	using namespace DirectX;
	using namespace SimpleMath;

	if (m_pCamera->IsDebugActive()) return; //Don't overlap controls

	float horiz = static_cast<float>((m_pInput->IsKeyDown('A') ? -1 : 0) + (m_pInput->IsKeyDown('D') ? 1 : 0));
	float vert = static_cast<float>((m_pInput->IsKeyDown('S') ? -1 : 0) + (m_pInput->IsKeyDown('W') ? 1 : 0));

	float playableWidth = 5;
	float playableHeight = 4;

	for (auto& entity : pManager->EntitiesWithComponents<InputComponent, PhysicsComponent, TransformComponent>())
	{
		TransformComponent* pTransform = pManager->GetComponent<TransformComponent>(entity);
		Transform& t = pTransform->transform;

		InputComponent* inputCom = pManager->GetComponent<InputComponent>(entity);
		PhysicsComponent* pPhysics = pManager->GetComponent<PhysicsComponent>(entity);

		DirectX::XMFLOAT3 pos = t.GetTranslation();
		if ((pos.x < -playableWidth / 2 && horiz < 0) ||
			(pos.x > playableWidth / 2 && horiz > 0)) {
			horiz = 0;
			pPhysics->velocity.x = 0;
		}
		if ((pos.y < -playableHeight / 2 && vert < 0) ||
			(pos.y > playableHeight / 2 && vert > 0)) {
			vert = 0;
			pPhysics->velocity.y = 0;
		}

		Vector3 rot = Vector3(t.GetRotationEuler().x, t.GetRotationEuler().y, t.GetRotationEuler().z);
		Vector3 accel = Vector3(horiz, vert, 0);// horizAdd + vertAdd;
		accel.Normalize();
		accel *= inputCom->movementSpeed;

		pPhysics->acceleration = XMFLOAT3(accel.x, accel.y, 0);

		float maxRot = 3.14f / 8.0f;
		//Rotating to action
		if ((abs(rot.z) < maxRot || rot.z*horiz > 0) && horiz != 0) {
			pPhysics->rotationalAcceleration = XMFLOAT3(0, 0, -horiz * 240.0f);
		}
		else if (horiz != 0) { //Still held down, but at maximum: stop
			pPhysics->rotationalAcceleration = XMFLOAT3(0, 0, 0);
		}
		else { //Return back to zero
			pPhysics->rotationalAcceleration = XMFLOAT3(0, 0, rot.z / maxRot  * -80.0f);
		}

		if ((abs(rot.x) < maxRot || rot.x*vert > 0) && vert != 0) {
			pPhysics->rotationalAcceleration = XMFLOAT3(-vert * 240.0f,0, pPhysics->rotationalAcceleration.z);
		}
		else if (vert != 0) { //Still held down, but at maximum: stop
			pPhysics->rotationalAcceleration = XMFLOAT3(0, 0, pPhysics->rotationalAcceleration.z);
		}
		else { //Return back to zero
			pPhysics->rotationalAcceleration = XMFLOAT3(rot.x / maxRot  * -80.0f,0, pPhysics->rotationalAcceleration.z);
		}

		t.SetRotation(rot.x, 0, rot.z); //Cleaning issues caused by euler rotation
	}
}
