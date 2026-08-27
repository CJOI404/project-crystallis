#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "InputHandler.h"
#include "GameActor.h"
#include "Character.h"
#include <ctime>
#include <string>
#include "graphics/UIRender.h"
#include "graphics/SpriteManager.h"
#include "CommandRegistry.h"
#include "GlobalDefs.h"
#include "CombatInstance.h"
#include "MainMenu.h"
#include "graphics/MeshManager.h"
#include "graphics/RenderState.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <Menu.h>
#include <iomanip>

PSP_MODULE_INFO("Project Crystallis", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
// PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

#define BUFFER_WIDTH 512
#define BUFFER_HEIGHT 272
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT BUFFER_HEIGHT

char list[0x20000] __attribute__((aligned(64)));
int running;

int exit_callback(int arg1, int arg2, void *common) {
    running = 0;
    return 0;
}

int callback_thread(SceSize args, void *argp) {
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

int setup_callbacks(void) {
    int thid = sceKernelCreateThread("update_thread", callback_thread, 0x11, 0xFA0, 0, 0);
    if(thid >= 0)
        sceKernelStartThread(thid, 0, 0);
    return thid;
}

//framebuffers
void * fbp0;
void * fbp1;
//depthbuffer
void * zbp;

void initGu(){

    sceGuInit();

    fbp0 = guGetStaticVramBuffer(BUFFER_WIDTH, BUFFER_HEIGHT, GU_PSM_8888);
    fbp1 = guGetStaticVramBuffer(BUFFER_WIDTH, BUFFER_HEIGHT, GU_PSM_8888);
    zbp = guGetStaticVramBuffer(BUFFER_WIDTH, BUFFER_HEIGHT, GU_PSM_8888);

    //Set up buffers
    sceGuStart(GU_DIRECT, list);
    sceGuDrawBuffer(GU_PSM_8888, fbp0, BUFFER_WIDTH);
    sceGuDispBuffer(SCREEN_WIDTH,SCREEN_HEIGHT,fbp1, BUFFER_WIDTH);

    sceGuDepthBuffer(zbp, BUFFER_WIDTH); // Set depth buffer to a length of 0
    sceGuEnable(GU_DEPTH_TEST); //enable depth testing
    sceGuDepthFunc(GU_LEQUAL);
    sceGuDepthRange(65535, 0);
    // sceGuDisable(GU_DEPTH_TEST); // Disable depth testing

    //Set up viewport
    sceGuOffset(2048 - (SCREEN_WIDTH / 2), 2048 - (SCREEN_HEIGHT / 2));
    sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


    // NEW WE WEREWR Projection Matrix Setup (Run once or on resize)
    sceGumMatrixMode(GU_PROJECTION);
    sceGumLoadIdentity();
    sceGumPerspective(75.0f, 480.0f / 272.0f, 0.5f, 1000.0f); // Field of view, Aspect ratio, Near, Far



    // Start a new frame and enable the display
    sceGuFinish();
    sceGuDisplay(GU_TRUE);
}

void endGu(){
    sceGuDisplay(GU_FALSE);
    sceGuTerm();
}

void startFrame(){
    sceGuStart(GU_DIRECT, list);
    sceGuClearColor(0xFF000000); // Black background
    sceGuClearDepth(0);
    sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
    // sceGuClear(GU_COLOR_BUFFER_BIT);
    // sceGuClearDepth();

    RenderState::resetCache();

                // View Matrix Setup (Run in your frame update)
        sceGumMatrixMode(GU_VIEW);
        sceGumLoadIdentity();
        ScePspFVector3 pos = { 0.0f, 0.0f, -5.0f }; // Back up 5 units from origin
        sceGumTranslate(&pos);
}

void endFrame(){
    sceGuFinish();
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
}


enum GameState {
    MAINMENU,
    COMBAT
};

int main() {

    GameState state = GameState::MAINMENU;

    // SceCtrlData pad;

    // sceCtrlSetSamplingCycle(0);
    // sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
    
    // pspDebugScreenInit();

    // Make exiting with the home button possible
    setup_callbacks();

    // Texture* texture = loadTexture("PSPGameFont.png");

    // Setup the library used for rendering
    initGu();
    
        //Load font data
    UI::loadFont("PSPGameFont128.fnt", "PSPGameFont128.png");

    //Load skills
    Commands::loadSkills("project_crystallis_skill_sheet.csv");

    // InputHandler playerInput;
    InputHandler::initializeInputHandler();


    float xMoveDir = 0;
    int xPos = 50;
    int yPos = 140;
    const int GROUND_LEVEL = 200;

    clock_t prevTime = 0;
    clock_t currTime = 0;
    float deltaTime = 0;

    running = 1;
    
    //writes to fixed text buffer
    // snprintf(UI::textBuffer, sizeof(UI::textBuffer), "Lightning Health: %d", playerCharacter.health);
    // CombatInstance testInstance(team, enemies);
    // testInstance.setEnemies(enemies);
    // testInstance.setTeam(team);

    MainMenu mainMenu;
    CombatInstance* combatInstance = nullptr;


    std::vector<Character*> team;
    std::vector<Character*> enemies;

    Character playerCharacter;
    Character character2;
    Character character3;

    Character enemy;
    Character enemy2;

    float fpsTimer = 0.0f;
    int frameCount = 0;


    while(running){

        //UPDATE DELTATIME
        currTime = clock();
        deltaTime = float(currTime - prevTime) / CLOCKS_PER_SEC;
        prevTime = currTime;

        //print fps
        fpsTimer += deltaTime;
        frameCount++;

        if (fpsTimer >= 1.0f)
        {
            printf("FPS: %d\n", frameCount);
            fflush(stdout);

            frameCount = 0;
            fpsTimer = 0.0f;
        }

        //GET INPUTS
        // playerInput.readInput();
        InputHandler::readInput();

        startFrame();

        if (state == MAINMENU){
            mainMenu.update(deltaTime);
            mainMenu.render(deltaTime);

            if (mainMenu.startFlag){

                Texture* spritesheet = TextureManager::load("testspritesheet.png", 256, 128);
                Mesh* myModel = MeshManager::loadOBJ("lightning.obj");

                // printf("vertices: %s", myModel->vertices);
                // printf("vertex count: %d", myModel->vertexCount);
                // printf("vertices: %s", myModel->vertices);

                SpriteManager::registerSprite("topleft", spritesheet, 0, 0, 64, 64);
                SpriteManager::registerSprite("topright", spritesheet, 64, 0, 64, 64);
                SpriteManager::registerSprite("bottomleft", spritesheet, 0, 64, 64, 64);
                SpriteManager::registerSprite("bottomright", spritesheet, 64, 64, 64, 64);

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
                playerCharacter.meshTexture = TextureManager::load("Lightning_01.png", 256, 256);

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
                enemy.xPos = 350;
                enemy.yPos = 70;
                enemy.name = "ENEMY 1";
                enemy.health = 45000;
                enemy.maxHealth = 45000;
                enemy.staggerPoint = 250;
                enemy.drawHealthBar = true;
                enemy.sprite = SpriteManager::getSprite("topright");

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

                //Fill ability list
                playerCharacter.addViableBattleCommands();

                //Create instance
                combatInstance = new CombatInstance(team, enemies);

                
                state = COMBAT;
            }

        } else if (state == COMBAT){

            combatInstance->update(deltaTime);
            combatInstance->render(deltaTime);


            if (InputHandler::getButtonDown(PSP_CTRL_START)){
                mainMenu.startFlag = false;
                delete combatInstance;
                state = MAINMENU;
            }

        }

 

        endFrame();

    }

    delete combatInstance;

    return 0;
}