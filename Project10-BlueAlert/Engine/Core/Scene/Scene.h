#ifndef SCENE_H
#define SCENE_H

#include "../Component/Collider.h"
#include "../../Render/Renderer.h"
#include "../Component/Script.h"
#include "../Prefab/PrefabManager.h"
#include <vector>
#include <algorithm>
#include <memory>
#include <SDL2/SDL.h>

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

struct GameObject;

struct Scene
{
    bool isStarted = false;

    // 场景的所有物体
    vector<shared_ptr<GameObject>> objects;
    vector<shared_ptr<GameObject>> toAdd;

    // 碰撞状态表
    vector<CollisionPair> activeCollisions;

    void AddObject(const shared_ptr<GameObject>& object);

    void Start();

    void Update(float deltaTime);

    /// @brief 分发窗口事件
    void ProcessEvent(const SDL_Event& e);

    void Render(Renderer& renderer);

    void Destroy();

    void DestroyObject(const shared_ptr<GameObject>& obj);

    void CleanUp();

    shared_ptr<GameObject> FindFirstByNameInRoot(const string& name);

    shared_ptr<GameObject> InstantiatePrefab(const string& key, const Vector2& pos);
    shared_ptr<GameObject> InstantiatePrefab(const string& key, const Vector2& pos,
        const std::function<void(shared_ptr<GameObject>)>& init);

    /// @brief 递归地将 obj 所有子物体（包括obj）推入 out 容器里
    /// @param obj 物体
    /// @param out 这里传非常量引用 会将物体直接推入 out
    void CollectObjectChildren(const shared_ptr<GameObject>& obj, vector<shared_ptr<GameObject>>& out);

    void EndGame();
};


#endif