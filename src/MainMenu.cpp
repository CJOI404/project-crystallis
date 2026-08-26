#include "MainMenu.h"

MainMenu::MainMenu(){
    selectedIdx = 0;
    startFlag = false;

    options[0] = "START GAME";
    options[1] = "OPTIONS";
}

void MainMenu::selectButton(){
    switch (selectedIdx){
        case 0:
            startFlag = true;
    }
}

void MainMenu::update(float dt){

    if (InputHandler::getButtonDown(PSP_CTRL_CROSS)){
        selectButton();
    }

    if (InputHandler::getButtonDown(PSP_CTRL_DOWN)){
        selectedIdx++;
    }
    if (InputHandler::getButtonDown(PSP_CTRL_UP)){
        selectedIdx--;
    }

    if (selectedIdx < 0) selectedIdx = 0;
    if (selectedIdx >= std::size(options)) selectedIdx = std::size(options);
}

void MainMenu::render(float dt){
    Colours colour = Colours::LIGHTGREY;

    int currY = 120;
    for (int i = 0; i < std::size(options); i++){
        if (selectedIdx == i) colour = Colours::RED;
        else colour = Colours::LIGHTGREY;

        
        UI::drawRect(165, currY, 150, 20, colour);
        UI::drawString(165, currY, 0x00000000, 0.5, 0.5, options[i]);
        currY += 25;
    }

}