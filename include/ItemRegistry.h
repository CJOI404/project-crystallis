#pragma once

#include <vector>

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
        char ability[32];
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