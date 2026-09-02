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
            
            if (strcmp(weapon.abilityName, "Attack: ATB Charge") == 0) weapon.ability = PassiveAbility::ATBCharge;
            else if (strcmp(weapon.abilityName, "Attack: ATB Charge II") == 0) weapon.ability = PassiveAbility::ATBCharge2;
            else if (strcmp(weapon.abilityName, "Improved Raise") == 0) weapon.ability = PassiveAbility::ImprovedRaise;
            else if (strcmp(weapon.abilityName, "Improved Raise II") == 0) weapon.ability = PassiveAbility::ImprovedRaise2;
            else if (strcmp(weapon.abilityName, "Ironstrike") == 0) weapon.ability = PassiveAbility::Ironstrike;
            else if (strcmp(weapon.abilityName, "Stagger Lock") == 0) weapon.ability = PassiveAbility::StaggerLock;
            else if (strcmp(weapon.abilityName, "Quick Stagger") == 0) weapon.ability = PassiveAbility::QuickStagger;
            else if (strcmp(weapon.abilityName, "Augment Maintenance") == 0) weapon.ability = PassiveAbility::AugmentMaintenance;
            else if (strcmp(weapon.abilityName, "Augment Maintenance II") == 0) weapon.ability = PassiveAbility::AugmentMaintenance2;
            else if (strcmp(weapon.abilityName, "Leadenstrike") == 0) weapon.ability = PassiveAbility::Leadenstrike;
            else if (strcmp(weapon.abilityName, "Paper Tiger") == 0) weapon.ability = PassiveAbility::PaperTiger;
            else if (strcmp(weapon.abilityName, "Silk Tiger") == 0) weapon.ability = PassiveAbility::SilkTiger;
            else if (strcmp(weapon.abilityName, "Chain Bonus Boost") == 0) weapon.ability = PassiveAbility::ChainBonusBoost;
            else if (strcmp(weapon.abilityName, "Chain Bonus Boost II") == 0) weapon.ability = PassiveAbility::ChainBonusBoost2;
            else if (strcmp(weapon.abilityName, "Stagger Maintenance") == 0) weapon.ability = PassiveAbility::StaggerMaintenance;
            else if (strcmp(weapon.abilityName, "Stagger Maintenance II") == 0) weapon.ability = PassiveAbility::StaggerMaintenance2;
            else if (strcmp(weapon.abilityName, "Improved Guard") == 0) weapon.ability = PassiveAbility::ImprovedGuard;
            else if (strcmp(weapon.abilityName, "Improved Guard II") == 0) weapon.ability = PassiveAbility::ImprovedGuard2;
            else if (strcmp(weapon.abilityName, "Critical: Power Surge") == 0) weapon.ability = PassiveAbility::CritPowerSurge;
            else if (strcmp(weapon.abilityName, "Critical: Power Surge II") == 0) weapon.ability = PassiveAbility::CritPowerSurge2;
            else if (strcmp(weapon.abilityName, "Enfeeblement") == 0) weapon.ability = PassiveAbility::Enfeeblement;
            else if (strcmp(weapon.abilityName, "Hindrance") == 0) weapon.ability = PassiveAbility::Hindrance;
            else if (strcmp(weapon.abilityName, "Improved Ward") == 0) weapon.ability = PassiveAbility::ImprovedWard;
            else if (strcmp(weapon.abilityName, "Improved Ward II") == 0) weapon.ability = PassiveAbility::ImprovedWard2;
            else if (strcmp(weapon.abilityName, "Critical: Shield") == 0) weapon.ability = PassiveAbility::Shield;
            else if (strcmp(weapon.abilityName, "Critical: Shield II") == 0) weapon.ability = PassiveAbility::Shield2;
            else if (strcmp(weapon.abilityName, "Siphon Boost") == 0) weapon.ability = PassiveAbility::SiphonBoost;
            else if (strcmp(weapon.abilityName, "Siphon Boost II") == 0) weapon.ability = PassiveAbility::SiphonBoost2;
            else if (strcmp(weapon.abilityName, "Defense Maintenance") == 0) weapon.ability = PassiveAbility::DefenseMaintenance;
            else if (strcmp(weapon.abilityName, "Defense Maintenance II") == 0) weapon.ability = PassiveAbility::DefenseMaintenance2;
            else if (strcmp(weapon.abilityName, "Stifled Magic") == 0) weapon.ability = PassiveAbility::StifledMagic;
            else if (strcmp(weapon.abilityName, "Fettered Magic") == 0) weapon.ability = PassiveAbility::FetteredMagic;
            else if (strcmp(weapon.abilityName, "Improved Cure") == 0) weapon.ability = PassiveAbility::ImprovedCure;
            else if (strcmp(weapon.abilityName, "Improved Cure II") == 0) weapon.ability = PassiveAbility::ImprovedCure2;
            else if (strcmp(weapon.abilityName, "Ally KO: Power Surge") == 0) weapon.ability = PassiveAbility::AllyKOPowerSurge;
            else if (strcmp(weapon.abilityName, "Ally KO: Power Surge II") == 0) weapon.ability = PassiveAbility::AllyKOPowerSurge2;
            else if (strcmp(weapon.abilityName, "Improved Debuffing") == 0) weapon.ability = PassiveAbility::ImprovedDebuffing;
            else if (strcmp(weapon.abilityName, "Improved Debuffing II") == 0) weapon.ability = PassiveAbility::ImprovedDebuffing2;
            else if (strcmp(weapon.abilityName, "Stagger: TP Charge") == 0) weapon.ability = PassiveAbility::TPCharge;
            else if (strcmp(weapon.abilityName, "Stagger: TP Charge II") == 0) weapon.ability = PassiveAbility::TPCharge2;
            else if (strcmp(weapon.abilityName, "Improved Debilitation") == 0) weapon.ability = PassiveAbility::ImprovedDebilitation;
            else if (strcmp(weapon.abilityName, "Improved Debilitation II") == 0) weapon.ability = PassiveAbility::ImprovedDebilitation2;
            else if (strcmp(weapon.abilityName, "Improved Counter") == 0) weapon.ability = PassiveAbility::ImprovedCounter;
            else if (strcmp(weapon.abilityName, "Improved Counter II") == 0) weapon.ability = PassiveAbility::ImprovedCounter2;
            else weapon.ability = PassiveAbility::NOABILITY;


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