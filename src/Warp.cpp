#include "Warp.h"

// Managers
#include "ResourceManager.h"
#include "CameraManager.h"
#include "EventManager.h"

#include "Tweens.h"

// Directx
#include <d3d11.h>

using namespace DirectX;

Warp::Warp()
{
	ppVS = ResourceManager::Instance()->GetShader("PPVS");
	ppPS = ResourceManager::Instance()->GetShader("PPWarpBlur");
	warpTime = 6;
	blurWidth = 0.3f;
	startFOV = CameraManager::Instance()->GetActiveCamera()->GetFOV();
	endFOV = 0.35f * XM_PI;
}

Warp::~Warp(void)
{
	/*Nothing to do*/
}

/*
* Mutator for Enabled state. When set to true this starts the warp effect
* @param   value              The state of the effect.
*/
void Warp::Enabled(bool value)
{
	enabled = value;
	if (enabled)
	{
		timeElapsed = 0;
	}
}

/*
* Update Loop for the effect
* @param   src                The Texture to read from.
* @param   pDeviceContext     The Device Context we are using
* @param   dt				  Elapsed time since last frame
* @param   tt                 Total Time Elapsed
*/
void Warp::RenderEffect(ID3D11ShaderResourceView * srv, ID3D11DeviceContext* pDeviceContext, float dt, float tt)
{
	// don't do anything if the effect isn't on
	if (enabled)
	{
		float time0to1;
		float blur, fov;

		if (timeElapsed <= warpTime / 3)
		{
			// map the time elapsed onto 0 - 1
			time0to1 = Tweens::Instance()->MapRange(timeElapsed, 0, warpTime / 3, 0, 1);

			// Ease in the blur
			blur = Tweens::Instance()->EaseIn(time0to1);

			// Map back from 0-1 into 0 - blurWidth
			blur = Tweens::Instance()->MapRange(blur, 0, 1, 0, blurWidth);
			blur = -blur;

			// Ease in the fov
			fov = Tweens::Instance()->EaseIn(time0to1);
			fov = Tweens::Instance()->MapRange(fov, 0, 1, startFOV, endFOV);
		}
		else if (timeElapsed > 2 * (warpTime / 3))
		{
			// map time onto 0-1
			time0to1 = Tweens::Instance()->MapRange(timeElapsed, 2 * (warpTime / 3), warpTime, 0, 1);

			// Ease out blur
			blur = Tweens::Instance()->EaseOut(time0to1);
			blur = Tweens::Instance()->MapRange(blur, 0, 1, 0, blurWidth);
			blur = -blur;

			// Ease out fov
			fov = Tweens::Instance()->EaseOut(time0to1);
			fov = Tweens::Instance()->MapRange(fov, 0, 1, startFOV, endFOV);
		}
		else
		{
			// hold max fov and blur for a few seconds in the middle of the effect
			blur = -blurWidth;
			fov = endFOV;
		}

		CameraManager::Instance()->GetActiveCamera()->SetFOV(fov);

		ID3D11SamplerState* sampler = ResourceManager::Instance()->GetSamplerState("trilinear");
		ppVS->SetShader();

		ppPS->SetShaderResourceView("pixels", srv);
		ppPS->SetSamplerState("trilinear", sampler);
		ppPS->SetFloat("BlurWidth", blur);
		ppPS->SetShader();

		// Draw a specific number of vertices (without buffers)
		pDeviceContext->Draw(3, 0);

		// Unbind shader resource view so we can render into the
		// texture at the beginning of next frame
		ppPS->SetShaderResourceView("pixels", 0);

		timeElapsed += dt;

		if (timeElapsed > warpTime)
		{
			enabled = false;
			EventManager::Instance()->Fire("WarpEnd", nullptr);
		}
	}
}
