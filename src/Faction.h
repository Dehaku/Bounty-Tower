#ifndef __FACTION_H_INCLUDED__
#define __FACTION_H_INCLUDED__

#include "Items.h"
#include "Effects.h"
#include "keys.h"
#include "JobManager.h"
#include "Text.h"
#include "Globals.h"

#include <list>

// TODO: Make it so faction units cannot harm eachother,
// Do the same with Allied Factions.
class Faction;

extern Faction *ConFact;

class NPC;
std::set<int> NPCTrace(int xa, int ya, int xb, int yb, int id,
                       std::set<int> exceptions);
extern std::vector<NPC> npclist;

class NPC
{
public:
    // TODO: Add racial defaults for attributes and skill growth,
    // Will allow interesting mechanics and fix spawning variety.
    sf::Sprite img;
    std::list<item> inventory;
    std::list<item> Equipped;
    std::string bloodcontent;
    std::string tags;
    std::string Faction;
    bool Attacking;
    bool FirstStrike;
    bool ImgRotates;
    float angle;
    sf::Vector2f speed;
    sf::Vector2f ShootPos;
    bool Prone;
    bool NeedsPath;
    bool ToDelete;

    bool HasJob;

    sf::Vector2f Momentum;

    class BodyDefinition
    {
    public:
        // Supposed to house the creature when it's fully healthy.
        std::string OriginalBodyParts;

        std::string BodyParts;

        void BodyPartFind(std::string Part, int amount);

        int head;
        int eyes;  // Mask = Eyes+Mouth
        int mouth; // Mask = Eyes+Mouth
        int neck;
        int chest;
        int back;
        int waist;
        int legs;
        int rightupperarm; // int rua
        int rightlowerarm; // int rla
        int righthand;     // rh
        int rightgrasp;    // rg
        int leftupperarm;  // int lua
        int leftlowerarm;  // int lla
        int lefthand;      // lh
        int leftgrasp;     // lg
        int rightupperleg; // rul
        int rightlowerleg; // rll
        int rightfoot;     // rf
        int leftupperleg;  // lul
        int leftlowerleg;  // lll
        int leftfoot;      // lf
    };
    BodyDefinition Body;

    float rot;
    float Xxx;
    float Yyy;
    float Degrees;
    std::vector<int> exceptions;

    std::set<int> Melee(int min, int max, int range,
                        std::set<int> exception = std::set<int>());

    class PathFinding
    {
    public:
        int *MypathBank[1];
        // stores length of the found path for critter
        int MypathLength;
        // stores current position along the chosen path for critter
        int MypathLocation;
        int MypathStatus;
        int MyxPath;
        int MyyPath;
        std::vector<int> xPa;
        std::vector<int> yPa;
        void EndMem();
        void MyFindPath(int Sx, int Sy, int Ex, int Ey);
        int MyReadPathX(int pathfinderID, int xpathLocation);
        int MyReadPathY(int pathfinderID, int ypathLocation);
        void MyReadPath(int pathfinderID, int currentX, int currentY,
                        int pixelsPerFrame);
    };
    PathFinding PathFinding;

    short imgstrx;
    short imgstry;
    short imgendx;
    short imgendy;
    bool IsPlayer;
    bool IsNPC;
    bool HasSpawned;
    bool NeedsFood;
    bool AllowedFood;
    bool NeedsWater;
    bool AllowedDrink;
    bool AllowedMove;
    bool Grappling;
    int cbaseid;
    int id;
    int id2;
    std::string name;
    char surname;
    bool alive;
    bool stillalive;
    unsigned int ticksalive;
    bool useditem;
    bool canmove;
    std::string target;
    sf::Vector2f TargetPos;
    int TargetId;
    int TargetVectorId;
    bool AtTarget;
    bool HasTarget;
    std::string action;
    short regenrate;
    short regentimer;
    short regentimerint;
    float moverateint;
    float moverate;
    float movetimer;
    float movetimerint;
    float movetimerrate;
    float attacktimer;
    float attacktimerint;
    short hungerrate;
    short hungertimer;
    short hungertimerint;
    short thirstrate;
    short thirsttimer;
    short thirsttimerint;
    short breathrate; // not needed?
    short breathtimer;
    short breathtimerint;
    std::string race;
    int Age;
    // 1 = Babeh, 2 = Child, 3 = Teenager, 4 = Young Adult,
    // 5 = Aged Adult, 6 = Elder
    int AgeType;
    std::string gender;
    char direction;
    int worshippers;
    int gypos;
    int gxpos;
    int planet;
    float ypos;
    float xpos;
    int rypos;
    int rxpos;
    float maxhealth;
    float health;
    short mana;
    short reginterntemp;
    short interntemp;
    short regtemp;
    short temp;
    short breathmax;
    long maxhunger;
    float hunger;
    short nutrition;
    long maxthirst;
    float thirst;
    short maxstamina;
    short stamina;
    int credits;

    sf::Vector2f TentArm1;
    sf::Vector2f TentArm2;
    sf::Vector2f TentEnd1;
    sf::Vector2f TentEnd2;

    void EffectStats();

    class Skill
    {
    public:
        int strength;
        int dexterity;
        int agility;
        int charisma;
        int endurance;
        int intelligence;
        int wisdom;
        int perception;
        int strengthxp;
        int dexterityxp;
        int charismaxp;
        int endurancexp;
        int intelligencexp;
        int wisdomxp;
        int perceptionxp;
        int agilityxp;

        void Train(std::string skill, int amount = 1, int skillgain = 1);
    };
    Skill Skills;

    int killcount;
    short reach;
    short size;
    short stench;
    float viewangle;
    float viewrange;

    NPC();
    void ReCreateSkills();
    void BlankSkills();
    bool HasWeapon(std::string weapon = "");
    item *GetItemType(int type);
    bool HasItemType(int type);
    void DrawImg();
    void Move(sf::Vector2f Tar);
    void DirMove(sf::Vector2f Tar);
    void MomMove();
    void MoveNorth();
    void MoveNorthEast();
    void MoveEast();
    void MoveSouthEast();
    void MoveSouth();
    void MoveSouthWest();
    void MoveWest();
    void MoveNorthWest();
    bool IsHungry();
    bool IsThirsty();
    bool IsHurt();
    void breath();
    void sethealth(float amount);
    bool modhealth(float amount);
    void fillhunger(float amount);
    void fillthirst(int amount);
    std::string getname();
    void spawn(int gposx, int gposy, int rposx, int rposy, int posx, int posy);
    bool boolblood(std::string ailment);
    bool HasTag(std::string tag);
    void Tag(std::string tag, int amount);
    float bloodwork(std::string aliment, float amount);
    float bloodworkXX(std::string aliment, int amount);
    bool CloseToTarget(int distance, sf::Vector2f Tar);
    void AddItem(std::string const &itemname, int amount);

    short minmeleedamage;
    short maxmeleedamage;
    short minrangeddamage;
    short maxrangeddamage;

    unsigned char dirgrid[gridy][gridx];
    unsigned char valuegrid[gridy][gridx];
    unsigned char followgrid[gridy][gridx];

    void PrintConsoleInfo();
    void PrintBloodContent();
    bool operator<(NPC const &other) const;
};

extern NPC NPCz;

void SetTestage();

void TimeTest();

class cNpcManager
{
public:
    std::vector<NPC> GlobalCritter;
    std::vector<NPC> AddedCritters;

    void AddCritters();
    void InitializeCritters();
};
extern cNpcManager npcmanager;

class MakeSquad
{
public:
    float MakeSquadPoints;
    int SquadMates;
    std::vector<NPC> Squad;
    int Aim;

    MakeSquad();
};

extern MakeSquad Squady;
extern std::vector<NPC> WorldCritters;

int GetNpcVectorId(int id);
std::set<int> NPCTrace(int xa, int ya, int xb, int yb, int id,
                       std::set<int> exceptions = std::set<int>());
void SaveNPC(int planet, sf::Vector2i Region, NPC &Critter);

class Territory
{
public:
    sf::Vector2i WorldTile;
    sf::Vector2i GalaxyTile;
    bool ToDelete;

    Territory();
};

void CleanTerritories(std::vector<Territory> &Territories);

class Faction
{
public:
    std::string Name;
    bool PlayerControlled;
    bool Initialized;

    int Members;

    sf::Color PrimaryColor;
    sf::Color SecondaryColor;

    int Creativity;
    int Aggressiveness;

    float TechAgriculture;
    float TechArchitecture;
    float TechEnergyProduction;
    float TechWeaponryMass;
    float TechWeaponryEnergy;
    float TechWeaponryExplosive;
    float TechWeaponrySharp;
    float TechWeaponryBlunt;
    float TechDiplomacy;
    float TechMedical;
    float TechRobotics;

    std::vector<Territory> Territories;

    std::vector<std::string> KnownMagic;
    std::vector<cItem> Items;

    std::vector<Job> JobList;
    Faction();
};

extern std::vector<Faction> UniFact;

int FactionMembers(std::string FactionName);
float FactionAggression(std::string FactionName);
int FactionTerritories(std::string FactionName);
float FactionPower(std::string FactionName);
int FactionPopulation();
void InitializeFactions(int GenerateMax = 10);
NPC *GetGlobalCritter(std::string strtype);
void SpawnCritter(std::string Object, int xpos, int ypos);

extern Faction PF;

void BuildStartingCritters(int ZedAmount = 0);
std::string LoadCritters(sf::Vector2i WorldPos, std::string Direction,
                         int planet = 500);
void Boom(int xpos, int ypos, int damage, int size);
void SquadHud();

#endif // FACTION_H_INCLUDED
