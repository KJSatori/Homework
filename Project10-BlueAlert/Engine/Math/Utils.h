#ifndef UTILS_H
#define UTILS_H

#include <random>
#include <algorithm>
#include <cmath>
#include "Vector2.h"

inline double RandomDouble(double a, double b)
{
    if (a == b) return a;
    if (a > b) std::swap(a, b);

    // 线程局部的随机数引擎，避免多线程竞争
    static thread_local std::mt19937_64 engine(std::random_device{}());
    std::uniform_real_distribution<double> dist(a, b); // 生成区间 [a, b)
    return dist(engine);
}

// 返回 a 与 b 之间的夹角，范围 [0, 180]
inline float AngleBetweenDegrees(const Vector2& a, const Vector2& b)
{
    const float EPS = 1e-6f;
    float magA = a.Magnitude();
    float magB = b.Magnitude();
    if (magA < EPS || magB < EPS) return 0.0f; // 任一向量接近零向量，返回 0

    float dot = a.Dot(b);
    float cosv = dot / (magA * magB);
    // 数值稳定性：把 cosv 限制在 [-1,1]
    cosv = std::max(-1.0f, std::min(1.0f, cosv));
    float rad = std::acos(cosv);
    return rad * (180.0f / static_cast<float>(M_PI));
}

// 返回从 from 指向 to 的有符号角度，范围 (-180, 180]
inline float SignedAngleDegrees(const Vector2& from, const Vector2& to)
{
    const float EPS = 1e-6f;
    float magF = from.Magnitude();
    float magT = to.Magnitude();
    if (magF < EPS || magT < EPS) return 0.0f;

    float dot = from.Dot(to);
    float cross = from.Cross(to); // 标量，等于 ax*by - ay*bx
    float rad = std::atan2(cross, dot); // atan2(y, x)
    return rad * (180.0f / static_cast<float>(M_PI));
}

/// @brief 将 (ix, iy) 映射为 int64_t 用于空间哈希网格的哈希键转换
/// @param ix 
/// @param iy 
/// @return 
inline int64_t CellKey(int ix, int iy)
{
    return ( (int64_t)ix << 32 ) ^ (uint32_t)iy;
}

/// @brief 一个三次函数
/// @param x 变量 x ∈ [0, t]
/// @param t f(0) = 0, f(t) = t
/// @param w 拐点出现的时间
/// @param d 拐点的导数值
/// @return 
inline float DurationCubeFunc(float x, float t, float w, float d)
{
    float denominator = 3*w*w-3*w*t+t*t;
    float a = (1-d)/denominator;
    float b = 3*w*(d-1)/denominator;
    float c = (3*w*w+d*t*t-3*w*t*d)/denominator;
    return a*x*x*x+b*x*x+c*x;
}

#endif