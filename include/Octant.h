#ifndef OCTANT
#define OCTANT

#include <CollisionSphere.h>
#include <DirectXMath.h>

/*
* Contains necessary data for constructing an Octant.
*/
struct Octant {
	DirectX::XMFLOAT3 position; // XYZ coordinate in space
	float halfW;				// Half width of octant
	float halfH;				// Half height of octant
	float halfD;				// Half depth of octant

	Octant(DirectX::XMFLOAT3 p, float w, float h, float d)
		: position(p), halfW(w / 2), halfH(h / 2), halfD(d / 2)
	{}

	bool Contains(CollisionSphere sphere) {
		DirectX::XMFLOAT3 pos = sphere.GetPosition();
		float r = sphere.GetRadius();

		if (pos.x >= position.x - halfW && pos.x <= position.x + halfW &&
			pos.y >= position.y - halfH && pos.y <= position.y + halfH &&
			pos.z >= position.z - halfD && pos.z <= position.z + halfD)
		{
			return true;
		}

		return false;
	}
};
#endif