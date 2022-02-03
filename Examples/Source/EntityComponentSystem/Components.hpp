#if !defined(_ECS_COMPONENTS_HPP_)
#define _ECS_COMPONENTS_HPP_
#pragma once

#include "Engine.h"
using namespace Elia2D;

struct TransformComponent
{
    v2f pos;
};

struct SpriteComponent
{
    Texture myTexture;
    v2 mySize;
    Color myColor;
};

#endif // _ECS_COMPONENTS_HPP_
