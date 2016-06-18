#ifndef __FACTION_H_INCLUDED__
#define __FACTION_H_INCLUDED__

#include "Items.h"
#include "Effects.h"
#include "InputState.h"
#include "JobManager.h"
#include "Text.h"
#include "defs.h"
#include "Tiles.h"
#include "Networking.h"
#include "menus.h"
#include "Skills.h"
#include "BountyTower.h"

#include <list>
#include <set>

// TODO: Make it so faction units cannot harm eachother,
// Do the same with Allied Factions.
class Faction;

extern Faction *conFact;

struct skillKeepInfo
{
    Npc * user;
    std::string skillName;
    sf::Vector2f usePos;
    sf::Vector2f heldPos;
    bool toDelete;
    bool legal;
    int age;
    skillKeepInfo();
};
extern std::vector<skillKeepInfo> skillKeeps;

void skillKeepLoop();
skillKeepInfo * getSkillKeep();

struct entityvectorpointercontainer
{
    std::set<Item*> items;
    std::set<Npc*> npcs;
    std::set<Tile*> tiles;
};

bool canSeeNpc(Npc &ori, Npc &target);
bool canSeeNpcv2(Npc &ori, Npc &target);
bool canSeeBetweenTiled(Vec3f ori, Vec3f target);
void assignItemsUser(Npc &npc, std::list<Npc> &container);

void critterEquip(Npc &npc, std::list<Npc> &container);
void critterPush(Npc &npc, std::list<Npc> &container);
void critterVision(Npc &npc, std::list<Npc> &container);
void critterSkillRefresh(Npc &npc, std::list<Npc> &container);


void scrapPickup(Npc &npc, std::list<Npc> &container);
void cashPickup(Npc &npc, std::list<Npc> &container);
void critterPickUp();
std::string useHealItem();
std::string dropItem();
void mouseItemFunctions();
void critterWallCheck(Npc &npc, std::list<Npc> &container);
void critterLevelUp(Npc &npc, std::list<Npc> &container);

void checkAmmo(Npc &npc, std::list<Npc> &container, Item * rangewep, bool forceReload = false);
void unloadAmmo(Item * rangewep, std::list<Item> * npcInv);

void buildTurret(Npc &npc, std::list<Npc> &container);
void craftAmmo(Npc &npc, std::list<Npc> &container);
void activeSkills(Npc &npc, std::list<Npc> &container);

void workDesire(Npc &npc, std::list<Npc> &container, Vec3 &endPos, bool &hasPath, bool &inComplete, Desire * highestDesire );
void workSwitch(Npc &npc, std::list<Npc> &container);

class Npc;
std::set<int> npcTrace(int xa, int ya, int xb, int yb, int id,
                       std::set<int> exceptions);
extern std::list<Npc> npclist;

struct npcPtrVector
{
    std::vector<Npc*> ptrs;
};

struct partGrasp
{
    std::string part;
    Item * graspItem;
    Npc * graspNpc;
    partGrasp()
    {
        graspItem = nullptr;
        graspNpc = nullptr;
    }
};

class directionAnimation
{
public:
    Animation aniLeftWalk;
    Animation aniDownWalk;
    Animation aniRightWalk;
    Animation aniUpWalk;
    AnimatedSprite spriteLeftWalk;
    AnimatedSprite spriteDownWalk;
    AnimatedSprite spriteRightWalk;
    AnimatedSprite spriteUpWalk;
};



class Modifiers
{
public:
    bool onDeath;
    bool onItemUse;
    int onItemUseType;
    bool onHit;
    int onHitType;


    float attackSpeedMod;
    float castSpeedMod;
    float reloadSpeedMod;
    float switchWorkSpeedMod;
    float moveSpeedMod;
    float freezeMod;
    float sleepMod;
    float stunMod;
    std::vector<StringFloat> affectDamageMod;
    std::vector<StringFloat> armorMod;
    float manaRegenMod;

    float applyMomentumMod;
    float momentumSensitivityMod;
    std::vector<StringFloat> immunityMod;

    std::vector<StringFloat> thornsMod;

    std::vector<StringFloat> causeExplosionOnItemUseMod; // should be turned into a conditional somehow.
    std::vector<StringFloat> causeDamageOnItemUseMod; // should be turned into a conditional somehow.
    float itemFindMod;
    float itemDropRateMod;
    float xpModifierMod;
    float deathXpModifierMod;

    int strMod;
    int perMod;
    int intMod;
    int chaMod;
    int endMod;
    int dexMod;

    std::vector<StringFloat> spawnCreatureOnDeathMod;
    std::vector<StringFloat> spawnItemOnDeathMod;

    int revivesOnDeathMod;
    int disableDeathMod;
    std::vector<StringFloat> autoDodgeMod;

    Modifiers();

    void clearAllMods();
    void clearAllPostConditions();
};

class Npc
{
public:
    // TODO: Add racial defaults for attributes and skill growth,
    // Will allow interesting mechanics and fix spawning variety.
    sf::Sprite img;
    directionAnimation bodyAnimation;
    directionAnimation hatAnimation;
    directionAnimation gloveAnimation;
    directionAnimation bootAnimation;

    void setupAnimations();


    std::list<Item> inventory;
    int getInventoryMax();
    int additionalSlots;
    std::list<Item> equipped;

    float getMaxHealth();



    std::string originalRacialAbility;
    std::string racialAbility;
    std::string getRacialAbility();


    int level;
    int xp;
    int skillpoints;


    std::string chasePriority; // Assault, Defend, Hold Position
    Vec3f chaseDefendPos;
    int chaseRange;


    bool recruitable;
    int value;
    int rarity;

    unsigned int deadFrames;
    unsigned int lastHitFrames;
    unsigned int framesSinceLastDamage;

    std::list<StatusEffect> statusEffects;
    void handleStatusEffects();

    Item * getLeftHandItem();
    Item * getRightHandItem();
    Item * getItemTypeInHands(int type);

    Item * invSlots[22];

    std::vector<Tile *> storedPath;

    std::string bloodcontent;
    std::string tags;
    Faction *factionPtr;
    std::string faction;

    bool needsPath;
    bool hasPath;
    Vec3 pathGrid;
    Vec3 endPos;
    Vec3 startPos;

    Vec3f getPos();
    sf::Vector2f getPos2d();

    float getMomentumMagnitude();

    Skills skills;

    float angle;
    float turnSpeed;


    sf::Vector2f desiredViewAngle;
    bool toDelete;

    bool hasJob;
    Job *jobPtr;

    sf::Vector2f momentum;




    class BodyDefinition
    {
    public:
        // Supposed to house the creature when it's fully healthy.
        std::string originalBodyParts;

        std::string bodyParts;

        void bodyPartFind(std::string part, int amount);
    };
    BodyDefinition body;

    std::list<partGrasp> graspers;


    bool isPlayer;
    bool isNPC;
    bool isSquaddie;
    bool isBoss;
    bool hasSpawned;

    bool needsFood;
    bool allowedFood;
    bool needsWater;
    bool allowedDrink;
    bool allowedMove;
    bool grappling;

    int cbaseid;
    int id;
    std::string name;

    bool alive;
    bool disabled;
    bool functional();

    unsigned int ticksalive;

    bool canmove;

    short regenrate;
    short regentimer;
    short regentimerint;

    float moverateint;
    float moverate;
    float movetimer;
    float movetimerint;
    float movetimerrate;
    float moveSpeed;
    float getMoveSpeed();

    float attacktimer;
    float attacktimerint;

    short hungerrate;
    short hungertimer;
    short hungertimerint;
    short thirstrate;
    short thirsttimer;
    short thirsttimerint;


    std::string race;
    std::string originalRace;
    std::string getRace();
    int age;
    std::string gender;

    int gypos;
    int gxpos;
    int planet;
    float ypos;
    float xpos;
    float zpos;
    int rypos;
    int rxpos;

    sf::Vector2f lastPos;
    int framesSinceLastMove;

    float maxhealth;
    float health;

    short mana;
    long maxhunger;
    float hunger;
    long maxthirst;
    float thirst;
    int credits;

    float blockTime;

    sf::Vector2f tentArm1;
    sf::Vector2f tentArm2;
    sf::Vector2f tentEnd1;
    sf::Vector2f tentEnd2;

    bool consumeFlesh;
    bool consumeVeggy;
    bool consumeWater;

    // ==== This section is for Status Effect Carrying

    Modifiers mods;




    // ====

    class Attribute
    {
    public:
        int strength;
        int strengthMin;
        int strengthMax;
        int dexterity;
        int dexterityMin;
        int dexterityMax;
        int agility;
        int agilityMin;
        int agilityMax;
        int charisma;
        int charismaMin;
        int charismaMax;
        int endurance;
        int enduranceMin;
        int enduranceMax;
        int intelligence;
        int intelligenceMin;
        int intelligenceMax;
        int wisdom;
        int wisdomMin;
        int wisdomMax;
        int perception;
        int perceptionMin;
        int perceptionMax;
        int strengthxp;
        int dexterityxp;
        int charismaxp;
        int endurancexp;
        int intelligencexp;
        int wisdomxp;
        int perceptionxp;
        int agilityxp;
        void randomizeAttributes();


        void Train(std::string skill, int amount = 1, int skillgain = 1);
    };
    Attribute attributes;

    int getStr();
    int getPer();
    int getInt();
    int getCha();
    int getEnd();
    int getDex();
    int getAgi();
    int getWis();



    class TargetInfo
    {
    public:
        Item *item;
        Npc *npc;
        Tile *tile;
    };
    TargetInfo targetInfo;

    int killcount;
    short reach;
    short size;
    short stench;
    float viewangle;
    float viewrange;

    Npc();
    void reCreateSkills();
    void blankSkills();
    npcPtrVector getEnemies();
    std::list<Npc> * container;
    bool hasWeapon(std::string weapon = "");
    Item *hasItem(std::string name);
    Item *getItemType(int type);
    bool hasItemType(int type);
    void drawImg();

    void dirMove(sf::Vector2f tar);
    void momMove();
    void angMove(float ang);

    bool isHungry();
    bool isThirsty();
    bool isHurt();
    void breath();
    void sethealth(float amount);
    bool modhealth(float amount);
    void fillhunger(float amount);
    void fillthirst(int amount);
    std::string getname();
    void spawn(int gposx, int gposy, int rposx, int rposy, int posx, int posy, int posz);
    bool boolblood(std::string ailment);
    bool hasTag(std::string tag);
    void tag(std::string tag, int amount);
    float bloodwork(std::string aliment, float amount);
    float bloodworkXX(std::string aliment, int amount);
    bool closeToTarget(int distance, sf::Vector2f Tar);
    void addItem(std::string const &itemname, int amount);
    std::string addItem(Item &item);
    std::string takeDamage(Npc *attacker, Item *weapon = nullptr, float amount = 0, int damageType = 0, critScore *crit = nullptr);
    std::string dealDamage(Npc *victim, Item *weapon = nullptr, float amount = 0, int damageType = 0);
    std::string onDeath(Npc *attacker, Item *weapon = nullptr, float amount = 0, int damageType = 0, critScore *crit = nullptr);

    void printConsoleInfo();
    void printBloodContent();
    bool operator<(Npc const &other) const;
};


extern std::vector<Npc*> selectedNPCs;

void setTestage();

void timeTest();

class NpcManager
{
public:
    std::vector<Npc> globalCritter;
    std::vector<Npc> addedCritters;

    void addCritters();
    void initializeCritters();
};
extern NpcManager npcmanager;

class MakeSquad
{
public:
    float makeSquadPoints;
    int squadMates;
    std::vector<Npc> squad;
    int aim;

    MakeSquad();
};

extern MakeSquad squady;
extern std::vector<Npc> worldCritters;

int getNpcVectorId(int id);
std::set<int> npcTrace(int xa, int ya, int xb, int yb, int id,
                       std::set<int> exceptions = std::set<int>());
void saveNPC(int planet, sf::Vector2i region, Npc &critter);

class Territory
{
public:
    sf::Vector2i worldTile;
    sf::Vector2i galaxyTile;
    bool toDelete;

    Territory();
};

void cleanTerritories(std::vector<Territory> &territories);

struct FactionRelation
{
    std::string faction;
    int appeal;
    int debt;
    //Later to add for Galaxy...
    //Trade Agreements
    //Interrelation members
    //Rivals
    //Tech envies
};

class Faction
{
public:
    std::string name;
    bool playerControlled;
    bool initialized;

    std::vector<FactionRelation> factRelations;
    int getFactionRelations(std::string factionName);
    int members;

    sf::Color primaryColor;
    sf::Color secondaryColor;

    int creativity;
    int aggressiveness;

    float techAgriculture;
    float techArchitecture;
    float techEnergyProduction;
    float techWeaponryMass;
    float techWeaponryEnergy;
    float techWeaponryExplosive;
    float techWeaponrySharp;
    float techWeaponryBlunt;
    float techDiplomacy;
    float techMedical;
    float techRobotics;

    float credits;

    std::vector<Territory> territories;

    std::vector<std::string> knownMagic;
    std::vector<Item> items;

    std::list<Job> jobList;
    Faction();
};

extern std::list<Faction> uniFact;



int factionMembers(std::string factionName);
float factionAggression(std::string factionName);
int factionTerritories(std::string factionName);
float factionPower(std::string factionName);
int factionPopulation();
void initializeFactions(int generateMax = 10);
Npc *getGlobalCritter(std::string strtype);
void spawnCritter(std::string object, int xpos, int ypos, int zpos);


void buildStartingCritters(int zedAmount = 0);
std::string loadCritters(sf::Vector2i worldPos, std::string direction,
                         int planet = 500);
void boom(int xpos, int ypos, int damage, int size);
void squadHud();
void removeNPCs(std::list<Npc> &NPCLIST, sf::Mutex &npcmutex);
void addInitialFaction();
void offloadNpcs();
void attractNPCs(sf::Vector2f position);
Npc *getCritter(int id);
void selectedNPCprocess();
void drawSelectedCritterHUD();
void runCritterBody(Npc &npc);
Faction * addFaction(std::string name);
void addMembers(int amount, std::string faction);
void drawNPCs(std::list<Npc> &container);


#endif // FACTION_H_INCLUDED
