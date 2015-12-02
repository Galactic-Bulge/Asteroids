#include "Camera.h"

using namespace DirectX;

const float Camera::DEFAULT_FOV = 0.25f * XM_PI;
const float Camera::DEFAULT_ASPECT_RATIO = 4.0f / 3.0f;
const float Camera::DEFAULT_NEAR_PLANE = 0.1f;
const float Camera::DEFAULT_FAR_PLANE = 300.0f;
const float Camera::DEFAULT_MOVE_SPEED = 5.0f;
const float Camera::DEFAULT_ROTATION_SPEED = 5.0f;

/*
 *	Construct an instance of the Camera at the provided (x,y,z) position.
 */
Camera::Camera(float x, float y, float z)
	: m_xRotation(0.0f), m_yRotation(0.0f), m_moveSpeed(Camera::DEFAULT_MOVE_SPEED), m_rotationSpeed(Camera::DEFAULT_ROTATION_SPEED)
{
	transform = Transform(XMFLOAT3(x, y, z));
	RecalculateViewMatrix();
	UpdateProjectionMatrix(Camera::DEFAULT_FOV, Camera::DEFAULT_ASPECT_RATIO, Camera::DEFAULT_NEAR_PLANE, Camera::DEFAULT_FAR_PLANE);
}

/*
 *	Construct an instance of the Camera at the origin.
 */
Camera::Camera() : Camera(0.0f, 0.0f, 0.0f) {}

/*
 * Default Destructor
 */
Camera::~Camera() { /* Nothing to do. */ }

#pragma region Movement functions

/*
 *	Move the Camera relative to its forward direction.
 *	@param	x		Distance in the x-coordinate.
 *	@param	y		Distance in the y-coordinate.
 *	@param	z		Distance in the z-coordinate.
 */
void Camera::MoveRelative(float x, float y, float z)
{
	// Rotate the desired movement vector and move in that direction
	XMVECTOR dir = XMVector3Rotate(XMVectorSet(x, y, z, 0), XMLoadFloat4(&transform.GetRotation()));
	transform.Translate(dir);

	// Update the View matrix
	RecalculateViewMatrix();
}

/*
 *	Move the Camera in world coordinates.
 *	@param	x		Distance in the x-coordinate.
 *	@param	y		Distance in the y-coordinate.
 *	@param	z		Distance in the z-coordinate.
 */
void Camera::MoveAbsolute(float x, float y, float z)
{
	transform.Translate(x, y, z);

	// Update the View matrix
	RecalculateViewMatrix();
}

/*
 *	Rotate the Camera along the x and y coordinate axes.
 *	@param	x		Amount to rotate around the x-axis (in radians).
 *	@param	y		Amount to rotate around the y-axis (in radains).
 */
void Camera::Rotate(float x, float y)
{
    // Adjust the current rotation
	m_xRotation += x;
	m_yRotation += y;

	// Clamp the x-component between PI/2 and -PI/2
	if (m_xRotation >= XM_PIDIV2 )
	{
		m_xRotation = XM_PIDIV2;
	}
	else if (m_xRotation <= -XM_PIDIV2 )
	{
		m_xRotation = -XM_PIDIV2;
	}

	// Wrap the y-component at 2 * PI and -2 * PI
	if (m_yRotation > XM_2PI )
	{
		m_yRotation = -XM_2PI;
	}
	else if (m_yRotation < -XM_2PI )
	{
		m_yRotation = XM_2PI;
	}

	transform.SetRotation(m_xRotation, m_yRotation, 0.0f);

	// Update the View matrix
	RecalculateViewMatrix();
}
#pragma endregion

#pragma region Setters
void Camera::SetFOV(float fov)
{
	UpdateProjectionMatrix(fov, m_aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::SetAspectRatio(float aspectRatio)
{
	UpdateProjectionMatrix(m_fov, aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::SetNearPlane(float nearPlane)
{
	UpdateProjectionMatrix(m_fov, m_aspectRatio, nearPlane, m_farPlane);
}

void Camera::SetFarPlane(float farPlane)
{
	UpdateProjectionMatrix(m_fov, m_aspectRatio, m_nearPlane, farPlane);
}

void Camera::SetMovementSpeed(float movementSpeed)
{
	m_moveSpeed = movementSpeed;
}

void Camera::SetRotationSpeed(float rotationSpeed)
{
	m_rotationSpeed = rotationSpeed;
}
#pragma endregion

/*
 *	Recalculate the View matrix based on the current position and rotation.
 */
void Camera::RecalculateViewMatrix()
{
	XMVECTOR	position = XMLoadFloat3(&transform.GetTranslation()),
		        rotation = XMLoadFloat4(&transform.GetRotation()),
		        forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
		        up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
		        dir = XMVector3Rotate(forward, rotation);

	XMMATRIX view = XMMatrixLookToLH(position, dir, up);
	XMStoreFloat4x4(&m_viewMatrix, XMMatrixTranspose(view));
}

/*
 *	Recalculate the Perspective projection based on the provided FOV, aspect ratio, near plane, and far plane.
 *	@param	fov			The field-of-view angle (in radians).
 *	@param	aspectRatio	The aspect ratio.
 *  @param	nearPlane	The distance to the near plane.
 *	@param	farPlane	The distance to the far plane.
 */
void Camera::UpdateProjectionMatrix( float fov, float aspectRatio, float nearPlane, float farPlane )
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane);
	XMStoreFloat4x4(&m_projectionMatrix, XMMatrixTranspose(P));

	m_fov = fov;
	m_aspectRatio = aspectRatio;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
}
