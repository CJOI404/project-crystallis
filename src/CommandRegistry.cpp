#include "CommandRegistry.h"
#include <fstream>
#include <sstream>


namespace Commands{

    std::vector<BattleCommand*> commandList;

    void loadSkills(const char* skillSheetPath){
        std::string data[20];

        std::ifstream inFS;
        std::string line;

        std::stringstream ss(line);
        std::string item;

        inFS.open(skillSheetPath);

        //skip first line (labels)
        std::getline(inFS, line);


        while (std::getline(inFS, line)) {
            int count = 0;
            // while (std::getline(ss, item, ',') && count < 20){
            //     data[count++] = item;
            // }
            size_t start = 0;
            size_t end = 0;

            while ((end = line.find(',', start)) != std::string::npos) {
                data[count++] = line.substr(start, end - start);
                start = end + 1;
            }

            // Add the last value (after the final comma)
            if (start < line.length()) {
                data[count++] = line.substr(start);
            }

            BattleCommand* com;
            CommandData cData;

                cData.name = data[0];
                cData.cost = std::stoi(data[1]);
                cData.type = data[2];

                if (data[3] == "COMMANDO") cData.paradigm = Paradigm::COMMANDO;
                else if (data[3] == "RAVAGER") cData.paradigm = Paradigm::RAVAGER;
                else if (data[3] == "SENTINEL") cData.paradigm = Paradigm::SENTINEL;
                else if (data[3] == "MEDIC") cData.paradigm = Paradigm::MEDIC;
                else if (data[3] == "SABOTEUR") cData.paradigm = Paradigm::SABOTEUR;
                else if (data[3] == "SYNERGIST") cData.paradigm = Paradigm::SYNERGIST;

                cData.atkDmgScale = std::stof(data[4]);
                cData.ravDmgScale = std::stof(data[5]);
                cData.chain = std::stof(data[6]);
                cData.target = std::stoi(data[7]);
                cData.variation = std::stof(data[8]);
                cData.duration = std::stof(data[9]);
                cData.cut = std::stoi(data[10]);
                cData.keep = std::stoi(data[11]);
                cData.useTime = 0.5;

                if (data[12] == "Y") cData.ev = true;
                else if (data[12] == "N") cData.ev = false;

                if (data[13] == "Y") cData.pain = true;
                else if (data[13] == "N") cData.pain = false;

                if (data[14] == "Y") cData.fog = true;
                else if (data[14] == "N") cData.fog = false;

                cData.rise = std::stof(data[15]);

                if (data[16] == "F") {cData.element = Element::FIRE;}
                else if (data[16] == "I") {cData.element = Element::ICE;}
                else if (data[16] == "T") {cData.element = Element::LIGHTNING;}
                else if (data[16] == "W") {cData.element = Element::WATER;}
                else if (data[16] == "A") {cData.element = Element::WIND;}
                else if (data[16] == "E") {cData.element = Element::EARTH;}
                else cData.element = Element::NOELEMENT;

                //17 = effect (debuff)
                //18 = chance (debuff)
                //19 = length (debuff)


            //data[2] determines type
            if (cData.type == "ATTACK") {com = new AttackCommand(cData);}

            
            commandList.push_back(com);

        }


    }

}