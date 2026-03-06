#include "GlobalDefs.h"
#include <random>

const char* roleToString(Role r) {
    switch (r) {
        case COMMANDO:      return "COM";
        case RAVAGER:       return "RAV";
        case MEDIC:         return "MED";
        case SENTINEL:      return "SEN";
        case SABOTEUR:      return "SAB";
        case SYNERGIST:     return "SYN";
        default:            return "???";
    }
}

float getRandomFloat() {
    // Use static variables for the random device, engine, and distribution
    // to ensure they are initialized only once and maintain state across calls.
    static std::random_device rd;
    static std::mt19937 engine(rd()); // Seed the engine with a non-deterministic value
    // Define the distribution for the range [0.0, 1.0)
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f); 
    
    return dist(engine);
}

// int calculateDamage(Character* sender, Character* receiver, float atkDmgScale, float ravDmgScale, float variation){
    
        
//             //simple dmg calculation
//         // receiver->health -= sender->atkDamage * atkDmgScale * (receiver->stagger/100);
//         // receiver->health -= sender->ravDamage * ravDmgScale * (receiver->stagger/100);

            
//             //full dmg calculation (work in progress)

//         //Base damage:
//         float atkDmg = (sender->atkDamage * atkDmgScale);
//         float ravDmg = (sender->ravDamage * ravDmgScale);

//         //1. check the following passive abilities, multiply base dmg to get D1
//         /*
//             Adrenaline                  *1.2 (+20%)
//             Critical: Power Surge       *1.2 (+20%)
//             Critical: Power Surge II    *1.4 (+40%)
//             Ally KO: Power Surge        *1.1 - *2.2 (+110% - 220%)
//             Ally KO: Power Surge II     *1.3 - *2.6 (+130% - 260%)
//             High HP: Power Surge        *1.2 (+20%)
//             Low HP: Power Surge         *1.5 (+50%)
//         */

//         float weaponAbilitiesMod = 1; //change to sum of weapon abilities when that is implemented

//         atkDmg *= weaponAbilitiesMod;
//         ravDmg *= weaponAbilitiesMod;
       
//         //2.Sum applicable status effects and multiply d1 to get d2
//         //ONLY ONE VERSION OF EACH EFFECT CAN BE USED
//         /*
//             Bravery (atk only)          *1.4
//             Bravera (atk only)          *1.8
//             Debrave (atk only)          *0.1
//             Faith (rav only)            *1.4
//             Faithra (rav only)          *1.8
//             Defaith (rav only)          *0.1
       
//         */
//         float statusEffectsModAtk = 1; 
//         float statusEffectsModRav = 1;

//         if (sender->activeDebuffs[DEBRAVE]) statusEffectsModAtk *= 0.1;
//         if (sender->activeBuffs[BRAVERY]) statusEffectsModAtk *= 1.4;
//         if (sender->activeBuffs[BRAVERA]) statusEffectsModAtk *= 1.8;

//         if (sender->activeDebuffs[DEFAITH]) statusEffectsModRav *= 0.1;
//         if (sender->activeBuffs[FAITH]) statusEffectsModRav *= 1.4;
//         if (sender->activeBuffs[FAITHRA]) statusEffectsModRav *= 1.8;

//         atkDmg *= statusEffectsModAtk;
//         ravDmg *= statusEffectsModRav;

//         //3. Get lower limit for random damage factor
//         float atkDmgL = (1 - variation) * atkDmg;
//         float ravDmgL = (1 - variation) * atkDmg;

//         //4. Apply chain to d2 get d3
//         atkDmg *= (receiver->stagger/100);
//         ravDmg *= (receiver->stagger/100);

//         //5.Calculate bonuses 
//         // (will skip for now. DONT FORGET TO ALL COMMANDO BONUS HERE WHEN DOING ROLE BONUSES)
//         // The rest of these seem stupid and not worth implementing, except maybe vendetta and entrench
//         /*
//             | Type | Ability                                  Effect |
//             |========================================================|
//             | Both | Commando Role Bonus ................ +5% - 180% |
//             | Both | X Elemental Damage% Up ............  +20% - 50% |
//             | Phys | Enfeeblement ............................. -50% |
//             | Phys | Hindrance ................................ -30% |
//             | Mag  | Stiffed Magic ............................ -50% |
//             | Mag  | Fettered Magic ........................... -30% |
//             | Phys | Vendetta .......................... +10% - 300% |
//             | Phys | Entrench .......................... +30% - 150% |
//             *========================================================*
//         */

//         float comRoleBonus = 1; //Ignoring role bonuses for now

//         atkDmg *= comRoleBonus;
//         ravDmg *= comRoleBonus;

//         //6. Calculate element bonus
//         //This checks if the Enhance Element buff aligns with the attack element

//         if ((sender->activeBuffs[Buff::ENFIRE] && element == Element::FIRE)
//             || (sender->activeBuffs[Buff::ENFROST] && element == Element::ICE)
//             || (sender->activeBuffs[Buff::ENTHUNDER] && element == Element::LIGHTNING)
//             || (sender->activeBuffs[Buff::ENWATER] && element == Element::WATER)){
//                 atkDmg *= 1.3;
//                 ravDmg *= 1.3;
//          }

//                             // if (sender->activeBuffs[element]){

//                             // }

//         //7. Check passive skill deathblow conditions
//         /*
//             If the target's HP is less than 1.2 times the damage that would be dealt 
//             before defense and resistance, damage is increased by 50%
//         */
        
//         //float d6Atk = d5Atk; // implement later when passive abilities are implemented
//         //float d6Rav = d5Rav;

//         //8. Calculate elemental Resistances

//         if (element != Element::NOELEMENT){
//             atkDmg *= receiver->getResistance(element);
//             ravDmg *= receiver->getResistance(element);
//         } 

//         //"Deprotect and deshell are subtracted directly from physical and magical resistance"
//         if (receiver->activeDebuffs[Debuff::DEPROTECT]){
//             atkDmg *= (receiver->getResistance(Element::PHYSICAL) * 1.89);
//         } else {
//             atkDmg *= receiver->getResistance(Element::PHYSICAL);
//         }
//         if (receiver->activeDebuffs[Debuff::DESHELL]){
//             ravDmg *= (receiver->getResistance(Element::MAGICAL) * 1.89);
//         } else {
//             ravDmg *= receiver->getResistance(Element::MAGICAL);
//         }

//         ravDmg *= receiver->getResistance(Element::MAGICAL);

//         //9. Calculate enemy status effects (except deprotect and deshell)
//         /*
//                 _____________________________________________
//                 | Type | Status Effect                 Effect |
//                 |=============================================|
//                 | Phys | Deprotect ..................... -89% |
//                 | Mag  | Deshell ....................... -89% |
//                 | Phys | Protect ..................... x 0.67 |
//                 | Phys | Protectra ................... x 0.50 |
//                 | Mag  | Shell ....................... x 0.67 |
//                 | Mag  | Shellra ..................... x 0.50 |
//                 | Both | Bar-(Element) ............... x 0.50 |
//                 | Both | Deathward ................... x 0.75 |
//                 | Both | Critical: Shield ............ x 0.85 |
//                 | Both | Critical: Shield II ......... x 0.70 |
//                 *=============================================*
//         */

//         float enemyStatusModAtk = 1;
//         float enemyStatusModRav = 1;

//         if (receiver->activeBuffs[Buff::PROTECTRA]) {enemyStatusModAtk *= 0.50;}
//         else if (receiver->activeBuffs[Buff::PROTECT]) {enemyStatusModAtk *= 0.67;}

//         if (receiver->activeBuffs[Buff::SHELLRA]) {enemyStatusModRav *= 0.50;}
//         else if (receiver->activeBuffs[Buff::SHELL]) {enemyStatusModRav *= 0.67;}

//         if (sender->activeBuffs[Buff::BARFIRE] && element == Element::FIRE
//             || sender->activeBuffs[Buff::BARFROST] && element == Element::ICE
//             || sender->activeBuffs[Buff::BARTHUNDER] && element == Element::LIGHTNING
//             || sender->activeBuffs[Buff::BARWATER] && element == Element::WATER){
//                 enemyStatusModAtk *= 0.5;
//                 enemyStatusModRav *= 0.5;
//         }

//         //Im ignoring ward and shield for now.

//         atkDmg *= enemyStatusModAtk;
//         ravDmg *= enemyStatusModRav;

//         //10. calculate bonus defense (sentinel abilities/bonus)
//         /*
//                 _____________________________________________
//                 | Type | Status Effect                 Effect |
//                 |=============================================|
//                 | Both | Sentinel Role Bonus ...... -8% - 63% |
//                 | Both | Steelguard ............. -20% - 100% |
//                 | Both | Mediguard ..................... -20% |
//                 | Both | Vendetta ...................... -10% |
//                 | Both | Entrench ...................... -10% |
//                 | Both | Elude ......................... -20% |
//                 *=============================================*
//         */

//         float senRoleBonus = 1; //TODO: implement role bonuses and then update here

//         atkDmg *= senRoleBonus;
//         ravDmg *= senRoleBonus;

//         //TODO: This wont work until currCommand and command timings are functioning as intended.
//         //May be smarter to have a "steelguard" and "mediguard" buff on a timer.
//         if (receiver->currCommand->name == "STEELGUARD") {/*find out how steelguard defense is calculated*/}
//         if (receiver->currCommand->name == "MEDIGUARD") { /*d9Atk *= 0.8;*/}


//         //11. Apply auto ability Fringeward 
//         // (i dont even know what this is, cant be fucked to implement until way later)
//         // float d10Atk = d9Atk;
//         // float d10Rav = d9Rav;

//         //12. Apply damage limit (implement lifted damage limit later, not sure how that works)

//         if (atkDmg > 99999) atkDmg = 99999;
//         if (ravDmg > 99999) ravDmg = 99999;

//         //13. Account for daze staus effect
//         //"If the Daze status effect is present on the target, D12 equals D11 x 2."

//         if (receiver->activeDebuffs[Debuff::DAZE]){
//             atkDmg *= 2;
//             ravDmg *= 2;
//         }

//         //14. “Damage Rarely Becomes 0” passive weapon ability
//         /************************************************************
//         * If the effect succeeds, D13 becomes 0; else D13 equals  *
//         * D12.                                                    *
//         ***********************************************************
//         */
//        //I'll implement when weapons are in the game (or never, ts seems super niche)

//        //15. apply flat damage reduction abilities (mad simple but still, need to wait for equipment to be in the game)

//        //16. Lower limit:
//        //Rerun all this shit to d2l and the pick a random number between 
//        //that and the max for the final damage output.

//        //Will do later because im getting lazy. Having non-randomness is good for testing rn anyway

//        return (int)(atkDmg + ravDmg);
// }
