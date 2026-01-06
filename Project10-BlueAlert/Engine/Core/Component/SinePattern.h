#ifndef SINEPATTERN_H
#define SINEPATTERN_H

#include "MovementPattern.h"

class SinePattern : public MovementPattern
{
public:
    Vector2 forward = Vector2(0, -1);
    Vector2 right = Vector2(1, 0);
    float speed = 200.0f;       
    float amplitude = 40.0f;    
    float freq = 4.0f;          

    SinePattern() {}

    SinePattern(const Vector2& _forward, const Vector2& _right, float _speed, float _amplitute, float _freq) :
        forward(_forward), right(_right), speed(_speed), amplitude(_amplitute), freq(_freq) {}

    Vector2 GetPosition(float t) const override
    {
        Vector2 p = origin + forward * (speed * t);
        float phase = freq * t;
        p += right * (amplitude * sinf(phase));
        return p;
    }
};

#endif