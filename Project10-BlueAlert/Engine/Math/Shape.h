#ifndef SHAPE_H
#define SHAPE_H

#include "Vector2.h"

struct Rectangle;
struct Circle;

struct Shape
{
    virtual void SetCenter(const Vector2& c) = 0;
    virtual bool Contains(const Vector2& point) const = 0;
    virtual bool Intersects(const Shape& other) const = 0;
    virtual bool IntersectsWithRectangle(const Rectangle& rect) const = 0;
    virtual bool IntersectsWithCircle(const Circle& circle) const = 0;
    virtual ~Shape() {}
};


#endif