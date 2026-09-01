#pragma once

#include <pspaudio.h>
#include <pspthreadman.h>
#include <string>
#include <unordered_map>
#include <cstdint>

constexpr int MAX_PSP_CHANNELS = 8;
constexpr int AUDIO_BUFFER_SAMPLES = 1024;

struct SoundSample {
    int16_t* pcmData = nullptr;
    uint32_t dataSize = 0;
    uint32_t sampleCount = 0;
    uint32_t sampleRate = 44100;
    int channels = 2;
};

namespace AudioManager {

    extern std::unordered_map<std::string, SoundSample> samples;

    bool init();
    void shutdown();

    bool loadSound(const std::string& name, const std::string& filePath);
    void unloadSound(const std::string& name);

    void play(const std::string& name, float volume, bool loop);
    void stop(const std::string& name);
    void stopAll();
}