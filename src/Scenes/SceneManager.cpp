#include "Scenes/SceneManager.h"

namespace SceneManager {
    Scene* currentScene = nullptr;
    Scene* nextScene = nullptr;

    void setNextScene(Scene* newScene){
        nextScene = newScene;
    }
    void changeScene(){
        if (nextScene != nullptr){
            currentScene->unload();
            currentScene = nextScene;
            nextScene = nullptr;
        }
    }
    void update(float dt){
        if (currentScene != nullptr)
            currentScene->update(dt);
    }
    void render(float dt){
        if (currentScene != nullptr)
            currentScene->render(dt);
    }
}