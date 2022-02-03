#include "SpriteSystem.h"

#include "EntityManager.h"
#include "ComponentArray.hpp"
#include "Components.hpp"

#include "Engine.h"
using namespace Elia2D;

namespace Systems
{
    void Sprite(
        EntityManager* em,
        ComponentArray<TransformComponent>* trs,
        ComponentArray<SpriteComponent>* spr)
    {
        TransformComponent* trsList = trs->GetComponents();
        SpriteComponent* sprList = spr->GetComponents();

        for (Entity e = 0U; e < MAX_ENTITIES; ++e)
        {
            if (!em->Validate(e) || !trs->HasComponent(e) || !spr->HasComponent(e))
                continue;

            DrawTexturedQuad(sprList[e].myTexture, Convertv2f_v2(trsList[e].pos),
                sprList[e].mySize, sprList[e].myColor);
        }
    }
}