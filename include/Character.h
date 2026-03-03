#pragma once

#include "GameActor.h"
#include "BattleCommand.h"
#include "GlobalDefs.h"
#include <string>
#include <queue>

enum CharacterState{
    Attacking,
    Waiting,
    AttackReady,
    AttackCooldown
};

class Character : public GameActor {


    public:
        BattleCommand* currCommand = nullptr;
        BattleCommand* prevCommand = nullptr;

        std::vector<BattleCommand*> commandQueue;
        std::vector<Character*> enemyList;

        CharacterState characterState;

        std::string name = "";

        int health = 0;

        float stagger = 100;
        float staggerPoint = 150;
        int chainResistance = 50;

        float chainDuration = 0;

        // int baseChainFallSpeed = 1;
        // float currChainFallSpeed = 1;

        bool staggered = false;

        int atkDamage = 10;
        int ravDamage  = 8;

        int atbSegments = 5;
        int atbRechargeSpeed = 1;
        float currAtbVal = 0;
        int atbChargeCooldown = 2;
        float currAtbCooldownVal = 0;
        float currCommandCooldownVal = 0;

        int targetIndex = -1;

        //resistances to elements/attack types
        float resistances[Element::ELEMENTCOUNT]{};
        //immunity to debuffs (debuff chances)
        float immunities[Debuff::DEBUFFCOUNT]{};

        //stores active buffs/debuffs
        bool activeBuffs[Buff::BUFFCOUNT]{};
        bool activeDebuffs[Debuff::DEBUFFCOUNT]{};

        void startAttack(int targetIndex);

        void setResistance(Element element, float val);
        float getResistance(Element element);

        void setImmunity(Debuff debuff, float val);
        float getImmunity(Debuff debuff);

        Character();

        void update(float dt) override;
        
};