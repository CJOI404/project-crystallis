#pragma once
#include <map>
#include <string>
#include "graphics/Sprite.h"
#include <unordered_map>

namespace SpriteManager {
    
    std::unordered_map<std::string, Sprite> sprites;

    void registerSprite(const std::string& id, Texture* tex, float x, float y, float w, float h) {
        Sprite data;
        data.texture = tex;
        data.u0 = x;
        data.v0 = y;
        data.u1 = x + w;
        data.v1 = y + h;
        data.width = w;
        data.height = h;

        sprites[id] = data;
    }

    Sprite* getSprite(const std::string& id) {
        auto it = sprites.find(id);
        if (it != sprites.end()) {
            return &it->second;
        }
        return nullptr;
    }

};