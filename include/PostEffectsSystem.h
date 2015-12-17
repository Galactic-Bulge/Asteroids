#ifndef POST_EFFECTS_SYSTEM_H
#define POST_EFFECTS_SYSTEM_H

#include "System.h"
#include <vector>
#include <unordered_map>
#include <string>

#include "IEventListener.h"

#include "IEffect.h"
#include "NoEffect.h"

enum Effect
{
	WarpBlur
};

class PostEffectsSystem : public System<PostEffectsSystem> , public IEventListener
{
public:
	PostEffectsSystem();
	virtual ~PostEffectsSystem(void);
	virtual void Update(EntityManager* pManager, float dt, float tt);
	virtual void EventRouter(const std::string& name, void* data) override;
	void EnableEffect(Effect effect);
	void SetEffectPriority(Effect effect, UINT priority);
private:
	std::vector<IEffect*> effectsOrdered;
	std::unordered_map<std::string, IEffect*> effectsMap;
	NoEffect* noEffect;
	float warpTime = 2;
	float timeElapsed = 0;
	bool warping = false;
};


#endif
