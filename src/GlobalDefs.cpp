#include "GlobalDefs.h"
#include <random>

const char* roleToString(Role r) {
    switch (r) {
        case COMMANDO:      return "COM";
        case RAVAGER:       return "RAV";
        case MEDIC:         return "MED";
        case SENTINEL:      return "SEN";
        case SABOTEUR:      return "SAB";
        case SYNERGIST:     return "SYN";
        default:            return "???";
    }
}

float getRandomFloat() {
    // Use static variables for the random device, engine, and distribution
    // to ensure they are initialized only once and maintain state across calls.
    static std::random_device rd;
    static std::mt19937 engine(rd()); // Seed the engine with a non-deterministic value
    // Define the distribution for the range [0.0, 1.0)
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f); 
    
    return dist(engine);
}