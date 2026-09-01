#pragma once
#include "graphics/Assets/Texture.h"
#include "graphics/Assets/Mesh.h"
#include "graphics/GraphicsUtils.h"
#include "graphics/Assets/Sprite.h"
#include "graphics/Pipeline/Renderer.h"
#include <variant>

/**
 * Define render types
 * Order here determines sub-layer sorting
 */
enum RenderType { 
    Mesh3D, 
    Texture2D,
    Sprite2D,
    Rect2D,
    TextUI,
    Vertices2D
};

struct RenderCommand {
    RenderType type;
    GraphicsUtils::Layer layer;
    uint32_t colour;

    union {
        struct {
            Mesh* mesh;
            Texture* texture;
            ScePspFVector3* position;
            ScePspFVector3* rotation;
        } mesh3D;

        struct {
            float x, y, w, h;
            Texture* texture;
        } texture2D;

        struct {
            float x, y, w, h;
            Texture* texture;
            Sprite* sprite;
        } sprite2D;

        struct {
            short x, y, w, h;
            // float width, height;
        } rect2D;

        struct {
            float x, y, xScale, yScale;
            const char* text;
        } text2D;
    };

};

#define MAX_RENDER_COMMANDS 512

struct RenderQueue {
    RenderCommand commands[MAX_RENDER_COMMANDS];
    RenderType lastType;
    int count = 0;
};

extern RenderQueue g_queue;

extern TextureVertex* vertBuffer2D;
extern Vertex3D* vertBuffer3D;

extern 

void RenderQueue_Clear(RenderQueue* queue);

void Submit3D(RenderQueue* queue, Mesh* mesh, Texture* tex, ScePspFVector3* pos, ScePspFVector3* rot, GraphicsUtils::Layer layer);
void Submit2D(RenderQueue* queue, Sprite* sprite, float x, float y, float w, float h, uint32_t colour, GraphicsUtils::Layer layer);
void Submit2D(RenderQueue* queue, Texture* tex, float x, float y, float w, float h, uint32_t colour, GraphicsUtils::Layer layer);
void SubmitRect(RenderQueue* queue, short x, short y, short w, short h, uint32_t colour, GraphicsUtils::Layer layer);
void SubmitText(RenderQueue* queue, const char* text, float x, float y, float xScale, float yScale, uint32_t colour, GraphicsUtils::Layer layer);

// void SubmitVertices(RenderQueue* queue, std::vector<TextureVertex>* vertices, GraphicsUtils::Layer layer);

void RenderPipeline_Flush(RenderQueue* queue);
