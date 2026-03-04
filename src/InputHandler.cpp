#include "InputHandler.h"

InputHandler::InputHandler(){

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    analogueX = 0;
    analogueY = 0;
    
}

bool InputHandler::getButtonDown(PspCtrlButtons button){
    if ((gamePad.Buttons & button)
        && !(oldGamePad.Buttons & button)){
            return true;
        } else {
            return false;
        }
}

void InputHandler::readInput(){

    oldGamePad = gamePad;
    sceCtrlReadBufferPositive(&gamePad, 1);

    analogueX = (gamePad.Lx >= 128) ? (gamePad.Lx - 128) / 127.0f : (gamePad.Lx - 128) / 128.0f;
    analogueY = (gamePad.Ly >= 128) ? (gamePad.Ly - 128) / 127.0f : (gamePad.Ly - 128) / 128.0f;  
    
}
