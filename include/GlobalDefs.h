#pragma once
#include <string>

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

struct Paradigm{
    const char* name;
    Role r1;
    Role r2;
    Role r3;
};

struct Effect{
    Debuff debuff;
    float length;
};

const char* roleToString(Role r);

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
    Debuff effect;
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
    LIGHTGREEN = 0xFF63c47c
};