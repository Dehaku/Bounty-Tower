#include "Items.h"
#include "filesystemUtils.hpp"
#include "globalvars.h"
#include "Textures.h"
#include "math.h"
#include "util.h"
#include "Bullet.h"

#include <fstream>
#include <sstream>
#include <iostream>

extern sf::RenderWindow window;

DamageTypes damageTypes;
int DamageTypes::getNum(std::string name)
{
    for(int i = 0; i != TypeStrings.size(); i++)
    {
        if(TypeStrings[i] == name)
            return i;
    }
    return 0;
}
std::string DamageTypes::getString(int num)
{
    for(int i = 0; i != TypeStrings.size(); i++)
    {
        if(i == num)
            return TypeStrings[i];
    }
    return "None";
}

DamageTypes::DamageTypes()
{ // WARNING, THESE MUST BE IN THE SAME ORDER AS 'enum Type'
    std::string entry;
    entry = "None";
    TypeStrings.push_back(entry);
    entry = "Blunt";
    TypeStrings.push_back(entry);
    entry = "Pierce";
    TypeStrings.push_back(entry);
    entry = "Slash";
    TypeStrings.push_back(entry);
    entry = "Fire";
    TypeStrings.push_back(entry);
    entry = "Cold";
    TypeStrings.push_back(entry);
    entry = "Poison";
    TypeStrings.push_back(entry);
    entry = "Electric";
    TypeStrings.push_back(entry);
    entry = "Acid";
    TypeStrings.push_back(entry);
    entry = "Energy";
    TypeStrings.push_back(entry);
}

ItemTypes::ItemTypes()
{
    list.push_back(StringFloat("None",0));
    list.push_back(StringFloat("Melee",1));
    list.push_back(StringFloat("Gun",2));
    list.push_back(StringFloat("Bullet",3));
    list.push_back(StringFloat("Shell",4));
    list.push_back(StringFloat("Missile",5));
    list.push_back(StringFloat("Pickups",12));
    list.push_back(StringFloat("Magic",23));
    list.push_back(StringFloat("Consumable",42));
    list.push_back(StringFloat("InventorySlotUnlocker",69));
    list.push_back(StringFloat("PartBody",100));
    list.push_back(StringFloat("PartBarrel",101));
    list.push_back(StringFloat("PartMagazine",102));
    list.push_back(StringFloat("PartFiringMechanism",103));
    list.push_back(StringFloat("PartGrip",104));
    list.push_back(StringFloat("PartSight",105));
    list.push_back(StringFloat("PartStock",106));
    list.push_back(StringFloat("PartAccessory",107));

    for(auto checker : list)
    {
        for(auto comparison : list)
        {
            if(checker.num == comparison.num)
                if(checker.str != comparison.str)
                {
                    std::cout << "\n\n ITEM TYPE ID CONFLICT:" << checker.str << "/" << comparison.str << ":" << checker.num << "/" << comparison.num << "\n\n";
                    fSleep(10);
                }
        }
        for(auto comparison : list)
        {
            if(checker.str == comparison.str)
                if(checker.num != comparison.num)
                {
                    std::cout << "\n\n ITEM TYPE NAME CONFLICT:" << checker.str << "/" << comparison.str << ":" << checker.num << "/" << comparison.num << "\n\n";
                    fSleep(10);
                }
        }
    }
}

StringFloat ItemTypes::getTypeID(int id)
{
    for(auto checker : list)
        if(checker.num == id)
            return checker;
    throw std::runtime_error("Couldn't find Item Type Entry ID: " + id);
}

StringFloat ItemTypes::getTypeID(std::string name)
{
    for(auto checker : list)
        if(checker.str == name)
            return checker;
    throw std::runtime_error("Couldn't find Item Type Entry Name: " + name);
}


ItemTypes itemTypes;

void clearBullets()
{
    bullets.clear();
}


Item * mouseItem = nullptr;

std::list<Item> worlditems;
ItemManager itemmanager;

void removeItems(std::list<Item> &items)
{
    bool done = false;
    while (done == false)
    {
        done = true;
        for (auto it = items.begin(); it != items.end(); ++it)
        {
            if (it->toDelete)
            {
                done = false;
                items.erase(it);
                break;
            }
        }
    }
}

void zSaveItem(int /*planet*/, sf::Vector2i region, Item &object)
{

    using namespace std;
    ofstream file; // Start to Load Map
    string newline("data/maps/Planet");
    stringstream newconvert;
    newline.append("500");
    newline.append("/stuff");
    newconvert << "x";
    newconvert << region.x;
    newconvert << "y";
    newconvert << region.y;
    newline.append(newconvert.str());
    string newending(".item");
    newline.append(newending);
    con(newline);

    file.open(newline.c_str(), fstream::in | fstream::ate);
    debug("looking for file...");

    if (file.is_open())
    {
        file << std::endl;
        file << "[name:" << object.name << "]"
             << "[xpos:" << object.xpos << "]"
             << "[ypos:" << object.ypos << "]";
        con("Added", false);
        con(object.name);
        file.close();
    }
    else
    {
        string line("data/maps/Planet500");
        stringstream convert;
        //convert << planet;
        line.append(convert.str());
        convert.clear();
        galaxy_mkdir(line);
        line.append("/stuff");
        convert << "x";
        convert << region.x;
        convert << "y";
        convert << region.y;
        line.append(convert.str());
        string ending(".item");
        line.append(ending);
        con(line);

        ofstream outputFile(line.c_str());

        outputFile << "[" << object.name << "," << object.xpos << ","
                   << object.ypos << "]";
    }
}

void saveItem(int planet, sf::Vector2i region, Item &critter)
{
    // I already did all the work once, Imma be lazy for some time.
    using namespace std;
    string line("data/maps/Planet");
    stringstream convert;
    convert << planet;
    line.append(convert.str());
    convert.clear();
    galaxy_mkdir(line);
    line.append("/stuff");
    convert << "x";
    convert << region.x;
    convert << "y";
    convert << region.y;
    line.append(convert.str());
    string ending(".item");
    line.append(ending);

    ofstream file;
    file.open(line.c_str(), fstream::in | fstream::ate);
    debug("looking for file...");

    if (file.is_open())
    {
        file << std::endl;
        file << "[name:" << critter.name << "]"
             << "[xpos:" << critter.xpos << "]"
             << "[ypos:" << critter.ypos << "]";
        file.close();
    }
    else
    {
        ofstream outputFile(line.c_str());

        outputFile << "[name:" << critter.name << "]"
                   << "[xpos:" << critter.xpos << "]"
                   << "[ypos:" << critter.ypos << "]";
    }
}

std::string loadItems(sf::Vector2i worldPos, std::string direction, int planet)
{

    std::string line("data/maps/Planet");
    std::stringstream convert;
    convert << planet;
    line.append(convert.str());
    convert.clear();
    galaxy_mkdir(line);
    line.append("/stuff");
    convert << "x";
    convert << worldPos.x;
    convert << "y";
    convert << worldPos.y;
    line.append(convert.str());
    std::string ending(".item");
    line.append(ending);

    std::ifstream input(line.c_str());
    if (input.is_open())
    {
        while (input.good())
        {
            std::string line;
            getline(input, line);
            Item critter;

            critter.name = "Debuggery";

            critter.name = stringFindString(line, "[name:");
            if (critter.name != "Debuggery")
                critter = *getGlobalItem(critter.name);
            critter.xpos = stringFindNumber(line, "[xpos:");
            critter.ypos = stringFindNumber(line, "[ypos:");
            std::cout << "Xpos: " << critter.xpos << "Ypos: " << critter.ypos
                      << std::endl;

            if (direction == "TopLeft")
            {
            }
            if (direction == "Top")
            {
                critter.xpos += 640;
            }
            if (direction == "TopRight")
            {
                critter.xpos += 640;
                critter.xpos += 640;
            }
            if (direction == "Right")
            {
                critter.xpos += 640;
                critter.xpos += 640;
                critter.ypos += 640;
            }
            if (direction == "BottomRight")
            {
                critter.xpos += 640;
                critter.xpos += 640;
                critter.ypos += 640;
                critter.ypos += 640;
            }
            if (direction == "Bottom")
            {
                critter.xpos += 640;
                critter.ypos += 640;
                critter.ypos += 640;
            }
            if (direction == "BottomLeft")
            {
                critter.ypos += 640;
                critter.ypos += 640;
            }
            if (direction == "Left")
            {
                critter.ypos += 640;
            }
            std::cout << "Xpos: " << critter.xpos << "Ypos: " << critter.ypos
                      << std::endl;

            if (critter.name != "Debuggery")
            {
                worlditems.push_back(critter);
            }
        }
    }
    return line;
}


void unloadAmmo(Item * rangewep, std::list<Item> * npcInv)
{
    if(rangewep == nullptr)
        return;
    if(npcInv == nullptr)
        return;
    for(auto &item : rangewep->internalitems)
    {
        if(item.type == rangewep->ammotype)
        {
            item.currentSlot = nullptr;
            item.slotted = false;
            item.id = gvars::globalid++;

            rangewep->user->addItem(item);
            //npcInv->push_back(item);
            item.toDelete = true;
        }
    }
    AnyDeletes(rangewep->internalitems);
}

void cycleFireMode(Item * rangewep)
{
    if(rangewep == nullptr)
        return;
    bool canSemi = rangewep->getFireSemi();
    bool canBurst = rangewep->getFireBurst();
    bool canAuto = rangewep->getFireAuto();

    std::cout << "Cycle: " << canSemi << "/" << canBurst << "/" << canAuto << std::endl;

    if(rangewep->fireMode == 0)
    {
        if(canBurst)
            rangewep->fireMode = 1;
        else if(canAuto)
            rangewep->fireMode = 2;
    }

    else if(rangewep->fireMode == 1)
    {
        if(canAuto)
            rangewep->fireMode = 2;
        else if(canSemi)
            rangewep->fireMode = 0;
    }

    else if(rangewep->fireMode == 2)
    {
        if(canSemi)
            rangewep->fireMode = 0;
        else if(canBurst)
            rangewep->fireMode = 1;
    }

    std::string modeText;
    if(rangewep->fireMode == 0)
        modeText = "Semi";
    if(rangewep->fireMode == 1)
        modeText = "Burst";
    if(rangewep->fireMode == 2)
        modeText = "Auto";

    chatBox.addChat(rangewep->name + " set to " + modeText + ".");
}


void spawnItem(std::string object, int xpos, int ypos, int zpos)
{
    if (gvars::debug)
    {
        std::cout << "Spawning" << object << " \n";
    }
    Item var;
    if (gvars::debug)
    {
        std::cout << "Pre var in SpawnItem"
                  << " \n";
    }
    var = *getGlobalItem(object);
    if (gvars::debug)
    {
        std::cout << "Post var in SpawnItem"
                  << " \n";
    }

    var.id = gvars::globalid++;
    var.xpos = xpos;
    var.ypos = ypos;
    var.zpos = zpos;
    itemmanager.addedItems.push_back(var);
    if (gvars::debug)
    {
        std::cout << "Done Spawning. \n";
    }
}

void refreshImages()
{
    // PANIC
    for (size_t i = 0; i != worlditems.size(); i++)
    {
        for (size_t t = 0; t != itemmanager.globalItems.size(); t++)
        {
            //if(worlditems.at(i).name == itemmanager.GlobalItem.at(t).name)
            {
                //worlditems.at(i).img.setTexture(*itemmanager.GlobalItem.at(t).img.getTexture());
            }
        }
    }
    con("Done");
}

Item *getItemPtrFromVector(std::list<Item> &vector, std::string name)
{
    debug("Doing GetItmPtr");
    //*for(int i = 0; i != Vector.size(); i++)
    for (auto &elem : vector)
    {
        if ((elem).name == name)
        {
            debug("Returning GetItmPtr");
            return &(elem);
        }
    }

    debug("GetItmPtr didn't find anything, returning NULL");
    return nullptr;
}

Item *getItemPtrfromVector2(std::list<Item> &vector, std::string name)
{
    debug("Doing GetItmPtr");
    //for(int i = 0; i != Vector.size(); i++)
    for (auto &elem : vector)
    {
        if ((elem).name == name)
        {
            debug("Returning GetItmPtr");
            return &(elem);
        }
    }

    debug("GetItmPtr didn't find anything, returning NULL");
    return nullptr;
}

Item *getItemPtrfromVectorVarSearch(std::list<Item> &vector,
                                    std::string varSearch, float atLeast)
{
    debug("Doing GetItmPtrVarSearch");
    //for(int i = 0; i != Vector.size(); i++)
    for (auto &elem : vector)
    {

        if ((elem).massFlesh >= atLeast && varSearch == "MassFlesh")
        {
            debug("Returning GetItmPtrVarSearch");
            return &(elem);
        }
        if ((elem).massVeggy >= atLeast && varSearch == "MassVeggy")
        {
            debug("Returning GetItmPtrVarSearch");
            return &(elem);
        }
        if ((elem).massWater >= atLeast && varSearch == "MassWater")
        {
            debug("Returning GetItmPtrVarSearch");
            return &(elem);
        }
        if ((elem).massMetal >= atLeast && varSearch == "MassMetal")
        {
            debug("Returning GetItmPtrVarSearch");
            return &(elem);
        }
        if ((elem).massPlastic >= atLeast && varSearch == "MassPlastic")
        {
            debug("Returning GetItmPtrVarSearch");
            return &(elem);
        }
    }

    debug("GetItmPtrVarSearch didn't find anything, returning NULL");
    return nullptr;
}

Item *findClosestItemPtr(int orix, int oriy, std::string tarItem, int /*Gxpos*/,
                         int /*Gypos*/, int /*Rxpos*/, int /*Rypos*/)
{
    //std::vector<item>::iterator Items;
    // This will be difficult, go through all the items,
    // and find the closest one, Silly old self, Watch how it's done.

    // Make sure the default starting number is
    // far beyond being the closest one.
    int closx = -1000000;
    int closy = -1000000;
    bool first = true;

    Item *returns = nullptr;

    for (auto &worlditem : worlditems)
    {
        if (first == true)
        {
            if ((worlditem).name == tarItem)
            {
                closx = (worlditem).xpos;
                closy = (worlditem).ypos;
                first = false;
                returns = &(worlditem);
            }
        }
        else
        {
            if ((worlditem).name == tarItem)
            {
                int one = math::closeish(orix, oriy, (worlditem).xpos,
                                         (worlditem).ypos);
                int two = math::closeish(orix, oriy, closx, closy);
                if (one < two)
                {
                    closx = (worlditem).xpos;
                    closy = (worlditem).ypos;
                    returns = &(worlditem);
                }
            }
        }
    }
    if (first != true)
    {
        return returns; // Returns = (xpos,ypos,id,Vector Position)
    }
    else
        return nullptr;
}


void Item::printInfo()
{

    using namespace std;
    cout << "Name: " << name.c_str() << " (" << id << ") " << endl;
    cout << "Weight: " << weight << endl;
    cout << "Value: " << value << endl;
    cout << "Min Damage: " << mindam << endl;
    cout << "Max Damage: " << maxdam << endl;
}

void Item::drawImg()
{
    img.setPosition(xpos, ypos);
    //img.setScale(globals::Scalex,globals::Scaley);
    window.draw(img);
}


void Item::printConsoleInfo()
{
    using namespace std;
    cout << "Name: " << name << endl;
    cout << "cBaseId: " << cbaseid << endl;
    cout << "Id: " << id << endl;
    cout << "Foodv:" << hungervalue << endl;
    cout << "Thirstv:" << thirstvalue << endl;
    cout << "Xpos: " << xpos << endl;
    cout << "Ypos: " << ypos << endl;
}

int Item::getRange()
{
    debug(name + " getRange()");
    int returns = range;
    if(type == 2)
    {
        Item * itemptr = getItemType(internalitems,ammotype);
        if(itemptr == nullptr)
            returns += 0;
        else
            returns += itemptr->range;
    }
    debug(name + " getRange() done");
    return returns;
}

int Item::getSpread()
{
    debug(name + " getSpread()");
    int returns = spread;
    if(type == 2)
    {
        Item * itemptr = getItemType(internalitems,ammotype);
        if(itemptr == nullptr)
            returns += 0;
        else
            returns += itemptr->spread;
    }
    debug(name + " getSpread() done");



    if(user != nullptr)
    {
        returns -= user->getPer();
    }


    if(returns < 0)
        returns = 0;

    return returns;
}

int Item::getKnockback()
{
    debug(name + " getKnockback()");
    int returns = knockback;
    if(type == 2)
    {
        Item * itemptr = getItemType(internalitems,ammotype);
        if(itemptr == nullptr)
            returns += 0;
        else
            returns += itemptr->knockback;
    }
    debug(name + " getKnockback() done");
    return returns;
}

int Item::getMinDamage()
{
    debug(name + " getMinDamage()");
    int returns = mindam;
    if(type == 2)
    {
        Item * itemptr = getItemType(internalitems,ammotype);
        if(itemptr == nullptr)
            returns += 0;
        else
            returns += itemptr->mindam;
    }
    debug(name + " getMinDamage() done");
    return returns;
}

int Item::getMaxDamage()
{
    debug(name + " getMaxDamage()");
    int returns = maxdam;
    if(type == 2)
    {
        Item * itemptr = getItemType(internalitems,ammotype);
        if(itemptr == nullptr)
            returns += 0;
        else
            returns += itemptr->maxdam;
    }
    debug(name + " getMaxDamage() done");
    return returns;
}

Vec3f Item::getPos()
{
    Vec3f myPos(xpos,ypos,zpos);
    return myPos;
}

Item::Item()
    : cbaseid{}, range{}, xpos{}, ypos{}, zpos{30*20}, rxpos{}, rypos{}, gxpos{},
      gypos{}, imgstrx{}, imgstry{}, imgendx{}, imgendy{}, isWeapon{},
    prodrate{}, prodratetimer{}
{
    firstPickup = true;
    toDelete = false;
    targetPos = sf::Vector2f(-1, -1);
    id = gvars::globalid++;
    slotted = false;
    name = "";
    weight = 0;
    value = 0;
    type = 0;
    age = 0;
    amount = 1;
    speed = 0;

    rarity = 0;

    // *---Gun Variables
    projectiles = 0;
    spread = 0;

    barrelCount = 0;
    damageMultiplier = 0;

    dispersion = 0;
    aimTime = 0;
    aimTimeCounter = 0;
    recoil = 0;
    recoilReduction = 0;

    fireDelay = 0;
    fireDelayCounter = 0;

    fireMode = 0;

    canFireSemi = false;
    canFireBurst = false;
    canFireAuto = false;

    burstCount = 0;
    burstCounter = 0;

    reloadTime = 0;
    reloadAmount = 0;
    ammoCapacity = 0;

    bulletSpeedMultiplier = 0;

    durability = 1;
    durabilityCost = 0;
    durabilityCounter = 1;


    // *---Gun Variables


    radius = 0;
    penetration = 0;
    richochets = 0;
    knockback = 0;
    damageType = 0;

    healAmount = 0;

    // This may be a little confusing, This is so the item can stay
    // inside without being ejected because one function doesn't need it.
    hasInternalUse = 0;

    user = nullptr;

    hungervalue = 0;
    thirstvalue = 0;
    massGlass = 0;
    massFlesh = 0;
    massMetal = 0;
    massOil = 0;
    massPlastic = 0;
    massVeggy = 0;
    massWater = 0;


    pickupable = true;


    stackSize = 1;
    //stacks = false;

    // 0 = none/melee, 6 = small bullet, 7 = large bullet
    ammotype = 0;
    mindam = 1;
    maxdam = 2;

    stackSize = 1;
    produces = false;
}

int Item::getBarrelCount()
{
    int returns = barrelCount;
    for(auto internals : internalitems)
        returns += internals.barrelCount;

    return returns;
}

float Item::getDamageMultiplier()
{
    float returns = damageMultiplier;
    for(auto internals : internalitems)
        returns += internals.damageMultiplier;

    return returns;
}

float Item::getDispersion()
{
    float returns = dispersion;
    for(auto internals : internalitems)
        returns += internals.dispersion;

    return returns;
}

float Item::getAimTime()
{
    float returns = aimTime;
    for(auto internals : internalitems)
        returns += internals.aimTime;

    return returns;
}

float Item::getRecoil()
{
    float returns = recoil;
    for(auto internals : internalitems)
        returns += internals.recoil;

    return returns;
}

float Item::getRecoilReduction()
{
    float returns = recoilReduction;
    for(auto internals : internalitems)
        returns += internals.recoilReduction;

    return returns;
}

float Item::getFireDelay()
{
    float returns = fireDelay;
    for(auto internals : internalitems)
        returns += internals.fireDelay;

    return returns;
}

bool Item::getFireSemi()
{
    int returns = canFireSemi;
    for(auto internals : internalitems)
        returns += internals.canFireSemi;
    if(returns > 0)
        return true;
    return false;
}

bool Item::getFireBurst()
{
    int returns = canFireBurst;
    for(auto internals : internalitems)
        returns += internals.canFireBurst;
    if(returns > 0)
        return true;
    return false;
}

bool Item::getFireAuto()
{
    int returns = canFireAuto;
    for(auto internals : internalitems)
        returns += internals.canFireAuto;
    if(returns > 0)
        return true;
    return false;
}

int Item::getBurstCount()
{
    int returns = burstCount;
    for(auto internals : internalitems)
        returns += internals.burstCount;

    return returns;
}

float Item::getReloadTime()
{
    float returns = reloadTime;
    for(auto internals : internalitems)
        returns += internals.reloadTime;

    return returns;
}

int Item::getReloadAmount()
{
    int returns = reloadAmount;
    for(auto internals : internalitems)
        returns += internals.reloadAmount;

    return returns;
}

int Item::getAmmoCapacity()
{
    int returns = ammoCapacity;
    for(auto internals : internalitems)
        returns += internals.ammoCapacity;

    return returns;
}

float Item::getBulletSpeedMultiplier()
{
    float returns = bulletSpeedMultiplier;
    for(auto internals : internalitems)
        returns += internals.bulletSpeedMultiplier;

    return returns;
}

float Item::getDurability()
{
    float returns = durability;
    for(auto internals : internalitems)
        returns += internals.durability;

    return returns;
}

float Item::getDurabilityCost()
{
    float returns = durabilityCost;
    for(auto internals : internalitems)
        returns += internals.durabilityCost;

    return returns;
}

float Item::getSpeed()
{
    float returns = speed;
    for(auto internals : internalitems)
        returns += internals.speed;

    return returns;
}


void ItemManager::addItems()
{
    for (auto const &item : addedItems)
    {
        worlditems.push_back(item);
    }
    addedItems.clear();
}

void ItemManager::initializeItems()
{
    // TODO: Have this read from an Items folder, and read from
    // all .txt files in it, Allowing greater compability between mods.
    std::ifstream input("data/items.txt");
    if (input.is_open())
    {
        while (input.good())
        {
            std::string line;
            getline(input, line);
            Item item;
            item.name = "Debuggery";
            std::string itemName = stringFindString(line, "[name:");
            if(itemName != "")
                item.name = itemName;

            item.value = stringFindNumber(line, "[value:");
            if(item.value == 0)
                item.value = -1;

            int stackSize = stringFindNumber(line, "[stackSize:");
            //std::cout << item.name << ", " << stackSize << std::endl;
            if(stackSize == 0)
                item.stackSize = 1;
            else
                item.stackSize = stackSize;

            item.hungervalue = stringFindNumber(line, "[hungervalue:");
            item.thirstvalue = stringFindNumber(line, "[thirstvalue:");

            item.rarity = stringFindNumber(line, "[Rarity:");

            item.spread = stringFindNumber(line, "[Spread:");
            item.projectiles = stringFindNumber(line, "[Projectiles:");
            item.radius = stringFindNumber(line, "[radius:");
            item.penetration = stringFindNumber(line, "[penetration:");
            item.richochets = stringFindNumber(line, "[richochets:");
            item.knockback = stringFindNumber(line, "[knockback:");
            item.healAmount = stringFindNumber(line, "[HealAmount:");
            std::string damageString = stringFindString(line, "[DamageType:");
            if(damageString != "")
                item.damageType = damageTypes.getNum(damageString);


            item.canFireSemi = booleanize(stringFindNumber(line, "[CanFireSemi:"));
            item.canFireBurst = booleanize(stringFindNumber(line, "[CanFireBurst:"));
            item.canFireAuto = booleanize(stringFindNumber(line, "[CanFireAuto:"));

            item.barrelCount = stringFindNumber(line, "[BarrelCount:");
            item.damageMultiplier = stringFindNumber(line, "[DamageMultiplier:");
            item.dispersion = stringFindNumber(line, "[Dispersion:");
            item.aimTime = stringFindNumber(line, "[AimTime:");
            item.recoil = stringFindNumber(line, "[Recoil:");
            item.recoilReduction = stringFindNumber(line, "[RecoilReduction:");
            item.fireDelay = stringFindNumber(line, "[FireDelay:");
            item.burstCount = stringFindNumber(line, "[BurstCount:");
            item.reloadTime = stringFindNumber(line, "[ReloadTime:");
            item.reloadAmount = stringFindNumber(line, "[ReloadAmount:");
            item.ammoCapacity = stringFindNumber(line, "[AmmoCapacity:");
            item.bulletSpeedMultiplier = stringFindNumber(line, "[BulletSpeedMultiplier:");
            item.durability = stringFindNumber(line, "[Durability:");
            item.durabilityCost = stringFindNumber(line, "[DurabilityCost:");

            item.speed = stringFindNumber(line, "[Speed:");




            item.massGlass = stringFindNumber(line, "[MassGlass:");
            item.massFlesh = stringFindNumber(line, "[MassFlesh:");
            item.massMetal = stringFindNumber(line, "[MassMetal:");
            item.massOil = stringFindNumber(line, "[MassOil:");
            item.massPlastic = stringFindNumber(line, "[MassPlastic:");
            item.massVeggy = stringFindNumber(line, "[MassVeggy:");
            item.massWater = stringFindNumber(line, "[MassWater:");
            item.ammoCapacity = stringFindNumber(line, "[AmmoCapacity:");

            item.size = 0;
            item.size = stringFindNumber(line, "[Size:");

            item.pickupable =
                booleanize(stringFindNumber(line, "[Pickupable:"));

            //item.type = stringFindNumber(line, "[type:");
            std::string typeString = stringFindString(line, "[type:");
            if(typeString != "")
                item.type = itemTypes.getTypeID(typeString).num;


            item.ammotype = stringFindNumber(line, "[ammotype:");
            item.cbaseid = stringFindNumber(line, "[baseid:");
            item.produces = booleanize(stringFindNumber(line, "[produces:"));
            item.prodrate = stringFindNumber(line, "[prodrate:");
            item.produce = stringFindString(line, "[produce:");
            item.mindam = stringFindNumber(line, "[mindam:");
            item.maxdam = stringFindNumber(line, "[maxdam:");

            item.range = stringFindNumber(line, "[range:");
            item.activaterate = 0 ; //stringFindNumber(line, "[range:");
            item.activaterategrowth = stringFindNumber(line, "[activaterategrowth:");
            item.activateratemax = stringFindNumber(line, "[activateratemax:");
            item.isWeapon = booleanize(stringFindNumber(line, "[IsWeapon:"));

            std::vector<std::string> statusEffects = stringFindVectorChaos(line,"[CarryStatusEffect:","]");
            if(!statusEffects.empty())
                for(auto &status : statusEffects)
                {
                    StatusEffect actualStatus = globalStatusEffects.getStatusEffect(status);
                    actualStatus.duration = 1;
                    item.statusEffectsCarried.push_back(actualStatus);
                }


            statusEffects.clear();
            statusEffects = stringFindVectorChaos(line,"[InflictStatusEffect:","]");
            if(!statusEffects.empty())
                for(auto &status : statusEffects)
                    item.statusEffectsInflict.push_back(globalStatusEffects.getStatusEffect(status));


            statusEffects.clear();
            statusEffects = stringFindVectorChaos(line,"[StatusEffect:","]");
            if(!statusEffects.empty())
                for(auto &status : statusEffects)
                {
                    StatusEffect actualStatus = globalStatusEffects.getStatusEffect(status);
                    actualStatus.duration = 1;
                    item.statusEffects.push_back(actualStatus);
                }












            std::string imagery = stringFindString(line, "[image:");
            for (auto const &image : texturemanager.textures)
            {
                if (image.name == imagery)
                {
                    item.img.setTexture(image.texture);
                    sf::IntRect tempz = item.img.getTextureRect();
                    sf::Vector2f size(tempz.height, tempz.width);
                    if (gvars::debug)
                    {
                        std::cout << "TextRect: " << tempz.height << ":"
                                  << tempz.width << std::endl;
                    }
                    item.img.setOrigin(size.x / 2, size.y / 2);
                }
            }
            if (item.name != "Debuggery")
            {
                con("Adding Item: " + item.name);
                globalItems.push_back(item);
            }
        }
    }

    std::ifstream input2("data/items.txt");
    if (input2.is_open())
    {
        while (input2.good())
        {
            std::string line;
            getline(input2, line);
            Item * item = getGlobalItem(stringFindString(line, "[name:"));

            std::vector<std::string> internalItems = stringFindVectorChaos(line,"[InternalItem:","]");
            if(!internalItems.empty())
            {
                std::cout << "Loading: " << item->name << "(";
                for(auto &itemstr : internalItems)
                {
                    std::cout << itemstr;
                    Item component = *getGlobalItem(itemstr);
                    item->internalitems.push_back(component);
                }
                std::cout << ")\n";
            }

        }
    }

    for (size_t i = 0; i != globalItems.size(); i++)
    {
        for (size_t t = 0; t != globalItems.size(); t++)
        {
            if (i != t)
            {
                if (globalItems[i].cbaseid == globalItems[t].cbaseid)
                {
                    std::cout
                        << " \n \n cbaseid Conflict Detected between "
                        << globalItems[i].name << " and " << globalItems[t].name
                        << ", cbaseid: " << globalItems[i].cbaseid << std::endl;
                }
            }
        }
    }
}

std::vector<Item> ItemManager::getAllofType(int type)
{
    std::vector<Item> returns;
    for(auto &items : globalItems)
        if(items.type == type)
            returns.push_back(items);

    return returns;
}

void offloadItems()
{
    for (auto &worlditem : worlditems)
    {
        if ((worlditem).xpos > 1920 && (worlditem).ypos < 640)
        {
            (worlditem).xpos = (worlditem).xpos - 640 - 640 - 640;
            saveItem(500, sf::Vector2i(gvars::currentregionx + 2,gvars::currentregiony - 1),(worlditem));
            (worlditem).toDelete = true;
        }
        else if ((worlditem).xpos > 1920 && (worlditem).ypos > 1280)
        {
            (worlditem).xpos = (worlditem).xpos - 640 - 640 - 640;
            (worlditem).ypos = (worlditem).ypos - 640 - 640;
            saveItem(500, sf::Vector2i(gvars::currentregionx + 2,gvars::currentregiony + 1),(worlditem));
            (worlditem).toDelete = true;
        }
        else if ((worlditem).xpos < 0 && (worlditem).ypos > 1280)
        {
            (worlditem).xpos = (worlditem).xpos + 640;
            (worlditem).ypos = (worlditem).ypos - 640 - 640;
            saveItem(500, sf::Vector2i(gvars::currentregionx - 2,gvars::currentregiony + 1),(worlditem));
            (worlditem).toDelete = true;
        }
        else if ((worlditem).xpos < 0 && (worlditem).ypos < 640)
        {
            (worlditem).xpos = (worlditem).xpos + 640;
            saveItem(500, sf::Vector2i(gvars::currentregionx - 2,gvars::currentregiony - 1),(worlditem));
            (worlditem).toDelete = true;
        }

        else if ((worlditem).ypos < 0 && (worlditem).xpos > 1280)
        {
            (worlditem).xpos = (worlditem).xpos - 640 - 640;
            (worlditem).ypos = (worlditem).ypos + 640;
            saveItem(500, sf::Vector2i(gvars::currentregionx + 1,gvars::currentregiony - 2),(worlditem));
            (worlditem).toDelete = true;
        }
        else if ((worlditem).ypos < 0 && (worlditem).xpos < 640)
        {
            (worlditem).ypos = (worlditem).ypos + 640;
            saveItem(500, sf::Vector2i(gvars::currentregionx - 1,gvars::currentregiony - 2),(worlditem));
            (worlditem).toDelete = true;
        }

        else if ((worlditem).ypos > 1920 && (worlditem).xpos > 1280)
        {
            (worlditem).xpos = (worlditem).xpos - 640 - 640;
            (worlditem).ypos = (worlditem).ypos - 640 - 640 - 640;
            saveItem(500, sf::Vector2i(gvars::currentregionx + 1,gvars::currentregiony + 2),(worlditem));
            (worlditem).toDelete = true;
        }
        else if ((worlditem).ypos > 1920 && (worlditem).xpos < 640)
        {
            (worlditem).ypos = (worlditem).ypos - 640 - 640 - 640;
            saveItem(500, sf::Vector2i(gvars::currentregionx - 1,gvars::currentregiony + 2),(worlditem));
            (worlditem).toDelete = true;
        }

        else if ((worlditem).xpos > 1920)
        {
            (worlditem).xpos = (worlditem).xpos - 640 - 640 - 640;
            (worlditem).ypos = (worlditem).ypos - 640;
            saveItem(500, sf::Vector2i(gvars::currentregionx + 2,gvars::currentregiony),(worlditem));
            (worlditem).toDelete = true;
        }
        else if ((worlditem).ypos > 1920)
        {
            (worlditem).xpos = (worlditem).xpos - 640;
            (worlditem).ypos = (worlditem).ypos - 640 - 640 - 640;
            saveItem(500, sf::Vector2i(gvars::currentregionx,gvars::currentregiony + 2),(worlditem));
            (worlditem).toDelete = true;
        }
        else if ((worlditem).xpos < 0)
        {
            (worlditem).xpos = (worlditem).xpos + 640;
            (worlditem).ypos = (worlditem).ypos - 640;
            saveItem(500, sf::Vector2i(gvars::currentregionx - 2,gvars::currentregiony),(worlditem));
            (worlditem).toDelete = true;
        }
        else if ((worlditem).ypos < 0)
        {
            (worlditem).xpos = (worlditem).xpos - 640;
            (worlditem).ypos = (worlditem).ypos + 640;
            saveItem(500, sf::Vector2i(gvars::currentregionx,gvars::currentregiony - 2),(worlditem));
            (worlditem).toDelete = true;
        }
    }
}

void hoverItemIDdisplay()
{
    for (auto const &item : worlditems)
    {
        if (math::closeish(gvars::mousePos.x, gvars::mousePos.y,item.xpos, item.ypos) <= 10)
            textList.createText(item.xpos, item.ypos, 11,sf::Color::White, item.name, " ID:",item.id);
    }
}

std::list<Item> itemlist;

itemPtrVector makeItems(std::list<Item> &items, int maxamount)
{
    itemPtrVector IPV;
    int totalNew = randz(1,maxamount);
    for(int i = 0; i != totalNew; i++)
    {
        Item item;
        int globalItem = randz(0,itemmanager.globalItems.size()-1);
        item = itemmanager.globalItems[globalItem];
        //item.maxdam = randz(3,9);
        item.id = gvars::globalid++;
        items.push_back(item);
        IPV.ptrs.push_back(&items.back());
    }
    return IPV;
}

void printItems(std::list<Item> &items)
{
    for( auto &item : items)
    {
        std::cout << "Item: " << item.name << ", Damage: " << item.maxdam << std::endl;
    }
}

int totalDamageofItems(std::list<Item> &items)
{
    int returns = 0;
    for( auto &item : items)
    {
        returns += item.maxdam;
    }
    float average = (returns/items.size());
    std::cout << "Average: " << average << std::endl;
    return returns;
}

int totalDamageofItemsInternalized(std::list<Item> &items)
{
    int returns = 0;
    for( auto &item : items)
    {
        int personalreturns = 0;
        if(!item.internalitems.empty())
        {
            personalreturns += totalDamageofItemsInternalized(item.internalitems);
        }

        returns += (item.maxdam + personalreturns);
    }
    float average = (returns/items.size());
    std::cout << "Average: " << average << std::endl;
    return returns;
}

Item *getItemType(std::list<Item> &inventory, int type)
{
    //for(int i = 0; i < inventory.size(); i++)
    for (auto &elem : inventory)
    {
        if ((elem).type == type)
        {
            return &(elem);
        }
    }
    return nullptr;
}


itemPtrVector randomEquipment(std::list<Item> &inventory)
{
    std::cout << "Pre existing items: ";
    for (auto &items : inventory)
    {
        std::cout << items.name << ", ";
    }
    std::cout << std::endl;
    itemPtrVector returns;

    returns = makeItems(inventory,7);
    return returns;
}

std::string Item::shootThing(Vec3f vPos, Item * itemptr)
{
    Vec3f muzzlePos(user->xpos,user->ypos,user->zpos);
        sf::Vector2f muzzlePosV2f(muzzlePos.x,muzzlePos.y);
        sf::Vector2f vPosV2f(vPos.x,vPos.y);

        Bullet boolet;
        boolet.owner = user;
        boolet.parent = this;
        boolet.weapon = *this;
        boolet.ammo = *itemptr;
        boolet.pos = muzzlePos;
        boolet.positions.push_back(boolet.pos);
        boolet.angle = math::angleBetweenVectors(muzzlePosV2f,vPosV2f);
        if(ammotype == 5)
            boolet.tex = &texturemanager.getTexture("RocketBulletv2.png");
        else
            boolet.tex = &texturemanager.getTexture("NormalBullet.png");

        boolet.radius = itemptr->radius;
        boolet.maxrichochet = itemptr->richochets;
        boolet.penetration = itemptr->penetration;



        boolet.damage = getMaxDamage()*getDamageMultiplier();
        int bulletDamage = boolet.damage;

        if(user->skills.getRanks("Bronze Bullet") > 0 && itemptr->amount == getAmmoCapacity())
        { // Bronze Bullet
            std::cout << "Bronzing Bullet: " << boolet.damage;
            int bronzemultiplier = user->skills.getRanks("Bronze Bullet")*10;
            boolet.damage += bulletDamage*(bronzemultiplier*0.01);
            std::cout << " to " << boolet.damage << std::endl;
        }

        int halfclip = getAmmoCapacity()/2;
        if(user->skills.getRanks("Silver Bullet") > 0 && itemptr->amount == halfclip)
        { // Silver Bullet
            std::cout << "Silvering Bullet: " << boolet.damage;
            int silvermultiplier = user->skills.getRanks("Silver Bullet")*20;
            boolet.damage += bulletDamage*(silvermultiplier*0.01);
            std::cout << " to " << boolet.damage << std::endl;
        }

        if(user->skills.getRanks("Gold Bullet") > 0 && itemptr->amount == 1)
        {// Gold Bullet
            std::cout << "Golding Bullet: " << boolet.damage;
            int goldmultiplier = user->skills.getRanks("Gold Bullet")*30;
            boolet.damage += bulletDamage*(goldmultiplier*0.01);
            std::cout << " to " << boolet.damage << std::endl;
        }


        boolet.targets = user->getEnemies();

        boolet.speed = getSpeed() * getBulletSpeedMultiplier();
        boolet.lifetime = 600;


        float totalDispersion = math::clamp(getDispersion(),0,9999999) + math::clamp(recoilCounter,0,9999999);

        int amountOfShots = 0;
        if(itemptr->amount > getBarrelCount())
            amountOfShots = getBarrelCount();
        else
            amountOfShots = itemptr->amount;

        for(int shots = 0; shots != amountOfShots; shots++)
        {
            for(int i = 0; i != itemptr->projectiles; i++)
            {


                if(randz(0,1) == 1)
                    boolet.angle += randz(0,totalDispersion/2);
                else
                    boolet.angle -= randz(0,totalDispersion/2);

                bullets.push_back(boolet);
            }
            recoilCounter += getRecoil();
            recoilCounter = math::clamp(recoilCounter,0,100); // Preventing the bullets from being TOO ridiculous.
        }




        int miscountRoll = randz(0,100);
        if(miscountRoll < user->skills.getRanks("Miscounted Shot")*5)
        {
            std::cout << "Miscounted shot! " << miscountRoll << " vs " << user->skills.getRanks("Miscounted Shot")*5 << std::endl;
            std::cout << "This critter possesses " << user->skills.getRanks("Miscounted Shot") << " ranks in miscounted Shot \n";
        }
        else
            itemptr->amount -= amountOfShots;

        if(itemptr->amount <= 0)
            itemptr->toDelete = true;

        int ranNum = randz(1,4);
        if(ranNum == 1)
            soundmanager.playSound("m16_lensflare_1.ogg");
        if(ranNum == 2)
            soundmanager.playSound("m16_lensflare_2.ogg");
        if(ranNum == 3)
            soundmanager.playSound("m16_lensflare_3.ogg");
        if(ranNum == 4)
            soundmanager.playSound("m16_lensflare_4.ogg");

        AnyDeletes(internalitems); // This must be run last, Otherwise the ammo pointer could become null.
        return "Success";
}

std::string Item::gunThing(Vec3f vPos)
{
    if(user == nullptr)
        return "No Owner";

    if(user->mods.stunMod > 0)
        return "Stunned";

    Item * itemptr = getItemType(internalitems,ammotype);
    if(itemptr == nullptr || itemptr->amount <= 0)
        return "No Ammo";

    if(!user->isSquaddie)
    {
        if(getFireAuto())
            fireMode = 3;
        else if(getFireBurst())
            fireMode = 2;
        else if(getFireSemi())
            fireMode = 1;
    }


    //user->angle = math::angleBetweenVectors(user->getPos2d(),gvars::mousePos)-90;

    { // Progress Cone
        sf::Color coneColor;
        if(type == 1)
            coneColor = sf::Color::Blue;
        if(type == 2)
            coneColor = sf::Color::Red;
        if(type == 3)
            coneColor = sf::Color::Green;

        //std::cout << aimTimeCounter << "/" << getAimTime();
        float remainingTime = aimTimeCounter / getAimTime();
        //std::cout << "Z \n";
        //remainingTime = 0 + remainingTime;


        //std::cout << "K \n";
        int coneRadius = 180*(remainingTime);
        if(coneRadius < 0)
            coneRadius = 0;

        shapes.createCone(user->getPos2d(),user->angle,coneRadius,20,coneColor);
        //std::cout << "X \n";
    }

    { // Dispersion/Recoil Area
        float totalDispersion = math::clamp(getDispersion(),0,9999999) + math::clamp(recoilCounter,0,9999999);
        sf::Vector2f uPos = user->getPos2d();

        int angle1 = user->angle - (totalDispersion/2)+90;
        int angle2 = user->angle + (totalDispersion/2)+90;

        sf::Vector2f leftAnglePos = math::angleCalc(uPos,angle1,40);
        sf::Vector2f rightAnglePos = math::angleCalc(uPos,angle2,40);

        shapes.createLine(uPos.x,uPos.y,leftAnglePos.x,leftAnglePos.y,1,sf::Color::Red);
        shapes.createLine(uPos.x,uPos.y,rightAnglePos.x,rightAnglePos.y,1,sf::Color::Red);
    }

    { // Recoil Reduction
        recoilCounter -= getRecoilReduction()/60;
        if(recoilCounter < 0)
            recoilCounter = 0;
    }

    //static int fireType = 0; // 0 = semi, 1 = burst, 2 = automatic.


    if(inputState.key[Key::Num1])
    {
        fireMode = 0;
        chatBox.addChat("Semi!");
    }

    if(inputState.key[Key::Num2])
    {
        fireMode = 1;
        chatBox.addChat("Burst!");
    }
    if(inputState.key[Key::Num3])
    {
        fireMode = 2;
        chatBox.addChat("Auto!");
    }


    if(aimTimeCounter > 0)
    {
        aimTimeCounter--;
        return "Aiming";
    }

        user->mods.onItemUse = true;
        user->mods.onItemUseType = type;



    if(fireMode == 0)
    {
        shootThing(vPos,itemptr);
        aimTimeCounter = getAimTime();
    }

    if(fireMode == 1)
    {
        //std::cout << "c/b: " << burstCounter << "/" << getBurstCount() << std::endl;
        if(burstCounter >= getBurstCount())
        {
            aimTimeCounter = getAimTime();
            burstCounter = 0;
        }

        if(fireDelayCounter <= 0)
        {
            burstCounter++;
            shootThing(vPos,itemptr);
            fireDelayCounter = getFireDelay();
        }
        fireDelayCounter--;

    }

    if(fireMode == 2)
    {
        if(fireDelayCounter <= 0)
        {
            shootThing(vPos,itemptr);
            fireDelayCounter = getFireDelay();
        }
        fireDelayCounter--;

        //aimTimeCounter = getAimTime();
    }




    //aimTimeCounter = getAimTime();






    return "Nothing Happened.";
}

std::string Item::activate(Vec3f vPos) // Returns a string declaring the problem.
{
    if(user == nullptr)
            return "No Owner";

    if(user->mods.stunMod > 0)
        return "Stunned";

    user->mods.onItemUse = true;
    user->mods.onItemUseType = type;

    if(type == 1)
    { // Melee weapons


        Vec3f muzzlePos(user->xpos,user->ypos,user->zpos);
        sf::Vector2f muzzlePosV2f(muzzlePos.x,muzzlePos.y);
        sf::Vector2f vPosV2f(vPos.x,vPos.y);
        int rot = math::angleBetweenVectors(muzzlePosV2f,vPosV2f);

        bool Struck = false;
        for(auto &npc : npclist)
        {
            if(npc.id == user->id)
                continue;
            if(npc.health <= 0)
                continue;
            if(npc.faction == user->faction)
                continue;

            sf::Vector2f npcPos(npc.xpos,npc.ypos);

            if(math::closeish(npcPos,muzzlePosV2f) <= range)
            {
                std::string outString;
                outString.append(user->name + " has struck " + npc.name + " for " + std::to_string(maxdam) );
                //chatBox.addChat(outString,sf::Color::Red);

                std::string atkStatus = user->dealDamage(&npc,this,maxdam,damageType);
                if(atkStatus == "Hit")
                {
                    for(auto &status : statusEffectsInflict)
                        npc.statusEffects.push_back(status);
                    Struck = true;
                }

                Skill * hamstringSkill = user->skills.getSkill("Hamstring");
                if(hamstringSkill == nullptr)
                {

                }
                else if(hamstringSkill->ranks > 0 && hamstringSkill->cooldown <= 0 && hamstringSkill->autouse && atkStatus == "Hit")
                {
                    hamstringSkill->cooldown = hamstringSkill->cooldownint;
                    StatusEffect hamStatus = globalStatusEffects.getStatusEffect("Hamstring");
                    hamStatus.duration = 600*hamstringSkill->ranks;
                    npc.statusEffects.push_back(hamStatus);

                    chatBox.addChat("* " + user->name + " Hamstringed " + npc.name + "!",sf::Color::White);


                }

                Skill * cleave = user->skills.getSkill("Cleave");
                if(cleave == nullptr)
                {

                }
                else if(cleave->ranks > 0 && cleave->cooldown <= 0 && cleave->autouse)
                {
                    cleave->cooldown = cleave->cooldownint;

                    sf::Vector2f oriPos(user->getPos2d());
                    sf::Vector2f offSet = math::angleCalc(oriPos,math::constrainAngle(user->angle+90),60);

                    shapes.createImageButton(offSet,texturemanager.getTexture("Slash.png"),"",math::constrainAngle(user->angle+180) );

                    for(auto &npcCleave : npclist)
                    {
                        //Making sure this critter isn't friendly, itself, or already been hit by the attack.
                        if(npcCleave.faction == user->faction || npcCleave.id == user->id || npcCleave.id == npc.id)
                            continue;

                        //Figuring out if the target is within range, and within an angle.
                        int targetAngle = math::angleBetweenVectors(oriPos,npcCleave.getPos2d());
                        int angleDiff = math::angleDiff(user->angle,targetAngle);
                        angleDiff = math::constrainAngle(angleDiff-90);
                        int dist = math::closeish(oriPos,npcCleave.getPos2d());

                        if(angleDiff < 90 && angleDiff > -90 && dist <= 120)
                        {
                            //Found someone within range! STRIKING!
                            std::cout << npcCleave.id << "; Someone was hit by the Cleave! \n";
                            int cleaveDamage = this->maxdam*(0.75+(cleave->ranks*0.25));
                            std::string cleaveAtkStatus = user->dealDamage(&npcCleave,this,cleaveDamage,damageType);
                            if(cleaveAtkStatus == "Hit")
                                for(auto &status : statusEffectsInflict)
                                    npc.statusEffects.push_back(status);



                        }
                    }
                }


            }
        }

        shapes.createImageButton(math::angleCalc(muzzlePosV2f,rot,60),*img.getTexture(),"", rot+90);

        soundmanager.playSound("Swing_xxchr0nosxx_1.ogg");
        if(Struck == true)
            return "Success";
    }

    if(type == 2)
    { // Ranged weapons

        Item * itemptr = getItemType(internalitems,ammotype);
        if(itemptr == nullptr || itemptr->amount <= 0)
            return "No Ammo";

        Vec3f muzzlePos(user->xpos,user->ypos,user->zpos);
        sf::Vector2f muzzlePosV2f(muzzlePos.x,muzzlePos.y);
        sf::Vector2f vPosV2f(vPos.x,vPos.y);

        Bullet boolet;
        boolet.owner = user;
        boolet.parent = this;
        boolet.weapon = *this;
        boolet.ammo = *itemptr;
        boolet.pos = muzzlePos;
        boolet.positions.push_back(boolet.pos);
        boolet.angle = math::angleBetweenVectors(muzzlePosV2f,vPosV2f);
        if(ammotype == 5)
            boolet.tex = &texturemanager.getTexture("RocketBulletv2.png");
        else
            boolet.tex = &texturemanager.getTexture("NormalBullet.png");

        boolet.radius = itemptr->radius;
        boolet.maxrichochet = itemptr->richochets;
        boolet.penetration = itemptr->penetration;



        boolet.damage = random(getMinDamage(),getMaxDamage());
        int bulletDamage = boolet.damage;

        if(user->skills.getRanks("Bronze Bullet") > 0 && itemptr->amount == getAmmoCapacity())
        { // Bronze Bullet
            std::cout << "Bronzing Bullet: " << boolet.damage;
            int bronzemultiplier = user->skills.getRanks("Bronze Bullet")*10;
            boolet.damage += bulletDamage*(bronzemultiplier*0.01);
            std::cout << " to " << boolet.damage << std::endl;
        }

        int halfclip = getAmmoCapacity()/2;
        if(user->skills.getRanks("Silver Bullet") > 0 && itemptr->amount == halfclip)
        { // Silver Bullet
            std::cout << "Silvering Bullet: " << boolet.damage;
            int silvermultiplier = user->skills.getRanks("Silver Bullet")*20;
            boolet.damage += bulletDamage*(silvermultiplier*0.01);
            std::cout << " to " << boolet.damage << std::endl;
        }

        if(user->skills.getRanks("Gold Bullet") > 0 && itemptr->amount == 1)
        {// Gold Bullet
            std::cout << "Golding Bullet: " << boolet.damage;
            int goldmultiplier = user->skills.getRanks("Gold Bullet")*30;
            boolet.damage += bulletDamage*(goldmultiplier*0.01);
            std::cout << " to " << boolet.damage << std::endl;
        }



        //boolet.damage += user->skills.getRanks("Miscounted Shot")
        //10%/20%/30% copper/silver/gold, first/middle/last


        boolet.targets = user->getEnemies();
        /*
        for(auto &i : nPV.ptrs)
        {
            i->img.setTexture(texturemanager.getTexture("Error.png"));
        }
        */

        //Vec3f velo((muzzlePos.x - vPos.x)/10,(muzzlePos.y - vPos.y)/10 );
        //boolet.velocity = velo;

        //boolet.speed = math::closeish(muzzlePos.x,muzzlePos.y,vPos.x,vPos.y) / 10;
        boolet.speed = 30;
        boolet.lifetime = 600;

        for(int i = 0; i != itemptr->projectiles; i++)
        {

            if(randz(0,1) == 1)
                boolet.angle += randz(0,getSpread());
            else
                boolet.angle -= randz(0,getSpread());

            bullets.push_back(boolet);
        }



        int miscountRoll = randz(0,100);
        if(miscountRoll < user->skills.getRanks("Miscounted Shot")*5)
        {
            std::cout << "Miscounted shot! " << miscountRoll << " vs " << user->skills.getRanks("Miscounted Shot")*5 << std::endl;
            std::cout << "This critter possesses " << user->skills.getRanks("Miscounted Shot") << " ranks in miscounted Shot \n";
        }
        else
            itemptr->amount--;



        //std::cout << itemptr->name << "'s amount: " << itemptr->amount << ", toDelete: " << itemptr->toDelete << std::endl;
        if(itemptr->amount <= 0)
            itemptr->toDelete = true;
        //std::cout << itemptr->name << "'s amount: " << itemptr->amount << ", toDelete: " << itemptr->toDelete << std::endl;
        int ranNum = randz(1,4);
        if(ranNum == 1)
            soundmanager.playSound("m16_lensflare_1.ogg");
        if(ranNum == 2)
            soundmanager.playSound("m16_lensflare_2.ogg");
        if(ranNum == 3)
            soundmanager.playSound("m16_lensflare_3.ogg");
        if(ranNum == 4)
            soundmanager.playSound("m16_lensflare_4.ogg");
        return "Success";
    }

    if(type == 23)
    { // PDA Spells
        activaterate = activateratemax;

        //user->desiredViewAngle = gvars::mousePos;

        Shape shape;
        shape.shape = shape.Line;
        shape.maincolor = sf::Color::White;
        shape.seccolor = gvars::cycleRed;
        shape.startPos = user->getPos2d() + sf::Vector2f(randz(-2,2),randz(-2,2));
        shape.endPos = vPos.twoD() + sf::Vector2f(randz(-2,2),randz(-2,2));

        int oldAlpha = shape.seccolor.a;
        if( (gvars::framesPassed % 30) == 0)
        {
            shape.seccolor.a = 50;
            shape.duration = 15;
            shape.outline = 10;
            shapes.shapes.push_back(shape);

            shape.outline = 7;
            shape.seccolor.a = 150;
            shapes.shapes.push_back(shape);
        }

        shape.duration = 0;
        shape.size = 2;
        shape.outline = 3;
        shape.seccolor.a = oldAlpha;
        shapes.shapes.push_back(shape);

        shape.shape = shape.Circle;
        shapes.shapes.push_back(shape);

        shape.startPos = shape.endPos;
        shape.size = 10;
        shape.outline = 6;
        shapes.shapes.push_back(shape);

        npcPtrVector nPV = user->getEnemies();
        for(auto &npc : nPV.ptrs)
        {
            int dist = math::distance(npc->getPos(),vPos);
            if(dist <= 20)
                user->dealDamage(npc,this,69,damageType);
        }
    }

    if(type == 42)
    { // Potions.
        for(auto &status : statusEffectsInflict)
            user->statusEffects.push_back(status);

        return "Success";
    }

    return "Failed";
}

bool Item::isReady()
{
    int rateCheck = activaterate;
    if( (rateCheck + activaterategrowth) >= activateratemax )
        return true;

    return false;
}


bool Item::trigger() // Processes activation time, if activation time is ready/reset, returns true, else, returns false.
{
    activaterate += activaterategrowth;

    if(activaterate >= activateratemax)
    {
        activaterate -= activateratemax;



        // TODO: Change this to return the amount of times activaterate surpasses the max for increased spamming goodness.
        return true;
    }
    return false;
}

void Item::remove()
{
    toDelete = true;
    debug("Item::remove() called. \n");
    if(user != nullptr)
    {
        debug("user valid \n");
        for(int i = 0; i != 20; i++)
        {
            debug(std::to_string(i) + " slot is iterated \n");
            if(user->invSlots[i] != nullptr && user->invSlots[i]->id == id)
            {
                debug("We gotta match! " + user->invSlots[i]->name + "/" + name);
                user->invSlots[i] = nullptr;
                return;
            }
        }
    }
}

void Item::handleStatusEffects()
{

}

void explosion(Vec3f vPos, int radius, float damage, Npc *attacker, std::vector<Npc*> *victims)
{
    sf::Vector2f gfxPos(vPos.x,vPos.y);
    shadermanager.setShockwave(gfxPos);

    Shape shape;
    shape.shape = shape.Circle;
    shape.duration = 15;
    shape.startPos = gfxPos;
    shape.size = radius;
    shape.maincolor = sf::Color::White;
    shape.fades = true;
    shape.texture = &texturemanager.getTexture("Explosion2.png");
    shapes.shapes.push_back(shape);
    shape.duration = 30;
    shape.size = radius/2;
    shape.texture = &texturemanager.getTexture("Explosion1.png");
    shapes.shapes.push_back(shape);

    int soundRan = random(1,3);
    if(soundRan == 1)
        soundmanager.playSound("Explosion1.ogg");
    if(soundRan == 2)
        soundmanager.playSound("Explosion2.ogg");
    if(soundRan == 3)
        soundmanager.playSound("Explosion3.ogg");

    screenShake(5);

    if(victims != nullptr) // TODO: Add a vision/wall check so you can hide behind walls from explosions.
        for(auto &i : *victims)
        {
            int distance = math::distance(i->getPos(),vPos);
            if(distance <= radius)
            {
                if(!canSeeBetweenTiled(vPos,i->getPos()))
                    continue;

                if(i->faction == conFact->name)
                    chatBox.addChat(i->name + " was hit by an explosion!",sf::Color::Red);

                if(attacker != nullptr)
                    std::string atkStatus = attacker->dealDamage(i,nullptr,damage,damageTypes.getNum("Blunt"));
                else
                    i->takeDamage(nullptr, nullptr, damage);

                {// Knockback Code

                    int knockbackAmount = damage;
                    sf::Vector2f AtkerPos(vPos.x,vPos.y);

                    sf::Vector2f VictPos = i->getPos2d();
                    float attackAngle = math::angleBetweenVectors(AtkerPos,VictPos);
                    sf::Vector2f finalPos = math::angleCalc(VictPos,attackAngle,knockbackAmount);

                    sf::Vector2f compared(finalPos.x-i->xpos,finalPos.y-i->ypos);
                    if(AtkerPos != sf::Vector2f())
                        i->momentum += compared;

                }


            }
        }
    else
        for(auto &i : npclist)
        {
            int distance = math::distance(i.getPos(),vPos);
            if(distance <= radius)
            {
                if(!canSeeBetweenTiled(vPos,i.getPos()))
                    continue;

                if(i.faction == conFact->name)
                    chatBox.addChat(i.name + " was hit by an explosion!",sf::Color::Red);

                if(attacker != nullptr)
                    std::string atkStatus = attacker->dealDamage(&i,nullptr,damage,damageTypes.getNum("Blunt"));
                else
                    i.takeDamage(nullptr, nullptr, damage);



                {// Knockback Code

                    int knockbackAmount = damage;
                    sf::Vector2f AtkerPos(vPos.x,vPos.y);

                    sf::Vector2f VictPos = i.getPos2d();
                    float attackAngle = math::angleBetweenVectors(AtkerPos,VictPos);
                    sf::Vector2f finalPos = math::angleCalc(VictPos,attackAngle,knockbackAmount);

                    sf::Vector2f compared(finalPos.x-i.xpos,finalPos.y-i.ypos);
                    if(AtkerPos != sf::Vector2f())
                        i.momentum += compared;

                }


            }
        }

    for(int x = 0; x != GRIDS; x++)
        for(int y = 0; y != GRIDS; y++)
    {
        if(tiles[x][y][gvars::currentz].id == 2012)
            if(math::closeish(vPos.x,vPos.y,x*GRID_SIZE,y*GRID_SIZE) <= radius)
            {
                tiles[x][y][gvars::currentz].BTstone();
            }

    }
}



