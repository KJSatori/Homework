#ifndef ENEMY_H
#define ENEMY_H

#include "../../Engine/Core/GameObject/GameObject.h"
#include "../../Engine/Core/Component/RendererComponent.h"
#include "../../Engine/Core/Component/Rigidbody.h"
#include "../../Engine/Core/Component/Collider.h"
#include "../../Engine/Core/Component/Script.h"
#include "../../Engine/Core/Component/MovementPattern.h"
#include "../../Engine/Core/Component/BezierPattern.h"
#include "../../Engine/Core/Component/SinePattern.h"
#include "../../Engine/Math/Utils.h"
#include "Bullet.h"
#include "BulletConfig.h"
#include "BulletPool.h"

#include <memory>

enum class EnemyState
{
    Patrol,
    Shoot
};

struct EnemyConfig
{
    int maxHealth = 3;
    float speed = 50.0f;

    std::shared_ptr<MovementPattern> pattern;

    EnemyConfig() {}
    EnemyConfig(int h, float spd) :
        maxHealth(h), speed(spd) {}
    EnemyConfig(int h, float spd, const std::shared_ptr<MovementPattern>& p) :
        maxHealth(h), speed(spd), pattern(p) {}
};

class Enemy : public Script
{
protected:
    int health;
    EnemyConfig config;
    vector<BulletConfig> bulletConfigs;
    EnemyState state = EnemyState::Patrol;
    std::shared_ptr<MovementPattern> pattern;
    float speed = 0.0f;
    Vector2 dir = Vector2::Down();

    float age = 0;
    std::weak_ptr<GameObject> playerRef;

public:
    Enemy(GameObject* owner) : Script(owner) {}

    void Start() override;

    void Update(float deltaTime) override
    {
        if (!gameObject) return;

        OnUpdate(deltaTime);

        age += deltaTime;

        if (!InBorder() || age >= 15.0f) gameObject->Destroy();
    }

    virtual void OnUpdate(float deltaTime) {}

    virtual void OnStart() {}

    void OnColliderEnter(GameObject* other) override
    {
        if (other->GetTag() == Tag::PlayerBullet)
        {
            auto playerBullet = other->GetComponent<Bullet>();
            if (!playerBullet) return;

            auto damage = playerBullet->GetDamage();
            health -= damage;

            if (health < 0)
            {
                SpawnDrop(gameObject->GetScene(), gameObject->transform.position);
                gameObject->Destroy();
            }
        }
    }

    void Destroy() override
    {
        Debug::Log("[Enemy] 被摧毁");
    }

    bool InBorder()
    {
        Vector2 pos = gameObject->transform.position;
        return pos.y < 750.0f && pos.y > -50.0f && pos.x > -40.0f && pos.x < 600.0f;
    }

    bool InVisibleBorder()
    {
        Vector2 pos = gameObject->transform.position;
        return pos.y < 580.0f && pos.y > 20.0f && pos.x > 40.0f && pos.x < 520.0f;
    }

    void SpawnDrop(Scene* scene, const Vector2& pos);
};

#endif