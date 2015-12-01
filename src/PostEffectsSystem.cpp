#include "PostEffectsSystem.h"

// Managers
#include "EntityManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"
#include "EventManager.h"

// Effects
#include "Warp.h"

// DirectX
#include <d3d11.h>

using namespace DirectX;

PostEffectsSystem::PostEffectsSystem()
{

#pragma region post process resource setup

	ResourceManager* pManager = ResourceManager::Instance();

	ID3D11RenderTargetView* ppRTV;
	ID3D11ShaderResourceView* ppSRV;

	ID3D11RenderTargetView* swapRTV;
	ID3D11ShaderResourceView* swapSRV;

	// Create post-process resources
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = pManager->GetWindowWidth();
	textureDesc.Height = pManager->GetWindowHeight();
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	// create the texture that we will render into
	ID3D11Texture2D* ppTexture;
	pManager->GetDevice()->CreateTexture2D(&textureDesc, 0, &ppTexture);

	// create the texture that we will render into
	ID3D11Texture2D* swapTexture;
	pManager->GetDevice()->CreateTexture2D(&textureDesc, 0, &swapTexture);

	// Create the Render Target View
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	pManager->GetDevice()->CreateRenderTargetView(ppTexture, &rtvDesc, &ppRTV);
	pManager->GetDevice()->CreateRenderTargetView(swapTexture, &rtvDesc, &swapRTV);

	// Create the Shader Resource View for the scene
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	pManager->GetDevice()->CreateShaderResourceView(ppTexture, &srvDesc, &ppSRV);
	pManager->GetDevice()->CreateShaderResourceView(swapTexture, &srvDesc, &swapSRV);

	// register the scene texture as our postEffectTexture
	pManager->RegisterTexture("PostEffectTexture", ppSRV);
	pManager->RegisterTexture("PostEffectSwapTexture", swapSRV);

	// We can release the texture because the RenderTarget and Shader Resource View now handle it
	ppTexture->Release();
	swapTexture->Release();

	// Register the post processing Render Targets
	pManager->RegisterRenderTargetView("PostRTV", ppRTV);
	pManager->RegisterRenderTargetView("PostSwapRTV", swapRTV);

#pragma endregion

	effects.emplace_back(new Warp());
	noEffect = new NoEffect();

	EventManager::Instance()->Register("WarpBegin", this);
}

PostEffectsSystem::~PostEffectsSystem(void)
{
	delete noEffect;

	for (std::vector<ImageEffect*>::iterator it = effects.begin(); it != effects.end(); ++it)
	{
		delete (*it);
	}
	effects.clear();
}

// TODO: break this up so that we can apply multiple effects at will
void PostEffectsSystem::Update(EntityManager * pManager, float dt, float tt)
{
	ResourceManager* pResource = ResourceManager::Instance();
	ID3D11Device* pDevice = pResource->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pResource->GetDeviceContext();

	ID3D11RenderTargetView* mainRTV = pResource->GetRenderTargetView("MainRTV");
	ID3D11RenderTargetView* postRTV = pResource->GetRenderTargetView("PostRTV");
	ID3D11RenderTargetView* swapRTV = pResource->GetRenderTargetView("PostSwapRTV");

	ID3D11DepthStencilView* dsv = pResource->GetDepthStencilView();

	// Grab the Texture we rendered into
	ID3D11ShaderResourceView* ppSRV = pResource->GetTexture("PostEffectTexture");
	ID3D11ShaderResourceView* swapSRV = pResource->GetTexture("PostEffectSwapTexture");

	// unbind the buffers and set up the "triangle" to draw onto
	ID3D11Buffer* nothing = 0;
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	UINT alt = -1;
	for (auto& effect : effects)
	{

		alt++;
		if (alt % 2 == 0)
		{
			//use the next render target
			pDeviceContext->OMSetRenderTargets(1, &swapRTV, dsv);

			//sample the last texture
			effect->RenderEffect(ppSRV, pDeviceContext);
		}
		else
		{
			pDeviceContext->OMSetRenderTargets(1, &postRTV, dsv);
			effect->RenderEffect(swapSRV, pDeviceContext);
		}
	}

	// switch back to rendering to the back buffer
	pDeviceContext->OMSetRenderTargets(1, &mainRTV, dsv);

	if (alt % 2 == 0)
	{
		noEffect->RenderEffect(swapSRV, pDeviceContext);
	}
	else
	{
		noEffect->RenderEffect(ppSRV, pDeviceContext);
	}
}

float warpTime = 2;
float timeElapsed = 0;
bool warping = false;
void PostEffectsSystem::EventRouter(const std::string & name, void * data)
{
	if (name == "WarpBegin")
	{
		CameraManager::Instance()->GetActiveCamera()->SetFOV(0.35f * XM_PI);
		//warp->Enabled(true);
	}
}
