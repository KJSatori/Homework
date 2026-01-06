#ifndef MOVEMENTPATTERN_H
#define MOVEMENTPATTERN_H

#include "../../Math/Vector2.h"

class MovementPattern
{
protected:
    Vector2 origin = Vector2(0, 0);
public:
    virtual ~MovementPattern() = default;
    virtual Vector2 GetPosition(float t) const = 0;
    virtual void SetOrigin(const Vector2& o)
    {
        origin = o;
    }
};

#endif