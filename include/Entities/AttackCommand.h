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

        void execute(Character* sender, Character* receiver) override;

        int calculateDmg(Character* sender, Character* receiver, float atkDmgBase, float ravDmgBase);
        void handleStatus(Character* sender, Character* receiver);
   

};