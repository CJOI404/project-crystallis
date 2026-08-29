#pragma once

class Scene {
    public:
        // void* data;
        
        virtual void update(float dt) = 0;
        virtual void render(float dt) = 0;
        virtual void unload() = 0;

};

