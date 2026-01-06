#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#ifndef BORDER_SIZE
#define BORDER_SIZE
#define BORDER_WIDTH 800
#define BORDER_HEIGHT 600
#endif

#include "../../Math/Transform.h"
#include "../../Render/Renderer.h"
#include "../Component/Component.h"
#include "../../../Game/Tag.h"
#include "../Prefab/PrefabManager.h"
#include "../Component/Debug.h"
#include <vector>
#include <string>
#include <memory>
#include <SDL2/SDL.h>

using namespace std;

struct Scene;

struct GameObject : public std::enable_shared_from_this<GameObject>
{
private:
    Scene* scene = nullptr; 

public: 
    bool queued = false; // 临时标志：是否已入队等待加入 Scene

    // 名称字段
    string name;
    Tag tag;
    Transform transform;
    // 父物体
    GameObject* parent = nullptr;
    // 子物体
    vector<shared_ptr<GameObject>> children;
    // 组件
    vector<shared_ptr<Component>> components;

    bool isDestroyed = false;
    bool isActive = true;

    GameObject(const string& n = "Unnamed", const Tag& t = Tag::None): name(n), tag(t) {};

    void SetTag(Tag t) { tag = t; }
    Tag GetTag() const { return tag; }

    void AddComponent(const shared_ptr<Component>& component)
    {
        components.push_back(component);
    }

    void AddChild(const shared_ptr<GameObject>& child)
    {
        child->parent = this;
        children.push_back(child);
        child->Awake();
    }

    void SetActive(bool active) { isActive = active; } 
    bool IsActive() { return isActive; }

    void SetScene(Scene* s)
    {
        scene = s;
    }

    Scene* GetScene()
    {
        if (scene == nullptr) Debug::Log("[Warning] " + name + " 返回一个空scene指针");
        return scene;
    }

    Scene* GetSceneNoWarn()
    {
        return scene;
    }

    void RemoveChild(GameObject* child)
    {
        if (!child) return;
        child->parent = nullptr;
        children.erase(
            remove_if(children.begin(), children.end(),
            [child](const shared_ptr<GameObject>& c)
            {
                return c.get() == child;
            }),
            children.end()
        );
    }

    void CleanUp()
    {
        children.erase
        (
            remove_if
            (
                children.begin(),
                children.end(),
                [](const shared_ptr<GameObject>& c)
                {
                    return c->isDestroyed;
                }
            ),
            children.end()
        );
        // 递归的处理子物体
        for (auto& c: children)
        {
            if (c) c->CleanUp();
        }
    }

    Matrix GetWorldMatrix() const
    {
        if (parent)
            return parent->GetWorldMatrix() * transform.GetMatrix();
        return transform.GetMatrix();
    }

    /// @brief 递归地获取世界坐标 
    Vector2 GetWorldPosition() const
    {
        Matrix world = GetWorldMatrix();
        return world.TransformPoint(Vector2(0, 0));
    }

    /// @brief 改为weak_ptr
    /// @tparam T 
    /// @return 
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

    template<typename T>
    vector<shared_ptr<T>> GetComponents()
    {
        vector<shared_ptr<T>> comps;
        for (auto component : components)
        {
            shared_ptr<T> casted = dynamic_pointer_cast<T>(component);
            if (casted)
            {
                comps.push_back(casted);
            }
        }
        return comps;
    }

    virtual void Awake()
    {
        for (auto& component : components) component->Awake();
    }

    virtual void Start()
    {
        for (auto& component : components) component->Start();
        for (auto& child : children) child->Start();
    }

    virtual void Update(float deltaTime)
    {
        if (!isActive) return;
        for (auto& component : components) component->Update(deltaTime);
        for (auto& child : children) child->Update(deltaTime);
    }

    virtual void Render(Renderer& renderer)
    {
        if (!isActive) return;
        for (auto& component : components) component->Render(renderer);
        for (auto& child : children) child->Render(renderer);
    }

    virtual void ProcessEvent(const SDL_Event& e) 
    {
        if (!isActive) return;
        for (auto& component : components) component->ProcessEvent(e);
        for (auto& child : children) child->ProcessEvent(e);
    }

    virtual void Destroy()
    {
        if (isDestroyed) return;
        for (auto& component : components) component->Destroy();
        
        if (parent)
        {
            parent->RemoveChild(this);
            parent = nullptr;
        }

        for (auto& child : children)
        {
            if (child && !child->isDestroyed)
            {
                child->Destroy();
            }
        }
        children.clear();
        
        isDestroyed = true;

        for (auto& child : children) child->Destroy();
    }
};

#endif