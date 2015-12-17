#include "UIUpdateSystem.h"

// Managers
#include "EntityManager.h"
#include "InputManager.h"
#include "EventManager.h"


// Components
#include "AABBComponent.h"
#include "ButtonComponent.h"

using namespace DirectX;

/*
 * TODO
 */
UIUpdateSystem::UIUpdateSystem(void)
{
    m_pInput = InputManager::Instance();
    m_pEvent = EventManager::Instance();
}

/*
 * TODO
 */
UIUpdateSystem::~UIUpdateSystem(void)
{
    /* Nothing to do. */
}

/*
 * System Interface.
 */
void UIUpdateSystem::Update(EntityManager* pManager, float dt, float tt)
{
    for(auto& entity : pManager->EntitiesWithComponents<AABBComponent, ButtonComponent>())
    {
        ButtonComponent* pButton = pManager->GetComponent<ButtonComponent>(entity);

        // Test if the button is enabled and the mouse was pressed this frame.
        if(!pButton->isEnabled || !m_pInput->IsMousePressed(MouseButton::LMB))
        {
            continue;
        }

        AABBComponent* pAABB = pManager->GetComponent<AABBComponent>(entity);
        AABB& bounds = pAABB->boundingBox;

        // Convert from POINT To XMFLOAT2.
        POINT pos = m_pInput->GetCurrentMousePos();
        XMFLOAT2 point{static_cast<float>(pos.x), static_cast<float>(pos.y)};

        // Test if the mouse was within the bounds of the button.
        if(bounds.ContainsPoint(point))
        {
            m_pEvent->Fire(pButton->event, nullptr);
        }
    }
}
