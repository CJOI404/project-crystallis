#pragma once

#include <vector>
#include "GlobalDefs.h"

namespace Items {

    struct Accessory {
        char* name;
        char* ability;
        int bonus;
        int price;
        int value;
        int rank;
        int levelIncrease;

    };
    struct Weapon {
        char name[32];
        char character[16];
        char abilityName[32];
        WeaponAbility ability;
        int atk;
        int rav;
        int price;
        int value;
        int rank;
        int levelIncrease;

    };

    void loadAccessories(const char* accessorySheetPath);
    void loadWeapons(const char* weaponSheetPath);

    extern std::vector<Accessory> accessoryList;
    extern std::vector<Weapon> weaponList;
}