#include "UIRenderSystem.h"

// Managers
#include "EntityManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"

// DirectX
#include "DXMacros.h"

// DirectXTK
#include <SpriteBatch.h>
#include <SpriteFont.h>

// Components
#include "UIRenderComponent.h"
#include "UITextComponent.h"

using namespace DirectX;

UIVertex UIRenderSystem::vertices[NUM_VERTICES] =
{
    UIVertex{XMFLOAT2{-0.5f, -0.5f}, XMFLOAT2{0.0f, 1.0f}},
    UIVertex{XMFLOAT2{0.5f, -0.5f}, XMFLOAT2{1.0f, 1.0f}},
    UIVertex{XMFLOAT2{-0.5f, 0.5f}, XMFLOAT2{0.0f, 0.0f}},
    UIVertex{XMFLOAT2{0.5f, 0.5f}, XMFLOAT2{1.0f, 0.0f}}
};

unsigned int UIRenderSystem::indices[NUM_INDICES] = {0, 2, 1, 1, 2, 3};

/*
 * TODO
 */
UIRenderSystem::UIRenderSystem(void)
{
    m_pResource = ResourceManager::Instance();
    m_pCamera = CameraManager::Instance();

    ID3D11Device* pDevice = m_pResource->GetDevice();
    ID3D11DeviceContext* pDeviceContext = m_pResource->GetDeviceContext();

    // Create the Vertex Buffer.
    {
        D3D11_BUFFER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.ByteWidth = sizeof(UIVertex) * NUM_VERTICES;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA bufferData;
        ZeroMemory(&bufferData, sizeof(bufferData));
        bufferData.pSysMem = vertices;

        pDevice->CreateBuffer(&desc, &bufferData, &vertexBuffer);
    }

    // Create the Index Buffer.
    {
        D3D11_BUFFER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.ByteWidth = sizeof(unsigned int) * NUM_INDICES;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA bufferData;
        ZeroMemory(&bufferData, sizeof(bufferData));
        bufferData.pSysMem = indices;

        pDevice->CreateBuffer(&desc, &bufferData, &indexBuffer);
    }

    // Create the SpriteBatch.
    spriteBatch = new SpriteBatch(pDeviceContext);
}

/*
 * TODO
 */
UIRenderSystem::~UIRenderSystem(void)
{
    delete spriteBatch;
    ReleaseMacro(vertexBuffer);
    ReleaseMacro(indexBuffer);
}

/*
 * System interface.
 */
void UIRenderSystem::Update(EntityManager* pManager, float dt, float tt )
{
    // Setup the Device Context
    unsigned int stride = sizeof(UIVertex);
    unsigned int offset = 0;
    ID3D11DeviceContext* pDeviceContext = m_pResource->GetDeviceContext();
    pDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    pDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Get the Material.
    Material* pMaterial{nullptr};

    // Get the Camera.
    Camera* pCamera = m_pCamera->GetActiveCamera();

    // Generate Orthographic Projection matrix.
    // TODO: Set from windowWidth/windowHeight
    XMFLOAT4X4 projection;
    XMStoreFloat4x4(&projection, XMMatrixTranspose(XMMatrixOrthographicLH(800.0f, 600.0f, Camera::DEFAULT_NEAR_PLANE, Camera::DEFAULT_FAR_PLANE)));

    // Get the Vertex Shader.
    ISimpleShader* pVertex = m_pResource->GetShader("UIVertex");
    pVertex->SetMatrix4x4("projection", projection);
    pVertex->CopyBufferData("PerFrame");

    // Get the Pixel Shader.
    ISimpleShader* pPixel = m_pResource->GetShader("UIPixel");

    // Render all button components.
    for(auto& entity : pManager->EntitiesWithComponents<UIRenderComponent>())
    {
        // Grab a reference to the UIRenderComponent.
        UIRenderComponent* pRender = pManager->GetComponent<UIRenderComponent>(entity);

        // Get the translation.
        XMFLOAT4X4 translation;
        XMStoreFloat4x4(&translation, XMMatrixTranspose(XMMatrixTranslation(pRender->center.x, pRender->center.y, 0.0f)));

        // Get the scaling.
        XMFLOAT4X4 scale;
        XMStoreFloat4x4(&scale, XMMatrixTranspose(XMMatrixScaling(pRender->width, pRender->height, 1.0f)));

        // Update the Vertex Shader.
        pVertex->SetMatrix4x4("translation", translation);
        pVertex->SetMatrix4x4("scale", scale);
        pVertex->CopyBufferData("PerObject");

        // Update the Pixel Shader.
        pPixel->SetSamplerState("trilinear", m_pResource->GetSamplerState("trilinear"));

        // Set the Material if necessary.
        if(!pMaterial || pMaterial != pRender->material)
        {
            pMaterial = pRender->material;
            pMaterial->WriteShaderInfo();
        }

        // Set the Shaders.
        pVertex->SetShader(false);
        pPixel->SetShader(false);

        // Finally render.
        pDeviceContext->DrawIndexed(NUM_INDICES, 0, 0);
    }

    // Render all text.
    spriteBatch->Begin();
    for(auto& entity : pManager->EntitiesWithComponents<UITextComponent>())
    {
        // Grab a reference to the component.
        UITextComponent* pText = pManager->GetComponent<UITextComponent>(entity);

        // Grab the attributes for the UITextComponent.
        std::wstring text = pText->text;
        XMFLOAT2 position = pText->position;
        XMVECTOR color = XMLoadFloat4(&pText->color);
        SpriteFont* pSpriteFnt = m_pResource->GetSpriteFont(pText->spritefontID);

        // Finally render the text.
        pSpriteFnt->DrawString(spriteBatch, text.c_str(), position, color);
    }
    spriteBatch->End();

    // Reset the Device Context. (THIS IS NECESSARY BECAUSE OF SpriteBatch::End)
    pDeviceContext->RSSetState(nullptr);
    pDeviceContext->OMSetDepthStencilState(nullptr, 0);
}
