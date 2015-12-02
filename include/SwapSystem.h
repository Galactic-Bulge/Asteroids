#ifndef SWAP_SYSTEM_H
#define SWAP_SYSTEM_H

// ECS
#include "System.h"

// Forward Declarations
class ResourceManager;

class SwapSystem : public System<SwapSystem>
{
private:
	ResourceManager* m_pResource;
public:
	SwapSystem(void);
    virtual ~SwapSystem(void);
    virtual void Update(EntityManager* pManager, float dt, float tt);
};

#endif
