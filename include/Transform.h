#ifndef TRANSFORM_H
#define TRANSFORM_H

// DirectX
#include <DirectXMath.h>

// STD
#include <vector>

class Transform
{
private:
	long long id;                       // Unique transform id
    Transform* parent;                  // Parent transform
    std::vector<Transform*> children;   // Child transforms
    DirectX::XMFLOAT3 translation;      // (X,Y,Z) coordinates
    DirectX::XMFLOAT3 scale;            // (X,Y,Z) scale
    DirectX::XMFLOAT4 rotation;         // Rotation Quaternion
    DirectX::XMFLOAT3 rotationEuler;    // Rotation Euler

public:
	Transform();
	Transform(DirectX::XMFLOAT3 translation);
	~Transform();

    /* Boolean operators */
    bool operator==( const Transform& rhs ) const;
    bool operator!=( const Transform& rhs ) const;

    /* Add/Remove child Transforms */
    void AddChild( Transform* child );
    void RemoveChild( Transform* transform );

	/* Accessors */
	DirectX::XMFLOAT4X4             GetWorldMatrix();
	DirectX::XMFLOAT3               GetTranslation()    const;
	DirectX::XMFLOAT3               GetScale()          const;
	DirectX::XMFLOAT4               GetRotation()       const;
	DirectX::XMFLOAT3               GetRotationEuler()  const;
	const std::vector<Transform*>&  GetChildren()       const;

	/* Setters */
	void SetTranslation(DirectX::XMVECTOR vec);
	void SetTranslation(DirectX::XMFLOAT3 vec);
	void SetTranslation(float x, float y, float z);

	void SetRotation(DirectX::XMVECTOR vec);
	void SetRotation(DirectX::XMFLOAT3 vec);
	void SetRotation(float x, float y, float z);

	void SetScale(float num);

	/* Transformation Functions */
	void Translate(DirectX::XMVECTOR vec);
	void Translate(DirectX::XMFLOAT3 vec);
	void Translate(float x, float y, float z);

	void Rotate(DirectX::XMVECTOR vec);
	void Rotate(DirectX::XMFLOAT3 vec);
	void Rotate(float x, float y, float z);

	void Scale(float num);
};

#endif
