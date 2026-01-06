#include "Renderer.h" 
#include <SDL2/SDL_image.h> 
#include <SDL2/SDL_ttf.h> 
#include <cmath> 
#include <iostream>
#include "../Core/Component/Debug.h"

#include <cstdint>

// 用于构建文本缓存key的帮助函数
static std::string MakeTextCacheKey(const std::string& fontKey, const std::string& text) 
{ 
    std::string key; key.reserve(fontKey.size() + 1 + text.size()); 
    key += fontKey; key.push_back('\0'); 
    // separator unlikely to appear in fontKey 
    key += text; 
    return key; 
}

// 辅助函数 
static double PerfMs(uint64_t start, uint64_t end) { return (double)(end - start) * 1000.0 / (double)SDL_GetPerformanceFrequency(); }

Renderer::Renderer(int w, int h) : width(w), height(h)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return;
    }

    window = SDL_CreateWindow("BlueAlert", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return;
    }

    sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!sdlRenderer)
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return;
    }

    int imgFlags = IMG_INIT_PNG;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) 
    { 
        std::cerr << "[Renderer] IMG_Init failed: " << IMG_GetError() << std::endl;
        return;
    } 

    if (TTF_Init() == -1) 
    { 
        std::cerr << "[Renderer] TTF_Init failed: " << TTF_GetError() << std::endl; 
        return;
    }
    /*
    SDL_RendererInfo info;
    SDL_GetRendererInfo(sdlRenderer, &info);
    std::cout << "SDL Renderer: " << (info.name ? info.name : "unknown") << "\n";
    std::cout << "Flags: " << info.flags << "\n";
    if (info.flags & SDL_RENDERER_ACCELERATED) std::cout << "Renderer is accelerated\n";
    if (info.flags & SDL_RENDERER_PRESENTVSYNC) std::cout << "VSync enabled\n";
    if (info.flags & SDL_RENDERER_SOFTWARE) std::cout << "Software renderer\n";
    */
}

Renderer::~Renderer()
{
    std::vector<std::string> keys;
    keys.reserve(textures.size());
    for (const auto& kv : textures) keys.push_back(kv.first);

    for (const auto& k : keys) {
        UnloadTexture(k); // UnloadTexture 会安全地 erase 单个键
    }

    // 卸载字体（如果 fonts 存的是 TTF_Font*）
    for (auto& kv : fonts) {
        if (kv.second) {
            TTF_Font* f = static_cast<TTF_Font*>(kv.second);
            TTF_CloseFont(f);
        }
    }
    fonts.clear();

    if (sdlRenderer) {
        SDL_DestroyRenderer(sdlRenderer);
        sdlRenderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    // SDL_ttf / SDL_image / SDL 的退出顺序：先关闭子系统再 SDL_Quit
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


Vector2 Renderer::WorldToScreen(const Vector2& worldPos) const
{
    float sx = offsetX + worldPos.x * scale;
    float sy = offsetY + (viewportH - worldPos.y * scale);
    return Vector2(sx, sy);
}

void Renderer::UpdateViewport(int windowW, int windowH)
{
    height = windowH;
    width = windowW;

    float sx = (float)windowW / (float)logicalWidth;
    float sy = (float)windowH / (float)logicalHeight;
    scale = std::min(sx, sy);

    viewportW = (int)round(logicalWidth * scale);
    viewportH = (int)round(logicalHeight * scale);

    // 居中视口 
    offsetX = (windowW - viewportW) / 2.0f;
    offsetY = (windowH - viewportH) / 2.0f;
}

/// @brief 绘制材质 (裁剪图集功能未实现)
/// @param key      名称
/// @param worldPos 绘制坐标
/// @param logicalW 宽度
/// @param logicalH 高度
/// @param tint     颜色混合
/// @param angleDeg 角度
/// @param objScale 缩放
/// @param center   中心绘制 或者 右下角
/// @param overlayBlend 
void Renderer::DrawSprite(const std::string& key, const Vector2& worldPos, int logicalW, int logicalH,
    SDL_Color tint, float angleDeg, float objScale, bool center, bool overlayBlend)
{
    // uint64_t t2 = SDL_GetPerformanceCounter();

    auto it = textures.find(key);
    if (it == textures.end()) return;
    Texture& t = it->second;
    if (!t.tex) return;

    // 传入 0 则用纹理像素作为逻辑尺寸
    int useLogicalW = (logicalW > 0) ? logicalW : t.width;
    int useLogicalH = (logicalH > 0) ? logicalH : t.height;

    // 目标尺寸 = 逻辑尺寸 * 窗口映射比例 * 物体缩放
    int dstW = (int)round(useLogicalW * scale * objScale);
    int dstH = (int)round(useLogicalH * scale * objScale);
    
    // 将世界坐标映射为渲染坐标
    Vector2 screen = WorldToScreen(worldPos);
    SDL_Rect dst;
    dst.w = dstW;
    dst.h = dstH;
    if (center)
    {
        dst.x = (int)round(screen.x - dstW / 2.0f);
        dst.y = (int)round(screen.y - dstH / 2.0f);
    }
    else
    {
        dst.x = (int)round(screen.x);
        dst.y = (int)round(screen.y - dstH);
    }

    // 颜色调制
    SDL_SetTextureColorMod(t.tex, tint.r, tint.g, tint.b);
    SDL_SetTextureAlphaMod(t.tex, tint.a);

    // 混合模式
    if (overlayBlend) SDL_SetTextureBlendMode(t.tex, SDL_BLENDMODE_BLEND);

    // 旋转中心
    SDL_Point centerPt = { dstW / 2, dstH / 2 };
    double angle = -angleDeg;

    // 裁剪图集功能未实现, 这里使用整张sprite
    SDL_Rect* src = nullptr;
    SDL_RenderCopyEx(sdlRenderer, t.tex, src, &dst, angle, &centerPt, SDL_FLIP_NONE);

    // uint64_t t3 = SDL_GetPerformanceCounter(); Debug::Log("[TIMING] DrawSprite ms=" + std::to_string(PerfMs(t2,t3)));
}

bool Renderer::LoadTexture(const std::string &key, const std::string &path)
{
    if (!sdlRenderer) return false;
    if (textures.find(key) != textures.end()) return true;  // 已加载

    SDL_Surface* surf = IMG_Load(path.c_str());
    if (!surf)
    {
        std::string errmsg = ""; 
        errmsg += "[Renderer] IMG_Load failed: ";
        errmsg += IMG_GetError();
        errmsg += " path=";
        errmsg += path;
        Debug::Log(errmsg);
        return false;
    }

    // 转换像素格式以创建纹理
    SDL_Texture* tex = SDL_CreateTextureFromSurface(sdlRenderer, surf);
    if (!tex)
    {
        std::string errmsg = ""; 
        errmsg += "[Renderer] SDL_CreateTextureFromSurface failed: ";
        errmsg += IMG_GetError();
        Debug::Log(errmsg);
        SDL_FreeSurface(surf);
        return false;
    }

    Texture t;
    t.tex = tex;
    t.width = surf->w;
    t.height = surf->h;
    t.key = key;
    textures[key] = t;

    SDL_FreeSurface(surf);
    return true;
}

void Renderer::UnloadTexture(const std::string &key)
{
    auto it = textures.find(key);
    if (it == textures.end()) return;
    if (it->second.tex)
    {
        SDL_DestroyTexture(it->second.tex);
        it->second.tex = nullptr;
    }
    textures.erase(it);
}

// DrawFilledRectColored：worldPos 为中心或左下（center 参数）
void Renderer::DrawFilledRectColored(const Vector2& worldPos, float logicalW, float logicalH, 
        SDL_Color tint, float angleDeg, float objScale, bool center)
{
    Vector2 screen = WorldToScreen(worldPos);
    int dstW = (int)round(logicalW * scale * objScale);
    int dstH = (int)round(logicalH * scale * objScale);

    SDL_Rect dst;
    dst.w = dstW;
    dst.h = dstH;
    if (center) {
        dst.x = (int)round(screen.x - dstW / 2.0f);
        dst.y = (int)round(screen.y - dstH / 2.0f);
    } else {
        // worldPos is bottom-left in logic -> convert to top-left pixel
        dst.x = (int)round(screen.x);
        dst.y = (int)round(screen.y - dstH); // screen.y is bottom pixel
    }

    SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(sdlRenderer, tint.r, tint.g, tint.b, tint.a);
    SDL_RenderFillRect(sdlRenderer, &dst);
}

// DrawFilledCircle：scanline 填充（worldPos 为中心或底部）
void Renderer::DrawFilledCircle(const Vector2& worldPos, float logicalRadius, SDL_Color tint, float objScale, bool center)
{
    Vector2 screen = WorldToScreen(worldPos);
    int radiusPx = (int)round(logicalRadius * scale * objScale);
    int cx = (int)round(screen.x);
    int cy = (int)round(screen.y);

    // 如果 worldPos 表示底部中心，则把 cy 上移 radiusPx
    if (!center) {
        cy = cy - radiusPx;
    }

    SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(sdlRenderer, tint.r, tint.g, tint.b, tint.a);

    // 简单的填充圆：对每个 dy 画水平线
    for (int dy = -radiusPx; dy <= radiusPx; ++dy) {
        int dx = (int)floor(sqrt((double)radiusPx * radiusPx - (double)dy * dy));
        int x1 = cx - dx;
        int x2 = cx + dx;
        int y = cy + dy;
        SDL_RenderDrawLine(sdlRenderer, x1, y, x2, y);
    }
}

// LoadFont / DrawText（使用 SDL_ttf）
bool Renderer::LoadFont(const std::string& key, const std::string& path, int ptsize)
{
    if (fonts.find(key) != fonts.end()) return true;
    TTF_Font* f = TTF_OpenFont(path.c_str(), ptsize);
    if (!f)
    {
        std::string errmsg = ""; 
        errmsg += "[Renderer] TTF_OpenFont failed: ";
        errmsg += IMG_GetError();
        errmsg += " path=";
        errmsg += path;
        Debug::Log(errmsg);
        return false;
    }
    fonts[key] = f;
    return true;
}

void Renderer::DrawText(const std::string& fontKey, const std::string& text, const Vector2& worldPos, SDL_Color tint, float angleDeg, float objScale, bool center)
{
    if (text.empty()) return;
    auto fit = fonts.find(fontKey);
    if (fit == fonts.end()) return;
    TTF_Font* f = static_cast<TTF_Font*>(fit->second);
    if (!f) return;

    std::string cacheKey = MakeTextCacheKey(fontKey, text);
    TextTexture* tt = nullptr;
    auto it = textCache.find(cacheKey);
    if (it != textCache.end()) {
        tt = &it->second;
    } else {
        // 创建 surface + texture 并缓存
        SDL_Surface* surf = TTF_RenderUTF8_Blended(f, text.c_str(), tint);
        if (!surf) {
            Debug::Log(std::string("[Renderer] TTF_RenderUTF8_Blended failed: ") + TTF_GetError());
            return;
        }

        // 可选：如果 surf 太大，先缩放 surface（降采样）以限制纹理尺寸
        const int MAX_TEX_SIZE = 2048; // 根据显卡能力调整
        SDL_Surface* finalSurf = surf;
        if (surf->w > MAX_TEX_SIZE || surf->h > MAX_TEX_SIZE) {
            float scaleX = (float)MAX_TEX_SIZE / (float)surf->w;
            float scaleY = (float)MAX_TEX_SIZE / (float)surf->h;
            float s = std::min(scaleX, scaleY);
            int newW = (int)round(surf->w * s);
            int newH = (int)round(surf->h * s);
            SDL_Surface* scaled = SDL_CreateRGBSurfaceWithFormat(0, newW, newH, surf->format->BitsPerPixel, surf->format->format);
            if (scaled) {
                SDL_Rect dstRect{0,0,newW,newH};
                SDL_BlitScaled(surf, nullptr, scaled, &dstRect);
                finalSurf = scaled;
                SDL_FreeSurface(surf);
            } else {
                // 缩放失败，继续用原 surf
                finalSurf = surf;
            }
        }

        SDL_Texture* tex = SDL_CreateTextureFromSurface(sdlRenderer, finalSurf);
        if (finalSurf != surf) SDL_FreeSurface(finalSurf);
        if (!tex) {
            Debug::Log(std::string("[Renderer] SDL_CreateTextureFromSurface failed: ") + SDL_GetError());
            return;
        }

        TextTexture newTT;
        newTT.tex = tex;
        newTT.width = surf->w; // 注意：如果缩放过，surf 已被释放，使用 finalSurf 的尺寸更准确
        newTT.height = surf->h;
        // If we scaled, we should set width/height from finalSurf; recompute:
        // (we used surf variable earlier; to be safe, query texture size)
        int tw, th;
        SDL_QueryTexture(tex, nullptr, nullptr, &tw, &th);
        newTT.width = tw;
        newTT.height = th;

        // insert into cache
        textCache.emplace(cacheKey, newTT);
        tt = &textCache[cacheKey];
    }

    if (!tt || !tt->tex) return;

    Vector2 screen = WorldToScreen(worldPos);
    int dstW = (int)round(tt->width * scale * objScale);
    int dstH = (int)round(tt->height * scale * objScale);
    SDL_Rect dst;
    dst.w = dstW;
    dst.h = dstH;
    if (center)
    {
        dst.x = (int)round(screen.x - dstW / 2.0f);
        dst.y = (int)round(screen.y - dstH / 2.0f);
    }
    else
    {
        dst.x = (int)round(screen.x);
        dst.y = (int)round(screen.y - dstH);
    }

    SDL_RenderCopyEx(sdlRenderer, tt->tex, nullptr, &dst, angleDeg, nullptr, SDL_FLIP_NONE);
}

void Renderer::UnloadFont(const std::string &key)
{
    auto it = fonts.find(key);
    if (it == fonts.end()) return;
    TTF_Font* f = static_cast<TTF_Font*>(it->second);
    if (f) TTF_CloseFont(f);
    fonts.erase(it);

    // 清除这个字体的文字缓存
    std::vector<std::string> toErase;
    for (auto &kv : textCache)
    {
        if (kv.first.rfind(key + std::string("\0"), 0) == 0)
        {
            if (kv.second.tex) SDL_DestroyTexture(kv.second.tex);
            toErase.push_back(kv.first);
        }
    }
    for (auto &k : toErase) textCache.erase(k);
}

void Renderer::ClearTextCache()
{
    for (auto &kv : textCache) 
    { 
        if (kv.second.tex) 
        { 
            SDL_DestroyTexture(kv.second.tex); 
            kv.second.tex = nullptr; 
        } 
    } 
    textCache.clear();
}

void Renderer::InvalidateText(const std::string &fontKey, const std::string &text)
{
    std::string key = MakeTextCacheKey(fontKey, text); 
    auto it = textCache.find(key); 
    if (it != textCache.end()) 
    { 
        if (it->second.tex) SDL_DestroyTexture(it->second.tex); 
        textCache.erase(it); 
    }
}

void Renderer::Clear()
{
    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer);
}

void Renderer::Refresh()
{
    //uint64_t t4 = SDL_GetPerformanceCounter();

    SDL_RenderPresent(sdlRenderer);

    //uint64_t t5 = SDL_GetPerformanceCounter(); Debug::Log("[TIMING] Present ms=" + std::to_string(PerfMs(t4,t5)));
}

void Renderer::DrawPoint(const Vector2& pos, char ch, int colorPair)
{
    Vector2 p = WorldToScreen(pos);
    int rw = std::max(1, (int)round(scale)); // 每个逻辑单位对应 scale 像素
    int rh = std::max(1, (int)round(scale));
    SDL_Rect r{ (int)round(p.x - rw/2.0f), (int)round(p.y - rh/2.0f), rw, rh };
    SDL_SetRenderDrawColor(sdlRenderer, 255,255,255,255);
    SDL_RenderFillRect(sdlRenderer, &r);
}

void Renderer::DrawWidePoint(const Vector2& pos, wchar_t ch, int colorPair)
{
    Vector2 p = WorldToScreen(pos);
    int rw = std::max(1, (int)round(scale)); // 每个逻辑单位对应 scale 像素
    int rh = std::max(1, (int)round(scale));
    SDL_Rect r{ (int)round(p.x - rw/2.0f), (int)round(p.y - rh/2.0f), rw, rh };
    SDL_SetRenderDrawColor(sdlRenderer, 255,255,255,255);
    SDL_RenderFillRect(sdlRenderer, &r);
}

void Renderer::DrawMsg(const Vector2& pos, const wchar_t* msg)
{
    // MVP: 输出到控制台，后续可用 SDL_ttf 渲染
    Vector2 p = WorldToScreen(pos);
    std::wstring ws(msg);
    std::string s(ws.begin(), ws.end());
    std::cout << "DrawMsg at (" << p.x << "," << p.y << "): " << s << std::endl;
}