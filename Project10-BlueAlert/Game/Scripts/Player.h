#ifndef PLAYER_H
#define PLAYER_H

#include "../../Engine/Core/GameObject/GameObject.h"
#include "../../Engine/Core/Scene/Scene.h"
#include "../../Engine/Core/Component/RendererComponent.h"
#include "../../Engine/Core/Component/Rigidbody.h"
#include "../../Engine/Core/Component/Collider.h"
#include "../../Engine/Core/Component/Script.h"
#include "../../Engine/Core/Component/SinePattern.h"

#include "Bullet.h"
#include "BulletPool.h"
#include "BulletConfig.h"
#include "Item.h"
#include "Enemy.h"

#include <memory>

class Player : public Script
{
private:
    weak_ptr<RendererComponent> rendererWp;
    weak_ptr<RendererComponent> triggerRendererWp;

    double lastFireTime = 0;

    // 持续发射子弹
    bool startFire = false;

    // 重生时屏蔽移动并添加特效
    bool isDuringSpawn = false;
    bool isInvincible = false;
    const float slowEffectVelocity = -60.0f;

    // 子弹发射间隔
    float fireDelta = 0.08f;

    // 数据为绀珠传博丽灵梦
    float normalVelocity = 337.5f;
    float slowVelocity = 150.0f;

    int tick = 0;
    float clock = 0;
    float age = 0;

    // 初始 2残机
    int health = 2;
    int maxHealth = 8;
    int score = 0;
    int maxScore = 10000000;
    int bomb = 2;
    int maxBomb = 8;
    int power = 000;
    int maxPower = 400;

    Vector2 spawnPoint = Vector2(280.0f, 80.0f);

    BulletConfig config1 = BulletConfig
    (
        "YellowMiddleJade", 
        1.5f,
        6.0f,
        Tag::PlayerBullet,
        1000.0f,
        1,
        3.0f,
        Vector2::Up(),
        Vector2(-8.0f, 10.0f)
    );

    BulletConfig config2 = BulletConfig
    (
        "YellowMiddleJade", 
        1.5f,
        6.0f,
        Tag::PlayerBullet,
        1000.0f,
        1,
        3.0f,
        Vector2::Up(),
        Vector2(8.0f, 10.0f)
    );

    BulletConfig config3 = BulletConfig
    (
        "YellowMiddleJade", 
        1.5f,
        6.0f,
        Tag::PlayerBullet,
        1000.0f,
        1,
        3.0f,
        Vector2::Up(),
        Vector2(0.0f, 10.0f),
        std::make_shared<SinePattern>(Vector2::Up(), Vector2::Right(), 800.0f, 40.0f, 30.0f)
    );

    BulletConfig config4 = BulletConfig
    (
        "YellowMiddleJade", 
        1.5f,
        6.0f,
        Tag::PlayerBullet,
        1000.0f,
        1,
        3.0f,
        Vector2::Up(),
        Vector2(0.0f, 10.0f),
        std::make_shared<SinePattern>(Vector2::Up(), Vector2::Left(), 800.0f, 40.0f, 30.0f)
    );

    BulletConfig config5 = BulletConfig
    (
        "YellowMiddleJade", 
        1.5f,
        6.0f,
        Tag::PlayerBullet,
        1200.0f,
        1,
        3.0f,
        Vector2(0.26f, 0.966f),
        Vector2(8.0f, 10.0f)
    );

    BulletConfig config6 = BulletConfig
    (
        "YellowMiddleJade", 
        1.5f,
        6.0f,
        Tag::PlayerBullet,
        1200.0f,
        1,
        3.0f,
        Vector2(-0.26f, 0.966f),
        Vector2(-8.0f, 10.0f)
    );

    BulletConfig config7 = BulletConfig
    (
        "YellowMiddleJade", 
        1.5f,
        6.0f,
        Tag::PlayerBullet,
        800.0f,
        1,
        3.0f,
        Vector2(0.5f, 0.866f),
        Vector2(8.0f, 10.0f)
    );

    BulletConfig config8 = BulletConfig
    (
        "YellowMiddleJade", 
        1.5f,
        6.0f,
        Tag::PlayerBullet,
        800.0f,
        1,
        3.0f,
        Vector2(-0.5f, 0.866f),
        Vector2(-8.0f, 10.0f)
    );

    BulletConfig config9 = BulletConfig
    (
        "YellowMiddleJade", 
        1.5f,
        6.0f,
        Tag::PlayerBullet,
        1000.0f,
        1,
        3.0f,
        Vector2::Up(),
        Vector2(0.0f, 10.0f),
        std::make_shared<SinePattern>(Vector2::Up(), Vector2::Right(), 600.0f, 20.0f, 50.0f)
    );

    BulletConfig config10 = BulletConfig
    (
        "YellowMiddleJade", 
        1.5f,
        6.0f,
        Tag::PlayerBullet,
        1000.0f,
        1,
        3.0f,
        Vector2::Up(),
        Vector2(0.0f, 10.0f),
        std::make_shared<SinePattern>(Vector2::Up(), Vector2::Left(), 600.0f, 20.0f, 50.0f)
    );

    void FireBullet()
    {   
        Fire(config1);
        Fire(config2);

        if (power < 100) return;
        Fire(config5);
        Fire(config6);

        if (power < 200) return;
        Fire(config3);
        Fire(config4);

        if (power < 300) return;
        Fire(config7);
        Fire(config8);

        if (power < 400) return;
        Fire(config9);
        Fire(config10);
    }

    void Fire(const BulletConfig& config)
    {
        auto bulletObj = BulletPool::Get().Acquire(config);
        if (!bulletObj) return;
        bulletObj->SetActive(true);
        auto bulletScirpt = bulletObj->GetComponent<Bullet>();
        bulletObj->transform.SetPosition(gameObject->GetWorldPosition() + bulletScirpt->GetSpawnOffset());
        if (!(bulletObj->GetComponent<Bullet>()->fromPool)) gameObject->GetScene()->AddObject(bulletObj);
    }
public:
    Player(GameObject* owner, weak_ptr<RendererComponent> render,weak_ptr<RendererComponent> triggerR) : Script(owner), rendererWp(render), triggerRendererWp(triggerR) {}

    void Update(float deltaTime) override
    {
        ++tick;
        if (tick % 65535 == 0) tick = 0;
        age += deltaTime;

        auto renderer = rendererWp.lock();
        auto triggerRenderer = triggerRendererWp.lock();
        // Debug::Log(to_string(age * deltaTime * slowEffectVelocity));
        triggerRenderer->SetRotation(age * slowEffectVelocity);

        if (!isDuringSpawn)
        {
            if (isInvincible) 
            {
                renderer->SetAlpha((tick%24 >= 12) ? 0 : 255);
                clock+=deltaTime;
                if (clock > 3.0f)
                {
                    clock = 0.0f;
                    isInvincible = false;
                }
            }
            else renderer->SetAlpha(255);

            if (!startFire) return;
            lastFireTime += deltaTime;
            if (lastFireTime <= fireDelta) return;

            lastFireTime = 0;
            FireBullet();
        }
        else
        {
            gameObject->transform.Translate(0.0f, slowVelocity * deltaTime);
            renderer->SetAlpha((tick%24 >= 12) ? 0 : 255);
            if (gameObject->transform.position.y > spawnPoint.y - 0.001f) 
            {
                isDuringSpawn = false;
                isInvincible = true;
                renderer->SetAlpha(255);
            }
        }
    }

    void OnColliderEnter(GameObject* other) override
    {
        if (isDuringSpawn || isInvincible) return;
        if (other->GetTag() == Tag::Enemy || other->GetTag() == Tag::EnemyBullet)
        {
            --health;
            if (health < 0) 
            {
                gameObject->GetScene()->EndGame();
                // gameObject->SetActive(false);
                return;
            }

            gameObject->transform.SetPosition(spawnPoint.x, -5.0f);
            isDuringSpawn = true;
            Debug::Log("[player] 与敌机碰撞");
        }
    }

    void UseBomb()
    {
        if (bomb <= 0) return;
        bomb--;

        auto scene = gameObject->GetScene();
        std::vector<std::shared_ptr<GameObject>> toDestroy; 
        for (auto& obj : scene->objects) 
        { 
            if (obj->GetTag() == Tag::Enemy) 
            { 
                toDestroy.push_back(obj); 
            } 
        } 
        for (auto& e : toDestroy) 
        { 
            auto eScript = e->GetComponent<Enemy>();
            eScript->SpawnDrop(e->GetScene(), e->transform.position);
            e->Destroy(); 
        }
        BulletPool::Get().ClearAllEnemyBullets();
        isInvincible = true;
    }

    float GetVelocity(bool isSlow)
    {
        return (isSlow) ? slowVelocity : normalVelocity;
    }

    bool IsDuringSpawn() { return isDuringSpawn; }

    void ContinuouslyFire(bool start)
    {
        startFire = start;
    }

    Vector2 GetSpawnPoint()
    {
        return spawnPoint;
    }

    void CollectItem(ItemType type, int value)
    {
        switch (type)
        {
        case ItemType::Score:
            score = std::min(maxScore, score + value);
            break;
        case ItemType::Life:
            health = std::min(maxHealth, health + value);
            break;
        case ItemType::Bomb:
            bomb = std::min(maxBomb, bomb + value);
            break;
        case ItemType::Power:
            power = std::min(maxPower, power + value);
            break;
        }
    }

    int GetHealth() const { return health; }
    int GetScore() const { return score; }
    int GetPower() const { return power; }
    int GetBomb() const { return bomb; }

    void SetSlowEffectVislble(bool isSlow)
    {
        auto sp = triggerRendererWp.lock();
        if (sp) sp->SetAlpha((isSlow)?255:0);
    }
};

#endif