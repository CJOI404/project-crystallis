#include <pspgu.h>
#include <pspgum.h>

void setupCamera() {
    // 1. Setup Projection Matrix (Lens & Frustum)
    sceGumMatrixMode(GU_PROJECTION);
    sceGumLoadIdentity();
    
    float fov    = 75.0f;               // Vertical FOV in degrees
    float aspect = 480.0f / 272.0f;      // PSP screen aspect ratio (approx 1.764)
    float zNear  = 0.5f;                // Geometry closer than 0.5 units is clipped
    float zFar   = 1000.0f;             // Geometry further than 1000 units is clipped
    
    sceGumPerspective(fov, aspect, zNear, zFar);

    // 2. Setup View Matrix (Camera Position & Rotation)
    sceGumMatrixMode(GU_VIEW);
    sceGumLoadIdentity();
    
    ScePspFVector3 eye    = { 0.0f, 2.0f, 5.0f }; // Camera location in world space
    ScePspFVector3 center = { 0.0f, 0.0f, 0.0f }; // Point the camera is aiming at
    ScePspFVector3 up     = { 0.0f, 1.0f, 0.0f }; // Up direction vector (+Y up)
    
    sceGumLookAt(&eye, &center, &up);
}