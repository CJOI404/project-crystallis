#include "graphics/RenderState.h"
#include <cstdio>
#include <cstring>

namespace RenderState {
    static BlendMode currentBlendMode = BLEND_UNKNOWN;
    static DepthState currentDepthMode = DEPTH_UNKNOWN;
    static CullMode currentCullMode = CULL_UNKNOWN;
    static Texture* currentTexture = nullptr;

    bool flags[64];

    void init() {
        resetCache();
        sceGuDisable(GU_BLEND);
    }

    void resetCache() {
        currentBlendMode   = BLEND_NONE;
        currentDepthMode   = DEPTH_DISABLED;
        currentCullMode    = CULL_NONE;
        currentTexture = nullptr;
        memset(flags, 0, sizeof(flags));
    }

    void setBlendMode(BlendMode mode) {
        // Skip redundant GPU state updates!
        if (currentBlendMode == mode) return;

        currentBlendMode = mode;

        switch (mode) {
            case BLEND_NONE:
                // sceGuDisable(GU_BLEND);
                set(GU_BLEND, false);
                break;

            case BLEND_ALPHA:
                // sceGuEnable(GU_BLEND);
                set(GU_BLEND, true);
                sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
                break;

            case BLEND_ADDITIVE:
                // sceGuEnable(GU_BLEND);
                set(GU_BLEND, true);
                sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_FIX, 0, 0xFFFFFFFF);
                break;

            case BLEND_MULTIPLY:
                // sceGuEnable(GU_BLEND);
                set(GU_BLEND, true);
                sceGuBlendFunc(GU_ADD, GU_DST_COLOR, GU_ZERO, 0, 0);
                break;
        }
    }

    void setDepthState(DepthState state) {
        if (currentDepthMode == state) return;
        currentDepthMode = state;

        switch (state) {
            case DEPTH_DISABLED:
                // sceGuDisable(GU_DEPTH_TEST);
                set(GU_DEPTH_TEST, false);
                sceGuDepthMask(GU_TRUE); // Disable writing to Z-buffer
                break;

            case DEPTH_READ_WRITE:
                // sceGuEnable(GU_DEPTH_TEST);
                set(GU_DEPTH_TEST, true);
                sceGuDepthFunc(GU_GEQUAL); // PSP hardware uses GEQUAL for front-to-back depth
                sceGuDepthMask(GU_FALSE);  // Enable writing to Z-buffer
                break;

            case DEPTH_READ_ONLY:
                // sceGuEnable(GU_DEPTH_TEST);
                set(GU_DEPTH_TEST, true);
                sceGuDepthFunc(GU_GEQUAL);
                sceGuDepthMask(GU_TRUE);   // Read depth, but don't overwrite Z-buffer
                break;
        }
    }

    void setCullMode(CullMode mode) {
        if (currentCullMode == mode) return;
        currentCullMode = mode;

        if (mode == CULL_NONE) {
            // sceGuDisable(GU_CULL_FACE);
            set(GU_CULL_FACE, false);
        } else {
            // sceGuEnable(GU_CULL_FACE);
            set(GU_DEPTH_TEST, true);
            sceGuFrontFace(mode == CULL_CW ? GU_CW : GU_CCW);
        }
    }

    void bindTexture(Texture* texture){
        if (currentTexture != texture){
            sceGuTexImage(0, texture->width, texture->height, texture->width, texture->data);
            currentTexture = texture;
        } 
    }

    void set(int state, bool enable) {
        if (flags[state] != enable) {
            flags[state] = enable;
            if (enable) {
                sceGuEnable(state);
            } else {
                sceGuDisable(state);
            }

        } 
    }

}