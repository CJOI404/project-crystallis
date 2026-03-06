#pragma once

#include "BattleCommand.h"
#include "Character.h"

class AttackCommand : public BattleCommand {

    public:

        //Attack 
        Element element = Element::NOELEMENT;

        //Status
        float chance;
        float length;
        Debuff debuff;
        Buff buff;

        AttackCommand();
        AttackCommand(CommandData& cmdData);
        // AttackCommand(std::string name, float atkDmgScale, float ravDmgScale, float chainValue, int atbCost);
        // AttackCommand(std::string name, float atkDmgScale, float ravDmgScale, float chainValue, int atbCost, float useTime);
        // AttackCommand(std::string name, float atkDmgScale, float ravDmgScale, float chainValue, int atbCost, float duration, float useTime);

        void execute(Character* sender, Character* receiver) override;

        int calculateDmg(Character* sender, Character* receiver);
        void handleStatus(Character* sender, Character* receiver);
   

};