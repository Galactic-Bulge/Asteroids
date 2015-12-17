#include "InputManager.h"

using namespace DirectX;

InputManager::InputManager(void)
    : m_didMouseMove(false)
{
    ZeroMemory(&m_prevPos, sizeof(POINT));
    ZeroMemory(&m_currPos, sizeof(POINT));
    ZeroMemory(&m_kbState, sizeof(Keyboard::State));
    ZeroMemory(&m_mouseState, sizeof(Mouse::State));

    m_pKeyboard = new Keyboard();
    m_pKBStateTracker = new Keyboard::KeyboardStateTracker();
    m_pMouse = new Mouse();
    m_pMouseStateTracker = new Mouse::ButtonStateTracker();
}

InputManager::~InputManager(void)
{
    delete m_pKeyboard;
    delete m_pKBStateTracker;
    delete m_pMouse;
    delete m_pMouseStateTracker;
}

void InputManager::SetWindow(HWND window)
{
    m_pMouse->SetWindow(window);
}

/*
 * Update the state of the InputManager
 * @param   deltaTime   Elapsed time between frames
 */
void InputManager::Update(float deltaTime)
{
    // Update Keyboard
    m_kbState = m_pKeyboard->GetState();
    m_pKBStateTracker->Update(m_kbState);

    // Update Mouse
    m_mouseState = m_pMouse->GetState();
    m_pMouseStateTracker->Update(m_mouseState);

    m_prevPos.x = m_currPos.x;
    m_prevPos.y = m_currPos.y;

    m_currPos.x = m_mouseState.x;
    m_currPos.y = m_mouseState.y;

    m_didMouseMove = (m_prevPos.x == m_currPos.x && m_prevPos.y == m_currPos.y);
}

/*
 * Check if the provided key is pressed.
 * @param   key         The key to test.
 */
bool InputManager::IsKeyDown(SHORT key) const
{
    return m_kbState.IsKeyDown(static_cast<Keyboard::Keys>(key));
}

/*
 * Check if the provided key is released.
 * @param   key         The key to test.
 */
bool InputManager::IsKeyUp(SHORT key) const
{
    return !IsKeyDown(key);
}

/*
 * Check if the provided key was pressed this frame.
 * @param   key         The key to test.
 */
bool InputManager::IsKeyPressed(SHORT key) const
{
    return m_pKBStateTracker->IsKeyPressed(static_cast<Keyboard::Keys>(key));
}

/*
 * Check if the provided key was released this frame.
 * @param   key         The key to test.
 */
bool InputManager::IsKeyReleased(SHORT key) const
{
    return m_pKBStateTracker->IsKeyReleased(static_cast<Keyboard::Keys>(key));
}

/*
 * Check if the provided MouseButton is pressed.
 * @param   btn         The MouseButton to test.
 */
bool InputManager::IsMouseDown(MouseButton btn) const
{
    switch(btn)
    {
        case MouseButton::LMB:
            return m_pMouseStateTracker->leftButton == Mouse::ButtonStateTracker::HELD;

        case MouseButton::MMB:
            return m_pMouseStateTracker->middleButton == Mouse::ButtonStateTracker::HELD;

        case MouseButton::RMB:
            return m_pMouseStateTracker->rightButton == Mouse::ButtonStateTracker::HELD;

        default:
            return false;
    }
}

/*
 * Check if the provided MouseButton is released.
 * @param   btn         The MouseButton to test.
 */
bool InputManager::IsMouseUp(MouseButton btn) const
{
    return !IsMouseDown(btn);
}

/*
 * Check if the provided MouseButton was pressed this frame.
 * @param   btn         The MouseButton to test.
 */
bool InputManager::IsMousePressed(MouseButton btn) const
{
    switch(btn)
    {
        case MouseButton::LMB:
            return m_pMouseStateTracker->leftButton == Mouse::ButtonStateTracker::PRESSED;

        case MouseButton::MMB:
            return m_pMouseStateTracker->middleButton == Mouse::ButtonStateTracker::PRESSED;

        case MouseButton::RMB:
            return m_pMouseStateTracker->rightButton == Mouse::ButtonStateTracker::PRESSED;

        default:
            return false;
    }
}

/*
 * Check if the provided MouseButton was released this frame.
 * @param   btn         The MouseButton to test.
 */
bool InputManager::IsMouseReleased(MouseButton btn) const
{
    switch(btn)
    {
        case MouseButton::LMB:
            return m_pMouseStateTracker->leftButton == Mouse::ButtonStateTracker::RELEASED;

        case MouseButton::MMB:
            return m_pMouseStateTracker->middleButton == Mouse::ButtonStateTracker::RELEASED;

        case MouseButton::RMB:
            return m_pMouseStateTracker->rightButton == Mouse::ButtonStateTracker::RELEASED;

        default:
            return false;
    }
}

/*
 * Returns the current location of the cursor.
 */
POINT InputManager::GetCurrentMousePos() const
{
    return m_currPos;
}

/*
 * Returns the previous location of the cursor.
 */
POINT InputManager::GetPreviousMousePos() const
{
    return m_prevPos;
}
