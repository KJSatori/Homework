#ifndef SCENE_H
#define SCENE_H

#include "../GameObject/GameObject.h"
#include "../Component/Collider.h"
#include "../../Render/Renderer.h"
#include "../Component/Scripts/Script.h"
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

struct CollisionPair
{
    GameObject* a;
    GameObject* b;
    bool operator==(const CollisionPair& other) const
    {
        return (a == other.a && b == other.b) || (a == other.b && b == other.a);
    }
};

struct Scene
{
    // 场景的所有物体
    vector<shared_ptr<GameObject>> objects;

    // 碰撞状态表
    vector<CollisionPair> activeCollisions;

    void AddObject(const shared_ptr<GameObject>& object)
    {
        objects.push_back(object);
        object->Awake();
    }

    void Start()
    {
        for (auto& obj : objects) 
        {
            obj->Start();
        }
    }

    void Update(float deltaTime)
    {        
        for (auto& obj : objects) 
        {
            obj->Update(deltaTime);
        }
        vector<CollisionPair> nextActiceCollisions;

        for (size_t i = 0; i < objects.size(); ++i)
        {
            auto colA = objects[i]->GetComponent<Collider>();
            if (!colA) continue;

            for (size_t j = i + 1; j < objects.size(); ++j)
            {
                auto colB = objects[j]->GetComponent<Collider>();
                if (!colB) continue;

                if (colA->Intersects(*colB))
                {
                    CollisionPair newColPair;
                    newColPair.a = colA->gameObject;
                    newColPair.b = colB->gameObject;
                    auto iter = find(activeCollisions.begin(), activeCollisions.end(), newColPair);
                    if (iter == activeCollisions.end())
                    {
                        // 调用 A 与 B 的 OnCollisionEnter;
                        auto scriptA = newColPair.a->GetComponent<Script>();
                        if (scriptA) scriptA->OnColliderEnter(newColPair.b);
                        auto scriptB = newColPair.b->GetComponent<Script>();
                        if (scriptB) scriptB->OnColliderEnter(newColPair.a);

                        nextActiceCollisions.push_back(newColPair);
                    }
                    else
                    {
                        // 调用 A 与 B 的 OnCollisionStay;
                        auto scriptA = newColPair.a->GetComponent<Script>();
                        if (scriptA) scriptA->OnColliderStay(newColPair.b);
                        auto scriptB = newColPair.b->GetComponent<Script>();
                        if (scriptB) scriptB->OnColliderStay(newColPair.a);

                        nextActiceCollisions.push_back(newColPair);
                    }
                }
            }
        }

        for (auto pair : activeCollisions)
        {
            auto iter = find(nextActiceCollisions.begin(), nextActiceCollisions.end(), pair);
            if (iter == nextActiceCollisions.end())
            {
                auto newColPair = *iter;
                // 调用 *iter 里的 A 与 B 的 OnCollisionExit;
                auto scriptA = newColPair.a->GetComponent<Script>();
                if (scriptA) scriptA->OnColliderExit(newColPair.b);
                auto scriptB = newColPair.b->GetComponent<Script>();
                if (scriptB) scriptB->OnColliderExit(newColPair.a);
            }
        }

        // 帧末尾统一处理删除的Obj
        CleanUp();
    }

    void Render(Renderer& renderer) 
    { 
        renderer.Erase();
        for (auto& obj : objects) 
        {
            obj->Render(renderer); 
        } 
        renderer.Refresh(); 
    }

    void Destroy()
    {
        for (auto& obj : objects) { obj->Destroy(); }
        CleanUp();
    }

    void DestroyObject(const shared_ptr<GameObject>& obj)
    {
        obj->Destroy();
    }

    void CleanUp()
    {
        objects.erase(
            remove_if(
                objects.begin(), 
                objects.end(), 
                [](const shared_ptr<GameObject>& obj) 
                { 
                    return obj->isDestroyed; 
                }
            ), 
            objects.end()
        );
    }
};


#endif