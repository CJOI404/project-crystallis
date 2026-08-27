#include "MainMenu.h"

MainMenu::MainMenu(){
    selectedIdx = 0;
    startFlag = false;

    options[0] = "START GAME";
    options[1] = "OPTIONS";

    //Load Textures
    logo = TextureManager::load("Logo.png", 256);
    logo = TextureManager::load("Logo copy.png", 256);
    logo = TextureManager::load("Logo copy 2.png", 256);
    logo = TextureManager::load("Logo copy 3.png", 256);
    logo = TextureManager::load("Logo copy 4.png", 256);
        logo = TextureManager::load("Logo.png", 256);
    logo = TextureManager::load("Logo copy.png", 256);
    logo = TextureManager::load("Logo copy 2.png", 256);
    logo = TextureManager::load("Logo copy 3.png", 256);
    logo = TextureManager::load("Logo copy 4.png", 256);

    // logo = TextureManager::load("Logo.png", 256);
    // logo = TextureManager::load("Logo.png", 256);
    // UI::loadSprite("Logo.png");
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

    UI::drawRect(0, 0, 480, 272, 0xFFFFFFFF);
    // UI::drawSprite(112, -40, 1, 1, 0x00000000, 1, 1);
    logo->draw(112, -40, 300, 300);


    Colours colour = Colours::LIGHTGREY;

    int currY = 200;

    for (int i = 0; i < std::size(options); i++){
        if (selectedIdx == i) colour = Colours::RED;
        else colour = Colours::LIGHTGREY;

        
        UI::drawRect(25, currY, 150, 20, colour);
        UI::drawString(25, currY, 0x00000000, 0.5, 0.5, options[i]);
        currY += 25;
    }

}