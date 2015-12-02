#include "MenuState.h"

// DirectX
#include <d3d11.h>
#include "DXMacros.h"

// Managers
#include "InputManager.h"

// State
#include "StateMachine.h"
#include "GameStates.h"

void MenuState::Enter( void )
{
	/* Nothing to do. */
}

void MenuState::Update( float deltaTime, float totalTime )
{
    // Check input
    InputManager* pInput = InputManager::Instance();
    StateMachine<GameStates>* pState = StateMachine<GameStates>::Instance();
    if( pInput->IsKeyDown( '2' ) )
    {
        pState->GoToState( GameStates::GAME );
    }
    else if( pInput->IsKeyDown( '3' ) )
    {
        pState->GoToState( GameStates::EXIT );
    }
}

void MenuState::Exit( void )
{
	/* Nothing to do. */
}
