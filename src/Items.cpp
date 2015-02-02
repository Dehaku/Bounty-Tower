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
cItemManager itemmanager;

void RemoveItems(std::list<Item> &Items)
{
    bool Done = false;
    while (Done == false)
    {
        Done = true;
        for (auto it = Items.begin(); it != Items.end(); ++it)
        {
            if (it->toDelete)
            {
                Done = false;
                Items.erase(it);
                break;
            }
        }
    }
}

void zSaveItem(int /*planet*/, sf::Vector2i Region, Item &object)
{

    using namespace std;
    ofstream File; // Start to Load Map
    string newline("data/maps/Planet");
    stringstream newconvert;
    newline.append("500");
    newline.append("/stuff");
    newconvert << "x";
    newconvert << Region.x;
    newconvert << "y";
    newconvert << Region.y;
    newline.append(newconvert.str());
    string newending(".item");
    newline.append(newending);
    Con(newline);

    File.open(newline.c_str(), fstream::in | fstream::ate);
    debug("looking for file...");

    if (File.is_open())
    {
        File << std::endl;
        File << "[name:" << object.name << "]"
             << "[xpos:" << object.xpos << "]"
             << "[ypos:" << object.ypos << "]";
        Con("Added", false);
        Con(object.name);
        File.close();
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
        convert << Region.x;
        convert << "y";
        convert << Region.y;
        line.append(convert.str());
        string ending(".item");
        line.append(ending);
        Con(line);

        ofstream outputFile(line.c_str());

        outputFile << "[" << object.name << "," << object.xpos << ","
                   << object.ypos << "]";
    }
}

void SaveItem(int planet, sf::Vector2i Region, Item &Critter)
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
    convert << Region.x;
    convert << "y";
    convert << Region.y;
    line.append(convert.str());
    string ending(".item");
    line.append(ending);

    ofstream File;
    File.open(line.c_str(), fstream::in | fstream::ate);
    debug("looking for file...");

    if (File.is_open())
    {
        File << std::endl;
        File << "[name:" << Critter.name << "]"
             << "[xpos:" << Critter.xpos << "]"
             << "[ypos:" << Critter.ypos << "]";
        File.close();
    }
    else
    {
        ofstream outputFile(line.c_str());

        outputFile << "[name:" << Critter.name << "]"
                   << "[xpos:" << Critter.xpos << "]"
                   << "[ypos:" << Critter.ypos << "]";
    }
}

std::string LoadItems(sf::Vector2i WorldPos, std::string Direction, int planet)
{

    std::string line("data/maps/Planet");
    std::stringstream convert;
    convert << planet;
    line.append(convert.str());
    convert.clear();
    galaxy_mkdir(line);
    line.append("/stuff");
    convert << "x";
    convert << WorldPos.x;
    convert << "y";
    convert << WorldPos.y;
    line.append(convert.str());
    std::string ending(".item");
    line.append(ending);

    std::ifstream Input(line.c_str());
    if (Input.is_open())
    {
        while (Input.good())
        {
            std::string line;
            getline(Input, line);
            Item Critter;

            Critter.name = "Debuggery";

            Critter.name = StringFindString(line, "[name:");
            if (Critter.name != "Debuggery")
                Critter = *GetGlobalItem(Critter.name);
            Critter.xpos = StringFindNumber(line, "[xpos:");
            Critter.ypos = StringFindNumber(line, "[ypos:");
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

            if (Critter.name != "Debuggery")
            {
                worlditems.push_back(Critter);
            }
        }
    }
    return line;
}

void SpawnItem(std::string Object, int xpos, int ypos)
{
    if (gvars::Debug)
    {
        std::cout << "Spawning" << Object << " \n";
    }
    cItem var;
    if (gvars::Debug)
    {
        std::cout << "Pre var in SpawnItem"
                  << " \n";
    }
    var = *GetGlobalItem(Object);
    if (gvars::Debug)
    {
        std::cout << "Post var in SpawnItem"
                  << " \n";
    }

    var.id = gvars::globalid++;
    var.xpos = xpos;
    var.ypos = ypos;
    itemmanager.AddedItems.push_back(var);
    if (gvars::Debug)
    {
        std::cout << "Done Spawning. \n";
    }
}

void RefreshImages()
{
    // PANIC
    for (size_t i = 0; i != worlditems.size(); i++)
    {
        for (size_t t = 0; t != itemmanager.GlobalItem.size(); t++)
        {
            //if(worlditems.at(i).name == itemmanager.GlobalItem.at(t).name)
            {
                //worlditems.at(i).img.setTexture(*itemmanager.GlobalItem.at(t).img.getTexture());
            }
        }
    }
    Con("Done");
}

Item *GetItemPtrfromVector(std::list<Item> &Vector, std::string Name)
{
    debug("Doing GetItmPtr");
    //*for(int i = 0; i != Vector.size(); i++)
    for (auto i = Vector.begin(); i != Vector.end(); i++)
    {
        if ((*i).name == Name)
        {
            debug("Returning GetItmPtr");
            return &(*i);
        }
    }

    debug("GetItmPtr didn't find anything, returning NULL");
    return nullptr;
}

Item *GetItemPtrfromVector2(std::list<Item> &Vector, std::string Name)
{
    debug("Doing GetItmPtr");
    //for(int i = 0; i != Vector.size(); i++)
    for (auto &elem : Vector)
    {
        if ((elem).name == Name)
        {
            debug("Returning GetItmPtr");
            return &(elem);
        }
    }

    debug("GetItmPtr didn't find anything, returning NULL");
    return nullptr;
}

Item *GetItemPtrfromVectorVarSearch(std::list<Item> &Vector,
                                    std::string VarSearch, float AtLeast)
{
    debug("Doing GetItmPtrVarSearch");
    //for(int i = 0; i != Vector.size(); i++)
    for (auto i = Vector.begin(); i != Vector.end(); i++)
    {

        if ((*i).massFlesh >= AtLeast && VarSearch == "MassFlesh")
        {
            debug("Returning GetItmPtrVarSearch");
            return &(*i);
        }
        if ((*i).massVeggy >= AtLeast && VarSearch == "MassVeggy")
        {
            debug("Returning GetItmPtrVarSearch");
            return &(*i);
        }
        if ((*i).massWater >= AtLeast && VarSearch == "MassWater")
        {
            debug("Returning GetItmPtrVarSearch");
            return &(*i);
        }
        if ((*i).massMetal >= AtLeast && VarSearch == "MassMetal")
        {
            debug("Returning GetItmPtrVarSearch");
            return &(*i);
        }
        if ((*i).massPlastic >= AtLeast && VarSearch == "MassPlastic")
        {
            debug("Returning GetItmPtrVarSearch");
            return &(*i);
        }
    }

    debug("GetItmPtrVarSearch didn't find anything, returning NULL");
    return nullptr;
}

Item *FindClosestItemPtr(int Orix, int Oriy, std::string TarItem, int /*Gxpos*/,
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

    Item *Returns;

    for (auto i = worlditems.begin(); i != worlditems.end(); i++)
    {
        if (first == true)
        {
            if ((*i).name == TarItem)
            {
                closx = (*i).xpos;
                closy = (*i).ypos;
                first = false;
                Returns = &(*i);
            }
        }
        else
        {
            if ((*i).name == TarItem)
            {
                int one = math::closeish(Orix, Oriy, (*i).xpos, (*i).ypos);
                int two = math::closeish(Orix, Oriy, closx, closy);
                if (one < two)
                {
                    closx = (*i).xpos;
                    closy = (*i).ypos;
                    Returns = &(*i);
                }
            }
        }
    }
    if (first != true)
    {
        return Returns; // Returns = (xpos,ypos,id,Vector Position)
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
                 int posx, int posy)
{
    gxpos = 100;
    gypos = 100;
    rxpos = 100;
    rypos = 100;
    xpos = posx;
    ypos = posy;
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
    : cbaseid{}, range{}, xpos{}, ypos{}, zpos{}, rxpos{}, rypos{}, gxpos{},
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

void cItemManager::AddItems()
{
    for (auto const &item : AddedItems)
    {
        worlditems.push_back(item);
    }
    AddedItems.clear();
}

void cItemManager::InitializeItems()
{
    // TODO: Have this read from an Items folder, and read from
    // all .txt files in it, Allowing greater compability between mods.
    std::ifstream Input("data/items.txt");
    if (Input.is_open())
    {
        while (Input.good())
        {
            std::string line;
            getline(Input, line);
            cItem Item;
            Item.name = "Debuggery";
            Item.name = StringFindString(line, "[name:");

            Item.hungervalue = StringFindNumber(line, "[hungervalue:");
            Item.thirstvalue = StringFindNumber(line, "[thirstvalue:");

            Item.massGlass = StringFindNumber(line, "[MassGlass:");
            Item.massFlesh = StringFindNumber(line, "[MassFlesh:");
            Item.massMetal = StringFindNumber(line, "[MassMetal:");
            Item.massOil = StringFindNumber(line, "[MassOil:");
            Item.massPlastic = StringFindNumber(line, "[MassPlastic:");
            Item.massVeggy = StringFindNumber(line, "[MassVeggy:");
            Item.massWater = StringFindNumber(line, "[MassWater:");

            Item.pickupable =
                Booleanize(StringFindNumber(line, "[Pickupable:"));
            Item.type = StringFindNumber(line, "[type:");
            Item.cbaseid = StringFindNumber(line, "[baseid:");
            Item.produces = Booleanize(StringFindNumber(line, "[produces:"));
            Item.prodrate = StringFindNumber(line, "[prodrate:");
            Item.produce = StringFindString(line, "[produce:");
            Item.mindam = StringFindNumber(line, "[mindam:");
            Item.maxdam = StringFindNumber(line, "[maxdam:");
            Item.range = StringFindNumber(line, "[range:");
            Item.isWeapon = Booleanize(StringFindNumber(line, "[IsWeapon:"));
            std::string Imagery = StringFindString(line, "[image:");
            for (auto const &image : texturemanager.textures)
            {
                if (image.name == Imagery)
                {
                    Item.img.setTexture(image.texture);
                    sf::IntRect Tempz = Item.img.getTextureRect();
                    sf::Vector2f Size(Tempz.height, Tempz.width);
                    if (gvars::Debug)
                    {
                        std::cout << "TextRect: " << Tempz.height << ":"
                                  << Tempz.width << std::endl;
                    }
                    Item.img.setOrigin(Size.x / 2, Size.y / 2);
                }
            }
            if (Item.name != "Debuggery")
            {
                GlobalItem.push_back(Item);
            }
        }
    }
    for (size_t i = 0; i != GlobalItem.size(); i++)
    {
        for (size_t t = 0; t != GlobalItem.size(); t++)
        {
            if (i != t)
            {
                if (GlobalItem[i].cbaseid == GlobalItem[t].cbaseid)
                {
                    std::cout
                        << " \n \n cbaseid Conflict Detected between "
                        << GlobalItem[i].name << " and " << GlobalItem[t].name
                        << ", cbaseid: " << GlobalItem[i].cbaseid << std::endl;
                }
            }
        }
    }
}
