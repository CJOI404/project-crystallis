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
    FAITH,
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

enum Paradigm{
    COMMANDO,
    RAVAGER,
    SENTINEL,
    MEDIC,
    SABOTEUR,
    SYNERGIST
};

struct CommandData{
    std::string name;
    int cost;
    std::string type;
    Paradigm paradigm;
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