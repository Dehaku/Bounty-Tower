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

int Item::getRange()
{
    debug(name + " getRange()");
    int returns = range;
    if(type == 2)
    {
        Item * itemptr = getItemType(internalitems,3);
        if(itemptr == nullptr)
            returns += 0;
        else
            returns += itemptr->range;
    }
    debug(name + " getRange() done");
    return returns;
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
    slotted = false;
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

            item.size = 0;
            item.size = stringFindNumber(line, "[Size:");

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
            item.activaterate = 0 ; //stringFindNumber(line, "[range:");
            item.activaterategrowth = stringFindNumber(line, "[activaterategrowth:");
            item.activateratemax = stringFindNumber(line, "[activateratemax:");
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
        item.maxdam = randz(3,9);
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

std::string Item::activate(Vec3f vPos) // Returns a string declaring the problem.
{
    if(type == 1)
    {
        if(user == nullptr)
            return "No Owner";

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
                chatBox.addChat(outString,sf::Color::Red);

                user->dealDamage(npc,*this);
                //npc.modhealth(-maxdam);

                Struck = true;
            }
        }


        //createImageButton(math::angleCalc(muzzlePosV2f,rot,60),texturemanager.getTexture("BTSword.png"),"", rot+90);
        //sf::Texture texy = ;
        createImageButton(math::angleCalc(muzzlePosV2f,rot,60),*img.getTexture(),"", rot+90);

        soundmanager.playSound("Swing_xxchr0nosxx_1.ogg");
        if(Struck == true)
            return "Success";
    }

    if(type == 2)
    {
        if(user == nullptr)
            return "No Owner";

        Item * itemptr = getItemType(internalitems,3);
        if(itemptr == nullptr || itemptr->amount <= 0)
            return "No Ammo";

        Vec3f muzzlePos(user->xpos,user->ypos,user->zpos);
        sf::Vector2f muzzlePosV2f(muzzlePos.x,muzzlePos.y);
        sf::Vector2f vPosV2f(vPos.x,vPos.y);

        Bullet boolet;
        boolet.owner = user;
        boolet.parent = this;
        boolet.pos = muzzlePos;
        boolet.positions.push_back(boolet.pos);
        boolet.angle = math::angleBetweenVectors(muzzlePosV2f,vPosV2f);


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
        boolet.speed = 60;
        boolet.lifetime = 600;
        bullets.push_back(boolet);
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
    return "Failed";
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






