#ifndef BULLET_H
#define BULLET_H

#include "../../Engine/Core/GameObject/GameObject.h"
#include "../../Engine/Core/Component/RendererComponent.h"
#include "../../Engine/Core/Component/Rigidbody.h"
#include "../../Engine/Core/Component/Collider.h"
#include "../../Engine/Core/Component/Script.h"
#include "../../Engine/Core/Component/MovementPattern.h"

#include "BulletConfig.h"
#include "BulletPool.h"

#include <functional>
#include <memory>

class Bullet : public Script
{
private:
    Vector2 dir = Vector2::Up();
    Vector2 spawnOffset = Vector2(0, 40.0f);
    float speed = 1200.0f;
    float lifeTime = 3.0f;
    float age = 0.0f;
    int damage = 1;

    std::shared_ptr<MovementPattern> pattern;
    std::function<float(float)> LinearToEasing = [](float age) { return age; };
    
    Vector2 origin;
public:
    bool fromPool = false;
    std::weak_ptr<GameObject> selfShared;

    Bullet(GameObject* owner) : Script(owner) {}
    Bullet(GameObject* owner, float spd, float dmg) : Script(owner), speed(spd), damage(dmg) {} 
    Bullet(GameObject* owner, float spd, float dmg, float lifeT, const Vector2& direction, const Vector2& spawnOfs) : 
        Script(owner), speed(spd), damage(dmg), lifeTime(lifeT), dir(direction), spawnOffset(spawnOfs) {}
    Bullet(GameObject* owner, float spd, float dmg, float lifeT, const Vector2& direction, const Vector2& spawnOfs, const std::shared_ptr<MovementPattern>& p) : 
        Script(owner), speed(spd), damage(dmg), lifeTime(lifeT), dir(direction), spawnOffset(spawnOfs), pattern(p) {}

    void SetParameters(float spd, int dmg, Vector2 d = Vector2::Up())
    {
        speed = spd;
        damage = dmg;
        dir = d;
    }

    void SetSpawnOffset(const Vector2 offset) { spawnOffset = offset; }

    void SetPattern(const std::shared_ptr<MovementPattern>& p)
    {
        pattern = p;
        if (pattern) pattern->SetOrigin(origin);
    }
    void SetPattern(const std::shared_ptr<MovementPattern>& p, const std::function<float(float)>& toEasing)
    {
        pattern = p;
        if (pattern) pattern->SetOrigin(origin);
        LinearToEasing = toEasing;
    }

    void Awake() override 
    {
        age = 0.0f;
        origin = gameObject->transform.position;
        if (pattern) pattern->SetOrigin(origin);
    }

    void Update(float deltaTime) override
    {
        if (!gameObject) return;
        // Debug::Log("Render: " + gameObject->name + " at " + to_string(gameObject->transform.position.x) + "," + to_string(gameObject->transform.position.y));
        if (pattern)
        {
            Vector2 pos = pattern->GetPosition(LinearToEasing(age));
            gameObject->transform.SetPosition(pos);
        }
        else
        {
            dir = dir.Normalized();
            gameObject->transform.Translate(speed * dir * deltaTime);
        }
        age += deltaTime;
        if (age >= lifeTime)
        {
            RequestRecycle();
        }
    }

    void OnColliderEnter(GameObject* other) override
    {
        if (other->GetTag() == Tag::Enemy && gameObject->GetTag() == Tag::PlayerBullet)
        {
            RequestRecycle();
        }
        if (other->GetTag() == Tag::Player && gameObject->GetTag() == Tag::EnemyBullet)
        {
            RequestRecycle();
        }
    }

    int GetDamage()
    {
        return damage;
    }

    Vector2 GetSpawnOffset() const
    {
        return spawnOffset;
    }

    void RequestRecycle()
    {
        if (fromPool)
        {
            auto sp = selfShared.lock();
            if (sp) BulletPool::Get().Release(sp);
            else gameObject->Destroy();
        }
        else gameObject->Destroy();
    }

    /// @brief 根据 子弹配置 重置子弹状态
    /// @param cfg 子弹配置
    void ResetForReuse(const BulletConfig& cfg)
    {
        speed       = cfg.speed;
        damage      = cfg.damage;
        lifeTime    = cfg.lifeTime;
        dir         = cfg.dir;
        spawnOffset = cfg.spawnOffset;
        SetPattern(cfg.pattern, cfg.LinearToEasing);

        auto collider = gameObject->GetComponent<Collider>();
        collider->SetCircle(gameObject->transform.position, cfg.colliderRadius);

        auto renderer = gameObject->GetComponent<RendererComponent>();
        renderer->SetScaleFactor(cfg.scaleFactor);
        renderer->SetSprite(cfg.spriteKey);

        gameObject->SetTag(cfg.tag);

        age = 0.0f;
        origin = gameObject->transform.position;

        // 确保对象未被标记为销毁
        gameObject->isDestroyed = false;
    }
};

#endif