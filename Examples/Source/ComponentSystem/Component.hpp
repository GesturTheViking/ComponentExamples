#if !defined(_COMPSYSTEM_COMPONENT_HPP_)
#define _COMPSYSTEM_COMPONENT_HPP_
#pragma once

class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    Component(const Component&) = default;
    Component(Component&&) = default;
    Component& operator=(const Component&) = default;
    Component& operator=(Component&&) = default;

    virtual void Init(class GameObject* parent) = 0;
    virtual void Update(GameObject* parent, float dt) = 0;
    virtual void Render(GameObject* parent) = 0;
};

#endif // _COMPSYSTEM_COMPONENT_HPP_
