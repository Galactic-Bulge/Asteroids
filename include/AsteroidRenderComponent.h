#pragma once

#include "Component.h"
struct AsteroidRenderComponent : public Component<AsteroidRenderComponent>
{
	int randomID=0;
	AsteroidRenderComponent(int id)
		: randomID(id)
	{ /* Nothing to do */
	}

	~AsteroidRenderComponent(void) {}
};