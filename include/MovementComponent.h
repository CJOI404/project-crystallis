#pragma once

#include "Component.h"
#include "math.h"
#include "GlobalDefs.h"
#include <string>

enum MovementState {
    STATE_IDLE,
    STATE_WALKING,
    STATE_RUNNING,
    STATE_DASHING
};

class MovementComponent : Component {
    private:
        float* xPos;
        float* yPos;

        float analogueX;
        float analogueY;
        float analogueMagnitude;

        float xDir;
        float yDir;
        float magnitude;

        //Timers
        float currDashTime = -1;
        float currDashCooldown = -1;

        //CONSTANTS:

        const int DASH_SPEED = 20;
        const int WALK_SPEED = 4;

        //Time in seconds for abilities
        const float DASH_COOLDOWN = 3;
        const float DASH_TIME = 0.1;



    public:

        MovementState state;
        Colours color = Colours::RED;

        MovementComponent(float* xPos, float* yPos);

        void dash();

        void setAnalogueMoveVals(float x, float y);

        void handleAnalogueMovement();

        void update(float dt) override;

};