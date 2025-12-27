#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector2.h"
#include "Matrix.h"

struct Transform
{
    Vector2 position;
    float rotation; // 以度为单位
    Vector2 scale;

    Transform() : position(0, 0), rotation(0), scale(1, 1) {}

    void Translate(const Vector2& delta)
    {
        position += delta;
    }

    void Translate(float dx, float dy)
    {
        position += Vector2(dx, dy);
    }

    void SetPosition(const Vector2& newPosition)
    {
        position = newPosition;
    }

    void SetPosition(float x, float y)
    {
        position = Vector2(x, y);
    }

    void Rotate(float deltaAngle)
    {
        rotation += deltaAngle;
    }

    void SetRotation(float newAngle)
    {
        rotation = newAngle;
    }

    void SetScale(const Vector2& newScale)
    {
        scale = newScale;
    }

    void SetScale(float sx, float sy)
    {
        scale = Vector2(sx, sy);
    }

    Matrix GetMatrix() const {
        Matrix translationMatrix = Matrix::Translation(position.x, position.y);
        Matrix rotationMatrix = Matrix::Rotation(rotation);
        Matrix scalingMatrix = Matrix::Scaling(scale.x, scale.y);
        return translationMatrix * rotationMatrix * scalingMatrix;
    }
    
    Vector2 TransformPoint(const Vector2& point) const {
        Matrix transformMatrix = GetMatrix();
        return transformMatrix.TransformPoint(point);
    }
};

#endif