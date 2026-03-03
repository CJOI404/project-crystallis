#pragma once

#include "BattleCommand.h"
#include "Character.h"

class AttackCommand : public BattleCommand {

    public:
        float atkDmgScale = 0;
        float ravDmgScale = 0;
        float chainValue = 0;
        float duration = 0;

        AttackCommand();
        AttackCommand(std::string name, float atkDmgScale, float ravDmgScale, float chainValue, int atbCost);
        AttackCommand(std::string name, float atkDmgScale, float ravDmgScale, float chainValue, int atbCost, float useTime);
        AttackCommand(std::string name, float atkDmgScale, float ravDmgScale, float chainValue, int atbCost, float duration, float useTime);

        void execute(Character* sender, Character* receiver) override;
   

};