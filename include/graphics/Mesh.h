#pragma once

#include <pspgu.h>
#include <pspgum.h>
#include <malloc.h>
#include <stdio.h>
#include "GlobalDefs.h"

typedef struct {
    Vertex* vertices;
    int vertexCount;
} Mesh;