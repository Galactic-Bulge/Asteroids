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
#include <CollisionComponent.h>

// Collisions
#include <Octant.h>
#include "AsteroidRenderComponent.h"

using namespace DirectX;

RenderSystem::RenderSystem(void)
{
	m_pCamera = CameraManager::Instance();
	m_pResource = ResourceManager::Instance();

	/* Sampler Creation */
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.MaxLOD = D3D11_FLOAT32_MAX;

		m_pResource->RegisterSamplerState("trilinear", desc);
	}
}

RenderSystem::~RenderSystem(void)
{
	/* Nothing to do. */
}

void RenderSystem::Update(EntityManager* pManager, float dt, float tt )
{
    ID3D11Device* pDevice = m_pResource->GetDevice();
    ID3D11DeviceContext* pDeviceContext = m_pResource->GetDeviceContext();
    Camera* pCamera = m_pCamera->GetActiveCamera();
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

			pPixelShader->SetSamplerState("trilinear", m_pResource->GetSamplerState("trilinear"));
		}

        // Update "PerObject" Constant Buffer.
        Transform& t = pTransform->transform;
        pVertexShader->SetMatrix4x4("world", t.GetWorldMatrix());
		if (pManager->HasComponent<AsteroidRenderComponent>(entity)) {
			AsteroidRenderComponent* asteroid = pManager->GetComponent<AsteroidRenderComponent>(entity);
			pVertexShader->SetInt("id", asteroid->randomID);
		}

        pVertexShader->CopyBufferData("PerObject");

        // Set the Shaders but do not copy buffers, this has been done already.
        pVertexShader->SetShader(false);
        pPixelShader->SetShader(false);

        // Draw the GameEntity
        pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        pDeviceContext->DrawIndexed(pMesh->GetIndexCount(), 0, 0);
    }
}

void RenderSystem::RenderCollisionSpheres(EntityManager* pManager)
{
	std::vector<GameEntity> collisionEntities = pManager->EntitiesWithComponents<CollisionComponent>();

	ResourceManager* rManager = ResourceManager::Instance();
	ID3D11Device* device = rManager->GetDevice();
	ID3D11DeviceContext* deviceContext = rManager->GetDeviceContext();
	ID3D11RasterizerState* rState = rManager->GetRasterizerState("Wireframe_Rasterizer");

	// Update the mesh
	Mesh* sphere = rManager->GetMesh("Sphere");
	ID3D11Buffer* vb = sphere->GetVertexBuffer();
	ID3D11Buffer* ib = sphere->GetIndexBuffer();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	deviceContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

	Material* colliderMat = rManager->GetMaterial("collider");
	colliderMat->WriteShaderInfo();
	CollisionSphere collider;
	XMMATRIX translation;
	XMMATRIX scale;
	XMFLOAT4X4 transform;

	colliderMat->GetVertexShader()->SetMatrix4x4("view", CameraManager::Instance()->GetActiveCamera()->GetViewMatrix());
	colliderMat->GetVertexShader()->SetMatrix4x4("projection", CameraManager::Instance()->GetActiveCamera()->GetProjectionMatrix());
	deviceContext->RSSetState(rState);

	for (GameEntity ge : collisionEntities)
    {
		collider = pManager->GetComponent<CollisionComponent>(ge)->collider;
		translation = XMMatrixTranslation(collider.GetPosition().x, collider.GetPosition().y, collider.GetPosition().z);
		scale = XMMatrixScaling(collider.GetRadius() * 2, collider.GetRadius() * 2, collider.GetRadius() * 2);
		XMStoreFloat4x4(&transform, XMMatrixTranspose(scale * translation));

		colliderMat->GetVertexShader()->SetMatrix4x4("world", transform);
		(collider.GetIsColliding()) ? colliderMat->GetPixelShader()->SetFloat("isColliding", 1) : colliderMat->GetPixelShader()->SetFloat("isColliding", 0);
		SimplePixelShader* pShader = colliderMat->GetPixelShader();

		colliderMat->GetVertexShader()->SetShader(true);
		colliderMat->GetPixelShader()->SetShader(true);

		deviceContext->DrawIndexed(sphere->GetIndexCount(), 0, 0);
	}

	deviceContext->RSSetState(nullptr);
}

void RenderSystem::RenderOctants(EntityManager* pManager) {
	ResourceManager* rManager = ResourceManager::Instance();
	ID3D11Device* device = rManager->GetDevice();
	ID3D11DeviceContext* deviceContext = rManager->GetDeviceContext();
	ID3D11RasterizerState* rState = rManager->GetRasterizerState("Wireframe_Rasterizer");

	// Update the mesh
	Mesh* cube = rManager->GetMesh("Cube");
	ID3D11Buffer* vb = cube->GetVertexBuffer();
	ID3D11Buffer* ib = cube->GetIndexBuffer();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	deviceContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

	Material* colliderMat = rManager->GetMaterial("collider");
	colliderMat->WriteShaderInfo();
	XMMATRIX translation;
	XMMATRIX scale;
	XMFLOAT4X4 transform;

	colliderMat->GetVertexShader()->SetMatrix4x4("view", CameraManager::Instance()->GetActiveCamera()->GetViewMatrix());
	colliderMat->GetVertexShader()->SetMatrix4x4("projection", CameraManager::Instance()->GetActiveCamera()->GetProjectionMatrix());
	deviceContext->RSSetState(rState);

	for (auto& o : pManager->Octants()) {
		translation = XMMatrixTranslation(o.position.x, o.position.y, o.position.z);
		scale = XMMatrixScaling(o.halfW * 2, o.halfH * 2, o.halfD * 2);
		XMStoreFloat4x4(&transform, XMMatrixTranspose(scale * translation));

		colliderMat->GetVertexShader()->SetMatrix4x4("world", transform);

		colliderMat->GetVertexShader()->SetShader(true);
		colliderMat->GetPixelShader()->SetShader(true);

		deviceContext->DrawIndexed(cube->GetIndexCount(), 0, 0);
	}
}
