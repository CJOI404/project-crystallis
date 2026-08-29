#include "graphics/Assets/Sprite.h"
#include <cstdio>
#include <pspgu.h>

void Sprite::draw(float x, float y, float width, float height, uint32_t colour = 0xFFFFFFFF){
    
    if (texture == nullptr || texture->data == nullptr){
        printf("Cannot draw %s. No texture data found", this);
        fflush(stdout);
        return;
    }

   texture->draw(u0, v0, u0+this->width, v0+this->height, x, y, width, height, colour);
}