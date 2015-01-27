#ifndef __FACTION_H_INCLUDED__
#define __FACTION_H_INCLUDED__

// *#include "Globals.h"
// *#include "Items.h"
// *#include "JobManager.h"

#include "Game.h"

#include <time.h>
#include <ctime>
#include <list>


//TODO: Make it so faction units cannot harm eachother, Do the same with Allied Factions.
class Faction;

Faction * ConFact;

class Group{
    public:

};

class NPC;
std::set<int> NPCTrace(int xa, int ya, int xb, int yb,int id, std::set<int> exceptions);
std::vector<NPC> npclist;


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
        public:  //Probably toss all these into a "Body Parts" Class Vector, Name, Bools, Ints, Ect. It'd allow for custom body parts and what not.




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








        std::string OriginalBodyParts; // Supposed to house the creature when it's fully healthy.

        std::string BodyParts;

        bool BodyPartFind(std::string Part, int amount){

            size_t LinePosOne;
            LinePosOne = BodyParts.find(Part);
            if(LinePosOne != std::string::npos)
            {

                size_t LinePosTwo;
                std::string LineValue;
                LinePosTwo = BodyParts.find("]",LinePosOne+1);
                if(LinePosTwo != std::string::npos)
                {
                    int len = Part.length();
                    LineValue.assign(BodyParts,LinePosOne+len, LinePosTwo-(LinePosOne+len));
                    amount += atoi(LineValue.c_str());

                    std::string amo;
                    std::stringstream out;
                    out << amount;
                    amo = out.str();
                    BodyParts.replace(LinePosOne+len, LinePosTwo-(LinePosOne+len),out.str());

                    if(out.str() == "0")
                    {
                        BodyParts.replace(LinePosOne,LinePosTwo,"");
                    }
                }
            }
            else if(amount == 0)
            {
                //cout<<"No "<<Part<<" Here\n";
            }
            else if(amount > 0)
            {
                BodyParts.assign(Part);
                std::string amo;
                std::stringstream out;
                out << amount;
                amo = out.str();
                BodyParts.append(out.str());
                BodyParts.append("]");
            }
        }









        int head;
        int eyes;// Mask = Eyes+Mouth
        int mouth;// Mask = Eyes+Mouth
        int neck;
        int chest;
        int back;
        int waist;
        int legs;
        int rightupperarm; // int rua
        int rightlowerarm; // int rla
        int righthand; // rh
        int rightgrasp; // rg
        int leftupperarm; // int lua
        int leftlowerarm; // int lla
        int lefthand; // lh
        int leftgrasp; // lg
        int rightupperleg; // rul
        int rightlowerleg; // rll
        int rightfoot; // rf
        int leftupperleg; // lul
        int leftlowerleg; // lll
        int leftfoot; // lf
    };
    BodyDefinition Body;






    float rot;
    float Xxx;
    float Yyy;
    float Degrees;
    std::vector<int> exceptions;

    std::set<int> Melee(int min, int max, int range, std::set<int> exception = std::set<int>())
    {
        std::set<int> Tar;
        if(FirstStrike == false)
        {
            Degrees = Math.AngleBetweenVectors(sf::Vector2f(xpos,ypos),TargetPos)-90;
            FirstStrike = true;
        }
        for(int i = 0; i != 30; i++)
        {
            Xxx = xpos + cosf(Degrees * PI/180) * range;
            Yyy = ypos + sinf(Degrees * PI/180) * range;
            Degrees += 1;
            Effectz.CreateLine(xpos,ypos,Xxx,Yyy,2,Blue);
            Tar = NPCTrace(xpos,ypos,Xxx,Yyy,id,std::set<int>());
        }
        std::list<item>::iterator Inv;
        for( Inv = inventory.begin(); Inv != inventory.end(); ++Inv )
        {
            if(Inv->name == "Sword"){  }
        }
        if(Degrees >= Math.AngleBetweenVectors(sf::Vector2f(xpos,ypos),TargetPos)+90)
        {
            Attacking = false;
            FirstStrike = false;
        }
        return Tar;
    }

    class PathFinding{
        public:
        int* MypathBank[1];
        int MypathLength;     //stores length of the found path for critter
	    int MypathLocation;   //stores current position along the chosen path for critter
        int MypathStatus;
        int MyxPath;
	    int MyyPath;
	    std::vector<int> xPa;
	    std::vector<int> yPa;
        void EndMem(){ free (MypathBank[1]); }
        bool MyFindPath(int Sx, int Sy, int Ex, int Ey){
            xPa.clear();
            yPa.clear();
            debug("Pathfind status");

            try{
            MypathStatus = FindPath(1,Sx,Sy,Ex,Ey);
            }catch (std::exception& e) { std::cout << " Find Path done messed up\n"; fSleep(60); }

            Con("Pathfind location");
            MypathLocation = pathLocation[1];
            Con("Pathfind length");
            MypathLength = pathLength[1];
            //MypathBank[1] = (int*) realloc (pathBank[1],pathLength[1]*8);
            Con("Pathfind stuff");
            for( int Stuff = 0; Stuff != MypathLength; ++Stuff )
            {
                //MypathBank[1] [Stuff*2-2] = pathBank[1] [Stuff*2-2];
                yPa.push_back(pathBank[1] [Stuff*2-1]);
                //std::cout << "*Added*:" << xPa.at(Stuff-1)<<std::endl;
                //MypathBank[1] [Stuff*2-1] = pathBank[1] [Stuff*2-1];
                xPa.push_back(pathBank[1] [Stuff*2-2]);
            }
            Con("Pathfind stuff - Complete");

        }

        int MyReadPathX(int pathfinderID,int xpathLocation){
            pathfinderID = 1;
	        int x;
	        if (xpathLocation <= MypathLength)
	        {
	            //Read coordinate from bank
	            //x = MypathBank[pathfinderID] [xpathLocation*2-2];
	            //Like... Yo bo... Ty this instead.
                try{x = xPa.at(xpathLocation);}catch (std::exception& e) {}
	            //Adjust the coordinates so they align with the center
	            //of the path square (optional). This assumes that you are using
	            //sprites that are centered -- i.e., with the midHandle command.
	            //Otherwise you will want to adjust this.
	            x = tileSize*x + .5*tileSize;
	        }
	        return x;
        }

//-----------------------------------------------------------------------------
// Name: ReadPathY
// Desc: Reads the y coordinate of the next path step
//-----------------------------------------------------------------------------
        int MyReadPathY(int pathfinderID,int ypathLocation){
            pathfinderID = 1;
	        int y;
	        if (ypathLocation <= MypathLength)
	        {
	            //Read coordinate from bank
	            //y = MypathBank[pathfinderID] [ypathLocation*2-1];
                try
                {
	                y = yPa.at(ypathLocation);
                }
	            catch (std::exception& e) {}
	            //Adjust the coordinates so they align with the center
	            //of the path square (optional). This assumes that you are using
	            //sprites that are centered -- i.e., with the midHandle command.
	            //Otherwise you will want to adjust this.
	            y = tileSize*y + .5*tileSize;
            }
	        return y;
        }

        bool MyReadPath(int pathfinderID,int currentX,int currentY,int pixelsPerFrame){
	        int ID = 1; //redundant, but makes the following easier to read
            //If a path has been found for the pathfinder	...
            if (MypathStatus == AStarFound)
	        {
		        //If path finder is just starting a new path or has reached the
		        //center of the current path square (and the end of the path
		        //hasn't been reached), look up the next path square.
		        if (MypathLocation < MypathLength)
		        {
			        //if just starting or if close enough to center of square
			        if (MypathLocation == 0 || (abs(currentX - MyxPath) < pixelsPerFrame && abs(currentY - MyyPath) < pixelsPerFrame)) MypathLocation = MypathLocation + 1;
		        }
		        //Read the path data.
		        MyxPath = MyReadPathX(ID,MypathLocation);
		        MyyPath = MyReadPathY(ID,MypathLocation);
		        //If the center of the last path square on the path has been
		        //reached then reset.
		        if (MypathLocation == MypathLength)
		        {
			        if (abs(currentX - MyxPath) < pixelsPerFrame && abs(currentY - MyyPath) < pixelsPerFrame) //if close enough to center of square
					MypathStatus = notStarted;
		        }
	        }
	        //If there is no path for this pathfinder, simply stay in the current
 	        //location.
	        else
	        {
		        MyxPath = currentX;
		        MyyPath = currentY;
	        }
        }

        PathFinding(){//MypathBank [1] = (int*) malloc(4);
        }
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
    std::string name; // .c_str()
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
    short breathrate;// not needed?
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
    short maxhunger;
    float hunger;
    short nutrition;
    short maxthirst;
    float thirst;
    short maxstamina;
    short stamina;
    int credits;

    sf::Vector2f TentArm1;
    sf::Vector2f TentArm2;
    sf::Vector2f TentEnd1;
    sf::Vector2f TentEnd2;



    bool EffectStats()
    {
        maxhealth = Skills.endurance*0.8;
        regenrate = Skills.endurance/10;
    }

    class Skill
    {
        public:// Read class Body's Public:
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

        bool Train(std::string skill, int amount = 1, int skillgain = 1){
            if(skill == "agility"){agilityxp+=amount;if(agilityxp >= 100){agility+=skillgain; agilityxp = 0;}}
            if(skill == "charisma"){charismaxp+=amount;if(charismaxp >= 100){charisma+=skillgain; charismaxp = 0;}}
            if(skill == "intelligence"){intelligencexp+=amount;if(intelligencexp >= 100){intelligence+=skillgain; intelligencexp = 0;}}
            if(skill == "dexterity"){dexterityxp+=amount;if(dexterityxp >= 100){dexterity+=skillgain; dexterityxp = 0;}}
            if(skill == "strength"){strengthxp+=amount;if(strengthxp >= 100){strength+=skillgain; strengthxp = 0;}}
            if(skill == "perception"){perceptionxp+=amount;if(perceptionxp >= 100){perception+=skillgain; perceptionxp = 0;}}
            if(skill == "endurance"){endurancexp+=amount;if(endurancexp >= 100){endurance+=skillgain; endurancexp = 0;}}
            if(skill == "wisdom"){wisdomxp+=amount;if(wisdomxp >= 100){wisdom+=skillgain; wisdomxp = 0;}}

        }
    };
    Skill Skills;

    int killcount;
    short reach;
    short size;
    short stench;
    float viewangle;
    float viewrange;

    NPC(){
        HasJob = false;

        ToDelete = false;
        viewangle = 180;
        viewrange = 200;
        stench = 0; killcount = 0; id = Globals.globalid++; NeedsFood = true; NeedsWater = true; TargetVectorId = -1;TargetId = -1; angle = 1;
        AllowedDrink = true; AllowedFood = true; AllowedMove = true;
        AtTarget = false;HasTarget = false;target = "Food";action = "Act";
        moverateint = 1;moverate = moverateint;movetimerint = 1000;movetimer = movetimerint; movetimerrate = 500;
        attacktimer = 0; attacktimerint = 25;
        maxhunger = 600;hungertimerint = 10;hungertimer = hungertimerint;hungerrate = 1;hunger = 600;
        maxthirst = 600;thirsttimerint = 5;thirsttimer = thirsttimerint;thirstrate = 1;thirst = 600;
        reach = 10;
        Skills.endurance = randz(20,80);
        Skills.strength = randz(20,80);
        Skills.dexterity = randz(20,80);
        Skills.intelligence = randz(20,80);
        Skills.wisdom = randz(20,80);
        Skills.charisma = randz(20,80);
        Skills.perception = randz(20,80);
        Skills.agility = randz(20,80);
        Skills.endurancexp = 0;
        Skills.strengthxp = 0;
        Skills.dexterityxp = 0;
        Skills.intelligencexp = 0;
        Skills.wisdomxp = 0;
        Skills.charismaxp = 0;
        Skills.perceptionxp = 0;
        Skills.agilityxp = 0;
        maxhealth = Skills.endurance*0.8;regentimerint = 100;regentimer=regentimerint;regenrate = Skills.endurance/10;health = maxhealth;
        stench = 50; race = "Human";
        int numz = randz(0,1); if(numz==0){gender = "Male";}else{gender = "Female";}
        int numzb = randz(0,1); if(numzb==0){gender = "Male";}else{gender = "Female";}
        if(gender == "Male")
        {
            int numzb = randz(0,6);
            if(numzb == 0){name = "John";}
            else if(numzb == 1){ name = "Jack";}
            else if(numzb == 2){ name = "Jonathan";}
            else if(numzb == 3){ name = "James";}
            else if(numzb == 4){ name = "Joseph";}
            else if(numzb == 5){ name = "Joe";}
            else if(numzb == 5){ name = "Justin";}
        };
        if(gender == "Female")
        {
            int numzb = randz(0,5);
            if(numzb==0){name = "Jennifer";}
            else if(numzb==1){name = "Jill";}
            else if(numzb==2){name = "Jamie";}
            else if(numzb==3){name = "Jackie";}
            else if(numzb==4){name = "Joan";}
            else if(numzb==5){name = "Jessica";}
        };
        //img.SetImage(Images);
        IsNPC = true;
        NeedsFood = true;
        AllowedFood = true;
        NeedsWater = true;
        AllowedDrink = true;
        NeedsPath = false;
    }

    ~NPC(){}

    void ReCreateSkills(){

        Skills.endurance = randz(20,80);
        Skills.strength = randz(20,80);
        Skills.dexterity = randz(20,80);
        Skills.intelligence = randz(20,80);
        Skills.wisdom = randz(20,80);
        Skills.charisma = randz(20,80);
        Skills.perception = randz(20,80);
        Skills.agility = randz(20,80);
        Skills.endurancexp = 0;
        Skills.strengthxp = 0;
        Skills.dexterityxp = 0;
        Skills.intelligencexp = 0;
        Skills.wisdomxp = 0;
        Skills.charismaxp = 0;
        Skills.perceptionxp = 0;
        Skills.agilityxp = 0;
        maxhealth = Skills.endurance*0.8;
        regentimerint = 100;
        regentimer=regentimerint;
        regenrate = Skills.endurance/10;
        health = maxhealth;
    }

    void BlankSkills(){

        Skills.endurance = 20;
        Skills.strength = 20;
        Skills.dexterity = 20;
        Skills.intelligence = 20;
        Skills.wisdom = 20;
        Skills.charisma = 20;
        Skills.perception = 20;
        Skills.agility = 20;
        Skills.endurancexp = 0;
        Skills.strengthxp = 0;
        Skills.dexterityxp = 0;
        Skills.intelligencexp = 0;
        Skills.wisdomxp = 0;
        Skills.charismaxp = 0;
        Skills.perceptionxp = 0;
        Skills.agilityxp = 0;
        maxhealth = Skills.endurance*0.8;
        regentimerint = 100;
        regentimer=regentimerint;
        regenrate = Skills.endurance/10;
        health = maxhealth;
    }


    bool HasWeapon(std::string weapon = ""){
        bool Specific = false;
        bool HasSpecifiedWeapon = false;
        bool FoundWeapon = false;
        if(weapon != "") Specific = true;
        std::list<item>::iterator Items;
        try
        {//You should always Try when dealing with vectors that could be empty.
            for(Items = inventory.begin(); Items != inventory.end(); Items++)
            {
                if(Items->IsWeapon) FoundWeapon = true;
                if(weapon != "")
                {
                    if(Items->name == weapon) HasSpecifiedWeapon = true;
                }
            }
            if(HasSpecifiedWeapon){return true;}
            if(FoundWeapon && Specific == false){return true;}
        } catch (std::exception& e){}/*This keeps the game from crashing if there is nothing in the vector.*/
        return false;
    }

    item * GetItemType(int type){
        //for(int i = 0; i < inventory.size(); i++)
        for(std::list<item>::iterator i = inventory.begin(); i != inventory.begin(); i++)
        {
            if( (*i).type == type )
            {
                if(Debug){std::cout << "Returning inventory class \n";} return &(*i);
            }
        }
        if(Debug){std::cout << "Returning inventory nothing. \n";}
    }

    bool HasItemType(int type){
        //for(int i=0; i<inventory.size(); i++)
        for(std::list<item>::iterator Items = inventory.begin(); Items != inventory.end(); Items++)
        {
            if( (*Items).type == type )
            {
                if(Debug){std::cout << "returning inventory bool true \n";}return true;
            }
        }
        if(Debug){std::cout << "returning inventory bool false \n";}return false;
    }

    //bool SetImg(int istrx,int istry,int iendx,int iendy){if(iendx == 0 || iendy == 0){img.SetSubRect(sf::IntRect(imgstrx, imgstry, imgendx, imgendy));}else{img.SetSubRect(sf::IntRect(istrx, istry, iendx, iendy));}}

    void DrawImg(){
        img.setPosition(xpos,ypos);
        App.draw(img);
    }

    bool Move(sf::Vector2f Tar){
        bool Above = false;
        bool Right = false;
        bool AtTargetx = false;
        bool AtTargety = false;
        if(Tar.x < xpos){Right = false;}
        else if(Tar.x == xpos){AtTargetx = true;}
        else{Right = true;}

        if(Tar.y < ypos){Above = true;}
        else if(Tar.y == ypos){AtTargety = true;}
        else{Above = false;}

        if(AtTargetx == true && AtTargety == true){ AtTarget = true;}
        else
        {
            if(AtTargety == true){}
            else if(Above == true){int numz = CloseishS(Tar.y,ypos);  if( numz < moverate){ypos += numz; }else{ ypos += moverate;}}
            else{int numz = CloseishS(Tar.y,ypos);  if( numz < moverate){ypos -= numz; }else{ ypos -= moverate;}}

            if(AtTargetx == true){}
            else if(Right == true){int numz = CloseishS(Tar.x,xpos);  if( numz < moverate){xpos += numz; }else{ xpos += moverate;}}
            else{int numz = CloseishS(Tar.x,xpos);  if( numz < moverate){xpos -= numz; }else{ xpos -= moverate;}}
        }
        angle = (180/PI)*(atan2f(xpos-Tar.x,ypos-Tar.y));
    }

    bool DirMove(sf::Vector2f Tar){
        bool Above = false;
        bool Right = false;
        bool AtTargetx = false;
        bool AtTargety = false;
        bool CloseX = false;
        bool CloseY = false;
        if(Tar.x < xpos){Right = false;}
        else if(Tar.x == xpos){AtTargetx = true;}
        else{Right = true;}

        if(Tar.y < ypos){Above = false;}
        else if(Tar.y == ypos){AtTargety = true;}
        else{Above = true;}

        if(AtTargetx == true && AtTargety == true){ AtTarget = true;}
        else
        {
            if(AtTargetx == false && AtTargety == false){}
            int numz0 = CloseishS(Tar.y,ypos); int numz1 = CloseishS(Tar.x,xpos);
            if( numz1 < moverate && numz0 < moverate){xpos = Tar.x; ypos = Tar.y;CloseX = true;CloseY = true;}
            else{AtTarget = false;}
        }


        if(CloseY == false && CloseX == false)
        {
            int xx = 0;
            int yy = 0;
            angle = 90-(180/PI)*(atan2f(xpos-Tar.x,ypos-Tar.y));//To be honest, I spent alot of time with trial and error to get this part to work.
            xx = cosf((angle) * PI/180) * moverate;
            yy = sinf((angle) * PI/180) * moverate;
            xpos -= xx;
            ypos -= yy;
        }

        int dx = Tar.x - xpos, dy = Tar.y - ypos, steps, k;
        float xIncrement, yIncrement, x = xpos, y = ypos;
        if(abs(dx) > abs(dy)) steps = abs(dx);
        else steps = abs(dy);
        xIncrement = dx / (float) steps;
        yIncrement = dy / (float) steps;
        for(int k = 0; k < steps; k++)
        {
            x += xIncrement;
            y += yIncrement;
        }
        //xpos += x/steps;
        //ypos += y/steps;

    }

    bool MomMove()
    {
        xpos += (Momentum.x/size);
        ypos += (Momentum.y/size);

        Momentum.x = Math.Clamp((Momentum.x-AirPressure),0,9999999);
        Momentum.y = Math.Clamp((Momentum.y-AirPressure),0,9999999);
    }

    bool MoveNorth(){ypos = ypos -= moverate;};
    bool MoveNorthEast(){ypos = ypos -= moverate;xpos = xpos += moverate;};
    bool MoveEast(){xpos = xpos += moverate;};
    bool MoveSouthEast(){ypos = ypos += moverate;xpos = xpos += moverate;};
    bool MoveSouth(){ypos = ypos += moverate;};
    bool MoveSouthWest(){ypos = ypos += moverate;xpos = xpos -= moverate;};
    bool MoveWest(){xpos = xpos -= moverate;};
    bool MoveNorthWest(){ypos = ypos -= moverate;xpos = xpos -= moverate;};

    bool IsHungry(){ if(AllowedFood == false){return false;}  if(hunger < (maxhunger*0.40)){return true;}else{return false;}  }
    bool IsThirsty(){ if(AllowedDrink == false){return false;}  if(thirst < (maxthirst*0.40)){return true;}else{return false;}  }
    bool IsHurt(){  if(health < (maxhealth*0.60)){return true;}else{return false;}  }

    bool breath(){};

    int sethealth(int amount){health = amount;}
    bool modhealth(int amount){health += amount;if(health < 1){health = 0;return false;} else{return true;}}

    int fillhunger(int amount){hunger += amount; if(hunger > maxhunger){hunger = maxhunger;}};
    int fillthirst(int amount){thirst += amount; if(thirst > maxthirst){thirst = maxthirst;}};

    std::string getname(){return name;}

    void spawn(int gposx,int gposy,int rposx,int rposy,int posx,int posy){gxpos = 100;gypos = 100;rxpos = 100;rypos = 100;xpos = posx;ypos = posy; HasSpawned = true;}

    bool boolblood(std::string ailment){
        using namespace std;
        size_t found;
        found = bloodcontent.find(ailment);
        if(found != string::npos)
        {
            //cout <<aliment<<" found at: " << int(found) << endl;
            return true;
        }
        else return false;
    }

    bool HasTag(std::string tag){
        using namespace std;
        size_t found;
        found = tags.find(tag);
        if(found != string::npos)
        {
            return true;
        }
        else return false;
    }

    bool Tag(std::string tag, int amount){
        using namespace std;
        size_t found;
        found = tags.find(tag);
        if(found != string::npos)
        {
            //cout <<tag<<" found at: " << int(found) << endl;
            size_t foundz;
            std::string funz;
            foundz = tags.find("]",found+1);
            if(foundz != string::npos)
            {
                int len = tag.length();funz.assign(tags,found+len, foundz-(found+len));
                amount += atoi(funz.c_str());
                std::string amo;
                std::stringstream out;out << amount;amo = out.str();
                tags.replace(found+len, foundz-(found+len),out.str());
                //cout<<"Out is:"<<out.str()<<endl;
                if(out.str() == "0")
                {
                    //cout<<"out is 0, It at least it's supposed to be\n";
                    tags.replace(found,foundz,"");
                }
            }
        }
        else if(amount == 0)
        {
            //cout<<"No "<<tag<<" Here\n";
        }
        else if(amount > 0)
        {
            tags.assign(tag);
            std::string amo;
            std::stringstream out;
            out << amount;
            amo = out.str();
            tags.append(out.str());
            tags.append("]");
        }

        /*
        found=tags.find(Zom);if (found!=string::npos){cout << "Zombification found at: " << int(found) << endl;}else{cout<<"No Zombification Here\n";}
        }*/

    }


    float bloodwork(std::string aliment, int amount){ // Returns the final altered value of the altered 'aliment'

        size_t tStart;
        aliment = AddString("[", aliment, ":"); // Doing this so the rest of the code is much, much simplier.
        tStart = bloodcontent.find(aliment);
        float Returns = amount;

        if(tStart != std::string::npos)
        {
            //cout <<aliment<<" tStart at: " << int(tStart) << endl;
            size_t tEnd;
            std::string StrNum;
            tEnd = bloodcontent.find("]",tStart+1);

            if(tEnd != std::string::npos)
            {
                int len = aliment.length();

                StrNum.append(bloodcontent, tStart+len, tEnd-(tStart+len));
                //std::cout << "StrNum: " << StrNum;
                amount += atoi(StrNum.c_str());
                //std::cout << ", became: " << amount << std::endl;
                Returns = amount;

                std::string amo;
                std::stringstream out;
                //Hargendy!
                out << amount;
                amo = out.str();
                bloodcontent.replace(tStart+len, tEnd-(tStart+len),out.str());
                //cout<<"Out is:"<<out.str()<<endl;

                if(amount <= 0) //if(out.str() == "0")
                {
                    //cout<<"out is 0, It at least it's supposed to be\n";
                    bloodcontent.replace(tStart,tEnd+1,"");
                }
            }
        }
        else if(amount == 0)
        {
            //cout<<"No "<<aliment<<" Here\n";
        }
        else// if(amount > 0)
        {
            bloodcontent.append(aliment);
            std::string amo;
            std::stringstream out;
            Returns = amount;
            out << amount;
            amo = out.str();
            bloodcontent.append(out.str());
            bloodcontent.append("]");
        }
        /*

        found=bloodcontent.find(Zom);if (found!=string::npos){cout << "Zombification found at: " << int(found) << endl;}else{cout<<"No Zombification Here\n";}

        }*/
        return Returns;
    }



    float bloodworkXX(std::string aliment, int amount){ // Returns the final altered value of the altered 'aliment'

        size_t tStart;
        tStart = bloodcontent.find(aliment);
        float Returns = amount;

        if(tStart != std::string::npos)
        {
            //cout <<aliment<<" tStart at: " << int(tStart) << endl;
            size_t tEnd;
            std::string StrNum;
            tEnd = bloodcontent.find("]",tStart+1);

            if(tEnd != std::string::npos)
            {
                int len = aliment.length();

                StrNum.append(bloodcontent, tStart+len, tEnd-(tStart+len));
                amount += atoi(StrNum.c_str());
                Returns = amount;

                std::string amo;
                std::stringstream out;
                //Hargendy!
                out << amount;
                amo = out.str();
                bloodcontent.replace(tStart+len, tEnd-(tStart+len),out.str());
                //cout<<"Out is:"<<out.str()<<endl;

                if(out.str() == "0")
                {
                    //cout<<"out is 0, It at least it's supposed to be\n";
                    bloodcontent.replace(tStart,tEnd,"");
                }
            }
        }
        else if(amount == 0)
        {
            //cout<<"No "<<aliment<<" Here\n";
        }
        else if(amount > 0)
        {
            bloodcontent.append(aliment);
            std::string amo;
            std::stringstream out;
            Returns = amount;
            out << amount;
            amo = out.str();
            bloodcontent.append(out.str());
            bloodcontent.append("]");
        }
        /*

        found=bloodcontent.find(Zom);if (found!=string::npos){cout << "Zombification found at: " << int(found) << endl;}else{cout<<"No Zombification Here\n";}

        }*/
        return Returns;
    }

    bool CloseToTarget(int distance, sf::Vector2f Tar){
        int numbz = 0;
        numbz = Math.Closeish(xpos,ypos,Tar.x,Tar.y);
        if(numbz <= distance){return true;}
        else {return false;}
    }

    int AddItem(std::string const & itemname, int amount){// TODO: Set this to optionally receive an item class instead, Will be useful for modded weapons and ect.
         if(Debug){ std::cout << "Pre var in AddItem" << " \n";}
         cItem var = *GetGlobalItem(itemname);
         if(Debug){ std::cout << "Post var in AddItem" << " \n";}
         for(int Times = 0; Times != amount; Times++)
         {
             inventory.push_back(var);
         }
    }

    short minmeleedamage;
    short maxmeleedamage;
    short minrangeddamage;
    short maxrangeddamage;

    unsigned char dirgrid[gridy][gridx];
    unsigned char valuegrid[gridy][gridx];
    unsigned char followgrid[gridy][gridx];

    void PrintConsoleInfo(){
        using namespace std;
        cout << "Name: " << name << endl;
        cout << "cBaseId: " << cbaseid << endl;
        cout << "Id: " << id << endl;
        cout << "Xpos: " << xpos << endl;
        cout << "Ypos: " << ypos << endl;
        if(alive == true){cout << " Alive" << endl;}
        if(alive == false){cout << " Dead" << endl;}
        cout << "Health: " << health << endl;
        cout << "Hunger: " << hunger << endl;
        cout << "Thirst: " << thirst << endl;
        cout << "Kill Count: " << killcount << endl;
        cout << "Target is " << target << " at:"<< TargetPos.x <<","<<TargetPos.y<< endl;
    }

    void PrintBloodContent(){
        using namespace std;
        cout << "Name: " << name << endl;
        cout<<bloodcontent<<endl;
    }

    bool operator<(NPC const & other) const { return other.id > id; }


};


class OldNPC
{
    public:

    // TODO: Add racial defaults for attributes and skill growth, Will allow interesting mechanics and fix spawning variety.
    sf::Sprite img;
    std::vector<item> inventory;
    std::vector<item> Equipped;
    std::string bloodcontent;
    std::string tags;
    bool Attacking;
    bool FirstStrike;
    bool ImgRotates;
    float angle;
    sf::Vector2f speed;
    sf::Vector2f ShootPos;
    bool Prone;
    bool NeedsPath;
    bool ToDelete;

    float rot;
    float Xxx;
    float Yyy;
    float Degrees;
    std::vector<int> exceptions;

    std::set<int> Melee(int min, int max, int range, std::set<int> exception = std::set<int>())
    {
        std::set<int> Tar;
        if(FirstStrike == false)
        {
            Degrees = Math.AngleBetweenVectors(sf::Vector2f(xpos,ypos),TargetPos)-90;
            FirstStrike = true;
        }
        for(int i = 0; i != 30; i++)
        {
            Xxx = xpos + cosf(Degrees * PI/180) * range;
            Yyy = ypos + sinf(Degrees * PI/180) * range;
            Degrees += 1;
            Effectz.CreateLine(xpos,ypos,Xxx,Yyy,2,Blue);
            Tar = NPCTrace(xpos,ypos,Xxx,Yyy,id,std::set<int>());
        }
        std::vector<item>::iterator Inv;
        for( Inv = inventory.begin(); Inv != inventory.end(); ++Inv )
        {
            if(Inv->name == "Sword"){  }
        }
        if(Degrees >= Math.AngleBetweenVectors(sf::Vector2f(xpos,ypos),TargetPos)+90)
        {
            Attacking = false;
            FirstStrike = false;
        }
        return Tar;
    }

    class PathFinding{
        public:
        int* MypathBank[1];
        int MypathLength;     //stores length of the found path for critter
	    int MypathLocation;   //stores current position along the chosen path for critter
        int MypathStatus;
        int MyxPath;
	    int MyyPath;
	    std::vector<int> xPa;
	    std::vector<int> yPa;
        void EndMem(){ free (MypathBank[1]); }
        bool MyFindPath(int Sx, int Sy, int Ex, int Ey){
            xPa.clear();
            yPa.clear();
            MypathStatus = FindPath(1,Sx,Sy,Ex,Ey);
            std::cout << "MypathStatus; " << MypathStatus;
            MypathLocation = pathLocation[1];
            MypathLength = pathLength[1];
            //MypathBank[1] = (int*) realloc (pathBank[1],pathLength[1]*8);
            for( int Stuff = 0; Stuff != MypathLength; ++Stuff )
            {
                //MypathBank[1] [Stuff*2-2] = pathBank[1] [Stuff*2-2];
                yPa.push_back(pathBank[1] [Stuff*2-1]);
                //std::cout << "*Added*:" << xPa.at(Stuff-1)<<std::endl;
                //MypathBank[1] [Stuff*2-1] = pathBank[1] [Stuff*2-1];
                xPa.push_back(pathBank[1] [Stuff*2-2]);
            }
        }

        int MyReadPathX(int pathfinderID,int xpathLocation){
            pathfinderID = 1;
	        int x;
	        if (xpathLocation <= MypathLength)
	        {
	            //Read coordinate from bank
	            //x = MypathBank[pathfinderID] [xpathLocation*2-2];
	            //Like... Yo bo... Ty this instead.
                try{x = xPa.at(xpathLocation);}catch (std::exception& e) {}
	            //Adjust the coordinates so they align with the center
	            //of the path square (optional). This assumes that you are using
	            //sprites that are centered -- i.e., with the midHandle command.
	            //Otherwise you will want to adjust this.
	            x = tileSize*x + .5*tileSize;
	        }
	        return x;
        }

//-----------------------------------------------------------------------------
// Name: ReadPathY
// Desc: Reads the y coordinate of the next path step
//-----------------------------------------------------------------------------
        int MyReadPathY(int pathfinderID,int ypathLocation){
            pathfinderID = 1;
	        int y;
	        if (ypathLocation <= MypathLength)
	        {
	            //Read coordinate from bank
	            //y = MypathBank[pathfinderID] [ypathLocation*2-1];
                try
                {
	                y = yPa.at(ypathLocation);
                }
	            catch (std::exception& e) {}
	            //Adjust the coordinates so they align with the center
	            //of the path square (optional). This assumes that you are using
	            //sprites that are centered -- i.e., with the midHandle command.
	            //Otherwise you will want to adjust this.
	            y = tileSize*y + .5*tileSize;
            }
	        return y;
        }

        bool MyReadPath(int pathfinderID,int currentX,int currentY,int pixelsPerFrame){
	        int ID = 1; //redundant, but makes the following easier to read
            //If a path has been found for the pathfinder	...
            if (MypathStatus == AStarFound)
	        {
		        //If path finder is just starting a new path or has reached the
		        //center of the current path square (and the end of the path
		        //hasn't been reached), look up the next path square.
		        if (MypathLocation < MypathLength)
		        {
			        //if just starting or if close enough to center of square
			        if (MypathLocation == 0 || (abs(currentX - MyxPath) < pixelsPerFrame && abs(currentY - MyyPath) < pixelsPerFrame)) MypathLocation = MypathLocation + 1;
		        }
		        //Read the path data.
		        MyxPath = MyReadPathX(ID,MypathLocation);
		        MyyPath = MyReadPathY(ID,MypathLocation);
		        //If the center of the last path square on the path has been
		        //reached then reset.
		        if (MypathLocation == MypathLength)
		        {
			        if (abs(currentX - MyxPath) < pixelsPerFrame && abs(currentY - MyyPath) < pixelsPerFrame) //if close enough to center of square
					MypathStatus = notStarted;
		        }
	        }
	        //If there is no path for this pathfinder, simply stay in the current
 	        //location.
	        else
	        {
		        MyxPath = currentX;
		        MyyPath = currentY;
	        }
        }

        PathFinding(){//MypathBank [1] = (int*) malloc(4);
        }
    };
    PathFinding PathFinding;

    int imgstrx;
    int imgstry;
    int imgendx;
    int imgendy;
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
    std::string name; // .c_str()
    char surname;
    bool alive;
    bool stillalive;
    int ticksalive;
    bool useditem;
    bool canmove;
    std::string target;
    sf::Vector2f TargetPos;
    int TargetId;
    int TargetVectorId;
    bool AtTarget;
    bool HasTarget;
    std::string action;
    int regenrate;
    int regentimer;
    int regentimerint;
    float moverateint;
    float moverate;
    float movetimer;
    float movetimerint;
    float movetimerrate;
    float attacktimer;
    float attacktimerint;
    int hungerrate;
    int hungertimer;
    int hungertimerint;
    int thirstrate;
    int thirsttimer;
    int thirsttimerint;
    int breathrate;// not needed?
    int breathtimer;
    int breathtimerint;
    std::string race;
    int Age;
    int AgeType; // 1 = Babeh, 2 = Child, 3 = Teenager, 4 = Young Adult, 5 = Aged Adult, 6 = Elder
    std::string gender;
    int direction;
    int worshippers;
    int gypos;
    int gxpos;
    int planet;
    float ypos;
    float xpos;
    int rypos;
    int rxpos;
    int maxhealth;
    float health;
    int mana;
    int reginterntemp;
    int interntemp;
    int regtemp;
    int temp;
    int breathmax;
    int maxhunger;
    float hunger;
    int nutrition;
    int maxthirst;
    float thirst;
    int maxstamina;
    int stamina;
    int credits;

    class Body
    {
        public:  //Probably toss all these into a "Body Parts" Class Vector, Name, Bools, Ints, Ect. It'd allow for custom body parts and what not.
        int head;
        int eyes;// Mask = Eyes+Mouth
        int mouth;// Mask = Eyes+Mouth
        int neck;
        int chest;
        int back;
        int waist;
        int legs;
        int rightupperarm; // int rua
        int rightlowerarm; // int rla
        int righthand; // rh
        int rightgrasp; // rg
        int leftupperarm; // int lua
        int leftlowerarm; // int lla
        int lefthand; // lh
        int leftgrasp; // lg
        int rightupperleg; // rul
        int rightlowerleg; // rll
        int rightfoot; // rf
        int leftupperleg; // lul
        int leftlowerleg; // lll
        int leftfoot; // lf
    };

    bool EffectStats()
    {
        maxhealth = Skills.endurance*0.8;
        regenrate = Skills.endurance/10;
    }

    class Skill
    {
        public:// Read class Body's Public:
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

        bool Train(std::string skill, int amount = 1, int skillgain = 1){
            if(skill == "agility"){agilityxp+=amount;if(agilityxp >= 100){agility+=skillgain; agilityxp = 0;}}
            if(skill == "charisma"){charismaxp+=amount;if(charismaxp >= 100){charisma+=skillgain; charismaxp = 0;}}
            if(skill == "intelligence"){intelligencexp+=amount;if(intelligencexp >= 100){intelligence+=skillgain; intelligencexp = 0;}}
            if(skill == "dexterity"){dexterityxp+=amount;if(dexterityxp >= 100){dexterity+=skillgain; dexterityxp = 0;}}
            if(skill == "strength"){strengthxp+=amount;if(strengthxp >= 100){strength+=skillgain; strengthxp = 0;}}
            if(skill == "perception"){perceptionxp+=amount;if(perceptionxp >= 100){perception+=skillgain; perceptionxp = 0;}}
            if(skill == "endurance"){endurancexp+=amount;if(endurancexp >= 100){endurance+=skillgain; endurancexp = 0;}}
            if(skill == "wisdom"){wisdomxp+=amount;if(wisdomxp >= 100){wisdom+=skillgain; wisdomxp = 0;}}

        }
    };
    Skill Skills;

    int killcount;
    int reach;
    int size;
    int stench;
    float viewangle;
    float viewrange;

    OldNPC(){
        ToDelete = false;
        viewangle = 180;
        viewrange = 200;
        stench = 0; killcount = 0; id = Globals.globalid++; NeedsFood = true; NeedsWater = true; TargetVectorId = -1;TargetId = -1; angle = 1;
        AllowedDrink = true; AllowedFood = true; AllowedMove = true;
        AtTarget = false;HasTarget = false;target = "Food";action = "Act";
        moverateint = 1;moverate = moverateint;movetimerint = 1000;movetimer = movetimerint; movetimerrate = 500;
        attacktimer = 0; attacktimerint = 25;
        maxhunger = 600;hungertimerint = 10;hungertimer = hungertimerint;hungerrate = 1;hunger = 600;
        maxthirst = 600;thirsttimerint = 5;thirsttimer = thirsttimerint;thirstrate = 1;thirst = 600;
        reach = 10;
        Skills.endurance = randz(20,80);
        Skills.strength = randz(20,80);
        Skills.dexterity = randz(20,80);
        Skills.intelligence = randz(20,80);
        Skills.wisdom = randz(20,80);
        Skills.charisma = randz(20,80);
        Skills.perception = randz(20,80);
        Skills.agility = randz(20,80);
        Skills.endurancexp = 0;
        Skills.strengthxp = 0;
        Skills.dexterityxp = 0;
        Skills.intelligencexp = 0;
        Skills.wisdomxp = 0;
        Skills.charismaxp = 0;
        Skills.perceptionxp = 0;
        Skills.agilityxp = 0;
        maxhealth = Skills.endurance*0.8;regentimerint = 100;regentimer=regentimerint;regenrate = Skills.endurance/10;health = maxhealth;
        stench = 50; race = "Human";
        int numz = randz(0,1); if(numz==0){gender = "Male";}else{gender = "Female";}
        int numzb = randz(0,1); if(numzb==0){gender = "Male";}else{gender = "Female";}
        if(gender == "Male")
        {
            int numzb = randz(0,6);
            if(numzb == 0){name = "John";}
            else if(numzb == 1){ name = "Jack";}
            else if(numzb == 2){ name = "Jonathan";}
            else if(numzb == 3){ name = "James";}
            else if(numzb == 4){ name = "Joseph";}
            else if(numzb == 5){ name = "Joe";}
            else if(numzb == 5){ name = "Justin";}
        };
        if(gender == "Female")
        {
            int numzb = randz(0,5);
            if(numzb==0){name = "Jennifer";}
            else if(numzb==1){name = "Jill";}
            else if(numzb==2){name = "Jamie";}
            else if(numzb==3){name = "Jackie";}
            else if(numzb==4){name = "Joan";}
            else if(numzb==5){name = "Jessica";}
        };
        //img.SetImage(Images);
        IsNPC = true;
        NeedsFood = true;
        AllowedFood = true;
        NeedsWater = true;
        AllowedDrink = true;
        NeedsPath = false;
    }

    ~OldNPC(){}

    void ReCreateSkills(){

        Skills.endurance = randz(20,80);
        Skills.strength = randz(20,80);
        Skills.dexterity = randz(20,80);
        Skills.intelligence = randz(20,80);
        Skills.wisdom = randz(20,80);
        Skills.charisma = randz(20,80);
        Skills.perception = randz(20,80);
        Skills.agility = randz(20,80);
        Skills.endurancexp = 0;
        Skills.strengthxp = 0;
        Skills.dexterityxp = 0;
        Skills.intelligencexp = 0;
        Skills.wisdomxp = 0;
        Skills.charismaxp = 0;
        Skills.perceptionxp = 0;
        Skills.agilityxp = 0;
        maxhealth = Skills.endurance*0.8;
        regentimerint = 100;
        regentimer=regentimerint;
        regenrate = Skills.endurance/10;
        health = maxhealth;
    }

    void BlankSkills(){

        Skills.endurance = 20;
        Skills.strength = 20;
        Skills.dexterity = 20;
        Skills.intelligence = 20;
        Skills.wisdom = 20;
        Skills.charisma = 20;
        Skills.perception = 20;
        Skills.agility = 20;
        Skills.endurancexp = 0;
        Skills.strengthxp = 0;
        Skills.dexterityxp = 0;
        Skills.intelligencexp = 0;
        Skills.wisdomxp = 0;
        Skills.charismaxp = 0;
        Skills.perceptionxp = 0;
        Skills.agilityxp = 0;
        maxhealth = Skills.endurance*0.8;
        regentimerint = 100;
        regentimer=regentimerint;
        regenrate = Skills.endurance/10;
        health = maxhealth;
    }


    bool HasWeapon(std::string weapon = ""){
        bool Specific = false;
        bool HasSpecifiedWeapon = false;
        bool FoundWeapon = false;
        if(weapon != "") Specific = true;
        std::vector<item>::iterator Items;
        try
        {//You should always Try when dealing with vectors that could be empty.
            for(Items = inventory.begin(); Items != inventory.end(); Items++)
            {
                if(Items->IsWeapon) FoundWeapon = true;
                if(weapon != "")
                {
                    if(Items->name == weapon) HasSpecifiedWeapon = true;
                }
            }
            if(HasSpecifiedWeapon){return true;}
            if(FoundWeapon && Specific == false){return true;}
        } catch (std::exception& e){}/*This keeps the game from crashing if there is nothing in the vector.*/
        return false;
    }

    item * GetItemType(int type){
        for(int i = 0; i < inventory.size(); i++)
        {
            if( inventory[i].type == type )
            {
                if(Debug){std::cout << "Returning inventory class \n";} return &inventory[i];
            }
        }
        if(Debug){std::cout << "Returning inventory nothing. \n";}
    }

    bool HasItemType(int type){
        for(int i=0; i<inventory.size(); i++)
        {
            if( inventory[i].type == type )
            {
                if(Debug){std::cout << "returning inventory bool true \n";}return true;
            }
        }
        if(Debug){std::cout << "returning inventory bool false \n";}return false;
    }

    //bool SetImg(int istrx,int istry,int iendx,int iendy){if(iendx == 0 || iendy == 0){img.SetSubRect(sf::IntRect(imgstrx, imgstry, imgendx, imgendy));}else{img.SetSubRect(sf::IntRect(istrx, istry, iendx, iendy));}}

    void DrawImg(){
        img.setPosition(xpos,ypos);
        App.draw(img);
    }

    bool Move(sf::Vector2f Tar){
        bool Above = false;
        bool Right = false;
        bool AtTargetx = false;
        bool AtTargety = false;
        if(Tar.x < xpos){Right = false;}
        else if(Tar.x == xpos){AtTargetx = true;}
        else{Right = true;}

        if(Tar.y < ypos){Above = true;}
        else if(Tar.y == ypos){AtTargety = true;}
        else{Above = false;}

        if(AtTargetx == true && AtTargety == true){ AtTarget = true;}
        else
        {
            if(AtTargety == true){}
            else if(Above == true){int numz = CloseishS(Tar.y,ypos);  if( numz < moverate){ypos += numz; }else{ ypos += moverate;}}
            else{int numz = CloseishS(Tar.y,ypos);  if( numz < moverate){ypos -= numz; }else{ ypos -= moverate;}}

            if(AtTargetx == true){}
            else if(Right == true){int numz = CloseishS(Tar.x,xpos);  if( numz < moverate){xpos += numz; }else{ xpos += moverate;}}
            else{int numz = CloseishS(Tar.x,xpos);  if( numz < moverate){xpos -= numz; }else{ xpos -= moverate;}}
        }
        angle = (180/PI)*(atan2f(xpos-Tar.x,ypos-Tar.y));
    }

    bool DirMove(sf::Vector2f Tar){
        bool Above = false;
        bool Right = false;
        bool AtTargetx = false;
        bool AtTargety = false;
        bool CloseX = false;
        bool CloseY = false;
        if(Tar.x < xpos){Right = false;}
        else if(Tar.x == xpos){AtTargetx = true;}
        else{Right = true;}

        if(Tar.y < ypos){Above = false;}
        else if(Tar.y == ypos){AtTargety = true;}
        else{Above = true;}

        if(AtTargetx == true && AtTargety == true){ AtTarget = true;}
        else
        {
            if(AtTargetx == false && AtTargety == false){}
            int numz0 = CloseishS(Tar.y,ypos); int numz1 = CloseishS(Tar.x,xpos);
            if( numz1 < moverate && numz0 < moverate){xpos = Tar.x; ypos = Tar.y;CloseX = true;CloseY = true;}
            else{AtTarget = false;}
        }


        if(CloseY == false && CloseX == false)
        {
            int xx = 0;
            int yy = 0;
            angle = 90-(180/PI)*(atan2f(xpos-Tar.x,ypos-Tar.y));//To be honest, I spent alot of time with trial and error to get this part to work.
            xx = cosf((angle) * PI/180) * moverate;
            yy = sinf((angle) * PI/180) * moverate;
            xpos -= xx;
            ypos -= yy;
        }

        int dx = Tar.x - xpos, dy = Tar.y - ypos, steps, k;
        float xIncrement, yIncrement, x = xpos, y = ypos;
        if(abs(dx) > abs(dy)) steps = abs(dx);
        else steps = abs(dy);
        xIncrement = dx / (float) steps;
        yIncrement = dy / (float) steps;
        for(int k = 0; k < steps; k++)
        {
            x += xIncrement;
            y += yIncrement;
        }
        //xpos += x/steps;
        //ypos += y/steps;
    }

    bool MoveNorth(){ypos = ypos -= moverate;};
    bool MoveNorthEast(){ypos = ypos -= moverate;xpos = xpos += moverate;};
    bool MoveEast(){xpos = xpos += moverate;};
    bool MoveSouthEast(){ypos = ypos += moverate;xpos = xpos += moverate;};
    bool MoveSouth(){ypos = ypos += moverate;};
    bool MoveSouthWest(){ypos = ypos += moverate;xpos = xpos -= moverate;};
    bool MoveWest(){xpos = xpos -= moverate;};
    bool MoveNorthWest(){ypos = ypos -= moverate;xpos = xpos -= moverate;};

    bool IsHungry(){ if(AllowedFood == false){return false;}  if(hunger < (maxhunger*0.40)){return true;}else{return false;}  }
    bool IsThirsty(){ if(AllowedDrink == false){return false;}  if(thirst < (maxthirst*0.40)){return true;}else{return false;}  }
    bool IsHurt(){  if(health < (maxhealth*0.60)){return true;}else{return false;}  }

    bool breath(){};

    int sethealth(int amount){health = amount;}
    bool modhealth(int amount){health += amount;if(health < 1){health = 0;return false;} else{return true;}}

    int fillhunger(int amount){hunger += amount; if(hunger > maxhunger){hunger = maxhunger;}};
    int fillthirst(int amount){thirst += amount; if(thirst > maxthirst){thirst = maxthirst;}};

    std::string getname(){return name;}

    void spawn(int gposx,int gposy,int rposx,int rposy,int posx,int posy){gxpos = 100;gypos = 100;rxpos = 100;rypos = 100;xpos = posx;ypos = posy; HasSpawned = true;}

    bool boolblood(std::string ailment){
        using namespace std;
        size_t found;
        found = bloodcontent.find(ailment);
        if(found != string::npos)
        {
            //cout <<aliment<<" found at: " << int(found) << endl;
            return true;
        }
        else return false;
    }

    bool HasTag(std::string tag){
        using namespace std;
        size_t found;
        found = tags.find(tag);
        if(found != string::npos)
        {
            return true;
        }
        else return false;
    }

    bool Tag(std::string tag, int amount){
        using namespace std;
        size_t found;
        found = tags.find(tag);
        if(found != string::npos)
        {
            //cout <<tag<<" found at: " << int(found) << endl;
            size_t foundz;
            std::string funz;
            foundz = tags.find("]",found+1);
            if(foundz != string::npos)
            {
                int len = tag.length();funz.assign(tags,found+len, foundz-(found+len));
                amount += atoi(funz.c_str());
                std::string amo;
                std::stringstream out;out << amount;amo = out.str();
                tags.replace(found+len, foundz-(found+len),out.str());
                //cout<<"Out is:"<<out.str()<<endl;
                if(out.str() == "0")
                {
                    //cout<<"out is 0, It at least it's supposed to be\n";
                    tags.replace(found,foundz,"");
                }
            }
        }
        else if(amount == 0)
        {
            //cout<<"No "<<tag<<" Here\n";
        }
        else if(amount > 0)
        {
            tags.assign(tag);
            std::string amo;
            std::stringstream out;
            out << amount;
            amo = out.str();
            tags.append(out.str());
            tags.append("]");
        }

        /*
        found=tags.find(Zom);if (found!=string::npos){cout << "Zombification found at: " << int(found) << endl;}else{cout<<"No Zombification Here\n";}
        }*/

    }

    bool bloodwork(std::string aliment,int amount){
        using namespace std;
        size_t found;
        found=bloodcontent.find(aliment);
        if(found!=string::npos)
        {
            //cout <<aliment<<" found at: " << int(found) << endl;
            size_t foundz;
            std::string funz;
            foundz=bloodcontent.find("]",found+1);
            if(foundz!=string::npos)
            {
                int len = aliment.length();
                funz.assign(bloodcontent,found+len, foundz-(found+len));
                amount += atoi(funz.c_str());
                std::string amo;
                std::stringstream out;
                out << amount;
                amo = out.str();
                bloodcontent.replace(found+len, foundz-(found+len),out.str());
                //cout<<"Out is:"<<out.str()<<endl;
                if(out.str() == "0")
                {
                    //cout<<"out is 0, It at least it's supposed to be\n";
                    bloodcontent.replace(found,foundz,"");
                }
            }
        }
        else if(amount == 0)
        {
            //cout<<"No "<<aliment<<" Here\n";
        }
        else if(amount > 0)
        {
            bloodcontent.assign(aliment);
            std::string amo;
            std::stringstream out;
            out << amount;
            amo = out.str();
            bloodcontent.append(out.str());
            bloodcontent.append("]");
        }
        /*

        found=bloodcontent.find(Zom);if (found!=string::npos){cout << "Zombification found at: " << int(found) << endl;}else{cout<<"No Zombification Here\n";}

        }*/

    }

    bool CloseToTarget(int distance, sf::Vector2f Tar){
        int numbz = 0;
        numbz = Math.Closeish(xpos,ypos,Tar.x,Tar.y);
        if(numbz <= distance){return true;}
        else {return false;}
    }

    int AddItem(char *itemname, int amount){// TODO: Set this to optionally receive an item class instead, Will be useful for modded weapons and ect.
         if(Debug){ std::cout << "Pre var in AddItem" << " \n";}
         cItem var = *GetGlobalItem(itemname);
         if(Debug){ std::cout << "Post var in AddItem" << " \n";}
         for(int Times = 0; Times != amount; Times++)
         {
             inventory.push_back(var);
         }
    }

    int minmeleedamage;
    int maxmeleedamage;
    int minrangeddamage;
    int maxrangeddamage;

    int dirgrid[gridy][gridx];
    int valuegrid[gridy][gridx];
    int followgrid[gridy][gridx];

    void PrintConsoleInfo(){
        using namespace std;
        cout << "Name: " << name << endl;
        cout << "cBaseId: " << cbaseid << endl;
        cout << "Id: " << id << endl;
        cout << "Xpos: " << xpos << endl;
        cout << "Ypos: " << ypos << endl;
        if(alive == true){cout << " Alive" << endl;}
        if(alive == false){cout << " Dead" << endl;}
        cout << "Health: " << health << endl;
        cout << "Hunger: " << hunger << endl;
        cout << "Thirst: " << thirst << endl;
        cout << "Kill Count: " << killcount << endl;
        cout << "Target is " << target << " at:"<< TargetPos.x <<","<<TargetPos.y<< endl;
    }

    void PrintBloodContent(){
        using namespace std;
        cout << "Name: " << name << endl;
        cout<<bloodcontent<<endl;
    }

    bool operator<(NPC const & other) const { return other.id > id; }


};
NPC NPCz;

void SetTestage()
{

    clock_t sInital = clock();
    std::set<NPC> TestSet;
    for(int n = 0; n != 500; n++)
    {
        NPC npc;
        TestSet.insert(npc);
    }
    clock_t sCreation = clock();
    int sJohns = 0;
    std::set<NPC>::iterator it;
    for(it = TestSet.begin(); it != TestSet.end(); it++)
    {
        if(it->name == "John")
        {
            sJohns++;
        }
        if(it->id == 10)
        {
            //sJohns++;
        }
        if(it->health == 10)
        {
            //sJohns++;
        }
        if(it->id == 10)
        {
            //sJohns++;
        }
        if(it->health == 10)
        {
            //sJohns++;
        }
        if(it->id == 10)
        {
            //sJohns++;
        }
        if(it->health == 10)
        {
            //sJohns++;
        }
        //else Con(it->name);
    }
    clock_t sFinished = clock();

}


void TimeTest()
{

    time_t timer;
    time_t timer2;
    //struct tm y2k;
    double seconds;

    //y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0; y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

    time(&timer);  // get current time; same as: timer = time(NULL)

    for(unsigned int i = 0; i != 100; i++)
    {
        //int Cake = randz(5,10);
        //std::cout << "Cake is: " << Cake << std::endl;
        //std::string TextLine = "Cake is: " + std::to_string(Cake); I need to upgrade to C++ 11
        //PrintToFile("Cake.txt",AddString("Cake Is: ",NumbertoString(Cake)), true);
    }
    time(&timer2);

    seconds = difftime(timer,timer2);

    //

    unsigned long long Int64 = 0;
    clock_t Start = clock();

    for(unsigned int i = 0; i != 100; i++)
    {
        //int Cake = randz(5,10);
        //std::cout << "Cake is: " << Cake << std::endl;
        //PrintToFile("Cake.txt",AddString("Cake Is: ",NumbertoString(Cake)), true);
    }


    std::cout << "Time Difference: " << clock() - Start << std::endl;
    printf ("%.f second difference", seconds);





    clock_t vInital = clock();
    std::vector<NPC> TestVector;
    for(int n = 0; n != 500; n++)
    {
        NPC npc;
        TestVector.push_back(npc);
    }
    clock_t vCreation = clock();
    int vJohns = 0;
    for(int n = 0; n != TestVector.size(); n++)
    {
        if(TestVector[n].name == "John")
        {
            vJohns++;
        }
        if(TestVector[n].id == 10)
        {
            //vJohns++;
        }
        if(TestVector[n].health == 10)
        {
            //vJohns++;
        }
        if(TestVector[n].id == 10)
        {
            //vJohns++;
        }
        if(TestVector[n].health == 10)
        {
            //vJohns++;
        }
        if(TestVector[n].id == 10)
        {
            //vJohns++;
        }
        if(TestVector[n].health == 10)
        {
            //vJohns++;
        }
        //else Con(TestVector[n].name);
    }
    clock_t vFinished = clock();



    clock_t sInital = clock();
    std::list<NPC> TestSet;
    for(int n = 0; n != 500; n++)
    {
        NPC npc;
        TestSet.push_back(npc);
    }
    clock_t sCreation = clock();
    int sJohns = 0;
    std::list<NPC>::iterator it;
    for(it = TestSet.begin(); it != TestSet.end(); it++)
    {
        if(it->name == "John")
        {
            sJohns++;
        }
        if(it->id == 10)
        {
            //sJohns++;
        }
        if(it->health == 10)
        {
            //sJohns++;
        }
        if(it->id == 10)
        {
            //sJohns++;
        }
        if(it->health == 10)
        {
            //sJohns++;
        }
        if(it->id == 10)
        {
            //sJohns++;
        }
        if(it->health == 10)
        {
            //sJohns++;
        }
        //else Con(it->name);
    }
    clock_t sFinished = clock();




    Con(TestSet.size());
    Con(sizeof(NPC));
    std::cout << "Vector Time, Creation: " << vCreation - vInital << ", John Counter: " << vFinished - vCreation << ", Total(" << vFinished - vInital << "), Johns: " << vJohns << std::endl;
    std::cout << "Set Time, Creation: " << sCreation - sInital << ", John Counter: " << sFinished - sCreation << ", Total(" << sFinished - sInital << "), Johns: " << sJohns << std::endl;















}







class cNpcManager
{
    public:
    std::vector<NPC> GlobalCritter;
    std::vector<NPC> AddedCritters;

    void AddCritters(){
        std::vector<NPC>::iterator Me;
        for(Me = AddedCritters.begin();Me != AddedCritters.end();Me++)
        {
            npclist.push_back(*Me);
        }
        AddedCritters.clear();
    }

    void InitializeCritters(){

       /*using namespace std;  I'll be using this soon enough, It'll allow the program to surf through the folder, and create all the stuff it needs.
        string dir = string(".//data//items");
        vector<string> files = vector<string>();
        getdir(dir,files);
        for(unsigned int i = 0; i < files.size(); i++)
        {
            string line("saves/maps/");
            string ending(files[i]);
            line.append(ending);
            if( remove(line.c_str()) != 0 ){}
        } */

        std::ifstream Input("data/npcs.txt"); // TODO: Have this read from an Items folder, and read from all .txt files in it, Allowing greater compability between mods.
        if(Input.is_open())
        {
            int stuffs;
            while(Input.good())
            {
                std::string line;
                getline (Input,line);
                NPC Critter;
                Critter.name = "Debuggery";
    /*attacktimer = 0; attacktimerint = 25; TODO: Not a clue if I should let these be moddable.*/

    //hungerrate = 1; // TODO: Should these be modded? Or only effected by Diseases/Bionics ect.
    //thirstrate = 1;

                Critter.name = StringFindString(line,"[Name:");
                Critter.race = Critter.name;
                if(Critter.name == "Zombie") Critter.race = "Zombie";
                if(Debug){ std::cout << "Working on " << Critter.name << "\n";}
                Critter.cbaseid = StringFindNumber(line,"[BaseId:");
                Critter.id = Globals.globalid++;

                Critter.target = StringFindString(line,"[Target:");
                Critter.NeedsFood = Booleanize(StringFindNumber(line,"[NeedsFood:"));
                Critter.AllowedFood = Booleanize(StringFindNumber(line,"[AllowedFood:"));
                Critter.NeedsWater = Booleanize(StringFindNumber(line,"[NeedsWater:"));
                Critter.AllowedDrink = Booleanize(StringFindNumber(line,"[AllowedDrink:"));
                Critter.canmove = Booleanize(StringFindNumber(line,"[CanMove:"));
                if(Debug){ std::cout << "1 \n";}
                Critter.maxhealth = StringFindNumber(line,"[MaxHealth:");
                Critter.maxhunger = StringFindNumber(line,"[MaxHunger:");
                Critter.maxthirst = StringFindNumber(line,"[MaxThirst:");
                Critter.regentimerint = StringFindNumber(line,"[RegenRate:");

                Critter.breathtimerint = StringFindNumber(line,"[BreathTimerInt:");
                Critter.hungertimerint = StringFindNumber(line,"[HungerTimerInt:");
                Critter.thirsttimerint = StringFindNumber(line,"[ThirstTimerInt:");

                Critter.moverateint = StringFindNumber(line,"[MoveSpeed:");
                Critter.movetimerrate = StringFindNumber(line,"[MoveRate:");
                Critter.viewangle = StringFindNumber(line,"[ViewAngle:");
                Critter.viewrange = StringFindNumber(line,"[ViewRange:");
                Critter.size = StringFindNumber(line,"[Size:");
                Critter.reach = StringFindNumber(line,"[Reach:");
                Critter.stench = StringFindNumber(line,"[Stench:");
                if(Debug){ std::cout << "2 \n";}
                Critter.Skills.endurance = randz(StringFindNumber(line,"[MinEnd:"),StringFindNumber(line,"[MaxEnd:"));
                Critter.Skills.strength = randz(StringFindNumber(line,"[MinStr:"),StringFindNumber(line,"[MaxStr:"));
                Critter.Skills.dexterity = randz(StringFindNumber(line,"[MinDex:"),StringFindNumber(line,"[MaxDex:"));
                Critter.Skills.intelligence = randz(StringFindNumber(line,"[MinInt:"),StringFindNumber(line,"[MaxInt:"));
                Critter.Skills.wisdom = randz(StringFindNumber(line,"[MinWis:"),StringFindNumber(line,"[MaxWis:"));
                Critter.Skills.charisma = randz(StringFindNumber(line,"[MinCha:"),StringFindNumber(line,"[MaxCha:"));
                Critter.Skills.perception = randz(StringFindNumber(line,"[MinPer:"),StringFindNumber(line,"[MaxPer:"));
                Critter.Skills.agility = randz(StringFindNumber(line,"[MinAgi:"),StringFindNumber(line,"[MaxAgi:"));
                if(Debug){ std::cout << "3 \n";}
                if(Critter.maxhealth == -1){Critter.maxhealth = Critter.Skills.endurance*0.8;}
                debug("v-Adding Tags-v");
                debug(StringFindChaos(line,"{Tags:","}"));
                debug("^-Added Tags-^");
                Critter.tags.append(StringFindChaos(line,"{Tags:","}"));
                std::set<std::string> Items;
                std::string List;
                List.append(StringFindChaos(line,"{Items:","}"));
                Items = StringFindSetChaos(List,"[","]");
                for(std::set<std::string>::iterator i = Items.begin(); i != Items.end(); i++)
                {
                    std::string OogaBooga = *i;
                    char *a=new char[OogaBooga.size()+1];a[OogaBooga.size()]=0;memcpy(a,OogaBooga.c_str(),OogaBooga.size());
                    if(Debug){ std::cout << "Pre Critter.AddItem \n";}
                    Critter.AddItem(a,1);
                    if(Debug){ std::cout << "*Post Critter.AddItem* \n";}
                }

                /*===================================================*/

                Critter.action = "Act";
                Critter.alive = true;
                Critter.HasSpawned = true;
                Critter.movetimerint = 1000;
                Critter.moverate = Critter.moverateint;

                Critter.health = Critter.maxhealth;
                Critter.hunger = Critter.maxhunger;
                Critter.thirst = Critter.maxthirst;
                if(Critter.HasTag("[EatsFlesh:"))
                {
                    Critter.hunger = 10;
                    Critter.thirst = 0;
                }
                Critter.regentimer = Critter.regentimerint;
                Critter.regenrate = Critter.Skills.endurance/10; // TODO: Have Skill based values update in the Train() function, So that stuff like Regen doesn't fall behind.

                Critter.breathtimer = Critter.breathtimerint;
                Critter.hungertimer = Critter.hungertimerint;
                Critter.thirsttimer = Critter.thirsttimerint;
                Critter.NeedsPath = false;

                if(true == true) // Critter Bodies
                {
                    if(Critter.race == "Human" || Critter.race == "Zombie" )
                    {
                        Critter.Body.BodyParts = "{[Name:UpperTorso][BloodPumpRate:100][AirCapacity:200][AirAbsorbtion:100][ObjectCapacity:1][MassFlesh:15:1000]}";
                        Critter.Body.BodyParts.append("{[Name:Head][Mind:true][Orafice:1][MassFlesh:5:1000][Dependant:UpperTorso]}");
                        Critter.Body.BodyParts.append("{[Name:LowerTorso][ObjectCapacity:10][DigestionRate:125][NutritionExtraction:10][DigestsFlesh:60][DigestsVeggy:60][DigestsWater:100][MassFlesh:15:1000][Dependant:UpperTorso]}");

                        Critter.Body.BodyParts.append("{[Name:Left Leg][Walk:3][MassFlesh:15:1000][Dependant:LowerTorso]}");
                        Critter.Body.BodyParts.append("{[Name:Right Leg][Walk:3][MassFlesh:15:1000][Dependant:LowerTorso]}");

                        Critter.Body.BodyParts.append("{[Name:Left Arm][Grasp:2][MassFlesh:10:1000][Dependant:UpperTorso]}");
                        Critter.Body.BodyParts.append("{[Name:Right Arm][Grasp:2][MassFlesh:10:1000][Dependant:UpperTorso]}");
                    }





                }



                std::string Imagery = StringFindString(line,"[Image:");
                std::vector<cImageHolder>::iterator i;
                if(Debug){ std::cout << "Pre Imagery \n";}
                for(i = imagemanager.GlobalImage.begin(); i != imagemanager.GlobalImage.end(); i++)
                {
                    if(i->name == Imagery)
                    {
                        Critter.img.setTexture(i->Image);
                        //Critter.img.SetSubRect(sf::IntRect(0, 0, i->Image.GetWidth(),i->Image.GetHeight()));
                        //Critter.img.SetCenter(i->Image.GetWidth()/2,i->Image.GetHeight()/2);
                    }
                }
                if(Critter.name != "Debuggery"){GlobalCritter.push_back(Critter);}
                if(Debug){ std::cout << "Post Imagery \n";}
            }
        }
    }

    cNpcManager()
    {

    }
}; cNpcManager npcmanager;


class MakeSquad
{
    public:
    float MakeSquadPoints;
    int SquadMates;
    std::vector<NPC> Squad;
    int Aim;

    MakeSquad()
    {
        MakeSquadPoints = 1000;
        SquadMates = 3;
        Aim = 0;
    }
};
MakeSquad Squady;

std::vector<NPC> WorldCritters;

int GetNpcVectorId(int id);

std::set<int> NPCTrace(int xa, int ya, int xb, int yb,int id, std::set<int> exceptions = std::set<int>()){//.at(0) = Item/NPC(23/69) .at(1) = id

    int dx = xb - xa, dy = yb - ya, steps, k;
    float xIncrement, yIncrement, x = xa, y = ya;
    if(abs(dx) > abs(dy)) steps = abs(dx);
    else steps = abs(dy);
    xIncrement = dx / (float) steps;
    yIncrement = dy / (float) steps;
    std::set<int> SetID;


    for(int k = 0; k < steps; k++){

        x += xIncrement;
        y += yIncrement;
        if(Key.g){Effectz.CreateCircle(x,y,1,Blue);}
        bool Kill = false;
        int Count = 0;
        if(true == true){// Merely doing this so I can reuse the same code, but for items, Hehe.
        std::vector<NPC>::iterator Me;

        for( Me = npclist.begin(); Me != npclist.end(); ++Me ){
            if(Math.Closeish(x,y,Me->xpos,Me->ypos) <= Me->size && Me->id != id){

                try{SetID.insert( GetNpcVectorId(Me->id));
            } catch (std::exception& e){std::cout<<"SetError\n";}

            }//23 = Item, 69 = NPC
            Count++;
        }
        }
    }
    if(SetID.size() == 0){SetID.insert(-1);}
    return SetID;
}


void SaveNPC(int planet, sf::Vector2i Region, NPC &Critter){
    using namespace std;
    string line("data/maps/Planet");
    stringstream convert;
    convert << planet;
    line.append(convert.str());
    convert.clear();
    galaxy_mkdir(line);
    line.append("/npc");
    convert << "x";
    convert << Region.x;
    convert << "y";
    convert << Region.y;
    line.append(convert.str());
    string ending(".crit");
    line.append(ending);


    ofstream File;
    File.open(line.c_str(), fstream::in | fstream::ate);
    debug("looking for file...");

    if(File.is_open())
    {
        //File << std::endl;
        File << std::endl;
        Con("Problem? 3");
        File << "[name:" << Critter.name << "]" << "[race:" << Critter.race << "]" << "[xpos:" << Critter.xpos << "]" << "[ypos:" << Critter.ypos << "]"
        << "[strength:" << Critter.Skills.strength << "]" << "[perception:" << Critter.Skills.perception << "]" << "[intelligence:" << Critter.Skills.intelligence << "]"
        << "[charisma:" << Critter.Skills.charisma << "]" << "[endurance:" << Critter.Skills.endurance << "]" << "[dexterity:" << Critter.Skills.dexterity << "]"
        << "[agility:" << Critter.Skills.agility << "]" << "[health:" << Critter.health << "]"
        << "[action:" << Critter.action << "]" << "[angle:" << Critter.angle << "]" << "[thirst:" << Critter.thirst << "]" << "[hunger:" << Critter.hunger << "]"
        << "[strengthxp:" << Critter.Skills.strengthxp << "]" << "[perceptionxp:" << Critter.Skills.perceptionxp << "]" << "[intelligencexp:" << Critter.Skills.intelligencexp << "]"
        << "[charismaxp:" << Critter.Skills.charismaxp << "]" << "[endurancexp:" << Critter.Skills.endurancexp << "]" << "[dexterityxp:" << Critter.Skills.dexterityxp << "]"
        << "[agilityxp:" << Critter.Skills.agilityxp << "]"
        << "[cbaseid:" << Critter.cbaseid << "]" << "[maxhealth:" << Critter.maxhealth << "]"
        << "{Tags:" << Critter.tags << "}" << "{Blood:" << Critter.bloodcontent << "}"
        << "{Items:";
        Con("Problem? 4");

        //for(int i = 0; i != Critter.inventory.size(); i++)
        for(std::list<item>::iterator i = Critter.inventory.begin(); i != Critter.inventory.begin(); i++)
        {
            File << "[" << (*i).name << "]";
        }
        File << "}";
        Con("Problem? 5");
        //Con("Added", false);
        //Con(object.name);
        File.close();
    } // End to Load Map


    /*debug("v-Adding Tags-v");
                debug(StringFindChaos(line,"{Tags:","}"));
                debug("^-Added Tags-^");
                Critter.tags.append(StringFindChaos(line,"{Tags:","}"));
                std::set<std::string> Items;
                std::string List;
                List.append(StringFindChaos(line,"{Items:","}"));
                Items = StringFindSetChaos(List,"[","]");
                for(std::set<std::string>::iterator i = Items.begin(); i != Items.end(); i++)
                {
                    std::string OogaBooga = *i;
                    char *a=new char[OogaBooga.size()+1];a[OogaBooga.size()]=0;memcpy(a,OogaBooga.c_str(),OogaBooga.size());
                    if(Debug){ std::cout << "Pre Critter.AddItem \n";}
                    Critter.AddItem(a,1);
                    if(Debug){ std::cout << "*Post Critter.AddItem* \n";}
                }*/


    else
    {
        ofstream outputFile(line.c_str());

        outputFile << "[name:" << Critter.name << "]" << "[race:" << Critter.race << "]" << "[xpos:" << Critter.xpos << "]" << "[ypos:" << Critter.ypos << "]"
        << "[strength:" << Critter.Skills.strength << "]" << "[perception:" << Critter.Skills.perception << "]" << "[intelligence:" << Critter.Skills.intelligence << "]"
        << "[charisma:" << Critter.Skills.charisma << "]" << "[endurance:" << Critter.Skills.endurance << "]" << "[dexterity:" << Critter.Skills.dexterity << "]"
        << "[agility:" << Critter.Skills.agility << "]" << "[health:" << Critter.health << "]"
        << "[action:" << Critter.action << "]" << "[angle:" << Critter.angle << "]" << "[thirst:" << Critter.thirst << "]" << "[hunger:" << Critter.hunger << "]"
        << "[strengthxp:" << Critter.Skills.strengthxp << "]" << "[perceptionxp:" << Critter.Skills.perceptionxp << "]" << "[intelligencexp:" << Critter.Skills.intelligencexp << "]"
        << "[charismaxp:" << Critter.Skills.charismaxp << "]" << "[endurancexp:" << Critter.Skills.endurancexp << "]" << "[dexterityxp:" << Critter.Skills.dexterityxp << "]"
        << "[agilityxp:" << Critter.Skills.agilityxp << "]"
        << "[cbaseid:" << Critter.cbaseid << "]" << "[maxhealth:" << Critter.maxhealth << "]"
        << "{Tags:" << Critter.tags << "}" << "{Blood:" << Critter.bloodcontent << "}"
        << "{Items:";
        for(std::list<item>::iterator i = Critter.inventory.begin(); i != Critter.inventory.begin(); i++)
        {
            outputFile << "[" << (*i).name << "]";
        }
        outputFile << "}";

    }
}


class Territory
{
    public:
    sf::Vector2i WorldTile;
    sf::Vector2i GalaxyTile;
    bool ToDelete;

    Territory()
    {
        ToDelete = false;
    }

};

void CleanTerritories(std::vector<Territory> &Territories)
{
    bool Done = false;
    while(Done == false)
    {
        std::vector<Territory>::iterator Iter;
        Done = true;
        for(Iter = Territories.begin(); Iter != Territories.end(); Iter++)
        {
            if(Iter->ToDelete)
            {
                Done = false;
                Territories.erase(Iter);
                debug("Territory deleted.");
                break;
            }
        }
    }
}


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
    Faction()
    {
        PlayerControlled = false;
        Initialized = false;

        Members = 0;

        PrimaryColor = sf::Color(randz(0,255),randz(0,255),randz(0,255));
        SecondaryColor = sf::Color(randz(0,255),randz(0,255),randz(0,255));
    }


};

std::vector<Faction> UniFact;

int FactionMembers(std::string FactionName)
{
    for(int i = 0; i != UniFact.size(); i++)
    {
        if(UniFact[i].Name == FactionName)
        {
            return UniFact[i].Members;
        }
    }
}

float FactionAggression(std::string FactionName)
{
    for(int i = 0; i != UniFact.size(); i++)
    {
        if(UniFact[i].Name == FactionName)
        {
            return UniFact[i].Aggressiveness;
        }
    }
}

int FactionTerritories(std::string FactionName)
{
    for(int i = 0; i != UniFact.size(); i++)
    {
        if(UniFact[i].Name == FactionName)
        {
            return UniFact[i].Territories.size();
        }
    }
}

float FactionPower(std::string FactionName)
{
    for(int i = 0; i != UniFact.size(); i++)
    {
        if(UniFact[i].Name == FactionName)
        {
            float TenantTech = 0;

            TenantTech += UniFact[i].TechMedical;
            TenantTech += UniFact[i].TechWeaponryBlunt;
            TenantTech += UniFact[i].TechWeaponryEnergy;
            TenantTech += UniFact[i].TechWeaponryExplosive;
            TenantTech += UniFact[i].TechWeaponryMass;
            TenantTech += UniFact[i].TechWeaponrySharp;

            float TenantTechnique = TenantTech + (TenantTech*(PercentIs(UniFact[i].Creativity,35)*0.01));

            float TenantPower = TenantTechnique*(PercentIs(UniFact[i].Members, 60)*0.01);

            TenantPower = TenantPower*8;

            return TenantPower;
        }
    }
}

int FactionPopulation()
{
    int Pop = 0;
    for(int i = 0; i != UniFact.size(); i++)
    {
        Pop += UniFact[i].Members;
    }
    return Pop;
}




void InitializeFactions(int GenerateMax = 10){
    for(int i = 0; i != GenerateMax; i++)
    {
        Faction GenFact;

        GenFact.Name = GenerateName();
        GenFact.PlayerControlled = false;

        GenFact.Members = randz(1,20);

        GenFact.PrimaryColor = sf::Color(randz(0,255),randz(0,255),randz(0,255));
        GenFact.SecondaryColor = sf::Color(randz(0,255),randz(0,255),randz(0,255));

        GenFact.Aggressiveness = randz(0, 100);
        GenFact.Creativity = randz(0, 100);

        GenFact.TechAgriculture = randz(0, 100);
        GenFact.TechArchitecture = randz(0, 100);
        GenFact.TechDiplomacy = randz(0, 100);
        GenFact.TechEnergyProduction = randz(0, 100);
        GenFact.TechMedical = randz(0, 100);
        GenFact.TechRobotics = randz(0, 100);
        GenFact.TechWeaponryBlunt = randz(0, 100);
        GenFact.TechWeaponryEnergy = randz(0, 100);
        GenFact.TechWeaponryExplosive = randz(0, 100);
        GenFact.TechWeaponryMass = randz(0, 100);
        GenFact.TechWeaponrySharp = randz(0, 100);

        UniFact.push_back(GenFact);
    }

}


NPC * GetGlobalCritter( std::string strtype ){
    for(int i=0; i<npcmanager.GlobalCritter.size(); i++)
    {
        if( npcmanager.GlobalCritter[i].name == strtype )
        {
            return &npcmanager.GlobalCritter[i];
        }
    }
    return NULL;
}


void SpawnCritter(std::string Object, int xpos, int ypos){
    if(Debug){ std::cout << "Spawning" << Object << " \n";}
    int i = 0;
    NPC var;
    var = *GetGlobalCritter(Object);
    // var.ReCreate();
    var.id = Globals.globalid++;
    var.xpos = xpos;
    var.ypos = ypos;
    var.ReCreateSkills();

    npcmanager.AddedCritters.push_back(var);
    if(Debug){ std::cout << "Done Spawning. \n";}


     /*bool searching = true;
    while( searching )
    {
        if( itemmanager.GlobalItem[i].name == Object && t != itemmanager.GlobalItem.end())
        {
            doThings();
            searching=false;
        }
    i++;
    t++;
    }*/


    /*for(i = itemmanager.GlobalItem.begin(); i != itemmanager.GlobalItem.end(); i++)
    {
        if(i->name == Object)
        {
            cItem var;
            var = *i;
            var.id = globalid++;
            var.xpos = xpos;
            var.ypos = ypos;
            worlditems.push_back(var);
            break;
        }
    }*/
}

Faction PF;


void BuildStartingCritters(int ZedAmount = 0){
    std::vector<NPC>::iterator zit;
            std::vector<item>::iterator zitz;

            if(true == true)
            {
                int testnum = 0;
                if(Debug){std::cout<<"PreInt\n";}
                try
                {
                    /*for( int count = 0; count <= 2; count++)
                    {
                        sf::Vector2f vPos = Math.CircleRandz(1000,1000,180);
                        SpawnCritter("Human",vPos.x,vPos.y);
                    }*/
                    for( int count = 0; count != Squady.Squad.size(); count++)
                    {
                        sf::Vector2f vPos = Math.CircleRandz(1000,1000,180);
                        //SpawnCritter("Human",vPos.x,vPos.y);
                        Squady.Squad.at(count).xpos = vPos.x;
                        Squady.Squad.at(count).ypos = vPos.y;
                        Squady.Squad[count].Faction = PF.Name;

                        npcmanager.AddedCritters.push_back(Squady.Squad.at(count));
                    }


                    for(int zeds = 0; zeds != ZedAmount; zeds++)
                    {
                        Con("Starting Zed");
                        sf::Vector2f vPos = Math.CircleRandz(1000,1000,580);
                        SpawnCritter("Zombie",vPos.x,vPos.y);
                        Con("Ending Zed");
                    }



                }catch (std::exception& e){std::cout << "Problem during initalization of NPC Spawns. \n";}
                if(Debug){std::cout<<"PostInt\n";}
            }
}


std::string LoadCritters(sf::Vector2i WorldPos, std::string Direction, int planet = 500){

    std::string line("data/maps/Planet");
    std::stringstream convert;
    convert << planet;
    line.append(convert.str());
    convert.clear();
    galaxy_mkdir(line);
    line.append("/npc");
    convert << "x";
    convert << WorldPos.x;
    convert << "y";
    convert << WorldPos.y;
    line.append(convert.str());
    std::string ending(".crit");
    line.append(ending);

    std::ifstream Input(line.c_str());
        if(Input.is_open())
        {
            int stuffs;
            while(Input.good())
            {
                std::string line;
                getline (Input,line);
                NPC Critter;

                Critter.name = "Debuggery";


                Critter.name = StringFindString(line,"[name:");
                Critter.race = StringFindString(line,"[race:");
                if(Critter.name != "Debuggery" && Critter.name != "Zombie") Critter = *GetGlobalCritter(Critter.race);
                if(Critter.name == "Zombie") Critter = *GetGlobalCritter("Zombie");
                Critter.id = Globals.globalid++;
                Critter.name = StringFindString(line,"[name:");
                Critter.race = StringFindString(line,"[race:");
                Critter.xpos = StringFindNumber(line,"[xpos:");
                Critter.cbaseid = StringFindNumber(line,"[cbaseid:");
                Critter.ypos = StringFindNumber(line,"[ypos:");
                Critter.health = StringFindNumber(line,"[health:");
                Critter.maxhealth = StringFindNumber(line,"[maxhealth:");
                Critter.action = StringFindString(line,"[action:");
                Critter.angle = StringFindNumber(line,"[angle:");
                Critter.thirst = StringFindNumber(line,"[thirst:");
                Critter.hunger = StringFindNumber(line,"[hunger:");
                Con("Problem?");
                Critter.tags.clear();
                Critter.tags.append(StringFindChaos(line,"{Tags:","}"));
                Critter.bloodcontent.append(StringFindChaos(line,"{Blood:","}"));
                Con("Problem? 2");

 /*debug("v-Adding Tags-v");
                debug(StringFindChaos(line,"{Tags:","}"));
                debug("^-Added Tags-^");
                Critter.tags.append(StringFindChaos(line,"{Tags:","}"));
                std::set<std::string> Items;
                std::string List;
                List.append(StringFindChaos(line,"{Items:","}"));
                Items = StringFindSetChaos(List,"[","]");
                for(std::set<std::string>::iterator i = Items.begin(); i != Items.end(); i++)
                {
                    std::string OogaBooga = *i;
                    char *a=new char[OogaBooga.size()+1];a[OogaBooga.size()]=0;memcpy(a,OogaBooga.c_str(),OogaBooga.size());
                    if(Debug){ std::cout << "Pre Critter.AddItem \n";}
                    Critter.AddItem(a,1);
                    if(Debug){ std::cout << "*Post Critter.AddItem* \n";}
                }*/



                std::cout << "Xpos: " << Critter.xpos << "Ypos: " << Critter.ypos << std::endl;

                if(Direction == "TopLeft"){}
                if(Direction == "Top")
                {
                    Critter.xpos += 640;
                }
                if(Direction == "TopRight")
                {
                    Critter.xpos += 640;
                    Critter.xpos += 640;
                }
                if(Direction == "Right")
                {
                    Critter.xpos += 640;
                    Critter.xpos += 640;
                    Critter.ypos += 640;
                }
                if(Direction == "BottomRight")
                {
                    Critter.xpos += 640;
                    Critter.xpos += 640;
                    Critter.ypos += 640;
                    Critter.ypos += 640;
                }
                if(Direction == "Bottom")
                {
                    Critter.xpos += 640;
                    Critter.ypos += 640;
                    Critter.ypos += 640;
                }
                if(Direction == "BottomLeft")
                {
                    Critter.ypos += 640;
                    Critter.ypos += 640;
                }
                if(Direction == "Left")
                {
                    Critter.ypos += 640;
                }
                std::cout << "Xpos: " << Critter.xpos << "Ypos: " << Critter.ypos << std::endl;





                Critter.Skills.strength = StringFindNumber(line,"[strength:");
                Critter.Skills.perception = StringFindNumber(line,"[perception:");
                Critter.Skills.intelligence = StringFindNumber(line,"[intelligence:");
                Critter.Skills.charisma = StringFindNumber(line,"[charisma:");
                Critter.Skills.endurance = StringFindNumber(line,"[endurance:");
                Critter.Skills.dexterity = StringFindNumber(line,"[dexterity:");
                Critter.Skills.agility = StringFindNumber(line,"[agility:");

                Critter.Skills.strengthxp = StringFindNumber(line,"[strengthxp:");
                Critter.Skills.perceptionxp = StringFindNumber(line,"[perceptionxp:");
                Critter.Skills.intelligencexp = StringFindNumber(line,"[intelligencexp:");
                Critter.Skills.charismaxp = StringFindNumber(line,"[charismaxp:");
                Critter.Skills.endurancexp = StringFindNumber(line,"[endurancexp:");
                Critter.Skills.dexterityxp = StringFindNumber(line,"[dexterityxp:");
                Critter.Skills.agilityxp = StringFindNumber(line,"[agilityxp:");

                /*std::string Imagery = StringFindString(line,"[Image:");
                std::vector<cImageHolder>::iterator i;
                if(Debug){ std::cout << "Pre Imagery \n";}
                for(i = imagemanager.GlobalImage.begin(); i != imagemanager.GlobalImage.end(); i++)
                {
                    if(i->name == Imagery)
                    {
                        Critter.img.setTexture(i->Image);
                    }
                }*/

                if(Critter.name != "Debuggery"){npclist.push_back(Critter);}

            }
        }
        return line;
}

bool Boom(int xpos, int ypos, int damage, int size){
    Effectz.CreateCircle(xpos,ypos,size,sf::Color(255,0,0,150),0,sf::Color(0,0,0));
    std::vector<NPC>::iterator Me;
    for( Me = npclist.begin(); Me != npclist.end(); ++Me ){
        if( Math.Closeish(xpos,ypos,Me->xpos,Me->ypos) < size){
            Me->modhealth(-damage);
        }
    }
}


void SquadHud() // This prints that "pretty" little Squad Unit display in the top left.
{
    try
    {
        for(int i = 0; i != npclist.size(); i++)
        {
            if(npclist[i].name != "debug")
            {

                Effectz.CreateSquare(Globals.TopLeft.x+(20),Globals.TopLeft.y+(20*i),Globals.TopLeft.x+20+(20),Globals.TopLeft.y+20+(20*i),sf::Color::Black);

                std::string Output = AddString(AddString(npclist[i].name,npclist[i].action),npclist[i].target);
                cText.CreateText(Globals.TopLeft.x+20+(20),Globals.TopLeft.y+(20*i),12,sf::Color::White,Output);

            }
        }
    }catch (std::exception& e) { std::cout << "\n \n Something went wrong in SquadHud \n \n"; }
}








#endif // FACTION_H_INCLUDED
