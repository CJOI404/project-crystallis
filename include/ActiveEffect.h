#pragma once

#include <string>
#include "Character.h"

class ActiveEffect {
    std::string ID = "";

    int duration = 0;

    void execute(Character& character);
};