#pragma once
#include "graphics/Assets/Texture.h"

typedef struct {
    Texture* texture;
    int u0, u1, v0, v1;
    int width;
    int height;  

    int refCount;

    void draw(float x, float y, float width, float height, uint32_t colour);

} Sprite;
