#include "DashCommand.h"

void DashCommand::execute(Character& character){
    if (character.moveComp) {
        // actor.moveComp->state == MovementState::STATE_DASHING;
        character.moveComp->dash();
    }
}