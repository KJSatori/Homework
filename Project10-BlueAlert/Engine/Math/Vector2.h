#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <string>
#include <sstream>

struct Vector2
{
public:
    float x;
    float y;

    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}

    float Magnitude() const
    {
        return sqrt(x * x + y * y);
    }

    float SquareMagnitude() const
    {
        return x * x + y * y;
    }

    Vector2 Normalized() const
    {
        float mag = Magnitude();
        if (mag == 0) return Vector2(0, 0);
        return Vector2(x / mag, y / mag);
    }
    
    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator-() const
    {
        return Vector2(-x, -y);
    }

    Vector2 operator*(float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    friend Vector2 operator*(float scalar, Vector2 vector2)
    {
        return Vector2(vector2.x * scalar, vector2.y * scalar);
    }

    Vector2 operator/(float scalar) const
    {
        return Vector2(x / scalar, y / scalar);
    }

    Vector2& operator+=(const Vector2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    bool operator==(const Vector2& other) const
    {
        return Equals(other);
    }

    bool operator!=(const Vector2& other) const
    {
        return !Equals(other);
    }

    float Dot(const Vector2& other) const 
    { 
        return x * other.x + y * other.y; 
    }
     
    float Cross(const Vector2& other) const 
    { 
        return x * other.y - y * other.x;
    }

    /// @brief 鉴于浮点数的不精确性，这里添加一个容差参数 epsilon 来判断两个向量是否近似相等
    bool Equals(const Vector2& other, float epsilon = 1e-6f) const
    {
        return (fabs(x - other.x) < epsilon) && (fabs(y - other.y) < epsilon);
    }

    operator std::string() const
    {
        std::ostringstream ss;
        ss << "(" << x << "," << y << ")";
        return ss.str();
    }

    static Vector2 Zero() { return Vector2(0, 0); }
    static Vector2 One() { return Vector2(1, 1); }
    static Vector2 Up() { return Vector2(0, 1); }      
    static Vector2 Right() { return Vector2(1, 0); }
    static Vector2 Down() { return Vector2(0, -1); }     
    static Vector2 Left() { return Vector2(-1, 0); }
};

// 简单AABB结构
struct AABB { Vector2 min; Vector2 max; };

#endif