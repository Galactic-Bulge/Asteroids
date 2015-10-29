#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

// Windows
#include <Windows.h>

#include "Singleton.h"

#include "Keycodes.h"

struct KeyInfo
{
    bool isKeyDown;
    float keyHeldDuration;
};

enum MouseButton{ LMB = VK_LBUTTON, RMB = VK_RBUTTON, MMB = VK_MBUTTON };

class InputManager : public Singleton<InputManager>
{
private:
    bool m_didMouseMove;            // Boolean indicating if the mouse moved this frame
    POINT m_prevPos, m_currPos;     // Current and previous mouse positions
    KeyInfo m_keys[ NUM_KEYS ];     // Array of keyboard state

public:
    // Private to prevent construction outside instance() method
	InputManager(void)
        : m_didMouseMove(false)
    {
        /* Nothing to do. */
    }

	virtual ~InputManager()
    {
        /* Nothing to do. */
    }

    /*
     * Update the state of the InputManager
     * @param   deltaTime   Elapsed time between frames
     */
    void Update( float deltaTime );

    /*
     * Check if the provided key is pressed.
     * @param   key         The key to test.
     */
	bool IsKeyDown(SHORT key) const;

    /*
     * Check if the provided MouseButton is pressed.
     * @param   btn         The MouseButton to test.
     */
    bool IsMouseDown(MouseButton btn) const;

    /*
     * Check if the provided key is released.
     * @param   key         The key to test.
     */
	bool IsKeyUp(SHORT key) const;

    /*
     * Check if the provided MouseButton is released.
     * @param   btn         The MouseButton to test.
     */
    bool IsMouseUp(MouseButton btn) const;

    /*
     * Return the duration that the provided key has been held.
     * @param   key         The key to test.
     */
    float KeyHeldDuration(SHORT key) const;

    /*
     * Return the duration that the provided MouseButton has been held.
     * @param   btn         The MouseButton to test.
     */
    float MouseHeldDuration(MouseButton btn) const;

    /*
     * Returns the current location of the cursor.
     */
    POINT GetCurrentMousePos() const;

    /*
     * Returns the previous location of the cursor.
     */
    POINT GetPreviousMousePos() const;

    /*
     * Invoked by the message processing function on WM_MOUSEMOVE
     */
    void ReceiveMouseDown( HWND hWnd, WPARAM btnState, int x, int y );

    /*
     *  Invoked by the message processing function on:
     *      WM_LBUTTONUP
     *      WM_RBUTTONUP
     *      WM_MBUTTONUP
     */
    void ReceiveMouseUp( HWND hWnd, WPARAM btnState, int x, int y );

    /*
     *  Invoked by the message processing function on:
     *      WM_LBUTTONDOWN
     *      WM_RBUTTONDOWN
     *      WM_MBUTTONDOWN
     */
    void ReceiveMouseMove( HWND hWnd, WPARAM btnState, int x, int y );
};

#endif

