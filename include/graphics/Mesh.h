#pragma once

#include <pspgu.h>
#include <pspgum.h>
#include <malloc.h>
#include <stdio.h>
#include "GlobalDefs.h"

struct __attribute__((packed, aligned(4))) Vertex3D {
    float u, v;       // 1. Texture UVs (GU_TEXTURE_32BITF)
    float nx, ny, nz; // 2. Normals     (GU_NORMAL_32BITF)
    float x, y, z;    // 3. Positions   (GU_VERTEX_32BITF)
};

typedef struct {
    Vertex3D* vertices;
    int vertexCount;
} Mesh;