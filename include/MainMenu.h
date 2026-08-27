#pragma once
#include "graphics/UIRender.h"
#include "GlobalDefs.h"
#include "InputHandler.h"

class MainMenu{


    public:
        MainMenu();

        void update(float dt);
        void render(float dt);

        void selectButton();
        
        bool startFlag;

        int selectedIdx;

        const char* options[2];

        Texture* logo;
};