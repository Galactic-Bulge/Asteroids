#ifndef ATTACK_COMPONENT
#define ATTACK_COMPONENT

#include "Component.h"

struct AttackComponent : public Component<AttackComponent>
{
	float damage;

	AttackComponent(float d)
		: damage(d)
	{
	}
};

#endif
