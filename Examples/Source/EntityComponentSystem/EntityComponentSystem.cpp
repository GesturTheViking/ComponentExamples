#include "EntityManager.h"
#include "ComponentArray.hpp"
#include "Components.hpp"

#include "SpriteSystem.h"

#include "Engine.h"
using namespace Elia2D;

struct GameState
{
    EntityManager entityManager;

    ComponentArray<TransformComponent> transforms = &entityManager;
    ComponentArray<SpriteComponent> sprites = &entityManager;
} gameState{};

Entity testObj = INVALID_ENTITY;

void Init()
{
    testObj = gameState.entityManager.GetEntity();

    gameState.transforms.AddComponent(testObj).pos = { 300.0f, 300.0f };

    SpriteComponent& sprComp = gameState.sprites.AddComponent(testObj);
    sprComp.myTexture = CreateTexture("../Assets/dawdwdawda.png", &sprComp.mySize);
}

void Update(float dt)
{
    static float totalTime = 0.0f;
    totalTime += dt / 2.0f;

    gameState.transforms.GetComponent(testObj).pos = { 400.0f + 300.0f * abs(sinf(totalTime)), 300.f };

    uint8_t color = uint8_t((abs(sinf(totalTime))) * 255.0f);
    gameState.sprites.GetComponent(testObj).myColor = { color, 0, uint8_t(255 - color) };

    /* Run systems here */
    Systems::Sprite(&gameState.entityManager, &gameState.transforms, &gameState.sprites);
}

int main()
{
    Start("Entity-Component-System Example", &Update, &Init);

    return 0;
}