#ifndef EXIT_STATE_H
#define EXIT_STATE_H

// State
#include "IGameState.h"

// DirectX
#include <d3d11.h>

class ExitState : public IGameState
{
private:
    virtual void Enter( void ) override;
    virtual void Update( float deltaTime, float totalTime ) override;
    virtual void Exit( void ) override;
    
public:
	virtual ~ExitState() {}
};

#endif
