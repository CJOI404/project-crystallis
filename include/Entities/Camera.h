#include <pspgum.h>
#include <pspgu.h>
#include <cmath>

enum ViewType{
    LookAt,
    Project
};

class Camera {
    public:
        ViewType cameraType = Project;

        ScePspFVector3 position = {0.0f, 0.0f, 0.0f};
        ScePspFVector3 target = {0.0f, 0.0f, 0.0f};
        ScePspFVector3 up = {0.0f, 1.0f, 0.0f};

        inline void setPos(ScePspFVector3 pos){
            position = pos;
        }

        inline void setTarget(ScePspFVector3 target){
            this->target = target;
        }

        inline void update() {
            sceGumMatrixMode(GU_VIEW);
            sceGumLoadIdentity();

            switch (cameraType){
                case Project: {
                    sceGumLoadIdentity();
                    ScePspFVector3 inverse = {-position.x, -position.y, -position.z};
                    sceGumTranslate(&inverse);
                    break;
                }

                case LookAt:
                    sceGumLookAt(&position, &target, &up);
                    break;
            }

        }
    
};