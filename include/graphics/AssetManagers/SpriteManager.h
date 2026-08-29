#pragma once
#include <map>
#include <string>
#include "graphics/Assets/Sprite.h"
#include <unordered_map>

namespace SpriteManager {
    
    extern std::unordered_map<std::string, Sprite> sprites;

    Sprite* getSprite(const std::string& id);

    void registerSprite(const std::string& id, Texture* tex, float x, float y, float w, float h);

    void unload(const std::string& id);
};