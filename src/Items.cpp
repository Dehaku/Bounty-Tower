#include "Items.h"
#include "filesystemUtils.hpp"
#include "globalvars.h"
#include "Textures.h"
#include "math.h"
#include "util.h"

#include <fstream>
#include <sstream>
#include <iostream>

extern sf::RenderWindow window;

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

void Item::chargecell(int amount)
{
    cellcharge += amount;
}

void Item::soulcell(int soultype)
{
    chargecell(soultype * 10);
}

bool Item::getid(int idz)
{
    if (idz == id)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Item::boolname(char *cakezebra)
{
    if (cakezebra == name)
    {
        return true;
    }
    return false;
}

void Item::printInfo()
{

    using namespace std;
    cout << "Name: " << name.c_str() << " (" << id << ") " << endl;
    cout << "Weight: " << weight << endl;
    cout << "Value: " << value << endl;
    if (useCell == true)
    {
        cout << "celltype is: " << celltype << endl;
    }
    if (useCell == true)
    {
        cout << "cellcharge is: " << cellcharge << endl;
    }
    if (ammotype > 0)
    {
        cout << "MaxClip: " << maxclip << endl;
    }
    if (ammotype > 0)
    {
        cout << "Current Clip: " << currentclip << endl;
    }
    cout << "Min Damage: " << mindam << endl;
    cout << "Max Damage: " << maxdam << endl;
    if (isMagic == true)
    {
        cout << " *Magical* " << endl;
    }
}

void Item::drawImg()
{
    img.setPosition(xpos, ypos);
    //img.setScale(globals::Scalex,globals::Scaley);
    window.draw(img);
}

void Item::spawn(int /*gposx*/, int /*gposy*/, int /*rposx*/, int /*rposy*/,
                 int posx, int posy, int posz)
{
    gxpos = 100;
    gypos = 100;
    rxpos = 100;
    rypos = 100;
    xpos = posx;
    ypos = posy;
    zpos = posz;
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

Item::Item()
    : cbaseid{}, range{}, xpos{}, ypos{}, zpos{30*20}, rxpos{}, rypos{}, gxpos{},
      gypos{}, imgstrx{}, imgstry{}, imgendx{}, imgendy{}, isWeapon{},
      tillrot{}, prodrate{}, prodratetimer{}, timerdecon{}, liquidContainMax{},
      objectContainMax{}, containAmount{}
{
    toDelete = false;
    targetPos = sf::Vector2f(-1, -1);
    id = gvars::globalid++;
    name = "";
    weight = 0;
    value = 0;
    type = 0;
    age = 0;
    amount = 1;

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

    hasCell = false;
    useCell = false;

    pickupable = true;

    celltype = 0;
    cellcharge = 0;
    cellcost = 0;

    isMagic = false;
    stacks = false;

    // 0 = none/melee, 6 = small bullet, 7 = large bullet
    ammotype = 0;
    maxclip = 0;
    currentclip = 0;
    mindam = 1;
    maxdam = 2;

    hasCell = false;
    useCell = false;
    isMagic = false;
    stacks = false;
    rotten = false;
    isLiquidContainer = false;
    isObjectContainer = false;
    pushable = false;
    blocksmovement = false;
    collectssolar = false;
    collectsheat = false;
    makeslight = false;
    needspower = false;
    produces = false;
    needmatprod = false;
    deconstrutionauto = false;
    buried = false;
    hidden = false;
    ProdOn = false;
    IsOn = false;
    Sleepable = false;
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
            item.name = stringFindString(line, "[name:");

            item.hungervalue = stringFindNumber(line, "[hungervalue:");
            item.thirstvalue = stringFindNumber(line, "[thirstvalue:");

            item.massGlass = stringFindNumber(line, "[MassGlass:");
            item.massFlesh = stringFindNumber(line, "[MassFlesh:");
            item.massMetal = stringFindNumber(line, "[MassMetal:");
            item.massOil = stringFindNumber(line, "[MassOil:");
            item.massPlastic = stringFindNumber(line, "[MassPlastic:");
            item.massVeggy = stringFindNumber(line, "[MassVeggy:");
            item.massWater = stringFindNumber(line, "[MassWater:");

            item.pickupable =
                booleanize(stringFindNumber(line, "[Pickupable:"));
            item.type = stringFindNumber(line, "[type:");
            item.cbaseid = stringFindNumber(line, "[baseid:");
            item.produces = booleanize(stringFindNumber(line, "[produces:"));
            item.prodrate = stringFindNumber(line, "[prodrate:");
            item.produce = stringFindString(line, "[produce:");
            item.mindam = stringFindNumber(line, "[mindam:");
            item.maxdam = stringFindNumber(line, "[maxdam:");
            item.range = stringFindNumber(line, "[range:");
            item.isWeapon = booleanize(stringFindNumber(line, "[IsWeapon:"));
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
                globalItems.push_back(item);
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
