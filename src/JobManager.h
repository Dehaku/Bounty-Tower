#ifndef __JOBMANAGER_H_INCLUDED__
#define __JOBMANAGER_H_INCLUDED__

#include "Items.h"

class NPC;
class Tile;

class MyItem
{
public:
    std::string Name;
    MyItem();
};
extern std::vector<MyItem> MyItemList;

class ItemContainer
{
public:
    MyItem *TheItemPtr;
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
    PointerContainer();
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
    std::string GetItemName();

    Job();
};

#endif // JOBMANAGER_H_INCLUDED
