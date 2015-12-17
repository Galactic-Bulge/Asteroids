#include "ClearSystem.h"

// Managers
#include "EntityManager.h"
#include "ResourceManager.h"

// DirectX
#include <d3d11.h>

using namespace DirectX;

ClearSystem::ClearSystem(void)
{
	m_pResource = ResourceManager::Instance();
}

ClearSystem::~ClearSystem(void)
{
    /* Nothing to do. */
}

void ClearSystem::Update(EntityManager* pManager, float dt, float tt)
{
    ResourceManager* pResource = ResourceManager::Instance();

    ID3D11DeviceContext* pDeviceContext = pResource->GetDeviceContext();
	ID3D11RenderTargetView* pEffectRenderTarget = pResource->GetRenderTargetView("PostRTV");
	ID3D11RenderTargetView* pEffectSwapRenderTarget = pResource->GetRenderTargetView("PostSwapRTV");
    ID3D11RenderTargetView* pMainRenderTarget = pResource->GetRenderTargetView("MainRTV");
    ID3D11DepthStencilView* pDepthStencilView = pResource->GetDepthStencilView();

    // Clear the screen to black.
    const float color[] = {0.0f, 0.0f, 0.0f, 0.0f };
	if (pEffectRenderTarget != nullptr)
	{
		pDeviceContext->ClearRenderTargetView(pEffectRenderTarget, color);
	}
	if (pEffectSwapRenderTarget != nullptr)
	{
		pDeviceContext->ClearRenderTargetView(pEffectSwapRenderTarget, color);
	}
	pDeviceContext->ClearRenderTargetView(pMainRenderTarget, color);
    pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
