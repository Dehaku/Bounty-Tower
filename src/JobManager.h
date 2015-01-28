#ifndef __JOBMANAGER_H_INCLUDED__
#define __JOBMANAGER_H_INCLUDED__
// *#include "Faction.h"
// *#include "Items.h"
// *#include "Tiles.h"

#include "Game.h"

class NPC;
class Tile;
//class item;

//class Faction;

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
std::vector<MyItem> MyItemList;

class ItemContainer
{
public:
    MyItem *TheItemPtr;
    ItemContainer()
    {
    }
};

std::vector<ItemContainer> IC;

void InsertItems()
{
    for (int i = 0; i != 10; i++)
    {
        MyItem TheItem;
        MyItemList.push_back(TheItem);
    }
}

void InsertItemsIntoContainers()
{
    for (int i = 0; i != MyItemList.size(); i++)
    {
        ItemContainer ItemHolder;
        ItemHolder.TheItemPtr = &MyItemList[i];
        IC.push_back(ItemHolder);
    }
}

void PrintIC()
{
    for (int i = 0; i != IC.size(); i++)
    {
        //std::cout << IC[i].TheItemPtr->Name << std::endl;
    }
}

void RunItAll()
{
    InsertItems();
    InsertItemsIntoContainers();
    PrintIC();
}

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

//std::vector<Job> JobList;

//extern std::vector<Faction> UniFact;

#endif // JOBMANAGER_H_INCLUDED
