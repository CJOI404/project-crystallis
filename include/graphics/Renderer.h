#pragma once
#include "graphics/Mesh.h"
#include <pspgum.h>
#include <pspgu.h>
#include <psputils.h>
#include "graphics/RenderState.h"

namespace Renderer {

    void renderMesh(Mesh* mesh, ScePspFVector3* pos, ScePspFVector3* rot) {
        if (!mesh || !mesh->vertices || mesh->vertexCount == 0) return;

        // 1. Set render state
        // sceGuDisable(GU_CULL_FACE);
        // sceGuDisable(GU_TEXTURE_2D);
        // sceGuDisable(GU_LIGHTING);
        RenderState::set(GU_CULL_FACE, false);
        RenderState::set(GU_TEXTURE_2D, false);
        RenderState::set(GU_LIGHTING, false);

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

    void renderTexturedMesh(Mesh* mesh, Texture* texture, ScePspFVector3* pos, ScePspFVector3* rot) {
        if (!mesh || !mesh->vertices || mesh->vertexCount == 0) return;

        //Enable 2D Texturing in the GU State
        // sceGuEnable(GU_TEXTURE_2D);
        RenderState::set(GU_TEXTURE_2D, true);

        //Bind texture details
        sceGuTexMode(GU_PSM_8888, 0, 0, GU_TRUE); 
        sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
        sceGuColor(0xFFFFFFFF);
        // sceGuTexImage(0, texture->width, texture->height, texture->width, texture->data);
        RenderState::bindTexture(texture);

        //Apply Model Matrix Transforms (Translate & Rotate)
        sceGumMatrixMode(GU_MODEL);

        /*  
            push and pop is needed for matrix transformation chains;
            that is, if i have a child object (or a skeleton or smth) 
            and I want to push a new matrix transform, then return to the parent transform
            For single object rendering it is unnecessary
        */

        // sceGumPushMatrix();

        sceGumLoadIdentity();

        //Apply parameters passed into renderMesh
        sceGumTranslate(pos);
        sceGumRotateX(rot->x);
        sceGumRotateY(rot->y);
        sceGumRotateZ(rot->z);

        //Push transformations to PSP hardware registers
        sceGumUpdateMatrix();

        //Draw using 32-bit float layout
        sceGumDrawArray(
            GU_TRIANGLES,
            GU_TEXTURE_32BITF | GU_NORMAL_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D,
            mesh->vertexCount,
            0,
            mesh->vertices
        );

        // Only needed if pushing matrix
        // sceGumPopMatrix();
    }

}