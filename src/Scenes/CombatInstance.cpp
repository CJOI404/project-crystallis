#include "Scenes/CombatInstance.h"
#include "Entities/Menu.h"
#include "graphics/AssetManagers/UIRender.h"
#include "graphics/RenderState.h"
#include "graphics/GraphicsUtils.h"
#include "InputHandler.h"
#include "graphics/AssetManagers/MeshManager.h"
#include "graphics/AssetManagers/SpriteManager.h"
#include <pspgum.h>
#include "Scenes/SceneManager.h"
#include "Scenes/MainMenu.h"


CombatInstance::CombatInstance(){

    background = TextureManager::load("background.png", 512, 512);
    testlogo = TextureManager::load("logo256.png", 256, 256);

    Texture* spritesheet = TextureManager::load("testspritesheet.png", 256, 128);
    Mesh* myModel = MeshManager::loadOBJ("Lightning/lightning.obj");
    Texture* lightningTex = TextureManager::load("Lightning/Lightning_01.png", 256, 256);

    SpriteManager::registerSprite("topleft", spritesheet, 0, 0, 64, 64);
    SpriteManager::registerSprite("topright", spritesheet, 64, 0, 64, 64);
    SpriteManager::registerSprite("bottomleft", spritesheet, 0, 64, 64, 64);
    SpriteManager::registerSprite("bottomright", spritesheet, 64, 64, 64, 64);


    //Make teams
    // std::vector<Character*> team;
    // std::vector<Character*> enemies;

    // Character playerCharacter;
    // Character character2;
    // Character character3;

    // Character enemy;
    // Character enemy2;


    //Initialize characters
    team.clear();
    enemies.clear();

    // Character playerCharacter;
    playerCharacter.health = 300;
    playerCharacter.maxHealth = 2000;
    playerCharacter.name = "LIGHTNING";
    playerCharacter.currentRole = Role::COMMANDO;
    playerCharacter.sprite = SpriteManager::getSprite("topleft");
    playerCharacter.mesh = myModel;
    playerCharacter.meshTexture = lightningTex;

    // Character character2;
    character2.health = 2200;
    character2.maxHealth = 3000;
    character2.xPos = 100;
    character2.yPos = 100;
    character2.name = "SAZH";
    character2.currentRole = Role::RAVAGER;
    character2.sprite = SpriteManager::getSprite("bottomright");
    

    // Character character3;
    character3.health = 1700;
    character3.maxHealth = 1700;
    character3.xPos = 150;
    character3.yPos = 150;
    character3.name = "VANILLE";
    character3.currentRole = Role::SABOTEUR;
    character3.sprite = SpriteManager::getSprite("bottomleft");

    // Character enemy;
    enemy.moveComp->color = Colours::BLUE;
    enemy.xPos = 120;
    enemy.yPos = 100;
    enemy.name = "ENEMY 1";
    enemy.health = 45000;
    enemy.maxHealth = 45000;
    enemy.staggerPoint = 250;
    enemy.drawHealthBar = true;
    enemy.sprite = SpriteManager::getSprite("topright");
    enemy.mesh = myModel;
    enemy.meshTexture = lightningTex;

    // Character enemy2;
    enemy2.moveComp->color = Colours::BLUE;
    enemy2.xPos = 400;
    enemy2.yPos = 120;
    enemy2.name = "ENEMY 2";
    enemy2.health = 450000;
    enemy2.maxHealth = 450000;
    enemy2.staggerPoint = 600;
    enemy2.drawHealthBar = true;

    team.push_back(&playerCharacter);
    team.push_back(&character2);
    team.push_back(&character3);

    enemies.push_back(&enemy);
    enemies.push_back(&enemy2);

    for (int i = 0; i < team.size(); i++){
        team.at(i)->teamList = team;
        team.at(i)->enemyList = enemies;
    }
    for (int i = 0; i < enemies.size(); i++){
        enemies.at(i)->teamList = enemies;
        enemies.at(i)->enemyList = team;
    }

    // setTeam(team);
    // setEnemies(enemies);


    //Fill ability list
    playerCharacter.addViableBattleCommands();


    complete = false;
    state = CombatState::BATTLE;

    commandMenu.setActiveCharacter(&playerCharacter);
    commandMenu.setParadigm();

    scannedEnemy = this->enemies[0];
    scanIdx = 0;


}


void CombatInstance::setEnemies(std::vector<Character*> enemies){
    this->enemies = enemies;
}

void CombatInstance::setTeam(std::vector<Character*> team){
    this->team = team;
    playerCharacter = *team.at(0);
}

void CombatInstance::update(float dt){

    if (state == CombatState::BATTLE){

        playerCharacter.moveComp->setAnalogueMoveVals(InputHandler::analogueX, InputHandler::analogueY);

        if (InputHandler::getButtonDown(PSP_CTRL_START)){
            SceneManager::setNextScene(new MainMenu());
        }
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
                if (enemies[i] == playerCharacter.target){
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

        enemies[scanIdx]->worldPos = {0.0f, -80.0f, -90.0f};
        
        if (InputHandler::getButtonDown(PSP_CTRL_RTRIGGER) || InputHandler::getButtonDown(PSP_CTRL_CIRCLE)){
            state = CombatState::BATTLE;
        }

        if (InputHandler::getButtonDown(PSP_CTRL_LEFT)) scanIdx--;
        if (InputHandler::getButtonDown(PSP_CTRL_RIGHT)) scanIdx++;

        if (scanIdx >= (int)enemies.size()) scanIdx = 0;
        if (scanIdx < 0) scanIdx = (int)enemies.size() - 1;

        scannedEnemy = enemies[scanIdx];



        // scannedEnemy->worldPos = {0.0f, 0.0f, -5.0f};

    }
    
}

void CombatInstance::render(float dt){

    //Background render
    RenderState::setDepthState(DEPTH_DISABLED);
    RenderState::setBlendMode(BLEND_NONE);
    
    background->draw(0, 0, 512, 512, GraphicsUtils::ColourRGBA(255, 255, 200, 1.0f));


    //Opaque 3d render
    // RenderState::setDepthState(DepthState::DEPTH_READ_WRITE);
    // RenderState::setBlendMode(BLEND_NONE);
    //     for (int i = 0; i < enemies.size(); i++){
    //     enemies[i]->render(dt);
    // }
    // for (int i = 0; i < team.size(); i++){
    //     team[i]->render(dt);
    // }


    //translucent 3d render

    //2d UI render
    // RenderState::setDepthState(DEPTH_DISABLED);
    // RenderState::setBlendMode(BLEND_ALPHA);
    // testlogo->draw(100, 100, 100, 100, GraphicsUtils::ColourRGBA(255, 255, 255, 0.5f));

    // RenderState::setDepthState(DepthState::DEPTH_READ_WRITE);
    //RENDER
    for (int i = 0; i < team.size(); i++){
        ScePspFMatrix4 viewProjMatrix = GraphicsUtils::getViewProjectionMatrix();
        team[i]->screenPos = GraphicsUtils::worldToScreen(team[i]->worldPos, viewProjMatrix);
        team[i]->render(dt);
    }

    for (int i = 0; i < enemies.size(); i++){
        ScePspFMatrix4 viewProjMatrix = GraphicsUtils::getViewProjectionMatrix();
        enemies[i]->screenPos = GraphicsUtils::worldToScreen(enemies[i]->worldPos, viewProjMatrix);
        enemies[i]->render(dt);
    }

    // RenderState::setDepthState(DepthState::DEPTH_DISABLED);

    commandMenu.drawMenu();

    if (state == CombatState::SCAN){

        UI::drawRect(0, 0, 480, 272, 0xdc000000);


        // snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s", scannedEnemy->name);
        UI::drawString(20, 20, 0xFFFFFFFF, 0.6, 0.6, scannedEnemy->name);

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


void CombatInstance::unload(){
    TextureManager::unload("Lightning/Lightning_01.png");

    TextureManager::unload("logo256.png");
    TextureManager::unload("background.png");

    TextureManager::unload("testspritesheet.png");
    SpriteManager::unload("topleft.png");
    SpriteManager::unload("topright.png");
    SpriteManager::unload("bottomleft.png");
    SpriteManager::unload("bottomright.png");
}