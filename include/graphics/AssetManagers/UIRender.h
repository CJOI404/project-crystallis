#pragma once
#include <pspgu.h>
#include <stdint.h>
#include <vector>
#include <string>
#include "graphics/AssetManagers/TextureManager.h"

namespace UI {
    struct FontChar {
        int id;
        int x, y;
        int width, height;
        int xoffset, yoffset;
        int xadvance, page, chnl;
    };

    //bitmap font doesn't go past id 128
    extern FontChar fontData[128];

    extern Texture* fontTexture;

    //fixed size textBuffer to save memory
    extern char textBuffer[64];

    void loadFont(const char* fntPath, const char* texturePath);

    void drawHealthBar(float x, float y, float w, float h, int health, int maxHealth);
    void drawButton(float x, float y, float w, float h, const char* text, uint32_t color);
}
