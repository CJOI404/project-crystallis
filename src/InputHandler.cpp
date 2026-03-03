#include "InputHandler.h"

InputHandler::InputHandler(){

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    analogueX = 0;
    analogueY = 0;

    buttonSquare = new DashCommand();
    buttonCross = nullptr;
    buttonTriangle = nullptr;
    buttonCircle = nullptr;

    buttonL = nullptr;
    buttonR = nullptr;

    buttonLeft = nullptr;
    buttonRight = nullptr;
    buttonUp = nullptr;
    buttonDown = nullptr;
    
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

//Digital Button input using command system
Command* InputHandler::handleInput(){
    if (gamePad.Buttons != 0)
    {
        
        if (gamePad.Buttons & PSP_CTRL_SQUARE){ return buttonSquare;  }
        if (gamePad.Buttons & PSP_CTRL_CROSS){ return buttonCross;   }
        if (gamePad.Buttons & PSP_CTRL_TRIANGLE){ return buttonTriangle;  }
        if (gamePad.Buttons & PSP_CTRL_CIRCLE){ return buttonCircle;   }

        if (gamePad.Buttons & PSP_CTRL_RIGHT){ return buttonRight;  }
        if (gamePad.Buttons & PSP_CTRL_LEFT){ return buttonLeft;  }
        if (gamePad.Buttons & PSP_CTRL_UP){ return buttonUp;  }
        if (gamePad.Buttons & PSP_CTRL_DOWN){ return buttonDown;  }

        if (gamePad.Buttons & PSP_CTRL_LTRIGGER){ return buttonL;   }
        if (gamePad.Buttons & PSP_CTRL_RTRIGGER){ return buttonR;  }

    }

    return nullptr;
}
