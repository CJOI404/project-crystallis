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
#include "CommandRegistry.h"
#include "GlobalDefs.h"
#include "CombatInstance.h"
#include "MainMenu.h"

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

void * fbp0;
void * fbp1;

void initGu(){
    // sceGuInit();
    
    // //Set up buffers
    // sceGuStart(GU_DIRECT, list);
    // sceGuDrawBuffer(GU_PSM_8888,(void*)0,BUFFER_WIDTH);
    // sceGuDispBuffer(SCREEN_WIDTH,SCREEN_HEIGHT,(void*)0x88000,BUFFER_WIDTH);
    // sceGuDepthBuffer((void*)0x110000,BUFFER_WIDTH);

    // //Set up viewport
    // sceGuOffset(2048 - (SCREEN_WIDTH / 2), 2048 - (SCREEN_HEIGHT / 2));
    // sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
    // sceGuEnable(GU_SCISSOR_TEST);
    // sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // //Set some stuff
    // sceGuDepthRange(65535, 0); //Use the full buffer for depth testing - buffer is reversed order

    // sceGuDepthFunc(GU_GEQUAL); //Depth buffer is reversed, so GEQUAL instead of LEQUAL
    // sceGuEnable(GU_DEPTH_TEST); //Enable depth testing

    // sceGuFinish();
    // sceGuDisplay(GU_TRUE);


    sceGuInit();

    fbp0 = guGetStaticVramBuffer(BUFFER_WIDTH, BUFFER_HEIGHT, GU_PSM_8888);
    fbp1 = guGetStaticVramBuffer(BUFFER_WIDTH, BUFFER_HEIGHT, GU_PSM_8888);

    //Set up buffers
    sceGuStart(GU_DIRECT, list);
    sceGuDrawBuffer(GU_PSM_8888, fbp0, BUFFER_WIDTH);
    sceGuDispBuffer(SCREEN_WIDTH,SCREEN_HEIGHT,fbp1, BUFFER_WIDTH);
    
    // We do not care about the depth buffer in this example
    sceGuDepthBuffer(fbp0, 0); // Set depth buffer to a length of 0
    sceGuDisable(GU_DEPTH_TEST); // Disable depth testing

    //Set up viewport
    sceGuOffset(2048 - (SCREEN_WIDTH / 2), 2048 - (SCREEN_HEIGHT / 2));
    sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

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
    sceGuClear(GU_COLOR_BUFFER_BIT);
}

void endFrame(){
    sceGuFinish();
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
}

// typedef struct {
//     unsigned short u, v;
//     short x, y, z;
// } Vertex;

// typedef struct
// {
//     float u, v;
//     uint32_t colour;
//     float x, y, z;
// } TextureVertex;

// typedef struct
// {
//     int width, height;
//     uint32_t* data;
// } Texture;

// Texture* loadTexture(const char * filename) {
//     Texture* texture = (Texture *) calloc(1, sizeof(Texture));

//     texture->data = (uint32_t *) stbi_load(filename, &(texture->width), &(texture->height), NULL, STBI_rgb_alpha);

//     // Make sure the texture cache is reloaded
//     sceKernelDcacheWritebackInvalidateAll();

//     return texture;
// }

// void drawTexture(Texture * texture, float x, float y, float w, float h) {
//     static TextureVertex vertices[2];

//     vertices[0].u = 0.0f;
//     vertices[0].v = 0.0f;
//     vertices[0].colour = 0xFFFFFFFF;
//     vertices[0].x = x;
//     vertices[0].y = y;
//     vertices[0].z = 0.0f;

//     vertices[1].u = w;
//     vertices[1].v = h;
//     vertices[1].colour = 0xFFFFFFFF;
//     vertices[1].x = x + w;
//     vertices[1].y = y + h;
//     vertices[1].z = 0.0f;

//     sceGuTexMode(GU_PSM_8888, 0, 0, GU_FALSE);
//     sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
//     sceGuTexImage(0, texture->width, texture->height, texture->width, texture->data);

//     //this is needed to allow transparency, dunno why
//     sceGuEnable(GU_BLEND);
//     sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

//     sceGuEnable(GU_TEXTURE_2D); 
//     sceGuDrawArray(GU_SPRITES, GU_COLOR_8888 | GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);
//     sceGuDisable(GU_TEXTURE_2D);
// }

// void drawRect(float x, float y, float w, float h, unsigned int colour) {

//     Vertex* vertices = (Vertex*)sceGuGetMemory(2 * sizeof(Vertex));

//     vertices[0].x = x;
//     vertices[0].y = y;

//     vertices[1].x = x + w;
//     vertices[1].y = y + h;

//     if (colour == Colours::RED){
//         sceGuColor(0xFF0000FF); // Red, colors are ABGR
//     } else if (colour == Colours::BLUE){
//         sceGuColor(0xFFFF0000); // Blue, colors are ABGR
//     } else {
//         sceGuColor(0xFF000000); // Black, colors are ABGR
//     }
//     sceGuDrawArray(GU_SPRITES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 2, 0, vertices);
// }

// enum GameState {
//     BATTLE,
//     SCAN
// };

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
    UI::loadFont("PSPGameFont.fnt", "PSPGameFont.png");

    //Load skills
    Commands::loadSkills("project_crystallis_skill_sheet.csv");

    // InputHandler playerInput;
    InputHandler::initializeInputHandler();

    // Character playerCharacter;
    // playerCharacter.health = 300;
    // playerCharacter.maxHealth = 2000;
    // playerCharacter.name = "LIGHTNING";
    // playerCharacter.currentRole = Role::COMMANDO;

    // Character character2;
    // character2.health = 2200;
    // character2.maxHealth = 3000;
    // character2.xPos = 100;
    // character2.yPos = 100;
    // character2.name = "SAZH";
    // character2.currentRole = Role::RAVAGER;

    // Character character3;
    // character3.health = 1700;
    // character3.maxHealth = 1700;
    // character3.xPos = 150;
    // character3.yPos = 150;
    // character3.name = "VANILLE";
    // character3.currentRole = Role::SABOTEUR;




    // Character enemy;
    // enemy.moveComp->color = Colours::BLUE;
    // enemy.xPos = 350;
    // enemy.yPos = 70;
    // enemy.name = "ENEMY 1";
    // enemy.health = 450000;
    // enemy.maxHealth = 450000;
    // enemy.staggerPoint = 600;
    // enemy.drawHealthBar = true;

    // Character enemy2;
    // enemy2.moveComp->color = Colours::BLUE;
    // enemy2.xPos = 400;
    // enemy2.yPos = 120;
    // enemy2.name = "ENEMY 2";
    // enemy2.health = 450000;
    // enemy2.maxHealth = 450000;
    // enemy2.staggerPoint = 600;
    // enemy2.drawHealthBar = true;

    // std::vector<Character*> team;
    // team.push_back(&playerCharacter);
    // team.push_back(&character2);
    // team.push_back(&character3);

    // std::vector<Character*> enemies;
    // enemies.push_back(&enemy);
    // enemies.push_back(&enemy2);

    // for (int i = 0; i < team.size(); i++){
    //     team.at(i)->teamList = team;
    //     team.at(i)->enemyList = enemies;
    // }
    // for (int i = 0; i < enemies.size(); i++){
    //     enemies.at(i)->teamList = enemies;
    //     enemies.at(i)->enemyList = team;
    // }

    float xMoveDir = 0;
    int xPos = 50;
    int yPos = 140;
    const int GROUND_LEVEL = 200;

    clock_t prevTime = 0;
    clock_t currTime = 0;
    float deltaTime = 0;

    //Fill character ability lists
    // for (int i = 0; i < 10 && i < Commands::commandList.size(); i++){
    //     playerCharacter.addBattleCommand(Commands::commandList.at(i), i);
    // }
    // playerCharacter.addViableBattleCommands();

    //start menu
    // Menu commandMenu;

    // commandMenu.setActiveCharacter(&playerCharacter);

    //Currently the activecharacter needs to be known for setparadigm to work
    //this is functional but not a particularly good design. Will probably (definitely) make a teammanager class of some sort to handle this but not rn lol
    // commandMenu.setParadigm();

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


    while(running){

        //UPDATE DELTATIME
        currTime = clock();
        deltaTime = float(currTime - prevTime) / CLOCKS_PER_SEC;
        prevTime = currTime;

        //GET INPUTS
        // playerInput.readInput();
        InputHandler::readInput();

        startFrame();

        if (state == MAINMENU){
            mainMenu.update(deltaTime);
            mainMenu.render(deltaTime);

            if (mainMenu.startFlag){

                //Initialize characters
                team.clear();
                enemies.clear();

                // Character playerCharacter;
                playerCharacter.health = 300;
                playerCharacter.maxHealth = 2000;
                playerCharacter.name = "LIGHTNING";
                playerCharacter.currentRole = Role::COMMANDO;

                // Character character2;
                character2.health = 2200;
                character2.maxHealth = 3000;
                character2.xPos = 100;
                character2.yPos = 100;
                character2.name = "SAZH";
                character2.currentRole = Role::RAVAGER;
 
                // Character character3;
                character3.health = 1700;
                character3.maxHealth = 1700;
                character3.xPos = 150;
                character3.yPos = 150;
                character3.name = "VANILLE";
                character3.currentRole = Role::SABOTEUR;

                // Character enemy;
                enemy.moveComp->color = Colours::BLUE;
                enemy.xPos = 350;
                enemy.yPos = 70;
                enemy.name = "ENEMY 1";
                enemy.health = 450000;
                enemy.maxHealth = 450000;
                enemy.staggerPoint = 600;
                enemy.drawHealthBar = true;

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