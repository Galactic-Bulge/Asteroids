#include "ExitState.h"

// DirectX
#include <d3d11.h>
#include "DXMacros.h"

// Managers
#include "InputManager.h"

// State
#include "StateMachine.h"
#include "GameStates.h"

void ExitState::Enter( void )
{
	/* Nothing to do. */
}

void ExitState::Update( float deltaTime, float totalTime )
{
    // Check input
    InputManager* pInput = InputManager::Instance();
    StateMachine<GameStates>* pState = StateMachine<GameStates>::Instance();

    if( pInput->IsKeyDown( '1' ) )
    {
        pState->GoToState( GameStates::MENU );
    }
    else if( pInput->IsKeyDown( '2' ) )
    {
        pState->GoToState( GameStates::GAME );
    }
}

void ExitState::Exit( void )
{
	/* Nothing to do. */
}
