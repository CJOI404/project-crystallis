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

const char* debuffToString(Debuff d) {
    switch (d) {
        case DEBRAVE:       return "DEBRAVE";
        case DEFAITH:       return "DEFAITH";
        case DEPROTECT:     return "DEPROTECT";
        case DESHELL:       return "DESHELL";
        case POISON:        return "POISON";
        case IMPERIL:       return "IMPERIL";
        case SLOW:          return "SLOW";
        case FOG:           return "FOG";
        case PAIN:          return "PAIN";
        case CURSE:         return "CURSE";
        case DAZE:          return "DAZE";
        case PROVOKE:       return "PROVOKE";
        case DISPEL:        return "DISPEL";
        default:            return "???";
    }
}

const char* buffToString(Buff b){
    switch (b) {
        case BRAVERY:       return "BRAVERY";      
        case BRAVERA:       return "BRAVERA";
        case FAITH:         return "FATH";
        case FAITHRA:       return "FAITHRA";
        case HASTE:         return "HASTE";
        case VIGILANCE:     return "VIGILANCE";
        case ENFIRE:        return "ENFIRE";
        case ENFROST:       return "ENFROST";
        case ENTHUNDER:     return "ENTHUNDER";
        case ENWATER:       return "ENWATER";
        case PROTECT:       return "PROTECT";
        case PROTECTRA:     return "PROTECTRA";
        case SHELL:         return "SHELL";
        case SHELLRA:       return "SHELLRA";
        case BARFIRE:       return "BARFIRE";
        case BARFROST:      return "BARFROST";
        case BARTHUNDER:    return "BARTHUNDER";
        case BARWATER:      return "BARWATER";
        case VEIL:          return "VEIL";
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


