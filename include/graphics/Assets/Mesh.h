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

typedef struct {
    Vertex3D* vertices;
    int vertexCount;
    int refCount;
} Mesh;