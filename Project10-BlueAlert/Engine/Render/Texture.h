#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <string>

struct Texture {
    SDL_Texture* tex = nullptr;
    int width = 0;
    int height = 0;
    std::string key;
};

#include <unordered_map> 
// 文本纹理缓存结构 不缓存文本就会变得超级无敌卡顿
struct TextTexture 
{ 
    SDL_Texture* tex = nullptr; 
    int width = 0; 
    int height = 0; 
};

#endif
