#pragma once

#include <pspctrl.h>

namespace InputHandler{
                   
        extern SceCtrlData gamePad;
        extern SceCtrlData oldGamePad;

        extern float analogueX;
        extern float analogueY;

        // InputHandler();
        void initializeInputHandler();

        void readInput();
        bool getButtonDown(PspCtrlButtons button);

};