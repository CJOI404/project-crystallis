#pragma once

#include "BattleCommand.h"
#include "Character.h"

class DebuffCommand : public BattleCommand {
    public:
        float chance;
        float length;
        Debuff effect;

        DebuffCommand(CommandData& cmdData);

        void execute(Character* sender, Character* receiver) override;
};