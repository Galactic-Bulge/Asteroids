#include "SwapSystem.h"

// Managers
#include "EntityManager.h"
#include "ResourceManager.h"

// DirectX
#include <d3d11.h>
#include "DXMacros.h"

using namespace DirectX;

SwapSystem::SwapSystem(void)
{
	m_pResource = ResourceManager::Instance();
}

SwapSystem::~SwapSystem(void)
{
    /* Nothing to do. */
}

void SwapSystem::Update(EntityManager* pManager, float dt, float tt)
{
    // Present the buffer
	IDXGISwapChain* pSwapChain = m_pResource->GetSwapChain();
    HR(pSwapChain->Present(0,0));
}
