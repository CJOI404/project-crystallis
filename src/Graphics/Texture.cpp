#include "graphics/Texture.h"
#include <pspgu.h>
#include <GlobalDefs.h>
#include <psputils.h>
#include "graphics/RenderState.h"

void Texture::draw(float x, float y, float width, float height, uint32_t colour = 0xFFFFFFFF){
    if (this == nullptr){
        printf("Cannot draw %s. No texture data found", this);
        fflush(stdout);
        return;
    }

    // sceGuTexMode(GU_PSM_8888, 0, 0, GU_FALSE);
    //GU_TRUE enables swizzled texture reads; WAY faster but textures need to be swizzled on load
    sceGuTexMode(GU_PSM_8888, 0, 0, GU_TRUE);
    sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
    
    // sceGuTexImage(0, this->width, this->height, this->width, this->data);
    RenderState::bindTexture(this);

    //enable transparency
    // sceGuEnable(GU_BLEND);
    RenderState::set(GU_BLEND, true);
    // sceGuDisable(GU_DEPTH_TEST);
    sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

    // sceGuEnable(GU_TEXTURE_2D); 
    RenderState::set(GU_TEXTURE_2D, true);

    TextureVertex* vertices = (TextureVertex*)sceGuGetMemory(2 * sizeof(TextureVertex));

    // Top-Left
    vertices[0].u = 0;
    vertices[0].v = 0;
    vertices[0].x = x; 
    vertices[0].y = y; 
    vertices[0].z = 0;
    vertices[0].colour = colour;

    // Bottom-Right
    vertices[1].u = this->width;
    vertices[1].v = this->height;
    vertices[1].x = x + width; 
    vertices[1].y = y + height; 
    vertices[1].z = 0;
    vertices[1].colour = colour;

    
    sceGuDrawArray(GU_SPRITES, GU_COLOR_8888 | GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);
    

    // sceGuDisable(GU_TEXTURE_2D);
    RenderState::set(GU_TEXTURE_2D, false);
}

void Texture::draw(float u1, float v1, float u2, float v2, float x, float y, float width, float height, uint32_t colour = 0xFFFFFFFF){
    if (this == nullptr){
        printf("Cannot draw %s. No texture data found", this);
        fflush(stdout);
        return;
    }

    // sceGuTexMode(GU_PSM_8888, 0, 0, GU_FALSE);
    //GU_TRUE enables swizzled texture reads; WAY faster but textures need to be swizzled on load
    sceGuTexMode(GU_PSM_8888, 0, 0, GU_TRUE);
    sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
    
    // sceGuTexImage(0, this->width, this->height, this->width, this->data);
    RenderState::bindTexture(this);

    //enable transparency
    // sceGuEnable(GU_BLEND);
    RenderState::set(GU_BLEND, true);
    sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

    // sceGuEnable(GU_TEXTURE_2D); 
    RenderState::set(GU_TEXTURE_2D, true);

    TextureVertex* vertices = (TextureVertex*)sceGuGetMemory(2 * sizeof(TextureVertex));

    // Top-Left
    vertices[0].u = u1;
    vertices[0].v = v1;
    vertices[0].x = x; 
    vertices[0].y = y; 
    vertices[0].z = 0;
    vertices[0].colour = colour;

    // Bottom-Right
    vertices[1].u = u2;
    vertices[1].v = v2;
    vertices[1].x = x + width; 
    vertices[1].y = y + height; 
    vertices[1].z = 0;
    vertices[1].colour = colour;

    
    sceGuDrawArray(GU_SPRITES, GU_COLOR_8888 | GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);
    

    sceGuDisable(GU_TEXTURE_2D);
}
