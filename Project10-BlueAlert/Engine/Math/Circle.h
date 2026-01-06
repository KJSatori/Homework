#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"
#include "Vector2.h"

struct Circle : public Shape
{
    float radius;

    Circle(const Vector2& c, float r)
        : radius(r) { center = c; }
        
    float GetRadius() const { return radius; }

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