#pragma once
#include <vector>
#include "Character.h"
#include "Menu.h"
#include "graphics/Texture.h"

enum CombatState {
    BATTLE,
    SCAN,
    PAUSE
};

class CombatInstance {

    public:
    

        std::vector<Character*> team;
        std::vector<Character*> enemies;

        Character* playerCharacter;

        Character* scannedEnemy = nullptr;

        CombatState state;
        bool complete;

        int scanIdx;

        Menu commandMenu;

        CombatInstance(std::vector<Character*> team, std::vector<Character*> enemies);


        void setTeam(std::vector<Character*> team);
        void setEnemies(std::vector<Character*> enemies);

        void update(float dt);
        void render(float dt);

        Texture* background;

        Texture* testlogo;
};