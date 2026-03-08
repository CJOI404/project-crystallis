#pragma once

#include "Component.h"
#include "MovementComponent.h"

class GameActor{


    public:
        //Add components here (nullptr for specific ones, assign in subclass)
        MovementComponent* moveComp = nullptr;

    public:
        float xPos = 50, yPos = 50, zPos = 0;

        virtual void update(float dt) = 0;
        virtual void render(float dt) = 0;
};