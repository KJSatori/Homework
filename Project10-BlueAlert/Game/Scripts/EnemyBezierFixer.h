#ifndef ENEMYBEZIERFIXER_H
#define ENEMYBEZIERFIXER_H

#include <functional>
#include <vector>
#include "Enemy.h"

class EnemyBezierFixer : public Enemy
{
private:
    bool isSetParameters = false;

    float fireInterval = 1.0f;
    float fireWait = 0.0f;
    float startShooting = 0.0f;
    float endShooting = 9999.0f;

    std::function<float(float)> LinearToEasing;
    
public:
    EnemyBezierFixer(GameObject* owner) : Enemy(owner) {}

    /// @brief 设置参数
    /// @param health 最大生命值
    /// @param startShooting 从生成到开始射击所经历的时间 s
    /// @param endShooting 从生成到结束射击所经历的时间 s
    /// @param fireInterval 射击间隔 s
    /// @param moveDuration 从起点移动到终点的时间 s
    /// @param bulletConfigs 每个射击间隔都遍历所有子弹设置
    /// @param moveCtrlPoints 贝塞尔控制点(相对位移: 相对于生成位置)
    /// @param LinearToEasing 将线性时间映射为缓动的函数
    void SetParameters(int health, float startShooting, float endShooting, 
        float fireInterval, float moveDuration, 
        const std::vector<BulletConfig>& bulletConfigs, 
        const std::vector<Vector2>& moveCtrlPoints, 
        const std::function<float(float)>& LinearToEasing
    )
    {
        if (isSetParameters) Debug::Log("[WARN] EnemyBezierFixer 已设置参数");
        isSetParameters = true;
        this->health = health;
        this->startShooting = startShooting;
        this->endShooting = endShooting;
        this->fireInterval = fireInterval;
        this->fireWait = fireInterval;
        this->bulletConfigs = bulletConfigs;
        this->LinearToEasing = LinearToEasing;

        Vector2 origin = gameObject->GetWorldPosition();
        this->pattern = std::make_shared<BezierPattern>(moveCtrlPoints, moveDuration);
        this->pattern->SetOrigin(origin);
    }

    void OnStart() override { if (!isSetParameters) Debug::Log("[WARN] EnemyBezierFixer 未初始化"); }

    void OnUpdate(float deltaTime) override
    {
        if (!isSetParameters) return;
        if (!LinearToEasing) return;
        float t = LinearToEasing(age);
        if (!pattern) return;
        Vector2 pos = pattern->GetPosition(t);
        gameObject->transform.SetPosition(pos);

        if (age < startShooting || age > endShooting) return;
        fireWait += deltaTime;
        if (fireWait < fireInterval) return;
        fireWait = 0.0f;

        if (!InVisibleBorder()) return;

        for (const auto &cfg : bulletConfigs)
        {
            FireBezierBullet(cfg);
        }
    }

    void FireBezierBullet(const BulletConfig& cfg)
    {
        Vector2 spawnWorld = gameObject->GetWorldPosition();
        auto bulletPattern = std::make_shared<BezierPattern>(cfg.pattern);
        bulletPattern->SetOrigin(spawnWorld);

        BulletConfig temp = cfg;
        temp.dir = cfg.dir.Normalized();
        auto bulletObj = BulletPool::Get().Acquire(temp); 
        if (!bulletObj) { Debug::Log("[WARN] BezierFixer Fire: Acquire 返回空"); return; } 
        auto bulletScript = bulletObj->GetComponent<Bullet>(); 
        if (bulletScript) 
        { 
            bulletScript->SetPattern(bulletPattern); 
            bulletObj->transform.SetPosition(spawnWorld + bulletScript->GetSpawnOffset()); 
            bulletObj->SetActive(true); 
            if (!bulletScript->fromPool) gameObject->GetScene()->AddObject(bulletObj); 
        } 
        else 
        { 
            bulletObj->transform.SetPosition(spawnWorld); 
            bulletObj->SetActive(true); 
            gameObject->GetScene()->AddObject(bulletObj); 
        }
    }
};

#endif