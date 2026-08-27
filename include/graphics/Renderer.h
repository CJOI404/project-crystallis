#pragma once
#include "graphics/Mesh.h"
#include <pspgum.h>
#include <pspgu.h>
#include <psputils.h>

namespace Renderer {

    void renderMesh(Mesh* mesh, ScePspFVector3* pos, ScePspFVector3* rot) {
    if (!mesh || !mesh->vertices || mesh->vertexCount == 0) return;

    // 1. Set render state
    sceGuDisable(GU_CULL_FACE);
    sceGuDisable(GU_TEXTURE_2D);
    sceGuDisable(GU_LIGHTING);

    // 2. Set solid render color (Green)
    sceGuColor(0xFFFFFFFF);

    // 3. Apply Model Matrix Transforms (Translate & Rotate)
    sceGumMatrixMode(GU_MODEL);
    sceGumPushMatrix();
    sceGumLoadIdentity();

    // Apply parameters passed into renderMesh
    sceGumTranslate(pos);
    sceGumRotateX(rot->x);
    sceGumRotateY(rot->y);
    sceGumRotateZ(rot->z);

    // Push transformations to PSP hardware registers
    sceGumUpdateMatrix();

    // 4. Invalidate D-cache for vertex DMA transfer (correct size!)
    sceKernelDcacheWritebackInvalidateRange(mesh->vertices, sizeof(Vertex3D) * mesh->vertexCount);

    // 5. Draw using 32-bit float layout matching your Vertex3D struct
    sceGumDrawArray(
        GU_TRIANGLES,
        GU_TEXTURE_32BITF | GU_NORMAL_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D,
        mesh->vertexCount,
        0,
        mesh->vertices
    );

    // 6. Clean up matrix stack
    sceGumPopMatrix();
}

//     void renderMesh(Mesh* mesh, ScePspFVector3* pos, ScePspFVector3* rot) {
//         // 1. Force state: No textures, no lighting, no culling
//         sceGuDisable(GU_TEXTURE_2D);
//         sceGuDisable(GU_LIGHTING);
//         sceGuDisable(GU_CULL_FACE);

//         // 2. Identity transforms (Ignore camera & pos for a second to rule out positioning)
//         sceGumMatrixMode(GU_VIEW);
//         sceGumLoadIdentity();
        
//         sceGumMatrixMode(GU_MODEL);
//         sceGumLoadIdentity();
//         sceGumUpdateMatrix();

//         // 3. Flush CPU cache so GPU sees your mesh buffer
//         sceKernelDcacheWritebackInvalidateRange(mesh->vertices, sizeof(Vertex) * mesh->vertexCount);

//         // 4. Force color to pure green (0xFF00FF00) for uncolored vertices
//         sceGuColor(0xFF00FF00);


//             if (mesh != NULL && mesh->vertices != NULL && mesh->vertexCount > 0) {
//                 printf("--- MESH DEBUG (Count: %d) ---\n", mesh->vertexCount);
                
//                 // Cast to your Vertex struct pointer
//                 Vertex3D* v = (Vertex3D*)mesh->vertices;
                
//                 for (int i = 0; i < 3 && i < mesh->vertexCount; i++) {
//                     // Print raw integer values
//                     printf("V[%d] int short: x=%d, y=%d, z=%d | u=%u, v=%u\n", 
//                             i, v[i].x, v[i].y, v[i].z, v[i].u, v[i].v);
                    
//                     // Print bitcast as float (to see if floats were stored in shorts)
//                     float* fx = (float*)&v[i].x;
//                     printf("V[%d] raw float reinterpret: %f\n", i, *fx);
//                 }
//                 printf("-------------------------------\n");
//                 fflush(stdout);
//             } else {
//                 printf("ERROR: mesh or mesh->vertices is NULL / vertexCount is 0!\n");
//                 fflush(stdout);
//             }


//         // 5. Draw raw geometry using YOUR vertex struct format
//         sceGumDrawArray(
//             GU_TRIANGLES,
//             GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_3D, // Matching your 16-bit Vertex struct
//             mesh->vertexCount,
//             0,
//             mesh->vertices
//     );
// }

    // void renderMesh(Mesh* mesh, ScePspFVector3* pos, ScePspFVector3* rot) {
    //     // 1. Force states to ensure nothing is being culled or lit out of view
    //     sceGuDisable(GU_CULL_FACE);
    //     sceGuDisable(GU_TEXTURE_2D);
    //     sceGuDisable(GU_LIGHTING);
    //     sceGuEnable(GU_DEPTH_TEST);
    //     sceGuDepthFunc(GU_GEQUAL);
    //     sceGuDepthMask(GU_FALSE);

    //     // 2. Clear Model & View matrices back to identity
    //     sceGumMatrixMode(GU_VIEW);
    //     sceGumLoadIdentity();
        
    //     sceGumMatrixMode(GU_MODEL);
    //     sceGumLoadIdentity();
    //     sceGumUpdateMatrix();

    //     // 3. Simple red 3D triangle sitting directly in front of camera (Z = -2.0f)
    //     typedef struct __attribute__((packed, aligned(4))) {
    //         unsigned int color;
    //         float x, y, z;
    //     } SimpleVertex;

    //     static SimpleVertex __attribute__((aligned(16))) testTri[3] = {
    //         { 0xFF0000FF,  0.0f,  0.5f, -2.0f }, // Top (Red)
    //         { 0xFF0000FF, -0.5f, -0.5f, -2.0f }, // Bottom Left
    //         { 0xFF0000FF,  0.5f, -0.5f, -2.0f }  // Bottom Right
    //     };

    //     // Flush CPU D-cache so the GPU DMA sees the array
    //     sceKernelDcacheWritebackInvalidateRange(testTri, sizeof(testTri));

    //     // 4. Draw using Color + Position
    //     sceGumDrawArray(
    //         GU_TRIANGLES,
    //         GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D,
    //         3,
    //         0,
    //         testTri
    //     );


    //     sceKernelDcacheWritebackInvalidateRange(mesh->vertices, sizeof(Vertex3D) * mesh->vertexCount);


    //     //Set material colors (Since no texture is bound)
    //     sceGuModelColor(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);



    //     sceGuDisable(GU_CULL_FACE);

    //     sceGumMatrixMode(GU_MODEL);
    //     sceGumPushMatrix();
        
    //     // Set position and rotation transforms
    //     sceGumTranslate(pos);
    //     sceGumRotateX(rot->x);
    //     sceGumRotateY(rot->y);
    //     sceGumRotateZ(rot->z);

    //     // Define matrix state to Graphics Engine
    //     sceGumUpdateMatrix();

    //     // Draw triangles directly from vertex buffer
    //     sceGumDrawArray(
    //         GU_TRIANGLES,                               // Primitive type
    //         GU_TEXTURE_32BITF | GU_NORMAL_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, // Vertex format flags
    //         mesh->vertexCount,                          // Number of vertices
    //         0,                                          // Index buffer (0 = linear)
    //         mesh->vertices                              // Pointer to data
    //     );

    //     sceGumPopMatrix();

    //     printf("Rendering model");
    //     fflush(stdout);

    // }

}