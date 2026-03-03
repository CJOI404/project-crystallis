#include "Menu.h"
#include "UIRender.h"
#include "AttackCommand.h"

Menu::Menu(){
    x = 10;
    y = 180;
    padding = 5;
    selectedIndex = 0;
    optionMax = 3;
    abilitiesMax = 1;

    buttonWidth = 110;
    buttonHeight = 15;

    menuState = CommandMenu;

    //these options will never change, can be initialized here
    options[0] = Button("ABILITIES");
    options[1] = Button("AUTO BATTLE");
    options[2] = Button("TECHNIQUES");
    options[3] = Button("ITEMS");

    //abilities array must be loaded from all of the character's ablities dynamically, once when the paradigm is shifted.
    //this is temporary for testing.
    abilities[0] = new AttackCommand("ATTACK", 1.0, 0, 0.5, 1, 3.3, 1);
    abilities[1] = new AttackCommand("RUIN", 0, 1.0, 10, 1, 0.9, 1);

    options[0].highlighted = true;
}

void Menu::setActiveCharacter(Character* character){
    activeCharacter = character;
}

void Menu::cursorDown(){
    if (optionMax > selectedIndex){
        options[selectedIndex].highlighted = false;
        selectedIndex++;
        options[selectedIndex].highlighted = true;
    } 

}

void Menu::cursorUp(){
    if (0 < selectedIndex){
        options[selectedIndex].highlighted = false;
        selectedIndex--;
        options[selectedIndex].highlighted = true;
    } 
}

void Menu::cursorLeft(){
    if (selectedIndex >= 4){
        options[selectedIndex].highlighted = false;
        selectedIndex -= 4;
        options[selectedIndex].highlighted = true;
    } 

}

void Menu::cursorRight(){
    if (selectedIndex <= (optionMax - 4)){
        options[selectedIndex].highlighted = false;
        selectedIndex += 4;
        options[selectedIndex].highlighted = true;
    } 
}

void Menu::selectButton(){
    switch (menuState){
        case CommandMenu:
            switch (selectedIndex){
                case 0:
                    //go to abilities menu if the character is not attacking or about to attack
                    if (activeCharacter->characterState != CharacterState::Attacking
                        && activeCharacter->characterState != CharacterState::AttackReady){
                        menuState = AbilitiesMenu;
                        selectedIndex = 0;
                        optionMax = abilitiesMax;
                    }
                    break;
                case 1:
                
                    break;
                case 2:

                    break;
                case 3:

                    break;
            }
            break;
        case AbilitiesMenu:
            //add corresponding attackcommand in list to the character's queue. 
            if (activeCharacter->commandQueue.size() < activeCharacter->atbSegments){
                activeCharacter->commandQueue.push_back(abilities[selectedIndex]);

                if (activeCharacter->commandQueue.size() == activeCharacter->atbSegments){
                    menuState = EnemyMenu;
                    selectedIndex = 0;
                    optionMax = activeCharacter->enemyList.size() - 1;
                }
            }

            break;
        case TechniquesMenu:
            break;
        case ItemsMenu:
            break;

        case EnemyMenu:
            //set the character's target and enter attack ready state, return to command menu
            activeCharacter->startAttack(selectedIndex);
            menuState = CommandMenu;
            optionMax = 3;
            selectedIndex = 0;

            break;
    }

}

void Menu::backButton(){
    switch (menuState){
        case CommandMenu:
            //cancel queued commands
            activeCharacter->commandQueue.clear();
            activeCharacter->characterState = CharacterState::Waiting;
            break;
        case AbilitiesMenu:
            //delete most recent command or return to command menu if queue is empty
            if (!(activeCharacter->commandQueue.empty())){
                activeCharacter->commandQueue.pop_back();
            } else {
                menuState = CommandMenu;
                selectedIndex = 0;
                optionMax = 3;
            }
            break;

        case EnemyMenu:
            //return to ability select
            menuState = AbilitiesMenu;
            selectedIndex = 0;
            optionMax = abilitiesMax;
            break;
    }
}

void Menu::earlyExecuteButton(){
    switch (menuState){
        case AbilitiesMenu:
            if (activeCharacter->commandQueue.empty()){
                menuState = CommandMenu;
                selectedIndex = 0;
                optionMax = 3;                
            } else {
                menuState = EnemyMenu;
                selectedIndex = 0;
                optionMax = activeCharacter->enemyList.size() - 1;
            }
            break;
        case CommandMenu:
            if (activeCharacter->characterState == CharacterState::AttackReady){
                for (int i = activeCharacter->commandQueue.size() - 1; i >= (int)activeCharacter->currAtbVal; i--){
                    activeCharacter->commandQueue.pop_back();
                }
            }

    }
}

void Menu::drawMenu(){
    // UI::drawRect(50, 200, 200, 20, 0x000000);
    int currX = x;
    int currY = y;

    switch (menuState){
        case CommandMenu:
            for (int i = 0; i <= optionMax; i++){
                Colours colour = Colours::LIGHTGREY;
                if (selectedIndex == i) colour = Colours::RED;
                UI::drawRect(currX, currY, buttonWidth, buttonHeight, colour);
                UI::drawString(currX + 5, currY + 5, 0xFFFFFFFF, 0.4, 0.25, options[i].text);
                currY += buttonHeight + padding;
            }
            break;
        case AbilitiesMenu:
            for (int i = 0; i <= abilitiesMax; i++){
                if (i != 0 && i % 4 == 0){
                    currX += buttonWidth + padding;
                    currY = y;
                }
                Colours colour = Colours::LIGHTGREY;
                if (selectedIndex == i) colour = Colours::RED;
                UI::drawRect(currX, currY, buttonWidth, buttonHeight, colour);
                UI::drawString(currX + 5, currY + 5, 0xFFFFFFFF, 0.4, 0.25, abilities[i]->name);
                currY += buttonHeight + padding;
            }

            break;
        case EnemyMenu:
            for (int i = 0; i <= optionMax; i++){
                if (i != 0 && i % 4 == 0){
                    currX += buttonWidth + padding;
                    currY = y;
                }
                Colours colour = Colours::LIGHTGREY;
                if (selectedIndex == i) colour = Colours::RED;
                UI::drawRect(currX, currY, buttonWidth, buttonHeight, colour);
                UI::drawString(currX + 5, currY + 5, 0xFFFFFFFF, 0.4, 0.25, activeCharacter->enemyList.at(i)->name);
                currY += buttonHeight + padding;
            }

            break;
    }

    //draw abilities in queue
    int atbSpacing = 0;
    for (int i = 0; i < activeCharacter->commandQueue.size(); i++){
        UI::drawString(10 + atbSpacing, 150, 0xFFFFFFFF, 0.3, 0.1, activeCharacter->commandQueue.at(i)->name); 
        atbSpacing += 50;              
    }

}
