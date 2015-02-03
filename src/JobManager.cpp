#include "JobManager.h"
#include "globalvars.h"
#include "util.h"

std::vector<MyItem> myItemList;
std::vector<ItemContainer> itemContainer;

void insertItems()
{
    for (int i = 0; i != 10; i++)
    {
        MyItem theItem;
        myItemList.push_back(theItem);
    }
}

void insertItemsIntoContainers()
{
    for (size_t i = 0; i != myItemList.size(); i++)
    {
        ItemContainer itemHolder;
        itemHolder.theItemPtr = &myItemList[i];
        itemContainer.push_back(itemHolder);
    }
}

void printItemContainer()
{
    for (size_t i = 0; i != itemContainer.size(); i++)
    {
        //std::cout << IC[i].TheItemPtr->Name << std::endl;
    }
}

void runItAll()
{
    insertItems();
    insertItemsIntoContainers();
    printItemContainer();
}

MyItem::MyItem()
{
    int random = randz(1, 2);
    if (random == 1)
        name = "Tool";
    if (random == 2)
        name = "Weapon";
}

PointerContainer::PointerContainer()
{
    pItem = nullptr;
    pNPC = nullptr;
    pTile = nullptr;
}

std::string Job::getItemName()
{
    return this->pItem->name;
}

Job::Job()
{
    pItem = nullptr;
    pWorker = nullptr;

    completionProgress = 0;
    completionTimer = 1000;

    priority = 4;
    toDelete = false;
}
