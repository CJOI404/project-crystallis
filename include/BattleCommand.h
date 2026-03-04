#pragma once
#include <string>
#include "GlobalDefs.h"

class Character;

class BattleCommand{
    public:
        std::string name = "";
        int cost = 0;
        std::string type = "";
        Paradigm paradigm;
        float atkDmgScale = 0;
        float ravDmgScale = 0;
        float chain = 0;

        //targeting type, 0 = single enemy
        int target = 0;
        float variation = 0;
        float duration = 0;
        int cut = 0;
        int keep = 0;
        float useTime = 0.5;
        bool ev = false;
        bool pain = false;
        bool fog = false;
        float rise = 0;

        virtual void execute(Character* sender, Character* receiver) = 0;
};