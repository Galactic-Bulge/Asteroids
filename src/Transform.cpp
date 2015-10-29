#include "Transform.h"

// STD
#include <chrono>
#include <functional>
#include <utility>
#include <algorithm>

using namespace DirectX;

Transform::Transform()
    : parent( nullptr )
{
    // Create an id by hashing the current system time
    auto now = std::chrono::system_clock::now();
	std::hash<long long> hash_fn;
	id = hash_fn(now.time_since_epoch().count());

	translation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	scale		= XMFLOAT3( 1.0f, 1.0f, 1.0f );
	rotation	= XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f); // Identity Quaternion
	rotationEuler = XMFLOAT3( 0.0f, 0.0f, 0.0f );
}

Transform::Transform(XMFLOAT3 translation)
    : parent( nullptr )
{
    // Create an id by hashing the current system time
	auto now = std::chrono::system_clock::now();
	std::hash<long long> hash_fn;
	id = hash_fn(now.time_since_epoch().count());

	this->translation = translation;
	scale		= XMFLOAT3(1.0f, 1.0f, 1.0f);
	rotation	= XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f); // Identity Quaternion
	rotationEuler = XMFLOAT3( 0.0f, 0.0f, 0.0f );
}


Transform::~Transform(){ /* Nothing to do. */ }

bool Transform::operator==( const Transform& rhs ) const
{
    return id == rhs.id;
}

bool Transform::operator!=( const Transform& rhs ) const
{
    return !( *this == rhs );
}

void Transform::AddChild( Transform* child )
{
    // Bail if not a valid pointer
    if(!child)
    {
        return;
    }

    // If the child has a parent already, remove it
    if(child->parent)
    {
        child->parent->RemoveChild( child );
    }

    // Add the child
    children.push_back( child );
    child->parent = this;
}

void Transform::RemoveChild( Transform* child )
{
    // Bail if not a vaild pointer
    if(!child || !child->parent)
    {
        return;
    }

    // Bail if we are not this child's parent
    if(*child->parent != *this)
    {
        return;
    }

    // Remove the child
    children.erase
    (
        std::remove_if(children.begin(), children.end(), [child]( Transform* t ){ return child->id == t->id; }),
        children.end()
    );
    child->parent = nullptr;
}

#pragma region Accessors

XMFLOAT4X4 Transform::GetWorldMatrix()
{
    XMVECTOR scalingVector = XMLoadFloat3(&scale);
    XMVECTOR translationVector = XMLoadFloat3(&translation);
    XMVECTOR rotationAxis = XMVectorZero();
    XMVECTOR rotationQuaternion = XMLoadFloat4(&rotation);

    // Iterate and get the cumulative transformations
    Transform* current = parent;
    while( current )
    {
        scalingVector = XMVectorMultiply(scalingVector, XMLoadFloat3(&current->GetScale()));
        translationVector = XMVectorAdd(translationVector, XMLoadFloat3(&current->GetTranslation()));
        rotationQuaternion = XMQuaternionMultiply(rotationQuaternion, XMLoadFloat4(&current->GetRotation()));
        current = current->parent;
    }

    // Return the final result
    XMFLOAT4X4 result;
    XMStoreFloat4x4(&result, XMMatrixTranspose(XMMatrixAffineTransformation(scalingVector, rotationAxis, rotationQuaternion, translationVector)));
    return result;
}

XMFLOAT3 Transform::GetTranslation() const
{
	return translation;
}

XMFLOAT3 Transform::GetScale() const
{
    return scale;
}

XMFLOAT4 Transform::GetRotation() const
{
	return rotation;
}

XMFLOAT3 Transform::GetRotationEuler() const
{
	return rotationEuler;
}

const std::vector<Transform*>& Transform::GetChildren() const
{
	return children;
}

#pragma endregion

#pragma region Setters

void Transform::SetTranslation(XMVECTOR vec)
{
	XMFLOAT3 container;
	XMStoreFloat3(&container, vec);
	translation = XMFLOAT3(container.x, container.y, container.z);
}

void Transform::SetTranslation(XMFLOAT3 vec)
{
	translation = vec;
}

void Transform::SetTranslation(float x, float y, float z)
{
	translation = XMFLOAT3(x, y, z);
}

void Transform::SetRotation(XMVECTOR vec)
{
	XMFLOAT3 container;
	XMStoreFloat3(&container, vec);
	rotationEuler = XMFLOAT3(container.x, container.y, container.z);
    XMStoreFloat4(&rotation, XMQuaternionRotationRollPitchYawFromVector(vec));
}

void Transform::SetRotation(XMFLOAT3 vec)
{
	rotationEuler = vec;
	XMVECTOR rot = XMLoadFloat3(&vec);
    XMStoreFloat4(&rotation, XMQuaternionRotationRollPitchYawFromVector(rot));
}

void Transform::SetRotation(float x, float y, float z)
{
	rotationEuler = XMFLOAT3(x, y, z);
    XMStoreFloat4(&rotation, XMQuaternionRotationRollPitchYaw(x, y, z));
}

void Transform::SetScale(float num)
{
	scale = XMFLOAT3(num, num, num);
}

#pragma endregion

#pragma region Transformation Functions

void Transform::Translate(XMVECTOR vec)
{
    XMVECTOR trans = XMLoadFloat3( &translation );
    XMStoreFloat3(&translation, XMVectorAdd(trans, vec));
}

void Transform::Translate(XMFLOAT3 vec)
{
	translation.x += vec.x;
	translation.y += vec.y;
	translation.z += vec.z;
}

void Transform::Translate(float x, float y, float z)
{
	translation.x += x;
	translation.y += y;
	translation.z += z;
}

void Transform::Rotate(XMVECTOR vec)
{
	XMVECTOR rot = XMLoadFloat3(&rotationEuler);
	XMStoreFloat3(&rotationEuler, XMVectorAdd(rot, vec));
    XMVECTOR newRotation = XMQuaternionMultiply(XMLoadFloat4(&rotation), XMQuaternionRotationRollPitchYawFromVector(vec));
    XMStoreFloat4(&rotation, newRotation);
}

void Transform::Rotate(XMFLOAT3 vec)
{
	rotationEuler.x += vec.x;
	rotationEuler.y += vec.y;
	rotationEuler.z += vec.z;
    XMVECTOR euler = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&vec));
    XMVECTOR newRotation = XMQuaternionMultiply(XMLoadFloat4(&rotation), euler);
    XMStoreFloat4(&rotation, newRotation);
}

void Transform::Rotate(float x, float y, float z)
{
	rotationEuler.x += x;
	rotationEuler.y += y;
	rotationEuler.z += z;
    XMVECTOR newRotation = XMQuaternionMultiply(XMLoadFloat4(&rotation), XMQuaternionRotationRollPitchYaw( x, y, z ));
    XMStoreFloat4(&rotation, newRotation);
}

void Transform::Scale(float num)
{
	scale.x += num;
	scale.y += num;
	scale.z += num;
}

#pragma endregion
