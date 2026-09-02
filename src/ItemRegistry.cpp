#include "ItemRegistry.h"
#include <string>
#include <fstream>
#include <cstring>

namespace Items {

    // struct Accessory {
    //     const char* name;
    //     const char* ability;
    //     int bonus;
    //     int price;
    //     int value;
    //     int rank;
    //     int levelIncrease;

    // };
    // struct Weapon {
    //     const char* name;
    //     const char* character;
    //     const char* ability;
    //     int atk;
    //     int rav;
    //     int price;
    //     int value;
    //     int rank;
    //     int levelIncrease;

    // };

    std::vector<Accessory> accessoryList;
    std::vector<Weapon> weaponList;

    void loadAccessories(const char* accessorySheetPath){

    }
    void loadWeapons(const char* weaponSheetPath){
        std::string data[9];

        std::ifstream inFS;
        std::string line;

        std::string item;

        inFS.open(weaponSheetPath);

        if (!inFS.is_open()) {
            return;  
        }

        //skip first line (labels)
        std::getline(inFS, line);

        while (std::getline(inFS, line)) {
            int count = 0;
            size_t start = 0;
            size_t end = 0;

            while ((end = line.find(',', start)) != std::string::npos && count < 8 ) {
                data[count++] = line.substr(start, end - start);
                start = end + 1;
            }

            if (start < line.length()) {
                data[count++] = line.substr(start);
            }

            Weapon weapon;
            
            strncpy(weapon.name, data[0].c_str(), sizeof(weapon.name) - 1);
            weapon.name[sizeof(weapon.name) - 1] = '\0';

            strncpy(weapon.character, data[1].c_str(), sizeof(weapon.character) - 1);
            weapon.character[sizeof(weapon.character) - 1] = '\0';

            strncpy(weapon.abilityName, data[2].c_str(), sizeof(weapon.abilityName) - 1);
            weapon.abilityName[sizeof(weapon.abilityName) - 1] = '\0';
            
            if (weapon.abilityName == "Attack: ATB Charge") weapon.ability = WeaponAbility::ATBCharge;
            else if (weapon.abilityName == "Attack: ATB Charge II") weapon.ability = WeaponAbility::ATBCharge2;
            else if (weapon.abilityName == "Improved Raise") weapon.ability = WeaponAbility::ImprovedRaise;
            else if (weapon.abilityName == "Improved Raise II") weapon.ability = WeaponAbility::ImprovedRaise2;
            else if (weapon.abilityName == "Ironstrike") weapon.ability = WeaponAbility::Ironstrike;
            else if (weapon.abilityName == "Stagger Lock") weapon.ability = WeaponAbility::StaggerLock;
            else if (weapon.abilityName == "Quick Stagger") weapon.ability = WeaponAbility::QuickStagger;
            else if (weapon.abilityName == "Augment Maintenance") weapon.ability = WeaponAbility::AugmentMaintenance;
            else if (weapon.abilityName == "Augment Maintenance II") weapon.ability = WeaponAbility::AugmentMaintenance2;
            else if (weapon.abilityName == "Leadenstrike") weapon.ability = WeaponAbility::Leadenstrike;
            else if (weapon.abilityName == "Paper Tiger") weapon.ability = WeaponAbility::PaperTiger;
            else if (weapon.abilityName == "Silk Tiger") weapon.ability = WeaponAbility::SilkTiger;
            else if (weapon.abilityName == "Chain Bonus Boost") weapon.ability = WeaponAbility::ChainBonusBoost;
            else if (weapon.abilityName == "Chain Bonus Boost II") weapon.ability = WeaponAbility::ChainBonusBoost2;
            else if (weapon.abilityName == "Stagger Maintenance") weapon.ability = WeaponAbility::StaggerMaintenance;
            else if (weapon.abilityName == "Stagger Maintenance II") weapon.ability = WeaponAbility::StaggerMaintenance2;
            else if (weapon.abilityName == "Improved Guard") weapon.ability = WeaponAbility::ImprovedGuard;
            else if (weapon.abilityName == "Improved Guard II") weapon.ability = WeaponAbility::ImprovedGuard2;
            else if (weapon.abilityName == "Critical: Power Surge") weapon.ability = WeaponAbility::CritPowerSurge;
            else if (weapon.abilityName == "Critical: Power Surge II") weapon.ability = WeaponAbility::CritPowerSurge2;
            else if (weapon.abilityName == "Enfeeblement") weapon.ability = WeaponAbility::Enfeeblement;
            else if (weapon.abilityName == "Hindrance") weapon.ability = WeaponAbility::Hindrance;
            else if (weapon.abilityName == "Improved Ward") weapon.ability = WeaponAbility::ImprovedWard;
            else if (weapon.abilityName == "Improved Ward II") weapon.ability = WeaponAbility::ImprovedWard2;
            else if (weapon.abilityName == "Critical: Shield") weapon.ability = WeaponAbility::Shield;
            else if (weapon.abilityName == "Critical: Shield II") weapon.ability = WeaponAbility::Shield2;
            else if (weapon.abilityName == "Siphon Boost") weapon.ability = WeaponAbility::SiphonBoost;
            else if (weapon.abilityName == "Siphon Boost II") weapon.ability = WeaponAbility::SiphonBoost2;
            else if (weapon.abilityName == "Defense Maintenance") weapon.ability = WeaponAbility::DefenseMaintenance;
            else if (weapon.abilityName == "Defense Maintenance II") weapon.ability = WeaponAbility::DefenseMaintenance2;
            else if (weapon.abilityName == "Stifled Magic") weapon.ability = WeaponAbility::StifledMagic;
            else if (weapon.abilityName == "Fettered Magic") weapon.ability = WeaponAbility::FetteredMagic;
            else if (weapon.abilityName == "Improved Cure") weapon.ability = WeaponAbility::ImprovedCure;
            else if (weapon.abilityName == "Improved Cure II") weapon.ability = WeaponAbility::ImprovedCure2;
            else if (weapon.abilityName == "Ally KO: Power Surge") weapon.ability = WeaponAbility::AllyKOPowerSurge;
            else if (weapon.abilityName == "Ally KO: Power Surge II") weapon.ability = WeaponAbility::AllyKOPowerSurge2;
            else if (weapon.abilityName == "Improved Debuffing") weapon.ability = WeaponAbility::ImprovedDebuffing;
            else if (weapon.abilityName == "Improved Debuffing II") weapon.ability = WeaponAbility::ImprovedDebuffing2;
            else if (weapon.abilityName == "Stagger: TP Charge") weapon.ability = WeaponAbility::TPCharge;
            else if (weapon.abilityName == "Stagger: TP Charge II") weapon.ability = WeaponAbility::TPCharge2;
            else if (weapon.abilityName == "Improved Debilitation") weapon.ability = WeaponAbility::ImprovedDebilitation;
            else if (weapon.abilityName == "Improved Debilitation II") weapon.ability = WeaponAbility::ImprovedDebilitation2;
            else if (weapon.abilityName == "Improved Counter") weapon.ability = WeaponAbility::ImprovedCounter;
            else if (weapon.abilityName == "Improved Counter II") weapon.ability = WeaponAbility::ImprovedCounter2;


            weapon.atk = std::atoi(data[3].c_str());
            weapon.rav = std::atoi(data[4].c_str());
            weapon.price = std::atoi(data[5].c_str());
            weapon.value = std::atoi(data[6].c_str());
            weapon.rank = std::atoi(data[7].c_str());
            weapon.levelIncrease = std::atoi(data[8].c_str());
        
            weaponList.push_back(weapon);
        }
    }

}