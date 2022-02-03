#if !defined(_ECS_COMPONENT_ARRAY_HPP_)
#define _ECS_COMPONENT_ARRAY_HPP_
#pragma once

#include "EntityManager.h"

#include <bitset>
#include <assert.h>

template <class ComponentType>
class ComponentArray
{
public:
    /* Constructors, Destructor, Assignment Operators */
    ComponentArray() = delete;
    ComponentArray(EntityManager* entityManager);
    ~ComponentArray() = default;

    ComponentArray(const ComponentArray&) = delete;
	ComponentArray(ComponentArray&&) = delete;
	ComponentArray& operator=(const ComponentArray&) = delete;
	ComponentArray& operator=(ComponentArray&&) = delete;

    /* Public Interface */
    bool HasComponent(Entity e) const;
	ComponentType& AddComponent(Entity e);
	void RemoveComponent(Entity e);
	ComponentType& GetComponent(Entity e);
	ComponentType* GetComponents();

private:
    void UpdateEntitiesContainingComponent();

    ComponentType myComponents[MAX_ENTITIES];
    std::bitset<MAX_ENTITIES> myEntitiesContainingComponent;

    EntityManager* myEntityManager;
};

template <class ComponentType>
ComponentArray<ComponentType>::ComponentArray(EntityManager* entityManager)
    : myEntityManager(entityManager)
{
    assert(("entityManager is nullptr.", entityManager));
}

template <class ComponentType>
bool ComponentArray<ComponentType>::HasComponent(Entity e) const
{
    assert(("Invalid entity.", myEntityManager->Validate(e)));

    return myEntitiesContainingComponent.test(e);
}

template <class ComponentType>
ComponentType& ComponentArray<ComponentType>::AddComponent(Entity e)
{
    assert(("Invalid entity.", myEntityManager->Validate(e)));

    myEntitiesContainingComponent.set(e);
    myComponents[e] = ComponentType();

    return myComponents[e];
}

template <class ComponentType>
void ComponentArray<ComponentType>::RemoveComponent(Entity e)
{
    assert(("Invalid entity.", myEntityManager->Validate(e)));

    myEntitiesContainingComponent.reset(e);
}

template <class ComponentType>
ComponentType& ComponentArray<ComponentType>::GetComponent(Entity e)
{
    UpdateEntitiesContainingComponent();

    assert(("Entity does not have a component of this type.", HasComponent(e)));
    assert(("Invalid entity.", myEntityManager->Validate(e)));

    return myComponents[e];
}

template <class ComponentType>
ComponentType* ComponentArray<ComponentType>::GetComponents()
{
    UpdateEntitiesContainingComponent();

    return myComponents;
}

template <class ComponentType>
void ComponentArray<ComponentType>::UpdateEntitiesContainingComponent()
{
    // This is to make sure components on removed entities are also removed.
    myEntitiesContainingComponent &= myEntityManager->GetOccupiedEntities();
}

#endif // _ECS_COMPONENT_ARRAY_HPP_
