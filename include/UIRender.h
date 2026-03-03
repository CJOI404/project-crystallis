#pragma once
#include <pspgu.h>
#include <stdint.h>
#include <vector>
#include <string>

namespace UI {
    struct FontChar {
        int id;
        int x, y;
        int width, height;
        int xoffset, yoffset;
        int xadvance, page, chnl;
    };

    extern FontChar fontData[256];

    void loadFont(const char* fntPath, const char* texturePath);
    void drawString(int x, int y, uint32_t color, float scale, float yScale, std::string text);
    void drawRect(float x, float y, float w, float h, unsigned int colour);
}
