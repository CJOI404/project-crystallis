#include "Character.h"
#include <UIRender.h>
#include <CommandRegistry.h>

Character::Character(){


    characterState = Waiting;

    //add components here 
    moveComp = new MovementComponent(&xPos, &yPos);

    //initialize resistances (0.5 = half damage taken, 2.0 = double, etc)
    resistances[Element::FIRE] = 1.0;
    resistances[Element::ICE] = 1.0;
    resistances[Element::LIGHTNING] = 1.0;
    resistances[Element::WATER] = 1.0;
    resistances[Element::WIND] = 1.0;
    resistances[Element::EARTH] = 1.0;
    resistances[Element::PHYSICAL] = 1.0;
    resistances[Element::MAGICAL] = 1.0;

    //initialize immunities (1.0 = no effect on debuff chance, 0.5 = half chance, etc)
    immunities[Debuff::DEBRAVE] = 1.0;
    immunities[Debuff::DEFAITH] = 1.0;
    immunities[Debuff::DEPROTECT] = 1.0;
    immunities[Debuff::DESHELL] = 1.0;
    immunities[Debuff::POISON] = 1.0;
    immunities[Debuff::IMPERIL] = 1.0;
    immunities[Debuff::SLOW] = 1.0;
    immunities[Debuff::FOG] = 1.0;
    immunities[Debuff::PAIN] = 1.0;
    immunities[Debuff::CURSE] = 1.0;
    immunities[Debuff::DAZE] = 1.0;
    immunities[Debuff::PROVOKE] = 1.0;


}

Character::~Character(){
    delete moveComp;
    for (BattleCommand* p : commandQueue){
        delete p;
    }
}

void Character::setResistance(Element element, float val){
    resistances[element] = val;
}

float Character::getResistance(Element element){
    return resistances[element];
}

void Character::setImmunity(Debuff debuff, float val){
    immunities[debuff] = val;
}

float Character::getImmunity(Debuff debuff){
    return immunities[debuff];
}

void Character::startAttack(int targetIndex){
    this->targetIndex = targetIndex;
    characterState = AttackReady;
}

void Character::addBattleCommand(BattleCommand* command, int index){
    abilities[index] = command;
}

void Character::addViableBattleCommands(){
    // int abilitiesIndex = 0;
    // //clear abilities array
    // for (int i = 0; i < abilities.size(); i++){
    //     abilities[i] = nullptr;
    // }
    abilities.clear();
    for (int i = 0; i < Commands::commandList.size(); i++){
        if (Commands::commandList.at(i)->role == currentRole){
            abilities.push_back(Commands::commandList.at(i));
        }
    }
}

void Character::updateEffects(float dt){
    //countdown effect timer
    for (int i = 0; i < sizeof(activeDebuffs); i++){
        if (activeDebuffs[i]){
            debuffDurations[i] -= dt;
            if (debuffDurations[i] <= 0){
                debuffDurations[i] = 0;
                activeDebuffs[i] = false;
                //revert debuff effects
                // switch (i){
                //     case Debuff::DEBRAVE:
                //     case Debuff::DEPROTECT:
                // }
            }
        } 

    }
}

void Character::update(float dt){

    // //Effect countdown (frees memory)
    // for (int i = activeEffects.size() - 1; i >= 0; i--){
    //     if (activeEffects[i]->length <= 0){
    //         delete activeEffects[i];
    //         activeEffects.erase(activeEffects.begin() + i);
    //     } else {
    //         //apply debuff
    //         // activeEffects[i]->debuff
    //     }
    // }

    updateEffects(dt);

    /*Poison causes the target to continuously lose HP every second equal to 0.32% of their maximum*/
    if (activeDebuffs[Debuff::POISON]){
        health -= (maxHealth * 0.0032) * dt;
    }

    //check if they should be staggered
    if (stagger >= staggerPoint && !staggered){
        stagger += 100;
        staggered = true;
        chainDuration = (chainDuration * 2) + 7;
        if (chainDuration < 8) chainDuration = 8;
        if (chainDuration > 45) chainDuration = 45;
        peakChainDuration = chainDuration;
    }
    //calculate chain fall, reset stagger/chain bonus is duration is over after stagger
    chainDuration -= dt;
    if (chainDuration < 0){
        chainDuration = 0;
        stagger = 100;
        if (staggered){
            staggered = false;
        }
    }

    if (moveComp) moveComp->update(dt);

    if (currAtbVal < 0){
        currAtbVal = 0;
    }

    //ATB recharge
    if (characterState != Attacking && characterState != AttackCooldown){
        if (currAtbVal < atbSegments){
            currAtbVal += atbRechargeSpeed * dt;
        } else {
            currAtbVal = atbSegments;
        }        
    } 
    if (characterState == AttackReady){
        if (currAtbVal >= commandQueue.size()){
            characterState = Attacking;
            currAtbCooldownVal = 0;
        }
    }
    if (characterState == Attacking){

        //wait until usetime cooldown is reached to execute attack.
        currCommandCooldownVal += dt;
        if (currCommandCooldownVal >= commandQueue.back()->useTime){
            // Change from vector implementation at some point; this is more suited for a queue or an array.
            // erase is not performant
            commandQueue.at(0)->execute(this, enemyList.at(targetIndex));
            commandQueue.erase(commandQueue.begin());
            currCommandCooldownVal = 0;
        }

        //end attack once command queue is depleted
        if (commandQueue.empty()){
            targetIndex = -1;
            characterState = AttackCooldown;
        }

    }
    if (characterState == AttackCooldown){
        currAtbCooldownVal += dt;
        if (currAtbCooldownVal >= atbChargeCooldown){
            characterState = Waiting;
        }
    }


}

void Character::render(float dt){
    UI::drawRect(xPos, yPos, 20, 20, moveComp->color);
    snprintf(UI::textBuffer, sizeof(UI::textBuffer), name);
    UI::drawString(xPos, yPos, 0xFFFFFFFF, 0.3, 0.3, UI::textBuffer); 
}