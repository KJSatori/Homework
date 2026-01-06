#include "BulletPool.h"
#include "Bullet.h"
#include "../../Engine/Core/Scene/Scene.h"

std::shared_ptr<GameObject> BulletPool::Acquire(const BulletConfig &cfg)
{
    std::shared_ptr<GameObject> obj;
    if (!freeList.empty())
    {
        obj = freeList.back();
        freeList.pop_back();
        obj->GetComponent<Bullet>()->fromPool = true;
    }
    else
    {
        // Debug::Log("[BulletPool] 尝试新建子弹...");
        obj = CreateNew();
        obj->GetComponent<Bullet>()->fromPool = false;
    }
    inUse.insert(obj);

    std::shared_ptr<Bullet> bulletComp = obj->GetComponent<Bullet>();
    if (!bulletComp) Debug::Log("[WARN BulletPool] 子弹模板组件为空");
    if (bulletComp)
    {
        bulletComp->selfShared = obj;
        bulletComp->ResetForReuse(cfg);
    }
    return obj;
}

std::shared_ptr<GameObject> BulletPool::DebugAcquire(const BulletConfig &cfg)
{
    std::shared_ptr<GameObject> obj;
    if (!freeList.empty())
    {
        obj = freeList.back();
        freeList.pop_back();
    }
    else
    {
        obj = DebugCreateNew();
    }
    inUse.insert(obj);

    std::shared_ptr<Bullet> bulletComp = obj->GetComponent<Bullet>();
    if (bulletComp)
    {
        bulletComp->selfShared = obj;
        bulletComp->ResetForReuse(cfg);
    }
    return obj;
}
void BulletPool::ClearAllEnemyBullets()
{
    // 避免在遍历 inUse 时修改容器 
    std::vector<std::shared_ptr<GameObject>> toRelease; 
    toRelease.reserve(inUse.size()); 
    for (auto& obj : inUse) 
    { 
        if (!obj) continue; 
        if (obj->GetTag() == Tag::EnemyBullet) 
        { 
            toRelease.push_back(obj); 
        } 
    } 
    for (auto& obj : toRelease) 
    { 
        auto bulletS = obj->GetComponent<Bullet>();
        bulletS->RequestRecycle();
    }
}
/*
void BulletPool::ClearEnemyBulletsInScene(Scene *scene)
{
    std::vector<std::shared_ptr<GameObject>> toDestroy;
    for (auto& obj : scene->objects)
    {
        if (!obj) continue;
        if (obj->GetTag() == Tag::EnemyBullet)
        {
            if (inUse.find(obj) != inUse.end())
            {
                Release(obj);
            }
            else
            {
                toDestroy.push_back(obj);
            }
        }
    }
    for (auto& obj : toDestroy)
    {
        obj->Destroy();
    }
}
*/