#ifndef ISCRIPT_H
#define ISCRIPT_H

#include "GameEntity.h"

class IScript
{
public:
	virtual ~IScript() {}
	virtual void Update(GameEntity entity, float dt, float tt) = 0;
};

#endif
