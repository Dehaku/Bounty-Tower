#ifndef __JOBMANAGER_H_INCLUDED__
#define __JOBMANAGER_H_INCLUDED__

#include "Items.h"
#include "defs.h"

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

class Job
{
public:
    std::string name;
    int priority;
    float completionTimer;
    float completionProgress;
    std::string type;
    std::string errorReason;
    Item *pItem;
    Npc *pWorker;
    //Tile * pTile;
    Vec3 workPos;
    Vec3 workplacePos;

    bool toDelete;
    std::string getItemName();

    Job();
};

#endif // JOBMANAGER_H_INCLUDED
