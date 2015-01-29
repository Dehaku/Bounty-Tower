#ifndef __FACTION_H_INCLUDED__
#define __FACTION_H_INCLUDED__

#include "Game.h"
#include "Items.h"
#include "Effects.h"
#include "aStarLibrary.h"
#include "keys.h"
#include "JobManager.h"
#include "Text.h"

#include <time.h>
#include <ctime>
#include <list>

//TODO: Make it so faction units cannot harm eachother, Do the same with Allied Factions.
class Faction;

extern Faction *ConFact;

class NPC;
std::set<int> NPCTrace(int xa, int ya, int xb, int yb, int id,
                       std::set<int> exceptions);
extern std::vector<NPC> npclist;

class NPC
{
public:
    // TODO: Add racial defaults for attributes and skill growth, Will allow interesting mechanics and fix spawning variety.
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
    public
        : //Probably toss all these into a "Body Parts" Class Vector, Name, Bools, Ints, Ect. It'd allow for custom body parts and what not.
          /*
            class Part
            {
                public:
                std::string Name;
                float BloodPumpRate;
                std::string PoisonFilter; // [Zombification:1]   [Disease:FilterRate]

                float DigestionRate;
                float ObjectCapacity; // /But where is it stored?/ Everything will have a volume, If it's volume is less than this, it can be stored in here, so long as this or
                                      // another part is big enough to let it through. Should probably also check for Dependency to allow it to go there, Dunno for now.
                item ItemsStored; // /Here/
                //NPC CrittersStored;// ^
                float NutritionExtraction;
                bool MeatDigestion; // Not happy with this.
                bool VeggyDigestion; // ^


                float AirCapacity;
                float AirAbsorbtion;

                Part()
                {
                    Heart = -1;
                    Liver = -1;
                    Stomach = -1;
                    Lung = -1;
                }
            };



            std::vector<Part> BodyParts;

            Part UT;
            UT.Name = "UpperTorso";
            UT.Heart = 100;
            UT.Lung = 200;

            BodyParts.push_back(UT);

            Part LT;
            LT.Name = "LowerTorso";
            LT.Stomach = 125;
            LT.Liver = 50;

            BodyParts.push_back(LT);
            */

        //Vs

        /*
            { } // Part Container, Allows the code to recognize when a part starts and ends, All following tags are ment to be inside of a set of this.
            [Name:Text] // Designates the name of the part to be displayed/referenced by Dependencies/ect, Examples: Head, Arm, Hand, Tentacle, Tail, Shoulder Mounted Cannon
            [Description:Text] // Meta.

            [BloodPumpRate:Amount] // How quickly blood will be pushed around the body, Mostly used for bleeding out...  /Addition/ Might want to have all blood content applied at this rate.

            [MassFlesh:Amount:Condition] // How much flesh this part has in millivolume, and in what health/state it's in.
            [MassVeggy:Amount:Condition] // How much VeggyMatter this part has in millivolume, and in what health/state it's in.
            [MassMetal:Amount:Condition] // How much Metal this part has in millivolume, and in what health/state it's in.
            [MassGlass:Amount:Condition] // How much Glass this part has in millivolume, and in what health/state it's in.
            // If any of a part's mass hits 0, that limb is useless, If all of a parts mass hits 0, the part is destroyed.


            [Regenerate:MassType:Amount:Text:Cost] // Allows a part to replenish it's 'MassType' by 'Amount' if 'Text' is present in the bloodstream.

            [AirCapacity:Amount] // Numerical value for how much air can be held in, Used for holding breath/ect, TODO: Make Air Container for what they have in their 'lungs'
            [AirAbsorbtion:Amount] // How much air they are holding is absorbed into the bloodstream.

            [ObjectCapacity:Amount] // Amount is in Volume/Mass, This dictates how much item/critter this part can hold, If a part has Digestion, It will attempt to eat what's inside.
            [Grasp:Amount] // Amount is in Volume/Mass, This dictates how much this part can hold.

            [DigestionRate:Amount] // Value for a formula(Amount/100?) to breakdown and consume whatever is held in ObjectCapacity and put into the bloodstream.
            [NutritionExtraction:Amount] // Percentage Amount of DigestionRate that is applied to the bloodstream.
            [DigestsFlesh:Amount] // Amount is how much nutrition is gathered from meat.
            [DigestsVeggy:Amount] // Amount is how much nutrition is gathered from vegetable matter. (Including fruits)
            [DigestsMinerals:Amount] // Amount is how much nutrition is gathered from Metals, Rust Monsters and ship eaters galore! (Difficult to smuggle metalic goods in this part due to this)
            [DigestsPlastics:Amount] // Amount is how much nutrition is gathered from Plastics, Probably won't be used.
            [DigestsGlass:Amount] // Amount is how much nutrition is gathered from Glass, Probably won't be used.
            [DigestsWater:Amount] // Amount is how much nutrition is gathered from Water.
            [DigestsOil:Amount] // Amount is how much nutrition is gathered from Oil, Beep.
            [DigestsBlood:Amount] // Amount is how much nutrition is gathered from Blood, Bleigh.
            [DigestsMagic:Amount] // Converts local background magic into nutrients into the blood directly.
            [ElectricalConduit:Amount] // This part can be shoved into various electrical objects to gain nutrients/power, If this part is a grasping part, It can also drain things it's holding.
            [ElectricStorage:Amount] // This part can hold an electric charge, Used for various purposes.

            [Orafice:Amount] // This determines both if this part has a mouth/ect, and how large of an object can go inside, Also dictates how much Air it breaths. (Amount*100 for air)

            [Reproduction:bool] // If this part is an acceptable critter conception location.

            [Weapon:bool] // Determines if this part is a natural weapon. // Redundent? WeaponType could become Weapon, and the existence of the tag could dictate if it's a weapon or not.
            [WeaponType:Text:Text2] // Should be filled with Melee/Range then damage type, Slash/Pierce/Blunt, Ect. TODO: Needs more information, attack rate(dex?) damage(str?)
            [WeaponPoison:Text:Text2:Amount] // Presence of this tag means this part applies poison.
                                             // Text is the delivery method Contact/Injected, Text2 is the Poison that will be applied on each attack, Amount is self explanatory.
            //TODO: Might want to convert the delivery method into the poison itself, when later making a poison manager/ect.

            [Exposed:Amount] // Amount is equal to the damage and/or time needed to fix it, Exposed parts have been cut open in some fashion that the contained objects can spill or be yanked out.
            [Dependant:PartName] // A part with this tag is directly connected to 'PartName' If 'PartName' is severed/non-existant, this part will come off with it or simply fall off respectively.
            [External:bool] // If a part has this tag, It can be targeted even if it has parts dependent on it.

            [Walk:Speed] // This tag gives this part the ability to move the creature, Each leg gives a 1.5x global speed multiplier.
            [Flight:Speed:Manuverability] // Might simply give everything perfect manuverability.

            [Mind:bool] // Perhaps use an amount later for base intelligence?, This tag makes this part the epicenter of thought, Without this tag, The creature is a lump of parts, no automation.
                        // Oh, Maybe make it a Type instead? Or perhaps that's where the [CanLearn] tag comes into play? Robots Vs Brains, ect.


            [PoisonFilter:Text:Amount] // This tag will search through the blood for anything with the Text as it's name, and get rid of it by the Amount.


            //  Possible Tags  //
            [Reach:Amount]  // This tag is to be used for grasping purposes, A part with this can grasp items equal to it's amount, Due to programming limitations and performance
                            // it may be required to treat it as simply a global tag, the longest reach allows grasping, Possible conflict with the Range in the weapon tag.




            // See 2014-11-30 Lithi chatlog for alot of great material on this.
            */

        //std::string BodyParts;
        //BodyParts.Append("{[Name:UpperTorso][BloodPumpRate:100][AirCapacity:200][AirAbsorbtion:100][ObjectCapacity:1]}");

        //BodyParts.Append("{[Name:LowerTorso][ObjectCapacity:10][DigestionRate:125][PoisonFilter:Zombification:100]}");

        std::string
            OriginalBodyParts; // Supposed to house the creature when it's fully healthy.

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
        int MypathLength; //stores length of the found path for critter
        int MypathLocation; //stores current position along the chosen path for critter
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
    int AgeType; // 1 = Babeh, 2 = Child, 3 = Teenager, 4 = Young Adult, 5 = Aged Adult, 6 = Elder
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
    public: // Read class Body's Public:
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
