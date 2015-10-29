#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"

#include "Material.h"
#include "Mesh.h"

struct RenderComponent : public Component<RenderComponent>
{
    Material* material;
    Mesh* mesh;

    RenderComponent(Material* const pMaterial, Mesh* const pMesh)
        : material(pMaterial), mesh(pMesh)
    { /* Nothing to do */ }

    ~RenderComponent(void){}
};

#endif
