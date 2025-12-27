#include "Rectangle.h"
#include "Circle.h" 

bool Rectangle::Intersects(const Shape &other) const
{
    return other.IntersectsWithRectangle(*this);
}

bool Rectangle::IntersectsWithRectangle(const Rectangle &rect) const
{
    Vector2 LeftT1 = GetTopLeft();
    Vector2 RightB1 = GetBottomRight();
    Vector2 LeftT2 = rect.GetTopLeft();
    Vector2 RightB2 = rect.GetBottomRight();
    bool isIntersecting = !(LeftT1.x > RightB2.x || RightB1.x < LeftT2.x ||
                         LeftT1.y > RightB2.y || RightB1.y < LeftT2.y);
    return isIntersecting;
}

bool Rectangle::IntersectsWithCircle(const Circle &circle) const
{
    Vector2 circleCenter = circle.GetCenter();
    Vector2 topLeft = GetTopLeft();
    Vector2 bottomRight = GetBottomRight();

    // clamp 圆心到矩形边界
    float closestX = std::max(topLeft.x, std::min(circleCenter.x, bottomRight.x));
    float closestY = std::max(topLeft.y, std::min(circleCenter.y, bottomRight.y));

    // 最近点
    Vector2 closestPoint(closestX, closestY);

    // 距离是否小于半径
    float distSq = (circleCenter - closestPoint).SquareMagnitude();
    return distSq <= circle.GetRadius() * circle.GetRadius();
}
