#ifndef IEFFECT_H
#define IEFFECT_H

// DirectX
#include <d3d11.h>
#include "SimpleShader.h"

class IEffect
{
protected:
	ISimpleShader* ppVS;
	ISimpleShader* ppPS;
	bool enabled;

public:
	virtual ~IEffect(void) = default;

	virtual void RenderEffect(ID3D11ShaderResourceView* srv, ID3D11DeviceContext* pDeviceContext, float dt, float tt) = 0;

	virtual bool Enabled(void) { return enabled; }
	virtual void Enabled(bool value) { enabled = value; }
};

#endif
