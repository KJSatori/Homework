#ifndef RENDERERCOMPONENT_H
#define RENDERERCOMPONENT_H


#include "../GameObject/GameObject.h"
#include "../../Render/Renderer.h"
#include "../../Math/Vector2.h"
#include <string>
#include "../../Utils/CharWidth.h"

struct RendererComponent : public Component
{
    wstring symbols;
    int colorPair;
    bool horizontalRender;

    RendererComponent(GameObject* owner = nullptr, const wstring& sym = L"*", int color = 1, bool horizontal = true)
        : Component(owner), symbols(sym), colorPair(color), horizontalRender(horizontal) {}

    void SetSymbol(const wstring& sym) { symbols = sym; }
    void SetColorPair(int color) { colorPair = color; }
    void SetHorizontal(bool isHorizon) { horizontalRender = isHorizon; }

    void Render(Renderer& renderer) override
    {
        Vector2 worldPos = gameObject->transform.position;
        int n = symbols.size();

        int totalWidth = 0;
        for (wchar_t ch : symbols)
        {
            int w = GetCharWidth(ch);
            if (w > 0) totalWidth += w;
        }

        int currentOffset = 0;
        float xScale = renderer.GetXScale();
        float yScale = renderer.GetYScale();
        for (int i = 0; i < n; ++i) 
        { 
            int w = GetCharWidth(symbols[i]);
            if (w < 0) w = 1; // fallback 
            float offsetX = (horizontalRender ? (currentOffset - totalWidth/2) : 0) * xScale; 
            float offsetY = (horizontalRender ? 0 : (i - n/2)) * yScale; 
            renderer.DrawWidePoint(worldPos + Vector2(offsetX, offsetY), symbols[i], colorPair); 
            currentOffset += w; // 累积宽度 
        }
    }
};

#endif