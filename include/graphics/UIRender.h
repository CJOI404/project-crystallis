#pragma once
#include <pspgu.h>
#include <stdint.h>
#include <vector>
#include <string>
#include "graphics/TextureManager.h"

namespace UI {
    struct FontChar {
        int id;
        int x, y;
        int width, height;
        int xoffset, yoffset;
        int xadvance, page, chnl;
    };

    // extern FontChar fontData[256];
    //bitmap font doesn't go past id 128 so this should help save mem
    extern FontChar fontData[128];

    //fixed size textBuffer to save memory
    extern char textBuffer[64];

    void loadFont(const char* fntPath, const char* texturePath);
    void loadIcons(int size, int num, const char* texturePath);

    void loadSprite(const char*texturePath);

    void drawSprite(int x, int y, int u, int v, uint32_t color, float xScale, float yScale);

    void drawString(int x, int y, uint32_t color, float scale, float yScale, const std::string& text);
    void drawRect(float x, float y, float w, float h, unsigned int colour);
    void drawTri(float x1, float y1, float x2, float y2, float x3, float y3, unsigned int color);

    void drawHealthBar(float x, float y, float w, float h, int health, int maxHealth);
    void drawButton(float x, float y, float w, float h, std::string text, uint32_t color);
}
