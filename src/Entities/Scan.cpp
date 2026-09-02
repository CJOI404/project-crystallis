#include "Entities/Scan.h"
#include "InputHandler.h"
#include "Scenes/CombatInstance.h"
#include "Graphics/Pipeline/RenderQueue.h"

void Scan::update(float dt){


    if (InputHandler::getButtonDown(PSP_CTRL_LEFT)) scanIdx--;
    if (InputHandler::getButtonDown(PSP_CTRL_RIGHT)) scanIdx++;

    if (InputHandler::getButtonDown(PSP_CTRL_SQUARE)){
        scanTeam = !scanTeam;
    }

    if (scanTeam){
        if (scanIdx >= (int)teamList.size()) scanIdx = 0;
        if (scanIdx < 0) scanIdx = (int)teamList.size() - 1;

        scannedCharacter = teamList[scanIdx];
    } else {
        if (scanIdx >= (int)enemyList.size()) scanIdx = 0;
        if (scanIdx < 0) scanIdx = (int)enemyList.size() - 1;

        scannedCharacter = enemyList[scanIdx];
    }

    

}

void Scan::render(float dt){
    SubmitRect(&g_queue, 0, 0, 480, 272, 0xdc000000, GraphicsUtils::Layer::UI_4);

    snprintf(UI::textBuffer, sizeof(UI::textBuffer), "HEALTH: %d", scannedCharacter->health);
    SubmitText(&g_queue, UI::textBuffer, 50, 40, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4);
    SubmitText(&g_queue, scannedCharacter->name, 20, 20, 0.6, 0.6, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4);

    //Just a test to see player equipment stuff before I implement the enemy/team scan toggle
    
    if (scanTeam && scannedCharacter->getWeapon() != nullptr){
        SubmitText(&g_queue, scannedCharacter->getWeapon()->name, 280, 20, 0.6, 0.6, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4);
        SubmitText(&g_queue, scannedCharacter->getWeapon()->abilityName, 280, 40, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4);
        snprintf(UI::textBuffer, sizeof(UI::textBuffer), "PHYSICAL: %d", scannedCharacter->getWeapon()->atk);
        SubmitText(&g_queue, UI::textBuffer, 280, 50, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4);
        snprintf(UI::textBuffer, sizeof(UI::textBuffer), "MAGIC: %d", scannedCharacter->getWeapon()->rav);
        SubmitText(&g_queue, UI::textBuffer, 280, 60, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4);

    }



    int tempy = 60;
    for (int i = 0; i < Debuff::DEBUFFCOUNT; i++){
        if (scannedCharacter->activeDebuffs[i]){

            snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s: %.2f", debuffToString(static_cast<Debuff>(i)), scannedCharacter->debuffDurations[i]);
            SubmitText(&g_queue, UI::textBuffer, 50, tempy, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4); 
            tempy += 10;                                
        }
    }
    tempy = 60;
    for (int i = 0; i < Buff::BUFFCOUNT; i++){
        if (scannedCharacter->activeBuffs[i]){

            snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s: %.2f", buffToString(static_cast<Buff>(i)), scannedCharacter->buffDurations[i]);  
            SubmitText(&g_queue, UI::textBuffer, 150, tempy, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4);
            tempy += 10;                                
        }
    }
}