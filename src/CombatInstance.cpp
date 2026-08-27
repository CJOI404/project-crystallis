#include "CombatInstance.h"
#include "Menu.h"
#include "graphics/UIRender.h"
#include "InputHandler.h"


CombatInstance::CombatInstance(std::vector<Character*> team, std::vector<Character*> enemies){
    complete = false;
    state = CombatState::BATTLE;

    setEnemies(enemies);
    setTeam(team);

    commandMenu.setActiveCharacter(playerCharacter);
    commandMenu.setParadigm();

    scannedEnemy = this->enemies[0];
    scanIdx = 0;

    background = TextureManager::load("background.png", 512);

}


void CombatInstance::setEnemies(std::vector<Character*> enemies){
    this->enemies = enemies;
}

void CombatInstance::setTeam(std::vector<Character*> team){
    this->team = team;
    playerCharacter = team.at(0);
}

void CombatInstance::update(float dt){
    // Menu commandMenu;

    if (state == CombatState::BATTLE){
        //DO MOVEMENT
        playerCharacter->moveComp->setAnalogueMoveVals(InputHandler::analogueX, InputHandler::analogueY);

        // if (InputHandler::getButtonDown(PSP_CTRL_SQUARE) && playerCharacter->currAtbVal >= 1){
        //     playerCharacter->moveComp->dash();
        //     playerCharacter->currAtbVal -= 1;
        // }

        if (InputHandler::getButtonDown(PSP_CTRL_DOWN)){
            commandMenu.cursorDown();
        }
        if (InputHandler::getButtonDown(PSP_CTRL_UP)){
            commandMenu.cursorUp();
        }
        if (InputHandler::getButtonDown(PSP_CTRL_LEFT)){
            commandMenu.cursorLeft();
        }
        if (InputHandler::getButtonDown(PSP_CTRL_RIGHT)){
            commandMenu.cursorRight();
        }
        
    
        //handle menu
        if (InputHandler::getButtonDown(PSP_CTRL_CROSS)){
            commandMenu.selectButton();
        } else if (InputHandler::getButtonDown(PSP_CTRL_CIRCLE)){
            commandMenu.backButton();
        } else if (InputHandler::getButtonDown(PSP_CTRL_TRIANGLE)){
            commandMenu.earlyExecuteButton();
        } else if (InputHandler::getButtonDown(PSP_CTRL_LTRIGGER)){
            commandMenu.paradigmSwitchButton();
        }


        if (InputHandler::getButtonDown(PSP_CTRL_RTRIGGER)){
            for (int i = 0; i < enemies.size(); i++){
                if (enemies[i] == playerCharacter->target){
                    scanIdx = i;
                }
            }
            state = CombatState::SCAN;
        }
        


        //UPDATE ACTORS
        for (int i = 0; i < team.size(); i++){
            team[i]->update(dt);
        }
        for (int i = 0; i < enemies.size(); i++){
            enemies[i]->update(dt);
        }

    } else if (state == CombatState::SCAN){
        
        if (InputHandler::getButtonDown(PSP_CTRL_RTRIGGER) || InputHandler::getButtonDown(PSP_CTRL_CIRCLE)){
            state = CombatState::BATTLE;
        }

        if (InputHandler::getButtonDown(PSP_CTRL_LEFT)) scanIdx--;
        if (InputHandler::getButtonDown(PSP_CTRL_RIGHT)) scanIdx++;

        if (scanIdx >= (int)enemies.size()) scanIdx = 0;
        if (scanIdx < 0) scanIdx = (int)enemies.size() - 1;

        scannedEnemy = enemies[scanIdx];

        // draw tint
        // UI::drawRect(0, 0, 480, 272, 0xdc000000);

        //HACKY TERRIBLE SCAN SCREEN FOR TESTING
        // snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s", );
        // UI::drawString(10, 5, 0xFFFFFFFF, 0.3, 0.3, UI::textBuffer);

        // snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%S", scannedEnemy->name);
        // UI::drawString(20, 20, 0xFFFFFFFF, 0.6, 0.6, UI::textBuffer);

        // // UI::drawHealthBar(140, 20, 120, 12, scannedEnemy->health, scannedEnemy->maxHealth);

        // snprintf(UI::textBuffer, sizeof(UI::textBuffer), "HEALTH: %d", scannedEnemy->health);
        // UI::drawString(50, 40, 0xFFFFFFFF, 0.3, 0.3, UI::textBuffer);

        // int tempy = 60;
        // for (int i = 0; i < Debuff::DEBUFFCOUNT; i++){
        //     // snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s", );
        //     // UI::drawString(10, 5, 0xFFFFFFFF, 0.3, 0.3, UI::textBuffer);
        //     if (scannedEnemy->activeDebuffs[i]){
        //         // UI::drawString(10, 5, 0xFFFFFFFF, 0.3, 0.3, "TEST DEBUFF IS ON");
        //         snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s: %.2f", debuffToString(static_cast<Debuff>(i)), scannedEnemy->debuffDurations[i]);
        //         UI::drawString(50, tempy, 0xFFFFFFFF, 0.3, 0.3, UI::textBuffer);   
        //         tempy += 10;                                
        //     }
        // }
        // tempy = 60;
        // for (int i = 0; i < Buff::BUFFCOUNT; i++){
        //     // snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s", );
        //     // UI::drawString(10, 5, 0xFFFFFFFF, 0.3, 0.3, UI::textBuffer);
        //     if (scannedEnemy->activeBuffs[i]){
        //         // UI::drawString(10, 5, 0xFFFFFFFF, 0.3, 0.3, "TEST DEBUFF IS ON");
        //         snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s: %.2f", buffToString(static_cast<Buff>(i)), scannedEnemy->buffDurations[i]);
        //         UI::drawString(150, tempy, 0xFFFFFFFF, 0.3, 0.3, UI::textBuffer);   
        //         tempy += 10;                                
        //     }
        // }

        // UI::drawString(10, 5, 0xFFFFFFFF, 0.8, 0.8, "DEBUFFS: " + std::to_string(playerCharacter.health) + "");
        // UI::drawString(10, 25, 0xFFFFFFFF, 0.4, 0.4, "RESISTANCES:");
        // UI::drawString(10, 45, 0xFFFFFFFF, 0.3, 0.3, "FIRE: " + std::to_string(playerCharacter.resistances[Element::FIRE]));



    }
    
}

void CombatInstance::render(float dt){
    background->draw(0, 0, 512, 512);

    //RENDER
    for (int i = 0; i < team.size(); i++){
        team[i]->render(dt);
    }
    for (int i = 0; i < enemies.size(); i++){
        enemies[i]->render(dt);
    }

    commandMenu.drawMenu();

    if (state == CombatState::SCAN){
        UI::drawRect(0, 0, 480, 272, 0xdc000000);

        snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s", scannedEnemy->name);
        UI::drawString(20, 20, 0xFFFFFFFF, 0.6, 0.6, UI::textBuffer);

        snprintf(UI::textBuffer, sizeof(UI::textBuffer), "HEALTH: %d", scannedEnemy->health);
        UI::drawString(50, 40, 0xFFFFFFFF, 0.3, 0.3, UI::textBuffer);

        int tempy = 60;
        for (int i = 0; i < Debuff::DEBUFFCOUNT; i++){
            // snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s", );
            // UI::drawString(10, 5, 0xFFFFFFFF, 0.3, 0.3, UI::textBuffer);
            if (scannedEnemy->activeDebuffs[i]){
                // UI::drawString(10, 5, 0xFFFFFFFF, 0.3, 0.3, "TEST DEBUFF IS ON");
                snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s: %.2f", debuffToString(static_cast<Debuff>(i)), scannedEnemy->debuffDurations[i]);
                UI::drawString(50, tempy, 0xFFFFFFFF, 0.3, 0.3, UI::textBuffer);   
                tempy += 10;                                
            }
        }
        tempy = 60;
        for (int i = 0; i < Buff::BUFFCOUNT; i++){
            // snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s", );
            // UI::drawString(10, 5, 0xFFFFFFFF, 0.3, 0.3, UI::textBuffer);
            if (scannedEnemy->activeBuffs[i]){
                // UI::drawString(10, 5, 0xFFFFFFFF, 0.3, 0.3, "TEST DEBUFF IS ON");
                snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s: %.2f", buffToString(static_cast<Buff>(i)), scannedEnemy->buffDurations[i]);
                UI::drawString(150, tempy, 0xFFFFFFFF, 0.3, 0.3, UI::textBuffer);   
                tempy += 10;                                
            }
        }
    }
}