#include "Scenes/CombatInstance.h"
#include "Entities/Menu.h"
#include "graphics/AssetManagers/UIRender.h"
#include "graphics/Pipeline/RenderState.h"
#include "graphics/GraphicsUtils.h"
#include "InputHandler.h"
#include "graphics/AssetManagers/MeshManager.h"
#include "graphics/AssetManagers/SpriteManager.h"
#include <pspgum.h>
#include "Scenes/SceneManager.h"
#include "Scenes/MainMenu.h"
#include "graphics/Pipeline/RenderQueue.h"


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
    playerCharacter.setworldPos({100.0f, 0.0f, -140.0f});
    playerCharacter.health = 300;
    playerCharacter.maxHealth = 2000;
    playerCharacter.name = "LIGHTNING";
    playerCharacter.currentRole = Role::COMMANDO;
    // playerCharacter.sprite = SpriteManager::getSprite("topleft");
    playerCharacter.drawName = true;
    playerCharacter.drawHealthBar = true;
    playerCharacter.mesh = myModel;
    playerCharacter.meshTexture = lightningTex;

    // Character character2;
    character2.health = 2200;
    character2.maxHealth = 3000;
    character2.setworldPos({100.0f, 0.0f, -200.0f});
    character2.name = "SAZH";
    character2.currentRole = Role::RAVAGER;
    character2.sprite = SpriteManager::getSprite("bottomright");
    

    // Character character3;
    character3.health = 1700;
    character3.maxHealth = 1700;
    character2.setworldPos({150.0f, 0.0f, -200.0f});
    character3.name = "VANILLE";
    character3.currentRole = Role::SABOTEUR;
    character3.sprite = SpriteManager::getSprite("bottomleft");

    // Character enemy;
    // enemy.moveComp->color = Colours::BLUE;
    enemy.setworldPos({5.0f, 0.0f, -250.0f});
    enemy.name = "ENEMY 1";
    enemy.health = 45000;
    enemy.maxHealth = 45000;
    enemy.staggerPoint = 250;
    enemy.drawHealthBar = true;
    enemy.sprite = SpriteManager::getSprite("topright");
    playerCharacter.drawName = true;
    enemy.mesh = myModel;
    enemy.meshTexture = lightningTex;

    // Character enemy2;
    // enemy2.moveComp->color = Colours::BLUE;
    enemy2.setworldPos({400.0f, 0.0f, -200.0f});
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


    // camera.position = {0.0f, 100.0f, 0.0f};
    camera.target = playerCharacter.worldPos;
    camera.setPos({0.0f, playerCharacter.worldPos.y + 70, 0.0f});


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

        playerCharacter.updateMovement(InputHandler::analogueX, InputHandler::analogueY, dt);

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
        if (InputHandler::gamePad.Buttons & PSP_CTRL_SQUARE){
            camera.position.x -= 15;
        }
        if (InputHandler::gamePad.Buttons & PSP_CTRL_CIRCLE){
            camera.position.x += 15;

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

        // enemies[scanIdx]->worldPos = {0.0f, -80.0f, -90.0f};
        
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
    

    // camera.setTarget({playerCharacter.worldPos.x, playerCharacter.worldPos.y, playerCharacter.worldPos.z});
    camera.update();

}

void CombatInstance::render(float dt){

    //Background render
    RenderState::setDepthState(DEPTH_DISABLED);
    RenderState::setBlendMode(BLEND_NONE);
    background->draw(0, 0, 512, 512, GraphicsUtils::ColourRGBA(255, 255, 200, 1.0f));


    //RENDER
    for (int i = 0; i < team.size(); i++){
        ScePspFMatrix4 viewProjMatrix = GraphicsUtils::getViewProjectionMatrix();
        team[i]->screenPos = GraphicsUtils::worldToScreen(team[i]->uiPos, viewProjMatrix);
        team[i]->render(dt);
    }

    for (int i = 0; i < enemies.size(); i++){
        ScePspFMatrix4 viewProjMatrix = GraphicsUtils::getViewProjectionMatrix();
        enemies[i]->screenPos = GraphicsUtils::worldToScreen(enemies[i]->uiPos, viewProjMatrix);
        enemies[i]->render(dt);
    }

    // RenderState::setDepthState(DepthState::DEPTH_DISABLED);

    commandMenu.drawMenu();

    if (state == CombatState::SCAN){

        SubmitRect(&g_queue, 0, 0, 480, 272, 0xdc000000, GraphicsUtils::Layer::UI_4);
        // SubmitText(&g_queue, scannedEnemy->health, 50, 40, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4); 

        snprintf(UI::textBuffer, sizeof(UI::textBuffer), "HEALTH: %d", scannedEnemy->health);
        SubmitText(&g_queue, UI::textBuffer, 50, 40, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4);
        SubmitText(&g_queue, scannedEnemy->name, 20, 20, 0.6, 0.6, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4);

        int tempy = 60;
        for (int i = 0; i < Debuff::DEBUFFCOUNT; i++){
            if (scannedEnemy->activeDebuffs[i]){

                snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s: %.2f", debuffToString(static_cast<Debuff>(i)), scannedEnemy->debuffDurations[i]);
                SubmitText(&g_queue, UI::textBuffer, 50, tempy, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4); 
                tempy += 10;                                
            }
        }
        tempy = 60;
        for (int i = 0; i < Buff::BUFFCOUNT; i++){
            if (scannedEnemy->activeBuffs[i]){

                snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%s: %.2f", buffToString(static_cast<Buff>(i)), scannedEnemy->buffDurations[i]);  
                SubmitText(&g_queue, UI::textBuffer, 150, tempy, 0.3, 0.3, 0xFFFFFFFF, GraphicsUtils::Layer::UI_4);
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

    MeshManager::unload("Lightning/lightning.obj");
}