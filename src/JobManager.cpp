#include "JobManager.h"

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
