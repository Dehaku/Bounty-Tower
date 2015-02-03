#ifndef __JOBMANAGER_H_INCLUDED__
#define __JOBMANAGER_H_INCLUDED__

#include "Items.h"

class Npc;
class Tile;

class MyItem
{
public:
    std::string name;
    MyItem();
};
extern std::vector<MyItem> myItemList;

class ItemContainer
{
public:
    MyItem *theItemPtr;
};

extern std::vector<ItemContainer> itemContainer;

void insertItems();
void insertItemsIntoContainers();
void printItemContainer();
void runItAll();

class PointerContainer
{
public:
    Item *pItem;
    Npc *pNPC;
    Tile *pTile;
    PointerContainer();
};

class Job
{
public:
    std::string name;
    int priority;
    float completionTimer;
    float completionProgress;
    std::string type;
    Item *pItem;
    Npc *pWorker;
    //Tile * pTile;
    sf::Vector2i workPos;
    sf::Vector2i workplacePos;

    bool toDelete;
    std::string getItemName();

    Job();
};

#endif // JOBMANAGER_H_INCLUDED
