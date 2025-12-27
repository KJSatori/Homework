#ifndef RENDERER_H
#define RENDERER_H

#include "../Math/Vector2.h"

struct Renderer {
    float xScale = 1.0f;
    float yScale = 1.0f;

    virtual void DrawPoint(const Vector2& pos, char ch, int colorPair = 1) = 0;
    virtual void DrawWidePoint(const Vector2& pos, wchar_t ch, int colorPair = 1) = 0;
    virtual void DrawMsg(const Vector2& pos, const wchar_t* msg) = 0;
    virtual void Clear() = 0;
    virtual void Erase() = 0;
    virtual void Refresh() = 0;
    virtual ~Renderer() {}

    float GetXScale() { return xScale; }
    float GetYScale() { return yScale; }
};


#endif