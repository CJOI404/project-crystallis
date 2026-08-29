#include "graphics/AssetManagers/SpriteManager.h"

namespace SpriteManager {

    std::unordered_map<std::string, Sprite> sprites;

    Sprite* getSprite(const std::string& id) {
        auto it = sprites.find(id);
        if (it != sprites.end()) {
            return &it->second;
        }
        return nullptr;
    }


    void registerSprite(const std::string& id, Texture* tex, float x, float y, float w, float h) {

        // Sprite* it = getSprite(id);

        // if (it){
        //     printf("%s already loaded, returning reference\n", id);
        //     fflush(stdout);
        //     it->refCount++;
        // } 
        Sprite data;
        data.texture = tex;
        data.u0 = x;
        data.v0 = y;
        data.u1 = x + w;
        data.v1 = y + h;
        data.width = w;
        data.height = h;

        sprites[id] = data;


    }

    //TODO: Sprite loading/unloading is absolutely jacked. It doesn't matter too much rn since its not a texture just uv data really but also needs to be fixed
    void unload(const std::string& id){
        auto it = sprites.find(id);
        
        if (it == sprites.end()){
            printf("DID NOT UNLOAD SPRITE. COULD NOT FIND %s\n", id);
            fflush(stdout);
            return;
        }
        it->second.refCount--;
        if (it->second.refCount <= 0) {
            sprites.erase(it);

            printf("UNLOADED %s\n", id);
            fflush(stdout);
        } else {
            printf("DID NOT UNLOAD: %s, REFCOUNT: %d\n", id, it->second.refCount);
            fflush(stdout);
        }

        // it->second.refCount--;
        // if (it->second.refCount <= 0) {
        //     if (it->second.refCount <= 0){
        //         sprites.erase(it);
        //     }
        //     printf("UNLOADED %s", id);
        //     fflush(stdout);
        // }
    }
}