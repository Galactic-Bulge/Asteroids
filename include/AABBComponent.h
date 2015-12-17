#ifndef AABB_COMPONENT_H
#define AABB_COMPONENT_H

// ECS
#include "Component.h"

// DirectX
#include <DirectXMath.h>

#include "AABB.h"

struct AABBComponent : public Component<AABBComponent>
{
    AABB boundingBox;

    /*
     * Construct an instance of the AABB component.
     * @param   min     The upper-left point of the AABB.
     * @param   max     The lower-right point of the AABB.
     */
    AABBComponent(DirectX::XMFLOAT2 min, DirectX::XMFLOAT2 max)
        : boundingBox(min, max)
    {
        /* Nothing to do. */
    }

    /*
     * Construct an instance of the AABB component.
     * @param   center      THe center point of the AABB.
     * @param   halfWidth   Half the width of the AABB.
     * @param   halfHeight  Half the height of the AABB.
     */
    AABBComponent(DirectX::XMFLOAT2 center, float halfWidth, float halfHeight)
        : boundingBox(center, halfWidth, halfHeight)
    {
        /* Nothing to do. */
    }

    virtual ~AABBComponent(void) override {}
};

#endif
