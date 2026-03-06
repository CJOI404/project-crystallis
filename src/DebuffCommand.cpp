#include "DebuffCommand.h"
#include <cstring>


DebuffCommand::DebuffCommand(CommandData& cmdData){
    memcpy(name, cmdData.name, sizeof(name));
    this->cost = cmdData.cost;
    // this->type = cmdData.type;
    memcpy(type, cmdData.type, sizeof(type));
    this->role = cmdData.role;
    this->atkDmgScale = cmdData.atkDmgScale;
    this->ravDmgScale = cmdData.ravDmgScale;
    this->chain = cmdData.chain;
    this->target = cmdData.target;
    this->variation = cmdData.variation;
    this->duration = cmdData.duration;
    this->cut = cmdData.cut;
    this->keep = cmdData.keep;
    this->useTime = cmdData.useTime;
    this->ev = cmdData.ev;
    this->pain = cmdData.pain;
    this->fog = cmdData.fog;
    this->rise = cmdData.rise;

    this->debuff = cmdData.debuff;
    this->chance = cmdData.chance;
    this->length = cmdData.length;


}

void DebuffCommand::execute(Character* sender, Character* receiver){
    //surround this by if statement for chance
    if (getRandomFloat() <= chance){ 
        receiver->activeDebuffs[debuff] = true;
        receiver->debuffDurations[debuff] = length;

        // xxx = accounted for in other algorithm (like damage calculation)
        /* _________________________________________________________________________
        |     Status     |     Effect     | Restoration | Overwrites |   Cancels  |
        |=========================================================================|
        | Debrave        | x0.1 physical  | Esuna - Y   |            |            |
        |                |  attack        | Dispel - N  |  (None)    | Bravery/ra | XXX
        |                |    (*3)        | Arise - Y   |            |            |
        |=========================================================================|
        | Defaith        | x0.1 magical   | Esuna - Y   |            |            |
        |                |  attack        | Dispel - N  |  (None)    | Faith/ra   | XXX
        |                |    (*3)        | Arise - Y   |            |            |
        |=========================================================================|
        | Deprotect      | physical damage| Esuna - Y   |            |            |
        |                | taken increases| Dispel - N  |  (None)    | Protect/ra | XXX
        |                | by 89%         | Arise - Y   |            |            |
        |=========================================================================|
        | Deshell        | magical damage | Esuna - Y   |            |            |
        |                | taken increases| Dispel - N  |  (None)    | Shell/ra   | XXX
        |                | by 89%         | Arise - Y   |            |            |
        |=========================================================================|
        | Poison         | Lose 1% max HP | Esuna - Y   |            |            |
        |                | every 3 secs   | Dispel - N  |  (None)    | (None)     | XXX these 4 are accounted for when damage is calculated
        |                |                | Arise - Y   |            |            |
        |=========================================================================|
        | Imperil        | Resistances    | Esuna - Y   |            |            |
        |                | decrease by 1  | Dispel - N  |  (None)    | (None)     |
        |                | rank (*4)      | Arise - Y   |            |            |
        |=========================================================================|
        | Slow           | ATB gauge fills| Esuna - Y   |            |            |
        |                | up at half the | Dispel - N  |  (None)    | Haste      |
        |                | normal speed   | Arise - Y   |            |            |
        |=========================================================================|
        | Fog            | Can no longer  | Esuna - Y   |            |            |
        |                | execute magical| Dispel - N  |  (None)    | (None)     | 
        |                | attacks        | Arise - Y   |            |            |
        |=========================================================================|
        | Pain           | Can no longer  | Esuna - Y   |            |            |
        |                | execute phys   | Dispel - N  |  (None)    | (None)     | Need to add a way to mark abilities valid/invalid 
        |                | attacks        | Arise - Y   |            |            |
        |=========================================================================|
        | Curse          | Reduces Cut and| Esuna - Y   |            |            |
        |                | Keep by 20 on  | Dispel - N  |  (None)    | Vigilance  |
        |                | all actions(*5)| Arise - Y   |            |            |
        |=========================================================================|
        | Daze           | Damage taken   | Esuna - Y   |            |            |
        |                | doubles. Can't | Dispel - N  |  (None)    | (None)     | xxx double damage, cant perform actions not implemented yet
        |                | perform actions| Arise - Y   |            |            |
        |=========================================================================|
        | Provoke        | Only targets   | Esuna - N   |            |            |
        |                | the one who    | Dispel - N  |  (None)    | (None)     |
        |                | provoked       | Arise - N   |            |            |
        |=========================================================================|
        | Death          | Dies once the  | Esuna - N   |            |            |
        | Sentence       | count reaches  | Dispel - N  |  (None)    | (None)     |
        |                | zero           | Arise - (*6)|            |            |
        |=========================================================================|
        | Death          | Incapacitated  | Esuna - N   |            |            |
        |                | until revived  | Dispel - N  |  (None)    | (None)     |
        |                |                | Arise -  Y  |            |            |
        *=========================================================================**/

        //Apply effect overwrites
        switch (debuff){
            case (DEBRAVE):
                receiver->activeBuffs[Buff::BRAVERY] = false;
                receiver->activeBuffs[Buff::BRAVERA] = false;
                break;
            case (DEFAITH): 
                receiver->activeBuffs[Buff::FAITH] = false;
                receiver->activeBuffs[Buff::FAITHRA] = false;
                break;
            case (DEPROTECT):
                receiver->activeBuffs[Buff::PROTECT] = false;
                receiver->activeBuffs[Buff::PROTECTRA] = false;
                break;
            case (DESHELL):
                receiver->activeBuffs[Buff::SHELL] = false;
                receiver->activeBuffs[Buff::SHELLRA] = false;
                break;
            case (SLOW):
                receiver->activeBuffs[Buff::HASTE] = false;
                break;
            case (CURSE):
                receiver->activeBuffs[Buff::VIGILANCE] = false;
                break;
        }


        // receiver->activeEffects.push_back(new Effect {debuff, length});
    /*    DEBRAVE,
        DEFAITH,
        DEPROTECT,
        DESHELL,
        POISON,
        IMPERIL,
        SLOW,
        FOG,
        PAIN,
        CURSE,
        DAZE,
        PROVOKE,*/
    }
}