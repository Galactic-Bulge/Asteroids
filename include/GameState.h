#ifndef GAME_STATE_H
#define GAME_STATE_H

// DirectX
#include <d3d11.h>

// State
#include "IGameState.h"

#include "EventListener.h"

// STD
#include <vector>

class GameState : public IGameState, public EventListener
{
private:
    bool     isInitialized;
    bool	 prevSpaceBar;
    unsigned currentEntity;

    virtual void Enter( void ) override;
    virtual void Update( float deltaTime, float totalTime ) override;
    virtual void Exit( void ) override;

public:
    virtual ~GameState() {}
	void EventRouter(std::string name, void* data) override;
};

#endif
