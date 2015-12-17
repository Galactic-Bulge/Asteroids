#ifndef PARTICLE
#define PARTICLE

#include <DirectXMath.h>

enum ParticleType { ROOT = 1, RESET = 2, STANDARD = 3 };

struct Particle
{
	DirectX::XMFLOAT4 startColor;
	DirectX::XMFLOAT4 midColor;
	DirectX::XMFLOAT4 endColor;
	float size;
	float age;
	float lifeTime;
	float startTime;
	int type;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 acceleration;

	Particle(DirectX::XMFLOAT4 sCol,
		DirectX::XMFLOAT4 mCol,
		DirectX::XMFLOAT4 eCol,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3 vel,
		DirectX::XMFLOAT3 accel,
		float s,
		float a,
		float lt,
		float st,
		int t)
		: startColor(sCol), midColor(mCol), endColor(eCol), position(pos), velocity(vel), acceleration(accel), size(s), age(a), lifeTime(lt), startTime(st), type(t)
	{
	}
};


#endif