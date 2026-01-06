#ifndef ITEM_H
#define ITEM_H

#include <functional>
#include "../../Engine/Core/Component/Script.h"
#include "../../Engine/Core/Component/Collider.h"

class Player;

enum class ItemType
{
    Score,
    Power,
    Bomb,
    Life
};

static constexpr float ITEM_PICKUP_RADIUS = 75.0f;
static constexpr float ITEM_DROP_SPEED = 50.0f;
static constexpr float ITEM_COLLECT_SPEED = 400.0f;
static constexpr float ITEM_ROTATE_SPEED = 1440.0f*2.0*1.5f;

class Item : public Script
{
public:
    ItemType type = ItemType::Score;
    int value = 0;
    bool collected = false;
    bool startLeap = false;
    bool justSpawn = true;

    Item(GameObject* owner) : Script(owner) {}

    std::weak_ptr<GameObject> playerRef;

    std::function<void(ItemType, int)> onPickup;

    void Start() override;

    void Update(float deltaTime) override;
};

#endif