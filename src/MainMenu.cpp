#include "MainMenu.h"
#include "graphics/RenderState.h"

MainMenu::MainMenu(){
    selectedIdx = 0;
    startFlag = false;

    options[0] = "START GAME";
    options[1] = "OPTIONS";

    //Load Textures
    logo = TextureManager::load("Logo.png", 512, 512);

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

    //DRAW 2d
    RenderState::setDepthState(DEPTH_DISABLED);
    UI::drawRect(0, 0, 480, 272, 0xFFFFFFFF);
    // UI::drawSprite(112, -40, 1, 1, 0x00000000, 1, 1);
    logo->draw(50, -80, 400, 400, 0xFFFFFFFF);


    Colours colour = Colours::LIGHTGREY;

    int currY = 200;

    for (int i = 0; i < std::size(options); i++){
        if (selectedIdx == i) colour = Colours::RED;
        else colour = Colours::LIGHTGREY;

        
        UI::drawRect(25, currY, 150, 20, colour);
        UI::drawString(25, currY, 0xFFFFFFFF, 0.5, 0.5, options[i]);
        currY += 25;
    }

}