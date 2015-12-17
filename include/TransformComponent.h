#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "Component.h"
#include "Transform.h"

struct TransformComponent : public Component<TransformComponent>
{
    Transform transform;

    TransformComponent()
    {
        /* Nothing to do. */
    }

    TransformComponent(DirectX::XMFLOAT3 t)
    {
        transform = Transform(t);
    }

    ~TransformComponent(void){}
};

#endif
