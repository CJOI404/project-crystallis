#include "GlobalDefs.h"

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