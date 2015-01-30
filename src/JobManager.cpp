#include "JobManager.h"
#include "globalvars.h"
#include "util.h"

std::vector<MyItem> MyItemList;
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
    for (size_t i = 0; i != MyItemList.size(); i++)
    {
        ItemContainer ItemHolder;
        ItemHolder.TheItemPtr = &MyItemList[i];
        IC.push_back(ItemHolder);
    }
}

void PrintIC()
{
    for (size_t i = 0; i != IC.size(); i++)
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

MyItem::MyItem()
{
    int Random = randz(1, 2);
    if (Random == 1)
        Name = "Tool";
    if (Random == 2)
        Name = "Weapon";
}

PointerContainer::PointerContainer()
{
    pItem = nullptr;
    pNPC = nullptr;
    pTile = nullptr;
}

std::string Job::GetItemName()
{
    return this->pItem->name;
}

Job::Job()
{
    pItem = nullptr;
    pWorker = nullptr;

    CompletionProgress = 0;
    CompletionTimer = 1000;

    Priority = 4;
    ToDelete = false;
}
