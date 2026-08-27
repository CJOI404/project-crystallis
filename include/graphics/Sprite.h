#pragma once
#include "graphics/Texture.h"

typedef struct {
    Texture* texture;
    int u;
    int v;
    int width;
    int height;  

    void draw();

} Sprite;
