#if !defined(_ECS_ENTITY_CONSTANTS_HPP_)
#define _ECS_ENTITY_CONSTANTS_HPP_
#pragma once

#include <stdint.h>
using Entity = uint32_t;

constexpr Entity MAX_ENTITIES = 64;
constexpr Entity INVALID_ENTITY = Entity(-1);

#endif // _ECS_ENTITY_CONSTANTS_HPP_
