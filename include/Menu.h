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
        EnemyMenu,
        ParadigmMenu
    };

    public:
        int selectedIndex, selectedParadigm;
        //highest index for each array of options (option num - 1)
        int optionMax, abilitiesMax, itemsMax, techniquesMax, paradigmMax, teamSize;
        int x, y, padding, cascadeOffset;
        int buttonWidth, buttonHeight;

        const char* options[4];

        Paradigm* paradigms[6];

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

        //this is for the entire render call
        void drawMenu();
        //this is for individual pages
        void drawPage(int buttonWidth, int buttonHeight, int yButtonAmt, int cascade);

        void drawAtb();
        void drawTeamStats();

        void selectButton();
        void backButton();
        void earlyExecuteButton();
        void paradigmSwitchButton();
        void changeMenuState(MenuState mState);
        void setParadigm();

        void setActiveCharacter(Character* character);



};