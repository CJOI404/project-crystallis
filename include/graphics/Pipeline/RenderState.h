#pragma once
#include <pspgu.h>
#include <cstdint>
#include "graphics/Assets/Texture.h"

#define VERTEX_STATE_2D (GU_COLOR_8888 | GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D)
#define VERTEX_STATE_3D (GU_TRIANGLES, | GU_TEXTURE_32BITF | GU_NORMAL_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D)

enum BlendMode {
    BLEND_NONE,       // Disables blending (opaque geometry)
    BLEND_ALPHA,      // Standard transparency (SrcAlpha, 1 - SrcAlpha)
    BLEND_ADDITIVE,   // Glowing effects / particles (SrcAlpha + Fixed 1.0)
    BLEND_MULTIPLY,    // Shadows / tinting (DstColor * Zero)
    BLEND_UNKNOWN
};

enum DepthState {
    DEPTH_DISABLED,    // 2D UI, HUD, screen overlays
    DEPTH_READ_WRITE,  // Opaque 3D meshes (Standard)
    DEPTH_READ_ONLY,    // Transparent 3D meshes (Water, glass, particles)
    DEPTH_UNKNOWN
};

enum CullMode {
    CULL_NONE, // 2D Sprites, double-sided planes, foliage
    CULL_CW,   // Clockwise (Standard 3D mesh rendering)
    CULL_CCW,   // Counter-clockwise
    CULL_UNKNOWN
};

enum TextureMode {
    Tex_
};

namespace RenderState {

    void init();
    void setBlendMode(BlendMode mode);
    void setDepthState(DepthState state);
    void setCullMode(CullMode mode);
    void bindTexture(Texture* texture);
    void set(int state, bool enable);
    
    void resetCache();
}


