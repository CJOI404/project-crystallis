#include "graphics/TextureManager.h"
#include <stb_image.h>
#include <psputils.h>
#include <pspsysmem.h>

namespace TextureManager {

    std::unordered_map<std::string, Texture> textures;

    Texture* load(const char* texturePath, int textureSize){
        // Already loaded?
        auto it = textures.find(texturePath);

        if (it != textures.end()){
            printf("%s already loaded, returning reference\n", texturePath);
            fflush(stdout);
            return &it->second;
        }

        //Check space availability

        // STBI_rgb_alpha forces 4 bytes per pixel.
        size_t requiredMemory = (size_t)textureSize * textureSize * 4;
        // Check available memory here.
        size_t freeMemory = sceKernelTotalFreeMemSize();
        if (requiredMemory > freeMemory){
            printf("load failed: not enough space. Required memory: %d, Available memory: %d\n", requiredMemory, freeMemory);
            fflush(stdout);
            return nullptr;
        }
        // Create a new texture

        Texture texture;
        texture.data = (uint32_t *) stbi_load(texturePath, &(textureSize), &(textureSize), NULL, STBI_rgb_alpha);
        texture.size = textureSize;

        if (!texture.data)
            return nullptr;

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

        if (it == textures.end())
            return;

        if (it->second.data)
            stbi_image_free(it->second.data);

        textures.erase(it);
    }

    void clear()
    {
        textures.clear();
    }

}