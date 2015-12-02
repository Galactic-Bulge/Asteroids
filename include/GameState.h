#ifndef GAME_STATE_H
#define GAME_STATE_H

// DirectX
#include <d3d11.h>

// State
#include "IGameState.h"

// Events
#include "IEventListener.h"

// STD
#include <vector>

class GameState : public IGameState, public IEventListener
{
private:
    bool     isInitialized;
    bool	 prevSpaceBar;
    unsigned currentEntity;
	unsigned int currentLevel = 0;
	unsigned int asteroids = 0;

    virtual void Enter( void ) override;
    virtual void Update( float deltaTime, float totalTime ) override;
    virtual void Exit( void ) override;

	void LoadCurrentLevel();
public:
    virtual ~GameState() {}
	virtual void EventRouter(const std::string& name, void* data) override;
};

#endif
