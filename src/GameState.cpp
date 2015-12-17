#include "GameState.h"

// STD
#include <ctime>
#include <cstdlib>

// Managers
#include "EntityFactory.h"
#include "EntityManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"

// Events
#include "EventManager.h"
#include "IEventListener.h"

// State
#include "StateMachine.h"
#include "GameStates.h"

// Systems
#include "PhysicsSystem.h"
#include "ScriptSystem.h"
#include "InputControllerSystem.h"
#include "ClearSystem.h"
#include "RenderSystem.h"
#include "SkyboxSystem.h"
#include "SwapSystem.h"
#include "PostEffectsSystem.h"
#include "UIRenderSystem.h"
#include "UIUpdateSystem.h"
#include "CollisionSystem.h"
#include "AttackSystem.h"
#include <ParticleSystem.h>

// For the DirectX Math library
using namespace DirectX;

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
void GameState::Enter(void)
{
    // Register resources with the ResourceManager
    if(!isInitialized)
    {
		CameraManager* camMan = CameraManager::Instance();
		camMan->RegisterCamera<Camera>("Main Camera", 0.0f, 0.0f, -5.0f);
		camMan->SetActiveCamera("Main Camera");

		this->LoadCurrentLevel();
		EventManager* pEventManager = EventManager::Instance();
		pEventManager->Register("WarpEnd", this);
		pEventManager->Register("AsteroidDestroyed", this);

        // Register Systems.
        EntityManager* pEntity = EntityManager::Instance();

        pEntity->AddSystemWithPriority<PhysicsSystem, 0>();
		pEntity->AddSystemWithPriority<CollisionSystem, 1>();
		pEntity->AddSystemWithPriority<InputControllerSystem, 2>();
        pEntity->AddSystemWithPriority<UIUpdateSystem, 3>();
        pEntity->AddSystemWithPriority<ClearSystem, 4>();
        pEntity->AddSystemWithPriority<RenderSystem, 5>();
		pEntity->AddSystemWithPriority<ParticleSystem, 6>();
        pEntity->AddSystemWithPriority<SkyboxSystem, 7>();
		pEntity->AddSystemWithPriority<PostEffectsSystem, 8>();
        pEntity->AddSystemWithPriority<UIRenderSystem, 9>();
		pEntity->AddSystemWithPriority<SwapSystem, 10>();
		pEntity->AddSystemWithPriority<ScriptSystem, 11>();
		pEntity->AddSystemWithPriority<AttackSystem, 12>();

        // Make a SpotLight
        GameEntity spotlight = EntityFactory::CreateSpotlight
        (
            XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), // Color
            XMFLOAT3(0.0f, 0.0f, -1.0f),      // Position
            XMFLOAT3(0.0f, 0.0f, 1.0f),       // Direction
            30.0f,                            // Specular Exponent
            1.0f                              // SpotLight Power
       );

		//Make Player
		GameEntity player = EntityFactory::CreatePlayer(XMFLOAT3(0.0f, 0.0f, 0.0f));

        isInitialized = true;
    }
}

#pragma endregion

void GameState::LoadCurrentLevel()
{
	srand(static_cast<std::time_t>(0));
	this->currentLevel++;
	int span = 10;
	int toAdd = 30 + currentLevel * 5;
	this->asteroids = toAdd;
	for (int i = 0; i < toAdd; ++i)
	{
		XMFLOAT3 position = XMFLOAT3(static_cast<float>(rand()*rand() % (span * 2) - span), static_cast<float>(rand()*rand() % (span * 2) - span), i * 5.0f + 155.0f);
        XMFLOAT3 velocity{0.0f, 0.0f, -15.0f - (rand() % 15)};
        XMFLOAT3 acceleration{0.0f, 0.0f, -1.0f + currentLevel * -2.0f};
        XMFLOAT3 rotation{rand() * 3.0f, rand() * 3.0f, rand() * 3.0f};
		float scale = 1.0f + rand() * 0.0001f;

        EntityFactory::CreateAsteroid(position, velocity, acceleration, rotation, scale, i+1);
	}
}

// One Listener can route multiple events, check the name to route properly
void GameState::EventRouter(const std::string& name, void* data)
{
    // Finished warp, initialize new level.
	if (name == "WarpEnd")
    {
		LoadCurrentLevel();
    }
    // Called whenever an asteroid is shot or auto destructs.
    else if (name == "AsteroidDestroyed")
    {
		asteroids--;
		if (asteroids <= 0) {
			EventManager::Instance()->Fire("WarpBegin", nullptr); //TODO change to WarpStart
		}
	}
}

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
void GameState::Update(float deltaTime, float totalTime)
{
    /* Nothing to do. */
}

void GameState::Exit(void) 
{ 
    /* Nothing to do. */ 
}
