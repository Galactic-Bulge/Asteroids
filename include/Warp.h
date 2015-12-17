#ifndef WARP_EFFECT_H
#define WARP_EFFECT_H

#include "IEffect.h"

class Warp : public IEffect
{
public:
	Warp();
	virtual ~Warp(void);
	virtual void RenderEffect(ID3D11ShaderResourceView* srv, ID3D11DeviceContext* pDeviceContext, float dt, float tt) override;
	virtual void Enabled(bool value) override;
private:
	float warpTime;
	float timeElapsed;
	float blurWidth;
	float startFOV;
	float endFOV;
};
#endif
