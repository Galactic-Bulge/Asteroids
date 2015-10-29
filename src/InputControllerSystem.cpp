#include "InputControllerSystem.h"

#include "EntityManager.h" // Need to to include this to access EntityManager::* functions.
#include "InputManager.h"

// Components
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "InputComponent.h"

#include "CameraManager.h"

#include "SimpleMath.h"
// DirectX
#include <DirectXMath.h>
void InputControllerSystem::Update(EntityManager* pManager, float dt, float tt)
{
	using namespace DirectX;
	using namespace SimpleMath;

	if (CameraManager::Instance()->IsDebugActive()) return; //Don't overlap controls

	InputManager* pInput = InputManager::Instance();

	float horiz = (pInput->IsKeyDown('A') ? -1 : 0) + (pInput->IsKeyDown('D') ? 1 : 0);
	float vert = (pInput->IsKeyDown('S') ? -1 : 0) + (pInput->IsKeyDown('W') ? 1 : 0);



	for (auto& entity : pManager->EntitiesWithComponents<InputComponent, PhysicsComponent, TransformComponent>())
	{
		TransformComponent* pTransform = pManager->GetComponent<TransformComponent>(entity);
		Transform& t = pTransform->transform;

		InputComponent* inputCom = pManager->GetComponent<InputComponent>(entity);
		PhysicsComponent* pPhysics = pManager->GetComponent<PhysicsComponent>(entity);

		Vector3 rot = Vector3(t.GetRotationEuler().x, t.GetRotationEuler().y, t.GetRotationEuler().z);
		//Vector3 horizAdd = Vector3(cos(rot.z) * horiz, sin(rot.z) * horiz, 0);
		//Vector3 vertAdd  = Vector3(-sin(rot.z) * vert, cos(rot.z) * vert, 0);
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
