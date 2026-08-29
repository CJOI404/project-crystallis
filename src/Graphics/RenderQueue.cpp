#include "graphics/Pipeline/RenderQueue.h"
#include <algorithm>
#include <cstring>
#include "graphics/AssetManagers/UIRender.h"

RenderQueue g_queue;

// void RenderQueue_Clear(RenderQueue* queue) {
//     queue->count = 0;
// }

void RenderQueue_Clear(RenderQueue* queue) {
    queue->count = 0;
}

void Submit3D(RenderQueue* queue, Mesh* mesh, Texture* tex, ScePspFVector3* pos, ScePspFVector3* rot, GraphicsUtils::Layer layer) {
    if (queue->count < MAX_RENDER_COMMANDS) {
        RenderCommand& cmd = queue->commands[queue->count++];
        cmd.type = RenderType::Mesh3D;

        cmd.mesh3D.mesh = mesh;
        cmd.mesh3D.texture = tex;
        cmd.mesh3D.position = pos;
        cmd.mesh3D.rotation = rot;
        cmd.layer = layer;
    }
}

void Submit2D(RenderQueue* queue, Texture* tex, float x, float y, float w, float h, uint32_t colour, GraphicsUtils::Layer layer){
    if (queue->count < MAX_RENDER_COMMANDS) {
        RenderCommand& cmd = queue->commands[queue->count++];
        cmd.type = RenderType::Sprite2D;

        cmd.sprite2D.texture = tex;
        cmd.sprite2D.x = x;
        cmd.sprite2D.y = y;
        cmd.sprite2D.w = w;
        cmd.sprite2D.h = h;
        // cmd.sprite2D.sprite = sprite;
        cmd.colour = colour;
        cmd.layer = layer;
    }
}


void Submit2D(RenderQueue* queue, Texture* tex, GraphicsUtils::ScreenPos* position, Sprite* sprite, uint32_t colour, GraphicsUtils::Layer layer) {
    if (queue->count < MAX_RENDER_COMMANDS) {
        RenderCommand& cmd = queue->commands[queue->count++];
        cmd.type = RenderType::Sprite2D;

        cmd.sprite2D.texture = tex;
        cmd.sprite2D.position = position;
        cmd.sprite2D.sprite = sprite;
        cmd.colour = colour;
        cmd.layer = layer;
    }
}

void SubmitText(RenderQueue* queue, std::string text, float x, float y, float xScale, float yScale, uint32_t colour, GraphicsUtils::Layer layer) {
    if (queue->count < MAX_RENDER_COMMANDS) {
        RenderCommand& cmd = queue->commands[queue->count++];
        cmd.type = RenderType::TextUI;

        cmd.text = text;
        cmd.x = x;
        cmd.y = y;
        cmd.xScale = xScale;
        cmd.yScale = yScale;
        cmd.colour = colour;
        cmd.layer = layer;

        
    }
}

void SubmitRect(RenderQueue* queue, short x, short y, short w, short h, uint32_t colour, GraphicsUtils::Layer layer){
    if (queue->count < MAX_RENDER_COMMANDS) {
        RenderCommand& cmd = queue->commands[queue->count++];
        cmd.type = RenderType::Rect2D;

        // cmd.rect2D.position = position;
        cmd.rect2D.x = x;
        cmd.rect2D.y = y;
        cmd.rect2D.w = w;
        cmd.rect2D.h = h;
        cmd.colour = colour;
        cmd.layer = layer; 

        // printf("DEBUG: Just wrote x=%d, read back x=%d\n", x, cmd.rect2D.x);
        // printf("DEBUG: Type:%d\n", cmd.type);
        // fflush(stdout);
    }
}

void SubmitVertices(RenderQueue* queue, std::vector<TextureVertex>* vertices, GraphicsUtils::Layer layer){
    if (queue->count < MAX_RENDER_COMMANDS) {
        RenderCommand& cmd = queue->commands[queue->count++];
        cmd.type = RenderType::Vertices2D;
        cmd.vertices = vertices;
    }
}

void RenderPipeline_Flush(RenderQueue* queue) {

    //sort by layer, then by render type
    std::stable_sort(queue->commands, queue->commands + queue->count, [](const RenderCommand& a, const RenderCommand& b) {
        if (a.layer != b.layer) {
            return a.layer < b.layer;
        }
        return static_cast<int>(a.type) < static_cast<int>(b.type);
    });

    for (int i = 0; i < queue->count; i++) {
        RenderCommand& cmd = queue->commands[i];

        RenderType type = cmd.type;
        int currentLayer = cmd.layer;

        // TODO: write better renderer functions so these commands can be batched in 1 draw call
        // Scan ahead to find the full contiguous run of this exact type + layer combination
        // int startIdx = i;
        // int count = 0;
        // while (i < queue->count && queue->commands[i].type == type && queue->commands[i].layer == currentLayer) {
        //     count++;
        //     i++;
        // }

        // Handle blending states per command
        // if (cmd.blendMode == BlendMode::Opaque) {
        //     sceGuDisable(GU_BLEND);
        //     sceGuEnable(GU_DEPTH_TEST);
        // } else {
        //     sceGuEnable(GU_BLEND);
        //     sceGuEnable(GU_DEPTH_TEST);
        //     sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
        // }

        // Branch cleanly based on explicit type check
        switch (cmd.type) {
            case RenderType::Mesh3D:
                    RenderState::set(GU_CULL_FACE, true);
                    RenderState::setDepthState(DepthState::DEPTH_READ_WRITE);
                    RenderState::setBlendMode(BLEND_NONE);
                    Renderer::renderTexturedMesh(cmd.mesh3D.mesh, cmd.mesh3D.texture, cmd.mesh3D.position, cmd.mesh3D.rotation);
                break;

            case RenderType::Sprite2D:
                    RenderState::setDepthState(DepthState::DEPTH_DISABLED);
                    RenderState::setBlendMode(BLEND_ALPHA);
                    Renderer::renderTexture(cmd.sprite2D.texture, cmd.sprite2D.x, cmd.sprite2D.y, cmd.sprite2D.w, cmd.sprite2D.h, cmd.colour);
                break;

            case RenderType::Rect2D:
                RenderState::setDepthState(DepthState::DEPTH_DISABLED);
                RenderState::setBlendMode(BLEND_ALPHA);
                Renderer::drawRect(cmd.rect2D.x, cmd.rect2D.y, cmd.rect2D.w, cmd.rect2D.h, cmd.colour);
                break;

            case RenderType::Vertices2D:
                RenderState::setDepthState(DepthState::DEPTH_DISABLED);
                RenderState::setBlendMode(BLEND_ALPHA);
                Renderer::renderVertices(cmd.vertices);
                break;

            case RenderType::TextUI:
                RenderState::setDepthState(DepthState::DEPTH_DISABLED);
                RenderState::setBlendMode(BLEND_NONE);
                Renderer::drawString(cmd.x, cmd.y, cmd.colour, cmd.xScale, cmd.yScale, cmd.text);
                break;

        }
    }

}