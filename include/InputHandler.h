#pragma once

#include "Command.h"
#include "DashCommand.h"
#include <pspctrl.h>

class InputHandler{

    private:
        Command* buttonCross;
        Command* buttonSquare;
        Command* buttonTriangle;
        Command* buttonCircle;

        Command* buttonL;
        Command* buttonR;

        Command* buttonLeft;
        Command* buttonRight;
        Command* buttonUp;
        Command* buttonDown;

                                

    public:
        SceCtrlData gamePad;
        SceCtrlData oldGamePad;

        float analogueX;
        float analogueY;

        InputHandler();

        ~InputHandler(){
            delete buttonSquare;
        }

        Command* handleInput();
        void readInput();
        bool getButtonDown(PspCtrlButtons button);



};