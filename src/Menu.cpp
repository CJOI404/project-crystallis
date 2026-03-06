#include "Menu.h"
#include "UIRender.h"
#include "AttackCommand.h"
#include "CommandRegistry.h"

Menu::Menu(){
    x = 5;
    y = 190;
    padding = 5;
    cascadeOffset = 10;
    selectedIndex = 0;
    selectedParadigm = 0;
    optionMax = 3;
    abilitiesMax = 9;
    paradigmMax = 5;
    teamSize = 0;

    buttonWidth = 110;
    buttonHeight = 15;

    menuState = CommandMenu;

    //these options will never change, can be initialized here
    options[0] = "ABILITIES";
    options[1] = "AUTO BATTLE";
    options[2] = "TECHNIQUES";
    options[3] = "ITEMS";

    //Set paradigms for testing (will have to abstract this later so they can be edited)
    Paradigm* p1 = new Paradigm {"RUTHLESS", Role::COMMANDO, Role::RAVAGER, Role::SABOTEUR};
    Paradigm* p2 = new Paradigm {"CERBERUS", Role::RAVAGER, Role::COMMANDO, Role::COMMANDO};
    Paradigm* p3 = new Paradigm {"Ruthless", Role::SABOTEUR, Role::RAVAGER, Role::SABOTEUR};
    Paradigm* p4 = new Paradigm {"Cerberus", Role::RAVAGER, Role::COMMANDO, Role::COMMANDO};
    Paradigm* p5 = new Paradigm {"Ruthless", Role::COMMANDO, Role::RAVAGER, Role::SABOTEUR};
    Paradigm* p6 = new Paradigm {"Cerberus", Role::RAVAGER, Role::COMMANDO, Role::COMMANDO};

    paradigms[0] = p1;
    paradigms[1] = p2;
    paradigms[2] = p3;
    paradigms[3] = p4;
    paradigms[4] = p5;
    paradigms[5] = p6;




}

void Menu::setActiveCharacter(Character* character){
    activeCharacter = character;
}

void Menu::cursorDown(){
    if (optionMax > selectedIndex){
        selectedIndex++;
    } 

}

void Menu::cursorUp(){
    if (0 < selectedIndex){
        selectedIndex--;
    } 
}

void Menu::cursorLeft(){
    if (selectedIndex >= 4){
        selectedIndex -= 4;
    } 

}

void Menu::cursorRight(){
    if (menuState != ParadigmMenu && selectedIndex <= (optionMax - 4)){
        selectedIndex += 4;
    } 
}

// void Menu::setOptionMax(){

// }
void Menu::changeMenuState(MenuState mState){
    menuState = mState;
    switch(mState){
        case CommandMenu: 
            selectedIndex = 0;
            optionMax = std::size(options) - 1;
            break;
        case AbilitiesMenu:
            selectedIndex = 0;
            optionMax = activeCharacter->abilities.size() - 1;
            break;
        case EnemyMenu:
            selectedIndex = 0;
            optionMax = activeCharacter->enemyList.size() - 1;
            break;
        case ItemsMenu:
        case ParadigmMenu:
            selectedIndex = selectedParadigm;
            optionMax = std::size(paradigms) - 1;
            break;
        case TechniquesMenu:
            break;
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
                            changeMenuState(AbilitiesMenu);
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
                activeCharacter->commandQueue.push_back(activeCharacter->abilities[selectedIndex]);
                if (activeCharacter->commandQueue.size() == activeCharacter->atbSegments){
                    changeMenuState(EnemyMenu);
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
            changeMenuState(CommandMenu);
            break;
        case ParadigmMenu:
            activeCharacter->currentRole = paradigms[selectedIndex]->r1;
            activeCharacter->teamList.at(1)->currentRole = paradigms[selectedIndex]->r2;
            activeCharacter->teamList.at(2)->currentRole = paradigms[selectedIndex]->r3;

            for (int i = 0; i < activeCharacter->teamList.size(); i++){
                activeCharacter->teamList.at(i)->addViableBattleCommands();
            }

            changeMenuState(CommandMenu);

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
                changeMenuState(CommandMenu);
            }
            break;

        case EnemyMenu:
            //return to ability select
            
            changeMenuState(AbilitiesMenu);
            break;


        case ParadigmMenu:
            changeMenuState(CommandMenu);
    }
}

void Menu::earlyExecuteButton(){
    switch (menuState){
        case AbilitiesMenu:
            if (activeCharacter->commandQueue.empty()){
                changeMenuState(CommandMenu);         
            } else {
                changeMenuState(EnemyMenu);
            }
            break;
        case CommandMenu:
            if (activeCharacter->characterState == CharacterState::AttackReady && activeCharacter->currAtbVal >= 1){
                for (int i = activeCharacter->commandQueue.size() - 1; i >= (int)activeCharacter->currAtbVal; i--){
                    activeCharacter->commandQueue.pop_back();
                }
            }

    }
}

void Menu::paradigmSwitchButton(){
    if (menuState == ParadigmMenu){
        changeMenuState(CommandMenu);
    } else {
        changeMenuState(ParadigmMenu);
    }
}

void Menu::drawPage(int buttonWidth, int buttonHeight, int yButtonAmt, int cascade){

}

void Menu::drawAtb(){
    UI::drawRect(5, 170, activeCharacter->atbSegments * 50, 12, Colours::LIGHTGREY);
    UI::drawRect(5, 172, activeCharacter->currAtbVal * 50, 8, Colours::LIGHTBLUE);
}

void Menu::drawTeamStats(){

    int currX = x + 250;
    int currY = y + 32;


    for (int i = 0; i < activeCharacter->teamList.size(); i++){
        UI::drawRect(currX + 80, currY + 2, 120, 6, Colours::LIGHTGREY);
        UI::drawRect(currX + 80, currY + 3, ((float) activeCharacter->health / activeCharacter->maxHealth) * 120, 4, Colours::LIGHTGREEN);

        UI::drawString(currX + 80, currY - 8, 0xFFFFFFFF, 0.3, 0.3, activeCharacter->teamList.at(i)->name);
        UI::drawString(currX + 150, currY - 8, 0xFFFFFFFF, 0.3, 0.3, roleToString(activeCharacter->teamList.at(i)->currentRole));
        currY += 15;
        currX += cascadeOffset;

    }
}

void Menu::drawMenu(){

    int currX = x;
    int currY = y;
    // int currCascadeOffset = cascadeOffset;

    drawTeamStats();

    switch (menuState){
        case CommandMenu:
            drawAtb();
            for (int i = 0; i <= optionMax; i++){
                Colours colour = Colours::LIGHTGREY;
                if (selectedIndex == i) colour = Colours::RED;
                UI::drawRect(currX, currY, buttonWidth, buttonHeight, colour);
                UI::drawString(currX + 5, currY + 2, 0xFFFFFFFF, 0.35, 0.35, options[i]);
                currY += buttonHeight + padding;
                currX += cascadeOffset;
            }
            break;
        case AbilitiesMenu:{
            drawAtb();
            int idx = (selectedIndex / 8) * 8;
            currX = x;
            currY = y;
            for (int i = idx; i < activeCharacter->abilities.size() && i < idx + 8; i++){
                if (i != idx && i % 4 == 0){
                    currX -= cascadeOffset*4;
                    currX += buttonWidth + padding;
                    currY = y;
                }
                Colours colour = Colours::LIGHTGREY;
                if (selectedIndex == i) colour = Colours::RED;
                UI::drawRect(currX, currY, buttonWidth, buttonHeight, colour);

                //IMPORTANT: CHECK FOR ENOUGH ABILITIES
                if (activeCharacter->abilities[i] != nullptr) UI::drawString(currX + 5, currY + 2, 0xFFFFFFFF, 0.35, 0.35, activeCharacter->abilities[i]->name);
                currY += buttonHeight + padding;
                currX += cascadeOffset;
            }
            break;
        }

        case EnemyMenu:
            drawAtb();
            for (int i = 0; i <= optionMax; i++){
                if (i != 0 && i % 4 == 0){
                    currX -= cascadeOffset*4;
                    currX += buttonWidth + padding;
                    currY = y;
                }
                Colours colour = Colours::LIGHTGREY;
                if (selectedIndex == i) colour = Colours::RED;
                UI::drawRect(currX, currY, buttonWidth, buttonHeight, colour);
                UI::drawString(currX + 5, currY + 2, 0xFFFFFFFF, 0.35, 0.35, activeCharacter->enemyList.at(i)->name);
                currY += buttonHeight + padding;
                currX += cascadeOffset;
            }

            break;
        case ParadigmMenu:
            currY -= 30;
            for (int i = 0; i <= optionMax; i++){
                // if (i != 0 && i % 4 == 0){
                //     currX += buttonWidth + padding;
                //     currY = y - 30;;
                // }
                Colours colour = Colours::LIGHTGREY;
                if (selectedIndex == i) colour = Colours::RED;
                UI::drawRect(currX, currY, buttonWidth * 1.5, buttonHeight * 0.9, colour);
                
                snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s \t %s %s %s", paradigms[i]->name, roleToString(paradigms[i]->r1), roleToString(paradigms[i]->r2), roleToString(paradigms[i]->r3));
                UI::drawString(currX + 5, currY, 0xFFFFFFFF, 0.35, 0.35, UI::textBuffer);
                currY += (buttonHeight * 0.9) + padding;
            }

            break;
    }

    //draw abilities in queue
    int atbSpacing = 0;
    for (int i = 0; i < activeCharacter->commandQueue.size(); i++){
        UI::drawString(5 + atbSpacing, 160, 0xFFFFFFFF, 0.3, 0.3, activeCharacter->commandQueue.at(i)->name); 
        atbSpacing += 50;              
    }

}
