#include "Item.h"
#include <math.h>

/// @brief 逆时针式的通过dir生成dirpos, (1, 0) 为 dir = 1, (0, 0) 为 dir = 0 
/// @param dir 
/// @return 
Vector2Int Item::GenerateDirectionPosByDirection(int dir)
{
    if (dir == 0) return Vector2Int(0, 0);
    int r = 1;
    while (dir > 4 * r * (r + 1)) r++;

    int start = 4 * (r - 1) * r + 1;
    int offset = dir - start;

    int edgeLen = 2 * r;

    if (offset < edgeLen) {
        return Vector2Int(offset-r, -r);
    } else if (offset < edgeLen*2) {
        return Vector2Int(r, offset - edgeLen - r);
    } else if (offset < edgeLen*3) {
        return Vector2Int(offset - 2*edgeLen - r + 1, r);
    } else {
        return Vector2Int(-r, offset - 3*edgeLen - r + 1);
    }
}

bool Item::IsPosInSelfborder(Vector2Int pos, int size)
{
    int upperBound = this->pos.row - size;
    int lowerBound = this->pos.row + size;
    int leftBound = this->pos.col - size;
    int rightBound = this->pos.col + size;
    return pos.row <= lowerBound && pos.row >= upperBound && pos.col <= rightBound && pos.col >= leftBound;
}

bool Item::IsPosInBorder(Vector2Int pos)
{
    return (pos.col <= MAXCOLPOS && pos.col >= MINCOLPOS && pos.row <= MAXROWPOS && pos.row >= MINROWPOS);
}
