#include "NoEffect.h"
#include "ResourceManager.h"

NoEffect::NoEffect()
{
	ppVS = ResourceManager::Instance()->GetShader("PPVS");
	ppPS = ResourceManager::Instance()->GetShader("PPNoEffect");
}

NoEffect::~NoEffect(void)
{
	/* Nothing to Do */
}

/*
* Update Loop for the effect
* @param   src                The Texture to read from.
* @param   pDeviceContext     The Device Context we are using
* @param   dt				  Elapsed time since last frame
* @param   tt                 Total Time Elapsed
*/
void NoEffect::RenderEffect(ID3D11ShaderResourceView * srv, ID3D11DeviceContext * pDeviceContext, float dt, float tt)
{
	ID3D11SamplerState* sampler = ResourceManager::Instance()->GetSamplerState("trilinear");
	ppVS->SetShader();

	ppPS->SetShaderResourceView("pixels", srv);
	ppPS->SetSamplerState("trilinear", sampler);
	ppPS->SetShader();
	

	// Draw a specific number of vertices (without buffers)
	pDeviceContext->Draw(3, 0);

	// Unbind shader resource view so we can render into the
	// texture at the beginning of next frame
	ppPS->SetShaderResourceView("pixels", 0);
}
