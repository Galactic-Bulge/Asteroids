#include "SkyboxSystem.h"

// Managers
#include "EntityManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"
#include "EventManager.h"
#include "InputManager.h"

// DirectX
#include <d3d11.h>
#include "DXMacros.h"
#include "SimpleShader.h"
#include "Vertex.h"
#include "Mesh.h"

using namespace DirectX;

SkyboxSystem::SkyboxSystem()
{
	EventManager::Instance()->Register("WarpBegin", this);
	textureNames[0] = "GreenSpace";
	textureNames[1] = "PurpleSpace";
	textureNames[2] = "Space";
	warping = false;
	timeElapsed = 0;
	currentTexture = 0;

	m_pResource = ResourceManager::Instance();

	// Create the ID3D11RasterizerState for the Skybox.
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_FRONT;
		desc.DepthClipEnable = true;

		m_pResource->RegisterRasterizerState("Skybox_Rasterizer", desc);
	}

	// Create the ID3D11DepthStencilState for the Skybox.
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		m_pResource->RegisterDepthStencilState("Skybox_DepthStencil", desc);
	}
}

SkyboxSystem::~SkyboxSystem(void)
{
    /* Nothing to do. */
}

void SkyboxSystem::EventRouter(const std::string& name, void* data)
{
	if (name == "WarpBegin")
	{
		currentTexture++;
		currentTexture = currentTexture > 2 ? 0 : currentTexture;
		warping = true;
		timeElapsed = 0;
	}
}

void SkyboxSystem::Update(EntityManager* pManager, float dt, float tt)
{
	//if (InputManager::Instance()->IsKeyDown('L') && !warping) EventManager::Instance()->Fire("WarpBegin", nullptr);

	int lastTexture = currentTexture - 1 < 0 ? 2 : currentTexture - 1;

	if (warping)
	{
		timeElapsed += dt;
		if (timeElapsed > warpTime)
		{
			warping = false;
			lerpT = 1;
			EventManager::Instance()->Fire("WarpEnd", nullptr);
		}
		else
		{
			lerpT = timeElapsed / warpTime;
		}
	}

    ID3D11DeviceContext* pDeviceContext = m_pResource->GetDeviceContext();
    ID3D11RasterizerState* pRasterizer = m_pResource->GetRasterizerState("Skybox_Rasterizer");
    ID3D11DepthStencilState* pDepthStencil = m_pResource->GetDepthStencilState("Skybox_DepthStencil");
	ID3D11ShaderResourceView* pToSkySRV = m_pResource->GetTexture(textureNames[currentTexture]);
	ID3D11ShaderResourceView* pFromSkySRV = m_pResource->GetTexture(textureNames[lastTexture]);
    ISimpleShader   *pSkyVertex = m_pResource->GetShader("SkyboxVertex"),
                    *pSkyPixel = m_pResource->GetShader("SkyboxPixel");
    Camera* pCamera = CameraManager::Instance()->GetActiveCamera();

    // Update Mesh data.
    Mesh* pCube = m_pResource->GetMesh("Cube");
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
	pSkyPixel->SetShaderResourceView("skybox", pToSkySRV);
	pSkyPixel->SetShaderResourceView("fromSkybox", pFromSkySRV);
	pSkyPixel->SetFloat("t", lerpT);
    pSkyPixel->SetSamplerState("trilinear", m_pResource->GetSamplerState("trilinear"));
	pSkyPixel->CopyBufferData("PerFrame");

    // Set Shader without copying buffers.
    pSkyVertex->SetShader(false);
    pSkyPixel->SetShader(false);

    // Set Rasterizer and DepthStencil.
    pDeviceContext->RSSetState(pRasterizer);
    pDeviceContext->OMSetDepthStencilState(pDepthStencil, 0);

    // Render the Skybox.
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pDeviceContext->DrawIndexed(pCube->GetIndexCount(), 0, 0);

    // Reset Rasterizer and DepthStencil.
    pDeviceContext->RSSetState(nullptr);
    pDeviceContext->OMSetDepthStencilState(nullptr, 0);
}
