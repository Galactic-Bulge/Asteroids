#ifndef SKYBOX_SYSTEM_H
#define SKYBOX_SYSTEM_H

// STD
#include <string>

// ECS
#include "System.h"

// Events
#include "IEventListener.h"

// Forward Declarations
class ResourceManager;

class SkyboxSystem : public System<SkyboxSystem>, public IEventListener
{
public:
	SkyboxSystem(void);
    virtual ~SkyboxSystem(void);
    virtual void Update(EntityManager* pManager, float dt, float tt);
	virtual void EventRouter(const std::string& name, void* data) override;
private:
	ResourceManager* m_pResource;
	std::string textureNames[3];
	int currentTexture;
	bool warping;
	float lerpT = 1;
	float timeElapsed;
	float warpTime = 2;
};

#endif
