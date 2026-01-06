#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "../Math/Vector2.h"
#include <string>
#include "Texture.h"
#include <unordered_map>

struct Renderer
{
    SDL_Window* window = nullptr;
    SDL_Renderer* sdlRenderer = nullptr;

    int logicalWidth = 800;
    int logicalHeight = 600;

    // 像素 / 逻辑 换算单位
    float scale = 1.0f;

    // 视口左上角在窗口中的像素坐标
    float offsetX = 0.0f;
    float offsetY = 0.0f;

    // 逻辑视口在窗口中的像素宽度 = logical??? * scale
    int viewportW = 0;
    int viewportH = 0;

    // 窗口大小
    int width = 800;
    int height = 600;

    std::unordered_map<std::string, Texture> textures; 
    std::unordered_map<std::string, void*> fonts;

    // 不缓存文本就会卡的要死
    std::unordered_map<std::string, TextTexture> textCache;

    // 构造与析构
    Renderer(int w = 800, int h = 600);
    ~Renderer();

    // 渲染接口（保留原来名字以兼容现有组件）
    void Clear();
    void Refresh();

    // 绘制文字
    void DrawPoint(const Vector2& pos, char ch, int colorPair = 1);
    void DrawWidePoint(const Vector2& pos, wchar_t ch, int colorPair = 1);
    void DrawMsg(const Vector2& pos, const wchar_t* msg);

    // 绘制材质
    void DrawSprite(const std::string& key, const Vector2& worldPos, int logicalW, int logicalH,
        SDL_Color tint = {255, 255, 255, 255}, float angleDeg = 0.0f, float scale = 1.0f, bool center = true, bool overlayBlend = false);
    bool LoadTexture(const std::string& key, const std::string& path);
    void UnloadTexture(const std::string& key);

    // 绘制图形 fxxk transform未实现
    void DrawFilledRectColored(const Vector2& worldPos, float logicalW, float logicalH, 
        SDL_Color tint = {255, 255, 255, 255}, float angleDeg = 0.0f, float scale = 1.0f, bool center = true); 
    void DrawFilledCircle(const Vector2& worldPos, float logicalRadius, SDL_Color tint = {255, 255, 255, 255}, float scale = 1.0f, bool center = true); 

    // 绘制有字体的文字
    bool LoadFont(const std::string& key, const std::string& path, int ptsize); 
    void DrawText(const std::string& fontKey, const std::string& text, const Vector2& worldPos, SDL_Color tint, float angleDeg = 0.0f, float scale = 1.0f, bool center = true);
    void UnloadFont(const std::string& key);
    void ClearTextCache(); 
    void InvalidateText(const std::string& fontKey, const std::string& text);

    // 帮助方法
    Vector2 WorldToScreen(const Vector2& worldPos) const;
    void UpdateViewport(int windowW, int windowH);

    // 窗口访问与文本输入控制
    SDL_Window* GetWindow() const { return window; }
    void StartTextInput() { SDL_StartTextInput(); }
    void StopTextInput() { SDL_StopTextInput(); }

    // 禁止拷贝
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
};

#endif
