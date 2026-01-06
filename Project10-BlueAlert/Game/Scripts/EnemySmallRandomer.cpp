#include "EnemySmallRandomer.h"
#include "../../Engine/Core/Scene/Scene.h"

void EnemySmallRandomer::OnUpdate(float deltaTime)
{
    fireTimer+=deltaTime;
    if (fireTimer < 0.33f) return;
    fireTimer = 0.0f;
    auto tempBulletConfig = bulletConfigs[0];
    tempBulletConfig.dir = Vector2(RandomDouble(-0.5f, 0.5f), tempBulletConfig.dir.y);
    auto bulletObj1 = BulletPool::Get().Acquire(tempBulletConfig);
    bulletObj1->transform.Rotate(SignedAngleDegrees(Vector2::Down(), tempBulletConfig.dir));
    if (!bulletObj1) return;
    bulletObj1->SetActive(true);
    auto bulletScript1 = bulletObj1->GetComponent<Bullet>();
    bulletObj1->transform.SetPosition(gameObject->GetWorldPosition() + bulletScript1->GetSpawnOffset());
    if (!(bulletObj1->GetComponent<Bullet>()->fromPool)) gameObject->GetScene()->AddObject(bulletObj1);
}