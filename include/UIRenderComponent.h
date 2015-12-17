#ifndef UI_RENDER_COMPONENT_H
#define UI_RENDER_COMPONENT_H

// ECS
#include "Component.h"

// DirectX
#include <DirectXMath.h>
#include "Material.h"

struct UIRenderComponent : public Component<UIRenderComponent>
{
public:
    DirectX::XMFLOAT2 center;
    float width, height;
    Material* material;

    UIRenderComponent(DirectX::XMFLOAT2 center, float width, float height, Material* pMaterial)
        : center(center), width(width), height(height), material(pMaterial)
    {
        /* Nothing to do. */
    }

    virtual ~UIRenderComponent(void) override {}
};

#endif
