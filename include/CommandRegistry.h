#pragma once

#include "AttackCommand.h"
#include "DebuffCommand.h"

namespace Commands{




        void loadSkills(const char* skillSheetPath);

        extern std::vector<BattleCommand*> commandList;


};