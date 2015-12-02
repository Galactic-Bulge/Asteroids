#ifndef CLEAR_SYSTEM_H
#define CLEAR_SYSTEM_H

// ECS
#include "System.h"

// Forward Declarations
class ResourceManager;

class ClearSystem : public System<ClearSystem>
{
private:
	ResourceManager* m_pResource;
public:
	ClearSystem(void);
    virtual ~ClearSystem(void);
    virtual void Update(EntityManager* pManager, float dt, float tt);
};

#endif
