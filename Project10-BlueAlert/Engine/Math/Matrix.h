#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include "Vector2.h"

/// @brief 这会需要一些线性代数知识
struct Matrix
{
    float m[3][3]; // 3x3 矩阵用于2D变换

    /// @brief 单位矩阵
    Matrix()
    {
        m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
        m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
        m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
    }

    /// @brief 平移矩阵
    static Matrix Translation(float tx, float ty)
    {
        Matrix result;
        result.m[0][2] = tx;
        result.m[1][2] = ty;
        return result;
    }

    /// @brief 旋转矩阵
    /// @param angle 单位是 度 °
    static Matrix Rotation(float angle)
    {
        Matrix result;
        float rad = angle * (3.14159265f / 180.0f);
        float cosA = cos(rad);
        float sinA = sin(rad);

        result.m[0][0] = cosA;  result.m[0][1] = -sinA;
        result.m[1][0] = sinA;  result.m[1][1] = cosA;
        return result;
    }

    /// @brief 缩放矩阵
    static Matrix Scaling(float sx, float sy)
    {
        Matrix result;
        result.m[0][0] = sx;
        result.m[1][1] = sy;
        return result;
    }

    Matrix operator*(const Matrix& other) const
    {
        Matrix result;
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                result.m[i][j] = m[i][0] * other.m[0][j] +
                                 m[i][1] * other.m[1][j] +
                                 m[i][2] * other.m[2][j];
            }
        }
        return result;
    }

    /// @brief 向量变换
    Vector2 TransformPoint(const Vector2& v) const
    {
        float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2];
        float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2];
        return Vector2(x, y);
    }
};

#endif