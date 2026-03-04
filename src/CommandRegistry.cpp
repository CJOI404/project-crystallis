#include "CommandRegistry.h"
#include <fstream>
#include <cstring>




CommandRegistry::CommandRegistry(){
}
CommandRegistry::~CommandRegistry(){
    for (BattleCommand* p : commandList){
        delete p;
    }
}


void CommandRegistry::loadSkills(const char* skillSheetPath){

    std::string data[20];

    std::ifstream inFS;
    std::string line;

    // std::stringstream ss(line);
    std::string item;

    inFS.open(skillSheetPath);

    if (!inFS.is_open()) {
        return;  
    }

    //skip first line (labels)
    std::getline(inFS, line);


    while (std::getline(inFS, line)) {
        int count = 0;
        size_t start = 0;
        size_t end = 0;

        while ((end = line.find(',', start)) != std::string::npos) {
            data[count++] = line.substr(start, end - start);
            start = end + 1;
        }

        if (start < line.length()) {
            data[count++] = line.substr(start);
        }

        BattleCommand* com = nullptr;
        CommandData cData = {}; 

            strncpy(cData.name, data[0].c_str(), sizeof(cData.name) - 1);
            cData.name[sizeof(cData.name) - 1] = '\0';

            cData.cost = std::atoi(data[1].c_str());

            strncpy(cData.type, data[2].c_str(), sizeof(cData.type) - 1);
            cData.type[sizeof(cData.type) - 1] = '\0';
            // cData.type = data[2];

            if (data[3] == "COMMANDO") cData.paradigm = Paradigm::COMMANDO;
            else if (data[3] == "RAVAGER") cData.paradigm = Paradigm::RAVAGER;
            else if (data[3] == "SENTINEL") cData.paradigm = Paradigm::SENTINEL;
            else if (data[3] == "MEDIC") cData.paradigm = Paradigm::MEDIC;
            else if (data[3] == "SABOTEUR") cData.paradigm = Paradigm::SABOTEUR;
            else if (data[3] == "SYNERGIST") cData.paradigm = Paradigm::SYNERGIST;

            cData.atkDmgScale = std::atof(data[4].c_str());
            cData.ravDmgScale = std::atof(data[5].c_str());
            cData.chain = std::atof(data[6].c_str());
            cData.target = std::atoi(data[7].c_str());
            cData.variation = std::atof(data[8].c_str());
            cData.duration = std::atof(data[9].c_str());
            cData.cut = std::atoi(data[10].c_str());
            cData.keep = std::atoi(data[11].c_str());
            cData.useTime = 0.5;

            if (data[12] == "Y") cData.ev = true;
            else cData.ev = false;

            if (data[13] == "Y") cData.pain = true;
            else cData.pain = false;

            if (data[14] == "Y") cData.fog = true;
            else cData.fog = false;

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
        if (strcmp(cData.type, "ATTACK") == 0) {com = new AttackCommand(cData);}

        
        if (com) commandList.push_back(com);

    }


}


