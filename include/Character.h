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

        const char* name;

        int health = 0;
        int maxHealth = 0;

        float stagger = 100;
        float staggerPoint = 150;
        int chainResistance = 50;

        float chainDuration = 1;
        //use this for stagger bar rendering
        float peakChainDuration = 1;


        bool staggered = false;

        int atkDamage = 400;
        int ravDamage  = 300;

        int atbSegments = 5;
        int atbRechargeSpeed = 1;
        float currAtbVal = 0;
        int atbChargeCooldown = 2;
        float currAtbCooldownVal = 0;
        float currCommandCooldownVal = 0;

        //character cast time percentages; mods the skill useTime. off = offensive roles (com rav sab)
        //sentinal is not included since cast time is generally not really important (guard takes longer on purpose for example)
        float offCastSpeed = 1;
        float medCastSpeed = 1;
        float synCastSpeed = 1;

        int targetIndex = -1;

        BattleCommand* abilities[10];

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

        void addBattleCommand(BattleCommand* command, int index);

        Character();
        ~Character();

        void update(float dt) override;
        
};