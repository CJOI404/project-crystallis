#pragma once

#include "Entities/Character.h"

class Scan {
    public:
        std::vector<Character*> enemyList;

        int scanIdx = 0;

        bool active = false;

        void update(float dt);
        void render(float dt);
};