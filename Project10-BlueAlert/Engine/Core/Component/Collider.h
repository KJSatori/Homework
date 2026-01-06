#ifndef COLLIDER_H
#define COLLIDER_H

#include "ColliderType.h"
#include "../../Math/Shape.h"
#include "../../Math/Circle.h"
#include "../../Math/Rectangle.h"
#include "Component.h"
#include "../../Math/Vector2.h"
#include <memory>
#include "../GameObject/GameObject.h"
#include "Debug.h"

using namespace std;

struct Collider : public Component
{
    ColliderType type;
    shared_ptr<Shape> shape;
    Vector2 offset;

    Collider(GameObject* owner) : Component(owner), offset(Vector2::Zero()) {}

    void SetCircle(const Vector2& center, float radius);

    void SetRectangle(const Vector2& center, float width, float height);

    void SetRectangle(const Vector2& topLeft, const Vector2& rightBottom);

    void Update(float deltaTime) override;

    bool Intersects(const Collider& other) const;

    AABB GetAABB() const;

    /// @brief fxxk 碰撞遮罩未实现
    /// @param other 
    /// @return 
    bool CanCollideWith(const Collider& other) const { return true; }
};

#endif