#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

// ECS
#include "System.h"

// Forward Declarations
class CameraManager;
class ResourceManager;

class RenderSystem : public System<RenderSystem>
{
private:
	CameraManager* m_pCamera;
	ResourceManager* m_pResource;
public:
	RenderSystem(void);
	virtual ~RenderSystem(void);

    /* System interface. */
    virtual void Update(EntityManager* pManager, float dt, float tt ) override;

	void RenderCollisionSpheres(EntityManager* pManager);
	void RenderOctants(EntityManager* pManager);
};

#endif
