#ifndef CAMERA_H
#define CAMERA_H

// DirectX
#include <DirectXMath.h>
#include "Transform.h"

class Camera
{
private:
    friend class CameraManager;

	float m_xRotation, m_yRotation;							// x and y rotation components

	DirectX::XMFLOAT4X4 m_viewMatrix, m_projectionMatrix;	// The View and Projection matrices

	float m_fov, m_aspectRatio, m_nearPlane, m_farPlane;	// Projection matrix values

	/*
	 *	Recalculate the View matrix based on the current position and rotation.
	 */
	void RecalculateViewMatrix();

    /*
	 *	Update function to be provided by the child class.
	 *	@param	dt		The delta time value this frame.
	 */
	virtual void Update(float dt) {};

protected:
	float m_moveSpeed, m_rotationSpeed;						// Camera's movement and rotation speeds				

public:
	Transform transform;

	// Public Constants
	const static float DEFAULT_FOV;
	const static float DEFAULT_ASPECT_RATIO;
	const static float DEFAULT_NEAR_PLANE;
	const static float DEFAULT_FAR_PLANE;
	const static float DEFAULT_MOVE_SPEED;
	const static float DEFAULT_ROTATION_SPEED;

	/*
	 *	Construct an instance of the Camera at the origin.
	 */
	Camera();

	/*
	 *	Construct an instance of the Camera at the provided (x,y,z) position.
	 */
	Camera(float x, float y, float z);

	/*
	 * Default Destructor
	 */
	virtual ~Camera();

    /*
	 *	Move the Camera relative to its forward direction.
	 *	@param	x		Distance in the x-coordinate.
	 *	@param	y		Distance in the y-coordinate.
	 *	@param	z		Distance in the z-coordinate.
	 */
	void MoveRelative(float x, float y, float z);

	/*
	 *	Move the Camera in world coordinates.
	 *	@param	x		Distance in the x-coordinate.
	 *	@param	y		Distance in the y-coordinate.
	 *	@param	z		Distance in the z-coordinate.
	 */
    void MoveAbsolute(float x, float y, float z);

	/*
	 *	Rotate the Camera along the x and y coordinate axes.
	 *	@param	x		Amount to rotate around the x-axis (in radians).
	 *	@param	y		Amount to rotate around the y-axis (in radains).
	 */
	void Rotate(float x, float y);

	/*
	 *	Recalculate the Perspective projection based on the provided FOV, aspect ratio, near plane, and far plane.
	 *	@param	fov			The field-of-view angle (in radians).
	 *	@param	aspectRatio	The aspect ratio.
	 *  @param	nearPlane	The distance to the near plane.
	 *	@param	farPlane	The distance to the far plane.
	 */
	void UpdateProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);

	// Getters
	const	DirectX::XMFLOAT4X4&	GetViewMatrix()			const { return m_viewMatrix; }
	const	DirectX::XMFLOAT4X4&	GetProjectionMatrix()	const { return m_projectionMatrix; }
			float					GetFOV()				const { return m_fov; }
			float					GetAspectRatio()		const { return m_aspectRatio; }
			float					GetNearPlane()			const { return m_nearPlane; }
			float					GetFarPlane()			const { return m_farPlane; }
			float					GetMovementSpeed()		const { return m_moveSpeed; }
			float					GetRotationSpeed()		const { return m_rotationSpeed; }

	// Setters
	void SetFOV(float fov);
	void SetAspectRatio(float aspectRatio);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);
	void SetMovementSpeed(float movementSpeed);
	void SetRotationSpeed(float rotationSpeed);
};

#endif
