#include "AttackCommand.h"
#include <cstring>

AttackCommand::AttackCommand(){

}

AttackCommand::AttackCommand(CommandData& cmdData){
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

    //attack commands
    this->element = cmdData.element;

    this->debuff = cmdData.debuff;
    this->buff = cmdData.buff;
    this->chance = cmdData.chance;
    this->length = cmdData.length;

}


void AttackCommand::execute(Character* sender, Character* receiver){
    if (cut < 0 || receiver->currCommand == nullptr || receiver->currCommand->keep + receiver->cutDiff <= sender->currCommand->cut - sender->cutDiff ){

        //full dmg calculation (work in progress)
        if (atkDmgScale > 0 || ravDmgScale > 0){
            receiver->health -= calculateDmg(sender, receiver);
        }

        handleStatus(sender, receiver);

        //simple chain calculation
        if (receiver->staggered == false){
            receiver->chainDuration += duration;
            receiver->peakChainDuration = receiver->chainDuration;
            receiver->stagger += ((chain) * ((float)(100 - receiver->chainResistance)/100));
        } else {
            receiver->stagger += ((chain));
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
    sender->currAtbVal -= cost;
    sender->prevCommand = sender->currCommand;
    sender->currCommand = nullptr;

}

void AttackCommand::handleStatus(Character* sender, Character* receiver){
    //Full stick% calculataion
    //(Base chance %) * (alteration due to target's resistance) * Chain Bonus
    float statusChanceMod = 1;
    if (receiver->activeBuffs[Buff::VEIL]) statusChanceMod *= 0.5;
    if (getRandomFloat() <= (chance * receiver->immunities[debuff] * (receiver->stagger / 100) * statusChanceMod)){ 
        if (debuff != Debuff::NODEBUFF){
            receiver->debuffDurations[debuff] = length;
            if (receiver->activeDebuffs[debuff] == false){
                receiver->activeDebuffs[debuff] = true;

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
                |                | decrease by 1  | Dispel - N  |  (None)    | (None)     | XXX
                |                | rank (*4)      | Arise - Y   |            |            |
                |=========================================================================|
                | Slow           | ATB gauge fills| Esuna - Y   |            |            |
                |                | up at half the | Dispel - N  |  (None)    | Haste      | XXX
                |                | normal speed   | Arise - Y   |            |            |
                |=========================================================================|
                | Fog            | Can no longer  | Esuna - Y   |            |            |
                |                | execute magical| Dispel - N  |  (None)    | (None)     | XXX
                |                | attacks        | Arise - Y   |            |            |
                |=========================================================================|
                | Pain           | Can no longer  | Esuna - Y   |            |            |
                |                | execute phys   | Dispel - N  |  (None)    | (None)     | XXX When writing AI this will need to be checked (rn it just prevents the player from selecting it)
                |                | attacks        | Arise - Y   |            |            |
                |=========================================================================|
                | Curse          | Reduces Cut and| Esuna - Y   |            |            |
                |                | Keep by 20 on  | Dispel - N  |  (None)    | Vigilance  | XXX
                |                | all actions(*5)| Arise - Y   |            |            |
                |=========================================================================|
                | Daze           | Damage taken   | Esuna - Y   |            |            |
                |                | doubles. Can't | Dispel - N  |  (None)    | (None)     | xxx 
                |                | perform actions| Arise - Y   |            |            |
                |=========================================================================|
                | Provoke        | Only targets   | Esuna - N   |            |            |
                |                | the one who    | Dispel - N  |  (None)    | (None)     |
                |                | provoked       | Arise - N   |            |            |
                |=========================================================================|
                | Death          | Dies once the  | Esuna - N   |            |            |
                | Sentence       | count reaches  | Dispel - N  |  (None)    | (None)     | not doing this
                |                | zero           | Arise - (*6)|            |            |
                |=========================================================================|
                | Death          | Incapacitated  | Esuna - N   |            |            |
                |                | until revived  | Dispel - N  |  (None)    | (None)     |
                |                |                | Arise -  Y  |            |            |
                *=========================================================================**/

                //Apply effect overwrites and effects
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
                        //half atb recharge rate
                        receiver->atbRechargeSpeed /= 2;
                        receiver->activeBuffs[Buff::HASTE] = false;
                        break;
                    case (CURSE):
                        receiver->activeBuffs[Buff::VIGILANCE] = false;
                        receiver->cutDiff = -20;
                        break;
                    case (IMPERIL): {
                        for (int i = 0; i < std::size(receiver->resistances); i++){
                            //Lower resistance by one (unless immune)
                            if (receiver->resistances[i] > Resistance::IMMUNE) receiver->resistances[i] = (Resistance)(receiver->resistances[i] - 1);
                        }
                        break;
                    }
                    case (DISPEL): 
                        receiver->activeBuffs[receiver->mostRecentBuff] = false;
                    
                }
            }
            
        }
        
    } else if (buff != Buff::NOBUFF){
        //buff always sticks so no need for getRandomFloat
        receiver->buffDurations[buff] = length;
        if (receiver->activeBuffs[buff] == false){
            receiver->activeBuffs[buff] = true;

                /*  |     Status     |     Effect     | Restoration | Overwrites |   Cancels  |
                    |=========================================================================|
                    | Bravery        | x 1.4 physical | Esuna - N   |            |            |
                    |                |   attack       | Dispel - Y  | Bravera    | Debrave    | XXX
                    |                |                | Arise - N   |            |            |
                    |=========================================================================|
                    | Bravera        | x 1.8 physical | Esuna - N   |            |            |
                    |                |   attack       | Dispel - Y  | Bravery    | Debrave    | XXX
                    |                |                | Arise - N   |            |            |
                    |=========================================================================|
                    | Faith          | x 1.4 magical  | Esuna - N   |            |            |
                    |                |   attack       | Dispel - Y  | Faithra    | Defaith    | XXX
                    |                |                | Arise - N   |            |            |
                    |=========================================================================|
                    | Faithra        | x 1.8 magical  | Esuna - N   |            |            |
                    |                |   attack       | Dispel - Y  | Faith      | Defaith    | XXX
                    |                |                | Arise - N   |            |            |
                    |=========================================================================|
                    | Haste          | x 1.5 ATB      | Esuna - N   |            |            |
                    |                |   gauge speed  | Dispel - Y  |  (None)    | Slow       | XXX
                    |                |                | Arise - N   |            |            |
                    |=========================================================================|
                    | Vigilance      | +10 Cut & Keep | Esuna - N   |            |            |
                    |                |  (*1)          | Dispel - Y  |  (None)    | Curse      | XXX
                    |                |                | Arise - N   |            |            |
                    |=========================================================================|
                    | Enfire (*2)    | Non-elemental  | Esuna - N   |            |            |
                    |                | attacks become | Dispel - Y  |  Any En-   | (None)     | XXX
                    |                | fire elemental | Arise - N   |            |            |
                    |=========================================================================|
                    | Enfrost (*2)   | Non-elemental  | Esuna - N   |            |            |
                    |                | attacks become | Dispel - Y  |  Any En-   | (None)     | XXX
                    |                | ice elemental  | Arise - N   |            |            |
                    |=========================================================================|
                    | Enthunder (*2) | Non-elemental  | Esuna - N   |            |            |
                    |                | attacks become | Dispel - Y  |  Any En-   | (None)     | XXX
                    |                | thunder element| Arise - N   |            |            |
                    |=========================================================================|
                    | Enwater (*2)   | Non-elemental  | Esuna - N   |            |            |
                    |                | attacks become | Dispel - Y  |  Any En-   | (None)     | XXX
                    |                | water elemental| Arise - N   |            |            |
                    |=========================================================================|
                    | Protect        | Receive 67%    | Esuna - N   |            |            |
                    |                | damage from    | Dispel - Y  |  Protectra | Deprotect  | XXX
                    |                | phys attacks   | Arise - N   |            |            |
                    |=========================================================================|
                    | Protectra      | Receive 50%    | Esuna - N   |            |            |
                    |                | damage from    | Dispel - Y  |  Protect   | Deprotect  | XXX
                    |                | phys attacks   | Arise - N   |            |            |
                    |=========================================================================|
                    | Shell          | Receive 67%    | Esuna - N   |            |            |
                    |                | damage from    | Dispel - Y  |  Shellra   | Deshell    | XXX
                    |                | mag attacks    | Arise - N   |            |            |
                    |=========================================================================|
                    | Shellra        | Receive 50%    | Esuna - N   |            |            |
                    |                | damage from    | Dispel - Y  |  Shell     | Deshell    | XXX
                    |                | mag attacks    | Arise - N   |            |            |
                    |=========================================================================|
                    | Barfire        | Receive half   | Esuna - N   |            |            |
                    |                | damage from the| Dispel - Y  |  Any Bar-  | (None)     | XXX
                    |                | fire element   | Arise - N   |            |            |
                    |=========================================================================|
                    | Barfrost       | Receive half   | Esuna - N   |            |            |
                    |                | damage from the| Dispel - Y  |  Any Bar-  | (None)     | XXX
                    |                | ice element    | Arise - N   |            |            |
                    |=========================================================================|
                    | Barthunder     | Receive half   | Esuna - N   |            |            |
                    |                | damage from the| Dispel - Y  |  Any Bar-  | (None)     | XXX
                    |                | thunder element| Arise - N   |            |            |
                    |=========================================================================|
                    | Barwater       | Receive half   | Esuna - N   |            |            |
                    |                | damage from the| Dispel - Y  |  Any Bar-  | (None)     | XXX
                    |                | water element  | Arise - N   |            |            |
                    |=========================================================================|
                    | Veil           | x0.5 chance of | Esuna - N   |            |            |
                    |                |  receiving a   | Dispel - Y  |  (None)    | (None)     | XXX
                    |                |  neg status    | Arise - N   |            |            |
                    |=========================================================================|*/


            //apply effects and overwrites
            //This switch statement is fucking absurd but it does the job 
            switch (buff){    
                case Buff::ENFIRE:
                    receiver->activeBuffs[Buff::ENFROST] = false;
                    receiver->activeBuffs[Buff::ENTHUNDER] = false;
                    receiver->activeBuffs[Buff::ENWATER] = false;
                    break;
                case Buff::ENFROST:
                    receiver->activeBuffs[Buff::ENFIRE] = false;
                    receiver->activeBuffs[Buff::ENTHUNDER] = false;
                    receiver->activeBuffs[Buff::ENWATER] = false;
                    break;
                case Buff::ENTHUNDER:
                    receiver->activeBuffs[Buff::ENFROST] = false;
                    receiver->activeBuffs[Buff::ENFIRE] = false;
                    receiver->activeBuffs[Buff::ENWATER] = false;
                    break;
                case Buff::ENWATER:
                    receiver->activeBuffs[Buff::ENFROST] = false;
                    receiver->activeBuffs[Buff::ENTHUNDER] = false;
                    receiver->activeBuffs[Buff::ENFIRE] = false;
                    break;
                case Buff::BARFIRE:
                    receiver->activeBuffs[Buff::BARFROST] = false;
                    receiver->activeBuffs[Buff::BARTHUNDER] = false;
                    receiver->activeBuffs[Buff::BARWATER] = false;
                    break;
                case Buff::BARFROST:
                    receiver->activeBuffs[Buff::BARFIRE] = false;
                    receiver->activeBuffs[Buff::BARTHUNDER] = false;
                    receiver->activeBuffs[Buff::BARWATER] = false;
                    break;
                case Buff::BARTHUNDER:
                    receiver->activeBuffs[Buff::BARFROST] = false;
                    receiver->activeBuffs[Buff::BARFIRE] = false;
                    receiver->activeBuffs[Buff::BARWATER] = false;
                    break;
                case Buff::BARWATER:
                    receiver->activeBuffs[Buff::BARFROST] = false;
                    receiver->activeBuffs[Buff::BARTHUNDER] = false;
                    receiver->activeBuffs[Buff::BARFIRE] = false;
                    break;
                case Buff::BRAVERY:
                    receiver->activeBuffs[Buff::BRAVERA] = false;
                    receiver->activeDebuffs[Debuff::DEBRAVE] = false;
                    break;
                case Buff::BRAVERA:
                    receiver->activeBuffs[Buff::BRAVERY] = false;
                    receiver->activeDebuffs[Debuff::DEBRAVE] = false;
                    break;
                case Buff::FAITH:
                    receiver->activeBuffs[Buff::FAITHRA] = false;
                    receiver->activeDebuffs[Debuff::DEFAITH] = false;
                    break;
                case Buff::FAITHRA:
                    receiver->activeBuffs[Buff::FAITH] = false;
                    receiver->activeDebuffs[Debuff::DEFAITH] = false;
                    break;
                case Buff::HASTE:
                    //double speed
                    receiver->atbRechargeSpeed *= 2;
                    receiver->activeDebuffs[Debuff::SLOW] = false;
                    break;
                case Buff::VIGILANCE:
                    receiver->cutDiff = 10;
                    receiver->activeDebuffs[Debuff::CURSE] = false;
                    break;
                case Buff::PROTECTRA:
                    receiver->activeBuffs[Buff::PROTECT] = false;
                    receiver->activeDebuffs[Debuff::DEPROTECT] = false;
                    break;
                case Buff::PROTECT:
                    receiver->activeBuffs[Buff::PROTECTRA] = false;
                    receiver->activeDebuffs[Debuff::DEPROTECT] = false;
                case Buff::SHELLRA:
                    receiver->activeBuffs[Buff::SHELL] = false;
                    receiver->activeDebuffs[Debuff::DESHELL] = false;
                    break;
                case Buff::SHELL:
                    receiver->activeBuffs[Buff::SHELLRA] = false;
                    receiver->activeDebuffs[Debuff::DESHELL] = false;
                    
            }
        }

    }

    
    
}


int AttackCommand::calculateDmg(Character* sender, Character* receiver){
    float atkDmg = (sender->atkDamage * atkDmgScale);
    float ravDmg = (sender->ravDamage * ravDmgScale);

    //1. check the following passive abilities, multiply base dmg to get D1
    /*
        Adrenaline                  *1.2 (+20%)
        Critical: Power Surge       *1.2 (+20%)
        Critical: Power Surge II    *1.4 (+40%)
        Ally KO: Power Surge        *1.1 - *2.2 (+110% - 220%)
        Ally KO: Power Surge II     *1.3 - *2.6 (+130% - 260%)
        High HP: Power Surge        *1.2 (+20%)
        Low HP: Power Surge         *1.5 (+50%)
    */

    float weaponAbilitiesMod = 1; //change to sum of weapon abilities when that is implemented

    atkDmg *= weaponAbilitiesMod;
    ravDmg *= weaponAbilitiesMod;
    
    //2.Sum applicable status effects and multiply d1 to get d2
    //ONLY ONE VERSION OF EACH EFFECT CAN BE USED
    /*
        Bravery (atk only)          *1.4
        Bravera (atk only)          *1.8
        Debrave (atk only)          *0.1
        Faith (rav only)            *1.4
        Faithra (rav only)          *1.8
        Defaith (rav only)          *0.1
    
    */
    float statusEffectsModAtk = 1; 
    float statusEffectsModRav = 1;

    if (sender->activeDebuffs[DEBRAVE]) statusEffectsModAtk *= 0.1;
    if (sender->activeBuffs[BRAVERA]) statusEffectsModAtk *= 1.8;
    else if (sender->activeBuffs[BRAVERY]) statusEffectsModAtk *= 1.4;

    if (sender->activeDebuffs[DEFAITH]) statusEffectsModRav *= 0.1;
    if (sender->activeBuffs[FAITHRA]) statusEffectsModRav *= 1.8;
    else if (sender->activeBuffs[FAITH]) statusEffectsModRav *= 1.4;

    atkDmg *= statusEffectsModAtk;
    ravDmg *= statusEffectsModRav;

    //3. Get lower limit for random damage factor
    float atkDmgL = (1 - variation) * atkDmg;
    float ravDmgL = (1 - variation) * atkDmg;

    //4. Apply chain to d2 get d3
    atkDmg *= (receiver->stagger/100);
    ravDmg *= (receiver->stagger/100);

    //5.Calculate bonuses 
    // (will skip for now. DONT FORGET TO ALL COMMANDO BONUS HERE WHEN DOING ROLE BONUSES)
    // The rest of these seem stupid and not worth implementing, except maybe vendetta and entrench
    /*
        | Type | Ability                                  Effect |
        |========================================================|
        | Both | Commando Role Bonus ................ +5% - 180% |
        | Both | X Elemental Damage% Up ............  +20% - 50% |
        | Phys | Enfeeblement ............................. -50% |
        | Phys | Hindrance ................................ -30% |
        | Mag  | Stiffed Magic ............................ -50% |
        | Mag  | Fettered Magic ........................... -30% |
        | Phys | Vendetta .......................... +10% - 300% |
        | Phys | Entrench .......................... +30% - 150% |
        *========================================================*
    */

    float comRoleBonus = 1; //Ignoring role bonuses for now

    atkDmg *= comRoleBonus;
    ravDmg *= comRoleBonus;

    //6. Calculate element bonus
    //This checks if the Enhance Element buff aligns with the attack element

    if ((sender->activeBuffs[Buff::ENFIRE] && element == Element::FIRE)
        || (sender->activeBuffs[Buff::ENFROST] && element == Element::ICE)
        || (sender->activeBuffs[Buff::ENTHUNDER] && element == Element::LIGHTNING)
        || (sender->activeBuffs[Buff::ENWATER] && element == Element::WATER)){
            atkDmg *= 1.3;
            ravDmg *= 1.3;
        }

                        // if (sender->activeBuffs[element]){

                        // }

    //7. Check passive skill deathblow conditions
    /*
        If the target's HP is less than 1.2 times the damage that would be dealt 
        before defense and resistance, damage is increased by 50%
    */
    
    //float d6Atk = d5Atk; // implement later when passive abilities are implemented
    //float d6Rav = d5Rav;

    //8. Calculate elemental Resistances

    if (element != Element::NOELEMENT){
        //apply en- elemental statuses
        Element currElement = element;
        if (sender->activeBuffs[Buff::ENFIRE]) currElement = Element::FIRE;
        else if (sender->activeBuffs[Buff::ENFROST]) currElement = Element::ICE;
        else if (sender->activeBuffs[Buff::ENTHUNDER]) currElement = Element::LIGHTNING;
        else if (sender->activeBuffs[Buff::ENWATER]) currElement = Element::WATER;

        atkDmg *= receiver->getResistance(currElement);
        ravDmg *= receiver->getResistance(currElement);
    } 

    //"Deprotect and deshell are subtracted directly from physical and magical resistance"
    if (receiver->activeDebuffs[Debuff::DEPROTECT]){
        atkDmg *= (receiver->getResistance(Element::PHYSICAL) * 1.89);
    } else {
        atkDmg *= receiver->getResistance(Element::PHYSICAL);
    }
    if (receiver->activeDebuffs[Debuff::DESHELL]){
        ravDmg *= (receiver->getResistance(Element::MAGICAL) * 1.89);
    } else {
        ravDmg *= receiver->getResistance(Element::MAGICAL);
    }

    ravDmg *= receiver->getResistance(Element::MAGICAL);

    //9. Calculate enemy status effects (except deprotect and deshell)
    /*
            _____________________________________________
            | Type | Status Effect                 Effect |
            |=============================================|
            | Phys | Deprotect ..................... -89% |
            | Mag  | Deshell ....................... -89% |
            | Phys | Protect ..................... x 0.67 |
            | Phys | Protectra ................... x 0.50 |
            | Mag  | Shell ....................... x 0.67 |
            | Mag  | Shellra ..................... x 0.50 |
            | Both | Bar-(Element) ............... x 0.50 |
            | Both | Deathward ................... x 0.75 |
            | Both | Critical: Shield ............ x 0.85 |
            | Both | Critical: Shield II ......... x 0.70 |
            *=============================================*
    */

    float enemyStatusModAtk = 1;
    float enemyStatusModRav = 1;

    if (receiver->activeBuffs[Buff::PROTECTRA]) {enemyStatusModAtk *= 0.50;}
    else if (receiver->activeBuffs[Buff::PROTECT]) {enemyStatusModAtk *= 0.67;}

    if (receiver->activeBuffs[Buff::SHELLRA]) {enemyStatusModRav *= 0.50;}
    else if (receiver->activeBuffs[Buff::SHELL]) {enemyStatusModRav *= 0.67;}

    if (receiver->activeBuffs[Buff::BARFIRE] && element == Element::FIRE
        || receiver->activeBuffs[Buff::BARFROST] && element == Element::ICE
        || receiver->activeBuffs[Buff::BARTHUNDER] && element == Element::LIGHTNING
        || receiver->activeBuffs[Buff::BARWATER] && element == Element::WATER){
            enemyStatusModAtk *= 0.5;
            enemyStatusModRav *= 0.5;
    }

    //Im ignoring ward and shield for now.

    atkDmg *= enemyStatusModAtk;
    ravDmg *= enemyStatusModRav;

    //10. calculate bonus defense (sentinel abilities/bonus)
    /*
            _____________________________________________
            | Type | Status Effect                 Effect |
            |=============================================|
            | Both | Sentinel Role Bonus ...... -8% - 63% |
            | Both | Steelguard ............. -20% - 100% |
            | Both | Mediguard ..................... -20% |
            | Both | Vendetta ...................... -10% |
            | Both | Entrench ...................... -10% |
            | Both | Elude ......................... -20% |
            *=============================================*
    */

    float senRoleBonus = 1; //TODO: implement role bonuses and then update here

    atkDmg *= senRoleBonus;
    ravDmg *= senRoleBonus;

    //TODO: This wont work until currCommand and command timings are functioning as intended.
    //May be smarter to have a "steelguard" and "mediguard" buff on a timer.
    if (receiver->currCommand->name == "STEELGUARD") {/*find out how steelguard defense is calculated*/}
    if (receiver->currCommand->name == "MEDIGUARD") { /*d9Atk *= 0.8;*/}


    //11. Apply auto ability Fringeward 
    // (i dont even know what this is, cant be fucked to implement until way later)
    // float d10Atk = d9Atk;
    // float d10Rav = d9Rav;

    //12. Apply damage limit (implement lifted damage limit later, not sure how that works)

    if (atkDmg > 99999) atkDmg = 99999;
    if (ravDmg > 99999) ravDmg = 99999;

    //13. Account for daze staus effect
    //"If the Daze status effect is present on the target, D12 equals D11 x 2."

    if (receiver->activeDebuffs[Debuff::DAZE]){
        atkDmg *= 2;
        ravDmg *= 2;
    }

    //14. “Damage Rarely Becomes 0” passive weapon ability
    /************************************************************
    * If the effect succeeds, D13 becomes 0; else D13 equals  *
    * D12.                                                    *
    ***********************************************************
    */
    //I'll implement when weapons are in the game (or never, ts seems super niche)

    //15. apply flat damage reduction abilities (mad simple but still, need to wait for equipment to be in the game)

    //16. Lower limit:
    //Rerun all this shit to d2l and the pick a random number between 
    //that and the max for the final damage output.

    //Will do later because im getting lazy. Having non-randomness is good for testing rn anyway
    
    //apply damage FINALLY
    return (int)(atkDmg + ravDmg);
}