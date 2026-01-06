#ifndef ENEMYSMALLRandomer_H
#define ENEMYSMALLRandomer_H

#include "Enemy.h"

class EnemySmallRandomer : public Enemy
{
private:
    float fireTimer = 0.0f;
public:
    EnemySmallRandomer(GameObject* owner) : Enemy(owner) 
    {
        config = EnemyConfig(3, 40.0f);
        bulletConfigs.push_back
        (
            BulletConfig
            (
                "GreenBullet", 
                2.0f,
                2.5f,
                Tag::EnemyBullet,
                300.0f,
                1,
                8.0f,
                Vector2::Down(),
                Vector2(0.0f, 0.0f)
            )  
        );
    }

    void OnUpdate(float deltaTime) override;
};

#endif