#include "MovementComponent.h"

MovementComponent::MovementComponent(float* xPos, float* yPos){
    state = STATE_IDLE;

    this->xPos = xPos;
    this->yPos = yPos;

    xDir = 0;
    yDir = 0;
        
}

void MovementComponent::setAnalogueMoveVals(float x, float y){
    analogueX = x;
    analogueY = y;
}

void MovementComponent::handleAnalogueMovement(){
    if (state == STATE_IDLE || state == STATE_WALKING){

        xDir = 0;
        yDir = 0;

        //Ignore Deadzone
        if (analogueX <= 0.1 && analogueX >= -0.1) xDir = 0;
        else xDir = analogueX;
        if (analogueY <= 0.1 && analogueY >= -0.1) yDir = 0;
        else yDir = analogueY;

        //Get Magnitude
        magnitude = sqrt(xDir * xDir + yDir * yDir);

        //Normalize
        if (magnitude > 0.0f){
            state = STATE_WALKING;
            xDir /= magnitude;
            yDir /= magnitude;

            xDir *= abs(analogueX);
            yDir *= abs(analogueY);
        }
    }
}

void MovementComponent::dash(){
    if (state == STATE_WALKING || state == STATE_RUNNING){
        if (currDashCooldown <= 0){
            state = STATE_DASHING;
            currDashTime = DASH_TIME;
            currDashCooldown = DASH_COOLDOWN;
            // *xPos += 20;
        }
    }
}

void MovementComponent::update(float dt){

    handleAnalogueMovement();

    //Calculate cooldowns
    if (currDashCooldown > 0){
        currDashCooldown -=dt;
        color = Colours::RED;
    } else {
        color = Colours::BLUE;
    }

    switch (state){
        case STATE_DASHING:
            
            currDashTime -= dt;

            if (currDashTime > 0){

                *xPos += DASH_SPEED * xDir;
                *yPos += DASH_SPEED * yDir;

            } else {
                state = STATE_IDLE;
            }
            
            // if (currDashCooldown <= 0){
            //     state = STATE_IDLE;
            //     currDashCooldown = DASH_COOLDOWN;
            //     currDashTime = DASH_TIME;
            // }

            break;
        
        case STATE_WALKING:
            *xPos += xDir * WALK_SPEED;
            *yPos += yDir * WALK_SPEED;

            break;

    }

}
