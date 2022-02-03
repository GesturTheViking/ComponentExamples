#if !defined(_COMPSYSTEM_SPRITE_COMPONENT_H_)
#define _COMPSYSTEM_SPRITE_COMPONENT_H_
#pragma once

#include "Component.hpp"

#include "Engine.h"
using namespace Elia2D;

class SpriteComponent : public Component
{
public:
    /* Constructors, Destructor, Assignment Operators */
    SpriteComponent();
    ~SpriteComponent();

    SpriteComponent(const SpriteComponent& sc);
    SpriteComponent(SpriteComponent&& sc) noexcept;
    SpriteComponent& operator=(const SpriteComponent& sc);
    SpriteComponent& operator=(SpriteComponent&& sc) noexcept;

    /* Component Virtual Methods */
    void Init(class GameObject* parent);
    void Update(GameObject* parent, float dt);
    void Render(GameObject* parent);

    /* Setters */
    void SetSpritePath(const char* const path);
    void SetSize(v2 size);
    void SetColor(Color color);

    /* Getters */
    v2 GetSize() const;
    Color GetColor() const;

private:
    Texture myTexture;
    v2 mySize;
    Color myColor;
    FixedString256 myTexturePath;
};

#endif // _COMPSYSTEM_SPRITE_COMPONENT_H_
