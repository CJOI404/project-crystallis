#pragma once

#include "BattleCommand.h"
#include "Character.h"

class DebuffCommand : public BattleCommand {
    public:
        float chance;
        float length;
        Debuff debuff;

        DebuffCommand(CommandData& cmdData);

        void execute(Character* sender, Character* receiver) override;
};