#ifndef __ITEMS_H_INCLUDED__
#define __ITEMS_H_INCLUDED__

//#include "Globals.h"
//#include "Faction.h"

//#include "FactionJobSecurity.h"
#include "Game.h"
#include "Globals.h"
#include "Images.h"

extern sf::RenderWindow App;

class NPC;
class item
{
public:
    bool ToDelete;
    bool Pickupable;
    sf::Sprite img;
    int id;
    int cbaseid;
    std::string name;
    std::string inventor;
    std::string Contains;
    std::string
        InsidePart; // This is to only be filled with a part name if it is put inside a creature, otherwise, it is to be left blank. This item is ment to be put inside a critters inventory, All inventory checks are to ignore instances where this has more than .size() = 0, Opposite is true for items contained in critter.
    int HasInternalUse;

    int range;
    int age;
    int weight;
    int value;
    sf::Vector2f TargetPos;
    float xpos;
    float ypos;
    float zpos;
    int rxpos;
    int rypos;
    int gxpos;
    int gypos;
    int imgstrx;
    int imgstry;
    int imgendx;
    int imgendy;
    bool IsWeapon;
    int type; // 0 = none, 1 = melee weapon, 2 = ranged weapon, 3 = thrown weapon, 4 = food, 5 = water, 6 = cell, 7 = Ore, 8 = Digging Tool, 9 = Cutting Tool, 10 = Fishing Tool, 11 = Liquid Container, 12 = Plant, 13 = ?
    std::string
        usedskill; // 0 = none, 1 = hand gun, 2 = rifle, 3 = slash, 4 = blunt, 5 = pierce
    float amount;
    int celltype; // 0 None 1 Chem 2 Solar 3 Nuclear 4 Soul 5 Magic
    int cellcharge;
    int cellcost;
    int tillrot;

    int hungervalue;
    int MassMetal;
    int MassFlesh;
    int MassVeggy;
    int MassGlass;
    int MassPlastic;
    int MassWater;
    int MassOil;

    int thirstvalue;
    int mindam;
    int maxdam;
    int ammotype;
    int maxclip;
    int currentclip;
    bool HasCell;
    bool UseCell;
    bool IsMagic;
    bool stacks;
    bool rotten;
    bool IsLiquidContainer;
    bool IsObjectContainer;
    bool Pushable;
    bool blocksmovement;
    bool collectssolar;
    bool collectsheat;
    bool makeslight;
    bool needspower;
    bool produces;
    int prodrate;
    int prodratetimer;
    std::string produce;
    std::string matreqprod;
    bool needmatprod;
    bool deconstrutionauto;
    int timerdecon;
    bool buried;
    bool hidden;
    bool ProdOn;
    bool IsOn;
    bool Sleepable;
    int LiquidContainMax;
    int ObjectContainMax;
    int ContainAmount;
    void chargecell(int amount)
    {
        cellcharge += amount;
    }
    void soulcell(int soultype)
    {
        chargecell(soultype * 10);
    }
    bool getid(int idz)
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
    //void useitem(){UseItem(name.c_str());}
    bool boolname(char *cakezebra)
    {
        if (cakezebra == name)
        {
            return true;
        }
        return false;
    }

    char getname()
    {
        //const char painiscupcake;
        //painiscupcake = name.c_str();
        //PaintDesktop()
        return *name.c_str();
    }

    void printInfo()
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

    //bool SetImg(int istrx,int istry,int iendx,int iendy){img.SetSubRect(sf::IntRect(istrx, istry, iendx, iendy));}

    void DrawImg()
    {
        img.setPosition(xpos, ypos);
        //img.setScale(Globals.Scalex,Globals.Scaley);
        App.draw(img);
    }

    void spawn(int gposx, int gposy, int rposx, int rposy, int posx, int posy)
    {
        gxpos = 100;
        gypos = 100;
        rxpos = 100;
        rypos = 100;
        xpos = posx;
        ypos = posy;
    }

    void PrintConsoleInfo()
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

    item()
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
    ~item()
    {
        //UnpointItem(this);
    }
};
extern item myitem;

class cItem : public item
{
public:
    cItem()
    {
    }
};

cItem *GetGlobalItem(std::string strtype);

extern std::list<item> worlditems;

class cItemManager
{
public:
    std::vector<cItem> GlobalItem;
    std::vector<item> AddedItems;

    void AddItems()
    {
        std::vector<item>::iterator Me;
        for (Me = AddedItems.begin(); Me != AddedItems.end(); Me++)
        {
            worlditems.push_back(*Me);
        }
        AddedItems.clear();
    }

    void InitializeItems()
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
                Item.produces =
                    Booleanize(StringFindNumber(line, "[produces:"));
                Item.prodrate = StringFindNumber(line, "[prodrate:");
                Item.produce = StringFindString(line, "[produce:");
                Item.mindam = StringFindNumber(line, "[mindam:");
                Item.maxdam = StringFindNumber(line, "[maxdam:");
                Item.range = StringFindNumber(line, "[range:");
                Item.IsWeapon =
                    Booleanize(StringFindNumber(line, "[IsWeapon:"));
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
                        std::cout << " \n \n cbaseid Conflict Detected between "
                                  << GlobalItem[i].name << " and "
                                  << GlobalItem[t].name
                                  << ", cbaseid: " << GlobalItem[i].cbaseid
                                  << std::endl;
                    }
                }
            }
        }
    }

    cItemManager()
    {
    }
};

extern cItemManager itemmanager;

void RemoveItems();

void zSaveItem(int planet, sf::Vector2i Region, item &object);

void SaveItem(int planet, sf::Vector2i Region, item &Critter);

std::string LoadItems(sf::Vector2i WorldPos, std::string Direction,
                      int planet = 500);

void SpawnItem(std::string Object, int xpos, int ypos);

void RefreshImages();

item *GetItemPtrfromVector(std::list<item> &Vector, std::string Name);

item *GetItemPtrfromVector2(std::list<item> &Vector, std::string Name);

item *GetItemPtrfromVectorVarSearch(std::list<item> &Vector,
                                    std::string VarSearch, float AtLeast = 1);

item *FindClosestItemPtr(int Orix, int Oriy, std::string TarItem, int Gxpos = 0,
                         int Gypos = 0, int Rxpos = 0, int Rypos = 0);

#endif // ITEMS_H_INCLUDED
