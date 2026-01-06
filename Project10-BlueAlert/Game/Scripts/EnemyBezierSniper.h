#ifndef ENEMYBEZIERSNIPER_H
#define ENEMYBEZIERSNIPER_H

#include <functional>
#include <vector>
#include "Enemy.h"

class EnemyBezierSniper : public Enemy
{
private:
    // 标识符用于检查是否设置初始参数
    bool isSetParameters = false;

    float fireInterval; // 射击间隔
    float fireWait = 0.0f;

    float startShooting;
    float endShooting;
    // float duration;     // 从起点到终点的总时间
    // vector<Vector2> positions;  // 贝塞尔曲线点集
    function<float(float)> LinearToEasing;  // 将线性过度转换为缓动的函数
public:
    EnemyBezierSniper(GameObject* owner) : Enemy(owner) {}

    /// @brief 设置参数
    /// @param health 最大生命值
    /// @param startShooting 从生成到开始射击所经历的时间
    /// @param endShooting 从生成到结束射击所经历的时间
    /// @param fireInterval 射击间隔 s
    /// @param duration 从起点移动到终点的时间 s
    /// @param bulletConfigs 每个射击间隔都会遍历所有的子弹配置并射向Player 这里子弹会向着Player偏移
    /// @param positions 贝塞尔曲线控制点(相对位移: 相对于生成位置)
    /// @param LinearToEasing 将线性时间映射为缓动的函数
    void SetParameters(int health, float startShooting, float endShooting, float fireInterval, float duration, const vector<BulletConfig>& bulletConfigs,
        const vector<Vector2>& positions, const function<float(float)>& LinearToEasing)
    {
        if (isSetParameters) Debug::Log("[WARN] Enemy bezier sniper 已经设置过初始参数");
        isSetParameters = true;
        this->health            = health;
        this->startShooting     = startShooting;
        this->endShooting       = endShooting;
        this->fireInterval      = fireInterval;
        this->fireWait          = fireInterval; // 只要一到开始射击的时间立马射出一发子弹
        // this->duration       = duration;   
        this->bulletConfigs     = bulletConfigs;
        // this->positions      = positions;
        this->LinearToEasing    = LinearToEasing;
        this->pattern           = make_shared<BezierPattern>(positions, duration);
        pattern->SetOrigin(gameObject->transform.position);
    }

    void OnStart() override
    {
        
    }

    void OnUpdate(float deltaTime) override
    {
        if (!isSetParameters) Debug::Log("[WARN] Enemy bezier sniper 未初始化参数");
        if (!LinearToEasing)
        {
            Debug::Log("[WARN] Enemy bezier sniper LinearToEasing 为空");
            return;
        }
        float easing = LinearToEasing(age);
        if (!pattern)
        {
            Debug::Log("[WARN] Enemy bezier sniper pattern 为空");
            return;
        }
        Vector2 pos = pattern->GetPosition(easing);
        gameObject->transform.SetPosition(pos);
        if (age < startShooting || age > endShooting) return;
        fireWait += deltaTime;
        if (fireWait < fireInterval) return;
        fireWait = 0.0f;
        
        if (!InVisibleBorder()) return;
        for (const auto& config : bulletConfigs)
        {
            Fire(config);
        }
    }

    void Fire(const BulletConfig& config)
    {
        auto playerSp = playerRef.lock();
        if (!playerSp) {
            Debug::Log("[WARN] Enemy bezier sniper Fire: playerRef 失效");
            return;
        }

        Vector2 enemyPos = gameObject->GetWorldPosition();
        Vector2 playerPos = playerSp->transform.position;

        Vector2 aimVec = playerPos - enemyPos;
        float aimMag = aimVec.Magnitude();
        const float EPS = 1e-6f;
        if (aimMag < EPS) {
            Debug::Log("[WARN] Fire: aim vector too small");
            return;
        }
        Vector2 aimDir = aimVec / aimMag;

        float offsetDeg = 0.0f;
        float cfgMag = config.dir.Magnitude();
        if (cfgMag >= EPS) {
            offsetDeg = std::atan2(config.dir.y, config.dir.x) * 180.0f / (float)M_PI;
        }

        float spawnOffsetDeg = 0.0f;
        float spawnOffsetcfgMag = config.spawnOffset.Magnitude();
        if (spawnOffsetcfgMag >= EPS)
        {
            spawnOffsetDeg = std::atan2(config.spawnOffset.y, config.spawnOffset.x) * 180.0f / (float)M_PI;
        }

        float aimDeg = std::atan2(aimDir.y, aimDir.x) * 180.0f / (float)M_PI;
        float finalDeg = aimDeg + offsetDeg;
        float finalSpawnOffsetDeg = aimDeg + spawnOffsetDeg;

        const float SPRITE_DEFAULT_DEG = 45.0f;

        float spriteAngle = finalDeg - SPRITE_DEFAULT_DEG;
        
        // Debug::Log("aimDeg=" + std::to_string(aimDeg) + " offsetDeg=" + std::to_string(offsetDeg) + " finalDeg=" + std::to_string(finalDeg) + " spriteAngle=" + std::to_string(spriteAngle) +
        //     " spawnOffsetDeg=" + std::to_string(spawnOffsetDeg) + " finalSpawnOffsetDeg=" + std::to_string(finalSpawnOffsetDeg));

        BulletConfig tempCfg = config;

        float finalRad = finalDeg * (float)M_PI / 180.0f;
        float spawnOffsetRad = finalSpawnOffsetDeg * (float)M_PI / 180.0f;
        tempCfg.dir = Vector2(std::cos(finalRad), std::sin(finalRad)).Normalized();
        tempCfg.spawnOffset = Vector2(std::cos(spawnOffsetRad), std::sin(spawnOffsetRad)).Normalized() * spawnOffsetcfgMag;

        auto bulletObj = BulletPool::Get().Acquire(tempCfg);
        if (!bulletObj) {
            Debug::Log("[WARN] Enemy bezier sniper Fire: Acquire 返回空");
            return;
        }

        bulletObj->transform.Rotate(spriteAngle);
        // Debug::Log(to_string(bulletObj->transform.rotation));
        bulletObj->SetActive(true);
        auto bulletScript = bulletObj->GetComponent<Bullet>();
        if (bulletScript) bulletObj->transform.SetPosition(enemyPos + bulletScript->GetSpawnOffset());
        else bulletObj->transform.SetPosition(enemyPos);

        if (!(bulletObj->GetComponent<Bullet>()->fromPool)) gameObject->GetScene()->AddObject(bulletObj);
    }
};

#endif