#include "graphics/AssetManagers/TextureManager.h"
#include <stb_image.h>
#include <psputils.h>
#include <pspsysmem.h>
#include <pspsdk.h>

namespace TextureManager {

    std::unordered_map<std::string, Texture> textures;

    void swizzle32(uint8_t* out, const uint8_t* in, unsigned int width, unsigned int height) {
        unsigned int blockx, blocky, j;
        
        // 16 bytes = 4 pixels in 32-bit mode
        unsigned int width_blocks = width / 4;
        unsigned int height_blocks = height / 8;
        
        // Pitch jump in uint32_t elements (width in pixels - 4 pixels processed per line)
        unsigned int src_pitch = width - 4;
        unsigned int src_row = width * 8 * 4; // 8 rows of 32-bit pixels in bytes
        
        const uint8_t* ysrc = in;
        uint32_t* dst = (uint32_t*)out;
        
        for (blocky = 0; blocky < height_blocks; ++blocky) {
            const uint8_t* xsrc = ysrc;
            for (blockx = 0; blockx < width_blocks; ++blockx) {
                const uint32_t* src = (uint32_t*)xsrc;
                for (j = 0; j < 8; ++j) {
                    *(dst++) = *(src++);
                    *(dst++) = *(src++);
                    *(dst++) = *(src++);
                    *(dst++) = *(src++);
                    src += src_pitch;
                }
                xsrc += 16; // Advance 16 bytes (4 pixels) horizontally
            }
            ysrc += src_row;
        }
    }

    Texture* load(const char* texturePath, int textureWidth, int textureHeight){
        // Already loaded?
        auto it = textures.find(texturePath);

        if (it != textures.end()){
            printf("%s already loaded, returning reference\n", texturePath);
            fflush(stdout);
            it->second.refCount++;
            return &it->second;
        }

        // Create a new texture

        Texture texture;

        uint8_t* originalData = (uint8_t *) stbi_load(texturePath, &(textureWidth), &(textureHeight), NULL, STBI_rgb_alpha);
        uint8_t* swizzledData = (uint8_t *) malloc(textureWidth * textureHeight * 4);

        swizzle32(swizzledData, originalData, textureWidth, textureHeight);

        // texture.data = (uint32_t *) stbi_load(texturePath, &(textureSize), &(textureSize), NULL, STBI_rgb_alpha);
        texture.data = (uint32_t*) swizzledData;
        texture.width = textureWidth;
        texture.height = textureHeight;
        texture.refCount = 1;

        if (!texture.data){
            printf("%s Not Found.", texturePath);
            fflush(stdout);
            return nullptr;
        }

        sceKernelDcacheWritebackAll();

        auto result = textures.emplace(texturePath, texture);

        if (!result.second)
        {
            // Something went wrong inserting it
            stbi_image_free(texture.data);
            return nullptr;
        }

        printf("%s Successfully Loaded\n", texturePath);
        fflush(stdout);
        return &result.first->second;
    }

    
    void unload(const char* texturePath)
    {
        auto it = textures.find(texturePath);

        if (it == textures.end()) return;

        it->second.refCount--;
        if (it->second.refCount <= 0 && it->second.data) {
            stbi_image_free(it->second.data);

            textures.erase(it);

            printf("UNLOADED %s\n", texturePath);
            fflush(stdout);
        } else {
            printf("DID NOT UNLOAD: %s, REFCOUNT: %d\n", texturePath, it->second.refCount);
            fflush(stdout);
        }

    }

    void clear()
    {
        textures.clear();
    }

}