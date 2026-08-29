#pragma once
#include "Scenes/Scene.h"

namespace SceneManager {
    extern Scene* currentScene;
    extern Scene* nextScene;

    void setNextScene(Scene* newScene);
    void update(float dt);
    void render(float dt);
    void changeScene();


}