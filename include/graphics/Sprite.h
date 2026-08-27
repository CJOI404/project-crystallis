#pragma once
#include "graphics/Texture.h"

typedef struct {
    Texture* texture;
    int u0, u1, v0, v1;
    int width;
    int height;  

    void draw(float x, float y, float width, float height, uint32_t colour);

} Sprite;
