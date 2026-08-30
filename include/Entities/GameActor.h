#pragma once

#include "graphics/GraphicsUtils.h"

class GameActor{

    public:
        ScePspFVector3 worldRot = {0.0f, 0.0f, 0.0f};
        ScePspFVector3 worldPos = {0, 0, 0};
        GraphicsUtils::ScreenPos screenPos = {0.0f, 0.0f, false};

        virtual void update(float dt) = 0;
        virtual void render(float dt) = 0;

        void setworldPos(ScePspFVector3 worldPos){
            this->worldPos = worldPos;
        }

        void setworldRot(ScePspFVector3 worldRot){
            this->worldRot = worldRot;
        }
};