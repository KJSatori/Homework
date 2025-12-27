#ifndef COLLIDER_H
#define COLLIDER_H

#include "ColliderType.h"
#include "../../Math/Shape.h"
#include "../../Math/Circle.h"
#include "../../Math/Rectangle.h"
#include "Component.h"
#include "../GameObject/GameObject.h"
#include "../../Math/Vector2.h"
#include <memory>

struct Collider : public Component
{
    ColliderType type;
    shared_ptr<Shape> shape;
    Vector2 offset;

    Collider(GameObject* owner) : Component(owner), offset(Vector2::Zero()) {}

    void SetCircle(const Vector2& center, float radius)
    {
        type = ColliderType::Circle;
        offset = center - gameObject->transform.position;
        shape = make_shared<Circle>(center, radius);
    }

    void SetRectangle(const Vector2& center, float width, float height)
    {
        type = ColliderType::Rectangle;
        offset = center - gameObject->transform.position;
        shape = make_shared<Rectangle>(center, width, height);
    }

    void SetRectangle(const Vector2& topLeft, const Vector2& rightBottom)
    {
        type = ColliderType::Rectangle;
        offset = (topLeft + rightBottom)/2 - gameObject->transform.position;
        shape = make_shared<Rectangle>(topLeft, rightBottom);
    }

    void Update(float deltaTime) override
    {
        shape->SetCenter(gameObject->transform.position + offset);
    }

    bool Intersects(const Collider& other) const
    {
        return shape->Intersects(*(other.shape));
    }
};

#endif