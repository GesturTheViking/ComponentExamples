#include "EntityManager.h"

#include <assert.h>

EntityManager::EntityManager()
    : myFirstAvailableEntity(0)
    , myAvailableEntitiesLL{}
    , myOccupiedEntities{}
{
    for (Entity e = 0U; e < MAX_ENTITIES; ++e)
    {
        myAvailableEntitiesLL[e] = e + 1;
    }
}

EntityManager::EntityManager(const EntityManager& other)
    : myFirstAvailableEntity(other.myFirstAvailableEntity)
    , myAvailableEntitiesLL{}
    , myOccupiedEntities(other.myOccupiedEntities)
{
    memcpy(myAvailableEntitiesLL, other.myAvailableEntitiesLL, sizeof(myAvailableEntitiesLL));
}
EntityManager::EntityManager(EntityManager&& other) noexcept
    : myFirstAvailableEntity(other.myFirstAvailableEntity)
    , myAvailableEntitiesLL{}
    , myOccupiedEntities(other.myOccupiedEntities)
{
    memcpy(myAvailableEntitiesLL, other.myAvailableEntitiesLL, sizeof(myAvailableEntitiesLL));
}
EntityManager& EntityManager::operator=(const EntityManager& other)
{
    myFirstAvailableEntity = other.myFirstAvailableEntity;
    myOccupiedEntities = other.myOccupiedEntities;
    memcpy(myAvailableEntitiesLL, other.myAvailableEntitiesLL, sizeof(myAvailableEntitiesLL));

    return *this;
}
EntityManager& EntityManager::operator=(EntityManager&& other)
{
    myFirstAvailableEntity = other.myFirstAvailableEntity;
    myOccupiedEntities = other.myOccupiedEntities;
    memcpy(myAvailableEntitiesLL, other.myAvailableEntitiesLL, sizeof(myAvailableEntitiesLL));

    return *this;
}

Entity EntityManager::GetEntity()
{
    assert(("There are no available entities.", myFirstAvailableEntity < MAX_ENTITIES));

    Entity newEntity = myFirstAvailableEntity;
    myFirstAvailableEntity = myAvailableEntitiesLL[myFirstAvailableEntity];
    myAvailableEntitiesLL[newEntity] = INVALID_ENTITY;
    myOccupiedEntities.set(newEntity);

    return newEntity;
}
void EntityManager::ReturnEntity(Entity e)
{
    assert(("Invalid entity.", Validate(e)));
    assert(("Attempting to return already available entity.", myAvailableEntitiesLL[e] >= MAX_ENTITIES));

    myOccupiedEntities.reset(e);
    myAvailableEntitiesLL[e] = myFirstAvailableEntity;
    myFirstAvailableEntity = e;
}

bool EntityManager::Validate(Entity e) const
{
    if (e >= MAX_ENTITIES)
        return false;
    return myOccupiedEntities.test(e);
}

const std::bitset<MAX_ENTITIES>& EntityManager::GetOccupiedEntities() const
{
    return myOccupiedEntities;
}