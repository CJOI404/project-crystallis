#pragma once
#include "GlobalDefs.h"
#include <unordered_map>
#include "graphics/Texture.h"

namespace TextureManager {

        Texture* load(const char* texturePath, int textureSize);
        void unload(const char* texturePath);
        void clear();

        extern std::unordered_map<std::string, Texture> textures;
};