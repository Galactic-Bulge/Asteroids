#ifndef PARTICLE
#define PARTICLE

#include <DirectXMath.h>

enum ParticleType { ROOT = 1, RESET = 2, STANDARD = 3 };

struct Particle
{
	DirectX::XMFLOAT4 startColor;
	DirectX::XMFLOAT4 midColor;
	DirectX::XMFLOAT4 endColor;
	float startSize;
	float midSize;
	float endSize;
	float age;
	int   type;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 acceleration;

	Particle(DirectX::XMFLOAT4 sCol,
		DirectX::XMFLOAT4 mCol,
		DirectX::XMFLOAT4 eCol,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3 vel,
		DirectX::XMFLOAT3 accel,
		float sSize,
		float mSize,
		float eSize,
		float a,
		int   t)
		: startColor(sCol), midColor(mCol), endColor(eCol), position(pos), velocity(vel), acceleration(accel), startSize(sSize), midSize(mSize), endSize(eSize), age(a), type(t)
	{
	}
};


#endif