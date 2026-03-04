#pragma once

#include "AttackCommand.h"

class CommandRegistry{

    public:

        CommandRegistry();
        ~CommandRegistry();

        void loadSkills(const char* skillSheetPath);

        std::vector<BattleCommand*> commandList;


};