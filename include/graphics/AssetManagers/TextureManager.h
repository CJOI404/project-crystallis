#pragma once
#include <psptypes.h>
#include "GlobalDefs.h"
#include <unordered_map>
#include "graphics/Assets/Texture.h"

namespace TextureManager {

        void swizzle_fast(u8* out, const u8* in, unsigned int width, unsigned int height);

        Texture* load(const char* texturePath, int textureWidth, int texturHeight);
        void unload(Texture* tex);
        void unload(const char* texturePath);
        void clear();

        extern std::unordered_map<std::string, Texture> textures;
};