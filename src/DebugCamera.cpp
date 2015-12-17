#include "DebugCamera.h"

#include "InputManager.h"

/*
 *	Construct an instance of the DebugCamera at the origin.
 */
DebugCamera::DebugCamera() : Camera( 0.0f, 0.0f, 0.0f) {}

/*
 * Construct an instance of the DebugCamera at the provided location.
 */
DebugCamera::DebugCamera( float x, float y, float z ) : Camera( x, y, z ) {}

/*
 * Default Destructor
 */
DebugCamera::~DebugCamera() { /* Nothing to do. */ }

/*
 *	Update the position of the DebugCamera based on user input.
 *	@param	dt		The deltaTime value this frame.
 */
void DebugCamera::Update(float dt)
{
	InputManager* pManager = InputManager::Instance();

	// Current speed
	float moveSpeed = m_moveSpeed * dt;
	float rotSpeed = m_rotationSpeed * dt;

	// Speed up when shift is pressed
	if (pManager->IsKeyDown(VK_SHIFT)) { moveSpeed *= 5; }

	// Movement
	if (pManager->IsKeyDown('W')) { MoveRelative(0, 0, moveSpeed); }
	if (pManager->IsKeyDown('S')) { MoveRelative(0, 0, -moveSpeed); }
	if (pManager->IsKeyDown('A')) { MoveRelative(-moveSpeed, 0, 0); }
	if (pManager->IsKeyDown('D')) { MoveRelative(moveSpeed, 0, 0); }
	if (pManager->IsKeyDown('X')) { MoveAbsolute(0, -moveSpeed, 0); }
	if (pManager->IsKeyDown(' ')) { MoveAbsolute(0, moveSpeed, 0); }
	if (pManager->IsKeyDown('Q')) { Rotate(0.0f, -rotSpeed); }
	if (pManager->IsKeyDown('E')) { Rotate(0.0f, rotSpeed); }

	// Handle rotation
	if (pManager->IsMouseDown(MouseButton::LMB))
	{
		POINT   prevMousePos = pManager->GetPreviousMousePos(),
				currMousePos = pManager->GetCurrentMousePos();

		float xDiff = min(m_rotationSpeed, (currMousePos.x - prevMousePos.x) * rotSpeed);
		float yDiff = min(m_rotationSpeed, (currMousePos.y - prevMousePos.y) * rotSpeed);

		Rotate(yDiff, xDiff);
	}
}
