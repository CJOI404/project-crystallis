#pragma once

#include "BattleCommand.h"
#include "Character.h"

class AttackCommand : public BattleCommand {

    public:

        //this will likely have to be changed later
        Element element = Element::NOELEMENT;

        AttackCommand();
        AttackCommand(CommandData& cmdData);
        // AttackCommand(std::string name, float atkDmgScale, float ravDmgScale, float chainValue, int atbCost);
        // AttackCommand(std::string name, float atkDmgScale, float ravDmgScale, float chainValue, int atbCost, float useTime);
        // AttackCommand(std::string name, float atkDmgScale, float ravDmgScale, float chainValue, int atbCost, float duration, float useTime);

        void execute(Character* sender, Character* receiver) override;
   

};