#pragma once

#include <pspctrl.h>

class InputHandler{
                   

    public:
        SceCtrlData gamePad;
        SceCtrlData oldGamePad;

        float analogueX;
        float analogueY;

        InputHandler();

        void readInput();
        bool getButtonDown(PspCtrlButtons button);



};