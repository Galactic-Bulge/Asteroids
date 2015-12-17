#include "MenuState.h"

// DirectX
#include <DirectXMath.h>

// Managers
#include "CameraManager.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "EventManager.h"

// Systems
#include "ClearSystem.h"
#include "UIUpdateSystem.h"
#include "UIRenderSystem.h"
#include "SkyboxSystem.h"
#include "SwapSystem.h"

// State
#include "StateMachine.h"
#include "GameStates.h"

#include "EntityFactory.h"

#include "StaticCamera.h"

using namespace DirectX;

#pragma region Destructor

MenuState::~MenuState(void)
{
    /* Nothing to do. */
}

#pragma endregion

#pragma region IGameState Interface

void MenuState::Enter(void)
{
    ResourceManager* pResource = ResourceManager::Instance();
    pResource->ParseJSONFile("json/resources.json");

    EventManager* pEvent = EventManager::Instance();
    pEvent->Register("PlayClicked", this);
    pEvent->Register("WarpBegin", this);
    pEvent->Register("WarpEnd", this);

    CameraManager* pCamera = CameraManager::Instance();
    pCamera->RegisterCamera<StaticCamera>("Menu_Camera", 0.0f, 0.0f, -5.0f);
    pCamera->SetActiveCamera("Menu_Camera");

    EntityManager* pEntity = EntityManager::Instance();
    pEntity->AddSystem<UIUpdateSystem>();
    pEntity->AddSystem<ClearSystem>();
    pEntity->AddSystem<SkyboxSystem>();
    pEntity->AddSystem<UIRenderSystem>();
    pEntity->AddSystem<SwapSystem>();

    GameEntity title = EntityFactory::CreateTextField
        (
            L"Asteroids",
            "SpaceAge54",
            XMFLOAT2{125.0f, 0.0f},
            XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}
        );

    GameEntity button = EntityFactory::CreateButton
        (
            XMFLOAT2{0.0f, 0.0f},
            800,
            600,
            "PlayClicked",
            L"Click to Play",
            "SpaceAge48",
            XMFLOAT2{75.0f, 275.0f},
            XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}
        );

    pEvent->Fire("WarpBegin", nullptr);
}

void MenuState::Update(float deltaTime, float totalTime)
{
    /* Nothing to do. */
}

void MenuState::Exit(void)
{
    EntityManager* pEntity = EntityManager::Instance();
    pEntity->Clear();
    pEntity->ClearSystems();

    EventManager* pEvent = EventManager::Instance();
    pEvent->UnRegister("WarpBegin", this);
    pEvent->UnRegister("WarpEnd", this);
    pEvent->UnRegister("PlayClicked", this);
}

#pragma endregion

void MenuState::EventRouter(const std::string& event, void* pData)
{
    if(event == "PlayClicked")
    {
        StateMachine<GameStates>* pState = StateMachine<GameStates>::Instance();
        pState->GoToState(GameStates::GAME);
    }
    else if(event == "WarpEnd")
    {
        EventManager* pEvent = EventManager::Instance();
        pEvent->Fire("WarpBegin", nullptr);
    }
}
