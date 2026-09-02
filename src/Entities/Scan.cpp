#include "Entities/Scan.h"
#include "InputHandler.h"
#include "Scenes/CombatInstance.h"
#include "Graphics/Pipeline/RenderQueue.h"

void Scan::update(float dt){

    if (InputHandler::getButtonDown(PSP_CTRL_LEFT)) scanIdx--;
    if (InputHandler::getButtonDown(PSP_CTRL_RIGHT)) scanIdx++;

    if (scanIdx >= (int)enemyList.size()) scanIdx = 0;
    if (scanIdx < 0) scanIdx = (int)enemyList.size() - 1;

}

void Scan::render(float dt){
    SubmitRect(&g_queue, 0, 0, 480, 272, 0xdc000000, GraphicsUtils::Layer::UI_4);

    snprintf(UI::textBuffer, sizeof(UI::textBuffer), "HEALTH: %d", enemyList[scanIdx]->health);
    SubmitText(&g_queue, UI::textBuffer, 50, 40, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4);
    SubmitText(&g_queue, enemyList[scanIdx]->name, 20, 20, 0.6, 0.6, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4);

    int tempy = 60;
    for (int i = 0; i < Debuff::DEBUFFCOUNT; i++){
        if (enemyList[scanIdx]->activeDebuffs[i]){

            snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s: %.2f", debuffToString(static_cast<Debuff>(i)), enemyList[scanIdx]->debuffDurations[i]);
            SubmitText(&g_queue, UI::textBuffer, 50, tempy, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4); 
            tempy += 10;                                
        }
    }
    tempy = 60;
    for (int i = 0; i < Buff::BUFFCOUNT; i++){
        if (enemyList[scanIdx]->activeBuffs[i]){

            snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s: %.2f", buffToString(static_cast<Buff>(i)), enemyList[scanIdx]->buffDurations[i]);  
            SubmitText(&g_queue, UI::textBuffer, 150, tempy, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4);
            tempy += 10;                                
        }
    }
}