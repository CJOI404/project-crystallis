#pragma once
#include "graphics/Mesh.h"
#include <pspgum.h>

namespace Renderer {

    void renderMesh(Mesh* mesh, ScePspFVector3* pos, ScePspFVector3* rot) {

        sceGuDisable(GU_CULL_FACE);

        sceGuEnable(GU_LIGHTING);
        sceGumMatrixMode(GU_MODEL);
        sceGumPushMatrix();
        
        // Set position and rotation transforms
        sceGumTranslate(pos);
        sceGumRotateX(rot->x);
        sceGumRotateY(rot->y);
        sceGumRotateZ(rot->z);

        // Define matrix state to Graphics Engine
        sceGumUpdateMatrix();

        // Draw triangles directly from vertex buffer
        sceGuDrawArray(
            GU_TRIANGLES,                               // Primitive type
            GU_TEXTURE_32BITF | GU_NORMAL_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, // Vertex format flags
            mesh->vertexCount,                          // Number of vertices
            0,                                          // Index buffer (0 = linear)
            mesh->vertices                              // Pointer to data
        );

        sceGumPopMatrix();

        printf("Rendering model");
        fflush(stdout);

    }

}