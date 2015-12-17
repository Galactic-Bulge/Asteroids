#include "EntityFactory.h"

// ECS
#include "EntityManager.h"
#include "ResourceManager.h"

// Components
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"
#include "InputComponent.h"
#include "LightComponent.h"
#include "ScriptComponent.h"
#include "AABBComponent.h"
#include "ButtonComponent.h"
#include "UIRenderComponent.h"
#include "UITextComponent.h"
#include "CollisionComponent.h"
#include "AttackComponent.h"
#include "AsteroidRenderComponent.h"
#include <ParticleComponent.h>

// Scripts
#include "AutoDestructScript.h"

using namespace DirectX;

GameEntity EntityFactory::CreateTextField(std::wstring text, std::string spritefont_id, DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color)
{
    EntityManager* pEntity = EntityManager::Instance();
    ResourceManager* pResource = ResourceManager::Instance();

    GameEntity textfield = pEntity->Create("Text");
    pEntity->AddComponent<UITextComponent>(textfield, text, spritefont_id, position, color);

    return textfield;
}

GameEntity EntityFactory::CreateButton(DirectX::XMFLOAT2 center, float width, float height, std::string event, std::wstring text, std::string spritefont_id, DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color)
{
    EntityManager* pEntity = EntityManager::Instance();
    ResourceManager* pResource = ResourceManager::Instance();

    GameEntity button = pEntity->Create("Button");
    pEntity->AddComponent<ButtonComponent>(button, true, event);
    pEntity->AddComponent<AABBComponent>(button, XMFLOAT2{center.x + 400.0f, center.y + 300.0f}, width / 2.0f, height / 2.0f); // TODO: Replace with half window width and half window height
    pEntity->AddComponent<UITextComponent>(button, text, spritefont_id, position, color);

    return button;
}

GameEntity EntityFactory::CreateAsteroid(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 rotation, float scale, int id)
{
    EntityManager* pEntity = EntityManager::Instance();
    ResourceManager* pResource = ResourceManager::Instance();

    GameEntity asteroid = pEntity->Create("Asteroid");
    pEntity->AddComponent<CollisionComponent>(asteroid, 0.95f*scale, position);
    pEntity->AddComponent<AsteroidRenderComponent>(asteroid, id);
    pEntity->AddComponent<RenderComponent>(asteroid, pResource->GetMaterial("asteroid"), pResource->GetMesh("Sphere"));
    pEntity->AddComponent<PhysicsComponent>(asteroid, velocity, acceleration);
    TransformComponent* pTransform = pEntity->AddComponent<TransformComponent>(asteroid, position);
    pTransform->transform.SetRotation(rotation);
    pTransform->transform.SetScale(scale);
    ScriptComponent* script = pEntity->AddComponent<ScriptComponent>(asteroid);
    script->AddScript<AutoDestructScript>(-5.0f);

    return asteroid;
}

GameEntity EntityFactory::CreatePlayer(DirectX::XMFLOAT3 position)
{
    EntityManager* pEntity = EntityManager::Instance();
	ResourceManager* pResource = ResourceManager::Instance();

    GameEntity player = pEntity->Create("Player");
    pEntity->AddComponent<RenderComponent>(player, pResource->GetMaterial("ship"), pResource->GetMesh("Ship"));
    pEntity->AddComponent<InputComponent>(player, 50.0f);
    pEntity->AddComponent<CollisionComponent>(player, *pResource->GetMesh("Ship"), XMFLOAT3(0, 0, 0), 0.0007f);
    pEntity->AddComponent<AttackComponent>(player, 5.0f);
    TransformComponent* pTrans = pEntity->AddComponent<TransformComponent>(player, position);
    pTrans->transform.SetScale(.001f);
    PhysicsComponent* pPhysics = pEntity->AddComponent<PhysicsComponent>(player, XMVectorZero(), XMVectorZero());
    pPhysics->drag = 0.95f;
    pPhysics->rotationalDrag = 0.85f;

	// Particles
	Particle p(XMFLOAT4(1, 0, 0, 1),
			   XMFLOAT4(1, 1, 0.1f, 0.8f),
			   XMFLOAT4(1, 0.5f, 0.1f, 0.9f),
			   XMFLOAT3(0, 0, -0.9),
			   XMFLOAT3(0, 0, -0.3),
			   XMFLOAT3(0, 0, 0),
			   0.9f,
			   0.4f,
			   0.1f,
			   0.0f,
			   0);

	ParticleGenerator* pGML = new ParticleGenerator(p, XMFLOAT3( 0.0f,  -0.16f, -1.05f), 0.08f, 0.00001, 10);
	ParticleGenerator* pGMU = new ParticleGenerator(p, XMFLOAT3( 0.0f,  +0.04f, -1.05f), 0.08f, 0.00001, 10);
	ParticleGenerator* pGRU = new ParticleGenerator(p, XMFLOAT3(+0.33f, +0.04f, -1.05f), 0.08f, 0.00001, 10);
	ParticleGenerator* pGLU = new ParticleGenerator(p, XMFLOAT3(-0.33f, +0.04f, -1.05f), 0.08f, 0.00001, 10);
	ParticleGenerator* pGRL = new ParticleGenerator(p, XMFLOAT3(+0.35f, -0.16f, -1.05f), 0.08f, 0.00001, 10);
	ParticleGenerator* pGLL = new ParticleGenerator(p, XMFLOAT3(-0.35f, -0.16f, -1.05f), 0.08f, 0.00001, 10);


	ParticleComponent* pParticles = pEntity->AddComponent<ParticleComponent>(player);
	pParticles->AddGenerator(pGML);
	pParticles->AddGenerator(pGMU);
	pParticles->AddGenerator(pGRU);
	pParticles->AddGenerator(pGLU);
	pParticles->AddGenerator(pGRL);
	pParticles->AddGenerator(pGLL);
    return player;
}

GameEntity EntityFactory::CreateDirectionalLight(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 direction, float specularExponent)
{
    EntityManager* pEntity = EntityManager::Instance();

    GameEntity dirLight = pEntity->Create("DirectionalLight");
    pEntity->AddComponent<DirectionalLightComponent>
    (
        dirLight,
        color,
        direction,
        specularExponent
    );

    return dirLight;
}

GameEntity EntityFactory::CreatePointLight(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 position, float specularExponent)
{
    EntityManager* pEntity = EntityManager::Instance();

    GameEntity pointLight = pEntity->Create("PointLight");
    pEntity->AddComponent<PointLightComponent>
    (
        pointLight,
        color,
        position,
        specularExponent
    );

    return pointLight;
}

GameEntity EntityFactory::CreateSpotlight(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction, float specularExponent, float spotlightPower)
{
    EntityManager* pEntity = EntityManager::Instance();

    GameEntity spotlight = pEntity->Create("SpotLight");
    pEntity->AddComponent<SpotLightComponent>
    (
        spotlight,
        color,
        position,
        direction,
        specularExponent,
        spotlightPower
    );

    return spotlight;
}
