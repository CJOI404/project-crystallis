#pragma once
#include <cstdint>

// typedef struct
// {
//     int size;
//     uint32_t* data;

//     void draw(float x, float y, float w, float h, uint32_t colour);
//     void draw(float u1, float v1, float u2, float v2, float x, float y, float width, float height, uint32_t colour);

// } Texture;

typedef struct
{
    int width;
    int height;
    int format;
    void* data;
    
    void draw(float x, float y, float w, float h, uint32_t colour);
    void draw(float u1, float v1, float u2, float v2, float x, float y, float width, float height, uint32_t colour);

} Texture;


typedef struct
{
    float u, v;
    unsigned int colour;
    float x, y, z;
} TextureVertex;