#include "Items.h"
#include "filesystemUtils.hpp"
#include "Globals.h"
#include "Images.h"

#include <fstream>
#include <sstream>
#include <iostream>

extern sf::RenderWindow App;

item myitem;
std::list<item> worlditems;
cItemManager itemmanager;

void RemoveItems()
{
    bool Done = false;
    while (Done == false)
    {
        bool Yet = false;
        //*for(int i = 0; i != worlditems.size(); i++)
        for (auto i = worlditems.begin(); i != worlditems.end(); i++)
        {
            if ((*i).ToDelete == true)
            {
                worlditems.erase(i);
                Yet = true;
                break;
            }
        }
        if (Yet == false)
        {
            Done = true;
        }
    }
}

void zSaveItem(int /*planet*/, sf::Vector2i Region, item &object)
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
    } // End to Load Map
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

void SaveItem(int planet, sf::Vector2i Region, item &Critter)
{ // I already did all the work once, Imma be lazy for some time.
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
        //File << std::endl;
        File << std::endl;
        File << "[name:" << Critter.name << "]"
             << "[xpos:" << Critter.xpos << "]"
             << "[ypos:" << Critter.ypos << "]";
        //Con("Added", false);
        //Con(object.name);
        File.close();
    } // End to Load Map
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
            item Critter;

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
                worlditems.push_back(Critter);
            }
        }
    }
    return line;
}

void SpawnItem(std::string Object, int xpos, int ypos)
{
    if (Debug)
    {
        std::cout << "Spawning" << Object << " \n";
    }
    cItem var;
    if (Debug)
    {
        std::cout << "Pre var in SpawnItem"
                  << " \n";
    }
    var = *GetGlobalItem(Object);
    if (Debug)
    {
        std::cout << "Post var in SpawnItem"
                  << " \n";
    }

    var.id = Globals.globalid++;
    var.xpos = xpos;
    var.ypos = ypos;
    itemmanager.AddedItems.push_back(var);
    if (Debug)
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

void RefreshImages()
{ // PANIC
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

item *GetItemPtrfromVector(std::list<item> &Vector, std::string Name)
{
    debug("Doing GetItmPtr");
    //*for(int i = 0; i != Vector.size(); i++)
    for (auto i = Vector.begin(); i != Vector.begin(); i++)
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

item *GetItemPtrfromVector2(std::list<item> &Vector, std::string Name)
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

item *GetItemPtrfromVectorVarSearch(std::list<item> &Vector,
                                    std::string VarSearch, float AtLeast)
{
    debug("Doing GetItmPtrVarSearch");
    //for(int i = 0; i != Vector.size(); i++)
    for (auto i = Vector.begin(); i != Vector.begin(); i++)
    {

        if ((*i).MassFlesh >= AtLeast && VarSearch == "MassFlesh")
        {
            debug("Returning GetItmPtrVarSearch");
            return &(*i);
        }
        if ((*i).MassVeggy >= AtLeast && VarSearch == "MassVeggy")
        {
            debug("Returning GetItmPtrVarSearch");
            return &(*i);
        }
        if ((*i).MassWater >= AtLeast && VarSearch == "MassWater")
        {
            debug("Returning GetItmPtrVarSearch");
            return &(*i);
        }
        if ((*i).MassMetal >= AtLeast && VarSearch == "MassMetal")
        {
            debug("Returning GetItmPtrVarSearch");
            return &(*i);
        }
        if ((*i).MassPlastic >= AtLeast && VarSearch == "MassPlastic")
        {
            debug("Returning GetItmPtrVarSearch");
            return &(*i);
        }
    }

    debug("GetItmPtrVarSearch didn't find anything, returning NULL");
    return nullptr;
}

item *FindClosestItemPtr(int Orix, int Oriy, std::string TarItem, int /*Gxpos*/,
                         int /*Gypos*/, int /*Rxpos*/, int /*Rypos*/)
{
    //std::vector<item>::iterator Items;
    // This will be difficult, go through all the items, and find the closest one, Silly old self, Watch how it's done.
    int closx =
        -1000000; // Make sure the default starting number is far beyond being the closest one.
    int closy = -1000000;
    bool first = true;

    item *Returns;

    for (auto i = worlditems.begin(); i != worlditems.begin(); i++)
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
                int one = Math::Closeish(Orix, Oriy, (*i).xpos, (*i).ypos);
                int two = Math::Closeish(Orix, Oriy, closx, closy);
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

void item::chargecell(int amount)
{
    cellcharge += amount;
}

void item::soulcell(int soultype)
{
    chargecell(soultype * 10);
}

bool item::getid(int idz)
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

bool item::boolname(char *cakezebra)
{
    if (cakezebra == name)
    {
        return true;
    }
    return false;
}

char item::getname()
{
    //const char painiscupcake;
    //painiscupcake = name.c_str();
    //PaintDesktop()
    return *name.c_str();
}

void item::printInfo()
{

    using namespace std;
    cout << "Name: " << name.c_str() << " (" << id << ") " << endl;
    cout << "Weight: " << weight << endl;
    cout << "Value: " << value << endl;
    if (UseCell == true)
    {
        cout << "celltype is: " << celltype << endl;
    }
    if (UseCell == true)
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
    if (IsMagic == true)
    {
        cout << " *Magical* " << endl;
    }
}

void item::DrawImg()
{
    img.setPosition(xpos, ypos);
    //img.setScale(Globals.Scalex,Globals.Scaley);
    App.draw(img);
}

void item::spawn(int /*gposx*/, int /*gposy*/, int /*rposx*/, int /*rposy*/,
                 int posx, int posy)
{
    gxpos = 100;
    gypos = 100;
    rxpos = 100;
    rypos = 100;
    xpos = posx;
    ypos = posy;
}

void item::PrintConsoleInfo()
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

item::item()
    : cbaseid{}, range{}, xpos{}, ypos{}, zpos{}, rxpos{}, rypos{}, gxpos{},
      gypos{}, imgstrx{}, imgstry{}, imgendx{}, imgendy{}, IsWeapon{},
      tillrot{}, prodrate{}, prodratetimer{}, timerdecon{}, LiquidContainMax{},
      ObjectContainMax{}, ContainAmount{}
{ // start of constructor
    ToDelete = false;
    TargetPos = sf::Vector2f(-1, -1);
    id = Globals.globalid++;
    name = "";
    weight = 0;
    value = 0;
    type = 0;
    age = 0;
    amount = 1;

    HasInternalUse =
        0; // This may be a little confusing, This is so the item can stay inside without being ejected because one function doesn't need it.

    hungervalue = 0;
    thirstvalue = 0;
    MassGlass = 0;
    MassFlesh = 0;
    MassMetal = 0;
    MassOil = 0;
    MassPlastic = 0;
    MassVeggy = 0;
    MassWater = 0;

    HasCell = false;
    UseCell = false;

    Pickupable = true;

    celltype = 0;
    cellcharge = 0;
    cellcost = 0;

    IsMagic = false;
    stacks = false;

    ammotype = 0; // 0 = none/melee, 6 = small bullet, 7 = large bullet
    maxclip = 0;
    currentclip = 0;
    mindam = 1;
    maxdam = 2;

    HasCell = false;
    UseCell = false;
    IsMagic = false;
    stacks = false;
    rotten = false;
    IsLiquidContainer = false;
    IsObjectContainer = false;
    Pushable = false;
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
    //img.SetImage(Images);
    //img.SetCenter(10,10);
}

void cItemManager::AddItems()
{
    std::vector<item>::iterator Me;
    for (Me = AddedItems.begin(); Me != AddedItems.end(); Me++)
    {
        worlditems.push_back(*Me);
    }
    AddedItems.clear();
}

void cItemManager::InitializeItems()
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
        "data/items.txt"); // TODO: Have this read from an Items folder, and read from all .txt files in it, Allowing greater compability between mods.
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

            Item.MassGlass = StringFindNumber(line, "[MassGlass:");
            Item.MassFlesh = StringFindNumber(line, "[MassFlesh:");
            Item.MassMetal = StringFindNumber(line, "[MassMetal:");
            Item.MassOil = StringFindNumber(line, "[MassOil:");
            Item.MassPlastic = StringFindNumber(line, "[MassPlastic:");
            Item.MassVeggy = StringFindNumber(line, "[MassVeggy:");
            Item.MassWater = StringFindNumber(line, "[MassWater:");

            Item.Pickupable =
                Booleanize(StringFindNumber(line, "[Pickupable:"));
            Item.type = StringFindNumber(line, "[type:");
            Item.cbaseid = StringFindNumber(line, "[baseid:");
            Item.produces = Booleanize(StringFindNumber(line, "[produces:"));
            Item.prodrate = StringFindNumber(line, "[prodrate:");
            Item.produce = StringFindString(line, "[produce:");
            Item.mindam = StringFindNumber(line, "[mindam:");
            Item.maxdam = StringFindNumber(line, "[maxdam:");
            Item.range = StringFindNumber(line, "[range:");
            Item.IsWeapon = Booleanize(StringFindNumber(line, "[IsWeapon:"));
            std::string Imagery = StringFindString(line, "[image:");
            std::vector<cImageHolder>::iterator i;
            for (i = imagemanager.GlobalImage.begin();
                 i != imagemanager.GlobalImage.end(); i++)
            {
                if (i->name == Imagery)
                {
                    Item.img.setTexture(i->Image);
                    sf::IntRect Tempz = Item.img.getTextureRect();
                    sf::Vector2f Size(Tempz.height, Tempz.width);
                    if (Debug)
                    {
                        std::cout << "TextRect: " << Tempz.height << ":"
                                  << Tempz.width << std::endl;
                    }
                    //sf::Vector2u Size = Item.img.getSize();

                    //Item.img.SetSubRect(sf::IntRect(0, 0, i->Image.GetWidth(),i->Image.GetHeight()));
                    Item.img.setOrigin(Size.x / 2, Size.y / 2);
                }
            }
            //Item.img.setTexture( *imagemanager.GetImage(Imagery) );
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
