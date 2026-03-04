#pragma once
#include <string>
#include "GlobalDefs.h"
#include <cstdint>

class Character;

class BattleCommand{
    public:
        char name[16];
        uint8_t cost = 0;
        char type[8];
        Paradigm paradigm = Paradigm::COMMANDO;
        float atkDmgScale = 0;
        float ravDmgScale = 0;
        float chain = 0;

        //targeting type, 0 = single enemy
        int8_t target = 0;
        float variation = 0;
        float duration = 0;
        int8_t cut = 0;
        int8_t keep = 0;
        float useTime = 0.5;
        bool ev = false;
        bool pain = false;
        bool fog = false;
        float rise = 0;

        virtual void execute(Character* sender, Character* receiver) = 0;
};