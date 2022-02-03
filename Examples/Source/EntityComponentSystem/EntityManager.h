#if !defined(_ECS_ENTITY_MANAGER_H_)
#define _ECS_ENTITY_MANAGER_H_
#pragma once

#include "EntityConstants.hpp"
#include <bitset>

class EntityManager
{
public:
    /* Constructors, Destructor, Assignment Operators */
    EntityManager();
    ~EntityManager() = default;

    EntityManager(const EntityManager& other);
    EntityManager(EntityManager&& other) noexcept;
    EntityManager& operator=(const EntityManager& other);
    EntityManager& operator=(EntityManager&& other);

    /* Public Interface */
    Entity GetEntity();
    void ReturnEntity(Entity e);

    bool Validate(Entity e) const;

    const std::bitset<MAX_ENTITIES>& GetOccupiedEntities() const;

private:
    Entity myAvailableEntitiesLL[MAX_ENTITIES];
	Entity myFirstAvailableEntity;
    std::bitset<MAX_ENTITIES> myOccupiedEntities;
};

#endif // _ECS_ENTITY_MANAGER_H_
