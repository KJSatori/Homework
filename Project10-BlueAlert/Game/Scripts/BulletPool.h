#ifndef BULLETPOOL_H
#define BULLETPOOL_H

#include <vector>
#include <memory>
#include <unordered_set>
#include "BulletConfig.h"
#include "../../Engine/Core/GameObject/GameObject.h"
#include "../../Engine/Core/Component/Debug.h"

class Bullet;

class BulletPool
{
private:
    BulletPool() = default;
    std::vector<std::shared_ptr<GameObject>> freeList;
    std::unordered_set<std::shared_ptr<GameObject>> inUse;

    std::shared_ptr<GameObject> CreateNew()
    {
        auto obj = PrefabManager::Get().Instantiate("templateBullet");
        obj->SetActive(false);
        // Debug::Log("[BulletPool] Instantiate template bullet");
        if (!obj) Debug::Log("[WARN: BulletPool] 模板子弹指针为空");
        return obj;
    }

    std::shared_ptr<GameObject> DebugCreateNew()
    {
        auto obj = PrefabManager::Get().Instantiate("playerYellowBullet");
        obj->SetActive(false);
        return obj;
    }

public:
    static BulletPool& Get()
    {
        // 单例
        static BulletPool inst;
        return inst;
    }

    void Prewarm(size_t n)
    {
        for (size_t i = 0; i < n; ++i) freeList.push_back(CreateNew());
    }

    std::shared_ptr<GameObject> Acquire(const BulletConfig& cfg);

    std::shared_ptr<GameObject> DebugAcquire(const BulletConfig& cfg);

    void Release(std::shared_ptr<GameObject> obj)
    {
        if (!obj) return;
        auto it = inUse.find(obj);
        if (it == inUse.end()) return;
        inUse.erase(it);

        // 确保未标记
        obj->isDestroyed = false;
        obj->SetActive(false);
        freeList.push_back(obj);
    }

    size_t Available() const { return freeList.size(); }

    void Clear()
    {
        inUse.clear();
        freeList.clear();
    }

    void ClearAllEnemyBullets();

    // void ClearEnemyBulletsInScene(Scene* scene);
};



#endif