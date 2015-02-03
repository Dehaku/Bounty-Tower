#include <cstring>
#ifndef GALAXY_LINUX
#include <windows.h>
#endif
#include "Items.h"
#include "Faction.h"
#include "Tiles.h"
#include "FactionJobSecurity.h"
#include "Galaxy.h"
#include "menus.h"
#include "astar.h"
#include "Textures.h"
#include "math.h"
#include "util.h"
#include "globalvars.h"

int RemoveMe = 0;

using std::abs;

Item *listGet(std::list<Item> &list, int position)
{
    auto i = list.begin();
    std::advance(i, position);
    return &(*i);
}

//-- Prototypes
//

void drawStuffs();
int getItemVectorId(int id);

std::string getClipboardText()
{
#ifdef GALAXY_LINUX
    return "Not implemented.";
#else
    // Try opening the clipboard
    if (!OpenClipboard(NULL))
        con("ClipError1");

    // Get handle of clipboard object for ANSI text
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == NULL)
        con("ClipError1");

    // Lock the handle to get the actual text pointer
    char *pszText = static_cast<char *>(GlobalLock(hData));
    if (pszText == NULL)
        con("ClipError1");

    // Save text in a string class instance
    std::string text(pszText);
    con("Your Clipboard Contains: ", false);
    con(text);

    // Release the lock
    GlobalUnlock(hData);

    // Release the clipboard
    CloseClipboard();

    return text;
#endif
}

// Create the main rendering window
std::string randomWindowName()
{
    std::vector<std::string> const names{
        "Zombificational Settlementation Experimentation", "Galaxy",
        "Ye Old Settlement, Ye New Galaxy",
        "The first Galaxy had Joey, Just saying",
        "First comes the universe, then comes Galaxy"};

    return names[randz(0, names.size() - 1)];
}

sf::RenderWindow window;

int randzorz(int min, int max)
{
    int num = 0;
    if (min == max)
    {
        return min;
    } // It will crash if it tries to randomize the same two numbers.
    //num = sf::Randomizer::Random(min, max);
    return num;
}

void unpointItems(std::list<Item> &items)
{

    //for(int i = 0; i != Items.size(); i++)
    for (auto i = items.begin(); i != items.begin(); i++)
    {
        if ((*i).toDelete)
        {
            std::cout << "JobList size: " << uniFact[0].jobList.size()
                      << std::endl;

            for (size_t t = 0; t != uniFact[0].jobList.size(); t++)
            {
                std::cout << "Job Point: " << &uniFact[0].jobList[t].pItem
                          << "/" << uniFact[0].jobList[t].pItem
                          << ", Item Point: " << &(*i) << "/" << &(*i)
                          << std::endl;
                fSleep(1);

                if (uniFact[0].jobList[t].pItem != nullptr &&
                    (&(*i)) != nullptr)
                {

                    std::cout << "ID! ";
                    try
                    {
                        std::cout << uniFact[0].jobList[t].pItem->id;
                    }
                    catch (std::exception &e)
                    {
                        std::cout << "Something went wrong in in here\n";
                    }
                    std::cout << uniFact[0].jobList[t].pItem->id;
                    std::cout << " ; ";
                    int id1 = uniFact[0].jobList[t].pItem->id;
                    std::cout << "ID 2! ";
                    std::cout << (*i).id;
                    std::cout << " ; ";
                    int id2 = (*i).id;
                    std::cout << "ID's: " << id1 << ":" << id2 << std::endl;

                    if (id1 ==
                        id2) //if(UniFact[0].JobList[t].pItem != NULL && &(*i) != NULL && UniFact[0].JobList[t].pItem == &(*i)) //if(ID1 == ID2)
                    {
                        std::cout << "Match! \n";
                        uniFact[0].jobList[t].pItem = nullptr;
                        //fSleep(1);
                    }
                }
            }
        }
    }
}

bool removeItem(int id)
{
    int tempInt = 0;
    std::list<Item>::iterator location;
    for (auto it = worlditems.begin(); it != worlditems.end(); ++it)
    {
        if (it->id == id)
        {
            location = it;
            tempInt = 1;
        }
    }
    if (tempInt == 1)
    {
        worlditems.erase(location);
        return true;
    }
    else if (tempInt == 0)
    {
        return false;
    }
    throw std::runtime_error("Couldn't return anything sensible");
}

void updateItem()
{
    if (gvars::debug)
    {
        std::cout << "Pre Item \n";
    }

    for (auto &item : worlditems)
    {
        try
        {
            if (gvars::debug)
            {
                std::cout << item.name << "'s turn! \n";
            }
            if (item.produces == true)
            {
                if (gvars::debug)
                {
                    std::cout << item.name << " can produce. \n";
                }
                item.prodratetimer++;
                if (item.prodratetimer >= item.prodrate)
                {
                    item.prodratetimer = 0;
                    std::string s;
                    s = item.produce;
                    auto a = new char[s.size() + 1];
                    a[s.size()] = 0;
                    memcpy(a, s.c_str(), s.size());
                    bool findEmpty = false;
                    int x;
                    int y;
                    while (findEmpty == false)
                    {
                        x = randz(item.xpos - 10, item.xpos + 10);
                        y = randz(item.ypos - 10, item.ypos + 10);
                        if (tiles[abs_to_index(x / GridSize)][abs_to_index(
                                y / GridSize)][30].walkable == true)
                        {
                            findEmpty = true;
                        }
                    }
                    if (gvars::debug)
                    {
                        std::cout << "Producing: " << s << " \n";
                    }
                    spawnItem(s, x, y);
                    if (gvars::debug)
                    {
                        std::cout << item.name << " has produced: " << s
                                  << " \n";
                    }
                }
            }
            if (gvars::debug)
            {
                std::cout << "Done with: " << item.name << " \n";
            }

            if (gvars::debug)
            {
                std::cout << "Acting on Missile \n";
            }
            if (item.targetPos != sf::Vector2f(-1, -1))
            {
                item.xpos = (item.xpos + item.targetPos.x) / 2;
                item.ypos = (item.ypos + item.targetPos.y) / 2;
                if (item.age >= 10)
                {
                    item.toDelete = true;
                    boom(item.xpos, item.ypos, randz(item.mindam, item.maxdam),
                         item.range);

                    for (int itLength = 0; itLength != 5; itLength++)
                    {
                        for (int rot = 1; rot != 361; rot++)
                        {
                            //int Rot = GX;
                            int XPos = ((abs(item.xpos / 20)) +
                                        cosf(rot * PI / 180) * itLength);
                            int YPos = ((abs(item.ypos / 20)) +
                                        sinf(rot * PI / 180) * itLength);
                            //XPos *= 20;
                            //YPos *= 20;

                            //Effectz.CreateCircle(MousePos.x,MousePos.y,5,Red);
                            //Effectz.CreateCircle(XPos,YPos,5,White);
                            tiles[XPos][YPos][30].stone();
                        }
                    }

                    con("Boom!");
                }
            }
            item.age = item.age + 1;
            if (gvars::debug)
            {
                std::cout << "Post Item \n";
            }
        }
        catch (std::exception &e)
        {
            std::cout << "Problem with an Item in Update Item \n";
        }
    }
}

std::vector<int> nngTrace(int xa, int ya, int xb, int yb, int id,
                          std::vector<int> /*exceptions*/)
{ //.at(0) = Item/NPC(23/69) .at(1) = id
    int dx = xb - xa, dy = yb - ya, steps;
    float xIncrement, yIncrement, x = xa, y = ya;
    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);
    xIncrement = dx / (float)steps;
    yIncrement = dy / (float)steps;
    std::vector<int> vectorID;
    for (int k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        if (key.g)
        {
            effects.createCircle(x, y, 1, sf::Color::Blue);
        }
        int count = 0;

        { // Merely doing this so I can reuse the same code, but for items, Hehe.
            std::vector<Npc>::iterator me;

            for (me = npclist.begin(); me != npclist.end(); ++me)
            {
                if (math::closeish(x, y, me->xpos, me->ypos) <= me->reach &&
                    me->id != id)
                {

                    std::vector<int>::iterator vec;
                    try
                    {
                        bool exists = false;
                        for (vec = vectorID.begin(); vec != vectorID.end();
                             ++vec)
                        {
                            if (*vec == me->id)
                            {
                                exists = true;
                            }
                        }
                        if (exists == false)
                        {
                            vectorID.push_back(69);
                            vectorID.push_back(getNpcVectorId(me->id));
                        }
                    }
                    catch (std::exception &e)
                    {
                        std::cout << "VectError\n";
                    }

                } //23 = Item, 69 = NPC
                count++;
            }
        }
        std::list<Item>::iterator me;

        for (me = worlditems.begin(); me != worlditems.end(); ++me)
        {
            if (math::closeish(x, y, me->xpos, me->ypos) <= 10 && me->id != id)
            {
                //Making sure not to constantly add the same
                //try{if( VectorID.at(Count-1) != Me->id){ VectorID.push_back(23); VectorID.push_back( GetItemVectorId(Me->id)); }} catch (std::exception& e){}

            } //23 = Item, 69 = NPC
            count++;
        }

        if (key.period)
        {
            effects.createCircle(x, y, 1, sf::Color::White);
        }
    }
    if (vectorID.size() == 0)
    {
        vectorID.push_back(-1);
    }
    return vectorID;
}

bool gridposTrace(int xa, int ya, int xb, int yb, int id, sf::Vector2f target)
{ // Looking in a straight line for a specific spot, Walls block vision.

    int dx = xb - xa, dy = yb - ya, steps;
    float xIncrement, yIncrement, x = xa, y = ya;
    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);
    xIncrement = dx / (float)steps;
    yIncrement = dy / (float)steps;

    for (int k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        if (tiles[abs_to_index(x / GridSize)][abs_to_index(y / GridSize)][30]
                .id == 1010)
        {
            if (key.period && id == gvars::myTargetid)
            {
                effects.createLine(x, y, xa, ya, 1, sf::Color::Blue);
            }
            //std::cout << "Shoulda Broke. " << std::endl;
            break;

        } // Stops the trace if it hits a wall.
        //std::cout << "Stuffs " << std::endl;
        sf::Vector2f pos(abs(x / GridSize), abs(y / GridSize));
        sf::Vector2f tar(abs(target.x / GridSize), abs(target.y / GridSize));
        if (pos == tar)
        {
            return true;
        } // Returns true and stops searching.
        if (key.period && id == gvars::myTargetid)
        {
            effects.createLine(x, y, xa, ya, 1, sf::Color::Blue);
        }
    }
    return false; // Returns false if the target was never found.
}

std::vector<int> npcTrace(int xa, int ya, int xb, int yb, int id,
                          std::vector<int> /*exceptions = std::vector<int>()*/)
{

    int dx = xb - xa, dy = yb - ya, steps;
    float xIncrement, yIncrement, x = xa, y = ya;
    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);
    xIncrement = dx / (float)steps;
    yIncrement = dy / (float)steps;
    std::vector<int> vectorID;

    for (int k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        bool kill = false;
        std::vector<Npc>::iterator me;
        int count = 0;
        for (me = npclist.begin(); me != npclist.end(); ++me)
        {
            if (math::closeish(x, y, me->xpos, me->ypos) <= me->reach &&
                me->id != id)
            {
                vectorID.push_back(count);
                kill = true;
            }
            count++;
        }
        if (kill)
            return vectorID;

        if (key.h)
        {
            effects.createCircle(x, y, 1, sf::Color::White);
        }
    }
    if (vectorID.size() == 0)
    {
        vectorID.push_back(-1);
    }

    throw std::runtime_error("npcTrace: couldn't return a value");
}

int getItemVectorId(int id)
{
    int index = 0;
    for (auto const &item : worlditems)
    {
        if (item.id == id)
        {
            return index;
        }
        index++;
    }
    return -1;
}

int getNpcVectorId(int id)
{
    int index = 0;
    for (auto const &npc : npclist)
    {
        if (npc.id == id)
        {
            return index;
        }
        index++;
    }
    return -1;
}

bool removeNPC(char * /*NPCname*/, int /*Id*/)
{
    int tempInt = 0;
    std::vector<Npc>::iterator location;
    for (auto it = npclist.begin(); it != npclist.end(); ++it)
    {
        if (it->health <= 0 || it->hasSpawned == false || it->alive == false)
        {
            location = it;
            tempInt = 1;
        }
    }
    if (tempInt == 1)
    {
        npclist.erase(location);
        return true;
    }
    else if (tempInt == 0)
    {
        return false;
    }
    throw std::runtime_error("RemoveNPC: Couldn't return anything sensible.");
}

struct ItemFindResult
{
    bool found;
    int xpos;
    int ypos;
    int id;
    int vectorPosition;
};

ItemFindResult findClosestItem(int orix, int oriy, std::string tarItem,
                               int /*Gxpos*/ = 0, int /*Gypos*/ = 0,
                               int /*Rxpos*/ = 0, int /*Rypos*/ = 0)
{
    // This will be difficult, go through all the items, and find the closest one.
    int closx =
        -1000000; // Make sure the default starting number is far beyond being the closest one.
    int closy = -1000000;
    int closid;
    int closVect;
    int vectPos = 0;
    bool first = true;
    for (auto const &item : worlditems)
    {
        if (first == true)
        {
            if (item.name == tarItem)
            {
                closx = item.xpos;
                closy = item.ypos;
                closid = item.id;
                closVect = vectPos;
                first = false;
            }
        }
        else
        {
            if (item.name == tarItem)
            {
                int one = math::closeish(orix, oriy, item.xpos, item.ypos);
                int two = math::closeish(orix, oriy, closx, closy);
                if (one < two)
                {
                    closx = item.xpos;
                    closy = item.ypos;
                    closid = item.id;
                    closVect = vectPos;
                }
            }
        }
        vectPos++;
    }
    if (first != true)
    {
        return {true, closx, closy, closid, closVect};
    }
    return {false, 0, 0, 0, 0};
}

std::set<int> npcList(int exceptions = -1)
{
    if (gvars::debug)
    {
        std::cout << "Pre npcList \n";
    }
    std::set<int> Returns;

    for (auto const &npc : npclist)
    {
        if (gvars::debug)
        {
            std::cout << "For NpcList \n";
        }
        if (npc.id != exceptions)
        {
            if (gvars::debug)
            {
                std::cout << "Post exception NpcList \n";
            }
            Returns.insert(getNpcVectorId(npc.id));
            if (gvars::debug)
            {
                std::cout << "Post Returns NpcList \n";
            }
        }
    }
    if (gvars::debug)
    {
        std::cout << "Post For NpcList \n";
    }
    if (!Returns.empty())
    {
        return Returns;
    }
    throw std::runtime_error("NpcList: Couldn't return anything!");
}

void critterBrain(std::vector<Npc> &NPCs)
{
    for (auto &npc : NPCs)
    {

        /*  BodyPart Loop
            First, Run through the bodyparts finding the 'global' tags, like
                Nutrient Extraction and such.
            Second, Run through each individual part running through all
                the local tags.
        */

        short int Parts = 0;
        size_t SearchPos = 0;
        size_t EndPos = 0;

        debug("Debug: Beginning Part Loop for" + npc.name);

        //  Global Part Tag Variables

        float PartsWalkSpeed = 0;
        float GlobalNutritionPercentage = 100;
        bool ConsumeFlesh = false;
        bool ConsumeVeggy = false;
        bool ConsumeWater = false;

        //  *   Global Part Tag Variables   *

        while (SearchPos != npc.body.bodyParts.npos) // Global Part Tags
        {
            SearchPos = npc.body.bodyParts.find("{", SearchPos);

            if (SearchPos != npc.body.bodyParts.npos)
            {
                EndPos = npc.body.bodyParts.find("}", SearchPos);

                std::string WorkingLine;

                WorkingLine.append(npc.body.bodyParts, SearchPos,
                                   EndPos - SearchPos);
                float PartNumber = 0;

                PartNumber = stringFindNumber(WorkingLine, "[Walk:");
                if (PartNumber != 0)
                {

                    if (PartsWalkSpeed != 0)
                    {
                        PartsWalkSpeed = PartNumber;
                    }
                    else
                    {
                        PartsWalkSpeed += (PartNumber * 0.5);
                    }
                }

                PartNumber =
                    stringFindNumber(WorkingLine, "[NutritionExtraction:");
                if (PartNumber != 0)
                {
                    GlobalNutritionPercentage += PartNumber;
                }

                PartNumber = stringFindNumber(WorkingLine, "[DigestsBlood:");
                if (PartNumber != 0)
                {
                }
                PartNumber = stringFindNumber(WorkingLine, "[DigestsFlesh:");
                if (PartNumber != 0)
                {
                    ConsumeFlesh = true;
                }
                PartNumber = stringFindNumber(WorkingLine, "[DigestsVeggy:");
                if (PartNumber != 0)
                {
                    ConsumeVeggy = true;
                }
                PartNumber = stringFindNumber(WorkingLine, "[DigestsWater:");
                if (PartNumber != 0)
                {
                    ConsumeWater = true;
                }

                SearchPos = EndPos;
            }
        }

        SearchPos = 0;

        for (auto i = npc.inventory.begin(); i != npc.inventory.end(); i++)
        {
            if ((*i).insidePart != "")
            {
                (*i).hasInternalUse--;
            }
        }

        while (SearchPos != npc.body.bodyParts.npos) // Individual Part Tags
        {

            SearchPos = npc.body.bodyParts.find("{", SearchPos);

            if (SearchPos != npc.body.bodyParts.npos)
            {
                EndPos = npc.body.bodyParts.find("}", SearchPos);
                Parts++;

                std::string WorkingLine;

                WorkingLine.append(npc.body.bodyParts, SearchPos,
                                   EndPos - SearchPos);

                float PartNumber = 0;
                std::string PartString = "";
                Item *PartItem;

                std::string CurrentPart =
                    stringFindString(WorkingLine, "[Name:");

                PartNumber = stringFindNumber(WorkingLine, "[DigestsBlood:");
                PartItem = getItemPtrFromVector(npc.inventory, "Blood");
                if (PartNumber != 0 && PartItem != nullptr)
                {

                    float WorkAmount = PartItem->amount;
                    float Diff = WorkAmount - (PartNumber / 1000);

                    if (Diff > 0)
                    {
                        PartItem->amount = Diff;
                        float Nutr = (WorkAmount - Diff) *
                                     100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Nutrients",
                            Nutr * percentageBuff(GlobalNutritionPercentage));
                    }
                    else
                    {
                        //*GetItemPtrfromVector(npc.inventory,"Blood").amount = 0;
                        getItemPtrFromVector(npc.inventory, "Blood")->toDelete =
                            true;
                        float Nutr =
                            WorkAmount * 100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Nutrients",
                            Nutr * percentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartNumber = stringFindNumber(WorkingLine, "[DigestsFlesh:");
                PartItem =
                    getItemPtrfromVectorVarSearch(npc.inventory, "MassFlesh");
                //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
                if (PartNumber != 0 && PartItem != nullptr &&
                    PartItem->massFlesh >
                        0) // TODO: Make sure the item is in THIS PART before digesting it!
                {
                    //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                    float WorkAmount = PartItem->massFlesh;
                    float Diff = WorkAmount - (PartNumber / 1000);

                    if (Diff > 0)
                    {
                        PartItem->massFlesh = Diff;
                        PartItem->hasInternalUse = 0;
                        float Nutr = (WorkAmount - Diff) *
                                     100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Nutrients",
                            Nutr * percentageBuff(GlobalNutritionPercentage));
                    }
                    if (PartItem->massFlesh <= 0)
                    {
                        //*GetItemPtrfromVector(npc.inventory,"Blood").amount = 0;
                        PartItem->toDelete = true;
                        //Add Food to everyone, Make sure they go hungry to eat it, Figure out a way to Eject the empty item, Or do water! Everyone starts with water.
                        //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartNumber = stringFindNumber(WorkingLine, "[DigestsVeggy:");
                PartItem =
                    getItemPtrfromVectorVarSearch(npc.inventory, "MassVeggy");
                //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
                if (PartNumber != 0 && PartItem != nullptr &&
                    PartItem->massVeggy >
                        0) // TODO: Make sure the item is in THIS PART before digesting it!
                {
                    //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                    float WorkAmount = PartItem->massVeggy;
                    float Diff = WorkAmount - (PartNumber / 1000);

                    if (Diff > 0)
                    {
                        PartItem->massVeggy = Diff;
                        PartItem->hasInternalUse = 0;
                        float Nutr = (WorkAmount - Diff) *
                                     100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Nutrients",
                            Nutr * percentageBuff(GlobalNutritionPercentage));
                    }
                    if (PartItem->massVeggy <= 0)
                    {
                        PartItem->toDelete = true;
                        //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartNumber = stringFindNumber(WorkingLine, "[DigestsWater:");
                PartItem =
                    getItemPtrfromVectorVarSearch(npc.inventory, "MassWater");
                //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
                if (PartNumber != 0 && PartItem != nullptr &&
                    PartItem->massWater >
                        0) // TODO: Make sure the item is in THIS PART before digesting it!
                {
                    //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                    float WorkAmount = PartItem->massWater;
                    float Diff = WorkAmount - (PartNumber / 1000);

                    if (Diff > 0)
                    {
                        PartItem->massWater = Diff;
                        PartItem->hasInternalUse = 0;
                        float Nutr = (WorkAmount - Diff) *
                                     100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Hydration",
                            Nutr * percentageBuff(GlobalNutritionPercentage));
                    }
                    if (PartItem->massWater <= 0)
                    {
                        PartItem->toDelete = true;
                        //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartString = stringFindString(WorkingLine, "[PoisonFilter:");
                if (PartString != "")
                {

                    std::vector<std::string> StrVec =
                        stringFindElements(PartString, ":");
                    if (gvars::debug)
                        std::cout << "StrVec[0]: " << StrVec[0] << std::endl;
                    float Leftover =
                        npc.bloodwork(StrVec[0], -atof(StrVec[1].c_str()));
                    if (gvars::debug)
                        std::cout << "Bloodwork leftover is: " << Leftover
                                  << std::endl;
                    //NPC Critter;

                    for (size_t i = 0; i != StrVec.size(); i++)
                    {
                        if (gvars::debug)
                            std::cout << StrVec[i] << std::endl;
                    }
                }

                PartNumber = stringFindNumber(WorkingLine, "[Orafice:");
                if (PartNumber > 0)
                {
                    //std::vector<item> * Inv = &npc.inventory;

                    //for(int i = 0; i != npc.inventory.size(); i++)
                    for (auto i = npc.inventory.begin();
                         i != npc.inventory.end(); i++)
                    {
                        bool FoundIt = false;
                        if ((*i).insidePart == "" && (*i).massFlesh > 0 &&
                            ConsumeFlesh) // Awww yeeessss, We gonna eat some flesh with our Orafice!
                        {
                            (*i).insidePart = CurrentPart;
                            FoundIt = true;
                        }

                        if ((*i).insidePart == "" && (*i).massVeggy > 0 &&
                            ConsumeVeggy) // Awww yeeessss, We gonna eat something with our Orafice!
                        {
                            (*i).insidePart = CurrentPart;
                            FoundIt = true;
                        }

                        if ((*i).insidePart == "" && (*i).massWater > 0 &&
                            ConsumeWater) // Awww yeeessss, We gonna eat something with our Orafice!
                        {
                            (*i).insidePart = CurrentPart;
                            FoundIt = true;
                        }

                        if (FoundIt)
                        {
                            std::string ChtStr;
                            ChtStr.append("* ");
                            ChtStr.append(npc.name);
                            ChtStr.append("(" + std::to_string(npc.id) + ")");
                            ChtStr.append(" has inserted ");
                            ChtStr.append((*i).name);
                            ChtStr.append(" into their ");
                            ChtStr.append(CurrentPart);
                            ChtStr.append("'s Orafice(");
                            ChtStr.append(std::to_string(PartNumber));
                            ChtStr.append(").");

                            chatBox.addChat(ChtStr, sf::Color(150, 150, 0));
                        }
                    }
                }

                PartNumber = stringFindNumber(
                    WorkingLine, "[BloodPumpRate:"); // TODO: Do this right.
                if (PartNumber != 0)
                {
                    float Blood =
                        stringFindNumber(npc.bloodcontent, "[Nutrients:");
                    if (Blood > PartNumber)
                    {
                        if ((npc.maxhunger - npc.hunger) > PartNumber)
                        {
                            npc.hunger += PartNumber;
                            npc.bloodwork("Nutrients", -PartNumber);
                        }
                    }
                    Blood = stringFindNumber(npc.bloodcontent, "[Hydration:");
                    if (Blood > PartNumber)
                    {
                        if ((npc.maxthirst - npc.thirst) > PartNumber)
                        {
                            npc.thirst += PartNumber;
                            npc.bloodwork("Hydration", -PartNumber);
                        }
                    }
                }
                SearchPos = EndPos;
            }
        }

        /*Simulating Hunger/Thirst, Needs to be nerfed/formulated to conditions, I.E. Attributes/Parts/Weather*/
        npc.bloodwork("Nutrients", -1);
        npc.bloodwork("Hydration", -1);

        debug("Debug: Ending Part Loop");
        /* *BodyPart Loop* */
        /* Critter Vision   */
        const sf::Vector2f npcPos(npc.xpos, npc.ypos);
        npc.angle = math::angleBetweenVectors(npcPos, gvars::mousePos) - 90;

        int EndAngle = -(npc.angle - (npc.viewangle / 2));
        int StartAngle = -(npc.angle - (-npc.viewangle / 2));

        sf::ConvexShape Shape;
        int PointCounter = 1;
        Shape.setPointCount(PointCounter);
        Shape.setPoint(0, sf::Vector2f(0, 0));

        for (int Rot = StartAngle; Rot != EndAngle; Rot++)
        {
            float XPos = npc.xpos + sinf(Rot * PI / 180) * npc.viewrange;
            float YPos = npc.ypos + cosf(Rot * PI / 180) * npc.viewrange;

            if (Rot == StartAngle)
            {
                PointCounter++;
                Shape.setPointCount(PointCounter);
                Shape.setPoint(PointCounter - 1,
                               sf::Vector2f(XPos - npc.xpos, YPos - npc.ypos));
            }

            int CenterPoint = (StartAngle + EndAngle) / 2;
            if (Rot == CenterPoint)
            {
                PointCounter++;
                Shape.setPointCount(PointCounter);
                Shape.setPoint(PointCounter - 1,
                               sf::Vector2f(XPos - npc.xpos, YPos - npc.ypos));
                effects.createLine(npc.xpos, npc.ypos, XPos, YPos, 1,
                                   sf::Color::Cyan);
            }
            if ((Rot % 10) == 0 && Rot != 0)
            {
                PointCounter++;
                Shape.setPointCount(PointCounter);
                Shape.setPoint(PointCounter - 1,
                               sf::Vector2f(XPos - npc.xpos, YPos - npc.ypos));
            }
            if (Rot == EndAngle - 1)
            {
                PointCounter++;
                Shape.setPointCount(PointCounter);
                Shape.setPoint(PointCounter - 1,
                               sf::Vector2f(XPos - npc.xpos, YPos - npc.ypos));
            }
        }

        sf::Color Filling(sf::Color::Yellow);
        Filling.a = (100);
        Shape.setFillColor(Filling);
        Shape.setOutlineColor(sf::Color::Yellow);
        Shape.setOutlineThickness(1);
        Shape.setPosition(npcPos);
        effects.polygons.push_back(Shape);

        /* Critter Prioritization */
        // Method Two, Struct Desires
        struct Desire
        {
            std::string DesireType;
            float Potency;
        };
        std::vector<Desire> Desires;

        // Declaring and adding Desires
        Desire NewDesire;
        { //Sustainence
            NewDesire.DesireType = "Sustainence";
            NewDesire.Potency = 0;
        }
        Desires.push_back(NewDesire);
        { //Apathy
            NewDesire.DesireType = "Apathy";
            NewDesire.Potency = 100;
        }
        Desires.push_back(NewDesire);
        { //SelfDefense
            NewDesire.DesireType = "SelfDefense";
            NewDesire.Potency = 0;
        }
        Desires.push_back(NewDesire);
        { //Social
            NewDesire.DesireType = "Social";
            NewDesire.Potency = 0;
        }
        Desires.push_back(NewDesire);
        { //Work
            NewDesire.DesireType = "Work";
            NewDesire.Potency = 0;
        }
        Desires.push_back(NewDesire);

        /*Causation to Desires*/
        // Get Critters max nutrition, then reduce it by critters nutrients in blood
        float Nutrients = npc.maxhunger - npc.bloodwork("Nutrients", 0);
        float Hydration = npc.maxthirst - npc.bloodwork("Hydration", 0);

        for (auto &Sus : Desires)
        {
            if (Sus.DesireType == "Sustainence")
                Sus.Potency += Hydration + Nutrients;
            if (Sus.DesireType == "SelfDefense")
            {
                // This line makes the game freeze
                Sus.Potency = 10000;
            }
        }

        // Finding the highest Desire
        bool InComplete;
        Desire *HighestDesire = nullptr;
        bool FirstIter = true;

    ReDesire:
        InComplete = false;

        for (auto &i : Desires)
        {
            if (FirstIter)
            {
                HighestDesire = &i;
                FirstIter = false;
            }
            if (i.Potency > (*HighestDesire).Potency)
                HighestDesire = &i;
        }

        if (HighestDesire == nullptr)
        {
            throw std::runtime_error("critterBrain: Something went wrong!");
        }

        // Acting on Highest Desire
        if ((*HighestDesire).DesireType == "Apathy")
        {
            effects.createCircle(npc.xpos, npc.ypos, 11, sf::Color::Red);
        }
        if ((*HighestDesire).DesireType == "SelfDefense")
        {
            (*HighestDesire).Potency = 0;
            InComplete = true;
        }
        if ((*HighestDesire).DesireType == "Sustainence")
        {
            bool InInv = false;
            for (auto &Inv : npc.inventory)
                if (Inv.massFlesh > 0)
                {
                    //npc.Target.item = &Inv;
                    InInv = true;
                    break;
                }
            if (InInv == false)
            {
                for (auto &LclItms : worlditems)
                {
                    if (npc.targetInfo.item == nullptr &&
                        LclItms.massFlesh > 0 && LclItms.user == nullptr)
                    {
                        npc.targetInfo.item =
                            &LclItms; // Is this creating a copy? The Behavior in testing still makes multiple critters target the same item.
                        LclItms.user = &npc;
                    }
                    else if (LclItms.massFlesh > 0 && LclItms.user == nullptr)
                    {

                        float CurrentItem = math::closeish(
                            npc.xpos, npc.ypos, (*npc.targetInfo.item).xpos,
                            (*npc.targetInfo.item).ypos);
                        float NewItem = math::closeish(
                            npc.xpos, npc.ypos, LclItms.xpos, LclItms.ypos);
                        if (NewItem < CurrentItem)
                        {
                            (*npc.targetInfo.item).user = nullptr;
                            npc.targetInfo.item = &LclItms;
                            (*npc.targetInfo.item).user = &npc;
                        }
                    }
                }
                if (npc.targetInfo.item != nullptr)
                {
                    sf::Vector2f ItemPos((*npc.targetInfo.item).xpos,
                                         (*npc.targetInfo.item).ypos);
                    npc.dirMove(sf::Vector2f((*npc.targetInfo.item).xpos,
                                             (*npc.targetInfo.item).ypos));

                    if (math::closeish(npc.xpos, npc.ypos, ItemPos.x,
                                       ItemPos.y) <= npc.size * 2)
                    {
                        Item *Tar = npc.targetInfo.item;
                        npc.targetInfo.item = nullptr;
                        (*Tar).user = nullptr;
                        npc.inventory.push_back((*Tar));
                        (*Tar).toDelete = true;
                    }
                }
            }

            if (npc.targetInfo.item != nullptr && InInv == false)
            {
                effects.createLine(
                    npc.xpos, npc.ypos, (*npc.targetInfo.item).xpos,
                    (*npc.targetInfo.item).ypos, 2, sf::Color::White);
            }
        }

        // Incase the highest desire isn't completable, Go through again for the next highest desire.
        if (InComplete)
            goto ReDesire;

        textList.createText(npc.xpos - 30, npc.ypos - 15, 10, sf::Color::Red,
                            (*HighestDesire).DesireType, ":",
                            (*HighestDesire).Potency);
        textList.createText(npc.xpos - 70, npc.ypos - 35, 10, sf::Color::Cyan,
                            npc.body.bodyParts);

        /* End of Critter Prioritization */

        removeItems(npc.inventory);
    }

    if (key.oTime == 1)
        RemoveMe++;
    if (key.lTime == 1)
        RemoveMe--;
}

void updateNpc()
{
    if (gvars::debug)
    {
        std::cout << "Pre NPC\n";
    }
    int IntegerIterator = 0;
    for (auto &npc : npclist)
    {
        // BodyPart Loop
        // First, Run through the bodyparts finding the 'global' tags, like Nutrient Extraction and such.
        // Second, Run through each individual part running through all the local tags.

        short int Parts = 0;
        size_t SearchPos = 0;
        size_t EndPos = 0;

        debug("Debug: Beginning Part Loop for" + npc.name);

        //  Global Part Tag Variables

        float PartsWalkSpeed = 0;
        float GlobalNutritionPercentage = 100;
        bool ConsumeFlesh = false;
        bool ConsumeVeggy = false;
        bool ConsumeWater = false;

        //  *   Global Part Tag Variables   *

        while (SearchPos != npc.body.bodyParts.npos) // Global Part Tags
        {
            SearchPos = npc.body.bodyParts.find("{", SearchPos);

            if (SearchPos != npc.body.bodyParts.npos)
            {
                EndPos = npc.body.bodyParts.find("}", SearchPos);

                std::string WorkingLine;

                WorkingLine.append(npc.body.bodyParts, SearchPos,
                                   EndPos - SearchPos);
                float PartNumber = 0;

                PartNumber = stringFindNumber(WorkingLine, "[Walk:");
                if (PartNumber != 0)
                {

                    if (PartsWalkSpeed != 0)
                    {
                        PartsWalkSpeed = PartNumber;
                    }
                    else
                    {
                        PartsWalkSpeed += (PartNumber * 0.5);
                    }
                }

                PartNumber =
                    stringFindNumber(WorkingLine, "[NutritionExtraction:");
                if (PartNumber != 0)
                {
                    GlobalNutritionPercentage += PartNumber;
                }

                PartNumber = stringFindNumber(WorkingLine, "[DigestsBlood:");
                if (PartNumber != 0)
                {
                }
                PartNumber = stringFindNumber(WorkingLine, "[DigestsFlesh:");
                if (PartNumber != 0)
                {
                    ConsumeFlesh = true;
                }
                PartNumber = stringFindNumber(WorkingLine, "[DigestsVeggy:");
                if (PartNumber != 0)
                {
                    ConsumeVeggy = true;
                }
                PartNumber = stringFindNumber(WorkingLine, "[DigestsWater:");
                if (PartNumber != 0)
                {
                    ConsumeWater = true;
                }

                SearchPos = EndPos;
            }
        }

        SearchPos = 0;

        //for(int i = 0; i != npc.inventory.size(); i++)
        for (auto i = npc.inventory.begin(); i != npc.inventory.begin(); i++)
        {
            if ((*i).insidePart != "")
            {
                (*i).hasInternalUse--;
            }
        }

        while (SearchPos != npc.body.bodyParts.npos) // Individual Part Tags
        {

            SearchPos = npc.body.bodyParts.find("{", SearchPos);

            if (SearchPos != npc.body.bodyParts.npos)
            {
                EndPos = npc.body.bodyParts.find("}", SearchPos);
                Parts++;

                std::string WorkingLine;

                WorkingLine.append(npc.body.bodyParts, SearchPos,
                                   EndPos - SearchPos);

                float PartNumber = 0;
                std::string PartString = "";
                Item *PartItem;

                std::string CurrentPart =
                    stringFindString(WorkingLine, "[Name:");

                PartNumber = stringFindNumber(WorkingLine, "[DigestsBlood:");
                PartItem = getItemPtrFromVector(npc.inventory, "Blood");
                if (PartNumber != 0 && PartItem != nullptr)
                {

                    float WorkAmount = PartItem->amount;
                    float Diff = WorkAmount - (PartNumber / 1000);

                    if (Diff > 0)
                    {
                        PartItem->amount = Diff;
                        float Nutr = (WorkAmount - Diff) *
                                     100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Nutrients",
                            Nutr * percentageBuff(GlobalNutritionPercentage));
                    }
                    else
                    {
                        //*GetItemPtrfromVector(npc.inventory,"Blood").amount = 0;
                        getItemPtrFromVector(npc.inventory, "Blood")->toDelete =
                            true;
                        float Nutr =
                            WorkAmount * 100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Nutrients",
                            Nutr * percentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartNumber = stringFindNumber(WorkingLine, "[DigestsFlesh:");
                PartItem =
                    getItemPtrfromVectorVarSearch(npc.inventory, "MassFlesh");
                //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
                if (PartNumber != 0 && PartItem != nullptr &&
                    PartItem->massFlesh >
                        0) // TODO: Make sure the item is in THIS PART before digesting it!
                {
                    //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                    float WorkAmount = PartItem->massFlesh;
                    float Diff = WorkAmount - (PartNumber / 1000);

                    if (Diff > 0)
                    {
                        PartItem->massFlesh = Diff;
                        PartItem->hasInternalUse = 0;
                        float Nutr = (WorkAmount - Diff) *
                                     100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Nutrients",
                            Nutr * percentageBuff(GlobalNutritionPercentage));
                    }
                    if (PartItem->massFlesh <= 0)
                    {
                        //*GetItemPtrfromVector(npc.inventory,"Blood").amount = 0;
                        PartItem->toDelete = true;
                        //Add Food to everyone, Make sure they go hungry to eat it, Figure out a way to Eject the empty item, Or do water! Everyone starts with water.
                        //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartNumber = stringFindNumber(WorkingLine, "[DigestsVeggy:");
                PartItem =
                    getItemPtrfromVectorVarSearch(npc.inventory, "MassVeggy");
                //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
                if (PartNumber != 0 && PartItem != nullptr &&
                    PartItem->massVeggy >
                        0) // TODO: Make sure the item is in THIS PART before digesting it!
                {
                    //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                    float WorkAmount = PartItem->massVeggy;
                    float Diff = WorkAmount - (PartNumber / 1000);

                    if (Diff > 0)
                    {
                        PartItem->massVeggy = Diff;
                        PartItem->hasInternalUse = 0;
                        float Nutr = (WorkAmount - Diff) *
                                     100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Nutrients",
                            Nutr * percentageBuff(GlobalNutritionPercentage));
                    }
                    if (PartItem->massVeggy <= 0)
                    {
                        PartItem->toDelete = true;
                        //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartNumber = stringFindNumber(WorkingLine, "[DigestsWater:");
                PartItem =
                    getItemPtrfromVectorVarSearch(npc.inventory, "MassWater");
                //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
                if (PartNumber != 0 && PartItem != nullptr &&
                    PartItem->massWater >
                        0) // TODO: Make sure the item is in THIS PART before digesting it!
                {
                    //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                    float WorkAmount = PartItem->massWater;
                    float Diff = WorkAmount - (PartNumber / 1000);

                    if (Diff > 0)
                    {
                        PartItem->massWater = Diff;
                        PartItem->hasInternalUse = 0;
                        float Nutr = (WorkAmount - Diff) *
                                     100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Hydration",
                            Nutr * percentageBuff(GlobalNutritionPercentage));
                    }
                    if (PartItem->massWater <= 0)
                    {
                        PartItem->toDelete = true;
                        //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartString = stringFindString(WorkingLine, "[PoisonFilter:");
                if (PartString != "")
                {

                    std::vector<std::string> StrVec =
                        stringFindElements(PartString, ":");
                    if (gvars::debug)
                        std::cout << "StrVec[0]: " << StrVec[0] << std::endl;
                    float Leftover =
                        npc.bloodwork(StrVec[0], -atof(StrVec[1].c_str()));
                    if (gvars::debug)
                        std::cout << "Bloodwork leftover is: " << Leftover
                                  << std::endl;
                    //NPC Critter;

                    for (size_t i = 0; i != StrVec.size(); i++)
                    {
                        if (gvars::debug)
                            std::cout << StrVec[i] << std::endl;
                    }
                }

                PartNumber = stringFindNumber(WorkingLine, "[Orafice:");
                if (PartNumber > 0)
                {
                    //std::vector<item> * Inv = &npc.inventory;

                    //for(int i = 0; i != npc.inventory.size(); i++)
                    for (auto i = npc.inventory.begin();
                         i != npc.inventory.begin(); i++)
                    {
                        bool FoundIt = false;
                        if ((*i).insidePart == "" && (*i).massFlesh > 0 &&
                            ConsumeFlesh) // Awww yeeessss, We gonna eat some flesh with our Orafice!
                        {
                            (*i).insidePart = CurrentPart;
                            FoundIt = true;
                        }

                        if ((*i).insidePart == "" && (*i).massVeggy > 0 &&
                            ConsumeVeggy) // Awww yeeessss, We gonna eat something with our Orafice!
                        {
                            (*i).insidePart = CurrentPart;
                            FoundIt = true;
                        }

                        if ((*i).insidePart == "" && (*i).massWater > 0 &&
                            ConsumeWater) // Awww yeeessss, We gonna eat something with our Orafice!
                        {
                            (*i).insidePart = CurrentPart;
                            FoundIt = true;
                        }

                        if (FoundIt)
                        {
                            std::string ChtStr;
                            ChtStr.append("* ");
                            ChtStr.append(npc.name);
                            ChtStr.append("(" + std::to_string(npc.id) + ")");
                            ChtStr.append(" has inserted ");
                            ChtStr.append((*i).name);
                            ChtStr.append(" into their ");
                            ChtStr.append(CurrentPart);
                            ChtStr.append("'s Orafice(");
                            ChtStr.append(std::to_string(PartNumber));
                            ChtStr.append(").");

                            chatBox.addChat(ChtStr, sf::Color(150, 150, 0));
                        }
                    }
                }

                PartNumber = stringFindNumber(
                    WorkingLine, "[BloodPumpRate:"); // TODO: Do this right.
                if (PartNumber != 0)
                {
                    float Blood =
                        stringFindNumber(npc.bloodcontent, "[Nutrients:");
                    if (Blood > PartNumber)
                    {
                        if ((npc.maxhunger - npc.hunger) > PartNumber)
                        {
                            npc.hunger += PartNumber;
                            npc.bloodwork("Nutrients", -PartNumber);
                        }
                    }
                    Blood = stringFindNumber(npc.bloodcontent, "[Hydration:");
                    if (Blood > PartNumber)
                    {
                        if ((npc.maxthirst - npc.thirst) > PartNumber)
                        {
                            npc.thirst += PartNumber;
                            npc.bloodwork("Hydration", -PartNumber);
                        }
                    }
                }

                SearchPos = EndPos;
            }
        }

        // TODO: Can't really use this until the critter brain rewrite.
        /*
        for(int i = 0; i != npc.inventory.size(); i++)
        {
            //std::cout << "Freakkin: " << npc.inventory[i].HasInternalUse << std::endl;
            if(npc.inventory[i].HasInternalUse <= -100)
            {
                //TODO: Ejection code, Just duplicate the item onto the ground with the same coords, eh? Also add a chatbox text that a critter did this.

                item ExpelItem = npc.inventory[i];
                ExpelItem.xpos = npc.xpos;
                ExpelItem.ypos = npc.ypos;
                ExpelItem.InsidePart = "";
                ExpelItem.HasInternalUse = 0;
                worlditems.push_back(ExpelItem);
                npc.inventory[i].ToDelete = true;

                std::string ChtStr;
                ChtStr.append("* ");
                ChtStr.append(npc.name);
                ChtStr.append(AddString("(",std::to_string(npc.id),")"));
                ChtStr.append(" has ejected ");
                ChtStr.append(npc.inventory[i].name);
                ChtStr.append(" from their ");
                ChtStr.append(npc.inventory[i].InsidePart);
                ChtStr.append(".");

                chatBox.AddChat(ChtStr,sf::Color(150,150,0));
            }
        }
        */

        debug("Debug: Ending Part Loop");
        // *BodyPart Loop*

        sf::Vector2f PathFindWorkPos(0, 0);

        float TempXpos = npc.xpos;
        float TempYpos = npc.ypos;
        npc.movetimer -= npc.movetimerrate;
        npc.attacktimer--;
        if (npc.attacktimer < 0)
        {
            npc.attacktimer = 0;
        }

        if (npc.name == "Zombie")
        {
        }

        if (npc.name == "Azabul")
        {

            npc.tentArm1.x =
                math::clamp(npc.tentArm1.x + randz(-3, 3), -20, 20);
            npc.tentArm1.y =
                math::clamp(npc.tentArm1.y + randz(-3, 3), -20, 20);
            npc.tentArm2.x =
                math::clamp(npc.tentArm2.x + randz(-3, 3), -20, 20);
            npc.tentArm2.y =
                math::clamp(npc.tentArm2.y + randz(-3, 3), -20, 20);
            npc.tentEnd1.x =
                math::clamp(npc.tentEnd1.x + randz(-3, 3), -20, 20);
            npc.tentEnd1.y =
                math::clamp(npc.tentEnd1.y + randz(-3, 3), -20, 20);
            npc.tentEnd2.x =
                math::clamp(npc.tentEnd2.x + randz(-3, 3), -20, 20);
            npc.tentEnd2.y =
                math::clamp(npc.tentEnd2.y + randz(-3, 3), -20, 20);
        }

        if (npc.id == gvars::myTargetid && key.space)
        {
            npc.attacking = true;
        }
        npc.hungertimer--;
        if (npc.hungertimer <= 0)
        {
            npc.hungertimer = npc.hungertimerint;
            npc.hunger -= npc.hungerrate;
            if (npc.hunger < 0)
            {
                npc.hunger = 0;
            }
        }
        npc.thirsttimer--;
        if (npc.thirsttimer <= 0)
        {
            npc.thirsttimer = npc.thirsttimerint;
            npc.thirst -= npc.thirstrate;
            if (npc.thirst < 0)
            {
                npc.thirst = 0;
            }
        }
        npc.regentimer--;
        if (npc.regentimer <= 0 && npc.health < npc.maxhealth && npc.health > 0)
        {
            npc.regentimer = npc.regentimerint;
            npc.health += npc.regenrate;
        }
        if (npc.health <= 0)
        {
            if (npc.canmove == true)
            {
                std::string ChtStr;
                ChtStr.append("* ");
                ChtStr.append(npc.name);
                ChtStr.append("(" + std::to_string(npc.id) + ")");
                ChtStr.append(" has died! ");

                chatBox.addChat(ChtStr, sf::Color(200, 0, 0));
            }
            npc.canmove = false;
        }
        if (npc.health > 0)
        {
            npc.canmove = true;
        }
        if (npc.health > 0 && npc.boolblood("[Zombification:") &&
            npc.name != "Zombie")
        {
            npc.bloodwork("Zombification", -0.01);
            npc.modhealth(-0.01);
            npc.fillhunger(-0.01);
        }
        if (npc.health <= 0 && npc.boolblood("[Zombification:") &&
            npc.name != "Zombie")
        {
            npc.bloodwork("Zombification", -9999999);
            npc.tag("[ZombieHater:", -9999999);
            npc.tag("[EatsFlesh:", 1);
            std::string OldName = npc.name;
            npc.name = "Zombie";
            npc.cbaseid = 666333;
            std::string Imagery = "Zombie.tga";
            for (auto const &image : texturemanager.textures)
            {
                if (image.name == Imagery)
                {
                    npc.img.setTexture(image.texture);
                    npc.img.setOrigin(npc.img.getTextureRect().height / 2,
                                      npc.img.getTextureRect().width / 2);
                }
            }
            npc.isNPC = true;
            npc.needsFood = true;
            npc.allowedFood = false;
            npc.needsWater = false;
            npc.allowedDrink = false;
            npc.canmove = true;
            npc.alive = true;
            npc.maxhealth = 200;
            npc.health = npc.maxhealth;
            npc.breathtimerint = 0;
            npc.hungertimerint = 0;
            npc.thirsttimerint = 0;
            npc.thirst = 0; //randz(10,600);
            npc.breathtimer = 2;
            npc.hungertimer = 0;
            npc.thirsttimer = 0;
            npc.maxhunger = 1000000;
            npc.maxthirst = 1000000;
            npc.target = "Flesh";
            npc.action = "Act";
            //npc.moverateint = 1;
            //npc.moverate = 1;
            npc.movetimerrate = 400;

            std::string ChtStr;
            ChtStr.append("* ");
            ChtStr.append(OldName);
            ChtStr.append("(" + std::to_string(npc.id) + ")");
            ChtStr.append(" has been zombified by Zombification! ");

            chatBox.addChat(ChtStr, sf::Color(200, 0, 0));
        };
        if (key.rshift && npc.id == gvars::myTargetid)
        {
            std::cout << npc.target << "At: " << npc.targetPos.x << ":"
                      << npc.targetPos.y << std::endl;
            std::cout << "HasTarget: " << npc.hasTarget
                      << " AtTarget: " << npc.atTarget << std::endl;
        }

        if (npc.attacking)
        {
            if (npc.attacktimer <= 0)
            {
                if (gvars::debug)
                {
                    std::cout << "Pre Mel Ran \n";
                }
                bool bMel = npc.hasItemType(1);
                Item Mel;
                if (bMel)
                {
                    Mel = *npc.getItemType(1);
                }
                Item Ran;
                bool bRan = npc.hasItemType(2);
                if (bRan == true)
                {
                    Ran = *npc.getItemType(2);
                    debug("Fish n Chips");
                }
                try
                {
                    bool Attacked = false;
                    if (gvars::debug)
                    {
                        std::cout << "Pre Mel \n";
                    }
                    if (bMel == true)
                    {
                        if (Mel.type == 1 &&
                            math::closeish(npc.xpos, npc.ypos, npc.shootPos.x,
                                           npc.shootPos.y) < Mel.range)
                        {
                            Attacked = true;
                            std::set<int> Tarz;
                            Tarz = npc.melee(1, 1, GridSize);
                            for (const auto &elem : Tarz)
                            {
                                npclist.at(elem)
                                    .modhealth(-(randz(Mel.mindam, Mel.maxdam) +
                                                 npc.skills.strength));
                                if (npc.hasTag("[CanLearn:"))
                                {
                                    npc.skills.Train("strength");
                                }
                            }
                            if (npc.attacking == false)
                            {
                                npc.attacktimer =
                                    (npc.attacktimerint -
                                     (math::clamp(npc.skills.agility / 10, 10,
                                                  100)));
                            } // Melee has a different method for saying it's done.
                            if (gvars::debug)
                            {
                                std::cout << "Post Mel \n";
                            }
                        }
                    }
                    if (gvars::debug)
                    {
                        std::cout << "Pre Ran \n";
                    }
                    if (bRan == true && Attacked == false)
                    {
                        if (Ran.type == 2)
                        {
                            bool Friendly = false;
                            std::set<int> ids =
                                npcTrace(npc.xpos, npc.ypos, npc.targetPos.x,
                                         npc.targetPos.y, npc.id);
                            if (!ids.empty())
                            {
                                try
                                {
                                    for (const auto &id : ids)
                                    {
                                        if (npclist.at(id).cbaseid ==
                                            npc.cbaseid)
                                        {
                                            Friendly = true;
                                        }
                                    }
                                }
                                catch (std::exception &e)
                                {
                                }
                            }
                            if (Friendly == false &&
                                gridposTrace(npc.xpos, npc.ypos, npc.shootPos.x,
                                             npc.shootPos.y, npc.id,
                                             npc.targetPos) == true &&
                                math::closeish(npc.xpos, npc.ypos,
                                               npc.shootPos.x,
                                               npc.shootPos.y) <= npc.viewrange)
                            {
                                int Tempx = randz(
                                    0,
                                    math::clamp(
                                        100 - npc.skills.dexterity, 0,
                                        100)); // This is to mess up the aiming.
                                int Tempy = randz(
                                    0, math::clamp(100 - npc.skills.dexterity,
                                                   0, 100));
                                if (npc.hasTag("[CanLearn:"))
                                {
                                    npc.skills.Train("dexterity");
                                }
                                if (randz(0, 1) == 1)
                                {
                                    Tempx = -Tempx;
                                }
                                if (randz(0, 1) == 1)
                                {
                                    Tempy = -Tempy;
                                }
                                sf::Vector2f SP(gvars::mousePos.x,
                                                gvars::mousePos.y);
                                sf::Vector2f Targ(npc.shootPos.x + Tempx,
                                                  npc.shootPos.y + Tempy);
                                effects.createLine(npc.xpos, npc.ypos, Targ.x,
                                                   Targ.y, 2, sf::Color::White);
                                std::set<int> ids = npcTrace(
                                    npc.xpos, npc.ypos, Targ.x, Targ.y, npc.id);
                                if (!ids.empty())
                                {
                                    for (size_t Ta = 0; Ta != ids.size(); Ta++)
                                    {
                                        try
                                        {
                                            for (const auto &id : ids)
                                            {
                                                npclist.at(id).modhealth(
                                                    -(randz(Ran.mindam,
                                                            Ran.maxdam) +
                                                      npc.skills.perception));
                                                if (npc.hasTag("[CanLearn:"))
                                                {
                                                    npc.skills.Train(
                                                        "perception");
                                                }
                                                effects.createCircle(
                                                    npclist.at(id).xpos,
                                                    npclist.at(id).ypos, 5,
                                                    sf::Color::Red);
                                            }
                                        }
                                        catch (std::exception &e)
                                        {
                                        }
                                        textList.createText(npc.xpos, npc.ypos,
                                                            11, sf::Color::Red,
                                                            "Bang!");
                                    }
                                }
                                npc.attacking = false;
                                npc.attacktimer =
                                    (npc.attacktimerint -
                                     (math::clamp(npc.skills.agility / 10, 10,
                                                  100)));
                            }
                            else
                            {
                                npc.attacking = false;
                            }
                            if (gvars::debug)
                            {
                                std::cout << "Post Ran \n";
                            }
                        }
                    }
                    else
                    {
                        npc.attacking = false;
                    }
                }
                catch (std::exception &e)
                {
                    std::cout << "Itemerror\n";
                }
            }
            else
            {
                npc.attacking = false;
            }
        } //End of Attacking
        if (npc.canmove == true && npc.attacking == false)
        {
            //std::cout << id << " is acting." << std::endl;
            // First, Check values, to see if hungry/thirsty/hurt then check if being attacked
            // Add an IsSafe before these checks
            if (npc.isHungry() == false && npc.isThirsty() == false)
            {
                //npc.moverate = npc.moverateint;
                npc.moverate = PartsWalkSpeed;
            }
            //else if(npc.IsThirsty() == false && npc.IsHungry() == true){npc.moverate = npc.moverateint/2;if(npc.moverate <= 0){npc.moverate=1;}}
            //else if(npc.IsHungry() == false  && npc.IsThirsty() == true){npc.moverate = npc.moverateint/2;if(npc.moverate <= 0){npc.moverate=1;}}
            //else if(npc.IsHungry() == true  && npc.IsThirsty() == true){npc.moverate = npc.moverateint/4;if(npc.moverate <= 0){npc.moverate=1;}}

            if (npc.name == "Zombie")
            {
                if (npc.name == "Zombie" && npc.hunger > 0)
                {
                    npc.moverate = npc.moverateint * 2;
                }
                if (npc.name == "Zombie" && npc.hunger >= 0)
                {
                    npc.target = "Flesh";
                }

                if (npc.name == "Zombie" && key.lctrl == true)
                {
                    npc.moverate = npc.moverateint * 4;
                }
                if (npc.name == "Zombie" && key.lalt == true)
                {
                    npc.moverate = 4;
                }

                float shake = 0.2;
                int bumz = randz(1, 10);
                if (bumz == 1)
                {
                    npc.ypos += shake;
                }
                if (bumz == 2)
                {
                    npc.xpos += shake;
                }
                if (bumz == 3)
                {
                    npc.ypos -= shake;
                }
                if (bumz == 4)
                {
                    npc.xpos -= shake;
                }
                if (gvars::groundmap[gvars::currentz][abs_to_index(
                        npc.xpos / GridSize)][abs_to_index(npc.ypos /
                                                           GridSize)] == 10)
                {
                    npc.xpos = TempXpos;
                    npc.ypos = TempYpos;
                }
            }

            if (npc.action == "Act")
            { // Start of "Act" action.
                if (npc.isHurt() == true)
                {
                    //target = "Meds"; //std::cout << id << " is hurt." << std::endl;
                }
                else if (npc.isHungry() == true && npc.allowedFood == true)
                {
                    npc.target =
                        "Food"; //std::cout << id << " is hungry." << std::endl;
                    if (npc.isThirsty())
                    {
                        bool IsFood = false;
                        for (auto &worlditem : worlditems)
                        {
                            if (worlditem.type == 4)
                            {
                                IsFood = true;
                            }
                        }
                        if (IsFood == false)
                        {
                            npc.target = "Water";
                        }
                    }
                }
                else if (npc.target == "Food" && npc.hunger < npc.maxhunger)
                {
                }
                else if (npc.target == "Flesh" && npc.name == "Zombie")
                {
                }
                else if (npc.isThirsty() == true)
                {
                    npc.target = "Water";
                    //std::cout << id << " is thirsty." << std::endl;
                    bool IsWater = false;
                    for (auto &worlditem : worlditems)
                    {
                        if (worlditem.massWater > 0)
                        {
                            IsWater = true;
                        }
                    }
                    if (IsWater == false)
                    {
                        npc.target = "Wander";
                    }
                }
                else if (npc.target == "Water" && npc.thirst < npc.maxthirst)
                {
                }
                else
                {
                    npc.target = "Wander";
                    //std::cout << id << " is fine." << std::endl;
                }
                if (npc.target == "Wander" && npc.hasTarget == true)
                { // TODO: Make sure this isn't needed anymore, Then delete it.
                    if (gvars::groundmap[gvars::currentz][abs_to_index(
                            npc.targetPos.x /
                            GridSize)][abs_to_index(npc.targetPos.y /
                                                    GridSize)] == 10)
                    {
                        npc.hasTarget = false;
                    }
                }
                if (npc.target == "Food")
                { // Compress this later by simply directly linking the target towards the search, Probably need a bool for Item or NPC to simplfy it.
                    Item *Item = findClosestItemPtr(npc.xpos, npc.ypos, "Food");
                    if (Item != nullptr)
                    {
                        if (Item->massFlesh > 0)
                        {
                            npc.targetPos =
                                sf::Vector2f(Item->xpos, Item->ypos);
                            npc.targetId = Item->id;
                            //npc.TargetVectorId = Item.at(3);
                            npc.target = "Food";
                            npc.hasTarget = true;
                        }
                    }
                    else
                    {
                        npc.target = "Wander";
                    }
                }
                if (npc.target == "Water")
                {
                    auto Item = findClosestItem(npc.xpos, npc.ypos, "Water");
                    if (Item.found)
                    {
                        npc.targetPos = sf::Vector2f(Item.xpos, Item.ypos);
                        npc.targetId = Item.id;
                        npc.targetVectorId = Item.vectorPosition;
                        npc.target = "Water";
                        npc.hasTarget = true;
                    }
                    else
                    {
                        npc.target = "Wander";
                    }
                }

                if (npc.name == "Azabul")
                    npc.target = "Flesh";
                if (npc.target == "Flesh")
                {
                    int closx = -100000;
                    int closy = -100000;
                    int ClosID = 0;
                    bool first = true;
                    for (auto &elem : npclist)
                    {
                        //if(first == true){ if(zitz->cbaseid != npc.cbaseid && zitz->health > 0){
                        if (first == true)
                        {
                            if (elem.cbaseid == 110110 && elem.health > 0)
                            {
                                closx = elem.xpos;
                                closy = elem.ypos;
                                first = false;
                            }
                        }
                        // else { if(zitz->cbaseid != npc.cbaseid && zitz->health > 0) {
                        else
                        {
                            if (elem.cbaseid == 110110 && elem.health > 0)
                            {
                                int one = math::closeish(npc.xpos, npc.ypos,
                                                         elem.xpos, elem.ypos);
                                int two = math::closeish(npc.xpos, npc.ypos,
                                                         closx, closy);
                                if (one < two)
                                {
                                    closx = elem.xpos;
                                    closy = elem.ypos;
                                    ClosID = elem.id;
                                }
                            }
                        }
                    }
                    if (closx == -100000)
                    {
                        npc.target = "Wander";
                    }
                    if (first == false)
                    {
                        npc.targetPos = sf::Vector2f(closx, closy);
                        npc.targetId = ClosID;
                        npc.target = "Flesh";
                        npc.hasTarget = true;
                    }
                    else
                    {
                        npc.target = "Wander";
                    }
                }

                try
                { // Jobs Section
                    if (uniFact[0].jobList.size() != 0 &&
                        npc.target == "Wander" && npc.Faction == "The Alphas")
                    {
                        debug("Preforming Job Routine; ");
                        for (size_t i = 0; i != uniFact[0].jobList.size(); i++)
                        {
                            debug("Starting Job " + std::to_string(i));
                            //if(!Deleting) Deleter++;
                            //Con(AddString(npc.name,JobList[i].Type));
                            if (uniFact[0].jobList[i].pItem == nullptr &&
                                uniFact[0].jobList[i].type ==
                                    "PickUp") // Deleting objectless pickup jobs.
                            { // Deleting objectless pickup jobs.
                                uniFact[0].jobList[i].toDelete = true;
                                break;
                            }

                            if ((uniFact[0].jobList[i].pWorker == nullptr &&
                                 npc.hasJob == false &&
                                 uniFact[0].jobList[i].toDelete == false) ||
                                (uniFact[0].jobList[i].pWorker != nullptr &&
                                 uniFact[0].jobList[i].pWorker->id == npc.id &&
                                 uniFact[0].jobList[i].toDelete == false))
                            {
                                debug("Comparitive Success");
                                if (uniFact[0].jobList[i].pWorker == nullptr)
                                {
                                    std::cout << npc.name
                                              << " has job: " << npc.hasJob
                                              << ", and yet :";
                                    uniFact[0].jobList[i].pWorker = &npclist.at(
                                        IntegerIterator); // Not sure if this is technically better or worse than repointing every frame.
                                    uniFact[0].jobList[i].pWorker->hasJob =
                                        true;
                                    std::cout << npc.hasJob << " does now? \n";
                                    //fSleep(2);
                                }

                                if (uniFact[0].jobList[i].type == "Build")
                                {
                                    debug("Starting Build");

                                    //if(GetItemPtrfromVector(worlditems, "Wood") != NULL) item WorkLoad = *GetItemPtrfromVector(worlditems, "Wood");
                                    Item *InvWood = getItemPtrFromVector(
                                        npc.inventory, "Wood");
                                    Item *WldWood = getItemPtrFromVector(
                                        worlditems, "Wood");

                                    if (InvWood != nullptr)
                                    {
                                        con("Success! I have Wood!");
                                        int x = uniFact[0].jobList[i].workPos.x;
                                        int y = uniFact[0].jobList[i].workPos.y;

                                        npc.targetPos.x = x;
                                        npc.targetPos.y = y;
                                        npc.hasTarget = true;
                                        npc.target = "BuildWoodWall";

                                        if (math::closeish(npc.xpos, npc.ypos,
                                                           x,
                                                           y) <= npc.size * 3)
                                        {

                                            npc.targetPos.x = npc.xpos;
                                            npc.targetPos.y = npc.ypos;

                                            uniFact[0]
                                                .jobList[i]
                                                .completionProgress +=
                                                npc.skills.intelligence / 2;

                                            //std::cout << "JobTimer: " << UniFact[0].JobList[i].CompletionProgress << std::endl;
                                            //Effectz.CreateCircle(UniFact[0].JobList[i].WorkPos.x,UniFact[0].JobList[i].WorkPos.y, (UniFact[0].JobList[i].CompletionProgress - UniFact[0].JobList[i].CompletionTimer)/10  ,sf::Color(150,150,150,150));

                                            //std::cout << 361*(PercentIs(UniFact[0].JobList[i].CompletionTimer, UniFact[0].JobList[i].CompletionProgress)/100) << std::endl;
                                            //fSleep(4);
                                            for (
                                                float Rot = 1;
                                                Rot <
                                                    361 *
                                                        (percentIs(
                                                             uniFact[0]
                                                                 .jobList[i]
                                                                 .completionTimer,
                                                             uniFact[0]
                                                                 .jobList[i]
                                                                 .completionProgress) /
                                                         100);
                                                Rot++)
                                            {
                                                //std::cout << "Rot: " << Rot << std::endl;

                                                float XPos =
                                                    x +
                                                    sin(Rot * PI / 180) * 10;
                                                float YPos =
                                                    y +
                                                    cos(Rot * PI / 180) * 10;

                                                effects.createLine(
                                                    x, y, XPos, YPos, 1,
                                                    sf::Color(150, 150, 150,
                                                              150));
                                            }

                                            if (uniFact[0]
                                                    .jobList[i]
                                                    .completionProgress >=
                                                uniFact[0]
                                                    .jobList[i]
                                                    .completionTimer)

                                            {
                                                tiles[abs_to_index(x / 20)]
                                                     [abs_to_index(y / 20)][30]
                                                         .wall();
                                                //Tiles[abs_to_index(x/20)][abs_to_index(y/20)][30].ID = 1010;
                                                //Tiles[abs_to_index(x/20)][abs_to_index(y/20)][30].Img.setTexture( *imagemanager.GetImage("Wall.png"));
                                                InvWood->toDelete = true;
                                                uniFact[0].jobList[i].toDelete =
                                                    true;
                                                uniFact[0]
                                                    .jobList[i]
                                                    .pWorker->hasJob = false;
                                                //Crashed instantly;
                                            }
                                        }
                                    }

                                    else if (WldWood != nullptr)
                                    {

                                        con("Partial Success! World has Wood!");
                                        //item WorkLoad = *GetItemPtrfromVector(worlditems, "Wood");

                                        npc.targetPos.x = WldWood->xpos;
                                        npc.targetPos.y = WldWood->ypos;
                                        npc.hasTarget = true;
                                        npc.target = WldWood->name;

                                        //Con(WorkLoad.id);

                                        debug("Post wood targeting, Pre "
                                              "Close-Ish function");

                                        if (math::closeish(npc.xpos, npc.ypos,
                                                           WldWood->xpos,
                                                           WldWood->ypos) <=
                                            npc.size)
                                        {
                                            //Con("I'm there! \n");
                                            //Deleting = true;
                                            debug("It's close!");
                                            npc.inventory.push_back(*WldWood);
                                            WldWood->toDelete = true;
                                        }
                                    }
                                    debug("Ended Build");
                                }

                                else if (uniFact[0].jobList[i].pItem != nullptr)
                                {
                                    debug("Starting pItem != NULL");

                                    npc.targetPos.x =
                                        uniFact[0].jobList[i].pItem->xpos;
                                    npc.targetPos.y =
                                        uniFact[0].jobList[i].pItem->ypos;
                                    npc.hasTarget = true;
                                    npc.target =
                                        uniFact[0].jobList[i].pItem->name;
                                    debug("Post HasTarget");

                                    if (uniFact[0].jobList[i].type ==
                                            "PickUp" &&
                                        math::closeish(
                                            npc.xpos, npc.ypos,
                                            uniFact[0].jobList[i].pItem->xpos,
                                            uniFact[0]
                                                .jobList[i]
                                                .pItem->ypos) <= npc.size)
                                    {
                                        //Con("I'm there! \n");
                                        //Deleting = true;
                                        npc.inventory.push_back(
                                            *uniFact[0].jobList[i].pItem);
                                        debug("Post Inventory Pushback");

                                        uniFact[0].jobList[i].pItem->toDelete =
                                            true;
                                        /*
                                for(std::list<item>::iterator v = worlditems.begin(); v != worlditems.end(); v++ )
                                {
                                    if((*v).id == UniFact[0].JobList[i].pItem->id)
                                    {
                                        v->ToDelete = true;
                                        std::cout << "Confirmed. \n";
                                        fSleep(1);
                                    }
                                }
                                */

                                        uniFact[0].jobList[i].pItem = nullptr;
                                        uniFact[0].jobList[i].toDelete = true;
                                        uniFact[0].jobList[i].pWorker->hasJob =
                                            false;
                                        unpointItems(worlditems);
                                        debug("Post Unpoint");
                                    }

                                    if (uniFact[0].jobList[i].type == "Chop" &&
                                        math::closeish(
                                            npc.xpos, npc.ypos,
                                            uniFact[0].jobList[i].pItem->xpos,
                                            uniFact[0]
                                                .jobList[i]
                                                .pItem->ypos) <= npc.size)
                                    {
                                        debug("Post Chopcheck");
                                        //Con("I'm there! \n");
                                        //Deleting = true;
                                        //npc.inventory.push_back(*UniFact[0].JobList[i].pItem);
                                        //FUCKNUTS start here, Just made the plank, Make the tree give the planks, MmkAY?!
                                        Item WoodStuffs =
                                            *getGlobalItem("Wood");

                                        debug("Post WoodStuffs");

                                        WoodStuffs.xpos =
                                            uniFact[0].jobList[i].pItem->xpos;
                                        WoodStuffs.ypos =
                                            uniFact[0].jobList[i].pItem->ypos;
                                        worlditems.push_back(WoodStuffs);
                                        debug("Post WoodSpawn");

                                        uniFact[0].jobList[i].pItem->toDelete =
                                            true;
                                        uniFact[0].jobList[i].pItem = nullptr;
                                        uniFact[0].jobList[i].toDelete = true;
                                        uniFact[0].jobList[i].pWorker->hasJob =
                                            false;
                                        debug("Post ToDelete");
                                        unpointItems(worlditems);

                                        //items are not getting deleted, ugh.
                                    }

                                    debug("Ending pItem != NULL");
                                }

                                if (uniFact[0].jobList[i].type == "Dig")
                                {

                                    PathFindWorkPos.x =
                                        uniFact[0].jobList[i].workPos.x;
                                    PathFindWorkPos.y =
                                        uniFact[0].jobList[i].workPos.y;

                                    npc.targetPos.x = PathFindWorkPos.x;
                                    npc.targetPos.y = PathFindWorkPos.y;
                                    npc.hasTarget = true;
                                    npc.target = "DigNaturalWall";

                                    if (math::closeish(npc.xpos, npc.ypos,
                                                       PathFindWorkPos.x,
                                                       PathFindWorkPos.y) <=
                                        npc.size * 3)
                                    {

                                        npc.targetPos.x = npc.xpos;
                                        npc.targetPos.y = npc.ypos;

                                        uniFact[0]
                                            .jobList[i]
                                            .completionProgress +=
                                            npc.skills.strength / 2;

                                        for (
                                            float Rot = 1;
                                            Rot <
                                                361 *
                                                    (percentIs(
                                                         uniFact[0]
                                                             .jobList[i]
                                                             .completionTimer,
                                                         uniFact[0]
                                                             .jobList[i]
                                                             .completionProgress) /
                                                     100);
                                            Rot++)
                                        { // Pretty circle progressing graphic.

                                            float XPos =
                                                PathFindWorkPos.x +
                                                sin(Rot * PI / 180) * 10;
                                            float YPos =
                                                PathFindWorkPos.y +
                                                cos(Rot * PI / 180) * 10;

                                            effects.createLine(
                                                PathFindWorkPos.x,
                                                PathFindWorkPos.y, XPos, YPos,
                                                1,
                                                sf::Color(150, 150, 150, 150));
                                        }

                                        if (uniFact[0]
                                                .jobList[i]
                                                .completionProgress >=
                                            uniFact[0]
                                                .jobList[i]
                                                .completionTimer)

                                        {
                                            tiles[abs_to_index(
                                                PathFindWorkPos.x / 20)]
                                                 [abs_to_index(
                                                     PathFindWorkPos.y /
                                                     20)][30].stone();
                                            debug("Spawning Rocks");
                                            int TAR = randz(
                                                3, 8); // Throw away random
                                            for (int z = 0; z != TAR; z++)
                                            {
                                                Item StoneStuffs =
                                                    *getGlobalItem("Rock");

                                                StoneStuffs.xpos =
                                                    PathFindWorkPos.x +
                                                    (4 - randz(1, 8));
                                                StoneStuffs.ypos =
                                                    PathFindWorkPos.y +
                                                    (4 - randz(1, 8));
                                                worlditems.push_back(
                                                    StoneStuffs);
                                            }

                                            uniFact[0].jobList[i].toDelete =
                                                true;
                                            uniFact[0]
                                                .jobList[i]
                                                .pWorker->hasJob = false;
                                            debug("Dig Wall Completed");
                                        }
                                    }
                                }
                            }

                            debug("End Job " + std::to_string(i));
                        }

                        removeJobs(uniFact[0].jobList);

                        debug("Finished Job Activity. \n ");
                    }

                    if (npc.target == "Wander" && npc.hasTarget == false)
                    { // Find somewhere random to walk towards, so long as there isn't anything on that spot.
                        bool FindEmpty = false;
                        while (FindEmpty == false)
                        {
                            npc.targetPos = sf::Vector2f(randz(700, 1300),
                                                         randz(700, 1300));
                            npc.hasTarget = true;
                            if (gvars::groundmap[gvars::currentz][abs_to_index(
                                    npc.targetPos.x /
                                    GridSize)][abs_to_index(npc.targetPos.y /
                                                            GridSize)] != 10)
                            {
                                FindEmpty = true;
                            }
                        }
                    }
                }
                catch (std::exception &e)
                {
                    std::cout << "Something went wrong in NPC Job Routine \n";
                }

                if (npc.name == "Shinobi" && npc.hasWeapon() == false)
                {
                    auto Item = findClosestItem(npc.xpos, npc.ypos, "Sword");
                    if (Item.found)
                    {
                        npc.targetPos = sf::Vector2f(Item.xpos, Item.ypos);
                        npc.targetId = Item.id;
                        npc.targetVectorId = Item.vectorPosition;
                        npc.target = "Sword";
                        npc.hasTarget = true;
                    }
                    else
                    {
                        npc.target = "Wander";
                    }
                }
                if (npc.hasWeapon())
                { // Attack nearby Hostiles.
                    int closx = -100000;
                    int closy = -100000;
                    bool FoundOne = false;
                    if (npc.hasTag("[ZombieHater:"))
                    {
                        bool first = true;
                        for (auto &elem : npclist)
                        {
                            if (first == true)
                            {
                                if (elem.cbaseid == 666333 && elem.health > 0)
                                {
                                    closx = elem.xpos;
                                    closy = elem.ypos;
                                    first = false;
                                    FoundOne = true;
                                }
                            }
                            else
                            {
                                if (elem.cbaseid == 666333 && elem.health > 0)
                                {
                                    int one =
                                        math::closeish(npc.xpos, npc.ypos,
                                                       elem.xpos, elem.ypos);
                                    int two = math::closeish(npc.xpos, npc.ypos,
                                                             closx, closy);
                                    if (one < two)
                                    {
                                        closx = elem.xpos;
                                        closy = elem.ypos;
                                    }
                                }
                            }
                        }
                        if (first == false)
                        {
                            npc.shootPos = sf::Vector2f(closx, closy);
                            if (math::closeish(npc.xpos, npc.ypos,
                                               npc.shootPos.x,
                                               npc.shootPos.y) <= npc.viewrange)
                            {
                                npc.attacking = true;
                            }
                        }
                    }
                    if (npc.isHungry() == false && npc.isThirsty() == false &&
                        FoundOne == true)
                    {
                        npc.targetPos = sf::Vector2f(closx, closy);
                    }
                }

                if (npc.closeToTarget(npc.reach, npc.targetPos) == true &&
                    npc.target == "Flesh")
                {
                    if (npc.attacktimer == 0)
                    {

                        std::string AtkType;
                        npc.attacktimer =
                            (npc.attacktimerint -
                             (math::clamp(npc.skills.agility / 10, 10, 100))) *
                            4;
                        int numba = -1;
                        int numbaz = -1;
                        int num;
                        bool foundit = false;
                        for (auto &elem : npclist)
                        {
                            if (foundit == false)
                            {
                                numba++;
                                if (math::closeishS(elem.xpos, npc.xpos) <=
                                        npc.reach &&
                                    math::closeishS(elem.ypos, npc.ypos) <=
                                        npc.reach &&
                                    elem.target != "Flesh")
                                {
                                    numbaz++;
                                    num = elem.id;
                                    foundit = true;
                                }
                            }
                        }
                        if (foundit == true && npclist.at(numba).id == num)
                        {
                            npc.atTarget = false;
                            npc.hasTarget =
                                false; // TODO: Fix Zombie Attacking, Give it some Delay, and some damage based on Strength and filled belly.
                            int zDamage = randz(0, npclist.at(numba).health);
                            if (npc.name != "Azabul")
                                npc.fillhunger(zDamage);
                            if (npc.name != "Azabul")
                                npclist.at(numba)
                                    .bloodwork("Zombification", 100000);
                            if (npc.name == "Azabul")
                            {
                                npc.tentArm1.x = npclist.at(numba).xpos;
                                npc.tentArm1.y = npclist.at(numba).ypos;
                                npc.tentArm2.x = npclist.at(numba).xpos;
                                npc.tentArm2.y = npclist.at(numba).ypos;
                                //BLARGGITY
                                Item Blood;
                                Blood.amount = zDamage;
                                Blood.name = "Blood";
                                Blood.insidePart = "LowerTorso";
                                npc.inventory.push_back(Blood);
                                AtkType = " has drunk from ";
                            }
                            npclist.at(numba).modhealth(-zDamage);

                            std::string AtkStr;
                            AtkStr.append("* ");
                            AtkStr.append(npc.name);
                            AtkStr.append("(" + std::to_string(npc.id) + ")");
                            if (AtkType != "")
                                AtkStr.append(" has biten ");
                            else
                                AtkStr.append(AtkType);
                            AtkStr.append(npclist.at(numba).name);
                            AtkStr.append("(" +
                                          std::to_string(npclist.at(numba).id) +
                                          ")");
                            AtkStr.append(", dealing ");
                            AtkStr.append(std::to_string(zDamage));
                            AtkStr.append(" damage.");

                            chatBox.addChat(AtkStr, sf::Color::Red);
                        }
                    }
                }
                if (npc.name == "Mini Turret")
                {
                    if (npc.hasWeapon() == false)
                    {
                        npc.addItem("5mmCannon", 1);
                        npc.skills.dexterity = 90;
                        npc.skills.agility = 10;
                    }
                    if (npc.hasTag("[ZombieHater:") == false)
                    {
                        npc.tag("[ZombieHater:", 1);
                        std::cout << "Giving MiniTurret [ZombieHater: \n";
                    }
                }
            } // Ending of "Act" action
            if (gvars::debug)
                std::cout << "Post Act Section \n";

            { // Vision check and Activation of Path Finding.
                if (gvars::debug)
                    std::cout << "Pre 'set' vision. \n";

                bool FoundGoal = false;
                if (math::closeish(npc.targetPos.x, npc.targetPos.y, npc.xpos,
                                   npc.ypos) <= npc.viewrange &&
                    npc.cbaseid != -1337)
                {

                    float Ang = math::angleBetweenVectors(
                        math::Vec2f(npc.targetPos.x, npc.targetPos.y),
                        math::Vec2f(npc.xpos, npc.ypos));
                    float MyAngle = npc.angle;
                    float difference = abs(Ang - MyAngle);
                    if (difference > 180.0f)
                    {
                        difference = 360.0f - difference;
                    }
                    if (difference >= -90.0f && difference <= 90.0f)
                    {
                        if (gridposTrace(npc.xpos, npc.ypos, npc.targetPos.x,
                                         npc.targetPos.y, npc.id,
                                         npc.targetPos) == true)
                        {
                            FoundGoal = true;
                            effects.createLine(npc.xpos, npc.ypos,
                                               npc.targetPos.x, npc.targetPos.y,
                                               1, sf::Color::White);
                            npc.needsPath = false;
                        }
                    }
                }
                gvars::sunmap[gvars::currentz][abs_to_index(
                    npc.xpos / GridSize)][abs_to_index(npc.ypos / GridSize)] =
                    255;
                for (int i = 0; i <= gridy - 1; i++)
                { // Vision Stuffs;
                    for (int t = 0; t <= gridx - 1; t++)
                    {
                        int z = gvars::currentz;
                        if (gvars::sunmap[z][i][t] != -1)
                        {
                            if (math::closeish((i * GridSize) + 10,
                                               (t * GridSize) + 10, npc.xpos,
                                               npc.ypos) <= npc.viewrange &&
                                npc.cbaseid != -1337)
                            {

                                float Ang = math::angleBetweenVectors(
                                    math::Vec2f((i * GridSize) + 10,
                                                (t * GridSize) + 10),
                                    math::Vec2f(npc.xpos, npc.ypos));
                                float MyAngle = npc.angle;
                                float difference = abs(Ang - MyAngle);
                                if (difference > 180.0f)
                                {
                                    difference = 360.0f - difference;
                                }
                                if (difference >= -90.0f && difference <= 90.0f)
                                {
                                    /*if(gridposTrace(npc.xpos,npc.ypos,(i*GridSize)+10,(t*GridSize)+10,npc.id,Math::Vec((i*GridSize)+10,(t*GridSize)+10)) == true)
                                        {
                                             globals::sunmap[z][i][t] = 255;
                                             //Effectz.CreateCircle((i*20)+10,(t*20)+10,2,White);
                                        }*/
                                }
                            }
                        }
                    }
                }
                if (FoundGoal == false && npc.cbaseid != -1337)
                {
                    if (gvars::debug)
                        std::cout << "FoundGoal == false";
                    npc.needsPath = true;

                    int Previous = -1;

                    if (PathFindWorkPos.x != 0)
                    {
                        Previous = astar::walkability[abs_to_index(
                            PathFindWorkPos.x /
                            20)][abs_to_index(PathFindWorkPos.y / 20)];
                        astar::walkability[abs_to_index(
                            PathFindWorkPos.x /
                            20)][abs_to_index(PathFindWorkPos.y / 20)] =
                            astar::walkable;
                    }

                    npc.pathFinding.myFindPath(
                        npc.xpos, npc.ypos, npc.targetPos.x,
                        npc.targetPos
                            .y); // TODO: This causes a crash for some reason.

                    if (PathFindWorkPos.x != 0)
                    {
                        astar::walkability[abs_to_index(
                            PathFindWorkPos.x /
                            20)][abs_to_index(PathFindWorkPos.y / 20)] =
                            Previous;
                    }
                }
                if (true == false)
                {
                    /*try{

        sList = NpcList(npc.id);
        std::set<int>::iterator iList;

        for(iList = sList.begin(); iList != sList.end(); iList++){
            float NpcPosX = npclist.at(*iList).xpos;
            float NpcPosY = npclist.at(*iList).ypos;
            if( Math::Closeish(npc.TargetPos.x,npc.TargetPos.y,npc.xpos,npc.ypos) <= npc.viewrange ){
                float Ang = Math::AngleBetweenVectors(Math::Vec(npc.TargetPos.x,npc.TargetPos.y),Math::Vec(npc.xpos,npc.ypos));
                if(Ang >= npc.angle-90 && Ang <= npc.angle+90){

                    std::cout << npc.name << npc.id << " is facing " << npclist.at(*iList).name <<npclist.at(*iList).id << std::endl;
                    Effectz.CreateLine(npc.xpos,npc.ypos,npc.TargetPos.x,npc.TargetPos.y,1,White);
                    if(gridposTrace(npc.xpos,npc.ypos,npc.TargetPos.x,npc.TargetPos.y,npc.id,npc.TargetPos) == true){ FoundGoal = true; }
                }

            }
        }


    }catch (std::exception& e){}*/
                }
                if (gvars::debug)
                    std::cout << "Post 'set' vision. \n";
            }

            if (npc.action ==
                "Orders") // Vision correction currently disabled to preserve laaaaaag.
            {             // Start of "Orders action.
                /*for (int i = 0; i <= gridy-1; i++)
                {
                    for( int t = 0; t <= gridx-1; t++)
                    {
                        int z = globals::currentz;
                        if(globals::sunmap[z][i][t] != -1)
                        {
                            if( Math::Closeish((i*GridSize)+10,(t*GridSize)+10,npc.xpos,npc.ypos) <= npc.viewrange && npc.cbaseid != -1337)
                            {
                                float Ang = Math::AngleBetweenVectors(Math::Vec((i*GridSize)+10,(t*GridSize)+10),Math::Vec(npc.xpos,npc.ypos));
                                float MyAngle = npc.angle;
                                float difference = abs(Ang - MyAngle);
                                if (difference > 180.0f)
                                {
                                    difference = 360.0f - difference;
                                }
                                if(difference >= -90.0f && difference <= 90.0f)
                                {
                                    if(gridposTrace(npc.xpos,npc.ypos,(i*GridSize)+10,(t*GridSize)+10,npc.id,Math::Vec((i*GridSize)+10,(t*GridSize)+10)) == true)
                                    {
                                        globals::sunmap[z][i][t] = 255;
                                        //Effectz.CreateCircle((i*20)+10,(t*20)+10,2,White);
                                    }
                                }
                            }
                        }
                    }
                }*/
                if (npc.target == "Goto")
                {
                }
                //if(npc.NeedsPath == false){if(npc.target != "None"){ npc.DirMove(npc.TargetPos);}}
                if (npc.atTarget == true && npc.target == "Goto")
                {
                    npc.atTarget = false;
                    npc.hasTarget = false;
                    npc.target = "None";
                }
            } // Ending of "Orders" action
            if (npc.action == "Pickup")
            {
                //if(npc.NeedsPath == false){if(npc.target != "None"){ npc.DirMove(npc.TargetPos);}}
            }
            if ((npc.target != "None" && npc.atTarget) ||
                (npc.target != "None" &&
                 math::closeish(npc.xpos, npc.ypos, npc.targetPos.x,
                                npc.targetPos.y) <= npc.size))
            {
                if (getItemVectorId(npc.targetId) != -1)
                {
                    auto ItemItr = worlditems.begin();
                    std::advance(ItemItr, getItemVectorId(npc.targetId));

                    //if(Math::Closeish(npc.xpos,npc.ypos,worlditems.at(GetItemVectorId(npc.TargetId)).xpos,worlditems.at(GetItemVectorId(npc.TargetId)).ypos) <= npc.reach && worlditems.at(GetItemVectorId(npc.TargetId)).Pickupable == true)
                    if (math::closeish(npc.xpos, npc.ypos, (*ItemItr).xpos,
                                       (*ItemItr).ypos) <= npc.reach &&
                        (*ItemItr).pickupable == true)
                    {
                        try
                        {
                            npc.inventory.push_back((*ItemItr));
                            removeItem(npc.targetId);
                        }
                        catch (std::exception &e)
                        {
                            textList.createText(npc.xpos, npc.ypos, 11,
                                                sf::Color::Red,
                                                "Somethings wrong!");
                        };
                    }
                }
                if (npc.target == "Wander")
                {
                    npc.atTarget = false;
                    npc.hasTarget = false;
                }
                if (true == false)
                {
                    if (npc.target == "Sword")
                    {
                        if (getItemVectorId(npc.targetId) != -1)
                        {
                            if (math::closeish(
                                    npc.xpos, npc.ypos,
                                    (*listGet(worlditems,
                                              getItemVectorId(npc.targetId)))
                                        .xpos,
                                    (*listGet(worlditems,
                                              getItemVectorId(npc.targetId)))
                                        .ypos) <= npc.reach * 2)
                            {
                                try
                                {
                                    npc.inventory.push_back(
                                        (*listGet(
                                             worlditems,
                                             getItemVectorId(npc.targetId))));
                                    removeItem(npc.targetId);
                                }
                                catch (std::exception &e)
                                {
                                };
                            }
                        }
                    }
                    else if (npc.target == "Food")
                    {
                        if (getItemVectorId(npc.targetId) != -1)
                        {
                            if (math::closeish(
                                    npc.xpos, npc.ypos,
                                    (*listGet(worlditems,
                                              getItemVectorId(npc.targetId)))
                                        .xpos,
                                    (*listGet(worlditems,
                                              getItemVectorId(npc.targetId)))
                                        .ypos) <= npc.size * 2)
                            {
                                try
                                {
                                    //npc.inventory.push_back(worlditems.at(GetItemVectorId(npc.TargetId)));
                                    npc.fillhunger((*listGet(worlditems,
                                                             getItemVectorId(
                                                                 npc.targetId)))
                                                       .hungervalue);
                                    removeItem(npc.targetId);
                                    npc.atTarget = false;
                                    npc.hasTarget = false;
                                    npc.targetId = -1;
                                    npc.targetVectorId = -1;
                                    textList.createText(npc.xpos, npc.ypos, 11,
                                                        sf::Color::Blue,
                                                        "*Crunch!*");
                                }
                                catch (std::exception &e)
                                {
                                    textList.createText(npc.xpos, npc.ypos, 11,
                                                        sf::Color::Red,
                                                        "What the FUCK?!");
                                };
                            }
                        }
                    }
                    else if (npc.target == "Water")
                    {
                        if (getItemVectorId(npc.targetId) != -1)
                        {
                            if (math::closeish(
                                    npc.xpos, npc.ypos,
                                    (*listGet(worlditems,
                                              getItemVectorId(npc.targetId)))
                                        .xpos,
                                    (*listGet(worlditems,
                                              getItemVectorId(npc.targetId)))
                                        .ypos) <= npc.size * 2)
                            {
                                try
                                {
                                    //npc.inventory.push_back(worlditems.at(GetItemVectorId(npc.TargetId)));
                                    npc.fillthirst((*listGet(worlditems,
                                                             getItemVectorId(
                                                                 npc.targetId)))
                                                       .thirstvalue);
                                    npc.atTarget = false;
                                    npc.hasTarget = false;
                                    npc.targetId = -1;
                                    npc.targetVectorId = -1;
                                    textList.createText(npc.xpos, npc.ypos, 11,
                                                        sf::Color::Blue,
                                                        "*Gulp!*");
                                }
                                catch (std::exception &e)
                                {
                                    textList.createText(npc.xpos, npc.ypos, 11,
                                                        sf::Color::Red,
                                                        "What the FUCK?!");
                                };
                            }
                        }
                    }
                }
            }
            if (gvars::debug)
                std::cout << "Post Item Pickups. \n";

            if (npc.movetimer <= 0)
            {
                npc.movetimer =
                    (npc.movetimerint +
                     npc.movetimer); // TODO: Figure out why I added 0 to this, Year later: It was because movetimer may be less than 0, I wanted the next to turn happen as soon as possible due to it.
                if (npc.target != "None" && npc.needsPath == false &&
                    math::Vec2f(npc.xpos, npc.ypos) != npc.targetPos)
                { // Walk Move
                    if (npc.attacking && npc.hasWeapon("Gun") &&
                        math::closeish(npc.xpos, npc.ypos, npc.targetPos.x,
                                       npc.targetPos.y) < npc.viewrange)
                    {
                    }
                    else if (npc.attacking && npc.hasWeapon("Sword") &&
                             math::closeish(npc.xpos, npc.ypos, npc.targetPos.x,
                                            npc.targetPos.y) < 10)
                    {
                    }
                    else
                    {
                        /*if(gridposTrace(npc.xpos,npc.ypos,npc.TargetPos.x,npc.TargetPos.y,npc.id,npc.TargetPos) != false)
                        {
                            //npc.DirMove(npc.TargetPos);
                        }*/
                        npc.dirMove(npc.targetPos);
                    }
                }
                if (npc.needsPath == true &&
                    math::Vec2f(npc.xpos, npc.ypos) != npc.targetPos)
                { // Acting on Path Finding.
                    if (randz(0, 20) < 3)
                    {
                        int Previous = -1;
                        if (PathFindWorkPos.x != 0)
                        {
                            Previous = astar::walkability[abs_to_index(
                                PathFindWorkPos.x /
                                20)][abs_to_index(PathFindWorkPos.y / 20)];
                            astar::walkability[abs_to_index(
                                PathFindWorkPos.x /
                                20)][abs_to_index(PathFindWorkPos.y / 20)] =
                                astar::walkable;
                        }

                        npc.pathFinding.myFindPath(npc.xpos, npc.ypos,
                                                   npc.targetPos.x,
                                                   npc.targetPos.y);

                        if (PathFindWorkPos.x != 0)
                        {
                            astar::walkability[abs_to_index(
                                PathFindWorkPos.x /
                                20)][abs_to_index(PathFindWorkPos.y / 20)] =
                                Previous;
                        }
                    }
                    for (int i = 0; i != Grids; i++)
                    {
                        for (int t = 0; t != Grids; t++)
                        {
                            // Add an Item Check here later to see if it blocks movement
                            if (tiles[i][t][30].walkable == false)
                            {
                                astar::walkability[i][t] = astar::unwalkable;
                            }
                            else
                            {
                                astar::walkability[i][t] = astar::walkable;
                            }
                            if (astar::walkability[i][t] > 1)
                                astar::walkability[i][t] = 0;
                        }
                    }

                    int Previous = -1;

                    if (PathFindWorkPos.x != 0)
                    {
                        Previous = astar::walkability[abs_to_index(
                            PathFindWorkPos.x /
                            20)][abs_to_index(PathFindWorkPos.y / 20)];
                        astar::walkability[abs_to_index(
                            PathFindWorkPos.x /
                            20)][abs_to_index(PathFindWorkPos.y / 20)] =
                            astar::walkable;
                    }

                    npc.pathFinding.myReadPath(1, npc.xpos, npc.ypos,
                                               npc.moverate);
                    npc.dirMove(math::Vec2f(npc.pathFinding.myxPath,
                                            npc.pathFinding.myyPath));

                    if (PathFindWorkPos.x != 0)
                    {
                        astar::walkability[abs_to_index(
                            PathFindWorkPos.x /
                            20)][abs_to_index(PathFindWorkPos.y / 20)] =
                            Previous;
                    }

                    /*if (npc.xpos > npc.pathFinding.MyxPath) {npc.xpos = npc.xpos - npc.moverate;}
    if (npc.xpos < npc.pathFinding.MyxPath) {npc.xpos = npc.xpos + npc.moverate;}
    if (npc.ypos > npc.pathFinding.MyyPath) {npc.ypos = npc.ypos - npc.moverate;}
    if (npc.ypos < npc.pathFinding.MyyPath) {npc.ypos = npc.ypos + npc.moverate;}*/

                    //npc.pathFinding.MyFindPath(npc.xpos,npc.ypos,npc.TargetPos.x,npc.TargetPos.y);

                    if (npc.id == gvars::myTargetid)
                    {
                        for (int Stuff = npc.pathFinding.mypathLocation;
                             Stuff != npc.pathFinding.mypathLength; ++Stuff)
                        {
                            if (Stuff != 1)
                            {
                                effects.createLine(
                                    npc.pathFinding.myReadPathX(1, Stuff - 1),
                                    npc.pathFinding.myReadPathY(1, Stuff - 1),
                                    npc.pathFinding.myReadPathX(1, Stuff),
                                    npc.pathFinding.myReadPathY(1, Stuff), 2,
                                    sf::Color::Blue);
                            }
                            //std::cout << npc.pathFinding.MyReadPathX(1,Stuff) << ":" << npc.pathFinding.MyReadPathY(1,Stuff) << std::endl;
                        }
                    }

                    if (npc.pathFinding.mypathLocation ==
                        npc.pathFinding.mypathLength)
                    {
                        if (abs(npc.xpos - npc.pathFinding.myxPath) <
                            npc.moverate)
                            npc.xpos = npc.pathFinding.myxPath;
                        if (abs(npc.ypos - npc.pathFinding.myyPath) <
                            npc.moverate)
                            npc.ypos = npc.pathFinding.myyPath;
                        npc.action = "Act";
                        npc.needsPath = false;
                    }

                    //npc.TargetPos.x = xPath[1];
                    //npc.TargetPos.y = yPath[1];
                }
            }
            for (auto &elem : npc.inventory)
            {
                if (elem.type == 4 &&
                    npc.hunger + elem.hungervalue <= npc.maxhunger &&
                    true == false)
                {
                    //if(Item->hungervalue > 0){ npc.fillhunger(1); Item->hungervalue--;}
                    npc.fillhunger(elem.hungervalue);
                    npc.fillthirst(elem.thirstvalue);
                    elem.toDelete = true;

                    std::string ChtStr;
                    ChtStr.append("* ");
                    ChtStr.append(npc.name);
                    ChtStr.append("(" + std::to_string(npc.id) + ")");
                    ChtStr.append(" has consumed ");
                    ChtStr.append(elem.name);
                    ChtStr.append(".");

                    chatBox.addChat(ChtStr, sf::Color(150, 150, 0));
                }
                if (elem.type == 5 &&
                    npc.thirst + elem.thirstvalue <= npc.maxthirst &&
                    true == false)
                {
                    //if(Item->hungervalue > 0){ npc.fillhunger(1); Item->hungervalue--;}
                    npc.fillhunger(elem.hungervalue);
                    npc.fillthirst(elem.thirstvalue);
                    elem.toDelete = true;

                    std::string ChtStr;
                    ChtStr.append("* ");
                    ChtStr.append(npc.name);
                    ChtStr.append("(" + std::to_string(npc.id) + ")");
                    ChtStr.append(" has consumed ");
                    ChtStr.append(elem.name);
                    ChtStr.append(".");

                    chatBox.addChat(ChtStr, sf::Color(150, 150, 0));
                }
            }
            if (gvars::debug)
                std::cout << "Post Item Usages. \n";

            unpointItems(npc.inventory);

            bool Done = false;

            while (Done == false)
            {
                bool Yet = false;
                for (auto it = npc.inventory.begin(); it != npc.inventory.end();
                     ++it)
                {
                    if (it->toDelete)
                    {
                        std::cout << it->name << " to be deleted. \n";
                        npc.inventory.erase(it);
                        Yet = true;
                        break;
                    }
                }
                if (Yet == false)
                {
                    Done = true;
                }
            }
            if (gvars::debug)
                std::cout << "Post Inventory Cleanup. \n";

        } // End of CanMove
        //npc.angle = Math::AngleBetweenVectors(Math::Vec(TempXpos,TempYpos),Math::Vec(npc.xpos,npc.ypos))-180;
        npc.angle = math::angleBetweenVectors(npc.targetPos,
                                              math::Vec2f(npc.xpos, npc.ypos));
        npc.momMove();
        //float f=Math::AngleBetweenVectors(sf::Vector2f(npc.xpos,npc.ypos), npc.TargetPos);npc.img.setRotation(f);
        if (gvars::debug)
            std::cout << npc.name << npc.id << " is done. \n";

        IntegerIterator++;
    }
    if (gvars::debug)
        std::cout << "Post NPC\n";
}

void drawTiles()
{
    int z = gvars::currentz;
    int iTS = GridSize;
    for (int i = 0; i <= gridy - 1; i++)
    {
        for (int t = 0; t <= gridx - 1; t++)
        {
            if ((gvars::following == true &&
                 i > (npclist.at(gvars::myTarget).xpos / GridSize) - 27 &&
                 i < (npclist.at(gvars::myTarget).xpos / GridSize) + 26) ||
                (i > gvars::currentx - 27 && i < gvars::currentx + 26))
            {
                if ((gvars::following == true &&
                     t > (npclist.at(gvars::myTarget).ypos / GridSize) - 21 &&
                     t < (npclist.at(gvars::myTarget).ypos / GridSize) + 20) ||
                    (t > gvars::currenty - 21 && t < gvars::currenty + 20))
                {
                    sf::Sprite Tile;
                    if (gvars::groundmap[gvars::currentz][i][t] == 1)
                    { // dirt
                        Tile.setTexture(texturemanager.getTexture("Dirt.bmp"));
                        Tile.setColor(
                            sf::Color(255, 255, 255, gvars::sunmap[z][i][t]));
                        Tile.setPosition(i * iTS, t * iTS);
                        window.draw(Tile);
                    }
                    if (gvars::groundmap[gvars::currentz][i][t] == 3)
                    { // grass
                        Tile.setTexture(texturemanager.getTexture("Grass.bmp"));
                        Tile.setColor(
                            sf::Color(255, 255, 255, gvars::sunmap[z][i][t]));
                        Tile.setPosition(i * iTS, t * iTS);
                        window.draw(Tile);
                    }
                    if (gvars::groundmap[gvars::currentz][i][t] == 7)
                    { // stone
                        Tile.setTexture(texturemanager.getTexture("Stone.bmp"));
                        Tile.setColor(
                            sf::Color(255, 255, 255, gvars::sunmap[z][i][t]));
                        Tile.setPosition(i * iTS, t * iTS);
                        window.draw(Tile);
                    }
                    if (gvars::groundmap[gvars::currentz][i][t] == 20)
                    { // water
                        Tile.setTexture(texturemanager.getTexture("Water.bmp"));
                        Tile.setColor(
                            sf::Color(255, 255, 255, gvars::sunmap[z][i][t]));
                        Tile.setPosition(i * iTS, t * iTS);
                        window.draw(Tile);
                    }
                    if (gvars::groundmap[gvars::currentz][i][t] == 53)
                    { // lava
                        Tile.setTexture(texturemanager.getTexture("Lava.bmp"));
                        Tile.setColor(
                            sf::Color(255, 255, 255, gvars::sunmap[z][i][t]));
                        Tile.setPosition(i * iTS, t * iTS);
                        window.draw(Tile);
                    }
                    if (gvars::groundmap[gvars::currentz][i][t] == 52)
                    { // deepwater
                        Tile.setTexture(
                            texturemanager.getTexture("DeepWater.bmp"));
                        Tile.setColor(
                            sf::Color(255, 255, 255, gvars::sunmap[z][i][t]));
                        Tile.setPosition(i * iTS, t * iTS);
                        window.draw(Tile);
                    }
                }
            }
        }
    }
}

void drawNPCs()
{
    for (auto &npc : npclist)
    {
        if (npc.hasSpawned == true)
        {
            if (npc.name == "Azabul")
            {
                sf::Vector2f TA1 = npc.tentArm1;
                sf::Vector2f TA2 = npc.tentArm2;
                sf::Vector2f TE1 = npc.tentEnd1;
                sf::Vector2f TE2 = npc.tentEnd2;

                effects.createLine(npc.xpos, npc.ypos, TA1.x + npc.xpos,
                                   TA1.y + npc.ypos, 2, sf::Color(0, 200, 200),
                                   1, sf::Color(0, 255, 255));
                effects.createLine(npc.xpos, npc.ypos, TA2.x + npc.xpos,
                                   TA2.y + npc.ypos, 2, sf::Color(0, 200, 200),
                                   1, sf::Color(0, 255, 255));
                effects.createLine(
                    TA1.x + npc.xpos, TA1.y + npc.ypos,
                    TE1.x + TA1.x + npc.xpos, TE1.y + TA1.y + npc.ypos, 2,
                    sf::Color(0, 200, 200), 1, sf::Color(0, 255, 255));
                effects.createLine(
                    TA2.x + npc.xpos, TA2.y + npc.ypos,
                    TE2.x + TA2.x + npc.xpos, TE2.y + TA2.y + npc.ypos, 2,
                    sf::Color(0, 200, 200), 1, sf::Color(0, 255, 255));
                effects.drawEffects();
            }

            int Alph = 255;
            npc.img.setColor(sf::Color(255, 255, 255, Alph));
            npc.img.setScale(gvars::scalex, gvars::scaley);
            npc.img.setOrigin(npc.img.getTextureRect().width / 2,
                              npc.img.getTextureRect().height / 2);
            npc.drawImg();
            effects.createCircle(npc.xpos, npc.ypos, npc.size,
                                 sf::Color(50, 50, 50, 50));
        }
    }
    debug("Done drawing NPCs");
}

void drawItems()
{
    //App.setActive(true);

    for (auto &worlditem : worlditems)
    {
        //if(zit->xpos/GridSize > globals::currentx-27 && zit->xpos/GridSize < globals::currentx+26 && zit->ypos/GridSize > globals::currenty-20 && zit->ypos/GridSize < globals::currenty+20)
        //{
        worlditem.img.setColor(sf::Color(255, 255, 255, 255));
        worlditem.img.setScale(gvars::scalex, gvars::scaley);
        worlditem.drawImg();
        //}
    }
    debug("Done Drawing Items");
}

void lightTrail(int x, int y, int z)
{
    int curx = math::clamp(x, 0, gridx - 1);
    int cury = math::clamp(y, 0, gridy - 1);
    int curz = math::clamp(z, 0, gridz - 1);
    gvars::sunmap[curz][curx][cury] = 255;
}

void displayChat(sf::Vector2f Position)
{
    if (gCtrl.phase != "MainMenu")
        effects.createSquare(
            Position.x - 10, Position.y + 10, Position.x + 500,
            (Position.y - ((chatBox.chatStorage.size() + 1) * 10)),
            sf::Color(0, 0, 0, 100), 2, sf::Color::Cyan);

    for (size_t i = 0; i != chatBox.chatStorage.size(); i++)
    {
        textList.createText(
            Position.x,
            (Position.y - ((chatBox.chatStorage.size()) * 10)) + (i * 10), 11,
            chatBox.chatStorage[i].color, chatBox.chatStorage[i].line);
    }
}

void drawStuffs()
{

    //sf::Context context;
    //App.setActive(true);
    drawNewTiles();

    //DrawPlanets();
    drawItems();

    drawNPCs();

    drawJobList(window.getView().getCenter().x - 500,
                window.getView().getCenter().y);
    debug("Drew Joblist");

    displayChat(sf::Vector2f(gvars::bottomLeft.x + 5, gvars::bottomLeft.y - 5));
    debug("Drew Chat");

    effects.drawEffects();
    debug("Drew Effects");
    //    DrawPlanets();//Remove this one, Reenable previous

    for (auto &button : vSquareButtonList)
    {
        button.draw();
    }
    vSquareButtonList.clear();
    debug("Drew and Cleared square buttons");

    textList.drawTextz();
    debug("Drew Text");

    for (auto &button : vButtonList)
    {
        button.draw();
    }
    vButtonList.clear();
    debug("Drew and Cleared buttons");

    gvars::drawStuffsDone = true;
}

Item *getGlobalItem(std::string strtype)
{
    if (gvars::debug)
    {
        std::cout << "Getting" << strtype << " \n";
    }
    for (auto &elem : itemmanager.globalItems)
    {
        if (elem.name == strtype)
        {
            if (gvars::debug)
            {
                std::cout << "Found" << strtype << " \n";
            }
            return &elem;
        }
    }
    if (gvars::debug)
    {
        std::cout << "Didn't Find" << strtype << " \n";
    }

    return nullptr;
}

Npc *getCritter(int id)
{
    if (gvars::debug)
    {
        std::cout << "Getting critter(" << id << ") \n";
    }
    for (auto &elem : npclist)
    {
        if (elem.id == id)
        {
            if (gvars::debug)
            {
                std::cout << "Found critter(" << id << ") \n";
            }
            return &elem;
        }
    }
    if (gvars::debug)
    {
        std::cout << "Didn't Find critter(" << id << ") \n";
    }
    return nullptr;
}

void removeNPCs()
{
    bool Done = false;
    while (Done == false)
    {
        bool Yet = false;
        for (auto it = npclist.begin(); it != npclist.end(); ++it)
        {
            if (it->toDelete)
            {
                std::cout << it->name << " to be deleted. \n";
                npclist.erase(it);
                Yet = true;
                break;
            }
        }
        if (Yet == false)
        {
            Done = true;
        }
    }
}

int main()
{
    window.create(sf::VideoMode(Rez.x, Rez.y, 32), randomWindowName());

    textList.loadFont();

    bool Paused = false;
    //Debug = true;

    bool PlyAct = false;
    window.setFramerateLimit(30); // 0 is unlimited
    sf::View Planetary(Center, HalfSize);

    window.setVerticalSyncEnabled(true);

    // Various temporary variables used for testing.
    int testage = 0;
    int testage2 = 0;
    float Xxx = 0;                      // global
    float Yyy = 0;                      // global
    int speeds = 1;                     // global
    int xanchor = 0;                    // global
    int yanchor = 0;                    // global
    float Degrees = randz(.0f, 359.0f); // global
    int radius = 200;

    gvars::view1.zoom(2);
    if (true == false)
    {   // TODO: Fix this icon crap.
        /*sf::Image icon;
    icon.LoadFromFile("gfx/icon.tga");
    App.SetIcon(32,32,icon.GetPixelsPtr());
    if (!Images.LoadFromFile("gfx/Images.tga"))return EXIT_FAILURE;
    sf::Image Aim;
    if (!Aim.LoadFromFile("gfx/Aim.tga"))return EXIT_FAILURE;*/
    }

    texturemanager.init();
    itemmanager.initializeItems();
    npcmanager.initializeCritters();

    // Building the players faction, This is temporary.
    g_pf.name = "The Alphas";
    g_pf.playerControlled = true;
    g_pf.initialized = true;
    uniFact.push_back(g_pf);
    conFact = &uniFact[0];

    // Setting the initial game phase.
    gCtrl.phase = "MainMenu";

    // For A*
    astar::init();

    while (window.isOpen())
    {
        if (gvars::cycleGrowth)
        {
            gvars::cycleRed.g++;
            gvars::cycleRed.b++;
            gvars::cycleGreen.r++;
            gvars::cycleGreen.b++;
            gvars::cycleBlue.r++;
            gvars::cycleBlue.g++;
            if (gvars::cycleRed.g >= 255)
                gvars::cycleGrowth = false;
        }
        else
        {
            gvars::cycleRed.g--;
            gvars::cycleRed.b--;
            gvars::cycleGreen.r--;
            gvars::cycleGreen.b--;
            gvars::cycleBlue.r--;
            gvars::cycleBlue.g--;
            if (gvars::cycleRed.g <= 0)
                gvars::cycleGrowth = true;
        }

        removeNPCs();
        sf::Event Event;
        while (window.pollEvent(Event))
        {
            if (Event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (Event.type == sf::Event::MouseWheelMoved)
            {
                if (Event.mouseWheel.delta > 0)
                {
                    std::cout << "Zoom Out \n";
                    if (gvars::cameraZoom < 2)
                    {
                        gvars::cameraZoom = gvars::cameraZoom / 0.5;
                        //CameraSize
                        gvars::view1.zoom(0.5);
                    }
                }
                if (Event.mouseWheel.delta < 0)
                {
                    std::cout << "Zoom In \n";
                    if (gvars::cameraZoom > 0.5)
                    {
                        gvars::cameraZoom = gvars::cameraZoom / 2;
                        gvars::view1.zoom(2);
                    }
                }

                //Develop the scaling camera, Probably using .setsize instead of zoom.
                //http://www.sfml-dev.org/tutorials/2.0/graphics-view.php
            }

            if (Event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                //sf::FloatRect visibleArea(0, 0, Event.size.width, Event.size.height);
                //View1.setViewport(visibleArea);
                //App.setView(sf::View(visibleArea));
            }

            if (Event.type == sf::Event::LostFocus)
            {
                gvars::inFocus = false;
            }
            if (Event.type == sf::Event::GainedFocus)
            {
                gvars::inFocus = true;
            }
        }
        window.setView(gvars::view1);
        gvars::buttonClicked = false;
        gvars::buttonClickedTime--; // Misleading Variable name, Sorry!
        if (gvars::buttonClickedTime < 0)
            gvars::buttonClickedTime = 0;

        key.update();
        sf::Vector2f MouseStagnationCheck = gvars::mousePos;

        gvars::mousePos =
            window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (MouseStagnationCheck == gvars::mousePos)
            gvars::mouseStagnation++;
        else
            gvars::mouseStagnation = 0;

        gvars::topLeft = sf::Vector2f(gvars::view1.getCenter().x - HalfSize.x,
                                      gvars::view1.getCenter().y - HalfSize.y);
        gvars::topRight = sf::Vector2f(gvars::view1.getCenter().x + HalfSize.x,
                                       gvars::view1.getCenter().y - HalfSize.y);
        gvars::bottomLeft =
            sf::Vector2f(gvars::view1.getCenter().x - HalfSize.x,
                         gvars::view1.getCenter().y + HalfSize.y);
        gvars::bottomRight =
            sf::Vector2f(gvars::view1.getCenter().x + HalfSize.x,
                         gvars::view1.getCenter().y + HalfSize.y);

        textList.createText(CZ(gvars::topRight.x - CZ(50)),
                            CZ(gvars::topRight.y + CZ(50)), CZ(11),
                            sf::Color::White, "x", "", gvars::cameraZoom);
        textList.createText((gvars::topRight.x - 50), (gvars::topRight.y + 50),
                            (11) / gvars::cameraZoom, sf::Color::White, "x", "",
                            gvars::cameraZoom);

        if (key.kTime == 1)
        { // Generates a random name from GenerateName(); and puts it into the console.
            std::cout << generateName() << std::endl;
        }

        if (key.gTime == 1)
        { // Fling all critters south.
            for (auto &i : npclist)
            {
                i.momentum = sf::Vector2f(0, 100);
            }
        }
        if (key.hTime == 1)
        { // Fling all critters north.
            for (auto &i : npclist)
            {
                i.momentum = sf::Vector2f(0, -100);
            }
        }

        // Game Mode Loops ================================================================================

        if (key.r)
        { // Debug (de)activation
            if (!gvars::debug)
            {
                gvars::debug = true;
                fSleep(0.2);
            }
            else if (gvars::debug)
            {
                gvars::debug = false;
                fSleep(0.2);
            }
        }
        if (gCtrl.phase == "Local")
        { //=======================================================*Local*============================================================================
            if (gvars::debug)
                textList.createText((gvars::currentx - 2) * GridSize,
                                    (gvars::currenty + 1) * GridSize, 11,
                                    sf::Color::Red, "Debug On");

            if (key.vTime == 1)
                chatBox.addChat(
                    randomWindowName(),
                    sf::Color(randz(0, 255), randz(0, 255), randz(0, 255)));

            squadHud();

            if (key.lshiftTime > 0)
            {
                int x = gvars::bottomLeft.x;
                int y = gvars::bottomLeft.y;
                effects.createSquare(x + 20, y - 20, x + 40, y - 40,
                                     sf::Color(0, 100, 255));
                textList.createText(x + 20, y - 30, 11, sf::Color::White,
                                    "Build");
            }
            if (key.rmbTime == 1 && key.lshift)
                rmbMenuTile(gvars::mousePos);

            if (key.lshift && key.tab) // Debug NPC Spawn Menu
            {

                gCtrl.menuType = "DebugCritterSpawn";
                menuPopUp();
            }
            else if (key.tab == true &&
                     key.lshift ==
                         false) // Debug Item Spawn Menu TODO: Sort them by Type.
            {
                gCtrl.menuType = "DebugItemSpawn";
                menuPopUp();
            }
            else if (gCtrl.menuType == "DebugItemSpawn" ||
                     gCtrl.menuType == "DebugCritterSpawn")
            {
                gCtrl.menuType = "NULL";
                gCtrl.menuPos = sf::Vector2f(-10000, -10000);
                gCtrl.menuEndPos = sf::Vector2f(-10000, -10000);
                menuPtrCon.pItem = nullptr;
                menuPtrCon.pNPC = nullptr;
                menuPtrCon.pTile = nullptr;
                con("Closing DebugMenus Due To Lack of Key Presses");
            }
            //                       if(Key.lshift && Key.z){GC.ZombieSwarmLocal();}

            if (key.lshift && key.z) // Debug Stuffs Menu
            {
                gCtrl.menuType = "DebugFunctions";
                menuPopUp();
            }

            if (key.left == true)
            {
                gvars::currentx--;
                PlyAct = true;
            }
            if (key.right == true)
            {
                gvars::currentx++;
                PlyAct = true;
            }
            if (key.up == true)
            {
                gvars::currenty--;
                PlyAct = true;
            }
            if (key.down == true)
            {
                gvars::currenty++;
                PlyAct = true;
            }

            if (gvars::initalZeds)
                gCtrl.wave();

            rightMouseButtonContextMenu();

            if (key.lshift == true && key.left == true)
            {
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                PlyAct = true;
            } //Sprite.Move(-100 * ElapsedTime, 0);
            if (key.lshift == true && key.right == true)
            {
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                PlyAct = true;
            } //Sprite.Move( 100 * ElapsedTime, 0);
            if (key.lshift == true && key.up == true)
            {
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                PlyAct = true;
            } //Sprite.Move(0, -100 * ElapsedTime);
            if (key.lshift == true && key.down == true)
            {
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                PlyAct = true;
            } //Sprite.Move(0,  100 * ElapsedTime);
            if (key.comma == true && key.lshift == true &&
                gvars::currentz <= gridz - 1)
            {
                gvars::currentz++;
                PlyAct = true;
                fSleep(0.1f);
            } //Sprite.Move(0, -100 * ElapsedTime);
            if (key.period == true && key.lshift == true &&
                gvars::currentz >= 1)
            {
                gvars::currentz--;
                PlyAct = true;
                fSleep(0.1f);
            } //Sprite.Move(0,  100 * ElapsedTime);
            if (key.comma == true && key.rshift == true &&
                gvars::currentz <= gridz - 1)
            {
                gvars::currentz++;
                PlyAct = true;
                fSleep(0.1f);
            } //Sprite.Move(0, -100 * ElapsedTime);
            if (key.period == true && key.rshift == true &&
                gvars::currentz >= 1)
            {
                gvars::currentz--;
                PlyAct = true;
                fSleep(0.1f);
            } //Sprite.Move(0,  100 * ElapsedTime);
            if (gvars::myTarget == -1)
            {
                gvars::following = false;
            }
            else if (key.c)
            {
                gvars::following = true;
            }
            if (gvars::following)
            {
                gvars::view1.setCenter(npclist.at(gvars::myTarget).xpos,
                                       npclist.at(gvars::myTarget).ypos);
            }

            effects.createSquare(
                32 * 20, 32 * 20, 64 * 20, 64 * 20, sf::Color(0, 0, 0, 0), 1,
                sf::Color::
                    Cyan); // These two are the chunk borders, Debug stuffs so you can see where the loading boundries are. (As if you needed 'em, Damn thing lags a bit, Should thread each loading bit soon.)
            effects.createSquare(
                0 * 20, 0 * 20, 32 * 20, 32 * 20, sf::Color(0, 0, 0, 0), 1,
                sf::Color::
                    Red); // This one reachs from 0 to 32 only because it draws from the left, not the center.

            if (key.lctrlTime > 10)
            {
                int Variable =
                    tiles[abs_to_index(gvars::mousePos.x / 20)][abs_to_index(
                        gvars::mousePos.y / 20)][30].id;
                textList.createText(gvars::mousePos.x, gvars::mousePos.y, 11,
                                    sf::Color::Red, "", "", Variable);
            }

            bool Transitioning = false;
            if (gvars::currenty > 64)
            {
                tilesGoUp();
                gvars::currenty = 33;

                for (size_t i = 0; i != npclist.size(); i++)
                {
                    npclist.at(i).ypos += -640;
                    npclist.at(i).targetPos.y += -640;
                }

                //for(int i = 0; i != worlditems.size(); i++) worlditems.at(i).ypos += -640;
                for (auto &worlditem : worlditems)
                    (worlditem).ypos += -640;

                for (size_t i = 0; i != uniFact[0].jobList.size(); i++)
                    uniFact[0].jobList[i].workPos.y += -640;

                Transitioning = true;
                std::string Line;
                Line = loadCritters(sf::Vector2i(gvars::currentregionx - 1,
                                                 gvars::currentregiony + 1),
                                    "BottomLeft", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadCritters(sf::Vector2i(gvars::currentregionx,
                                                 gvars::currentregiony + 1),
                                    "Bottom", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadCritters(sf::Vector2i(gvars::currentregionx + 1,
                                                 gvars::currentregiony + 1),
                                    "BottomRight", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadItems(sf::Vector2i(gvars::currentregionx - 1,
                                              gvars::currentregiony + 1),
                                 "BottomLeft", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadItems(sf::Vector2i(gvars::currentregionx,
                                              gvars::currentregiony + 1),
                                 "Bottom", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadItems(sf::Vector2i(gvars::currentregionx + 1,
                                              gvars::currentregiony + 1),
                                 "BottomRight", gvars::currentplanet);
                remove(Line.c_str());
            }
            if (gvars::currenty < 32)
            {
                tilesGoDown();
                gvars::currenty = 63;
                for (size_t i = 0; i != npclist.size(); i++)
                {
                    npclist.at(i).ypos += 640;
                    npclist.at(i).targetPos.y += 640;
                }

                //for(int i = 0; i != worlditems.size(); i++) worlditems.at(i).ypos += 640;
                for (auto &worlditem : worlditems)
                    (worlditem).ypos += 640;

                for (size_t i = 0; i != uniFact[0].jobList.size(); i++)
                    uniFact[0].jobList[i].workPos.y += 640;

                Transitioning = true;
                std::string Line;
                Line = loadCritters(sf::Vector2i(gvars::currentregionx - 1,
                                                 gvars::currentregiony - 1),
                                    "TopLeft", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadCritters(sf::Vector2i(gvars::currentregionx,
                                                 gvars::currentregiony - 1),
                                    "Top", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadCritters(sf::Vector2i(gvars::currentregionx + 1,
                                                 gvars::currentregiony - 1),
                                    "TopRight", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadItems(sf::Vector2i(gvars::currentregionx - 1,
                                              gvars::currentregiony - 1),
                                 "TopLeft", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadItems(sf::Vector2i(gvars::currentregionx,
                                              gvars::currentregiony - 1),
                                 "Top", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadItems(sf::Vector2i(gvars::currentregionx + 1,
                                              gvars::currentregiony - 1),
                                 "TopRight", gvars::currentplanet);
                remove(Line.c_str());
            }
            if (gvars::currentx > 64)
            {
                tilesGoLeft();
                gvars::currentx = 33;
                for (size_t i = 0; i != npclist.size(); i++)
                {
                    npclist.at(i).xpos += -640;
                    npclist.at(i).targetPos.x += -640;
                }

                //for(int i = 0; i != worlditems.size(); i++) worlditems.at(i).xpos += -640;
                for (auto &worlditem : worlditems)
                    (worlditem).xpos += -640;

                for (size_t i = 0; i != uniFact[0].jobList.size(); i++)
                    uniFact[0].jobList[i].workPos.x += -640;

                Transitioning = true;
                std::string Line;
                Line = loadCritters(sf::Vector2i(gvars::currentregionx + 1,
                                                 gvars::currentregiony - 1),
                                    "TopRight", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadCritters(sf::Vector2i(gvars::currentregionx + 1,
                                                 gvars::currentregiony),
                                    "Right", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadCritters(sf::Vector2i(gvars::currentregionx + 1,
                                                 gvars::currentregiony + 1),
                                    "BottomRight", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadItems(sf::Vector2i(gvars::currentregionx + 1,
                                              gvars::currentregiony - 1),
                                 "TopRight", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadItems(sf::Vector2i(gvars::currentregionx + 1,
                                              gvars::currentregiony),
                                 "Right", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadItems(sf::Vector2i(gvars::currentregionx + 1,
                                              gvars::currentregiony + 1),
                                 "BottomRight", gvars::currentplanet);
                remove(Line.c_str());
            }
            if (gvars::currentx < 32)
            {
                con("Starting GoRight");
                tilesGoRight();
                con("Ending GoRight");
                gvars::currentx = 63;
                con("Starting GoRight with NPC's and Items");
                for (size_t i = 0; i != npclist.size(); i++)
                {
                    npclist.at(i).xpos += 640;
                    npclist.at(i).targetPos.x += 640;
                }

                //for(int i = 0; i != worlditems.size(); i++) worlditems.at(i).xpos += 640;
                for (auto &worlditem : worlditems)
                    (worlditem).xpos += 640;

                for (size_t i = 0; i != uniFact[0].jobList.size(); i++)
                    uniFact[0].jobList[i].workPos.x += 640;

                con("Done GoRight with NPC's and Items");
                Transitioning = true;
                std::string Line;
                Line = loadCritters(sf::Vector2i(gvars::currentregionx - 1,
                                                 gvars::currentregiony - 1),
                                    "TopLeft", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadCritters(sf::Vector2i(gvars::currentregionx - 1,
                                                 gvars::currentregiony),
                                    "Left", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadCritters(sf::Vector2i(gvars::currentregionx - 1,
                                                 gvars::currentregiony + 1),
                                    "BottomLeft", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadItems(sf::Vector2i(gvars::currentregionx - 1,
                                              gvars::currentregiony - 1),
                                 "TopLeft", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadItems(sf::Vector2i(gvars::currentregionx - 1,
                                              gvars::currentregiony),
                                 "Left", gvars::currentplanet);
                remove(Line.c_str());
                Line = loadItems(sf::Vector2i(gvars::currentregionx - 1,
                                              gvars::currentregiony + 1),
                                 "BottomLeft", gvars::currentplanet);
                remove(Line.c_str());
            }

            if (Transitioning == true)
            {
                for (size_t i = 0; i != npclist.size(); i++)
                {

                    if (npclist.at(i).xpos > 1920 && npclist.at(i).ypos < 640)
                    {
                        npclist.at(i).xpos =
                            npclist.at(i).xpos - 640 - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx + 2,
                                                  gvars::currentregiony - 1),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }
                    else if (npclist.at(i).xpos > 1920 &&
                             npclist.at(i).ypos > 1280)
                    {
                        npclist.at(i).xpos =
                            npclist.at(i).xpos - 640 - 640 - 640;
                        npclist.at(i).ypos = npclist.at(i).ypos - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx + 2,
                                                  gvars::currentregiony + 1),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }

                    else if (npclist.at(i).xpos < 0 &&
                             npclist.at(i).ypos > 1280)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos + 640;
                        npclist.at(i).ypos = npclist.at(i).ypos - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx - 2,
                                                  gvars::currentregiony + 1),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }
                    else if (npclist.at(i).xpos < 0 && npclist.at(i).ypos < 640)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos + 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx - 2,
                                                  gvars::currentregiony - 1),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }

                    else if (npclist.at(i).ypos < 0 &&
                             npclist.at(i).xpos > 1280)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos - 640 - 640;
                        npclist.at(i).ypos = npclist.at(i).ypos + 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx + 1,
                                                  gvars::currentregiony - 2),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }
                    else if (npclist.at(i).ypos < 0 && npclist.at(i).xpos < 640)
                    {
                        npclist.at(i).ypos = npclist.at(i).ypos + 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx - 1,
                                                  gvars::currentregiony - 2),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }

                    else if (npclist.at(i).ypos > 1920 &&
                             npclist.at(i).xpos > 1280)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos - 640 - 640;
                        npclist.at(i).ypos =
                            npclist.at(i).ypos - 640 - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx + 1,
                                                  gvars::currentregiony + 2),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }
                    else if (npclist.at(i).ypos > 1920 &&
                             npclist.at(i).xpos < 640)
                    {
                        npclist.at(i).ypos =
                            npclist.at(i).ypos - 640 - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx - 1,
                                                  gvars::currentregiony + 2),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }

                    //HAGGINABAGGINA  Some reason, When a critter is saved, It'll have more than 640 for it's position, This is unacceptable.

                    else if (npclist.at(i).xpos > 1920)
                    {
                        npclist.at(i).xpos =
                            npclist.at(i).xpos - 640 - 640 - 640;
                        npclist.at(i).ypos = npclist.at(i).ypos - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx + 2,
                                                  gvars::currentregiony),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }
                    else if (npclist.at(i).ypos > 1920)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos - 640;
                        npclist.at(i).ypos =
                            npclist.at(i).ypos - 640 - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx,
                                                  gvars::currentregiony + 2),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }
                    else if (npclist.at(i).xpos < 0)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos + 640;
                        npclist.at(i).ypos = npclist.at(i).ypos - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx - 2,
                                                  gvars::currentregiony),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }
                    else if (npclist.at(i).ypos < 0)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos - 640;
                        npclist.at(i).ypos = npclist.at(i).ypos + 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx,
                                                  gvars::currentregiony - 2),
                                npclist.at(i));

                        npclist.at(i).toDelete = true;
                    }
                }

                removeNPCs();
                /*
                for(int i = 0; i != worlditems.size(); i++)
                {
                   if(worlditems.at(i).xpos > 1920)
                    {
                        SaveItem(500,sf::Vector2i(globals::currentregionx+2,globals::currentregiony),worlditems.at(i));
                        worlditems.at(i).ToDelete = true;
                    }
                    if(worlditems.at(i).ypos > 1920)
                    {
                        SaveItem(500,sf::Vector2i(globals::currentregionx,globals::currentregiony+2),worlditems.at(i));
                        worlditems.at(i).ToDelete = true;
                    }
                    if(worlditems.at(i).xpos < 0)
                    {
                        SaveItem(500,sf::Vector2i(globals::currentregionx-2,globals::currentregiony),worlditems.at(i));
                        worlditems.at(i).ToDelete = true;
                    }
                    if(worlditems.at(i).ypos < 0)
                    {
                        SaveItem(500,sf::Vector2i(globals::currentregionx,globals::currentregiony-2),worlditems.at(i));

                        worlditems.at(i).ToDelete = true;
                    }
                }
                */

                //for(int i = 0; i != worlditems.size(); i++)
                for (auto &worlditem : worlditems)
                {

                    if ((worlditem).xpos > 1920 && (worlditem).ypos < 640)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640 - 640 - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx + 2,
                                                   gvars::currentregiony - 1),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }
                    else if ((worlditem).xpos > 1920 && (worlditem).ypos > 1280)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640 - 640 - 640;
                        (worlditem).ypos = (worlditem).ypos - 640 - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx + 2,
                                                   gvars::currentregiony + 1),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }

                    else if ((worlditem).xpos < 0 && (worlditem).ypos > 1280)
                    {
                        (worlditem).xpos = (worlditem).xpos + 640;
                        (worlditem).ypos = (worlditem).ypos - 640 - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx - 2,
                                                   gvars::currentregiony + 1),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }
                    else if ((worlditem).xpos < 0 && (worlditem).ypos < 640)
                    {
                        (worlditem).xpos = (worlditem).xpos + 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx - 2,
                                                   gvars::currentregiony - 1),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }

                    else if ((worlditem).ypos < 0 && (worlditem).xpos > 1280)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640 - 640;
                        (worlditem).ypos = (worlditem).ypos + 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx + 1,
                                                   gvars::currentregiony - 2),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }
                    else if ((worlditem).ypos < 0 && (worlditem).xpos < 640)
                    {
                        (worlditem).ypos = (worlditem).ypos + 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx - 1,
                                                   gvars::currentregiony - 2),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }

                    else if ((worlditem).ypos > 1920 && (worlditem).xpos > 1280)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640 - 640;
                        (worlditem).ypos = (worlditem).ypos - 640 - 640 - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx + 1,
                                                   gvars::currentregiony + 2),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }
                    else if ((worlditem).ypos > 1920 && (worlditem).xpos < 640)
                    {
                        (worlditem).ypos = (worlditem).ypos - 640 - 640 - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx - 1,
                                                   gvars::currentregiony + 2),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }

                    //HAGGINABAGGINA  Some reason, When a critter is saved, It'll have more than 640 for it's position, This is unacceptable.

                    else if ((worlditem).xpos > 1920)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640 - 640 - 640;
                        (worlditem).ypos = (worlditem).ypos - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx + 2,
                                                   gvars::currentregiony),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }
                    else if ((worlditem).ypos > 1920)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640;
                        (worlditem).ypos = (worlditem).ypos - 640 - 640 - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx,
                                                   gvars::currentregiony + 2),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }
                    else if ((worlditem).xpos < 0)
                    {
                        (worlditem).xpos = (worlditem).xpos + 640;
                        (worlditem).ypos = (worlditem).ypos - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx - 2,
                                                   gvars::currentregiony),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }
                    else if ((worlditem).ypos < 0)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640;
                        (worlditem).ypos = (worlditem).ypos + 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx,
                                                   gvars::currentregiony - 2),
                                 (worlditem));

                        (worlditem).toDelete = true;
                    }
                }
                unpointItems(worlditems);
                removeItems(worlditems);
            }

            if (gvars::myTarget != -1 && key.rmb &&
                tiles[abs_to_index(gvars::mousePos.x / GridSize)][abs_to_index(
                    gvars::mousePos.y / GridSize)][30].id != 1010)
            { // Giving Orders
                npclist.at(gvars::myTarget).targetPos = gvars::mousePos;
                npclist.at(gvars::myTarget).action = "Orders";
                if (math::closeish(npclist.at(gvars::myTarget).xpos,
                                   npclist.at(gvars::myTarget).ypos,
                                   gvars::mousePos.x, gvars::mousePos.y) <= 10)
                {
                    npclist.at(gvars::myTarget).action = "Act";
                    npclist.at(gvars::myTarget).needsPath = false;
                }

                for (auto const &item : worlditems)
                {
                    if (math::closeish(gvars::mousePos.x, gvars::mousePos.y,
                                       item.xpos, item.ypos) <= 10)
                    {
                        gCtrl.menuType = "CritterContext";
                    }
                }
            } //End of Giving Orders

            if (gCtrl.menuType != "NULL")
            {
                menuPopUp();
            }
            else
            {
                gCtrl.menuPos = math::Vec2f(-10000, -10000);
            }
        } //=============================================================================*End of Local*========================================================================
        if (gCtrl.phase == "Solar")
        { //=======================================================*Solar*============================================================================
            if (key.lshift)
            {
                /*std::vector<planet>::iterator Me;
                for(Me = Planets.begin(); Me != Planets.end(); ++Me )
                {
                    std::cout<<"Position: X:"<<Me->Pos.x<<" Y:"<<Me->Pos.y<<std::endl;
                    std::cout<<"Speed: X:"<<Me->speed.x<<" Y:"<<Me->speed.y<<std::endl;
                }*/
            }

            // TODO: Fix Later if(Key.LMB && Key.lctrl){ GC.CreatePlanet(MousePos.x,MousePos.y,randz(10,200));sf::Sleep(0.1);}
            // TODO: Fix Later            if(Key.RMB && Key.lctrl){ Planets.pop_back(); sf::Sleep(1); }
            // TODO: Fix Laterif(Key.r && Key.lctrl){ Planets.clear(); sf::Sleep(1); }
            // TODO: Fix Laterif(Key.q){aim--; std::cout << aim << std::endl; sf::Sleep(0.2);}
            // TODO: Fix Laterif(Key.e){aim++; std::cout << aim << std::endl; sf::Sleep(0.2);}

            if (key.left == true)
            {
                gvars::currentx--;
                PlyAct = true;
            }
            if (key.right == true)
            {
                gvars::currentx++;
                PlyAct = true;
            }
            if (key.up == true)
            {
                gvars::currenty--;
                PlyAct = true;
            }
            if (key.down == true)
            {
                gvars::currenty++;
                PlyAct = true;
            }
            if (key.lshift == true && key.left == true)
            {
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                PlyAct = true;
            } //Sprite.Move(-100 * ElapsedTime, 0);
            if (key.lshift == true && key.right == true)
            {
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                PlyAct = true;
            } //Sprite.Move( 100 * ElapsedTime, 0);
            if (key.lshift == true && key.up == true)
            {
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                PlyAct = true;
            } //Sprite.Move(0, -100 * ElapsedTime);
            if (key.lshift == true && key.down == true)
            {
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                PlyAct = true;
            } //Sprite.Move(0,  100 * ElapsedTime);

        } //=============================================================================*End of Solar*========================================================================
        if (gCtrl.phase == "Test")
        {
            if (key.up)
                testage++;
            if (key.down)
                testage--;
            if (key.right)
                testage2++;
            if (key.left)
                testage2--;
            if (key.pad2)
                radius++;
            if (key.pad8)
                radius--;

            if (gvars::currenty >
                64) // TODO: Make the auto removing tiles use the current windows border to get it's range, Allowing proper resizing and stuffs. Edit: Herp, That's not what this is.
            {
                tilesGoUp();
                gvars::currenty = 33;
            }
            if (gvars::currenty < 32)
            {
                tilesGoDown();
                gvars::currenty = 63;
            }
            if (gvars::currentx > 64)
            {
                tilesGoLeft();
                gvars::currentx = 33;
            }
            if (gvars::currentx < 32)
            {
                tilesGoRight();
                gvars::currentx = 63;
            }

            if (key.g)
                initalizeWorldTiles();
            //DrawNewTiles();

            drawWorldTiles();
            if (key.j)
                tilesGoUp();
            if (key.k)
                tilesRandom();

            if (key.m)
            {
                std::set<int> Setage;
                Setage.insert(1);
                Setage.insert(1);
                Setage.insert(4);
                Setage.insert(2);
                Setage.insert(1);
                Setage.insert(3);
                for (const auto &elem : Setage)
                {
                    std::cout << elem << "\n";
                }
            }

            if (key.left == true)
            {
                gvars::currentx--;
                PlyAct = true;
            }
            if (key.right == true)
            {
                gvars::currentx++;
                PlyAct = true;
            }
            if (key.up == true)
            {
                gvars::currenty--;
                PlyAct = true;
            }
            if (key.down == true)
            {
                gvars::currenty++;
                PlyAct = true;
            }
            if (key.lshift == true && key.left == true)
            {
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                PlyAct = true;
            } //Sprite.Move(-100 * ElapsedTime, 0);
            if (key.lshift == true && key.right == true)
            {
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                PlyAct = true;
            } //Sprite.Move( 100 * ElapsedTime, 0);
            if (key.lshift == true && key.up == true)
            {
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                PlyAct = true;
            } //Sprite.Move(0, -100 * ElapsedTime);
            if (key.lshift == true && key.down == true)
            {
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                PlyAct = true;
            } //Sprite.Move(0,  100 * ElapsedTime);

            //button var; var.Color = sf::Color::Red; var.iSize = 5; var.vPos = sf::Vector2f(200,200); var.sButtonText = "Howdy"; vButtonList.push_back(var);
            // TODO: Fix Later    if(ButtonClicked(var.id)){ std::cout << "Starting the Building. \n"; GC.BuildTileTest(); std::cout << "Done Building. \n"; sf::Sleep(1); GC.bTest = true; }

            Degrees += speeds; // Should probably leave speed at 1, If not less.

            Xxx = xanchor + cosf(Degrees * PI / 180) * radius;
            Yyy = yanchor + sinf(Degrees * PI / 180) * radius;
            effects.createCircle(xanchor, yanchor, 5, sf::Color::Blue);
            effects.createCircle(Xxx, Yyy, 5, sf::Color::White);

            int distence = math::closeish(xanchor, yanchor, Xxx, Yyy);
            textList.createText(xanchor, yanchor, 11, sf::Color::White,
                                "Distence:", "", distence);
            textList.createText(xanchor, yanchor + 11, 11, sf::Color::White,
                                "Radius:", "", radius);

            //std::cout << testmonkey << std::endl;
            if (gCtrl.bTest == true)
            {
                // TODO: Fix Later
                /*for(T = Tiles.begin(); T != Tiles.end(); T++){

        if( T->ID == 1 ){ // dirt
        int iTS = 20;
        sf::Sprite Tile(Images);
        Tile.SetColor(sf::Color(255,255,255,255));
        int imgposx = 2;int imgposy = 1;
        Tile.SetSubRect(sf::IntRect(((imgposx-1)*20)+imgposx, ((imgposy-1)*20)+imgposy, (imgposx*20)+imgposx, (imgposy*20)+imgposy));
        Tile.SetPosition(abs(T->LocPos.x)*iTS, abs(T->LocPos.y)*iTS);
        Tile.SetColor(sf::Color(255,255,255,255));
        App.Draw(Tile);}

        if( T->ID == 3 ){ // dirt
        int iTS = 20;
        sf::Sprite Tile(Images);
        Tile.SetColor(sf::Color(255,255,255,255));
        int imgposx = 1;int imgposy = 1;
        Tile.SetSubRect(sf::IntRect(((imgposx-1)*20)+imgposx, ((imgposy-1)*20)+imgposy, (imgposx*20)+imgposx, (imgposy*20)+imgposy));
        Tile.SetPosition(abs(T->LocPos.x)*iTS, abs(T->LocPos.y)*iTS);
        Tile.SetColor(sf::Color(255,255,255,255));
        App.Draw(Tile);}

        if( T->ID == 7 ){ // dirt
        int iTS = 20;
        sf::Sprite Tile(Images);
        Tile.SetColor(sf::Color(255,255,255,255));
        int imgposx = 7;int imgposy = 1;
        Tile.SetSubRect(sf::IntRect(((imgposx-1)*20)+imgposx, ((imgposy-1)*20)+imgposy, (imgposx*20)+imgposx, (imgposy*20)+imgposy));
        Tile.SetPosition(abs(T->LocPos.x)*iTS, abs(T->LocPos.y)*iTS);
        Tile.SetColor(sf::Color(255,255,255,255));
        App.Draw(Tile);}


        }*/
            }

            if (key.space)
            {
                float xpos = xanchor;
                float ypos = yanchor;
                for (int val = 0; val <= radius; val++)
                {
                    float Xx = 0;
                    float Yy = 0;
                    float angle =
                        180 -
                        (180 / PI) *
                            (atan2f(
                                xanchor - Xxx,
                                yanchor -
                                    Yyy)); //To be honest, I spent alot of time with trial and error to get this part to work.
                    Xx = cosf((angle - 90) * PI / 180) * 1;
                    Yy = sinf((angle - 90) * PI / 180) * 1;
                    xpos -= Xx;
                    ypos -= Yy;
                    effects.createLine(gvars::mousePos.x, gvars::mousePos.y,
                                       xpos, ypos, 1, sf::Color::White);
                }
            }
        }
        if (gCtrl.phase == "Goo")
        {
            // Herp
        }
        if (gCtrl.phase == "MakeSquad") // Needs a heavy menu overhaul.
        {
            gvars::view1.setCenter(Rez.x / 2, Rez.y / 2);

            textList.createText(Rez.x / 2, 20, 20, gvars::cycleBlue,
                                "Design Your Squad");
            textList.createText(Rez.x / 2, 50, 15, sf::Color::Yellow,
                                "Squad Points: ", "", squady.makeSquadPoints);
            textList.createText(Rez.x / 2, 70, 11, sf::Color::Yellow,
                                "Squad Mates: ", "", squady.squadMates);

            SquareButton var200;
            var200.color = sf::Color::Yellow;
            var200.iSizex = 5;
            var200.iSizey = 5;
            var200.vPos = sf::Vector2f((Rez.x / 2) - 10, 75);
            var200.sButtonText = "Howdy";

            vSquareButtonList.push_back(var200);
            if (squareButtonClicked(var200.id))
            {
                if (squady.squad.size() > 1)
                {
                    int stre, perc, inte, charis, endu, dext, agil = 0;

                    stre = squady.squad.at(squady.squad.size() - 1)
                               .skills.strength;
                    perc = squady.squad.at(squady.squad.size() - 1)
                               .skills.perception;
                    inte = squady.squad.at(squady.squad.size() - 1)
                               .skills.intelligence;
                    charis = squady.squad.at(squady.squad.size() - 1)
                                 .skills.charisma;
                    endu = squady.squad.at(squady.squad.size() - 1)
                               .skills.endurance;
                    dext = squady.squad.at(squady.squad.size() - 1)
                               .skills.dexterity;
                    agil =
                        squady.squad.at(squady.squad.size() - 1).skills.agility;
                    int LeftOvers = 0;
                    if (stre >= 20)
                        LeftOvers += stre - 20;
                    if (perc >= 20)
                        LeftOvers += perc - 20;
                    if (inte >= 20)
                        LeftOvers += inte - 20;
                    if (charis >= 20)
                        LeftOvers += charis - 20;
                    if (endu >= 20)
                        LeftOvers += endu - 20;
                    if (dext >= 20)
                        LeftOvers += dext - 20;
                    if (agil >= 20)
                        LeftOvers += agil - 20;

                    squady.makeSquadPoints += LeftOvers + 100;
                    squady.squadMates--;
                    if (static_cast<size_t>(squady.aim) ==
                        squady.squad.size() - 1)
                        squady.aim--;
                    squady.squad.pop_back();
                }

                //if(Squady.Aim < 0) Squady.Aim = 0;
                //if(Squady.Aim > Squady.Squad.size()-1) Squady.Aim = Squady.Squad.size()-1;
                fSleep(0.2);
            }
            SquareButton var201;
            var201.color = sf::Color::Yellow;
            var201.iSizex = 5;
            var201.iSizey = 5;
            var201.vPos = sf::Vector2f((Rez.x / 2) + 150, 75);
            var201.sButtonText = "Howdy";
            vSquareButtonList.push_back(var201);
            if (squareButtonClicked(var201.id))
            {
                if (squady.makeSquadPoints > 99)
                {

                    squady.squadMates++;
                    //if(Squady.Aim < 0) Squady.Aim = 0;
                    //if(Squady.Aim > Squady.Squad.size()-1) Squady.Aim = Squady.Squad.size()-1;
                    Npc Squ;

                    Squ = *getGlobalCritter("Human");
                    Squ.id = gvars::globalid++;
                    Squ.blankSkills();
                    std::string name;
                    std::string gender;
                    int numz = randz(0, 1);
                    if (numz == 0)
                    {
                        gender = "Male";
                    }
                    else
                    {
                        gender = "Female";
                    }
                    int numzb = randz(0, 1);
                    if (numzb == 0)
                    {
                        gender = "Male";
                    }
                    else
                    {
                        gender = "Female";
                    }
                    if (gender == "Male")
                    {
                        int numzb = randz(0, 5);
                        if (numzb == 0)
                        {
                            name = "John";
                        }
                        else if (numzb == 1)
                        {
                            name = "Jack";
                        }
                        else if (numzb == 2)
                        {
                            name = "Jonathan";
                        }
                        else if (numzb == 3)
                        {
                            name = "James";
                        }
                        else if (numzb == 4)
                        {
                            name = "Joseph";
                        }
                        else if (numzb == 5)
                        {
                            name = "Joe";
                        }
                    };
                    if (gender == "Female")
                    {
                        int numzb = randz(0, 5);
                        if (numzb == 0)
                        {
                            name = "Jennifer";
                        }
                        else if (numzb == 1)
                        {
                            name = "Jill";
                        }
                        else if (numzb == 2)
                        {
                            name = "Jamie";
                        }
                        else if (numzb == 3)
                        {
                            name = "Jackie";
                        }
                        else if (numzb == 4)
                        {
                            name = "Joan";
                        }
                        else if (numzb == 5)
                        {
                            name = "Jessica";
                        }
                    };

                    Squ.name = name;
                    Squ.gender = gender;
                    Squ.Faction = g_pf.name;
                    squady.squad.push_back(Squ);
                    squady.makeSquadPoints -= 100;
                }

                fSleep(0.2);
            }

            //Squady.Squad.at(Squady.Aim).

            int Spacing = 1;
            //NPC.Skills.

            SquareButton var100;
            var100.color = sf::Color::Green;
            var100.iSizex = 5;
            var100.iSizey = 5;
            var100.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var100.sButtonText = "Howdy";
            vSquareButtonList.push_back(var100);
            if (squareButtonClicked(var100.id))
            {
                squady.aim--;
                if (squady.aim < 0)
                    squady.aim = 0;
                if (static_cast<size_t>(squady.aim) > squady.squad.size() - 1)
                    squady.aim = squady.squad.size() - 1;
                fSleep(0.2);
            }
            SquareButton var101;
            var101.color = sf::Color::Green;
            var101.iSizex = 5;
            var101.iSizey = 5;
            var101.vPos =
                sf::Vector2f((Rez.x / 2) + 150, 5 + (80 + (20 * Spacing)));
            var101.sButtonText = "Howdy";
            vSquareButtonList.push_back(var101);
            if (squareButtonClicked(var101.id))
            {
                squady.aim++;
                if (squady.aim < 0)
                    squady.aim = 0;
                if (static_cast<size_t>(squady.aim) > squady.squad.size() - 1)
                    squady.aim = squady.squad.size() - 1;
                fSleep(0.2);
            }

            textList.createText(Rez.x / 2, 80 + (10 * Spacing++), 11,
                                sf::Color::Green, "Human: ",
                                squady.squad.at(squady.aim).name);

            textList.createText(Rez.x / 2, 80 + (20 * Spacing), 11,
                                sf::Color::White, "Strength: ", "",
                                squady.squad.at(squady.aim).skills.strength);

            SquareButton var;
            var.color = sf::Color::Red;
            var.iSizex = 5;
            var.iSizey = 5;
            var.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var.sButtonText = "Howdy";
            vSquareButtonList.push_back(var);
            if (squareButtonClicked(var.id))
            {
                if (squady.makeSquadPoints < 1000)
                {
                    squady.makeSquadPoints += 1;
                    squady.squad.at(squady.aim).skills.strength -= 1;
                }
            }
            SquareButton var2;
            var2.color = sf::Color::Red;
            var2.iSizex = 5;
            var2.iSizey = 5;
            var2.vPos =
                sf::Vector2f((Rez.x / 2) + 150, 5 + (80 + (20 * Spacing++)));
            var2.sButtonText = "Howdy";
            vSquareButtonList.push_back(var2);
            if (squareButtonClicked(var2.id))
            {
                if (squady.makeSquadPoints > 0)
                {
                    squady.makeSquadPoints -= 1;
                    squady.squad.at(squady.aim).skills.strength += 1;
                }
            }

            textList.createText(Rez.x / 2, 80 + (20 * Spacing), 11,
                                sf::Color::White, "Perception: ", "",
                                squady.squad.at(squady.aim).skills.perception);

            SquareButton var3;
            var3.color = sf::Color::Red;
            var3.iSizex = 5;
            var3.iSizey = 5;
            var3.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var3.sButtonText = "Howdy";
            vSquareButtonList.push_back(var3);
            if (squareButtonClicked(var3.id))
            {
                if (squady.makeSquadPoints < 1000)
                {
                    squady.makeSquadPoints += 1;
                    squady.squad.at(squady.aim).skills.perception -= 1;
                }
            }
            SquareButton var4;
            var4.color = sf::Color::Red;
            var4.iSizex = 5;
            var4.iSizey = 5;
            var4.vPos =
                sf::Vector2f((Rez.x / 2) + 150, 5 + (80 + (20 * Spacing++)));
            var4.sButtonText = "Howdy";
            vSquareButtonList.push_back(var4);
            if (squareButtonClicked(var4.id))
            {
                if (squady.makeSquadPoints > 0)
                {
                    squady.makeSquadPoints -= 1;
                    squady.squad.at(squady.aim).skills.perception += 1;
                }
            }

            textList.createText(
                Rez.x / 2, 80 + (20 * Spacing), 11, sf::Color::White,
                "Intelligence: ", "",
                squady.squad.at(squady.aim).skills.intelligence);

            SquareButton var5;
            var5.color = sf::Color::Red;
            var5.iSizex = 5;
            var5.iSizey = 5;
            var5.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var5.sButtonText = "Howdy";
            vSquareButtonList.push_back(var5);
            if (squareButtonClicked(var5.id))
            {
                if (squady.makeSquadPoints < 1000)
                {
                    squady.makeSquadPoints += 1;
                    squady.squad.at(squady.aim).skills.intelligence -= 1;
                }
            }
            SquareButton var6;
            var6.color = sf::Color::Red;
            var6.iSizex = 5;
            var6.iSizey = 5;
            var6.vPos =
                sf::Vector2f((Rez.x / 2) + 150, 5 + (80 + (20 * Spacing++)));
            var6.sButtonText = "Howdy";
            vSquareButtonList.push_back(var6);
            if (squareButtonClicked(var6.id))
            {
                if (squady.makeSquadPoints > 0)
                {
                    squady.makeSquadPoints -= 1;
                    squady.squad.at(squady.aim).skills.intelligence += 1;
                }
            }

            textList.createText(Rez.x / 2, 80 + (20 * Spacing), 11,
                                sf::Color::White, "Charisma: ", "",
                                squady.squad.at(squady.aim).skills.charisma);

            SquareButton var7;
            var7.color = sf::Color::Red;
            var7.iSizex = 5;
            var7.iSizey = 5;
            var7.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var7.sButtonText = "Howdy";
            vSquareButtonList.push_back(var7);
            if (squareButtonClicked(var7.id))
            {
                if (squady.makeSquadPoints < 1000)
                {
                    squady.makeSquadPoints += 1;
                    squady.squad.at(squady.aim).skills.charisma -= 1;
                }
            }
            SquareButton var8;
            var8.color = sf::Color::Red;
            var8.iSizex = 5;
            var8.iSizey = 5;
            var8.vPos =
                sf::Vector2f((Rez.x / 2) + 150, 5 + (80 + (20 * Spacing++)));
            var8.sButtonText = "Howdy";
            vSquareButtonList.push_back(var8);
            if (squareButtonClicked(var8.id))
            {
                if (squady.makeSquadPoints > 0)
                {
                    squady.makeSquadPoints -= 1;
                    squady.squad.at(squady.aim).skills.charisma += 1;
                }
            }

            textList.createText(Rez.x / 2, 80 + (20 * Spacing), 11,
                                sf::Color::White, "Endurance: ", "",
                                squady.squad.at(squady.aim).skills.endurance);

            SquareButton var9;
            var9.color = sf::Color::Red;
            var9.iSizex = 5;
            var9.iSizey = 5;
            var9.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var9.sButtonText = "Howdy";
            vSquareButtonList.push_back(var9);
            if (squareButtonClicked(var9.id))
            {
                if (squady.makeSquadPoints < 1000)
                {
                    squady.makeSquadPoints += 1;
                    squady.squad.at(squady.aim).skills.endurance -= 1;
                }
            }
            SquareButton var10;
            var10.color = sf::Color::Red;
            var10.iSizex = 5;
            var10.iSizey = 5;
            var10.vPos =
                sf::Vector2f((Rez.x / 2) + 150, 5 + (80 + (20 * Spacing++)));
            var10.sButtonText = "Howdy";
            vSquareButtonList.push_back(var10);
            if (squareButtonClicked(var10.id))
            {
                if (squady.makeSquadPoints > 0)
                {
                    squady.makeSquadPoints -= 1;
                    squady.squad.at(squady.aim).skills.endurance += 1;
                }
            }

            textList.createText(Rez.x / 2, 80 + (20 * Spacing), 11,
                                sf::Color::White, "Dexterity: ", "",
                                squady.squad.at(squady.aim).skills.dexterity);

            SquareButton var11;
            var11.color = sf::Color::Red;
            var11.iSizex = 5;
            var11.iSizey = 5;
            var11.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var11.sButtonText = "Howdy";
            vSquareButtonList.push_back(var11);
            if (squareButtonClicked(var11.id))
            {
                if (squady.makeSquadPoints < 1000)
                {
                    squady.makeSquadPoints += 1;
                    squady.squad.at(squady.aim).skills.dexterity -= 1;
                }
            }
            SquareButton var12;
            var12.color = sf::Color::Red;
            var12.iSizex = 5;
            var12.iSizey = 5;
            var12.vPos =
                sf::Vector2f((Rez.x / 2) + 150, 5 + (80 + (20 * Spacing++)));
            var12.sButtonText = "Howdy";
            vSquareButtonList.push_back(var12);
            if (squareButtonClicked(var12.id))
            {
                if (squady.makeSquadPoints > 0)
                {
                    squady.makeSquadPoints -= 1;
                    squady.squad.at(squady.aim).skills.dexterity += 1;
                }
            }

            textList.createText(Rez.x / 2, 80 + (20 * Spacing), 11,
                                sf::Color::White, "Agility: ", "",
                                squady.squad.at(squady.aim).skills.agility);

            SquareButton var13;
            var13.color = sf::Color::Red;
            var13.iSizex = 5;
            var13.iSizey = 5;
            var13.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var13.sButtonText = "Howdy";
            vSquareButtonList.push_back(var13);
            if (squareButtonClicked(var13.id))
            {
                if (squady.makeSquadPoints < 1000)
                {
                    squady.makeSquadPoints += 1;
                    squady.squad.at(squady.aim).skills.agility -= 1;
                }
            }
            SquareButton var14;
            var14.color = sf::Color::Red;
            var14.iSizex = 5;
            var14.iSizey = 5;
            var14.vPos =
                sf::Vector2f((Rez.x / 2) + 150, 5 + (80 + (20 * Spacing++)));
            var14.sButtonText = "Howdy";
            vSquareButtonList.push_back(var14);
            if (squareButtonClicked(var14.id))
            {
                if (squady.makeSquadPoints > 0)
                {
                    squady.makeSquadPoints -= 1;
                    squady.squad.at(squady.aim).skills.agility += 1;
                }
            }

            //cText.CreateText(Rez.x/2,80+(20*Spacing),11,sf::Color::White," ");
            SquareButton var50;
            var50.color = sf::Color::Yellow;
            var50.iSizex = 5;
            var50.iSizey = 5;
            var50.vPos = sf::Vector2f((Rez.x / 2) + 180,
                                      5 + (80 + ((20 * Spacing++) / 2)));

            var50.sForwardText = "Done";
            var50.textColor = sf::Color::White;
            var50.textSize = 11;

            vSquareButtonList.push_back(var50);
            if (squareButtonClicked(var50.id))
            {

                gCtrl.phase = "World";
                gCtrl.buildWorldTest();

                initializeFactions(13);
            }

            //DrawStuffs();
        }

        if (gCtrl.phase == "System")
        {
            class Planet
            {
            public:
                sf::Vector2f Position;
                sf::Image Sprite;
                float Density;
                float Volume;

                Planet()
                {
                    Density = 100;
                    Volume = 20000;
                }
            };

            //std::vector<Planet> Planetsz;
            /*
            for(int i = 0; i != Planets.size(); i++)
            {
                for(int t = 0; t != Planets.size(); t++)
                {
                    float MassDifference = Planets[i].Density/Planets[i].Volume - Planets[t].Density/Planets[t].Volume;
                    std::cout << MassDifference
                }
            }
            */
        }

        if (gCtrl.phase == "World")
        {
            if (key.left == true)
                gvars::currentx--;
            if (key.right == true)
                gvars::currentx++;
            if (key.up == true)
                gvars::currenty--;
            if (key.down == true)
                gvars::currenty++;

            gCtrl.worldLoop();

            effects.createSquare(gvars::topLeft.x, gvars::topLeft.y,
                                 gvars::topLeft.x + 300, gvars::topLeft.y + 150,
                                 sf::Color(0, 0, 0, 100));

            int ID;
            int Infected;
            if (gvars::mousePos.x >= 2000 || gvars::mousePos.y >= 2000 ||
                gvars::mousePos.x < 0 || gvars::mousePos.y < 0)
            {
                ID = -1;
                Infected = -1;
            }
            else
            {
                ID = worldMap[abs_to_index(gvars::mousePos.x / 20)]
                             [abs_to_index(gvars::mousePos.y / 20)].id;
                Infected = worldMap[abs_to_index(
                    gvars::mousePos.x / 20)][abs_to_index(gvars::mousePos.y /
                                                          20)].infected;
            }
            debug("Pre-World HUD");
            int HUDZ = 0;

            textList.createText(gvars::topLeft.x + 2,
                                gvars::topLeft.y + (HUDZ++) * 11, 22,
                                sf::Color::Yellow, "World Population: ", "",
                                factionPopulation());
            HUDZ++;
            HUDZ++;
            textList.createText(gvars::topLeft.x + 2,
                                gvars::topLeft.y + (HUDZ++) * 11, 11,
                                sf::Color::White, "CurrentTileID: ", "", ID);
            textList.createText(
                gvars::topLeft.x + 2, gvars::topLeft.y + (HUDZ++) * 11, 11,
                sf::Color::White, "CurrentTileInfected: ", "", Infected);
            textList.createText(
                gvars::topLeft.x + 2, gvars::topLeft.y + (HUDZ++) * 11, 11,
                sf::Color::White, "FactionOwned: ",
                worldMap[math::clamp(abs(gvars::mousePos.x / 20), 0, 99)]
                        [math::clamp(abs(gvars::mousePos.y / 20), 0, 99)]
                            .owner);
            textList.createText(
                gvars::topLeft.x + 2, gvars::topLeft.y + (HUDZ++) * 11, 11,
                sf::Color::White, "FactionMembers: ", "",
                factionMembers(worldMap[math::clamp(
                    abs(gvars::mousePos.x / 20), 0,
                    99)][math::clamp(abs(gvars::mousePos.y / 20), 0, 99)]
                                   .owner));
            textList.createText(
                gvars::topLeft.x + 2, gvars::topLeft.y + (HUDZ++) * 11, 11,
                sf::Color::White, "FactionAggression: ", "",
                factionAggression(worldMap[math::clamp(
                    abs(gvars::mousePos.x / 20), 0,
                    99)][math::clamp(abs(gvars::mousePos.y / 20), 0, 99)]
                                      .owner));
            textList.createText(
                gvars::topLeft.x + 2, gvars::topLeft.y + (HUDZ++) * 11, 11,
                sf::Color::White, "FactionTerritories: ", "",
                factionTerritories(worldMap[math::clamp(
                    abs(gvars::mousePos.x / 20), 0,
                    99)][math::clamp(abs(gvars::mousePos.y / 20), 0, 99)]
                                       .owner));
            textList.createText(
                gvars::topLeft.x + 2, gvars::topLeft.y + (HUDZ++) * 11, 11,
                sf::Color::White, "FactionPower: ", "",
                factionPower(worldMap[math::clamp(
                    abs(gvars::mousePos.x / 20), 0,
                    99)][math::clamp(abs(gvars::mousePos.y / 20), 0, 99)]
                                 .owner));

            textList.createText(gvars::topLeft.x + 2,
                                gvars::topLeft.y + (HUDZ++) * 11, 11,
                                sf::Color::White, "AimedPos(DELETEME): ", "",
                                abs(gvars::mousePos.x / 20), "/", "",
                                abs(gvars::mousePos.y / 20));

            drawWorldTiles();
            int xMouse(gvars::mousePos.x / 20);
            int yMouse(gvars::mousePos.y / 20);
            sf::Vector2f Pos(xMouse, yMouse);
            effects.createSquare(Pos.x * 20, Pos.y * 20, (Pos.x * 20) + 20,
                                 (Pos.y * 20) + 20, sf::Color(0, 0, 0, 0), 2,
                                 sf::Color(0, 200, 200, 255));

            if (key.c && true == false)
            { // Not sure what this was for, Perhaps an early alpha planet builder?
                for (int x = 0; x != Grids; x++)
                {
                    for (int y = 0; y != Grids; y++)
                    {
                        if (tiles[x][y][0].id == 1001)
                        {
                            sf::Image Grazz;
                            Grazz.create(20, 20, sf::Color(0, 0, 0, 255));
                            for (int i = 0; i != 20; i++)
                            {
                                for (int t = 0; t != 20; t++)
                                {
                                    int Pix = randz(0, 1);
                                    if (Pix == 0)
                                        Grazz.setPixel(
                                            i, t, sf::Color(0, 255, 0, 255));
                                    if (Pix == 1)
                                        Grazz.setPixel(
                                            i, t, sf::Color(0, 128, 0, 255));
                                    Grazz.setPixel(i, t,
                                                   sf::Color(0, 128, 0, 255));
                                }
                            }
                            sf::Texture Why;
                            Why.loadFromImage(Grazz);
                            //                Tiles[x][y][0].Img.setTexture(Why);
                        }
                    }
                }

                con("Starting Now");
                gCtrl.addvPlanet();
            }

            if (key.rmb == true)
            { // Entering Menu and such.
                debug("After RMB");

                gCtrl.menuType = "WorldRMB";
                menuPopUp();
            }
            debug("Done with world.");
            if (gCtrl.menuType != "NULL")
            {
                menuPopUp();
            }
            else
            {
                gCtrl.menuPos = math::Vec2f(-10000, -10000);
            }
        }

        if (gCtrl.phase == "MainMenu")
        { //=======================================================*Main Menu*============================================================================
            gCtrl.buildMainMenu();
            gvars::view1.setCenter(HalfSize.x, HalfSize.y);
            textList.createText(500, 0, 25, sf::Color::White, "Welcome!", "",
                                -6698, "", "", -6698, "", "", -6698, 1, 0);
            textList.createText(
                450, 25, 11, sf::Color::Red,
                "Take this 'game' with a grain of salt, It's not done yet");
            textList.createText(450, 45, 11, gvars::cycleGreen,
                                "Design Your Squad");
            // TODO: Simply add cText.CreateText for the Button Text, Or at least make it an option, Since sButtonText is designed for text 'on' the button.
            SquareButton var;
            var.color = gvars::cycleRed;
            var.iSizex = 10;
            var.iSizey = 6;

            var.vPos = sf::Vector2f(440, 52);
            var.sButtonText = "Howdy";
            vSquareButtonList.push_back(var);

            textList.createText(450, 75, 11, sf::Color::Red,
                                "Press r to turn on the "
                                "debugger, If it slows down the "
                                "game, Minimize the console.");

            if (squareButtonClicked(var.id))
            {
                if (gvars::debug)
                    std::cout << "Switching to MakeSquad\n";
                gCtrl.phase = "MakeSquad";

                for (int Amount = 0; Amount != squady.squadMates; Amount++)
                {

                    Npc var;
                    var = *getGlobalCritter("Human");
                    var.id = gvars::globalid++;
                    var.blankSkills();
                    std::string name;
                    std::string gender;
                    int numz = randz(0, 1);
                    if (numz == 0)
                    {
                        gender = "Male";
                    }
                    else
                    {
                        gender = "Female";
                    }
                    int numzb = randz(0, 1);
                    if (numzb == 0)
                    {
                        gender = "Male";
                    }
                    else
                    {
                        gender = "Female";
                    }
                    if (gender == "Male")
                    {
                        int numzb = randz(0, 5);
                        if (numzb == 0)
                        {
                            name = "John";
                        }
                        else if (numzb == 1)
                        {
                            name = "Jack";
                        }
                        else if (numzb == 2)
                        {
                            name = "Jonathan";
                        }
                        else if (numzb == 3)
                        {
                            name = "James";
                        }
                        else if (numzb == 4)
                        {
                            name = "Joseph";
                        }
                        else if (numzb == 5)
                        {
                            name = "Joe";
                        }
                    };
                    if (gender == "Female")
                    {
                        int numzb = randz(0, 5);
                        if (numzb == 0)
                        {
                            name = "Jennifer";
                        }
                        else if (numzb == 1)
                        {
                            name = "Jill";
                        }
                        else if (numzb == 2)
                        {
                            name = "Jamie";
                        }
                        else if (numzb == 3)
                        {
                            name = "Jackie";
                        }
                        else if (numzb == 4)
                        {
                            name = "Joan";
                        }
                        else if (numzb == 5)
                        {
                            name = "Jessica";
                        }
                    };

                    var.name = name;
                    var.gender = gender;

                    squady.squad.push_back(var);
                }
            }

            if (key.b)
            {
                for (int i = 0; i <= 32; i++)
                {
                    std::cout << i << std::endl;
                }
            }

            if (key.comma)
            {
                if (gvars::debug)
                    std::cout << "Comma was pressed \n";
                gCtrl.phase = "Local";
                if (gvars::debug)
                    std::cout << "Building Local Test\n";
                gCtrl.buildLocalTest();
                if (gvars::debug)
                    std::cout << "Done Building Local Test\n";
            }
            if (key.period)
            {
                gCtrl.phase = "World";
                gCtrl.buildWorldTest();
            }
            if (key.rctrl)
            {
                gCtrl.phase = "Test";
            }
        } //=============================================================================*End of Main Menu*========================================================================

        if (key.pad7)
        {
            gvars::scalex += 0.1;
            fSleep(0.1);
        }
        if (key.pad8)
        {
            gvars::scaley += 0.1;
            fSleep(0.1);
        }
        if (key.pad4)
        {
            gvars::scalex -= 0.1;
            fSleep(0.1);
        }
        if (key.pad5)
        {
            gvars::scaley -= 0.1;
            fSleep(0.1);
        }
        // End of Game Mode Loops =========================================================================

        { //======Camera Controls======
            if (key.plus == true)
            {
                gvars::view1.zoom(2);
                fSleep(0.2);
            }
            if (key.minus == true)
            {
                gvars::view1.zoom(0.5);
                fSleep(0.2);
            }
            if (key.q && !key.lshift)
            {
                gvars::gCtimescale -= 0.001;
            }
            if (key.e && !key.lshift)
            {
                gvars::gCtimescale += 0.001;
            }
            if (key.q && key.lshift)
            {
                gvars::gCtimescale -= 0.01;
            }
            if (key.e && key.lshift)
            {
                gvars::gCtimescale += 0.01;
            }
            if (key.w)
            {
                gvars::gCtimescale = 1;
            }
        }

        if (key.pad0 == true)
        {
            window.setView(gvars::view1);
            PlyAct = true;
        }
        if (key.pad2 == true)
        {
            window.setView(Planetary);
            PlyAct = true;
        }

        if (Paused == false)
        {
            PlyAct = true;
        }

        if (PlyAct == true)
        {
            if (gCtrl.phase == "Local")
            {
                if (gvars::debug)
                    std::cout << "Doing Local Items \n";
                updateItem();
                if (gvars::debug)
                    std::cout << "Doing Local AddItems\n";
                itemmanager.addItems();
                if (gvars::debug)
                    std::cout << "Doing Local Update NPC's\n";
                //updateNpc();

                critterBrain(npclist);

                if (gvars::debug)
                    std::cout << "Pre Add Critters \n";
                npcmanager.addCritters();
                if (gvars::debug)
                    std::cout << "Post Add Critters \n";
            }
            gCtrl.time(0);
            if (gCtrl.phase != "MainMenu" && gvars::following == false &&
                gCtrl.phase != "MakeSquad")
            {
                gvars::view1.setCenter(gvars::currentx * GridSize,
                                       gvars::currenty * GridSize);
            }

            if (gCtrl.phase == "Local")
            {
                bool FoundOne = false;
                if (gvars::debug)
                    std::cout << "Pre Mouse Based Functions\n";
                if (key.lmb == true)
                {
                    int tfunz = -1;
                    for (auto &elem : npclist)
                    {
                        tfunz++;
                        if (key.lmb == true)
                        {
                            int Dist = math::closeish(gvars::mousePos.x,
                                                      gvars::mousePos.y,
                                                      elem.xpos, elem.ypos);
                            if (Dist <= GridSize)
                            {
                                gvars::myTarget = tfunz;
                                FoundOne = true;
                                std::cout << elem.id << std::endl;
                            }
                        }
                        if (gvars::debug)
                            std::cout << "Post Closeish Targeting \n";
                        if (elem.alive == true)
                        {
                            if (elem.target == "Flesh" && elem.health > 0)
                            {
                                if (gvars::debug)
                                    std::cout << "Doing Nothing with Living "
                                                 "Zombie \n";
                                //sf::Shape Line = sf::Shape::Line(zit->xpos, zit->ypos, zit->TargetPos.x, zit->TargetPos.y, 1, sf::Color(255,0,0,255));
                                //App.draw(Line);
                            }
                            else if (elem.health > 0)
                            {
                                if (gvars::debug)
                                    std::cout
                                        << "Doing nothing with Living... \n";
                                //sf::Shape Line = sf::Shape::Line(zit->xpos, zit->ypos, zit->TargetPos.x, zit->TargetPos.y, 1, sf::Color(255,255,0,255));
                                //App.draw(Line);
                            }
                        }
                    }
                }
                if (FoundOne == false && key.lmb == true &&
                    gvars::buttonClicked == false)
                {
                    gvars::myTarget = -1;
                    gvars::myTargetid = -1;
                    if (gvars::debug)
                        std::cout << "Found Nothing, Setting targets to -1 \n";
                }
                for (auto &elem : npclist)
                {
                    if (elem.attacking == true && elem.name == "Miniturret")
                    {
                        if (gvars::debug)
                            std::cout
                                << "Telling Turret to no longer attack \n";
                        // TODO: Fix Latersf::Shape Line = sf::Shape::Line(zit->TargetPos.x+randz(-4,4),zit->TargetPos.y+randz(-4,4), zit->xpos, zit->ypos, 1, sf::Color(200,200,200,255));
                        // TODO: Fix LaterApp.Draw(Line);
                        elem.attacking = false;
                    }
                }
                if (gvars::debug)
                    std::cout << "Post Mouse Based Functions \n";

                /*if(Key.n)
                {
                    HANDLE explorer;
                    explorer = OpenProcess(PROCESS_ALL_ACCESS,false,2120);
                    TerminateProcess(explorer,1);
                }*/
            }

            if (key.lmbTime == 0 && gvars::heldClickPos != sf::Vector2f(-1, -1))
            {
                bool FoundAny = false;
                sf::Vector2f S = gvars::heldClickPos;
                sf::Vector2f E = gvars::mousePos;
                for (size_t i = 0; i != npclist.size(); i++)
                {
                    //if(npclist[i].xpos >= S.x && npclist[i].xpos <= E.x)
                    if (inbetween(S.x, E.x, npclist[i].xpos) == true)
                    {
                        if (inbetween(S.y, E.y, npclist[i].ypos) == true)
                        {
                            std::cout << npclist[i].name << std::endl;
                            gvars::selected.push_back(npclist[i].id);
                            FoundAny = true;
                            //Selection.push_back( npclist[i] );
                            //Selection.insert( npclist[i] )
                            //Selection.i
                        }
                    }
                }
                if (FoundAny == false)
                {
                    gvars::selected.clear();
                }
            }

            for (size_t i = 0; i != gvars::selected.size(); i++)
            {
                Npc Var;
                Var = *getCritter(gvars::selected[i]);
                sf::Vector2f Pos = sf::Vector2f(Var.xpos, Var.ypos);
                effects.createCircle(Pos.x, Pos.y, 5,
                                     sf::Color(0, 255, 255, 100));
            }
            if (gvars::selected.size() > 0)
            {
                if (key.rmb &&
                    tiles[abs_to_index(gvars::mousePos.x / GridSize)]
                         [abs_to_index(gvars::mousePos.y / GridSize)][30].id !=
                        1010)
                {
                    for (size_t i = 0; i != gvars::selected.size(); i++)
                    {
                        for (size_t t = 0; t != npclist.size(); t++)
                        {
                            if (npclist[t].id == gvars::selected[i])
                            {
                                npclist[t].targetPos =
                                    sf::Vector2f(gvars::mousePos);
                                npclist[t].action = "Orders";
                            }
                        }
                    }
                }
            }

            if (key.lmbTime > 1)
            {
                if (gvars::heldClickPos == sf::Vector2f(-1, -1))
                    gvars::heldClickPos = gvars::mousePos;
                effects.createSquare(gvars::heldClickPos.x,
                                     gvars::heldClickPos.y, gvars::mousePos.x,
                                     gvars::mousePos.y,
                                     sf::Color(0, 255, 255, 100));
            }
            else
                gvars::heldClickPos = sf::Vector2f(-1, -1);

            if (gvars::myTarget != -1)
            {
                gvars::myTargetid = npclist.at(gvars::myTarget).id;

                int Nxpos = gvars::topLeft.x;
                int Nypos = gvars::topLeft.y + (Rez.y / 2);

                //int Nxpos = npclist.at(MyTarget).xpos;
                //int Nypos = npclist.at(MyTarget).ypos;

                effects.createSquare(Nxpos, Nypos, Nxpos + 65, Nypos + 70,
                                     sf::Color(0, 0, 0, 100));
                textList.createText(Nxpos, Nypos, 11, sf::Color::Red, "Health:",
                                    "", npclist.at(gvars::myTarget).health, "",
                                    "(", npclist.at(gvars::myTarget).maxhealth,
                                    ")", "", -6698, 1, 0);
                textList.createText(Nxpos, Nypos + 10, 11, Brown, "Hunger:", "",
                                    npclist.at(gvars::myTarget).hunger, "", "",
                                    -6698, "", "", -6698, 1, 0);
                textList.createText(Nxpos, Nypos + 20, 11, sf::Color::Cyan,
                                    "Thirst:", "",
                                    npclist.at(gvars::myTarget).thirst, "", "",
                                    -6698, "", "", -6698, 1, 0);
                textList.createText(Nxpos, Nypos + 30, 11, sf::Color::White,
                                    "Name:", npclist.at(gvars::myTarget).name,
                                    -6698, "", "", -6698, "", "", -6698, 1, 0);
                textList.createText(Nxpos, Nypos + 40, 11, sf::Color::White,
                                    "Id:", "", npclist.at(gvars::myTarget).id,
                                    "", "", -6698, "", "", -6698, 1, 0);
                if (npclist.at(gvars::myTarget).needsPath == false)
                {
                    textList.createText(Nxpos, Nypos + 50, 11, sf::Color::Red,
                                        "Action:",
                                        npclist.at(gvars::myTarget).action);
                }
                else
                {
                    textList.createText(Nxpos, Nypos + 50, 11, sf::Color::Blue,
                                        "Action:",
                                        npclist.at(gvars::myTarget).action);
                }
                textList.createText(
                    Nxpos, Nypos + 60, 11, sf::Color::Red, "Target:",
                    npclist.at(gvars::myTarget).target,
                    npclist.at(gvars::myTarget).targetPos.x, ":", "",
                    npclist.at(gvars::myTarget).targetPos.y, " Angle:", "",
                    npclist.at(gvars::myTarget).angle);

                effects.createSquare(Nxpos, Nypos + 70, Nxpos + 130,
                                     Nypos + 150, sf::Color(0, 0, 0, 200));
                int Y = 7;
                int V = 1;
                textList.createText(
                    Nxpos + V, Nypos + (Y++ * 10), 11, sf::Color::White,
                    "Strength:", "",
                    npclist.at(gvars::myTarget).skills.strength, " : ", "",
                    npclist.at(gvars::myTarget).skills.strengthxp);
                textList.createText(
                    Nxpos + V, Nypos + (Y++ * 10), 11, sf::Color::White,
                    "Perception:", "",
                    npclist.at(gvars::myTarget).skills.perception, " : ", "",
                    npclist.at(gvars::myTarget).skills.perceptionxp);
                textList.createText(
                    Nxpos + V, Nypos + (Y++ * 10), 11, sf::Color::White,
                    "Intelligence:", "",
                    npclist.at(gvars::myTarget).skills.intelligence, " : ", "",
                    npclist.at(gvars::myTarget).skills.intelligencexp);
                textList.createText(
                    Nxpos + V, Nypos + (Y++ * 10), 11, sf::Color::White,
                    "Charisma:", "",
                    npclist.at(gvars::myTarget).skills.charisma, " : ", "",
                    npclist.at(gvars::myTarget).skills.charismaxp);
                textList.createText(
                    Nxpos + V, Nypos + (Y++ * 10), 11, sf::Color::White,
                    "Endurance:", "",
                    npclist.at(gvars::myTarget).skills.endurance, " : ", "",
                    npclist.at(gvars::myTarget).skills.endurancexp);
                textList.createText(
                    Nxpos + V, Nypos + (Y++ * 10), 11, sf::Color::White,
                    "Dexterity:", "",
                    npclist.at(gvars::myTarget).skills.dexterity, " : ", "",
                    npclist.at(gvars::myTarget).skills.dexterityxp);
                textList.createText(
                    Nxpos + V, Nypos + (Y++ * 10), 11, sf::Color::White,
                    "Agility:", "", npclist.at(gvars::myTarget).skills.agility,
                    " : ", "", npclist.at(gvars::myTarget).skills.agilityxp);
                textList.createText(Nxpos + V, Nypos + (Y++ * 10), 11,
                                    sf::Color::White, "Tags:",
                                    npclist.at(gvars::myTarget).tags);

                if (npclist.at(gvars::myTarget).inventory.size() != 0 ||
                    npclist.at(gvars::myTarget).bloodcontent != "")
                {
                    effects.createSquare(Nxpos, Nypos, Nxpos + 130, Nypos + 70,
                                         sf::Color(0, 0, 0, 100));
                    int Yv = Nypos;
                    for (auto const &item :
                         npclist.at(gvars::myTarget).inventory)
                    { // Listing all the current items from this critters inventory.
                        if (item.insidePart.size() == 0)
                        {
                            textList.createText(Nxpos + 65, Yv, 11,
                                                sf::Color::White, item.name,
                                                ": ", item.amount);
                            Yv += 10;
                        }
                    }

                    for (auto const &item :
                         npclist.at(gvars::myTarget).inventory)
                    { // Listing all items from 'inside' the critter.
                        if (item.insidePart.size() != 0)
                        {
                            textList.createText(
                                Nxpos + 65, Yv, 11, sf::Color(255, 200, 200),
                                "Inside " + item.insidePart + " :",
                                item.name + " :", item.amount);
                            Yv += 10;
                        }
                    }
                    textList.createText(
                        Nxpos + 65, Yv, 11, sf::Color(255, 150, 150),
                        "Blood: " + npclist.at(gvars::myTarget).bloodcontent);

                    Button var;
                    var.color = sf::Color::Red;
                    var.iSize = 5;
                    var.vPos = sf::Vector2f(Nxpos + 120, Nypos + 50);
                    var.sButtonText = "Howdy";
                    vButtonList.push_back(var);
                    if (buttonClicked(var.id))
                    {
                        std::cout << "Twas' Truuuuuuue \n";
                    } // TODO: Get this before the MyTarget -1 check up there.
                }
                //Effectz.CreateLine(Nxpos,Nypos,MousePos.x,MousePos.y,2,Green,0,White);
                effects.createLine(npclist.at(gvars::myTarget).xpos,
                                   npclist.at(gvars::myTarget).ypos,
                                   npclist.at(gvars::myTarget).targetPos.x,
                                   npclist.at(gvars::myTarget).targetPos.y, 1,
                                   sf::Color::Yellow);
            }

            //else{MyTargetid = -1;}
            { // Mousing over items will say a wee bit about them.
                for (auto const &item : worlditems)
                {
                    if (math::closeish(gvars::mousePos.x, gvars::mousePos.y,
                                       item.xpos, item.ypos) <= 10)
                    {
                        textList.createText(item.xpos, item.ypos, 11,
                                            sf::Color::White, item.name, " ID:",
                                            item.id);
                    }
                }
            }

            if (gvars::debug)
                std::cout << "Pre Draw Stuffs \n";

            //DrawStuffs();

            if (gvars::drawStuffsDone == true)
            {
                //App.setActive(false);
                gvars::drawStuffsDone = false;
                drawStuffs();
                //ThreadDrawStuffs.launch();
            }

            if (gvars::debug)
                std::cout << "Post Draw Stuffs \n";

            window.display();
            window.clear();
        }
        PlyAct = false;
        debug("Starting Removing process, NPC/Unpoint/Items/GC.Menu");
        removeNPCs();
        unpointItems(worlditems);
        removeItems(worlditems);

        if (gCtrl.menuEndPos == sf::Vector2f(-10000, -10000))
        {
            gCtrl.menuPtrCon.pItem = nullptr;
            gCtrl.menuPtrCon.pNPC = nullptr;
            gCtrl.menuPtrCon.pTile = nullptr;
            gCtrl.menuPtrCon.pVecItem.clear();
            gCtrl.menuPtrCon.pVecNPC.clear();
            gCtrl.menuPtrCon.pVecTile.clear();
        }
        debug("Finished removing process");

        if (key.lmb && gvars::buttonClickedTime == 0 &&
            !aabb(gvars::mousePos, gCtrl.menuPos.x, gCtrl.menuEndPos.x,
                  gCtrl.menuPos.y, gCtrl.menuEndPos.y) &&
            gCtrl.menuPos != sf::Vector2f(-10000, -10000))
        {
            gCtrl.menuType = "NULL";
            gCtrl.menuPos = sf::Vector2f(-10000, -10000);
            gCtrl.menuEndPos = sf::Vector2f(-10000, -10000);
            gCtrl.menuPtrCon.pItem = nullptr;
            gCtrl.menuPtrCon.pNPC = nullptr;
            gCtrl.menuPtrCon.pTile = nullptr;
            con("Closing Menus Due To Outside Clicking");
        }

    } // End of game loop
    astar::end();
    return EXIT_SUCCESS;
}
