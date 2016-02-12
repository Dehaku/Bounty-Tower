#ifndef __ITEMS_H_INCLUDED__
#define __ITEMS_H_INCLUDED__

#include <list>

#include <SFML/Graphics.hpp>
#include "Text.h"
#include "defs.h"
#include "StatusEffects.h"



class Npc;

void clearBullets();

class DamageTypes
{
public:
    enum Type
    { // WARNING, THESE MUST BE IN THE SAME ORDER AS 'TypeStrings'
        None,
        Blunt,
        Pierce,
        Slash,
        Fire,
        Cold,
        Poison,
        Electric,
        Acid,
        Energy
    };
    std::vector<std::string> TypeStrings;
    int getNum(std::string name);
    std::string getString(int num);

    DamageTypes();
};
extern DamageTypes damageTypes;

class ItemTypes
{
public:
    enum Type
    { // WARNING, THESE MUST BE IN THE SAME ORDER AS 'TypeStrings'
        None,
        Melee,
        Gun,
        Bullet,
        Shell,
        Missile,
        Pickups = 12,
        Magic = 23,
        Consumable = 42,
        InventorySlotUnlocker = 69,
        PartBody = 100,
        PartBarrel,
        PartMagazine,
        PartFiringMechanism,
        PartGrip,
        PartSight,
        PartStock,
        PartAccessory

    };

    std::vector<std::string> TypeStrings;
    int getNum(std::string name);
    std::string getString(int num);

    ItemTypes();
};
extern ItemTypes itemTypes;

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
    enum fireMode
    {
        None,
        Semi,
        Burst,
        Auto
    };

    // This is to only be filled with a part name if it is put inside
    // a creature, otherwise, it is to be left blank. This item is
    // meant to be put inside a critters inventory,
    // All inventory checks are to ignore instances where this has
    // more than .size() = 0, Opposite is true for items contained in critter.
    std::string insidePart;
    int hasInternalUse;

    Npc *user;



    void remove();

    int rarity;

    int range;
    int radius;
    int penetration;
    int richochets;
    int knockback;
    int age;
    int weight;
    int value;

    int damageType;

    int healAmount;

    std::list<StatusEffect> statusEffectsCarried;
    std::list<StatusEffect> statusEffectsInflict;
    std::list<StatusEffect> statusEffects;


    std::list<StatusEffect> spells;

    void handleStatusEffects();

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


    // *---v Gun Variables
    int projectiles;
    int spread;

    int barrelCount;
    int getBarrelCount();

    float damageMultiplier;
    float getDamageMultiplier();

    float dispersion;
    float getDispersion();


    float aimTime;
    float getAimTime();
    float aimTimeCounter;

    float recoil;
    float getRecoil();
    float recoilCounter;

    float recoilReduction;
    float getRecoilReduction();

    float fireDelay;
    float getFireDelay();
    float fireDelayCounter;

    int fireMode; // Semi, Burst, Automatic.

    int burstCount;
    int getBurstCount();
    int burstCounter;

    float reloadTime;
    float getReloadTime();
    float reloadTimeCounter;

    int reloadAmount;
    int getReloadAmount();

    int ammoCapacity;
    int getAmmoCapacity();


    float bulletSpeedMultiplier;
    float getBulletSpeedMultiplier();

    float durability;
    float getDurability();
    float durabilityCounter;

    float durabilityCost;
    float getDurabilityCost();


    // *---^ Gun Variables



    int type;
    float speed;
    float getSpeed();
    // 0 = none, 1 = Melee, 2 = Gun, 3 = Ammo Bullet, 4 = Ammo Shell, 5 = Ammo Rocket, 12 = Pickupable/Misc, 42 = Consumable/Potion, 69 = InventoryUpgradeSlot,
    std::string usedskill;
    int amount;

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

    int stackSize;


    bool produces;
    int prodrate;
    int prodratetimer;
    std::string produce;

    int containAmount;
    void printInfo();
    void drawImg();
    void printConsoleInfo();
    int getRange();
    int getSpread();
    int getKnockback();
    int getMinDamage();
    int getMaxDamage();
    std::string gunThing(Vec3f vPos);
    std::string shootThing(Vec3f vPos, Item * itemptr);
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

void explosion(Vec3f vPos, int radius, float damage, Npc *attacker = nullptr, std::vector<Npc*> * victims = nullptr);

#endif // ITEMS_H_INCLUDED
