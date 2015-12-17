#ifndef UI_TEXT_COMPONENT_H
#define UI_TEXT_COMPONENT_H

// ECS
#include "Component.h"

// STD
#include <string>

// DirectX
#include <DirectXMath.h>

struct UITextComponent : public Component<UITextComponent>
{
public:
    std::wstring text;
    std::string spritefontID;
    DirectX::XMFLOAT2 position;
    DirectX::XMFLOAT4 color;
    
    /*
     * Construct an instance of the UITextComponent.
     * @param   text            The text to display.
     * @param   spritefontID    The id corresponding to the DirectX::SpriteFont.
     * @param   position        Position of the text in screen coordinates.
     * @param   color           The color of the text to be displayed.
     */
    UITextComponent(std::wstring text, std::string spritefontID, DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color)
        : text(text), spritefontID(spritefontID), position(position), color(color)
    {
        /* Nothing to do. */
    }

    /*
     * Destruct the UITextComponent instance.
     */
    virtual ~UITextComponent(void)
    {
        /* Nothing to do. */
    }
};

#endif
