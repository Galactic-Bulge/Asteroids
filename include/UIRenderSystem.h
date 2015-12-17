#ifndef UI_RENDER_SYSTEM_H
#define UI_RENDER_SYSTEM_H

// ECS
#include "System.h"

// DirectX
#include <d3d11.h>
#include <DirectXMath.h>

// DirectXTK
#include <SpriteBatch.h>

// Forward Declarations
class ResourceManager;
class CameraManager;

struct UIVertex
{
    DirectX::XMFLOAT2 position;
    DirectX::XMFLOAT2 uv;
};

class UIRenderSystem : public System<UIRenderSystem>
{
private:
    static constexpr unsigned int NUM_VERTICES{4};
    static constexpr unsigned int NUM_INDICES{6};
    static UIVertex vertices[NUM_VERTICES];
    static unsigned int indices[NUM_INDICES];

    ResourceManager* m_pResource;
    CameraManager* m_pCamera;
    ID3D11Buffer *vertexBuffer, *indexBuffer;
    DirectX::SpriteBatch* spriteBatch;
public:
    /*
     * Construct an instance of the UIRenderSystem.
     */
	UIRenderSystem(void);

    /*
     * Destruct the UIRenderSystem instance.
     */
	virtual ~UIRenderSystem(void) override;

    /*
     * System interface.
     */
    virtual void Update(EntityManager* pManager, float dt, float tt) override;
};

#endif
