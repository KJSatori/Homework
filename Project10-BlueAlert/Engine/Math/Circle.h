#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"
#include "Vector2.h"

struct Circle : public Shape
{
    Vector2 center;
    float radius;

    Circle(const Vector2& c, float r)
        : center(c), radius(r) {}

    Vector2 GetCenter() const { return center; }
    float GetRadius() const { return radius; }

    void SetCenter(const Vector2& c) override { center = c; }

    bool Contains(const Vector2& point) const override
    {
        Vector2 diff = point - center;
        return diff.Magnitude() <= radius;
    }

    bool Intersects(const Shape& other) const override;
    bool IntersectsWithRectangle(const struct Rectangle& rect) const override;
    bool IntersectsWithCircle(const struct Circle& circle) const override;
};

#endif