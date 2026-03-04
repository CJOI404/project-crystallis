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
#include "UIRender.h"
#include "CommandRegistry.h"
#include "GlobalDefs.h"

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

enum GameState {
    BATTLE,
    SCAN
};


int main() {

    GameState state = GameState::BATTLE;

    // SceCtrlData pad;

    // sceCtrlSetSamplingCycle(0);
    // sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
    
    // pspDebugScreenInit();

    // Make exiting with the home button possible
    setup_callbacks();

    // Texture* texture = loadTexture("PSPGameFont.png");

    // Setup the library used for rendering
    initGu();

    CommandRegistry commands;

    InputHandler playerInput;
    Character playerCharacter;
    playerCharacter.health = 2000;
    Character enemy;
    enemy.moveComp->color = Colours::BLUE;
    enemy.xPos = 350;
    enemy.yPos = 70;
    enemy.name = "ENEMY 1";
    enemy.health = 450000;
    enemy.maxHealth = 450000;
    enemy.staggerPoint = 600;

    //assign enemies to characters
    playerCharacter.enemyList.push_back(&enemy);
    // playerCharacter.enemyList.push_back(&enemy2);

    float xMoveDir = 0;
    int xPos = 50;
    int yPos = 140;
    const int GROUND_LEVEL = 200;

    clock_t prevTime = 0;
    clock_t currTime = 0;
    float deltaTime = 0;

    //Load font data
    UI::loadFont("PSPGameFont.fnt", "PSPGameFont.png");

    //Load skills
    commands.loadSkills("project_crystallis_skill_sheet.csv");

    //Fill character ability lists
    for (int i = 0; i < 10 && i < commands.commandList.size(); i++){
        playerCharacter.addBattleCommand(commands.commandList.at(i), i);
    }

    //start menu
    Menu commandMenu;

    commandMenu.setActiveCharacter(&playerCharacter);

    running = 1;

    //writes to fixed text buffer
    // snprintf(UI::textBuffer, sizeof(UI::textBuffer), "Lightning Health: %d", playerCharacter.health);


    while(running){


        //UPDATE DELTATIME
        currTime = clock();
        deltaTime = float(currTime - prevTime) / CLOCKS_PER_SEC;
        prevTime = currTime;

        //GET INPUTS
        playerInput.readInput();

        startFrame();

        UI::drawRect(playerCharacter.xPos, playerCharacter.yPos, 20, 20, playerCharacter.moveComp->color);

        UI::drawRect(enemy.xPos, enemy.yPos, 20, 20, enemy.moveComp->color);

        if (state == GameState::BATTLE){
            //DO MOVEMENT
            playerCharacter.moveComp->setAnalogueMoveVals(playerInput.analogueX, playerInput.analogueY);

            if (playerInput.getButtonDown(PSP_CTRL_SQUARE) && playerCharacter.currAtbVal >= 1){
                    playerCharacter.moveComp->dash();
                    playerCharacter.currAtbVal -= 1;
                }
            if (playerInput.getButtonDown(PSP_CTRL_DOWN)){
                    commandMenu.cursorDown();
                }
            if (playerInput.getButtonDown(PSP_CTRL_UP)){
                    commandMenu.cursorUp();
                }
            if (playerInput.getButtonDown(PSP_CTRL_LEFT)){
                    commandMenu.cursorLeft();
                }
            if (playerInput.getButtonDown(PSP_CTRL_RIGHT)){
                    commandMenu.cursorRight();
                }
            
            //handle menu
            if (playerInput.getButtonDown(PSP_CTRL_CROSS)){
                commandMenu.selectButton();
            } else if (playerInput.getButtonDown(PSP_CTRL_CIRCLE)){
                commandMenu.backButton();
            } else if (playerInput.getButtonDown(PSP_CTRL_TRIANGLE)){
                commandMenu.earlyExecuteButton();
            }

            commandMenu.drawMenu();

            snprintf(UI::textBuffer, sizeof(UI::textBuffer), "Lightning Health: %d", playerCharacter.health);
            UI::drawString(300, 200, 0xFFFFFFFF, 0.5, 0.5, UI::textBuffer);

            snprintf(UI::textBuffer, sizeof(UI::textBuffer), "Health: %d", enemy.health);
            UI::drawString(50, 50, 0xFFFFFFFF, 0.5, 0.5, UI::textBuffer);

            //draw stagger
            // std::string eStagger = std::to_string(enemy.stagger);
            // std::string eStaggerPoint = std::to_string(enemy.staggerPoint);

            UI::drawRect(230, 5, 200, 12, Colours::LIGHTGREY);

            float barPercent;
            if (!enemy.staggered) barPercent = ((enemy.stagger - 100) / (enemy.staggerPoint - 100)) * ((enemy.chainDuration / enemy.peakChainDuration));
            else barPercent =  (enemy.chainDuration / enemy.peakChainDuration);

            if (enemy.stagger > 100){
                UI::drawRect(230, 7, barPercent * 200, 8, Colours::STAGGERBAR);
            }

            // snprintf(UI::textBuffer, sizeof(UI::textBuffer), "Health: %.2f", enemy.health);

            // eStagger = eStagger.substr(0, eStagger.find('.') + 3);
            // eStaggerPoint = eStaggerPoint.substr(0, eStaggerPoint.find('.') + 3);
            
            
            if (!enemy.staggered){
                snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%.2f / %.2f", enemy.stagger, enemy.staggerPoint);
                UI::drawString(230, 20, 0xFFFFFFFF, 0.5, 0.5, UI::textBuffer);
            } 
            else {
                snprintf(UI::textBuffer, sizeof(UI::textBuffer), "%.2f", enemy.stagger);
                UI::drawString(230, 20, 0xFFFFFFFF, 0.5, 0.5, UI::textBuffer);
            }

            //draw health bar

            UI::drawRect(enemy.xPos - 60, enemy.yPos - 10, 120, 6, Colours::LIGHTGREY);
            UI::drawRect(enemy.xPos - 60, enemy.yPos - 9, ((float) enemy.health / enemy.maxHealth) * 120, 4, Colours::LIGHTGREEN);

            if ((playerInput.gamePad.Buttons & PSP_CTRL_RTRIGGER) && !(playerInput.oldGamePad.Buttons & PSP_CTRL_RTRIGGER)){
                state = GameState::SCAN;
            }

            //temp draw atb bar
            UI::drawRect(10, 160, playerCharacter.atbSegments * 50, 12, Colours::LIGHTGREY);
            UI::drawRect(10, 162, playerCharacter.currAtbVal * 50, 8, Colours::LIGHTBLUE);

            //temp draw stagger info
            // UI::drawString(250, 40, 0xFFFFFFFF, 0.5, 0.5, "Duration: " + std::to_string(enemy.chainDuration) + "");
            if (enemy.staggered) UI::drawString(320, 20, 0xFFFFFFFF, 0.5, 0.5, "STAGGERED!!");


            //UPDATE ACTORS
            playerCharacter.update(deltaTime);
            enemy.update(deltaTime);
            // enemy2.update(deltaTime);

        } else if (state == GameState::SCAN){
            // std::string playerHealth = "HEALTH: " + std::to_string(playerCharacter.health);
            // std::string fireResistance = "HEALTH: " + std::to_string(playerCharacter.health);

            // UI::drawString(10, 5, 0xFFFFFFFF, 0.8, 0.8, "HEALTH: " + std::to_string(playerCharacter.health) + "");
            // UI::drawString(10, 25, 0xFFFFFFFF, 0.4, 0.4, "RESISTANCES:");
            // UI::drawString(10, 45, 0xFFFFFFFF, 0.3, 0.3, "FIRE: " + std::to_string(playerCharacter.resistances[Element::FIRE]));


            if (playerInput.getButtonDown(PSP_CTRL_RTRIGGER) || playerInput.getButtonDown(PSP_CTRL_CIRCLE)){
                state = GameState::BATTLE;
            }
        }

        endFrame();


        // float xMoveDir = 0;
        // float yMoveDir = 0;
        // float length = 0;
        // int moveSpeed = 5;




        // xMoveDir += (pad.Lx - 128) / 127.0f ; //sets center to 0, left to -128 and right to 128
        // yMoveDir += (pad.Ly - 128) / 127.0f ; //sets center to 0, left to -128 and right to 128

        // //clamp
        // if (xMoveDir > 1) xMoveDir = 1;
        // if (xMoveDir < -1) xMoveDir = -1;
        // if (yMoveDir > 1) yMoveDir = 1;
        // if (yMoveDir < -1) yMoveDir = -1;

        // length = sqrt(xMoveDir * xMoveDir + yMoveDir * yMoveDir);

        // if (length < 0.1){
        //     xMoveDir = 0;
        //     yMoveDir = 0;
        // }

        // printf("length: %.2f", length);
        // printf("XDir: %.2f", playerInput.analogueX);

        // if (length > 0.0f){
        //     xMoveDir /= length;
        //     yMoveDir /= length;
        // }

        // pspDebugScreenPrintf("xMoveDir: %0.2f", playerInput.analogueX);

        // xPos += xMoveDir * moveSpeed;
        // yPos += yMoveDir * moveSpeed;

        // pspDebugScreenPrintf("\nXpos: %d", xPos);


        // if (xPos > SCREEN_WIDTH - 34) xPos = SCREEN_WIDTH - 34;
        // if (xPos < 0) xPos = 0;

        // drawRect(xPos, yPos, 20, 20);

        // endFrame();

    }

    return 0;
}