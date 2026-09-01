#include "graphics/Pipeline/RenderQueue.h"
#include <algorithm>
#include <cstring>
#include "graphics/AssetManagers/UIRender.h"

RenderQueue g_queue;
TextureVertex* vertBuffer2D;
Vertex3D* vertBuffer3D;

void RenderQueue_Clear(RenderQueue* queue) {
    for (int i = 0; i < queue->count; i++){
        if (queue->commands[i].type == RenderType::TextUI && queue->commands[i].text2D.text){
            free((void *) queue->commands[i].text2D.text);
            queue->commands[i].text2D.text = nullptr;
        }
    }
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
        cmd.type = RenderType::Texture2D;

        cmd.texture2D.texture = tex;
        cmd.texture2D.x = x;
        cmd.texture2D.y = y;
        cmd.texture2D.w = w;
        cmd.texture2D.h = h;
        cmd.colour = colour;
        cmd.layer = layer;
    }
}


void Submit2D(RenderQueue* queue, Sprite* sprite, float x, float y, float w, float h, uint32_t colour, GraphicsUtils::Layer layer) {
    if (queue->count < MAX_RENDER_COMMANDS) {
        RenderCommand& cmd = queue->commands[queue->count++];
        cmd.type = RenderType::Sprite2D;

        cmd.sprite2D.sprite = sprite;
        cmd.sprite2D.texture = sprite->texture;
        cmd.sprite2D.x = x;
        cmd.sprite2D.y = y;
        cmd.sprite2D.w = w;
        cmd.sprite2D.h = h;
        cmd.colour = colour;
        cmd.layer = layer;
    }
}

void SubmitText(RenderQueue* queue, const char* text, float x, float y, float xScale, float yScale, uint32_t colour, GraphicsUtils::Layer layer) {
    if (queue->count < MAX_RENDER_COMMANDS) {
        RenderCommand& cmd = queue->commands[queue->count++];
        cmd.type = RenderType::TextUI;

        cmd.text2D.text = strdup(text);
        cmd.text2D.x = x;
        cmd.text2D.y = y;
        cmd.text2D.xScale = xScale;
        cmd.text2D.yScale = yScale;
        cmd.colour = colour;
        cmd.layer = layer;

        
    }
}

void SubmitRect(RenderQueue* queue, short x, short y, short w, short h, uint32_t colour, GraphicsUtils::Layer layer){
    if (queue->count < MAX_RENDER_COMMANDS) {
        RenderCommand& cmd = queue->commands[queue->count++];
        cmd.type = RenderType::Rect2D;

        cmd.rect2D.x = x;
        cmd.rect2D.y = y;
        cmd.rect2D.w = w;
        cmd.rect2D.h = h;
        cmd.colour = colour;
        cmd.layer = layer; 
    }
}

void RenderPipeline_Flush(RenderQueue* queue) {

    float startTime = clock();

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

        /*
        How do I batch?
        save full render state (depth, blend, cull, texture binding, etc)
        while current state is the same as last state:
            append vertex data to list
        
        if current state is different from last state
            call render function with vertex array

        
        
        
        */


        // Branch cleanly based on explicit type check
        switch (cmd.type) {
            case RenderType::Mesh3D:
                    RenderState::set(GU_CULL_FACE, true);
                    RenderState::setDepthState(DepthState::DEPTH_READ_WRITE);
                    RenderState::setBlendMode(BLEND_NONE);
                    Renderer::renderTexturedMesh(cmd.mesh3D.mesh, cmd.mesh3D.texture, cmd.mesh3D.position, cmd.mesh3D.rotation);
                break;

            case RenderType::Texture2D:
                    RenderState::setDepthState(DepthState::DEPTH_DISABLED);
                    RenderState::setBlendMode(BLEND_ALPHA);
                    Renderer::renderTexture(cmd.sprite2D.texture, cmd.sprite2D.x, cmd.sprite2D.y, cmd.sprite2D.w, cmd.sprite2D.h, cmd.colour);
                break;                    

            case RenderType::Sprite2D:
                    RenderState::setDepthState(DepthState::DEPTH_DISABLED);
                    RenderState::setBlendMode(BLEND_ALPHA);
                    Renderer::renderSprite(cmd.sprite2D.sprite, cmd.sprite2D.x, cmd.sprite2D.y, cmd.sprite2D.w, cmd.sprite2D.h, cmd.colour);
                break;

            case RenderType::Rect2D:
                RenderState::setDepthState(DepthState::DEPTH_DISABLED);
                RenderState::setBlendMode(BLEND_ALPHA);
                Renderer::drawRect(cmd.rect2D.x, cmd.rect2D.y, cmd.rect2D.w, cmd.rect2D.h, cmd.colour);
                break;

            case RenderType::TextUI:
                RenderState::setDepthState(DepthState::DEPTH_DISABLED);
                RenderState::setBlendMode(BLEND_NONE);
                Renderer::drawString(cmd.text2D.x, cmd.text2D.y, cmd.colour, cmd.text2D.xScale, cmd.text2D.yScale, cmd.text2D.text);
                break;

        }
    }

    float renderTime = (clock() - startTime) / CLOCKS_PER_SEC;
    // printf("RENDER QUEUE FLUSH TIME:%.5f\n", (renderTime * 1000));
    // fflush(stdout);

}