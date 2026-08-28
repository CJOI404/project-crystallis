#pragma once
#include <pspgu.h>
#include <cstdint>
#include "Texture.h"

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
    
    // Call at the very beginning of each frame loop
    void resetCache();
}


