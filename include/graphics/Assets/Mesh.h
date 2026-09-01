#pragma once

#include <pspgu.h>
#include <pspgum.h>
#include <malloc.h>
#include <stdio.h>
#include "GlobalDefs.h"

struct __attribute__((packed, aligned(4))) Vertex3D {
    float u, v;       //(GU_TEXTURE_32BITF)
    float nx, ny, nz; //(GU_NORMAL_32BITF)
    float x, y, z;    //(GU_VERTEX_32BITF)
};

// struct __attribute__((packed, aligned(4))) Vertex3D_16 {
struct Vertex3D_16 {
    int16_t u, v;       //(GU_TEXTURE_16BIT) ( 4 bytes )
    int8_t nx, ny, nz; //(GU_NORMAL_8BIT) ( 3 bytes )
    int16_t x, y, z;    //(GU_VERTEX_16BIT) ( 6 bytes )
};

typedef struct {
    Vertex3D* vertices;
    int vertexCount;
    int refCount;
} Mesh;