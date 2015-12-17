#ifndef MENU_STATE_H
#define MENU_STATE_H

// Interfaces
#include "IGameState.h"
#include "IEventListener.h"

// STD
#include <string>

class MenuState : public IGameState, public IEventListener
{
public:
	virtual ~MenuState(void) override;

private:
    virtual void EventRouter(const std::string& event, void* pData) override;
    virtual void Enter(void) override;
    virtual void Update(float deltaTime, float totalTime) override;
    virtual void Exit(void) override;
};

#endif
