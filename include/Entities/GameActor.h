#pragma once

#include "Components/Component.h"
#include "Components/MovementComponent.h"
#include "graphics/GraphicsUtils.h"

class GameActor{


    public:
        //Add components here (nullptr for specific ones, assign in subclass)
        MovementComponent* moveComp = nullptr;

    public:
        float xPos = 50, yPos = 50, zPos = 0;
        ScePspFVector3 worldRot = {0.0f, 0.0f, 0.0f};
        ScePspFVector3 worldPos = {0, 0, 0};
        GraphicsUtils::ScreenPos screenPos = {0.0f, 0.0f, false};

        virtual void update(float dt) = 0;
        virtual void render(float dt) = 0;
};