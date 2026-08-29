#include "Entities/Menu.h"
#include "graphics/AssetManagers/UIRender.h"
#include "Entities/AttackCommand.h"
#include "CommandRegistry.h"
#include "graphics/Pipeline/RenderQueue.h"

Menu::Menu(){
    x = 5;
    y = 190;
    padding = 5;
    cascadeOffset = 10;
    selectedIndex = 0;
    selectedParadigm = 2;
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
    Paradigm* p4 = new Paradigm {"Cerberus", Role::SYNERGIST, Role::COMMANDO, Role::COMMANDO};
    Paradigm* p5 = new Paradigm {"Ruthless", Role::MEDIC, Role::RAVAGER, Role::SABOTEUR};
    Paradigm* p6 = new Paradigm {"Cerberus", Role::SENTINEL, Role::COMMANDO, Role::COMMANDO};

    paradigms[0] = p1;
    paradigms[1] = p2;
    paradigms[2] = p3;
    paradigms[3] = p4;
    paradigms[4] = p5;
    paradigms[5] = p6;

    // vertices.push_back({0, 0, staggerBar.colour, staggerBar.x, staggerBar.y, 0});
    // vertices.push_back({0, 0, staggerBar.colour, staggerBar.x + staggerBar.w, staggerBar.y + staggerBar.y, 0});

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

void Menu::setParadigm(){
    activeCharacter->currentRole = paradigms[selectedParadigm]->r1;
    activeCharacter->teamList.at(1)->currentRole = paradigms[selectedParadigm]->r2;
    activeCharacter->teamList.at(2)->currentRole = paradigms[selectedParadigm]->r3;

    for (int i = 0; i < activeCharacter->teamList.size(); i++){
        activeCharacter->teamList.at(i)->addViableBattleCommands();
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
        case TeamMenu:
            selectedIndex = 0;
            optionMax = activeCharacter->teamList.size() - 1;
            break;
        case ItemsMenu:
        case ParadigmMenu:
            selectedIndex = selectedParadigm;
            optionMax = std::size(paradigms) - 1;
            break;
        case TechniquesMenu:
            break;
        case Scan:
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

            activeCharacter->queueCommand(activeCharacter->abilities[selectedIndex]);
            
            if (activeCharacter->atbQueueAmt == activeCharacter->atbSegments){
                if (activeCharacter->currentRole == Role::SYNERGIST || activeCharacter->currentRole == Role::MEDIC){
                    changeMenuState(TeamMenu);
                } else {
                    changeMenuState(EnemyMenu);
                }
            }
            // }

            break;
        case TechniquesMenu:
            break;
        case ItemsMenu:
            break;
        case TeamMenu:
            activeCharacter->setTarget(activeCharacter->teamList.at(selectedIndex));
            activeCharacter->startAttack();
            changeMenuState(CommandMenu);    
            break;        
        case EnemyMenu:
            //set the character's target and enter attack ready state, return to command menu
            activeCharacter->setTarget(activeCharacter->enemyList.at(selectedIndex));
            activeCharacter->startAttack();
            changeMenuState(CommandMenu);
            break;
        case ParadigmMenu:
            selectedParadigm = selectedIndex;
            setParadigm();
            // activeCharacter->currentRole = paradigms[selectedIndex]->r1;
            // activeCharacter->teamList.at(1)->currentRole = paradigms[selectedIndex]->r2;
            // activeCharacter->teamList.at(2)->currentRole = paradigms[selectedIndex]->r3;

            // for (int i = 0; i < activeCharacter->teamList.size(); i++){
            //     activeCharacter->teamList.at(i)->addViableBattleCommands();
            // }

            changeMenuState(CommandMenu);

    }

}

void Menu::backButton(){
    switch (menuState){
        case CommandMenu:
            //cancel queued commands
            activeCharacter->commandQueue.clear();
            activeCharacter->atbQueueAmt = 0;
            activeCharacter->characterState = CharacterState::Waiting;
            break;
        case AbilitiesMenu:
            //delete most recent command or return to command menu if queue is empty
            if (!(activeCharacter->commandQueue.empty())){
                activeCharacter->dequeueCommand();
            } else {
                changeMenuState(CommandMenu);
            }
            break;

        case EnemyMenu:
            //return to ability select
            changeMenuState(AbilitiesMenu);
            break;
        case TeamMenu:
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
                if (activeCharacter->currentRole == Role::SYNERGIST || activeCharacter->currentRole == Role::MEDIC){
                    changeMenuState(TeamMenu);
                } else {
                    changeMenuState(EnemyMenu);
                }
            }
            break;
        case CommandMenu:
            if (activeCharacter->characterState == CharacterState::AttackReady && activeCharacter->currAtbVal >= 1){
                while (activeCharacter->atbQueueAmt > activeCharacter->currAtbVal){
                // for (int i = activeCharacter->commandQueue.size() - 1; i >= (int)activeCharacter->currAtbVal; i--){
                    activeCharacter->dequeueCommand();
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

void Menu::scanButton(){
    if (menuState == Scan){
        changeMenuState(CommandMenu);
    } else {
        changeMenuState(Scan);
    }
}

void Menu::drawPage(int buttonWidth, int buttonHeight, int yButtonAmt, int cascade){

}

void Menu::drawAtb(){

    SubmitRect(&g_queue, 5, 172, activeCharacter->atbSegments * 50, 10, Colours::LIGHTGREY, GraphicsUtils::Layer::UI_3);

    SubmitRect(&g_queue, 5, 174, activeCharacter->currAtbVal * 50, 6, Colours::LIGHTBLUE, GraphicsUtils::Layer::UI_3);
}

//TODO: This is still on old render system (need to refactor UIRender so that it is the one handling render submissions)
void Menu::drawTeamStats(){

    int currX = x + 250;
    int currY = y + 32;


    for (int i = 0; i < activeCharacter->teamList.size(); i++){
        UI::drawHealthBar(currX + 80, currY + 2, 120, 6, activeCharacter->teamList.at(i)->health, activeCharacter->teamList.at(i)->maxHealth);

        SubmitText(&g_queue, activeCharacter->teamList.at(i)->name, currX + 80, currY - 8, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_3);
        SubmitText(&g_queue, roleToString(activeCharacter->teamList.at(i)->currentRole), currX + 150, currY - 8, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_3);
        currY += 15;
        currX += cascadeOffset;
    }
}

void Menu::drawStagger(){
    // vertices.clear();

    if (activeCharacter->target == nullptr){
        return;
    }

    Character* enemy = activeCharacter->target;

    SubmitRect(&g_queue, 230, 5, 200, 12, Colours::LIGHTGREY, GraphicsUtils::Layer::UI_3);

    float barPercent;

    if (enemy->chainDuration != 0){  //protect against FPU exception (div by 0)
        if (!enemy->staggered) barPercent = ((enemy->stagger - 100) / (enemy->staggerPoint - 100)) * ((enemy->chainDuration / enemy->peakChainDuration));
        else barPercent = (enemy->chainDuration / enemy->peakChainDuration);
    } else {
        barPercent = 0;
    }

    if (activeCharacter->target->stagger > 100){
        SubmitRect(&g_queue, 230, 7, barPercent * 200, 8, Colours::STAGGERBAR, GraphicsUtils::Layer::UI_3);
    }

    if (!enemy->staggered){
        snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%.2f / %.2f", enemy->stagger, enemy->staggerPoint);
        SubmitText(&g_queue, UI::textBuffer, 230, 20, 0.5, 0.5, 0xFFFFFFFF, GraphicsUtils::Layer::UI_3);

    } 
    else {
        snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%.2f", enemy->stagger);
        SubmitText(&g_queue, UI::textBuffer, 230, 20, 0.5, 0.5, 0xFFFFFFFF, GraphicsUtils::Layer::UI_3);

    }

    if (enemy->staggered){
    SubmitText(&g_queue, "STAGGERED!!", 320, 20, 0.5, 0.5, 0xFFFFFFFF, GraphicsUtils::Layer::UI_3);
    } 

}

void Menu::drawScan(){
    
}

void Menu::drawMenu(){

    int currX = x;
    int currY = y;

    drawTeamStats();
    drawStagger();

    switch (menuState){
        case CommandMenu:
            drawAtb();
            for (int i = 0; i <= optionMax; i++){
                Colours colour = Colours::LIGHTGREY;
                if (selectedIndex == i) colour = Colours::RED;

                UI::drawButton(currX, currY, buttonWidth, buttonHeight, options[i], colour);
                
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
                if ((activeCharacter->activeDebuffs[Debuff::FOG] && activeCharacter->abilities[i]->fog)
                    || (activeCharacter->activeDebuffs[Debuff::PAIN] && activeCharacter->abilities[i]->pain)
                    || activeCharacter->activeDebuffs[Debuff::DAZE]){
                        if (selectedIndex == i) colour = Colours::MIDGREY;
                        else colour = Colours::DARKGREY;
                    } else {
                        if (selectedIndex == i) colour = Colours::RED;
                    }

                if (activeCharacter->abilities[i] != nullptr){

                    UI::drawButton(currX, currY, buttonWidth, buttonHeight, activeCharacter->abilities[i]->name, colour);

                } 
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

                UI::drawButton(currX, currY, buttonWidth, buttonHeight, activeCharacter->enemyList.at(i)->name, colour);

                currY += buttonHeight + padding;
                currX += cascadeOffset;
            }

            break;
        case TeamMenu:
            drawAtb();
            for (int i = 0; i <= optionMax; i++){
                if (i != 0 && i % 4 == 0){
                    currX -= cascadeOffset*4;
                    currX += buttonWidth + padding;
                    currY = y;
                }
                Colours colour = Colours::LIGHTGREY;
                if (selectedIndex == i) colour = Colours::RED;

                UI::drawButton(currX, currY, buttonWidth, buttonHeight, activeCharacter->teamList.at(i)->name, colour);
                
                currY += buttonHeight + padding;
                currX += cascadeOffset;
            }

            break;
            
        case ParadigmMenu:
            currY -= 30;
            for (int i = 0; i <= optionMax; i++){

                Colours colour = Colours::LIGHTGREY;
                if (selectedIndex == i) colour = Colours::RED;

                SubmitRect(&g_queue, currX, currY, buttonWidth * 1.5, buttonHeight * 0.9, colour, GraphicsUtils::Layer::UI_3);
                
                snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s \t %s %s %s", paradigms[i]->name, roleToString(paradigms[i]->r1), roleToString(paradigms[i]->r2), roleToString(paradigms[i]->r3));
                SubmitText(&g_queue, UI::textBuffer, currX + 5, currY, 0.35, 0.35, 0xFFFFFFFF, GraphicsUtils::Layer::UI_3);

                currY += (buttonHeight * 0.9) + padding;
            }

            break;
    }

    //draw abilities in queue
    int atbSpacing = 0;
    for (int i = 0; i < activeCharacter->commandQueue.size(); i++){

        SubmitText(&g_queue, activeCharacter->commandQueue.at(i)->name, 5 + atbSpacing, 160, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_3);
        atbSpacing += 50 * activeCharacter->commandQueue.at(i)->cost;              
    }

    //draw scan
    if (menuState == Scan){
        drawScan();
    }


}
