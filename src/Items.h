#ifndef __ITEMS_H_INCLUDED__
#define __ITEMS_H_INCLUDED__

#include <list>

#include <SFML/Graphics.hpp>
#include "Text.h"
#include "defs.h"


class Npc;

class Item
{
public:
    std::list<Item> internalitems;
    bool toDelete;
    bool pickupable;
    bool firstPickup;
    bool slotted;
    Item **currentSlot;
    sf::Sprite img;
    int id;
    int cbaseid;
    int size;
    std::string name;
    std::string inventor;
    std::string contains;
    // This is to only be filled with a part name if it is put inside
    // a creature, otherwise, it is to be left blank. This item is
    // meant to be put inside a critters inventory,
    // All inventory checks are to ignore instances where this has
    // more than .size() = 0, Opposite is true for items contained in critter.
    std::string insidePart;
    int hasInternalUse;

    Npc *user;

    void remove();

    int range;
    int radius;
    int penetration;
    int richochets;
    int age;
    int weight;
    int value;
    sf::Vector2f targetPos;
    float xpos;
    float ypos;
    float zpos;
    Vec3f getPos();
    int rxpos;
    int rypos;
    int gxpos;
    int gypos;
    int imgstrx;
    int imgstry;
    int imgendx;
    int imgendy;
    bool isWeapon;
    float activaterate;
    float activaterategrowth;
    float activateratemax;

    int projectiles;
    int spread;

    // 0 = none, 1 = melee weapon, 2 = ranged weapon,
    // 3 = thrown weapon, 4 = food, 5 = water, 6 = cell, 7 = Ore,
    // 8 = Digging Tool, 9 = Cutting Tool, 10 = Fishing Tool,
    // 11 = Liquid Container, 12 = Plant, 13 = ?
    int type;
    // 0 = none, 1 = hand gun, 2 = rifle, 3 = slash, 4 = blunt, 5 = pierce
    std::string usedskill;
    int amount;
    // 0 None 1 Chem 2 Solar 3 Nuclear 4 Soul 5 Magic
    int celltype;
    int cellcharge;
    int cellcost;
    int tillrot;

    int hungervalue;
    int massMetal;
    int massFlesh;
    int massVeggy;
    int massGlass;
    int massPlastic;
    int massWater;
    int massOil;

    int thirstvalue;
    int mindam;
    int maxdam;
    int ammotype;
    int maxclip;
    int currentclip;
    bool hasCell;
    bool useCell;
    bool isMagic;
    int stackSize;
    bool rotten;
    bool isLiquidContainer;
    bool isObjectContainer;
    bool pushable;
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
    int liquidContainMax;
    int objectContainMax;
    int containAmount;
    void chargecell(int amount);
    void soulcell(int soultype);
    bool getid(int idz);
    bool boolname(char *cakezebra);
    void printInfo();
    void drawImg();
    void spawn(int gposx, int gposy, int rposx, int rposy, int posx, int posy, int posz);
    void printConsoleInfo();
    int getRange();
    std::string activate(Vec3f vPos);
    bool trigger();
    bool isReady();
    Item();
};

extern Item * mouseItem;

Item *getGlobalItem(std::string strtype);

extern std::list<Item> worlditems;

class ItemManager
{
public:
    std::vector<Item> globalItems;
    std::vector<Item> addedItems;
    std::vector<Item> getAllofType(int type);

    void addItems();
    void initializeItems();
};

extern ItemManager itemmanager;
void removeItems(std::list<Item> &items);
void zSaveItem(int planet, sf::Vector2i region, Item &object);
void saveItem(int planet, sf::Vector2i region, Item &critter);
std::string loadItems(sf::Vector2i worldPos, std::string direction,
                      int planet = 500);
void spawnItem(std::string object, int xpos, int ypos, int zpos = 30*20);
void refreshImages();
Item *getItemPtrFromVector(std::list<Item> &vector, std::string name);
Item *getItemPtrfromVector2(std::list<Item> &vector, std::string name);
Item *getItemPtrfromVectorVarSearch(std::list<Item> &vector,
                                    std::string varSearch, float atLeast = 1);
Item *findClosestItemPtr(int orix, int oriy, std::string tarItem, int Gxpos = 0,
                         int Gypos = 0, int Rxpos = 0, int Rypos = 0);
void offloadItems();
void hoverItemIDdisplay();

extern std::list<Item> itemlist;

struct itemPtrVector
{
    std::list<Item*> ptrs;
};

itemPtrVector makeItems(std::list<Item> &items, int maxamount);

void printItems(std::list<Item> &items);

int totalDamageofItems(std::list<Item> &items);

int totalDamageofItemsInternalized(std::list<Item> &items);

Item *getItemType(std::list<Item> &inventory, int type);

itemPtrVector randomEquipment(std::list<Item> &inventory);

#endif // ITEMS_H_INCLUDED
