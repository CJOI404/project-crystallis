#include "graphics/TextureManager.h"
#include <stb_image.h>
#include <psputils.h>
#include <pspsysmem.h>
#include <pspsdk.h>

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

        // Create a new texture

        Texture texture;
        texture.data = (uint32_t *) stbi_load(texturePath, &(textureSize), &(textureSize), NULL, STBI_rgb_alpha);
        texture.size = textureSize;

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

        if (it == textures.end())
            return;

        if (it->second.data)
            stbi_image_free(it->second.data);

        printf("Unloaded %s", texturePath);
        fflush(stdout);

        textures.erase(it);
    }

    void clear()
    {
        textures.clear();
    }

}