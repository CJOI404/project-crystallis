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
        std::vector<Character*> teamList;

        CharacterState characterState;

        Role currentRole;

        const char* name;

        int health = 0;
        int maxHealth = 0;

        float stagger = 100;
        float staggerPoint = 150;
        int chainResistance = 50;

        float chainDuration = 1;
        //use this for stagger bar rendering
        float peakChainDuration = 1;

        //cut/keep decrease or increase
        int cutDiff = 0;

        bool staggered = false;

        int atkDamage = 400;
        int ravDamage  = 300;

        int atbSegments = 5;
        int atbQueueAmt = 0;
        int atbRechargeSpeed = 1;
        float currAtbVal = 0;
        int atbChargeCooldown = 1;
        float currAtbCooldownVal = 0;
        float currCommandCooldownVal = 0;

        //character cast time percentages; mods the skill useTime. off = offensive roles (com rav sab)
        //sentinal is not included since cast time is generally not really important (guard takes longer on purpose for example)
        float offCastSpeed = 1;
        float medCastSpeed = 1;
        float synCastSpeed = 1;

        int targetIndex = -1;

        // std::vector<BattleCommand*> allCharacterCommands;
        std::vector<BattleCommand*> abilities;


        //resistances to elements/attack types
        Resistance resistances[Element::ELEMENTCOUNT]{};
        //immunity to debuffs (debuff chances)
        alignas(4) float immunities[Debuff::DEBUFFCOUNT]{};

        //needed for dispel
        Buff mostRecentBuff;

        //stores active buffs/debuffs
        //supposedly PSP requires 4 byte alignment for float operations, making attribute alligned necessary
        bool activeBuffs[Buff::BUFFCOUNT]{};
        bool activeDebuffs[Debuff::DEBUFFCOUNT]{};
        alignas(4) float buffDurations[Buff::BUFFCOUNT]{};
        alignas(4) float debuffDurations[Debuff::DEBUFFCOUNT]{};

        void startAttack(int targetIndex);

        void setResistance(Element element, Resistance val);
        float getResistance(Element element);

        void setImmunity(Debuff debuff, float val);
        float getImmunity(Debuff debuff);

        void queueCommand(BattleCommand* command);

        void addBattleCommand(BattleCommand* command, int index);
        void addViableBattleCommands();

        Character();
        ~Character();

        void update(float dt) override;
        void render(float dt) override;

        void updateEffects(float dt);
        void revertDebuff(int effectIdx);
        void revertBuff(int effectIdx);
        
};