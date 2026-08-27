#pragma once
#include <cstdint>

namespace GraphicsUtils {

    // Converts RGBA (0-255 colors, 0.0f-1.0f float alpha) to PSP 0xAABBGGRR format
    inline uint32_t ColourRGBA(uint8_t r, uint8_t g, uint8_t b, float alpha = 1.0f) {
        uint8_t a = (uint8_t)(alpha * 255.0f);
        return ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)g << 8) | (uint32_t)r;
    }

    

}