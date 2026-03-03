#include "AttackCommand.h"

AttackCommand::AttackCommand(){

}
AttackCommand::AttackCommand(std::string name, float atkDmgScale, float ravDmgScale, float chainValue, int atbCost){
    this->name = name;
    this->atkDmgScale = atkDmgScale;
    this->ravDmgScale = ravDmgScale;
    this->chainValue = chainValue;
    this->atbCost = atbCost;
}

AttackCommand::AttackCommand(std::string name, float atkDmgScale, float ravDmgScale, float chainValue, int atbCost, float useTime){
    this->name = name;
    this->atkDmgScale = atkDmgScale;
    this->ravDmgScale = ravDmgScale;
    this->chainValue = chainValue;
    this->atbCost = atbCost;
    this->useTime = useTime;
}

AttackCommand::AttackCommand(std::string name, float atkDmgScale, float ravDmgScale, float chainValue, int atbCost, float duration, float useTime){
    this->name = name;
    this->atkDmgScale = atkDmgScale;
    this->ravDmgScale = ravDmgScale;
    this->chainValue = chainValue;
    this->atbCost = atbCost;
    this->duration = duration;
    this->useTime = useTime;
}

void AttackCommand::execute(Character* sender, Character* receiver){
    if (receiver->currCommand == nullptr || receiver->currCommand->keep <= sender->currCommand->cut){

        receiver->health -= sender->atkDamage * atkDmgScale * (receiver->stagger/100);
        receiver->health -= sender->ravDamage * ravDmgScale * (receiver->stagger/100);

        if (receiver->staggered == false){
            receiver->chainDuration += duration;
            receiver->stagger += ((chainValue) * ((float)(100 - receiver->chainResistance)/100));
        } else {
            receiver->stagger += ((chainValue));
        }

        //HEALTH DAMAGE FORMULA
        //MAX DAMAGE = BASE STAT X DMG MULTIPLIER X MODIFIERS X STAGGER CHAIN % - RANDOM %


        //Current Chain Bonus algorithm THIS IS FOR STAGGER DAMAGE CALCULATION:
        //[(chainValue) * ((100 - chainResist)/100)] * ResistMod

        //Final Goal for Chain Bonus calculation:
        //[(chainValue + BaseMod) * ((100 - chainResist)/100) + ConditionalMod] * ResistMod

        //BaseMod is for passive abilities on the enemy so do that later but dont dont forget
        //Conditional mod is for conditional stuff (i.e commando gets more chain bonus when attacking launched character). Do this later too

        //This can be done soon but not rn
        //ResistMod: "The resistance modifier itself is 1 by default, 0.5 when the affinity is Halved or Resistant, and 0.25 when Immune or Absorbed"


    } 
    sender->currAtbVal -= atbCost;
    sender->prevCommand = sender->currCommand;
    sender->currCommand = nullptr;

}