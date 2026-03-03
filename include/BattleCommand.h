#pragma once
#include <string>

class Character;

class BattleCommand{
    public:
        std::string name;
        int cut = 0;
        int keep = 0;
        int atbCost = 0;
        float useTime = 0.5;
        virtual void execute(Character* sender, Character* receiver) = 0;
};