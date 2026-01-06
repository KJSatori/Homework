#ifndef RENDERERCOMPONENT_H
#define RENDERERCOMPONENT_H

#include "../GameObject/GameObject.h"
#include "../../Render/Renderer.h"
#include "../../Math/Vector2.h"
#include <string>
#include "../../Utils/CharWidth.h"
#include <algorithm>
#include "Debug.h"

enum class RenderKind
{
    Empty,
    Rectangle,
    Circle,
    Sprite,
    Text
};

enum class RenderLayer : int
{
    BackGround  = 0,
    Default     = 100,
    Bullet      = 150,
    Effects     = 200,
    Foreground  = 300,
    UI          = 1000
};

/// @brief 暂时不会应用transform的scale，但是可以自己设定scale
class RendererComponent : public Component
{
private:
    RenderKind kind;
    std::string text;      // 当 kind == RenderKind::Text
    std::string fontKey;
    std::string spriteKey;  // 当 kind == Renderkind::Sprite

    float scaleFactor = 1.0f;
    SDL_Color tint = {255, 255, 255, 255}; 
    float logicalW = 0.0f;     // 0 则默认按图像像素大小渲染
    float logicalH = 0.0f;
    float logicalRadius = 5.0f;
    bool center = true;
    bool applyTransform = true;
    float rotation = 0.0f;
    
    Vector2 offset = Vector2::Zero();

    // 渲染层级
    RenderLayer layer = RenderLayer::Default;
    int orderInLayer = 0;

public:
    RendererComponent(GameObject* owner = nullptr, RenderKind k = RenderKind::Empty, SDL_Color color = {255, 255, 255, 255}, bool applyTrans = false)
        : Component(owner), kind(k), tint(color), applyTransform(applyTrans) {}

    void SetCenter(bool c)                  { center    = c;    }
    void SetLayer(RenderLayer l)            { layer     = l;    }
    void SetColor(SDL_Color color)          { tint      = color;}
    void SetText(const std::string& t)      { text      = t;    }
    void SetFontKey(const std::string& key) { fontKey   = key;  }
    void SetOffset(const Vector2& ofs)      { offset    = ofs;  }
    void SetRadius(float r)                 { logicalRadius = r;}
    void SetSprite(const string& key)       { spriteKey     = key;  } 
    void SetRotation(float angle)           { rotation      = angle;}
    void SetScaleFactor(float s)            { scaleFactor   = s;    }
    void SetOrderInLayer(int order)         { orderInLayer  = order;}
    void SetApplyTransform(bool applyTrans) { applyTransform= applyTrans;                   }
    void SetAlpha(unsigned char alpha)      { tint      = {tint.r, tint.g, tint.b, alpha};  }
    void SetSize(float width, float height) { logicalW  = width; logicalH = height;         }

    RenderLayer GetLayer() const { return layer; }
    int GetOrderInLayer() const { return orderInLayer; }

    void Render(Renderer& renderer) override
    {
        // Debug::Log("Render: " + gameObject->name + " at " + to_string(gameObject->transform.position.x) + "," + to_string(gameObject->transform.position.y));

        Vector2 worldPos = gameObject->GetWorldPosition() + offset;
        float angleDeg = gameObject->transform.rotation + rotation;

        switch (kind)
        {
        case RenderKind::Rectangle:
            renderer.DrawFilledRectColored(worldPos, logicalW, logicalH, tint, angleDeg, scaleFactor, center);
            break;
        case RenderKind::Circle: 
            renderer.DrawFilledCircle(worldPos, logicalRadius * scaleFactor, tint, scaleFactor, center); 
            break; 
        case RenderKind::Sprite: 
            renderer.DrawSprite(spriteKey, worldPos, logicalW, logicalH, tint, angleDeg, scaleFactor, center, true); 
            break; 
        case RenderKind::Text: 
        {
            std::string s = text; 
            renderer.DrawText(fontKey, s, worldPos, tint, angleDeg, scaleFactor, center); 
            break; 
        }
        case RenderKind::Empty:
            Debug::Log(std::string("[Warning RendererComp] ") + gameObject->name + " RenderKind is Empty!");
            break;
        }
    }
};

#endif