#include "Scenes/CombatInstance.h"
#include "Entities/Menu.h"
#include "graphics/AssetManagers/UIRender.h"
#include "graphics/GraphicsUtils.h"
#include "InputHandler.h"
#include "graphics/AssetManagers/MeshManager.h"
#include "graphics/AssetManagers/SpriteManager.h"
#include <pspgum.h>
#include "Scenes/SceneManager.h"
#include "Scenes/MainMenu.h"
#include "graphics/Pipeline/RenderQueue.h"
#include "ItemRegistry.h"


CombatInstance::CombatInstance(){

    background = TextureManager::load("assets/background.png", 512, 512);
    testlogo = TextureManager::load("assets/logo256.png", 256, 256);

    Texture* spritesheet = TextureManager::load("assets/testspritesheet.png", 256, 128);
    Mesh* myModel = MeshManager::loadOBJ("assets/Lightning/lightning.obj");
    Texture* lightningTex = TextureManager::load("assets/Lightning/Lightning_01.png", 256, 256);
    Mesh* placeholder = MeshManager::loadOBJ("assets/placeholder.obj");

    SpriteManager::registerSprite("topleft", spritesheet, 0, 0, 64, 64);
    SpriteManager::registerSprite("topright", spritesheet, 64, 0, 64, 64);
    SpriteManager::registerSprite("bottomleft", spritesheet, 0, 64, 64, 64);
    SpriteManager::registerSprite("bottomright", spritesheet, 64, 64, 64, 64);


    for (int i = 0; i < Items::weaponList.size(); i++){
        printf("%d: %s, %s, %s, %d, %d\n", i, Items::weaponList[i].name, Items::weaponList[i].character, Items::weaponList[i].ability, Items::weaponList[i].atk, Items::weaponList[i].rav);
    }

    //Initialize characters
    team.clear();
    enemies.clear();

    // Character playerCharacter;
    playerCharacter.setworldPos({-100.0f, 0.0f, -140.0f});
    playerCharacter.health = 300;
    playerCharacter.maxHealth = 2000;
    playerCharacter.name = "LIGHTNING";
    playerCharacter.currentRole = Role::COMMANDO;
    playerCharacter.drawName = true;
    playerCharacter.drawHealthBar = true;
    playerCharacter.mesh = myModel;
    playerCharacter.meshTexture = lightningTex;
    team.push_back(&playerCharacter);

    playerCharacter.equippedWeapon = &Items::weaponList[0];

    // Character character2;
    character2.health = 2200;
    character2.maxHealth = 3000;
    character2.setworldPos({-100.0f, 0.0f, -200.0f});
    character2.name = "SAZH";
    character2.currentRole = Role::RAVAGER;
    // character2.sprite = SpriteManager::getSprite("bottomright");
    team.push_back(&character2);
    

    // // Character character3;
    // character3.health = 1700;
    // character3.maxHealth = 1700;
    // character3.setworldPos({-120.0f, 0.0f, -200.0f});
    // character3.name = "VANILLE";
    // character3.currentRole = Role::SABOTEUR;
    // character3.sprite = SpriteManager::getSprite("bottomleft");
    // team.push_back(&character3);
    

    // Character enemy;
    enemy.setworldPos({50.0f, 0.0f, -350.0f});
    enemy.name = "ENEMY 1";
    enemy.health = 45000;
    enemy.maxHealth = 45000;
    enemy.staggerPoint = 250;
    enemy.drawHealthBar = true;
    // enemy.sprite = SpriteManager::getSprite("topright");
    playerCharacter.drawName = true;
    enemy.mesh = placeholder;
    enemies.push_back(&enemy);

    // Character enemy2;
    enemy2.setworldPos({150.0f, 0.0f, -275.0f});
    enemy2.name = "ENEMY 2";
    enemy2.health = 450000;
    enemy2.maxHealth = 450000;
    enemy2.staggerPoint = 600;
    enemy2.drawHealthBar = true;
    enemy2.mesh = placeholder;
    enemies.push_back(&enemy2);

    //Fill ability list
    playerCharacter.addViableBattleCommands();

    complete = false;
    state = CombatState::BATTLE;

    commandMenu.setActiveCharacter(&playerCharacter);
    commandMenu.setParadigm();

    // scannedEnemy = this->enemies[0];
    scanIdx = 0;
    scanScreen.enemyList = enemies;
    // scanScreen.scannedEnemy = &enemy;

    initTeamLists();

    // camera.position = {0.0f, 100.0f, 0.0f};
    camera.target = playerCharacter.worldPos;
    camera.setPos({0.0f, playerCharacter.worldPos.y + 70, 100.0f});


}

void CombatInstance::initTeamLists(){
    commandMenu.teamList = team;
    commandMenu.enemyList = enemies;
    scanScreen.enemyList = enemies;
    scanScreen.teamList = team;
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
        // if (InputHandler::gamePad.Buttons & PSP_CTRL_SQUARE){
        //     camera.position.x -= 15;
        // }
        // if (InputHandler::gamePad.Buttons & PSP_CTRL_TRIANGLE){
        //     camera.position.x += 15;
        // }
    
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

        if (InputHandler::getButtonDown(PSP_CTRL_RTRIGGER) || InputHandler::getButtonDown(PSP_CTRL_CIRCLE)){
            state = CombatState::BATTLE;
        }

        scanScreen.update(dt);

    }
    
    camera.update();

}

void CombatInstance::render(float dt){

    Submit2D(&g_queue, background, 0, 0, 512, 512, GraphicsUtils::ColourRGBA(255, 255, 200, 1.0f), GraphicsUtils::Layer::BACKGROUND_0);

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

    if (state == CombatState::BATTLE){
        commandMenu.drawMenu();
    } else if (state == CombatState::SCAN){
        scanScreen.render(dt);
    }
    
}


void CombatInstance::unload(){
    TextureManager::unload("assets/Lightning/Lightning_01.png");
    TextureManager::unload("assets/logo256.png");
    TextureManager::unload("assets/background.png");

    TextureManager::unload("assets/testspritesheet.png");
    SpriteManager::unload("topleft");
    SpriteManager::unload("topright");
    SpriteManager::unload("bottomleft");
    SpriteManager::unload("bottomright");

    MeshManager::unload("assets/Lightning/lightning.obj");
}