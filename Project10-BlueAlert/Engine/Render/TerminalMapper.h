#ifndef TERMINALMAPPER_H
#define TERMINALMAPPER_H

#include "../Math/Vector2.h"

struct TerminalMapper
{
    static Vector2 ToTerminalCoordinates(const Vector2& worldPos)
    {
        return Vector2(worldPos.x*2, worldPos.y);
    }
};

#endif