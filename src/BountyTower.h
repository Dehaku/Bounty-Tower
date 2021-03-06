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
#include "StatusEffects.h"
#include "Tiles.h"
#include "Faction.h"
#include "SaveLoad.h"


extern sf::RenderWindow window;

extern void lmbPress();
extern void critterBrain(Npc &npc, std::list<Npc> &container);
extern void critterBrain(std::list<Npc> &npcs);
extern void resetPathingController();

extern std::vector<Npc*> Squaddies;

extern std::list<Item> itemStorage;

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

class dynamicVariable
{
public:
    std::string name;
    bool toDelete;

    int varInt;
    float varFloat;

    std::string varString;

    Npc * varNpcPtr;
    Item * varItemPtr;
};

class baseMenu
{
public:
    int age;
    bool toDelete;
    std::string name;
    sf::Vector2f Pos;
    Vec3f makePos;
    Npc *npc;

    float scrollOne;
    float scrollTwo;

    std::list<dynamicVariable> vars;

    bool hasVar(std::string varName);
    dynamicVariable * getVar(std::string varName);

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

void setTileImages(std::string towerName);

StatusAspect generateRandomStatusAspectConditionOnce(int rankNum);
StatusAspect generateRandomStatusAspectConditionConstant(int rankNum);
StatusAspect generateRandomStatusAspectConstant(int rankNum);
StatusAspect generateRandomStatusAspectOnce(int rankNum);

StatusEffect generateRandomStatusEffect(RandomWeightList rankList = RandomWeightList());

Item generateRandomItem(RandomWeightList equipmentSet);

void skillMenu(Npc &npc);
void squaddieMenu(Npc &npc);
void merchantMenu(Vec3f creationPos);
void recruiterMenu(Vec3f creationPos);
void towerMenu(Vec3f creationPos);
void escapeMenu(Vec3f creationPos);
void gunModMenu(Item* item);
void itemHotBarRMBMenu(Item* item);

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
    extern bool gameBeaten;
}

extern std::vector<Tower> towers;

void loadTavern();

#endif // BOUNTYTOWER_H_INCLUDED
