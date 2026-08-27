#include "graphics/RenderState.h"

namespace RenderState {
    static BlendMode currentBlendMode = BLEND_NONE;

    void init() {
        currentBlendMode = BLEND_NONE;
        sceGuDisable(GU_BLEND);
    }

    void setBlendMode(BlendMode mode) {
        // Skip redundant GPU state updates!
        if (currentBlendMode == mode) return;

        currentBlendMode = mode;

        switch (mode) {
            case BLEND_NONE:
                sceGuDisable(GU_BLEND);
                break;

            case BLEND_ALPHA:
                sceGuEnable(GU_BLEND);
                sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
                break;

            case BLEND_ADDITIVE:
                sceGuEnable(GU_BLEND);
                sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_FIX, 0, 0xFFFFFFFF);
                break;

            case BLEND_MULTIPLY:
                sceGuEnable(GU_BLEND);
                sceGuBlendFunc(GU_ADD, GU_DST_COLOR, GU_ZERO, 0, 0);
                break;
        }
    }
}