#ifndef VECTOR2INT_H
#define VECTOR2INT_H

#include <iostream>
#include <functional>

using namespace std;

struct Vector2Int
{
    int row;
    int col;

    Vector2Int(int r = 0, int c = 0): row(r), col(c) {}

    bool operator==(const Vector2Int& other) const
    {
        return row == other.row && col == other.col;
    }
    bool operator!=(const Vector2Int& other) const
    {
        return row != other.row || col != other.col;
    }
    Vector2Int operator+(const Vector2Int& other) const
    {
        return Vector2Int(row + other.row, col + other.col);
    }
    Vector2Int operator-(const Vector2Int& other) const
    {
        return Vector2Int(row - other.row, col - other.col);
    }
    Vector2Int& operator+=(const Vector2Int& other)
    {
        row += other.row;
        col += other.col;
        return *this;
    }
    Vector2Int& operator-=(const Vector2Int& other)
    {
        row -= other.row;
        col -= other.col;
        return *this;
    }
};

struct Vector2IntHash
{
    size_t operator()(const Vector2Int& v) const
    {
        return hash<int>()(v.row) ^ (hash<int>()(v.col) << 1);
    }
};


#endif