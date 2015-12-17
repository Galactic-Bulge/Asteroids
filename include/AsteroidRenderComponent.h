#ifndef ASTEROID_RENDER_COMPONENT_H
#define ASTEROID_RENDER_COMPONENT_H

// ECS
#include "Component.h"

struct AsteroidRenderComponent : public Component<AsteroidRenderComponent>
{
public:
	int randomID{0};

	AsteroidRenderComponent(int id)
		: randomID(id)
	{
        /* Nothing to do. */
	}

	virtual ~AsteroidRenderComponent(void) override
    {
        /* Nothing to do. */
    }
};

#endif
