#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#include "Component.h"

// DirectX
#include <DirectXMath.h>

// STD
#include <cstdint>

struct DirectionalLightComponent : public Component<DirectionalLightComponent>
{
    static constexpr std::uint32_t MAX_LIGHTS{10};
    
    struct Light
    {
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT3 direction;
        float specularIntensity;
    } light;

    DirectionalLightComponent(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 direction, float specularIntensity)
    {
        light.color = color;
        light.direction = direction;
        light.specularIntensity = specularIntensity;
    }

    virtual ~DirectionalLightComponent(void) {}
};

struct PointLightComponent : public Component<PointLightComponent>
{
    static constexpr std::uint32_t MAX_LIGHTS{10};

    struct Light
    {
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT3 position;
        float specularIntensity;
    } light;

    PointLightComponent(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 position, float specularIntensity)
    {
        light.color = color;
        light.position = position;
        light.specularIntensity = specularIntensity;
    }

    virtual ~PointLightComponent(void) {}
};

struct SpotLightComponent : public Component<SpotLightComponent>
{
    static constexpr std::uint32_t MAX_LIGHTS{10};

    struct Light
    {
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT3 position;
        float specularIntensity;
        DirectX::XMFLOAT3 direction;
        float spotLightPower;
    } light;

    SpotLightComponent(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction, float specularIntensity, float spotLightPower)
    {
        light.color = color;
        light.direction = direction;
        light.position = position;
        light.specularIntensity = specularIntensity;
        light.spotLightPower = spotLightPower;
    }

    virtual ~SpotLightComponent(void) {}
};

#endif
