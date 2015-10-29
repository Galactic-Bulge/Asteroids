#include "RenderSystem.h"

// STD
#include <cassert>
#include <algorithm>

// Managers
#include "ResourceManager.h"
#include "EntityManager.h"
#include "CameraManager.h"

// DirectX
#include <d3d11.h>
#include <DirectXMath.h>
#include "DXMacros.h"
#include "SimpleShader.h"
#include "Vertex.h"

// Components
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "LightComponent.h"

using namespace DirectX;

void RenderSystem::Update(EntityManager* pManager, float dt, float tt )
{
    ResourceManager* pResource = ResourceManager::Instance();
    ID3D11Device* pDevice = pResource->GetDevice();
    ID3D11DeviceContext* pDeviceContext = pResource->GetDeviceContext();
    ID3D11RenderTargetView* pRender = pResource->GetRenderTargetView();
    ID3D11DepthStencilView* pDepthStencil = pResource->GetDepthStencilView();
    IDXGISwapChain* pSwapChain = pResource->GetSwapChain();

    // Background color (Cornflower Blue in this case) for clearing
    const float color[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    // Clear the render target and depth buffer (erases what's on the screen)
    pDeviceContext->ClearRenderTargetView(pRender, color);
    pDeviceContext->ClearDepthStencilView(pDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Camera* pCamera = CameraManager::Instance()->GetActiveCamera();
    XMFLOAT3 camPos = pCamera->transform.GetTranslation();

    // Declare a MAX_LIGHTS sized array for each type of LightComponent
    DirectionalLightComponent::Light dirLights[DirectionalLightComponent::MAX_LIGHTS];
    PointLightComponent::Light pointLights[PointLightComponent::MAX_LIGHTS];
    SpotLightComponent::Light spotLights[SpotLightComponent::MAX_LIGHTS];

    // Zero Memory to provide default values
    ZeroMemory(dirLights, sizeof(DirectionalLightComponent::Light) * DirectionalLightComponent::MAX_LIGHTS);
    ZeroMemory(pointLights, sizeof(PointLightComponent::Light) * PointLightComponent::MAX_LIGHTS);
    ZeroMemory(spotLights, sizeof(SpotLightComponent::Light) * SpotLightComponent::MAX_LIGHTS);

    // Copy the Light struct out of the DirectionalLightComponent.
    std::vector<DirectionalLightComponent*> dirComponents = pManager->GetComponents<DirectionalLightComponent>();
    for(std::uint32_t i = 0; i < dirComponents.size() && i < DirectionalLightComponent::MAX_LIGHTS; ++i)
    {
        dirLights[i] = dirComponents[i]->light;
    }

    // Copy the Light struct out of the PointLightComponent.
    std::vector<PointLightComponent*> pointComponents = pManager->GetComponents<PointLightComponent>();
    for(std::uint32_t i = 0; i < pointComponents.size() && i < PointLightComponent::MAX_LIGHTS; ++i)
    {
        pointLights[i] = pointComponents[i]->light;
    }

    // Copy the Light struct out of the SpotLightComponent.
    std::vector<SpotLightComponent*> spotComponents = pManager->GetComponents<SpotLightComponent>();
    for(std::uint32_t i = 0; i < spotComponents.size() && i < SpotLightComponent::MAX_LIGHTS; ++i)
    {
        spotLights[i] = spotComponents[i]->light;
    }

    // Render every GameEntity.
    Material* pMaterial = nullptr;
    Mesh* pMesh = nullptr;
	ISimpleShader   *pVertexShader = nullptr;
	ISimpleShader	*pPixelShader = nullptr;

    for(auto& entity : pManager->EntitiesWithComponents<RenderComponent, TransformComponent>())
    {
        RenderComponent* pRender = pManager->GetComponent<RenderComponent>(entity);
        TransformComponent* pTransform = pManager->GetComponent<TransformComponent>(entity);

        // Attempt to cache the Mesh.
        if(!pMesh || pMesh != pRender->mesh)
        {
            // Set the Mesh.
            pMesh = pRender->mesh;

            // Update Mesh information.
            ID3D11Buffer* vb = pMesh->GetVertexBuffer();
            ID3D11Buffer* ib = pMesh->GetIndexBuffer();
            UINT stride = sizeof(Vertex);
            UINT offset = 0;
            pDeviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
            pDeviceContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
        }

        // Attempt to cache the Material.
        if(!pMaterial || pMaterial != pRender->material)
        {
            // Set the Material.
            pMaterial = pRender->material;

            // Update Material information.
            pMaterial->WriteShaderInfo();
        }

		// Attempt to cache the Vertex Shader
		if (!pVertexShader || pVertexShader != pRender->material->GetVertexShader())
		{
			pVertexShader = pRender->material->GetVertexShader();

			// Update "PerFrame" Vertex Shader data.
			pVertexShader->SetMatrix4x4("view", pCamera->GetViewMatrix());
			pVertexShader->SetMatrix4x4("projection", pCamera->GetProjectionMatrix());
			pVertexShader->CopyBufferData("PerFrame");

		}

		// Attempt to cache the pixel shader
		if (!pPixelShader || pPixelShader != pRender->material->GetPixelShader())
		{
			pPixelShader = pRender->material->GetPixelShader();

			// Update "Lights" Constant Buffer.
			pPixelShader->SetData("directionalLights", reinterpret_cast<void*>(dirLights), DirectionalLightComponent::MAX_LIGHTS * sizeof(DirectionalLightComponent::Light));
			pPixelShader->SetData("pointLights", reinterpret_cast<void*>(pointLights), PointLightComponent::MAX_LIGHTS * sizeof(PointLightComponent::Light));
			pPixelShader->SetData("spotLights", reinterpret_cast<void*>(spotLights), SpotLightComponent::MAX_LIGHTS * sizeof(SpotLightComponent::Light));
			pPixelShader->CopyBufferData("Lights");

			// Update "PerFrame" Constant Buffer.
			pPixelShader->SetFloat3("cameraPosition", XMFLOAT3(camPos.x, camPos.y, camPos.z));
			pPixelShader->CopyBufferData("PerFrame");

			pPixelShader->SetSamplerState("trilinear", pResource->GetSamplerState("trilinear"));
		}

        // Update "PerObject" Constant Buffer.
        Transform& t = pTransform->transform;
        pVertexShader->SetMatrix4x4("world", t.GetWorldMatrix());
        pVertexShader->CopyBufferData("PerObject");

        // Set the Shaders but do not copy buffers, this has been done already.
        pVertexShader->SetShader(false);
        pPixelShader->SetShader(false);

        // Draw the GameEntity
        pDeviceContext->DrawIndexed(pMesh->GetIndexCount(), 0, 0);
    }

    // Render the Skybox.
    {
        ISimpleShader   *pSkyVertex = pResource->GetShader("SkyboxVertex"),
                        *pSkyPixel = pResource->GetShader("SkyboxPixel");
        ID3D11RasterizerState* pRasterizer = pResource->GetRasterizerState("Skybox_Rasterizer");
        ID3D11DepthStencilState* pDepthStencil = pResource->GetDepthStencilState("Skybox_DepthStencil");
        ID3D11ShaderResourceView* pSkySRV = pResource->GetTexture("CubeMap");

        // Update Mesh data.
        Mesh* pCube = pResource->GetMesh("Cube"); 
        ID3D11Buffer* vb = pCube->GetVertexBuffer();
        ID3D11Buffer* ib = pCube->GetIndexBuffer();
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        pDeviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
        pDeviceContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

        // Send the matrices.
        pSkyVertex->SetMatrix4x4("view", pCamera->GetViewMatrix());
        pSkyVertex->SetMatrix4x4("projection", pCamera->GetProjectionMatrix());
        pSkyVertex->CopyBufferData("PerFrame");

        // Send the texure and sampler.
        pSkyPixel->SetShaderResourceView("skybox", pSkySRV);
        pSkyPixel->SetSamplerState("trilinear", pResource->GetSamplerState("trilinear"));

        // Set Shader without copying buffers.
        pSkyVertex->SetShader(false);
        pSkyPixel->SetShader(false);

        // Set Rasterizer and DepthStencil.
        pDeviceContext->RSSetState(pRasterizer);
        pDeviceContext->OMSetDepthStencilState(pDepthStencil, 0);

        // Render the Skybox.
        pDeviceContext->DrawIndexed(pCube->GetIndexCount(), 0, 0);

        // Reset Rasterizer and DepthStencil.
        pDeviceContext->RSSetState(nullptr);
        pDeviceContext->OMSetDepthStencilState(nullptr, 0);
    }

    // Present the buffer
    HR(pSwapChain->Present(0, 0));
}
