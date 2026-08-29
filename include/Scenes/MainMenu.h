#pragma once
#include "graphics/AssetManagers/UIRender.h"
#include "GlobalDefs.h"
#include "InputHandler.h"
#include "GlobalDefs.h"
#include "Scenes/Scene.h"

class MainMenu : public Scene {


    public:
        SceneID sceneID = SceneID::MAIN_MENU;

        MainMenu();

        void update(float dt) override;
        void render(float dt) override;
        void unload() override;

        void selectButton();
        
        bool startFlag;

        int selectedIdx;

        const char* options[2];

        Texture* logo;
};