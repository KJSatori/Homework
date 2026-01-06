#include "Item.h"
#include "Player.h"

void Item::Start()
{
    playerRef = gameObject->GetScene()->FindFirstByNameInRoot("Player");
}

void Item::Update(float deltaTime)
{
    if (collected && !startLeap) return;
    auto player = playerRef.lock();
    if (!player) return;
    const Vector2& playerPos = player->transform.position;

    if (startLeap)
    {
        gameObject->transform.Translate((playerPos - gameObject->transform.position).Normalized() * deltaTime * ITEM_COLLECT_SPEED);
        if ((playerPos - gameObject->transform.position).Magnitude() < 1.0f)
        {
            gameObject->Destroy();
            player->GetComponent<Player>()->CollectItem(type, value);
        }
        return;
    }
    else
    {
        if(!justSpawn) gameObject->transform.Translate(0.0f, -ITEM_DROP_SPEED*deltaTime);
        else 
        {
            gameObject->transform.Rotate(ITEM_ROTATE_SPEED * deltaTime);
            gameObject->transform.Translate(0.0f, 4*ITEM_DROP_SPEED*deltaTime);
            if (gameObject->transform.rotation > 720.0f)
            {
                gameObject->transform.SetRotation(0.0f);
                justSpawn = false;
            }
        }
    }
    if (gameObject->transform.position.y < -50.0f) gameObject->Destroy();
    if ((playerPos - gameObject->transform.position).SquareMagnitude() <= ITEM_PICKUP_RADIUS*ITEM_PICKUP_RADIUS || playerPos.y > 410.0f)
    {
        collected = true;
        startLeap = true;
    }
}