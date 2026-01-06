#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"
#include "Vector2.h"

struct Rectangle : public Shape {
    float width, height;

    Rectangle(const Vector2& c, float w, float h)
        : width(w), height(h) { center = c; }

    Rectangle(const Vector2& tl, const Vector2& br) {
        center = Vector2((tl.x + br.x) / 2, (tl.y + br.y) / 2);
        width = br.x - tl.x;
        height = br.y - tl.y;
    }

    Vector2 GetTopLeft() const {
        return Vector2(center.x - width / 2, center.y - height / 2);
    }

    Vector2 GetBottomRight() const {
        return Vector2(center.x + width / 2, center.y + height / 2);
    }

    bool Contains(const Vector2& point) const override {
        return point.x >= GetTopLeft().x && point.x <= GetBottomRight().x &&
               point.y >= GetTopLeft().y && point.y <= GetBottomRight().y;
    }

    bool Intersects(const Shape& other) const override;
    bool IntersectsWithRectangle(const struct Rectangle& rect) const override;
    bool IntersectsWithCircle(const struct Circle& circle) const override;
};



#endif