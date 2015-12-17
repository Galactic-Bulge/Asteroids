#ifndef NO_EFFECT_H
#define NO_EFFECT_H

#include "IEffect.h"

class NoEffect : public IEffect
{
public:
	NoEffect(void);
	virtual ~NoEffect(void);
	virtual void RenderEffect(ID3D11ShaderResourceView* srv, ID3D11DeviceContext* pDeviceContext, float dt, float tt) override;
};

#endif
