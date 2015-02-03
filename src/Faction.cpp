#include "Faction.h"
#include "filesystemUtils.hpp"
#include "astar.h"
#include "Textures.h"
#include "math.h"
#include "util.h"
#include "globalvars.h"

#include <sstream>
#include <fstream>
#include <cstring>
#include <iostream>

Faction *conFact;
std::vector<Npc> npclist;
MakeSquad squady;
std::vector<Npc> worldCritters;
std::vector<Faction> uniFact;
Faction g_pf;
NpcManager npcmanager;
extern sf::RenderWindow window;

void Npc::BodyDefinition::BodyPartFind(std::string Part, int amount)
{

    size_t LinePosOne;
    LinePosOne = bodyParts.find(Part);
    if (LinePosOne != std::string::npos)
    {

        size_t LinePosTwo;
        std::string LineValue;
        LinePosTwo = bodyParts.find("]", LinePosOne + 1);
        if (LinePosTwo != std::string::npos)
        {
            int len = Part.length();
            LineValue.assign(bodyParts, LinePosOne + len,
                             LinePosTwo - (LinePosOne + len));
            amount += atoi(LineValue.c_str());

            std::stringstream out;
            out << amount;
            bodyParts.replace(LinePosOne + len, LinePosTwo - (LinePosOne + len),
                              out.str());

            if (out.str() == "0")
            {
                bodyParts.replace(LinePosOne, LinePosTwo, "");
            }
        }
    }
    else if (amount == 0)
    {
        //cout<<"No "<<Part<<" Here\n";
    }
    else if (amount > 0)
    {
        bodyParts.assign(Part);
        std::stringstream out;
        out << amount;
        bodyParts.append(out.str());
        bodyParts.append("]");
    }
}

std::set<int> Npc::melee(int /*min*/, int /*max*/, int range,
                         std::set<int> /*exception*/)
{
    std::set<int> Tar;
    if (firstStrike == false)
    {
        degrees =
            math::angleBetweenVectors(sf::Vector2f(xpos, ypos), targetPos) - 90;
        firstStrike = true;
    }
    for (int i = 0; i != 30; i++)
    {
        xxx = xpos + cosf(degrees * PI / 180) * range;
        yyy = ypos + sinf(degrees * PI / 180) * range;
        degrees += 1;
        effects.createLine(xpos, ypos, xxx, yyy, 2, sf::Color::Blue);
        Tar = npcTrace(xpos, ypos, xxx, yyy, id, std::set<int>());
    }
    std::list<Item>::iterator Inv;
    for (Inv = inventory.begin(); Inv != inventory.end(); ++Inv)
    {
        if (Inv->name == "Sword")
        {
        }
    }
    if (degrees >=
        math::angleBetweenVectors(sf::Vector2f(xpos, ypos), targetPos) + 90)
    {
        attacking = false;
        firstStrike = false;
    }
    return Tar;
}

void Npc::PathFinding::endMem()
{
    free(mypathBank[0]);
}

void Npc::PathFinding::myFindPath(int Sx, int Sy, int Ex, int Ey)
{
    xPa.clear();
    yPa.clear();
    debug("Pathfind status");

    try
    {
        mypathStatus = astar::findPath(1, Sx, Sy, Ex, Ey);
    }
    catch (std::exception &e)
    {
        std::cout << " Find Path done messed up\n";
        fSleep(60);
    }

    mypathLocation = astar::pathLocation[1];

    mypathLength = astar::pathLength[1];
    //MypathBank[1] = (int*) realloc (pathBank[1],pathLength[1]*8);

    for (int i = 0; i != mypathLength; ++i)
    {

        yPa.push_back(astar::pathBank[1][i * 2 - 1]);
        xPa.push_back(astar::pathBank[1][i * 2 - 2]);
    }
}

int Npc::PathFinding::myReadPathX(int /*pathfinderID*/, int xpathLocation)
{
    if (xpathLocation <= mypathLength)
    {
        //Read coordinate from bank
        //x = MypathBank[pathfinderID] [xpathLocation*2-2];
        //Like... Yo bo... Ty this instead.
        int x = xPa.at(xpathLocation);
        //Adjust the coordinates so they align with the center
        //of the path square (optional). This assumes that you are using
        //sprites that are centered -- i.e., with the midHandle command.
        //Otherwise you will want to adjust this.
        x = astar::tileSize * x + .5 * astar::tileSize;
        return x;
    }
    throw std::runtime_error(
        "MyReadPathX: Couldn't return a meaningful value!");
}

int Npc::PathFinding::myReadPathY(int /*pathfinderID*/, int ypathLocation)
{
    if (ypathLocation <= mypathLength)
    {
        //Read coordinate from bank
        //y = MypathBank[pathfinderID] [ypathLocation*2-1];
        int y = yPa.at(ypathLocation);
        //Adjust the coordinates so they align with the center
        //of the path square (optional). This assumes that you are using
        //sprites that are centered -- i.e., with the midHandle command.
        //Otherwise you will want to adjust this.
        y = astar::tileSize * y + .5 * astar::tileSize;
        return y;
    }
    throw std::runtime_error(
        "MyReadPathY: Couldn't return a meaningful value!");
}

void Npc::PathFinding::myReadPath(int /*pathfinderID*/, int currentX,
                                  int currentY, int pixelsPerFrame)
{
    //If a path has been found for the pathfinder	...
    if (mypathStatus == astar::AStarFound)
    {
        //If path finder is just starting a new path or has reached the
        //center of the current path square (and the end of the path
        //hasn't been reached), look up the next path square.
        if (mypathLocation < mypathLength)
        {
            //if just starting or if close enough to center of square
            if (mypathLocation == 0 ||
                (abs(currentX - myxPath) < pixelsPerFrame &&
                 abs(currentY - myyPath) < pixelsPerFrame))
                mypathLocation = mypathLocation + 1;
        }
        //Read the path data.
        try
        {
            const int ID = 1;
            myxPath = myReadPathX(ID, mypathLocation);
            myyPath = myReadPathY(ID, mypathLocation);
        }
        catch (std::exception const &ex)
        {
            // Log the exception and bail out of this function
            std::cerr << "NPC::PathFinding: " << ex.what();
            return;
        }

        //If the center of the last path square on the path has been
        //reached then reset.
        if (mypathLocation == mypathLength)
        {
            if (abs(currentX - myxPath) < pixelsPerFrame &&
                abs(currentY - myyPath) <
                    pixelsPerFrame) //if close enough to center of square
                mypathStatus = astar::notStarted;
        }
    }
    //If there is no path for this pathfinder, simply stay in the current
    //location.
    else
    {
        myxPath = currentX;
        myyPath = currentY;
    }
}

void Npc::effectStats()
{
    maxhealth = skills.endurance * 0.8;
    regenrate = skills.endurance / 10;
}

void Npc::Skill::Train(std::string skill, int amount, int skillgain)
{
    if (skill == "agility")
    {
        agilityxp += amount;
        if (agilityxp >= 100)
        {
            agility += skillgain;
            agilityxp = 0;
        }
    }
    if (skill == "charisma")
    {
        charismaxp += amount;
        if (charismaxp >= 100)
        {
            charisma += skillgain;
            charismaxp = 0;
        }
    }
    if (skill == "intelligence")
    {
        intelligencexp += amount;
        if (intelligencexp >= 100)
        {
            intelligence += skillgain;
            intelligencexp = 0;
        }
    }
    if (skill == "dexterity")
    {
        dexterityxp += amount;
        if (dexterityxp >= 100)
        {
            dexterity += skillgain;
            dexterityxp = 0;
        }
    }
    if (skill == "strength")
    {
        strengthxp += amount;
        if (strengthxp >= 100)
        {
            strength += skillgain;
            strengthxp = 0;
        }
    }
    if (skill == "perception")
    {
        perceptionxp += amount;
        if (perceptionxp >= 100)
        {
            perception += skillgain;
            perceptionxp = 0;
        }
    }
    if (skill == "endurance")
    {
        endurancexp += amount;
        if (endurancexp >= 100)
        {
            endurance += skillgain;
            endurancexp = 0;
        }
    }
    if (skill == "wisdom")
    {
        wisdomxp += amount;
        if (wisdomxp >= 100)
        {
            wisdom += skillgain;
            wisdomxp = 0;
        }
    }
}

Npc::Npc()
    : attacking{}, firstStrike{}, imgRotates{}, prone{}, body{}, rot{}, xxx{},
      yyy{}, degrees{}, pathFinding{}, imgstrx{}, imgstry{}, imgendx{},
      imgendy{}, isPlayer{}, hasSpawned{}, grappling{}, cbaseid{}, id2{},
      surname{}, alive{}, stillalive{}, ticksalive{}, useditem{}, canmove{},
      breathrate{}, breathtimer{}, breathtimerint{}, age{}, ageType{},
      direction{}, worshippers{}, gypos{}, gxpos{}, planet{}, ypos{}, xpos{},
      rypos{}, rxpos{}, mana{}, reginterntemp{}, interntemp{}, regtemp{},
      temp{}, breathmax{}, nutrition{}, maxstamina{}, stamina{}, credits{},
      size{}, minmeleedamage{}, maxmeleedamage{}, minrangeddamage{},
      maxrangeddamage{}, dirgrid{}, valuegrid{}, followgrid{}
{
    hasJob = false;

    toDelete = false;
    viewangle = 180;
    viewrange = 200;
    stench = 0;
    killcount = 0;
    id = gvars::globalid++;
    needsFood = true;
    needsWater = true;
    targetVectorId = -1;
    targetId = -1;
    angle = 1;
    allowedDrink = true;
    allowedFood = true;
    allowedMove = true;
    atTarget = false;
    hasTarget = false;
    target = "Food";
    action = "Act";
    moverateint = 1;
    moverate = moverateint;
    movetimerint = 1000;
    movetimer = movetimerint;
    movetimerrate = 500;
    attacktimer = 0;
    attacktimerint = 25;
    maxhunger = 600;
    hungertimerint = 10;
    hungertimer = hungertimerint;
    hungerrate = 1;
    hunger = 600;
    maxthirst = 600;
    thirsttimerint = 5;
    thirsttimer = thirsttimerint;
    thirstrate = 1;
    thirst = 600;
    reach = 10;
    skills.endurance = randz(20, 80);
    skills.strength = randz(20, 80);
    skills.dexterity = randz(20, 80);
    skills.intelligence = randz(20, 80);
    skills.wisdom = randz(20, 80);
    skills.charisma = randz(20, 80);
    skills.perception = randz(20, 80);
    skills.agility = randz(20, 80);
    skills.endurancexp = 0;
    skills.strengthxp = 0;
    skills.dexterityxp = 0;
    skills.intelligencexp = 0;
    skills.wisdomxp = 0;
    skills.charismaxp = 0;
    skills.perceptionxp = 0;
    skills.agilityxp = 0;

    targetInfo.item = nullptr;
    targetInfo.npc = nullptr;
    targetInfo.tile = nullptr;

    maxhealth = skills.endurance * 0.8;
    regentimerint = 100;
    regentimer = regentimerint;
    regenrate = skills.endurance / 10;
    health = maxhealth;
    stench = 50;
    race = "Human";
    int numz = randz(0, 1);
    if (numz == 0)
    {
        gender = "Male";
    }
    else
    {
        gender = "Female";
    }
    int numzb = randz(0, 1);
    if (numzb == 0)
    {
        gender = "Male";
    }
    else
    {
        gender = "Female";
    }
    if (gender == "Male")
    {
        int numzb = randz(0, 6);
        if (numzb == 0)
        {
            name = "John";
        }
        else if (numzb == 1)
        {
            name = "Jack";
        }
        else if (numzb == 2)
        {
            name = "Jonathan";
        }
        else if (numzb == 3)
        {
            name = "James";
        }
        else if (numzb == 4)
        {
            name = "Joseph";
        }
        else if (numzb == 5)
        {
            name = "Joe";
        }
        else if (numzb == 6)
        {
            name = "Justin";
        }
    };
    if (gender == "Female")
    {
        int numzb = randz(0, 5);
        if (numzb == 0)
        {
            name = "Jennifer";
        }
        else if (numzb == 1)
        {
            name = "Jill";
        }
        else if (numzb == 2)
        {
            name = "Jamie";
        }
        else if (numzb == 3)
        {
            name = "Jackie";
        }
        else if (numzb == 4)
        {
            name = "Joan";
        }
        else if (numzb == 5)
        {
            name = "Jessica";
        }
    };
    //img.SetImage(Images);
    isNPC = true;
    needsFood = true;
    allowedFood = true;
    needsWater = true;
    allowedDrink = true;
    needsPath = false;
}

void Npc::reCreateSkills()
{

    skills.endurance = randz(20, 80);
    skills.strength = randz(20, 80);
    skills.dexterity = randz(20, 80);
    skills.intelligence = randz(20, 80);
    skills.wisdom = randz(20, 80);
    skills.charisma = randz(20, 80);
    skills.perception = randz(20, 80);
    skills.agility = randz(20, 80);
    skills.endurancexp = 0;
    skills.strengthxp = 0;
    skills.dexterityxp = 0;
    skills.intelligencexp = 0;
    skills.wisdomxp = 0;
    skills.charismaxp = 0;
    skills.perceptionxp = 0;
    skills.agilityxp = 0;
    maxhealth = skills.endurance * 0.8;
    regentimerint = 100;
    regentimer = regentimerint;
    regenrate = skills.endurance / 10;
    health = maxhealth;
}

void Npc::blankSkills()
{

    skills.endurance = 20;
    skills.strength = 20;
    skills.dexterity = 20;
    skills.intelligence = 20;
    skills.wisdom = 20;
    skills.charisma = 20;
    skills.perception = 20;
    skills.agility = 20;
    skills.endurancexp = 0;
    skills.strengthxp = 0;
    skills.dexterityxp = 0;
    skills.intelligencexp = 0;
    skills.wisdomxp = 0;
    skills.charismaxp = 0;
    skills.perceptionxp = 0;
    skills.agilityxp = 0;
    maxhealth = skills.endurance * 0.8;
    regentimerint = 100;
    regentimer = regentimerint;
    regenrate = skills.endurance / 10;
    health = maxhealth;
}

bool Npc::hasWeapon(std::string weapon)
{
    bool Specific = false;
    bool HasSpecifiedWeapon = false;
    bool FoundWeapon = false;
    if (weapon != "")
        Specific = true;
    for (auto const &item : inventory)
    {
        if (item.isWeapon)
            FoundWeapon = true;
        if (weapon != "")
        {
            if (item.name == weapon)
                HasSpecifiedWeapon = true;
        }
    }
    if (HasSpecifiedWeapon)
    {
        return true;
    }
    if (FoundWeapon && Specific == false)
    {
        return true;
    }
    return false;
}

Item *Npc::getItemType(int type)
{
    //for(int i = 0; i < inventory.size(); i++)
    for (auto &elem : inventory)
    {
        if ((elem).type == type)
        {
            if (gvars::debug)
            {
                std::cout << "Returning inventory class \n";
            }
            return &(elem);
        }
    }
    if (gvars::debug)
    {
        std::cout << "Returning inventory nothing. \n";
    }
    return nullptr;
}

bool Npc::hasItemType(int type)
{
    //for(int i=0; i<inventory.size(); i++)
    for (auto &elem : inventory)
    {
        if ((elem).type == type)
        {
            if (gvars::debug)
            {
                std::cout << "returning inventory bool true \n";
            }
            return true;
        }
    }
    if (gvars::debug)
    {
        std::cout << "returning inventory bool false \n";
    }
    return false;
}

void Npc::drawImg()
{
    img.setPosition(xpos, ypos);
    window.draw(img);
}

void Npc::move(sf::Vector2f Tar)
{
    bool Above = false;
    bool Right = false;
    bool AtTargetx = false;
    bool AtTargety = false;
    if (Tar.x < xpos)
    {
        Right = false;
    }
    else if (Tar.x == xpos)
    {
        AtTargetx = true;
    }
    else
    {
        Right = true;
    }

    if (Tar.y < ypos)
    {
        Above = true;
    }
    else if (Tar.y == ypos)
    {
        AtTargety = true;
    }
    else
    {
        Above = false;
    }

    if (AtTargetx == true && AtTargety == true)
    {
        atTarget = true;
    }
    else
    {
        if (AtTargety == true)
        {
        }
        else if (Above == true)
        {
            int numz = math::closeishS(Tar.y, ypos);
            if (numz < moverate)
            {
                ypos += numz;
            }
            else
            {
                ypos += moverate;
            }
        }
        else
        {
            int numz = math::closeishS(Tar.y, ypos);
            if (numz < moverate)
            {
                ypos -= numz;
            }
            else
            {
                ypos -= moverate;
            }
        }

        if (AtTargetx == true)
        {
        }
        else if (Right == true)
        {
            int numz = math::closeishS(Tar.x, xpos);
            if (numz < moverate)
            {
                xpos += numz;
            }
            else
            {
                xpos += moverate;
            }
        }
        else
        {
            int numz = math::closeishS(Tar.x, xpos);
            if (numz < moverate)
            {
                xpos -= numz;
            }
            else
            {
                xpos -= moverate;
            }
        }
    }
    angle = (180 / PI) * (atan2f(xpos - Tar.x, ypos - Tar.y));
}

void Npc::dirMove(sf::Vector2f Tar)
{
    bool AtTargetx = false;
    bool AtTargety = false;
    bool CloseX = false;
    bool CloseY = false;
    if (Tar.x < xpos)
    {
    }
    else if (Tar.x == xpos)
    {
        AtTargetx = true;
    }
    else
    {
    }

    if (Tar.y < ypos)
    {
    }
    else if (Tar.y == ypos)
    {
        AtTargety = true;
    }
    else
    {
    }

    if (AtTargetx == true && AtTargety == true)
    {
        atTarget = true;
    }
    else
    {
        if (AtTargetx == false && AtTargety == false)
        {
        }
        int numz0 = math::closeishS(Tar.y, ypos);
        int numz1 = math::closeishS(Tar.x, xpos);
        if (numz1 < moverate && numz0 < moverate)
        {
            xpos = Tar.x;
            ypos = Tar.y;
            CloseX = true;
            CloseY = true;
        }
        else
        {
            atTarget = false;
        }
    }

    if (CloseY == false && CloseX == false)
    {
        int xx = 0;
        int yy = 0;
        angle =
            90 -
            (180 / PI) *
                (atan2f(
                    xpos - Tar.x,
                    ypos -
                        Tar.y)); //To be honest, I spent alot of time with trial and error to get this part to work.
        xx = cosf((angle)*PI / 180) * moverate;
        yy = sinf((angle)*PI / 180) * moverate;
        xpos -= xx;
        ypos -= yy;
    }
}

void Npc::momMove()
{
    xpos += (momentum.x / size);
    ypos += (momentum.y / size);

    momentum.x = math::clamp((momentum.x - gvars::airPressure), 0, 9999999);
    momentum.y = math::clamp((momentum.y - gvars::airPressure), 0, 9999999);
}

void Npc::moveNorth()
{
    ypos -= moverate;
}

void Npc::moveNorthEast()
{
    ypos -= moverate;
    xpos += moverate;
}

void Npc::moveEast()
{
    xpos += moverate;
}

void Npc::moveSouthEast()
{
    ypos += moverate;
    xpos += moverate;
}

void Npc::moveSouth()
{
    ypos += moverate;
}

void Npc::moveSouthWest()
{
    ypos += moverate;
    xpos -= moverate;
}

void Npc::moveWest()
{
    xpos -= moverate;
}

void Npc::moveNorthWest()
{
    ypos -= moverate;
    xpos -= moverate;
}

bool Npc::isHungry()
{
    if (allowedFood == false)
    {
        return false;
    }
    if (hunger < (maxhunger * 0.40))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Npc::isThirsty()
{
    if (allowedDrink == false)
    {
        return false;
    }
    if (thirst < (maxthirst * 0.40))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Npc::isHurt()
{
    if (health < (maxhealth * 0.60))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Npc::breath()
{
}

void Npc::sethealth(float amount)
{
    health = amount;
}

bool Npc::modhealth(float amount)
{
    health += amount;
    if (health < 1)
    {
        health = 0;
        return false;
    }
    else
    {
        return true;
    }
}

void Npc::fillhunger(float amount)
{
    hunger += amount;
    if (hunger > maxhunger)
    {
        hunger = maxhunger;
    }
}

void Npc::fillthirst(int amount)
{
    thirst += amount;
    if (thirst > maxthirst)
    {
        thirst = maxthirst;
    }
}

std::string Npc::getname()
{
    return name;
}

void Npc::spawn(int /*gposx*/, int /*gposy*/, int /*rposx*/, int /*rposy*/,
                int posx, int posy)
{
    gxpos = 100;
    gypos = 100;
    rxpos = 100;
    rypos = 100;
    xpos = posx;
    ypos = posy;
    hasSpawned = true;
}

bool Npc::boolblood(std::string ailment)
{
    using namespace std;
    size_t found;
    found = bloodcontent.find(ailment);
    if (found != string::npos)
    {
        //cout <<aliment<<" found at: " << int(found) << endl;
        return true;
    }
    else
        return false;
}

bool Npc::hasTag(std::string tag)
{
    using namespace std;
    size_t found;
    found = tags.find(tag);
    if (found != string::npos)
    {
        return true;
    }
    else
        return false;
}

void Npc::tag(std::string tag, int amount)
{
    using namespace std;
    size_t found;
    found = tags.find(tag);
    if (found != string::npos)
    {
        //cout <<tag<<" found at: " << int(found) << endl;
        size_t foundz;
        std::string funz;
        foundz = tags.find("]", found + 1);
        if (foundz != string::npos)
        {
            int len = tag.length();
            funz.assign(tags, found + len, foundz - (found + len));
            amount += atoi(funz.c_str());
            std::stringstream out;
            out << amount;
            tags.replace(found + len, foundz - (found + len), out.str());
            //cout<<"Out is:"<<out.str()<<endl;
            if (out.str() == "0")
            {
                //cout<<"out is 0, It at least it's supposed to be\n";
                tags.replace(found, foundz, "");
            }
        }
    }
    else if (amount == 0)
    {
        //cout<<"No "<<tag<<" Here\n";
    }
    else if (amount > 0)
    {
        tags.assign(tag);
        std::stringstream out;
        out << amount;
        tags.append(out.str());
        tags.append("]");
    }

    /*
        found=tags.find(Zom);if (found!=string::npos){cout << "Zombification found at: " << int(found) << endl;}else{cout<<"No Zombification Here\n";}
        }*/
}

float Npc::bloodwork(std::string aliment, float amount)
{ // Returns the final altered value of the altered 'aliment'

    size_t tStart;
    aliment = "[" + aliment +
              ":"; // Doing this so the rest of the code is much, much simplier.
    tStart = bloodcontent.find(aliment);
    float Returns = amount;

    if (tStart != std::string::npos)
    {
        //cout <<aliment<<" tStart at: " << int(tStart) << endl;
        size_t tEnd;
        std::string StrNum;
        tEnd = bloodcontent.find("]", tStart + 1);

        if (tEnd != std::string::npos)
        {
            int len = aliment.length();

            StrNum.append(bloodcontent, tStart + len, tEnd - (tStart + len));
            //std::cout << "StrNum: " << StrNum;
            amount += atoi(StrNum.c_str());
            //std::cout << ", became: " << amount << std::endl;
            Returns = amount;

            std::stringstream out;
            //Hargendy!
            out << amount;
            bloodcontent.replace(tStart + len, tEnd - (tStart + len),
                                 out.str());
            //cout<<"Out is:"<<out.str()<<endl;

            if (amount <= 0) //if(out.str() == "0")
            {
                //cout<<"out is 0, It at least it's supposed to be\n";
                bloodcontent.replace(tStart, tEnd + 1, "");
            }
        }
    }
    else if (amount == 0)
    {
        //cout<<"No "<<aliment<<" Here\n";
    }
    else // if(amount > 0)
    {
        bloodcontent.append(aliment);
        std::stringstream out;
        Returns = amount;
        out << amount;
        bloodcontent.append(out.str());
        bloodcontent.append("]");
    }
    /*

        found=bloodcontent.find(Zom);if (found!=string::npos){cout << "Zombification found at: " << int(found) << endl;}else{cout<<"No Zombification Here\n";}

        }*/
    return Returns;
}

float Npc::bloodworkXX(std::string aliment, int amount)
{ // Returns the final altered value of the altered 'aliment'

    size_t tStart;
    tStart = bloodcontent.find(aliment);
    float Returns = amount;

    if (tStart != std::string::npos)
    {
        //cout <<aliment<<" tStart at: " << int(tStart) << endl;
        size_t tEnd;
        std::string StrNum;
        tEnd = bloodcontent.find("]", tStart + 1);

        if (tEnd != std::string::npos)
        {
            int len = aliment.length();

            StrNum.append(bloodcontent, tStart + len, tEnd - (tStart + len));
            amount += atoi(StrNum.c_str());
            Returns = amount;

            std::stringstream out;
            //Hargendy!
            out << amount;
            bloodcontent.replace(tStart + len, tEnd - (tStart + len),
                                 out.str());
            //cout<<"Out is:"<<out.str()<<endl;

            if (out.str() == "0")
            {
                //cout<<"out is 0, It at least it's supposed to be\n";
                bloodcontent.replace(tStart, tEnd, "");
            }
        }
    }
    else if (amount == 0)
    {
        //cout<<"No "<<aliment<<" Here\n";
    }
    else if (amount > 0)
    {
        bloodcontent.append(aliment);
        std::stringstream out;
        Returns = amount;
        out << amount;
        bloodcontent.append(out.str());
        bloodcontent.append("]");
    }
    /*

        found=bloodcontent.find(Zom);if (found!=string::npos){cout << "Zombification found at: " << int(found) << endl;}else{cout<<"No Zombification Here\n";}

        }*/
    return Returns;
}

bool Npc::closeToTarget(int distance, sf::Vector2f Tar)
{
    int numbz = 0;
    numbz = math::closeish(xpos, ypos, Tar.x, Tar.y);
    if (numbz <= distance)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Npc::addItem(const std::string &itemname, int amount)
{ // TODO: Set this to optionally receive an item class instead, Will be useful for modded weapons and ect.
    if (gvars::debug)
    {
        std::cout << "Pre var in AddItem"
                  << " \n";
    }
    Item var = *getGlobalItem(itemname);
    if (gvars::debug)
    {
        std::cout << "Post var in AddItem"
                  << " \n";
    }
    for (int Times = 0; Times != amount; Times++)
    {
        inventory.push_back(var);
    }
}

void Npc::printConsoleInfo()
{
    using namespace std;
    cout << "Name: " << name << endl;
    cout << "cBaseId: " << cbaseid << endl;
    cout << "Id: " << id << endl;
    cout << "Xpos: " << xpos << endl;
    cout << "Ypos: " << ypos << endl;
    if (alive == true)
    {
        cout << " Alive" << endl;
    }
    if (alive == false)
    {
        cout << " Dead" << endl;
    }
    cout << "Health: " << health << endl;
    cout << "Hunger: " << hunger << endl;
    cout << "Thirst: " << thirst << endl;
    cout << "Kill Count: " << killcount << endl;
    cout << "Target is " << target << " at:" << targetPos.x << ","
         << targetPos.y << endl;
}

void Npc::printBloodContent()
{
    using namespace std;
    cout << "Name: " << name << endl;
    cout << bloodcontent << endl;
}

bool Npc::operator<(const Npc &other) const
{
    return other.id > id;
}

void timeTest()
{

    time_t timer;
    time_t timer2;
    //struct tm y2k;
    double seconds;

    //y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0; y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

    time(&timer); // get current time; same as: timer = time(NULL)

    for (unsigned int i = 0; i != 100; i++)
    {
        //int Cake = randz(5,10);
        //std::cout << "Cake is: " << Cake << std::endl;
        //std::string TextLine = "Cake is: " + std::to_string(Cake); I need to upgrade to C++ 11
        //PrintToFile("Cake.txt",AddString("Cake Is: ",NumbertoString(Cake)), true);
    }
    time(&timer2);

    seconds = difftime(timer, timer2);

    //

    clock_t Start = clock();

    for (unsigned int i = 0; i != 100; i++)
    {
        //int Cake = randz(5,10);
        //std::cout << "Cake is: " << Cake << std::endl;
        //PrintToFile("Cake.txt",AddString("Cake Is: ",NumbertoString(Cake)), true);
    }

    std::cout << "Time Difference: " << clock() - Start << std::endl;
    printf("%.f second difference", seconds);

    clock_t vInital = clock();
    std::vector<Npc> TestVector;
    for (int n = 0; n != 500; n++)
    {
        Npc npc;
        TestVector.push_back(npc);
    }
    clock_t vCreation = clock();
    int vJohns = 0;
    for (size_t n = 0; n != TestVector.size(); n++)
    {
        if (TestVector[n].name == "John")
        {
            vJohns++;
        }
        if (TestVector[n].id == 10)
        {
            //vJohns++;
        }
        if (TestVector[n].health == 10)
        {
            //vJohns++;
        }
        if (TestVector[n].id == 10)
        {
            //vJohns++;
        }
        if (TestVector[n].health == 10)
        {
            //vJohns++;
        }
        if (TestVector[n].id == 10)
        {
            //vJohns++;
        }
        if (TestVector[n].health == 10)
        {
            //vJohns++;
        }
        //else Con(TestVector[n].name);
    }
    clock_t vFinished = clock();

    clock_t sInital = clock();
    std::list<Npc> TestSet;
    for (int n = 0; n != 500; n++)
    {
        Npc npc;
        TestSet.push_back(npc);
    }
    clock_t sCreation = clock();
    int sJohns = 0;
    for (auto const &elem : TestSet)
    {
        if (elem.name == "John")
        {
            sJohns++;
        }
        if (elem.id == 10)
        {
            //sJohns++;
        }
        if (elem.health == 10)
        {
            //sJohns++;
        }
        if (elem.id == 10)
        {
            //sJohns++;
        }
        if (elem.health == 10)
        {
            //sJohns++;
        }
        if (elem.id == 10)
        {
            //sJohns++;
        }
        if (elem.health == 10)
        {
            //sJohns++;
        }
        //else Con(it->name);
    }
    clock_t sFinished = clock();

    Con(TestSet.size());
    Con(sizeof(Npc));
    std::cout << "Vector Time, Creation: " << vCreation - vInital
              << ", John Counter: " << vFinished - vCreation << ", Total("
              << vFinished - vInital << "), Johns: " << vJohns << std::endl;
    std::cout << "Set Time, Creation: " << sCreation - sInital
              << ", John Counter: " << sFinished - sCreation << ", Total("
              << sFinished - sInital << "), Johns: " << sJohns << std::endl;
}

void NpcManager::addCritters()
{
    for (auto const &c : addedCritters)
    {
        npclist.push_back(c);
    }
    addedCritters.clear();
}

void NpcManager::initializeCritters()
{

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

    std::ifstream Input(
        "data/npcs.txt"); // TODO: Have this read from an Items folder, and read from all .txt files in it, Allowing greater compability between mods.
    if (Input.is_open())
    {
        while (Input.good())
        {
            std::string line;
            getline(Input, line);
            Npc Critter;
            Critter.name = "Debuggery";

            //hungerrate = 1; // TODO: Should these be modded? Or only effected by Diseases/Bionics ect.
            //thirstrate = 1;

            Critter.name = StringFindString(line, "[Name:");
            Critter.race = Critter.name;
            if (Critter.name == "Zombie")
                Critter.race = "Zombie";
            if (gvars::debug)
            {
                std::cout << "Working on " << Critter.name << "\n";
            }
            Critter.cbaseid = StringFindNumber(line, "[BaseId:");
            Critter.id = gvars::globalid++;

            Critter.target = StringFindString(line, "[Target:");
            Critter.needsFood =
                Booleanize(StringFindNumber(line, "[NeedsFood:"));
            Critter.allowedFood =
                Booleanize(StringFindNumber(line, "[AllowedFood:"));
            Critter.needsWater =
                Booleanize(StringFindNumber(line, "[NeedsWater:"));
            Critter.allowedDrink =
                Booleanize(StringFindNumber(line, "[AllowedDrink:"));
            Critter.canmove = Booleanize(StringFindNumber(line, "[CanMove:"));
            if (gvars::debug)
            {
                std::cout << "1 \n";
            }
            Critter.maxhealth = StringFindNumber(line, "[MaxHealth:");
            Critter.maxhunger = StringFindNumber(line, "[MaxHunger:");
            Critter.maxthirst = StringFindNumber(line, "[MaxThirst:");
            Critter.regentimerint = StringFindNumber(line, "[RegenRate:");

            Critter.breathtimerint = StringFindNumber(line, "[BreathTimerInt:");
            Critter.hungertimerint = StringFindNumber(line, "[HungerTimerInt:");
            Critter.thirsttimerint = StringFindNumber(line, "[ThirstTimerInt:");

            Critter.moverateint = StringFindNumber(line, "[MoveSpeed:");
            Critter.movetimerrate = StringFindNumber(line, "[MoveRate:");
            Critter.viewangle = StringFindNumber(line, "[ViewAngle:");
            Critter.viewrange = StringFindNumber(line, "[ViewRange:");
            Critter.size = StringFindNumber(line, "[Size:");
            Critter.reach = StringFindNumber(line, "[Reach:");
            Critter.stench = StringFindNumber(line, "[Stench:");
            if (gvars::debug)
            {
                std::cout << "2 \n";
            }
            Critter.skills.endurance =
                randz(StringFindNumber(line, "[MinEnd:"),
                      StringFindNumber(line, "[MaxEnd:"));
            Critter.skills.strength = randz(StringFindNumber(line, "[MinStr:"),
                                            StringFindNumber(line, "[MaxStr:"));
            Critter.skills.dexterity =
                randz(StringFindNumber(line, "[MinDex:"),
                      StringFindNumber(line, "[MaxDex:"));
            Critter.skills.intelligence =
                randz(StringFindNumber(line, "[MinInt:"),
                      StringFindNumber(line, "[MaxInt:"));
            Critter.skills.wisdom = randz(StringFindNumber(line, "[MinWis:"),
                                          StringFindNumber(line, "[MaxWis:"));
            Critter.skills.charisma = randz(StringFindNumber(line, "[MinCha:"),
                                            StringFindNumber(line, "[MaxCha:"));
            Critter.skills.perception =
                randz(StringFindNumber(line, "[MinPer:"),
                      StringFindNumber(line, "[MaxPer:"));
            Critter.skills.agility = randz(StringFindNumber(line, "[MinAgi:"),
                                           StringFindNumber(line, "[MaxAgi:"));
            if (gvars::debug)
            {
                std::cout << "3 \n";
            }
            if (Critter.maxhealth == -1)
            {
                Critter.maxhealth = Critter.skills.endurance * 0.8;
            }
            debug("v-Adding Tags-v");
            debug(StringFindChaos(line, "{Tags:", "}"));
            debug("^-Added Tags-^");
            Critter.tags.append(StringFindChaos(line, "{Tags:", "}"));
            std::set<std::string> Items;
            std::string List;
            List.append(StringFindChaos(line, "{Items:", "}"));
            Items = StringFindSetChaos(List, "[", "]");
            for (auto OogaBooga : Items)
            {

                auto a = new char[OogaBooga.size() + 1];
                a[OogaBooga.size()] = 0;
                memcpy(a, OogaBooga.c_str(), OogaBooga.size());
                if (gvars::debug)
                {
                    std::cout << "Pre Critter.AddItem \n";
                }
                Critter.addItem(a, 1);
                if (gvars::debug)
                {
                    std::cout << "*Post Critter.AddItem* \n";
                }
            }

            /*===================================================*/

            Critter.action = "Act";
            Critter.alive = true;
            Critter.hasSpawned = true;
            Critter.movetimerint = 1000;
            Critter.moverate = Critter.moverateint;

            Critter.health = Critter.maxhealth;
            Critter.hunger = Critter.maxhunger;
            Critter.thirst = Critter.maxthirst;
            if (Critter.hasTag("[EatsFlesh:"))
            {
                Critter.hunger = 10;
                Critter.thirst = 0;
            }
            Critter.regentimer = Critter.regentimerint;
            Critter.regenrate =
                Critter.skills.endurance /
                10; // TODO: Have Skill based values update in the Train() function, So that stuff like Regen doesn't fall behind.

            Critter.breathtimer = Critter.breathtimerint;
            Critter.hungertimer = Critter.hungertimerint;
            Critter.thirsttimer = Critter.thirsttimerint;
            Critter.needsPath = false;

            // Critter Bodies
            {
                if (Critter.race == "Human" || Critter.race == "Zombie")
                {
                    Critter.body.bodyParts =
                        "{[Name:UpperTorso][BloodPumpRate:100][AirCapacity:"
                        "200][AirAbsorbtion:100][ObjectCapacity:1]["
                        "MassFlesh:15:1000]}";
                    Critter.body.bodyParts.append(
                        "{[Name:Head][Mind:true][Orafice:1][MassFlesh:5:"
                        "1000][Dependant:UpperTorso]}");
                    Critter.body.bodyParts.append(
                        "{[Name:LowerTorso][ObjectCapacity:10]["
                        "DigestionRate:125][NutritionExtraction:10]["
                        "DigestsFlesh:60][DigestsVeggy:60][DigestsWater:"
                        "100][MassFlesh:15:1000][Dependant:UpperTorso]}");

                    Critter.body.bodyParts.append("{[Name:Left "
                                                  "Leg][Walk:3][MassFlesh:"
                                                  "15:1000][Dependant:"
                                                  "LowerTorso]}");
                    Critter.body.bodyParts.append("{[Name:Right "
                                                  "Leg][Walk:3][MassFlesh:"
                                                  "15:1000][Dependant:"
                                                  "LowerTorso]}");

                    Critter.body.bodyParts.append("{[Name:Left "
                                                  "Arm][Grasp:2][MassFlesh:"
                                                  "10:1000][Dependant:"
                                                  "UpperTorso]}");
                    Critter.body.bodyParts.append("{[Name:Right "
                                                  "Arm][Grasp:2][MassFlesh:"
                                                  "10:1000][Dependant:"
                                                  "UpperTorso]}");
                }
                if (Critter.race == "Azabul")
                {
                    Critter.body.bodyParts =
                        "{[Name:UpperTorso][BloodPumpRate:100][AirCapacity:200]"
                        "["
                        "AirAbsorbtion:100][ObjectCapacity:1]["
                        "NutritionExtraction:"
                        "25][MassFlesh:15:1000]}";
                    Critter.body.bodyParts.append(
                        "\n{[Name:Head][Mind:true]["
                        "Orafice:1][MassFlesh:5:1000]["
                        "Dependant:UpperTorso]}");
                    Critter.body.bodyParts.append(
                        "\n{[Name:LowerTorso][ObjectCapacity:10][DigestionRate:"
                        "125]"
                        "[NutritionExtraction:50][PoisonFilter:Zombification:"
                        "10]["
                        "DigestsBlood:100][MassFlesh:15:1000][Dependant:"
                        "UpperTorso]"
                        "}");

                    Critter.body.bodyParts.append(
                        "\n{[Name:Left "
                        "Leg][Walk:8][MassFlesh:15:1000]["
                        "Dependant:LowerTorso]}");
                    Critter.body.bodyParts.append(
                        "\n{[Name:Right "
                        "Leg][Walk:8][MassFlesh:15:1000]["
                        "Dependant:LowerTorso]}");

                    Critter.body.bodyParts.append(
                        "\n{[Name:Left "
                        "Arm][Grasp:2][MassFlesh:10:1000]"
                        "[Dependant:UpperTorso]}");
                    Critter.body.bodyParts.append(
                        "\n{[Name:Right "
                        "Arm][Grasp:2][MassFlesh:10:1000]"
                        "[Dependant:UpperTorso]}");
                }
            }

            std::string Imagery = StringFindString(line, "[Image:");
            if (gvars::debug)
            {
                std::cout << "Pre Imagery \n";
            }
            for (auto const &image : texturemanager.textures)
            {
                if (image.name == Imagery)
                {
                    Critter.img.setTexture(image.texture);
                    //Critter.img.SetSubRect(sf::IntRect(0, 0, i->Image.GetWidth(),i->Image.GetHeight()));
                    //Critter.img.SetCenter(i->Image.GetWidth()/2,i->Image.GetHeight()/2);
                }
            }
            if (Critter.name != "Debuggery")
            {
                globalCritter.push_back(Critter);
            }
            if (gvars::debug)
            {
                std::cout << "Post Imagery \n";
            }
        }
    }
}

MakeSquad::MakeSquad()
{
    makeSquadPoints = 1000;
    squadMates = 3;
    aim = 0;
}

std::set<int> npcTrace(int xa, int ya, int xb, int yb, int id,
                       std::set<int> /*exceptions*/)
{ //.at(0) = Item/NPC(23/69) .at(1) = id

    int dx = xb - xa, dy = yb - ya, steps;
    float xIncrement, yIncrement, x = xa, y = ya;
    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);
    xIncrement = dx / (float)steps;
    yIncrement = dy / (float)steps;
    std::set<int> SetID;

    for (int k = 0; k < steps; k++)
    {

        x += xIncrement;
        y += yIncrement;
        if (key.g)
        {
            effects.createCircle(x, y, 1, sf::Color::Blue);
        }

        // Merely doing this so I can reuse the same code, but for items, Hehe.
        {
            std::vector<Npc>::iterator Me;

            for (Me = npclist.begin(); Me != npclist.end(); ++Me)
            {
                if (math::closeish(x, y, Me->xpos, Me->ypos) <= Me->size &&
                    Me->id != id)
                {

                    try
                    {
                        SetID.insert(getNpcVectorId(Me->id));
                    }
                    catch (std::exception &e)
                    {
                        std::cout << "SetError\n";
                    }

                } //23 = Item, 69 = NPC
            }
        }
    }
    if (SetID.empty())
    {
        SetID.insert(-1);
    }
    return SetID;
}

void saveNPC(int planet, sf::Vector2i Region, Npc &Critter)
{
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

    if (File.is_open())
    {
        //File << std::endl;
        File << std::endl;
        Con("Problem? 3");
        File << "[name:" << Critter.name << "]"
             << "[race:" << Critter.race << "]"
             << "[xpos:" << Critter.xpos << "]"
             << "[ypos:" << Critter.ypos << "]"
             << "[strength:" << Critter.skills.strength << "]"
             << "[perception:" << Critter.skills.perception << "]"
             << "[intelligence:" << Critter.skills.intelligence << "]"
             << "[charisma:" << Critter.skills.charisma << "]"
             << "[endurance:" << Critter.skills.endurance << "]"
             << "[dexterity:" << Critter.skills.dexterity << "]"
             << "[agility:" << Critter.skills.agility << "]"
             << "[health:" << Critter.health << "]"
             << "[action:" << Critter.action << "]"
             << "[angle:" << Critter.angle << "]"
             << "[thirst:" << Critter.thirst << "]"
             << "[hunger:" << Critter.hunger << "]"
             << "[strengthxp:" << Critter.skills.strengthxp << "]"
             << "[perceptionxp:" << Critter.skills.perceptionxp << "]"
             << "[intelligencexp:" << Critter.skills.intelligencexp << "]"
             << "[charismaxp:" << Critter.skills.charismaxp << "]"
             << "[endurancexp:" << Critter.skills.endurancexp << "]"
             << "[dexterityxp:" << Critter.skills.dexterityxp << "]"
             << "[agilityxp:" << Critter.skills.agilityxp << "]"
             << "[cbaseid:" << Critter.cbaseid << "]"
             << "[maxhealth:" << Critter.maxhealth << "]"
             << "{Tags:" << Critter.tags << "}"
             << "{Blood:" << Critter.bloodcontent << "}"
             << "{Items:";
        Con("Problem? 4");

        //for(int i = 0; i != Critter.inventory.size(); i++)
        for (auto i = Critter.inventory.begin(); i != Critter.inventory.begin();
             i++)
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

        outputFile << "[name:" << Critter.name << "]"
                   << "[race:" << Critter.race << "]"
                   << "[xpos:" << Critter.xpos << "]"
                   << "[ypos:" << Critter.ypos << "]"
                   << "[strength:" << Critter.skills.strength << "]"
                   << "[perception:" << Critter.skills.perception << "]"
                   << "[intelligence:" << Critter.skills.intelligence << "]"
                   << "[charisma:" << Critter.skills.charisma << "]"
                   << "[endurance:" << Critter.skills.endurance << "]"
                   << "[dexterity:" << Critter.skills.dexterity << "]"
                   << "[agility:" << Critter.skills.agility << "]"
                   << "[health:" << Critter.health << "]"
                   << "[action:" << Critter.action << "]"
                   << "[angle:" << Critter.angle << "]"
                   << "[thirst:" << Critter.thirst << "]"
                   << "[hunger:" << Critter.hunger << "]"
                   << "[strengthxp:" << Critter.skills.strengthxp << "]"
                   << "[perceptionxp:" << Critter.skills.perceptionxp << "]"
                   << "[intelligencexp:" << Critter.skills.intelligencexp << "]"
                   << "[charismaxp:" << Critter.skills.charismaxp << "]"
                   << "[endurancexp:" << Critter.skills.endurancexp << "]"
                   << "[dexterityxp:" << Critter.skills.dexterityxp << "]"
                   << "[agilityxp:" << Critter.skills.agilityxp << "]"
                   << "[cbaseid:" << Critter.cbaseid << "]"
                   << "[maxhealth:" << Critter.maxhealth << "]"
                   << "{Tags:" << Critter.tags << "}"
                   << "{Blood:" << Critter.bloodcontent << "}"
                   << "{Items:";
        for (auto i = Critter.inventory.begin(); i != Critter.inventory.begin();
             i++)
        {
            outputFile << "[" << (*i).name << "]";
        }
        outputFile << "}";
    }
}

Territory::Territory()
{
    toDelete = false;
}

void cleanTerritories(std::vector<Territory> &Territories)
{
    bool Done = false;
    while (Done == false)
    {
        Done = true;
        for (auto it = Territories.begin(); it != Territories.end(); ++it)
        {
            if (it->toDelete)
            {
                Done = false;
                Territories.erase(it);
                debug("Territory deleted.");
                break;
            }
        }
    }
}

Faction::Faction()
    : creativity{}, aggressiveness{}, techAgriculture{}, techArchitecture{},
      techEnergyProduction{}, techWeaponryMass{}, techWeaponryEnergy{},
      techWeaponryExplosive{}, techWeaponrySharp{}, techWeaponryBlunt{},
      techDiplomacy{}, techMedical{}, techRobotics{}
{
    playerControlled = false;
    initialized = false;

    members = 0;

    primaryColor = sf::Color(randz(0, 255), randz(0, 255), randz(0, 255));
    secondaryColor = sf::Color(randz(0, 255), randz(0, 255), randz(0, 255));
}

int factionMembers(std::string FactionName)
{
    for (size_t i = 0; i != uniFact.size(); i++)
    {
        if (uniFact[i].name == FactionName)
        {
            return uniFact[i].members;
        }
    }
    return 0;
}

float factionAggression(std::string FactionName)
{
    for (size_t i = 0; i != uniFact.size(); i++)
    {
        if (uniFact[i].name == FactionName)
        {
            return uniFact[i].aggressiveness;
        }
    }
    return 0;
}

int factionTerritories(std::string FactionName)
{
    for (size_t i = 0; i != uniFact.size(); i++)
    {
        if (uniFact[i].name == FactionName)
        {
            return uniFact[i].territories.size();
        }
    }
    return 0;
}

float factionPower(std::string FactionName)
{
    for (size_t i = 0; i != uniFact.size(); i++)
    {
        if (uniFact[i].name == FactionName)
        {
            float TenantTech = 0;

            TenantTech += uniFact[i].techMedical;
            TenantTech += uniFact[i].techWeaponryBlunt;
            TenantTech += uniFact[i].techWeaponryEnergy;
            TenantTech += uniFact[i].techWeaponryExplosive;
            TenantTech += uniFact[i].techWeaponryMass;
            TenantTech += uniFact[i].techWeaponrySharp;

            float TenantTechnique =
                TenantTech +
                (TenantTech * (PercentIs(uniFact[i].creativity, 35) * 0.01));

            float TenantPower =
                TenantTechnique * (PercentIs(uniFact[i].members, 60) * 0.01);

            TenantPower = TenantPower * 8;

            return TenantPower;
        }
    }
    return 0;
}

int factionPopulation()
{
    int Pop = 0;
    for (size_t i = 0; i != uniFact.size(); i++)
    {
        Pop += uniFact[i].members;
    }
    return Pop;
}

void initializeFactions(int GenerateMax)
{
    for (int i = 0; i != GenerateMax; i++)
    {
        Faction GenFact;

        GenFact.name = GenerateName();
        GenFact.playerControlled = false;

        GenFact.members = randz(1, 20);

        GenFact.primaryColor =
            sf::Color(randz(0, 255), randz(0, 255), randz(0, 255));
        GenFact.secondaryColor =
            sf::Color(randz(0, 255), randz(0, 255), randz(0, 255));

        GenFact.aggressiveness = randz(0, 100);
        GenFact.creativity = randz(0, 100);

        GenFact.techAgriculture = randz(0, 100);
        GenFact.techArchitecture = randz(0, 100);
        GenFact.techDiplomacy = randz(0, 100);
        GenFact.techEnergyProduction = randz(0, 100);
        GenFact.techMedical = randz(0, 100);
        GenFact.techRobotics = randz(0, 100);
        GenFact.techWeaponryBlunt = randz(0, 100);
        GenFact.techWeaponryEnergy = randz(0, 100);
        GenFact.techWeaponryExplosive = randz(0, 100);
        GenFact.techWeaponryMass = randz(0, 100);
        GenFact.techWeaponrySharp = randz(0, 100);

        uniFact.push_back(GenFact);
    }
}

Npc *getGlobalCritter(std::string strtype)
{
    for (auto &elem : npcmanager.globalCritter)
    {
        if (elem.name == strtype)
        {
            return &elem;
        }
    }
    return nullptr;
}

void spawnCritter(std::string Object, int xpos, int ypos)
{
    if (gvars::debug)
    {
        std::cout << "Spawning" << Object << " \n";
    }
    Npc var;
    var = *getGlobalCritter(Object);
    // var.ReCreate();
    var.id = gvars::globalid++;
    var.xpos = xpos;
    var.ypos = ypos;
    var.reCreateSkills();

    npcmanager.addedCritters.push_back(var);
    if (gvars::debug)
    {
        std::cout << "Done Spawning. \n";
    }

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

void buildStartingCritters(int ZedAmount)
{
    {
        if (gvars::debug)
        {
            std::cout << "PreInt\n";
        }
        try
        {
            /*for( int count = 0; count <= 2; count++)
                    {
                        sf::Vector2f vPos = Math::CircleRandz(1000,1000,180);
                        SpawnCritter("Human",vPos.x,vPos.y);
                    }*/
            for (size_t count = 0; count != squady.squad.size(); count++)
            {
                sf::Vector2f vPos = math::circleRandz(1000, 1000, 180);
                //SpawnCritter("Human",vPos.x,vPos.y);
                squady.squad.at(count).xpos = vPos.x;
                squady.squad.at(count).ypos = vPos.y;
                squady.squad[count].Faction = g_pf.name;

                npcmanager.addedCritters.push_back(squady.squad.at(count));
            }

            for (int zeds = 0; zeds != ZedAmount; zeds++)
            {
                Con("Starting Zed");
                sf::Vector2f vPos = math::circleRandz(1000, 1000, 580);
                spawnCritter("Zombie", vPos.x, vPos.y);
                Con("Ending Zed");
            }
        }
        catch (std::exception &e)
        {
            std::cout << "Problem during initalization of NPC Spawns. \n";
        }
        if (gvars::debug)
        {
            std::cout << "PostInt\n";
        }
    }
}

std::string loadCritters(sf::Vector2i WorldPos, std::string Direction,
                         int planet)
{

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
    if (Input.is_open())
    {
        while (Input.good())
        {
            std::string line;
            getline(Input, line);
            Npc Critter;

            Critter.name = "Debuggery";

            Critter.name = StringFindString(line, "[name:");
            Critter.race = StringFindString(line, "[race:");
            if (Critter.name != "Debuggery" && Critter.name != "Zombie")
                Critter = *getGlobalCritter(Critter.race);
            if (Critter.name == "Zombie")
                Critter = *getGlobalCritter("Zombie");
            Critter.id = gvars::globalid++;
            Critter.name = StringFindString(line, "[name:");
            Critter.race = StringFindString(line, "[race:");
            Critter.xpos = StringFindNumber(line, "[xpos:");
            Critter.cbaseid = StringFindNumber(line, "[cbaseid:");
            Critter.ypos = StringFindNumber(line, "[ypos:");
            Critter.health = StringFindNumber(line, "[health:");
            Critter.maxhealth = StringFindNumber(line, "[maxhealth:");
            Critter.action = StringFindString(line, "[action:");
            Critter.angle = StringFindNumber(line, "[angle:");
            Critter.thirst = StringFindNumber(line, "[thirst:");
            Critter.hunger = StringFindNumber(line, "[hunger:");
            Con("Problem?");
            Critter.tags.clear();
            Critter.tags.append(StringFindChaos(line, "{Tags:", "}"));
            Critter.bloodcontent.append(StringFindChaos(line, "{Blood:", "}"));
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

            std::cout << "Xpos: " << Critter.xpos << "Ypos: " << Critter.ypos
                      << std::endl;

            if (Direction == "TopLeft")
            {
            }
            if (Direction == "Top")
            {
                Critter.xpos += 640;
            }
            if (Direction == "TopRight")
            {
                Critter.xpos += 640;
                Critter.xpos += 640;
            }
            if (Direction == "Right")
            {
                Critter.xpos += 640;
                Critter.xpos += 640;
                Critter.ypos += 640;
            }
            if (Direction == "BottomRight")
            {
                Critter.xpos += 640;
                Critter.xpos += 640;
                Critter.ypos += 640;
                Critter.ypos += 640;
            }
            if (Direction == "Bottom")
            {
                Critter.xpos += 640;
                Critter.ypos += 640;
                Critter.ypos += 640;
            }
            if (Direction == "BottomLeft")
            {
                Critter.ypos += 640;
                Critter.ypos += 640;
            }
            if (Direction == "Left")
            {
                Critter.ypos += 640;
            }
            std::cout << "Xpos: " << Critter.xpos << "Ypos: " << Critter.ypos
                      << std::endl;

            Critter.skills.strength = StringFindNumber(line, "[strength:");
            Critter.skills.perception = StringFindNumber(line, "[perception:");
            Critter.skills.intelligence =
                StringFindNumber(line, "[intelligence:");
            Critter.skills.charisma = StringFindNumber(line, "[charisma:");
            Critter.skills.endurance = StringFindNumber(line, "[endurance:");
            Critter.skills.dexterity = StringFindNumber(line, "[dexterity:");
            Critter.skills.agility = StringFindNumber(line, "[agility:");

            Critter.skills.strengthxp = StringFindNumber(line, "[strengthxp:");
            Critter.skills.perceptionxp =
                StringFindNumber(line, "[perceptionxp:");
            Critter.skills.intelligencexp =
                StringFindNumber(line, "[intelligencexp:");
            Critter.skills.charismaxp = StringFindNumber(line, "[charismaxp:");
            Critter.skills.endurancexp =
                StringFindNumber(line, "[endurancexp:");
            Critter.skills.dexterityxp =
                StringFindNumber(line, "[dexterityxp:");
            Critter.skills.agilityxp = StringFindNumber(line, "[agilityxp:");

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

            if (Critter.name != "Debuggery")
            {
                npclist.push_back(Critter);
            }
        }
    }
    return line;
}

void boom(int xpos, int ypos, int damage, int size)
{
    effects.createCircle(xpos, ypos, size, sf::Color(255, 0, 0, 150), 0,
                         sf::Color(0, 0, 0));
    std::vector<Npc>::iterator Me;
    for (Me = npclist.begin(); Me != npclist.end(); ++Me)
    {
        if (math::closeish(xpos, ypos, Me->xpos, Me->ypos) < size)
        {
            Me->modhealth(-damage);
        }
    }
}

void
squadHud() // This prints that "pretty" little Squad Unit display in the top left.
{
    try
    {
        for (size_t i = 0; i != npclist.size(); i++)
        {
            if (npclist[i].name != "debug")
            {

                effects.createSquare(
                    gvars::topLeft.x + (20), gvars::topLeft.y + (20 * i),
                    gvars::topLeft.x + 20 + (20),
                    gvars::topLeft.y + 20 + (20 * i), sf::Color::Black);

                std::string Output =
                    npclist[i].name + npclist[i].action + npclist[i].target;
                cText.CreateText(gvars::topLeft.x + 20 + (20),
                                 gvars::topLeft.y + (20 * i), 12,
                                 sf::Color::White, Output);
            }
        }
    }
    catch (std::exception &e)
    {
        std::cout << "\n \n Something went wrong in SquadHud \n \n";
    }
}

void setTestage()
{
    std::set<Npc> TestSet;
    for (int n = 0; n != 500; n++)
    {
        Npc npc;
        TestSet.insert(npc);
    }
    int sJohns = 0;
    for (const auto &npc : TestSet)
    {
        if (npc.name == "John")
        {
            sJohns++;
        }
        if (npc.id == 10)
        {
            //sJohns++;
        }
        if (npc.health == 10)
        {
            //sJohns++;
        }
        if (npc.id == 10)
        {
            //sJohns++;
        }
        if (npc.health == 10)
        {
            //sJohns++;
        }
        if (npc.id == 10)
        {
            //sJohns++;
        }
        if (npc.health == 10)
        {
            //sJohns++;
        }
        //else Con(it->name);
    }
}
