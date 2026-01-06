#include "Enemy.h"
#include "../../Engine/Core/Scene/Scene.h"
void Enemy::Start()
{
    playerRef = gameObject->GetScene()->FindFirstByNameInRoot("Player");
    OnStart();
}

void Enemy::SpawnDrop(Scene *scene, const Vector2 &pos)
{ 
    float r = RandomDouble(0.0, 1.0); 
    if (r < 0.7) 
    { 
        scene->InstantiatePrefab("scoreItem", pos); 
    } 
    else if (r < 0.975) 
    { 
        scene->InstantiatePrefab("powerItem", pos); 
    }
    else if (r < 0.995) 
    { 
        scene->InstantiatePrefab("bombItem", pos); 
    } 
    else 
    { 
        scene->InstantiatePrefab("lifeItem", pos); 
    } 
}