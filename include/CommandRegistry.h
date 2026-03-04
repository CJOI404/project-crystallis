#pragma once

#include "AttackCommand.h"

namespace Commands{

    void loadSkills(const char* skillSheetPath);

    extern std::vector<BattleCommand*> commandList;

}