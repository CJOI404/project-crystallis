#pragma once
#include <cstdint>
#include <pspgum.h>
#include <pspgu.h>

namespace GraphicsUtils {

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


    
    inline ScreenPos worldToScreen(const ScePspFVector3& worldPos, const ScePspFMatrix4& viewProjMatrix) {
        ScreenPos result;

        // 1. Multiply World Position by View-Projection Matrix -> Clip Space (Homogeneous Coords)
        float clipX = worldPos.x * viewProjMatrix.x.x + worldPos.y * viewProjMatrix.y.x + worldPos.z * viewProjMatrix.z.x + viewProjMatrix.w.x;
        float clipY = worldPos.x * viewProjMatrix.x.y + worldPos.y * viewProjMatrix.y.y + worldPos.z * viewProjMatrix.z.y + viewProjMatrix.w.y;
        float clipZ = worldPos.x * viewProjMatrix.x.z + worldPos.y * viewProjMatrix.y.z + worldPos.z * viewProjMatrix.z.z + viewProjMatrix.w.z;
        float clipW = worldPos.x * viewProjMatrix.x.w + worldPos.y * viewProjMatrix.y.w + worldPos.z * viewProjMatrix.z.w + viewProjMatrix.w.w;

        // 2. Check if behind camera (culling)
        if (clipW <= 0.001f) {
            result.visible = false;
            return result;
        }

        // 3. Perspective Divide -> Normalized Device Coordinates (NDC: -1.0 to +1.0)
        float ndcX = clipX / clipW;
        float ndcY = clipY / clipW;

        // 4. Map NDC to PSP Screen Dimensions (480x272)
        // Note: Invert Y because PSP screen coordinates start at Top-Left (0,0)
        result.x = (ndcX + 1.0f) * 0.5f * 480.0f;
        result.y = (1.0f - ndcY) * 0.5f * 272.0f;
        result.visible = true;

        return result;
    }

    inline ScePspFMatrix4 getViewProjectionMatrix() {
        ScePspFMatrix4 view, proj, viewProj;

        // 1. Grab View Matrix from VFPU stack
        sceGumMatrixMode(GU_VIEW);
        sceGumStoreMatrix(&view);

        // 2. Grab Projection Matrix from VFPU stack
        sceGumMatrixMode(GU_PROJECTION);
        sceGumStoreMatrix(&proj);

        // 3. Multiply Proj * View = ViewProj Matrix
        gumMultMatrix(&viewProj, &proj, &view);

        // sceGumMatrixMode(GU_MODEL);

        return viewProj;
    }
}