#include "Circle.h"
#include "Rectangle.h"

bool Circle::Intersects(const Shape &other) const
{
    return other.IntersectsWithCircle(*this);
}

bool Circle::IntersectsWithRectangle(const Rectangle &rect) const
{
    Vector2 circleCenter = GetCenter();
    Vector2 topLeft = rect.GetTopLeft();
    Vector2 bottomRight = rect.GetBottomRight();

    // clamp 圆心到矩形边界
    float closestX = std::max(topLeft.x, std::min(circleCenter.x, bottomRight.x));
    float closestY = std::max(topLeft.y, std::min(circleCenter.y, bottomRight.y));

    // 最近点
    Vector2 closestPoint(closestX, closestY);

    // 距离是否小于半径
    float distSq = (circleCenter - closestPoint).SquareMagnitude();
    return distSq <= GetRadius() * GetRadius();
}

bool Circle::IntersectsWithCircle(const Circle &circle) const
{
    Vector2 diff = circle.GetCenter() - GetCenter();
    float radiiSum = circle.GetRadius() + GetRadius();
    return diff.SquareMagnitude() <= radiiSum * radiiSum;
}
