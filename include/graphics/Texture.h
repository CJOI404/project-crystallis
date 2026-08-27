#pragma once
#include <cstdint>

typedef struct
{
    int size;
    uint32_t* data;

    void draw(float x, float y, float w, float h);

} Texture;