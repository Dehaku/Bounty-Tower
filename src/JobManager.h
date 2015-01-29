#ifndef __JOBMANAGER_H_INCLUDED__
#define __JOBMANAGER_H_INCLUDED__
// *#include "Faction.h"
// *#include "Items.h"
// *#include "Tiles.h"

#include "Game.h"
#include "Globals.h"
#include "Items.h"

class NPC;
class Tile;

class MyItem
{
public:
    std::string Name;
    MyItem()
    {
        int Random = randz(1, 2);
        if (Random == 1)
            Name = "Tool";
        if (Random == 2)
            Name = "Weapon";
    }
};
extern std::vector<MyItem> MyItemList;

class ItemContainer
{
public:
    MyItem *TheItemPtr;
    ItemContainer()
    {
    }
};

extern std::vector<ItemContainer> IC;

void InsertItems();
void InsertItemsIntoContainers();
void PrintIC();
void RunItAll();

class PointerContainer
{
public:
    item *pItem;
    NPC *pNPC;
    Tile *pTile;
    PointerContainer()
    {
        pItem = nullptr;
        pNPC = nullptr;
        pTile = nullptr;
    }
};

class Job
{
public:
    std::string Name;
    int Priority;
    float CompletionTimer;
    float CompletionProgress;
    std::string Type;
    item *pItem;
    NPC *pWorker;
    //Tile * pTile;
    sf::Vector2i WorkPos;
    sf::Vector2i WorkplacePos;

    bool ToDelete;
    std::string GetItemName()
    {
        return this->pItem->name;
    }

    Job()
    {
        pItem = nullptr;
        pWorker = nullptr;

        CompletionProgress = 0;
        CompletionTimer = 1000;

        Priority = 4;
        ToDelete = false;
    }
};

#endif // JOBMANAGER_H_INCLUDED
