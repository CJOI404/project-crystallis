#include "Character.h"
#include <UIRender.h>
#include <CommandRegistry.h>

Character::Character(){


    characterState = Waiting;

    //add components here 
    moveComp = new MovementComponent(&xPos, &yPos);

    //initialize resistances (0.5 = half damage taken, 2.0 = double, etc)
    resistances[Element::FIRE] = Resistance::NORMAL;
    resistances[Element::ICE] = Resistance::NORMAL;
    resistances[Element::LIGHTNING] = Resistance::NORMAL;
    resistances[Element::WATER] = Resistance::NORMAL;
    resistances[Element::WIND] = Resistance::NORMAL;
    resistances[Element::EARTH] = Resistance::NORMAL;
    resistances[Element::PHYSICAL] = Resistance::NORMAL;
    resistances[Element::MAGICAL] = Resistance::NORMAL;

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

void Character::setResistance(Element element, Resistance val){
    resistances[element] = val;
}

float Character::getResistance(Element element){
    switch (resistances[element]){
        case Resistance::IMMUNE:
            return 0;
        case Resistance::RESISTANT:
            return 0.1;
        case Resistance::HALVED:
            return 0.5;
        case Resistance::NORMAL:
            return 1;
        case Resistance::WEAK:
            return 2;
        default:
            return 1;
    }
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

void Character::revertDebuff(int effectIdx){
    switch (effectIdx){
        case Debuff::IMPERIL:
            //restore resistances
            for (int i = 0; i < std::size(resistances); i++){
                //Lower resistance by one (unless immune)
                Resistance r = resistances[i];
                if (r > 0) resistances[i] = (Resistance)((int)r - 1);
            }
            break;
        case Debuff::SLOW:
            atbRechargeSpeed *= 2;
            break;
        case Debuff::CURSE:
            curseCutDiff = 0;
            break;
        
    }
}

void Character::updateEffects(float dt){
    //countdown effect timer
    for (int i = 0; i < std::size(activeDebuffs); i++){
        if (activeDebuffs[i]){
            debuffDurations[i] -= dt;
            if (debuffDurations[i] <= 0){
                debuffDurations[i] = 0;
                activeDebuffs[i] = false;
                revertDebuff(i);
            }
        } 

    }
    
    /*Poison causes the target to continuously lose HP every second equal to 0.32% of their maximum*/
    if (activeDebuffs[Debuff::POISON]){
        health -= (maxHealth * 0.0032) * dt;
    }
}

void Character::update(float dt){

    updateEffects(dt);

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