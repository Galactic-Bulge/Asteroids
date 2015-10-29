#include "InputManager.h"

/*
 * Update the state of the InputManager
 * @param   deltaTime   Elapsed time between frames
 */
void InputManager::Update( float deltaTime )
{
    // Retrieve the entire keyboard state from Windows
    BYTE keys[ NUM_KEYS ];
    GetKeyboardState( keys );

    // Update state of KeyInfo vector
    for( SHORT i = 0; i <= 0xFF; ++i )
    {
        KeyInfo& info = m_keys[ i ];

        // Update hold duration
		bool isKeyDown = ( keys[ i ] & 0x80 ) != 0x0;
        if( isKeyDown )
        {
            info.keyHeldDuration += deltaTime;
        }
        // Reset hold duration if key is released
        else if( info.isKeyDown )
        {
            info.keyHeldDuration = 0.0f;
        }

        info.isKeyDown = isKeyDown;
    }

    // Update PreviousMousePos if the mouse did not move
    if( !m_didMouseMove )
    {
        m_prevPos.x = m_currPos.x;
        m_prevPos.y = m_currPos.y;
    }
    m_didMouseMove = false;
}

/*
 * Check if the provided key is pressed.
 * @param   key         The key to test.
 */
bool InputManager::IsKeyDown(SHORT key) const
{
    return m_keys[ key ].isKeyDown;
}

/*
 * Check if the provided key is released.
 * @param   key         The key to test.
 */
bool InputManager::IsKeyUp(SHORT key) const
{
    return !( m_keys[ key ].isKeyDown );
}

/*
 * Return the duration that the provided key has been held.
 * @param   key         The key to test.
 */
float InputManager::KeyHeldDuration(SHORT key) const
{
    return m_keys[ key ].keyHeldDuration;
}

/*
 * Check if the provided MouseButton is pressed.
 * @param   btn         The MouseButton to test.
 */
bool InputManager::IsMouseDown( MouseButton btn ) const
{
    return m_keys[ btn ].isKeyDown;
}

/*
 * Check if the provided MouseButton is released.
 * @param   btn         The MouseButton to test.
 */
bool InputManager::IsMouseUp( MouseButton btn ) const
{
    return !( m_keys[ btn ].isKeyDown );
}

/*
 * Return the duration that the provided MouseButton has been held.
 * @param   btn         The MouseButton to test.
 */
float InputManager::MouseHeldDuration( MouseButton btn ) const
{
    return m_keys[ btn ].keyHeldDuration;
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

/*
 * Invoked by the message processing function on WM_MOUSEMOVE
 */
void InputManager::ReceiveMouseDown( HWND hWnd, WPARAM btnState, int x, int y )
{
    m_prevPos.x = m_currPos.x;
    m_prevPos.y = m_currPos.y;
    m_currPos.x = x;
    m_currPos.y = y;

    SetCapture(hWnd);
}

/*
 *  Invoked by the message processing function on:
 *      WM_LBUTTONUP
 *      WM_RBUTTONUP
 *      WM_MBUTTONUP
 */
void InputManager::ReceiveMouseUp( HWND hWnd, WPARAM btnState, int x, int y )
{
    m_prevPos.x = m_currPos.x;
    m_prevPos.y = m_currPos.y;
    m_currPos.x = x;
    m_currPos.y = y;

    ReleaseCapture();
}

/*
 *  Invoked by the message processing function on:
 *      WM_LBUTTONDOWN
 *      WM_RBUTTONDOWN
 *      WM_MBUTTONDOWN
 */
void InputManager::ReceiveMouseMove( HWND hWnd, WPARAM btnState, int x, int y )
{
    m_prevPos.x = m_currPos.x;
    m_prevPos.y = m_currPos.y;
    m_currPos.x = x;
    m_currPos.y = y;

    m_didMouseMove = true;
}
