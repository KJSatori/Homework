#ifndef BULLETCONFIG_H
#define BULLETCONFIG_H

#include <string>
#include <memory>
#include "../../Engine/Math/Vector2.h"
#include "../../Engine/Core/Component/MovementPattern.h"
#include "../Tag.h"
#include <functional>

struct BulletConfig
{
    std::string spriteKey;
    float scaleFactor = 1.0f;
    float colliderRadius = 3.5f;

    Tag tag = Tag::None;
    float speed = 1200.0f;
    int damage = 1;
    float lifeTime = 3.0f;
    Vector2 dir = Vector2::Up();
    Vector2 spawnOffset = Vector2(0, 40.0f);
    std::shared_ptr<MovementPattern> pattern;
    std::function<float(float)> LinearToEasing = [](float age) { return age; };

    /// @brief 
    /// @param sKey 
    /// @param scale 
    /// @param colRadius 
    /// @param t 
    /// @param spd 
    /// @param dmg 
    /// @param lifeT 
    /// @param d 
    /// @param spawnOfs 
    BulletConfig(const std::string& sKey, float scale, float colRadius, Tag t, float spd, int dmg, float lifeT, Vector2 d, Vector2 spawnOfs) :
        spriteKey(sKey), scaleFactor(scale), colliderRadius(colRadius), tag(t), speed(spd), damage(dmg), lifeTime(lifeT), dir(d), spawnOffset(spawnOfs) {}

    BulletConfig(const std::string& sKey, float scale, float colRadius, Tag t, float spd, int dmg, float lifeT, Vector2 d, Vector2 spawnOfs, const std::shared_ptr<MovementPattern>& p) :
        spriteKey(sKey), scaleFactor(scale), colliderRadius(colRadius), tag(t), speed(spd), damage(dmg), lifeTime(lifeT), dir(d), spawnOffset(spawnOfs), pattern(p) {}

    BulletConfig(const std::string& sKey, float scale, float colRadius, Tag t, float spd, int dmg, float lifeT, Vector2 d, Vector2 spawnOfs, const std::shared_ptr<MovementPattern>& p, std::function<float(float)> ToEasing) :
        spriteKey(sKey), scaleFactor(scale), colliderRadius(colRadius), tag(t), speed(spd), damage(dmg), lifeTime(lifeT), dir(d), spawnOffset(spawnOfs), pattern(p), LinearToEasing(ToEasing) {}

    void SetPattern(const std::shared_ptr<MovementPattern>& p) { pattern = p; }
    void SetPattern(const std::shared_ptr<MovementPattern>& p, std::function<float(float)> ToEasing) { pattern = p; LinearToEasing = ToEasing; }
};

#endif