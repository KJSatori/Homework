#include "Collider.h"

void Collider::SetCircle(const Vector2 &center, float radius)
{
    type = ColliderType::Circle;
    offset = center - gameObject->transform.position;
    shape = make_shared<Circle>(center, radius);
}

void Collider::SetRectangle(const Vector2 &center, float width, float height)
{
    type = ColliderType::Rectangle;
    offset = center - gameObject->transform.position;
    shape = make_shared<Rectangle>(center, width, height);
}

void Collider::SetRectangle(const Vector2 &topLeft, const Vector2 &rightBottom)
{
    type = ColliderType::Rectangle;
    offset = (topLeft + rightBottom)/2 - gameObject->transform.position;
    shape = make_shared<Rectangle>(topLeft, rightBottom);
}

void Collider::Update(float deltaTime)
{
    if (!shape) Debug::Log("[WARN Collider Update] shape 为空");
    shape->SetCenter(gameObject->transform.position + offset);
}

bool Collider::Intersects(const Collider &other) const
{
    if (!shape) Debug::Log("[WARN Collider Intersects] shape 为空");
    return shape->Intersects(*(other.shape));
}

AABB Collider::GetAABB() const
{
    AABB output;
    if (!shape)
    {
        Debug::Log("[WARN Collider GetAABB] shape 为空");
        return output;
    }
    Vector2 center = shape->GetCenter();
    switch (type)
    {
    case ColliderType::Rectangle:
        {
            auto rect = dynamic_pointer_cast<Rectangle>(shape);
            output.min = Vector2(rect->GetTopLeft().x, rect->GetBottomRight().y);
            output.max = Vector2(rect->GetBottomRight().x, rect->GetTopLeft().y);
            break;
        }
    case ColliderType::Circle:
        {
            auto circle = dynamic_pointer_cast<Circle>(shape);
            float r = circle->GetRadius();
            output.min = Vector2(circle->GetCenter() - Vector2(r, r));
            output.max = Vector2(circle->GetCenter() + Vector2(r, r));
            break;
        }
    }
    return AABB();
}
