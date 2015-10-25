#ifndef BOUNTYTOWER_H_INCLUDED
#define BOUNTYTOWER_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "Sounds.h"
#include "Textures.h"
#include "util.h"
#include "Galaxy.h"
#include "globalvars.h"
#include "InputState.h"
#include "menus.h"
#include "Camera.h"
#include "math.h"
#include "Tiles.h"
#include "Faction.h"

extern sf::RenderWindow window;

extern void lmbPress();
extern void critterBrain(Npc &npc, std::list<Npc> &container);
extern void critterBrain(std::list<Npc> &npcs);
extern void resetPathingController();

extern std::vector<Npc*> Squaddies;

bool onScreen(sf::Vector2f vPos);

void screenShake(float intensity);
void renderScreenShake();

void bountyTowerSetup();
void bountyTowerLoop();

void buildTower(std::string towerName);

void towerVictory();


int totalLevelXp(int level);
int nextLevelXpRequired(int level);
int getFloorDifficulty(float currentFloor, float towerFloors, float towerDifficulty);


int getLivingFactionMemberCount(std::string faction);
int getFactionMemberCount(std::string faction);

class baseMenu
{
public:
    int age;
    bool toDelete;
    std::string name;
    sf::Vector2f Pos;
    Vec3f makePos;
    Npc *npc;

    baseMenu();

};

extern std::list<baseMenu> menus;

class Tower
{
public:
    sf::Texture *tex;
    std::string name;
    int mapID; // For loading from the maps.
    int difficulty;
    int minioncount;
    std::string bountyTarget;
    int bountyPay;
    int floors;
    Tower();
};

void bountyBrain(Npc &npc, std::list<Npc> &container);





namespace bountytower
{
    extern bool elevatoravailable;
    extern bool towerlingassault;
    extern bool bountytower;
    extern bool pausewaves;
    extern std::string towerLoaded;
    extern Tower * currentTower;
    extern int switchesRemain;
    extern bool towerVictory;
    extern bool floorCleared;
}

extern std::vector<Tower> towers;

#endif // BOUNTYTOWER_H_INCLUDED
