#include "graphics/Texture.h"
#include <pspgu.h>
#include <GlobalDefs.h>
#include <psputils.h>

void Texture::draw(float x, float y, float width, float height){
    if (this == nullptr){
        printf("Cannot draw %s. No texture data found", this);
        fflush(stdout);
        return;
    }

    // sceGuTexMode(GU_PSM_8888, 0, 0, GU_FALSE);
    //GU_TRUE enables swizzled texture reads; WAY faster but textures need to be swizzled on load
    sceGuTexMode(GU_PSM_8888, 0, 0, GU_TRUE);
    sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
    sceGuTexImage(0, this->size, this->size, this->size, this->data);

    //enable transparency
    sceGuEnable(GU_BLEND);
    sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

    sceGuEnable(GU_TEXTURE_2D); 

    TextureVertex* vertices = (TextureVertex*)sceGuGetMemory(2 * sizeof(TextureVertex));

    // Top-Left
    vertices[0].u = 0;
    vertices[0].v = 0;
    vertices[0].x = x; 
    vertices[0].y = y; 
    vertices[0].z = 0;

    // Bottom-Right
    vertices[1].u = this->size;
    vertices[1].v = this->size;
    vertices[1].x = x + width; 
    vertices[1].y = y + height; 
    vertices[1].z = 0;

    
    sceGuDrawArray(GU_SPRITES, GU_COLOR_8888 | GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);
    

    sceGuDisable(GU_TEXTURE_2D);
}
