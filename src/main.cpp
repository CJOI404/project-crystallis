#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "InputHandler.h"
#include "Entities/GameActor.h"
#include "Entities/Character.h"
#include <ctime>
#include <string>
#include "graphics/AssetManagers/UIRender.h"
#include "graphics/AssetManagers/SpriteManager.h"
#include "CommandRegistry.h"
#include "GlobalDefs.h"
#include "Scenes/CombatInstance.h"
#include "Scenes/MainMenu.h"
#include "graphics/AssetManagers/MeshManager.h"
#include "Scenes/SceneManager.h"
#include "graphics/Pipeline/RenderState.h"
#include "graphics/Pipeline/RenderQueue.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <Entities/Menu.h>
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

        // View Matrix Setup
        // sceGumMatrixMode(GU_VIEW);
        // sceGumLoadIdentity();
        // ScePspFVector3 pos = { 0.0f, -90.0f, -5.0f };
        // sceGumTranslate(&pos);
}

void endFrame(){
    sceGuFinish();
    // sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
}


enum GameState {
    MAINMENU,
    COMBAT
};

int main() {

    GameState state = GameState::MAINMENU;

    // Make exiting with the home button possible
    setup_callbacks();

    // Setup the library used for rendering
    initGu();
    
    //Load font data
    UI::loadFont("PSPGameFont128.fnt", "PSPGameFont128.png");

    //Load skills
    Commands::loadSkills("project_crystallis_skill_sheet.csv");

    // InputHandler playerInput;
    InputHandler::initializeInputHandler();

    // const int GROUND_LEVEL = 200;

    clock_t prevTime = 0;
    clock_t currTime = 0;
    float deltaTime = 0;

    running = 1;

    float fpsTimer = 0.0f;
    int frameCount = 0;

    //Set beginning scene
    SceneManager::currentScene = new MainMenu();

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

        // startFrame();

        //Update
        SceneManager::update(deltaTime);

        //Render
        startFrame();

        SceneManager::render(deltaTime);

        RenderPipeline_Flush(&g_queue);

        RenderQueue_Clear(&g_queue);

        //Switch if nextscene changed
        SceneManager::changeScene();

        endFrame();

    }


    return 0;
}