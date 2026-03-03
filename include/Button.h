#pragma once

#include <string>
#include "GlobalDefs.h"

class Button{

    public:
        Button();
        Button(std::string name);

        std::string text;
        bool highlighted;
        int x, y, width, height;
        Colours colour;
};