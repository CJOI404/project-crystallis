#pragma once
#include <map>
#include <string>
#include "graphics/Assets/Sprite.h"
#include <unordered_map>

namespace SpriteManager {
    
    extern std::unordered_map<std::string, Sprite> sprites;

    Sprite* getSprite(const char* id);

    void registerSprite(const char* id, Texture* tex, float x, float y, float w, float h);

    void unload(const char* id);
};