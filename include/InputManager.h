#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

// Windows
#include <windows.h>

// Singleton
#include "Singleton.h"

// DirectXTK
#include <Keyboard.h>
#include <Mouse.h>

/*
 * Enum representing mouse buttons:
 * LMB - Left Mouse Button.
 * MMB - Middle Mouse Button.
 * RMB - Right Mouse Button.
 */
enum class MouseButton { LMB, MMB, RMB };

class InputManager : public Singleton<InputManager>
{
private:
    // Keyboard variables
    DirectX::Keyboard* m_pKeyboard;
    DirectX::Keyboard::State m_kbState;
    DirectX::Keyboard::KeyboardStateTracker* m_pKBStateTracker;

    // Mouse variables
    DirectX::Mouse* m_pMouse;
    DirectX::Mouse::State m_mouseState;
    DirectX::Mouse::ButtonStateTracker* m_pMouseStateTracker;
    bool m_didMouseMove;            // Boolean indicating if the mouse moved this frame
    POINT m_prevPos, m_currPos;     // Current and previous mouse positions

public:
	InputManager(void);
	virtual ~InputManager(void);

    /*
     * Set the HWND to be used by the InputManager.
     * @param   window      The window handle to use.
     */
    void SetWindow(HWND window);

    /*
     * Update the state of the InputManager
     * @param   deltaTime   Elapsed time between frames
     */
    void Update(float deltaTime);

    /*
     * Check if the provided key is down.
     * @param   key         The key to test.
     */
	bool IsKeyDown(SHORT key) const;

    /*
     * Check if the provided key is up.
     * @param   key         The key to test.
     */
	bool IsKeyUp(SHORT key) const;

    /*
     * Check if the provided key was pressed this frame.
     * @param   key         The key to test.
     */
	bool IsKeyPressed(SHORT key) const;

    /*
     * Check if the provided key was released this frame.
     * @param   key         The key to test.
     */
    bool IsKeyReleased(SHORT key) const;

    /*
     * Check if the provided MouseButton is down.
     * @param   btn         The MouseButton to test.
     */
    bool IsMouseDown(MouseButton btn) const;

    /*
     * Check if the provided MouseButton is up.
     * @param   btn         The MouseButton to test.
     */
    bool IsMouseUp(MouseButton btn) const;

    /*
     * Check if the provided MouseButton was pressed this frame.
     * @param   btn         The MouseButton to test.
     */
	bool IsMousePressed(MouseButton btn) const;

    /*
     * Check if the provided MouseButton was released this frame.
     * @param   btn         The MouseButton to test.
     */
    bool IsMouseReleased(MouseButton btn) const;

    /*
     * Returns the current location of the cursor.
     */
    POINT GetCurrentMousePos() const;

    /*
     * Returns the previous location of the cursor.
     */
    POINT GetPreviousMousePos() const;
};

#endif

