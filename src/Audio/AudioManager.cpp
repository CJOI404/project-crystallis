#include "Audio/AudioManager.h"
#include <pspaudiolib.h>
#include <string>

namespace AudioManager {

    std::unordered_map<std::string, SoundSample> samples;

    bool init(){
        return pspAudioInit();
    }
    void shutdown(){
        pspAudioEnd();
    }

    bool loadSound(const std::string& name, const std::string& filePath){
        return 0;
    }
    void unloadSound(const std::string& name){

    }

    void play(const std::string& name, float volume, bool loop){
        auto it = samples.find(name);

    }
    void stop(const std::string& name){

    }
    void stopAll(){

    }
}