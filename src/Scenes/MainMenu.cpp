#include "Scenes/MainMenu.h"
#include "Scenes/CombatInstance.h"
#include "Scenes/SceneManager.h"
#include "graphics/Pipeline/RenderQueue.h"
#include "graphics/GraphicsUtils.h"

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
            SceneManager::setNextScene(new CombatInstance());
            // startFlag = true;
    }
}

void MainMenu::update(float dt) {

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

    SubmitRect(&g_queue, 0, 0, 480, 272, 0xFFFFFFFF, GraphicsUtils::Layer::BACKGROUND_0);
    Submit2D(&g_queue, logo, 50, -80, 400, 400, 0xFFFFFFFF, GraphicsUtils::Layer::UI_3);


    Colours colour = Colours::LIGHTGREY;

    int currY = 200;

    for (int i = 0; i < std::size(options); i++){
        if (selectedIdx == i) colour = Colours::RED;
        else colour = Colours::LIGHTGREY;

        SubmitRect(&g_queue, 25, currY, 150, 20, colour, GraphicsUtils::Layer::UI_3);
        SubmitText(&g_queue, options[i], 25, currY, 0.5, 0.5, 0xFFFFFFFF, GraphicsUtils::Layer::UI_3);

        currY += 25;
    }

}

void MainMenu::unload(){
    TextureManager::unload("Logo.png");
}