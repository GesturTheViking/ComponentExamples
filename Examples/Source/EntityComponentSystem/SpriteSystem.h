#if !defined(_ECS_SPRITE_SYSTEM_H_)
#define _ECS_SPRITE_SYSTEM_H_
#pragma once

class EntityManager;
template <class T>
class ComponentArray;
struct TransformComponent;
struct SpriteComponent;

namespace Systems
{
    void Sprite(
        EntityManager* em,
        ComponentArray<TransformComponent>* trs,
        ComponentArray<SpriteComponent>* spr);
}

#endif // _ECS_SPRITE_SYSTEM_H_
