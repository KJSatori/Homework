#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../../Math/Transform.h"
#include "../../Render/Renderer.h"
#include "../Component/Component.h"
#include <vector>
#include <memory>

using namespace std;

struct GameObject
{
    Transform transform;
    vector<shared_ptr<Component>> components;

    bool isDestroyed = false;

    GameObject() {};

    void AddComponent(const shared_ptr<Component>& component)
    {
        components.push_back(component);
    }

    template<typename T>
    shared_ptr<T> GetComponent()
    {
        for (auto component : components)
        {
            shared_ptr<T> casted = dynamic_pointer_cast<T>(component);
            if (casted)
            {
                return casted;
            }
        }
        return nullptr;
    }

    virtual void Awake()
    {
        for (auto component : components) component->Awake();
    }

    virtual void Start()
    {
        for (auto component : components) component->Start();
    }

    virtual void Update(float deltaTime)
    {
        for (auto component : components) component->Update(deltaTime);
    }

    virtual void Render(Renderer& renderer)
    {
        for (auto component : components) component->Render(renderer);
    }

    virtual void Destroy()
    {
        for (auto component : components) component->Destroy();
        isDestroyed = true;
    }
};

#endif