#include "CommandRegistry.h"
#include <fstream>
#include <cstring>




// CommandRegistry::CommandRegistry(){
// }
// CommandRegistry::~CommandRegistry(){
//     for (BattleCommand* p : commandList){
//         delete p;
//     }
// }

namespace Commands{

    std::vector<BattleCommand*> commandList;

    void loadSkills(const char* skillSheetPath){

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

                if (data[3] == "COMMANDO") cData.role = Role::COMMANDO;
                else if (data[3] == "RAVAGER") cData.role = Role::RAVAGER;
                else if (data[3] == "SENTINEL") cData.role = Role::SENTINEL;
                else if (data[3] == "MEDIC") cData.role = Role::MEDIC;
                else if (data[3] == "SABOTEUR") cData.role = Role::SABOTEUR;
                else if (data[3] == "SYNERGIST") cData.role = Role::SYNERGIST;

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

                if (data[17] == "DEPRO") {cData.debuff = Debuff::DEPROTECT;}
                else if (data[17] == "DESHL") {cData.debuff = Debuff::DESHELL;}
                else if (data[17] == "WEAK") {cData.debuff = Debuff::IMPERIL;}
                else if (data[17] == "POISON") {cData.debuff = Debuff::POISON;}
                else if (data[17] == "SLOW") {cData.debuff = Debuff::SLOW;}
                else if (data[17] == "FOG") {cData.debuff = Debuff::FOG;}
                else if (data[17] == "PAIN") {cData.debuff = Debuff::PAIN;}
                else if (data[17] == "CURSE") {cData.debuff = Debuff::CURSE;}
                else if (data[17] == "DAZE") {cData.debuff = Debuff::DAZE;}
                else if (data[17] == "PROV") {cData.debuff = Debuff::PROVOKE;}
                else if (data[17] == "DSPEL") {cData.debuff = Debuff::DISPEL;}
                else {cData.debuff = Debuff::NODEBUFF;}

                if (data[17] == "BRAVE") {cData.buff = Buff::BRAVERY;}
                else if (data[17] == "BRVRA") {cData.buff = Buff::BRAVERA;}
                else if (data[17] == "FAITH") {cData.buff = Buff::FAITH;}
                else if (data[17] == "FAIDA") {cData.buff = Buff::FAITHRA;}
                else if (data[17] == "HASTE") {cData.buff = Buff::HASTE;}
                else if (data[17] == "VIGIL") {cData.buff = Buff::VIGILANCE;}
                else if (data[17] == "ENFIR") {cData.buff = Buff::ENFIRE;}
                else if (data[17] == "ENBLZ") {cData.buff = Buff::ENFROST;}
                else if (data[17] == "ENTHN") {cData.buff = Buff::ENTHUNDER;}
                else if (data[17] == "ENWTR") {cData.buff = Buff::ENWATER;}
                else if (data[17] == "PROT") {cData.buff = Buff::PROTECT;}
                else if (data[17] == "PRTRA") {cData.buff = Buff::PROTECTRA;}
                else if (data[17] == "SHELL") {cData.buff = Buff::SHELL;}
                else if (data[17] == "SHLDA") {cData.buff = Buff::SHELLRA;}
                else if (data[17] == "HLFFR") {cData.buff = Buff::BARFIRE;}
                else if (data[17] == "HLFBZ") {cData.buff = Buff::BARFROST;}
                else if (data[17] == "HLFTH") {cData.buff = Buff::BARTHUNDER;}
                else if (data[17] == "HLFWT") {cData.buff = Buff::BARWATER;}
                else if (data[17] == "VEIL") {cData.buff = Buff::VEIL;}
                else {cData.buff = Buff::NOBUFF;}




                cData.chance = std::atof(data[18].c_str());

                cData.length = std::atoi(data[19].c_str());




                //17 = effect (debuff)
                //18 = chance (debuff)
                //19 = length (debuff)


            com = new AttackCommand(cData);

            
            if (com) commandList.push_back(com);

        }


    }

}


