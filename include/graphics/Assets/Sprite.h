#pragma once
#include "graphics/Assets/Texture.h"

typedef struct {
    Texture* texture;
    int u0, u1, v0, v1;
    int width;
    int height;  

    int refCount;

} Sprite;
