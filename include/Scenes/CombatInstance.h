#pragma once
#include <vector>
#include "Entities/Character.h"
#include "Entities/Menu.h"
#include "graphics/Assets/Texture.h"
#include "GlobalDefs.h"
#include "Scenes/Scene.h"

enum CombatState {
    BATTLE,
    SCAN,
    PAUSE
};

class CombatInstance : public Scene {

    public:

        SceneID sceneID = SceneID::COMBAT_INSTANCE;

        std::vector<Character*> team;
        std::vector<Character*> enemies;

        // Character* playerCharacter;

        Character* scannedEnemy = nullptr;

        CombatState state;
        bool complete;

        int scanIdx;

        Menu commandMenu;

        CombatInstance();


        void setTeam(std::vector<Character*> team);
        void setEnemies(std::vector<Character*> enemies);

        void update(float dt) override;
        void render(float dt) override;
        void unload() override;

        Texture* background;

        Texture* testlogo;


        Character playerCharacter;
        Character character2;
        Character character3;

        Character enemy;
        Character enemy2;
};