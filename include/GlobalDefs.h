#pragma once
#include <string>
#include <cstdint>

#define MAX_ENTITIES = 100;

enum WeaponAbility{
    ATBCharge,
    ATBCharge2,
    ImprovedRaise,
    ImprovedRaise2,
    Leadenstrike,
    Ironstrike,
    StaggerLock,
    QuickStagger,
    AugmentMaintenance,
    AugmentMaintenance2,
    SilkTiger,
    ChainBonusBoost,
    ChainBonusBoost2,
    StaggerMaintenance,
    StaggerMaintenance2,
    ImprovedGuard,
    ImprovedGuard2,
    CritPowerSurge,             //IMPLEMENTED! in dmg calculation
    CritPowerSurge2,            //IMPLEMENTED! in dmg calculation
    AllyKOPowerSurge,
    AllyKOPowerSurge2,
    Enfeeblement,
    Hindrance,
    PaperTiger,
    ImprovedWard,
    ImprovedWard2,
    Shield,
    Shield2,
    SiphonBoost,
    SiphonBoost2,
    DefenseMaintenance,
    DefenseMaintenance2,
    StifledMagic,
    FetteredMagic,
    ImprovedCure,
    ImprovedCure2,
    ImprovedDebuffing,
    ImprovedDebuffing2,
    TPCharge,
    TPCharge2,
    ImprovedDebilitation,
    ImprovedDebilitation2,
    ImprovedCounter,
    ImprovedCounter2
};

enum Element{
    FIRE,
    ICE,
    LIGHTNING,
    WATER,
    WIND,
    EARTH,
    PHYSICAL,
    MAGICAL,
    NOELEMENT,
    ELEMENTCOUNT
};

enum Debuff{
    DEBRAVE,
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
    PROVOKE,
    DISPEL,
    NODEBUFF,
    DEBUFFCOUNT
};

enum Buff{
    BRAVERY,
    BRAVERA,
    FAITH,
    FAITHRA,
    HASTE,
    VIGILANCE,
    ENFIRE,
    ENFROST,
    ENTHUNDER,
    ENWATER,
    PROTECT,
    PROTECTRA,
    SHELL,
    SHELLRA,
    BARFIRE,
    BARFROST,
    BARTHUNDER,
    BARWATER,
    VEIL,
    NOBUFF,
    BUFFCOUNT
};

enum Role{
    COMMANDO,
    RAVAGER,
    SENTINEL,
    MEDIC,
    SABOTEUR,
    SYNERGIST
};

enum Resistance{
    IMMUNE,
    RESISTANT,
    HALVED,
    NORMAL,
    WEAK
};

struct Paradigm{
    const char* name;
    Role roles[3];
    // Role r1;
    // Role r2;
    // Role r3;
};

const char* roleToString(Role r);
const char* debuffToString(Debuff d);
const char* buffToString(Buff b);

float getRandomFloat();

struct CommandData{
    char name[16];
    int cost;
    char type[8];
    Role role;
    float atkDmgScale;
    float ravDmgScale;
    float chain;
    int target;
    float variation;
    float duration;
    int cut;
    int keep;
    float useTime;
    bool ev;
    bool pain;
    bool fog;
    float rise;
    Element element;
    Debuff debuff;
    Buff buff;
    float chance;
    int length;

};

//Colours

enum Colours{
    RED = 0xFF0000FF,
    BLUE = 0xFFFF0000,
    LIGHTBLUE = 0xFFFFDD1C,
    LIGHTGREY = 0xFF8a928c,
    STAGGERBAR = 0xFF67b4e6,
    LIGHTGREEN = 0xFF63c47c,
    DARKGREY = 0xFF2e2e34,
    MIDGREY = 0xFF45454d
};

typedef struct {
    unsigned short u, v;
    short x, y, z;
} Vertex;

typedef struct {
    float u, v;
    unsigned int colour;
    float x, y, z;
} TextureVertex;

typedef struct {
    float x, y, w, h;
    unsigned int colour;
} Rect;




