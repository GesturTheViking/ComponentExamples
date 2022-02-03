#if !defined(_COMPSYSTEM_GAME_OBJECT_H_)
#define _COMPSYSTEM_GAME_OBJECT_H_
#pragma once

#include <vector>

#include "Engine.h"
using namespace Elia2D;

class GameObject
{
public:
    /* Constructors, Destructor, Assignment Operators */
    GameObject();
    virtual ~GameObject();

    GameObject(const GameObject& other);
    GameObject(GameObject&& other) noexcept;
    GameObject& operator=(const GameObject& other);
    GameObject& operator=(GameObject&& other) noexcept;

    /* Virtual Functions */
    virtual void Init();
    virtual void Update(float dt);
    virtual void Render();

    /* Components */
    template <class ComponentType>
    ComponentType* AddComponent()
    {
        ComponentType* comp = new ComponentType();
        myComponents.push_back(comp);
        return comp;
    }
    template <class ComponentType>
    ComponentType* GetComponent()
    {
        for (Component* comp : myComponents)
        {
            ComponentType* ptr = dynamic_cast<ComponentType*>(comp);
            if (ptr)
            {
                return ptr;
            }
        }

        return nullptr;
    }

    /* Position */
    void SetPosition(v2f pos);
    v2f GetPosition() const;

    /* Managing */
    static void InitAll();
    static void UpdateAll(float dt);
    static void RenderAll();

protected:
    std::vector<class Component*> myComponents;
    v2f myPos;

    static std::vector<GameObject*> ourGameObjects;
};

#endif // _COMPSYSTEM_GAME_OBJECT_H_
