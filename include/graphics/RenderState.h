#pragma once
#include <pspgu.h>
#include <cstdint>

enum BlendMode {
    BLEND_NONE,       // Disables blending (opaque geometry)
    BLEND_ALPHA,      // Standard transparency (SrcAlpha, 1 - SrcAlpha)
    BLEND_ADDITIVE,   // Glowing effects / particles (SrcAlpha + Fixed 1.0)
    BLEND_MULTIPLY    // Shadows / tinting (DstColor * Zero)
};

namespace RenderState {
    void init();
    void setBlendMode(BlendMode mode);
}