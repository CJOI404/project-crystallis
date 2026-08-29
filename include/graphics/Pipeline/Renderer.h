#pragma once
#include "graphics/Assets/Mesh.h"
#include <pspgum.h>
#include <pspgu.h>
#include <psputils.h>
#include "graphics/Pipeline/RenderState.h"
#include "graphics/GraphicsUtils.h"
#include <vector>
#include <cstring>
#include "graphics/AssetManagers/UIRender.h"

namespace Renderer {

    inline void renderMesh(Mesh* mesh, ScePspFVector3* pos, ScePspFVector3* rot) {
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

    inline void renderTexturedMesh(Mesh* mesh, Texture* texture, ScePspFVector3* pos, ScePspFVector3* rot) {
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

    inline void renderVertices(std::vector<TextureVertex>* vertices){
        RenderState::set(GU_TEXTURE_2D, true);
        // sceGuTexMode(GU_PSM_8888, 0, 0, GU_TRUE);
        // sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
        // for (int i = 0; i < vertices->size(); i++){
        //     printf("%d: x = %f, y = %f, z = %f\n", i, (*vertices)[i].x, (*vertices)[i].y, (*vertices)[i].z);
        //     fflush(stdout);
        // }

        if (vertices->empty()) return;

        // Allocate directly from the GE display list buffer
        TextureVertex* nvertices = (TextureVertex*)sceGuGetMemory(vertices->size() * sizeof(TextureVertex));
        
        // Copy your batched vertex data into the GE scratchpad memory
        std::memcpy(nvertices, vertices->data(), vertices->size() * sizeof(TextureVertex));
        sceGuDrawArray(GU_SPRITES, GU_COLOR_8888 | GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, vertices->size(), 0, vertices->data());
    }

    inline void renderTexture(Texture* tex, float x, float y, float width, float height, uint32_t colour = 0xFFFFFFFF){
        if (tex == nullptr){
            printf("Cannot draw %s. No texture data found", tex);
            fflush(stdout);
            return;
        }

        // sceGuTexMode(GU_PSM_8888, 0, 0, GU_FALSE);
        //GU_TRUE enables swizzled texture reads; WAY faster but textures need to be swizzled on load
        sceGuTexMode(GU_PSM_8888, 0, 0, GU_TRUE);
        sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
        
        // sceGuTexImage(0, this->width, this->height, this->width, this->data);
        RenderState::bindTexture(tex);

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
        vertices[1].u = tex->width;
        vertices[1].v = tex->height;
        vertices[1].x = x + width; 
        vertices[1].y = y + height; 
        vertices[1].z = 0;
        vertices[1].colour = colour;

        
        sceGuDrawArray(GU_SPRITES, GU_COLOR_8888 | GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);
        

        // sceGuDisable(GU_TEXTURE_2D);
        RenderState::set(GU_TEXTURE_2D, false);
    }

    inline void renderTexture(Texture* tex, float u1, float v1, float u2, float v2, float x, float y, float width, float height, uint32_t colour = 0xFFFFFFFF){
        if (tex == nullptr){
        printf("Cannot draw %s. No texture data found", tex);
        fflush(stdout);
        return;
        }

        // sceGuTexMode(GU_PSM_8888, 0, 0, GU_FALSE);
        //GU_TRUE enables swizzled texture reads; WAY faster but textures need to be swizzled on load
        sceGuTexMode(GU_PSM_8888, 0, 0, GU_TRUE);
        sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
        
        // sceGuTexImage(0, this->width, this->height, this->width, this->data);
        RenderState::bindTexture(tex);

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
        
        // sceGuDisable(GU_TEXTURE_2D);
        RenderState::set(GU_TEXTURE_2D, false);
    }

    inline void renderSprite(Sprite* sprite, GraphicsUtils::ScreenPos* position, float width, float height, uint32_t colour = 0xFFFFFFFF){
    
        if (sprite == nullptr || sprite->texture == nullptr){
            printf("Cannot draw %s. No sprite/texture data found", sprite);
            fflush(stdout);
            return;
        }

        renderTexture(sprite->texture, sprite->u0, sprite->v0, sprite->u0+width, sprite->v0+height, position->x, position->y, width, height, colour);
    //    te->draw(u0, v0, u0+this->width, v0+this->height, x, y, width, height, colour);
    }

    inline void drawString(int x, int y, uint32_t color, float xScale, float yScale, std::string text) {

        int currentX = x;

        sceGuTexMode(GU_PSM_8888, 0, 0, GU_TRUE);
        sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
        // sceGuTexImage(0, textWidth, textHeight, textWidth, fontTexture->data);
        RenderState::bindTexture(UI::fontTexture);

        //enable transparency
        // sceGuEnable(GU_BLEND);
        RenderState::set(GU_BLEND, true);
        sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

        // sceGuEnable(GU_TEXTURE_2D); 
        RenderState::set(GU_TEXTURE_2D, true);

        TextureVertex* vertices = (TextureVertex*)sceGuGetMemory(text.length() * 2 * sizeof(TextureVertex));

        int idx = 0;

        for (int i = 0; text[i] != '\0'; i++){

            int j = text[i];

            // Top-Left
            vertices[idx].u = UI::fontData[j].x; 
            vertices[idx].v = UI::fontData[j].y;
            vertices[idx].x = currentX + (UI::fontData[j].xoffset * xScale); 
            vertices[idx].y = y + (UI::fontData[j].yoffset * yScale); 
            vertices[idx].z = 0;
            vertices[idx].colour = color;

            // Bottom-Right
            vertices[idx+1].u = UI::fontData[j].x + UI::fontData[j].width; 
            vertices[idx+1].v = UI::fontData[j].y + UI::fontData[j].height;
            vertices[idx+1].x = currentX + ((UI::fontData[j].width + UI::fontData[j].xoffset) * xScale); 
            vertices[idx+1].y = y + ((UI::fontData[j].height + UI::fontData[j].yoffset) * yScale); 
            vertices[idx+1].z = 0;
            vertices[idx+1].colour = color;

            idx += 2;
            currentX += UI::fontData[j].xadvance * xScale;
        }

        sceGuDrawArray(GU_SPRITES, GU_COLOR_8888 | GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, text.length() * 2, 0, vertices);

        // sceGuDisable(GU_TEXTURE_2D);
        RenderState::set(GU_TEXTURE_2D, false);
        
    }

    inline void drawRect(short x, short y, short w, short h, unsigned int colour) {
        RenderState::set(GU_TEXTURE_2D, false);

        Vertex* vertices = (Vertex*)sceGuGetMemory(2 * sizeof(Vertex));

        vertices[0].x = x;
        vertices[0].y = y;
        vertices[0].z = 0;

        vertices[1].x = x + w;
        vertices[1].y = y + h;
        vertices[1].z = 0;

        sceGuColor(colour);

        sceGuDrawArray(GU_SPRITES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 2, 0, vertices);
    }
}