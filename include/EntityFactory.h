#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

// ECS
#include "GameEntity.h"

// DirectX
#include <DirectXMath.h>

// STD
#include <string>

class EntityFactory
{
private:
    EntityFactory(void) = delete;
    ~EntityFactory(void) = delete;
    EntityFactory(const EntityFactory& rhs) = delete;
    EntityFactory& operator=(const EntityFactory& rhs) = delete;
    EntityFactory(EntityFactory&& rhs) = delete;
    EntityFactory& operator=(EntityFactory&& rhs) = delete;

public:
    static GameEntity CreateTextField(std::wstring text, std::string spritefont_id, DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color);
    static GameEntity CreateButton(DirectX::XMFLOAT2 center, float width, float height, std::string event, std::wstring text, std::string spritefont_id, DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color);
    static GameEntity CreateAsteroid(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 rotation, float scale, int id);
    static GameEntity CreatePlayer(DirectX::XMFLOAT3 position);
    static GameEntity CreateDirectionalLight(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 direction, float specularExponent);
    static GameEntity CreatePointLight(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 position, float specularExponent);
    static GameEntity CreateSpotlight(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction, float specularExponent, float spotlightPower);
};

#endif
