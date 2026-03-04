#pragma once
#include "GlobalDefs.h"
#include <string>
#include "BattleCommand.h"



class Menu {

    enum MenuState{
        CommandMenu,
        AbilitiesMenu,
        TechniquesMenu,
        ItemsMenu,
        EnemyMenu
    };

    public:
        int selectedIndex;
        //highest index for each array of options (option num - 1)
        int optionMax, abilitiesMax, itemsMax, techniquesMax;
        int x, y, padding;
        int buttonWidth, buttonHeight;

        const char* options[4];
        // BattleCommand* abilities[10];
        BattleCommand* items[10];
        BattleCommand* techniques[10];

        MenuState menuState;

        Character* activeCharacter;



        Menu();
    
        void cursorDown();
        void cursorUp();
        void cursorLeft();
        void cursorRight();
        void drawMenu();
        void selectButton();
        void backButton();
        void earlyExecuteButton();
        void setActiveCharacter(Character* character);



};