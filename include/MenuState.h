#ifndef MENU_STATE_H
#define MENU_STATE_H

// IGameState interface
#include "IGameState.h"

// DirectX
#include <d3d11.h>

class MenuState : public IGameState
{
public:
	virtual ~MenuState() {}
private:
    virtual void Enter( void ) override;
    virtual void Update( float deltaTime, float totalTime ) override;
    virtual void Exit( void ) override;
};

#endif
