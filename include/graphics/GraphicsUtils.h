#pragma once
#include <cstdint>
#include <pspgum.h>
#include <pspgu.h>

namespace GraphicsUtils {


    /**
     * Define layers for render passes here
     */
    enum Layer {
        BACKGROUND_0,
        OPAQUE_3D_1,
        TRANSLUCENT_3D_2,
        UI_3,
        UI_4
    };

    struct ScreenPos {
        float x;
        float y;
        bool visible; // True if the 3D point is in front of the camera
    };

    /**
     * Converts RGBA (0-255 colors, 0.0f-1.0f float alpha) to PSP 0xAABBGGRR format
     */ 
    inline uint32_t ColourRGBA(uint8_t r, uint8_t g, uint8_t b, float alpha = 1.0f) {
        uint8_t a = (uint8_t)(alpha * 255.0f);
        return ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)g << 8) | (uint32_t)r;
    }


    /**
     * Converts world position to screen position
     */
    inline ScreenPos worldToScreen(const ScePspFVector3& worldPos, const ScePspFMatrix4& viewProjMatrix) {
        ScreenPos result;

        //multiply world position by view-projection matrix -> clip Space
        float clipX = worldPos.x * viewProjMatrix.x.x + worldPos.y * viewProjMatrix.y.x + worldPos.z * viewProjMatrix.z.x + viewProjMatrix.w.x;
        float clipY = worldPos.x * viewProjMatrix.x.y + worldPos.y * viewProjMatrix.y.y + worldPos.z * viewProjMatrix.z.y + viewProjMatrix.w.y;
        float clipZ = worldPos.x * viewProjMatrix.x.z + worldPos.y * viewProjMatrix.y.z + worldPos.z * viewProjMatrix.z.z + viewProjMatrix.w.z;
        float clipW = worldPos.x * viewProjMatrix.x.w + worldPos.y * viewProjMatrix.y.w + worldPos.z * viewProjMatrix.z.w + viewProjMatrix.w.w;

        //check if behind camera
        if (clipW <= 0.001f) {
            result.visible = false;
            return result;
        }

        //normalize coordinates (NDC: -1.0 to 1.0)
        float ndcX = clipX / clipW;
        float ndcY = clipY / clipW;

        //map NDC to screen dimensions (480x272)
        result.x = (ndcX + 1.0f) * 0.5f * 480.0f;
        result.y = (1.0f - ndcY) * 0.5f * 272.0f;
        result.visible = true;

        return result;
    }

    inline ScePspFMatrix4 getViewProjectionMatrix() {
        ScePspFMatrix4 view, proj, viewProj;

        sceGumMatrixMode(GU_VIEW);
        sceGumStoreMatrix(&view);

        sceGumMatrixMode(GU_PROJECTION);
        sceGumStoreMatrix(&proj);

        gumMultMatrix(&viewProj, &proj, &view);

        return viewProj;
    }
}