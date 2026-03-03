#pragma once

#include "Command.h"

class DashCommand : public Command{
    public:
        virtual void execute(Character& character);
};