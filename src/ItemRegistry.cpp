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

            strncpy(weapon.ability, data[2].c_str(), sizeof(weapon.ability) - 1);
            weapon.ability[sizeof(weapon.ability) - 1] = '\0';

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