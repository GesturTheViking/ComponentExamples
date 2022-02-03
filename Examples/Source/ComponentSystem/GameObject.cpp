#include "GameObject.h"

#include "Component.hpp"

std::vector<GameObject*> GameObject::ourGameObjects;

GameObject::GameObject()
    : myComponents{}
{
    // Adding this GameObject to the global list of GameObjects.
    ourGameObjects.push_back(this);
}
GameObject::~GameObject()
{
    for (Component* comp : myComponents)
    {
        delete comp;
    }
    myComponents.clear();

    {
        // Removing this GameObject from the global list of GameObjects.
        const size_t size = ourGameObjects.size();
        for (size_t index = 0U; index < size; ++index)
        {
            if (ourGameObjects[index] == this)
            {
                ourGameObjects.erase(ourGameObjects.begin() + index);
                break;
            }
        }
    }
}

GameObject::GameObject(const GameObject& other)
{
    myComponents = other.myComponents;
}

GameObject::GameObject(GameObject&& other)
{
    myComponents = other.myComponents;
    other.myComponents.clear();
}

GameObject& GameObject::operator=(const GameObject& other)
{
    myComponents = other.myComponents;

    return *this;
}
GameObject& GameObject::operator=(GameObject&& other)
{
    myComponents = other.myComponents;
    other.myComponents.clear();

    return *this;
}

void GameObject::Init()
{
    for (Component* comp : myComponents)
    {
        comp->Init(this);
    }
}
void GameObject::Update(float dt)
{
    for (Component* comp : myComponents)
    {
        comp->Update(this, dt);
    }
}
void GameObject::Render()
{
    for (Component* comp : myComponents)
    {
        comp->Render(this);
    }
}

void GameObject::SetPosition(v2f pos)
{
    myPos = pos;
}
v2f GameObject::GetPosition() const
{
    return myPos;
}

void GameObject::InitAll()
{
    for (GameObject* obj : ourGameObjects)
    {
        obj->Init();
    }
}
void GameObject::UpdateAll(float dt)
{
    for (GameObject* obj : ourGameObjects)
    {
        obj->Update(dt);
    }
}
void GameObject::RenderAll()
{
    for (GameObject* obj : ourGameObjects)
    {
        obj->Render();
    }
}