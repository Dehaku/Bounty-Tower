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

using std::abs;

item *ListGet(std::list<item> &List, int Position)
{
    auto i = List.begin();
    std::advance(i, Position);
    return &(*i);
}

//-- Prototypes
//

void DrawStuffs();
int GetItemVectorId(int id);

std::string GetClipboardText()
{
#ifdef GALAXY_LINUX
    return "Not implemented.";
#else
    // Try opening the clipboard
    if (!OpenClipboard(NULL))
        Con("ClipError1");

    // Get handle of clipboard object for ANSI text
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == NULL)
        Con("ClipError1");

    // Lock the handle to get the actual text pointer
    char *pszText = static_cast<char *>(GlobalLock(hData));
    if (pszText == NULL)
        Con("ClipError1");

    // Save text in a string class instance
    std::string text(pszText);
    Con("Your Clipboard Contains: ", false);
    Con(text);

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

void UnpointItems(std::list<item> &Items)
{

    //for(int i = 0; i != Items.size(); i++)
    for (auto i = Items.begin(); i != Items.begin(); i++)
    {
        if ((*i).ToDelete)
        {
            std::cout << "JobList size: " << UniFact[0].JobList.size()
                      << std::endl;

            for (size_t t = 0; t != UniFact[0].JobList.size(); t++)
            {
                std::cout << "Job Point: " << &UniFact[0].JobList[t].pItem
                          << "/" << UniFact[0].JobList[t].pItem
                          << ", Item Point: " << &(*i) << "/" << &(*i)
                          << std::endl;
                fSleep(1);

                if (UniFact[0].JobList[t].pItem != nullptr &&
                    (&(*i)) != nullptr)
                {

                    std::cout << "ID! ";
                    try
                    {
                        std::cout << UniFact[0].JobList[t].pItem->id;
                    }
                    catch (std::exception &e)
                    {
                        std::cout << "Something went wrong in in here\n";
                    }
                    std::cout << UniFact[0].JobList[t].pItem->id;
                    std::cout << " ; ";
                    int ID1 = UniFact[0].JobList[t].pItem->id;
                    std::cout << "ID 2! ";
                    std::cout << (*i).id;
                    std::cout << " ; ";
                    int ID2 = (*i).id;
                    std::cout << "ID's: " << ID1 << ":" << ID2 << std::endl;

                    if (ID1 ==
                        ID2) //if(UniFact[0].JobList[t].pItem != NULL && &(*i) != NULL && UniFact[0].JobList[t].pItem == &(*i)) //if(ID1 == ID2)
                    {
                        std::cout << "Match! \n";
                        UniFact[0].JobList[t].pItem = nullptr;
                        //fSleep(1);
                    }
                }
            }
        }
    }
}

bool RemoveItem(int Id)
{
    int TempInt = 0;
    std::list<item>::iterator location;
    for (auto it = worlditems.begin(); it != worlditems.end(); ++it)
    {
        if (it->id == Id)
        {
            location = it;
            TempInt = 1;
        }
    }
    if (TempInt == 1)
    {
        worlditems.erase(location);
        return true;
    }
    else if (TempInt == 0)
    {
        return false;
    }
    throw std::runtime_error("Couldn't return anything sensible");
}

void updateItem()
{
    if (gvars::Debug)
    {
        std::cout << "Pre Item \n";
    }

    for (auto &item : worlditems)
    {
        try
        {
            if (gvars::Debug)
            {
                std::cout << item.name << "'s turn! \n";
            }
            if (item.produces == true)
            {
                if (gvars::Debug)
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
                    bool FindEmpty = false;
                    int x;
                    int y;
                    while (FindEmpty == false)
                    {
                        x = randz(item.xpos - 10, item.xpos + 10);
                        y = randz(item.ypos - 10, item.ypos + 10);
                        if (Tiles[abs_to_index(x / GridSize)][abs_to_index(
                                y / GridSize)][30].Walkable == true)
                        {
                            FindEmpty = true;
                        }
                    }
                    if (gvars::Debug)
                    {
                        std::cout << "Producing: " << s << " \n";
                    }
                    SpawnItem(s, x, y);
                    if (gvars::Debug)
                    {
                        std::cout << item.name << " has produced: " << s
                                  << " \n";
                    }
                }
            }
            if (gvars::Debug)
            {
                std::cout << "Done with: " << item.name << " \n";
            }

            if (gvars::Debug)
            {
                std::cout << "Acting on Missile \n";
            }
            if (item.TargetPos != sf::Vector2f(-1, -1))
            {
                item.xpos = (item.xpos + item.TargetPos.x) / 2;
                item.ypos = (item.ypos + item.TargetPos.y) / 2;
                if (item.age >= 10)
                {
                    item.ToDelete = true;
                    Boom(item.xpos, item.ypos, randz(item.mindam, item.maxdam),
                         item.range);

                    for (int ItLength = 0; ItLength != 5; ItLength++)
                    {
                        for (int Rot = 1; Rot != 361; Rot++)
                        {
                            //int Rot = GX;
                            int XPos = ((abs(item.xpos / 20)) +
                                        cosf(Rot * PI / 180) * ItLength);
                            int YPos = ((abs(item.ypos / 20)) +
                                        sinf(Rot * PI / 180) * ItLength);
                            //XPos *= 20;
                            //YPos *= 20;

                            //Effectz.CreateCircle(MousePos.x,MousePos.y,5,Red);
                            //Effectz.CreateCircle(XPos,YPos,5,White);
                            Tiles[XPos][YPos][30].Stone();
                        }
                    }

                    Con("Boom!");
                }
            }
            item.age = item.age + 1;
            if (gvars::Debug)
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

std::vector<int> NnGTrace(int xa, int ya, int xb, int yb, int id,
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
    std::vector<int> VectorID;
    for (int k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        if (Key.g)
        {
            Effectz.CreateCircle(x, y, 1, sf::Color::Blue);
        }
        int Count = 0;

        { // Merely doing this so I can reuse the same code, but for items, Hehe.
            std::vector<NPC>::iterator Me;

            for (Me = npclist.begin(); Me != npclist.end(); ++Me)
            {
                if (math::closeish(x, y, Me->xpos, Me->ypos) <= Me->reach &&
                    Me->id != id)
                {

                    std::vector<int>::iterator Vec;
                    try
                    {
                        bool Exists = false;
                        for (Vec = VectorID.begin(); Vec != VectorID.end();
                             ++Vec)
                        {
                            if (*Vec == Me->id)
                            {
                                Exists = true;
                            }
                        }
                        if (Exists == false)
                        {
                            VectorID.push_back(69);
                            VectorID.push_back(GetNpcVectorId(Me->id));
                        }
                    }
                    catch (std::exception &e)
                    {
                        std::cout << "VectError\n";
                    }

                } //23 = Item, 69 = NPC
                Count++;
            }
        }
        std::list<item>::iterator Me;

        for (Me = worlditems.begin(); Me != worlditems.end(); ++Me)
        {
            if (math::closeish(x, y, Me->xpos, Me->ypos) <= 10 && Me->id != id)
            {
                //Making sure not to constantly add the same
                //try{if( VectorID.at(Count-1) != Me->id){ VectorID.push_back(23); VectorID.push_back( GetItemVectorId(Me->id)); }} catch (std::exception& e){}

            } //23 = Item, 69 = NPC
            Count++;
        }

        if (Key.period)
        {
            Effectz.CreateCircle(x, y, 1, sf::Color::White);
        }
    }
    if (VectorID.size() == 0)
    {
        VectorID.push_back(-1);
    }
    return VectorID;
}

bool gridposTrace(int xa, int ya, int xb, int yb, int id, sf::Vector2f Target)
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
        if (Tiles[abs_to_index(x / GridSize)][abs_to_index(y / GridSize)][30]
                .ID == 1010)
        {
            if (Key.period && id == gvars::MyTargetid)
            {
                Effectz.CreateLine(x, y, xa, ya, 1, sf::Color::Blue);
            }
            //std::cout << "Shoulda Broke. " << std::endl;
            break;

        } // Stops the trace if it hits a wall.
        //std::cout << "Stuffs " << std::endl;
        sf::Vector2f Pos(abs(x / GridSize), abs(y / GridSize));
        sf::Vector2f Tar(abs(Target.x / GridSize), abs(Target.y / GridSize));
        if (Pos == Tar)
        {
            return true;
        } // Returns true and stops searching.
        if (Key.period && id == gvars::MyTargetid)
        {
            Effectz.CreateLine(x, y, xa, ya, 1, sf::Color::Blue);
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
    std::vector<int> VectorID;

    for (int k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        bool Kill = false;
        std::vector<NPC>::iterator Me;
        int Count = 0;
        for (Me = npclist.begin(); Me != npclist.end(); ++Me)
        {
            if (math::closeish(x, y, Me->xpos, Me->ypos) <= Me->reach &&
                Me->id != id)
            {
                VectorID.push_back(Count);
                Kill = true;
            }
            Count++;
        }
        if (Kill)
            return VectorID;

        if (Key.h)
        {
            Effectz.CreateCircle(x, y, 1, sf::Color::White);
        }
    }
    if (VectorID.size() == 0)
    {
        VectorID.push_back(-1);
    }

    throw std::runtime_error("npcTrace: couldn't return a value");
}

int GetItemVectorId(int id)
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

int GetNpcVectorId(int id)
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

bool RemoveNPC(char * /*NPCname*/, int /*Id*/)
{
    int TempInt = 0;
    std::vector<NPC>::iterator location;
    for (auto it = npclist.begin(); it != npclist.end(); ++it)
    {
        if (it->health <= 0 || it->HasSpawned == false || it->alive == false)
        {
            location = it;
            TempInt = 1;
        }
    }
    if (TempInt == 1)
    {
        npclist.erase(location);
        return true;
    }
    else if (TempInt == 0)
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

ItemFindResult FindClosestItem(int Orix, int Oriy, std::string TarItem,
                               int /*Gxpos*/ = 0, int /*Gypos*/ = 0,
                               int /*Rxpos*/ = 0, int /*Rypos*/ = 0)
{
    // This will be difficult, go through all the items, and find the closest one.
    int closx =
        -1000000; // Make sure the default starting number is far beyond being the closest one.
    int closy = -1000000;
    int Closid;
    int closVect;
    int VectPos = 0;
    bool first = true;
    for (auto const &item : worlditems)
    {
        if (first == true)
        {
            if (item.name == TarItem)
            {
                closx = item.xpos;
                closy = item.ypos;
                Closid = item.id;
                closVect = VectPos;
                first = false;
            }
        }
        else
        {
            if (item.name == TarItem)
            {
                int one = math::closeish(Orix, Oriy, item.xpos, item.ypos);
                int two = math::closeish(Orix, Oriy, closx, closy);
                if (one < two)
                {
                    closx = item.xpos;
                    closy = item.ypos;
                    Closid = item.id;
                    closVect = VectPos;
                }
            }
        }
        VectPos++;
    }
    if (first != true)
    {
        return {true, closx, closy, Closid, closVect};
    }
    return {false, 0, 0, 0, 0};
}

std::set<int> NpcList(int exceptions = -1)
{
    if (gvars::Debug)
    {
        std::cout << "Pre npcList \n";
    }
    std::set<int> Returns;

    for (auto const &npc : npclist)
    {
        if (gvars::Debug)
        {
            std::cout << "For NpcList \n";
        }
        if (npc.id != exceptions)
        {
            if (gvars::Debug)
            {
                std::cout << "Post exception NpcList \n";
            }
            Returns.insert(GetNpcVectorId(npc.id));
            if (gvars::Debug)
            {
                std::cout << "Post Returns NpcList \n";
            }
        }
    }
    if (gvars::Debug)
    {
        std::cout << "Post For NpcList \n";
    }
    if (!Returns.empty())
    {
        return Returns;
    }
    throw std::runtime_error("NpcList: Couldn't return anything!");
}

void critterBrain(std::vector<NPC> &NPCs)
{
    for (auto &npc : NPCs)
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

        while (SearchPos != npc.Body.BodyParts.npos) // Global Part Tags
        {
            SearchPos = npc.Body.BodyParts.find("{", SearchPos);

            if (SearchPos != npc.Body.BodyParts.npos)
            {
                EndPos = npc.Body.BodyParts.find("}", SearchPos);

                std::string WorkingLine;

                WorkingLine.append(npc.Body.BodyParts, SearchPos,
                                   EndPos - SearchPos);
                float PartNumber = 0;

                PartNumber = StringFindNumber(WorkingLine, "[Walk:");
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
                    StringFindNumber(WorkingLine, "[NutritionExtraction:");
                if (PartNumber != 0)
                {
                    GlobalNutritionPercentage += PartNumber;
                }

                PartNumber = StringFindNumber(WorkingLine, "[DigestsBlood:");
                if (PartNumber != 0)
                {
                }
                PartNumber = StringFindNumber(WorkingLine, "[DigestsFlesh:");
                if (PartNumber != 0)
                {
                    ConsumeFlesh = true;
                }
                PartNumber = StringFindNumber(WorkingLine, "[DigestsVeggy:");
                if (PartNumber != 0)
                {
                    ConsumeVeggy = true;
                }
                PartNumber = StringFindNumber(WorkingLine, "[DigestsWater:");
                if (PartNumber != 0)
                {
                    ConsumeWater = true;
                }

                SearchPos = EndPos;
            }
        }

        SearchPos = 0;

        //for(int i = 0; i != npc.inventory.size(); i++)
        for (auto i = npc.inventory.begin(); i != npc.inventory.end(); i++)
        {
            if ((*i).InsidePart != "")
            {
                (*i).HasInternalUse--;
            }
        }

        while (SearchPos != npc.Body.BodyParts.npos) // Individual Part Tags
        {

            SearchPos = npc.Body.BodyParts.find("{", SearchPos);

            if (SearchPos != npc.Body.BodyParts.npos)
            {
                EndPos = npc.Body.BodyParts.find("}", SearchPos);
                Parts++;

                std::string WorkingLine;

                WorkingLine.append(npc.Body.BodyParts, SearchPos,
                                   EndPos - SearchPos);

                float PartNumber = 0;
                std::string PartString = "";
                item *PartItem;

                std::string CurrentPart =
                    StringFindString(WorkingLine, "[Name:");

                PartNumber = StringFindNumber(WorkingLine, "[DigestsBlood:");
                PartItem = GetItemPtrfromVector(npc.inventory, "Blood");
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
                            Nutr * PercentageBuff(GlobalNutritionPercentage));
                    }
                    else
                    {
                        //*GetItemPtrfromVector(npc.inventory,"Blood").amount = 0;
                        GetItemPtrfromVector(npc.inventory, "Blood")->ToDelete =
                            true;
                        float Nutr =
                            WorkAmount * 100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Nutrients",
                            Nutr * PercentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartNumber = StringFindNumber(WorkingLine, "[DigestsFlesh:");
                PartItem =
                    GetItemPtrfromVectorVarSearch(npc.inventory, "MassFlesh");
                //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
                if (PartNumber != 0 && PartItem != nullptr &&
                    PartItem->MassFlesh >
                        0) // TODO: Make sure the item is in THIS PART before digesting it!
                {
                    //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                    float WorkAmount = PartItem->MassFlesh;
                    float Diff = WorkAmount - (PartNumber / 1000);

                    if (Diff > 0)
                    {
                        PartItem->MassFlesh = Diff;
                        PartItem->HasInternalUse = 0;
                        float Nutr = (WorkAmount - Diff) *
                                     100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Nutrients",
                            Nutr * PercentageBuff(GlobalNutritionPercentage));
                    }
                    if (PartItem->MassFlesh <= 0)
                    {
                        //*GetItemPtrfromVector(npc.inventory,"Blood").amount = 0;
                        PartItem->ToDelete = true;
                        //Add Food to everyone, Make sure they go hungry to eat it, Figure out a way to Eject the empty item, Or do water! Everyone starts with water.
                        //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartNumber = StringFindNumber(WorkingLine, "[DigestsVeggy:");
                PartItem =
                    GetItemPtrfromVectorVarSearch(npc.inventory, "MassVeggy");
                //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
                if (PartNumber != 0 && PartItem != nullptr &&
                    PartItem->MassVeggy >
                        0) // TODO: Make sure the item is in THIS PART before digesting it!
                {
                    //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                    float WorkAmount = PartItem->MassVeggy;
                    float Diff = WorkAmount - (PartNumber / 1000);

                    if (Diff > 0)
                    {
                        PartItem->MassVeggy = Diff;
                        PartItem->HasInternalUse = 0;
                        float Nutr = (WorkAmount - Diff) *
                                     100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Nutrients",
                            Nutr * PercentageBuff(GlobalNutritionPercentage));
                    }
                    if (PartItem->MassVeggy <= 0)
                    {
                        PartItem->ToDelete = true;
                        //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartNumber = StringFindNumber(WorkingLine, "[DigestsWater:");
                PartItem =
                    GetItemPtrfromVectorVarSearch(npc.inventory, "MassWater");
                //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
                if (PartNumber != 0 && PartItem != nullptr &&
                    PartItem->MassWater >
                        0) // TODO: Make sure the item is in THIS PART before digesting it!
                {
                    //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                    float WorkAmount = PartItem->MassWater;
                    float Diff = WorkAmount - (PartNumber / 1000);

                    if (Diff > 0)
                    {
                        PartItem->MassWater = Diff;
                        PartItem->HasInternalUse = 0;
                        float Nutr = (WorkAmount - Diff) *
                                     100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Hydration",
                            Nutr * PercentageBuff(GlobalNutritionPercentage));
                    }
                    if (PartItem->MassWater <= 0)
                    {
                        PartItem->ToDelete = true;
                        //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartString = StringFindString(WorkingLine, "[PoisonFilter:");
                if (PartString != "")
                {

                    std::vector<std::string> StrVec =
                        StringFindElements(PartString, ":");
                    if (gvars::Debug)
                        std::cout << "StrVec[0]: " << StrVec[0] << std::endl;
                    float Leftover =
                        npc.bloodwork(StrVec[0], -atof(StrVec[1].c_str()));
                    if (gvars::Debug)
                        std::cout << "Bloodwork leftover is: " << Leftover
                                  << std::endl;
                    //NPC Critter;

                    for (size_t i = 0; i != StrVec.size(); i++)
                    {
                        if (gvars::Debug)
                            std::cout << StrVec[i] << std::endl;
                    }
                }

                PartNumber = StringFindNumber(WorkingLine, "[Orafice:");
                if (PartNumber > 0)
                {
                    //std::vector<item> * Inv = &npc.inventory;

                    //for(int i = 0; i != npc.inventory.size(); i++)
                    for (auto i = npc.inventory.begin();
                         i != npc.inventory.end(); i++)
                    {
                        bool FoundIt = false;
                        if ((*i).InsidePart == "" && (*i).MassFlesh > 0 &&
                            ConsumeFlesh) // Awww yeeessss, We gonna eat some flesh with our Orafice!
                        {
                            (*i).InsidePart = CurrentPart;
                            FoundIt = true;
                        }

                        if ((*i).InsidePart == "" && (*i).MassVeggy > 0 &&
                            ConsumeVeggy) // Awww yeeessss, We gonna eat something with our Orafice!
                        {
                            (*i).InsidePart = CurrentPart;
                            FoundIt = true;
                        }

                        if ((*i).InsidePart == "" && (*i).MassWater > 0 &&
                            ConsumeWater) // Awww yeeessss, We gonna eat something with our Orafice!
                        {
                            (*i).InsidePart = CurrentPart;
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

                            ChatBox.AddChat(ChtStr, sf::Color(150, 150, 0));
                        }
                    }
                }

                PartNumber = StringFindNumber(
                    WorkingLine, "[BloodPumpRate:"); // TODO: Do this right.
                if (PartNumber != 0)
                {
                    float Blood =
                        StringFindNumber(npc.bloodcontent, "[Nutrients:");
                    if (Blood > PartNumber)
                    {
                        if ((npc.maxhunger - npc.hunger) > PartNumber)
                        {
                            npc.hunger += PartNumber;
                            npc.bloodwork("Nutrients", -PartNumber);
                        }
                    }
                    Blood = StringFindNumber(npc.bloodcontent, "[Hydration:");
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

        debug("Debug: Ending Part Loop");
        // *BodyPart Loop*

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
            Effectz.CreateCircle(npc.xpos, npc.ypos, 11, sf::Color::Red);
        }
        if ((*HighestDesire).DesireType == "SelfDefense")
        {
            (*HighestDesire).Potency = 0;
            std::cout << (*HighestDesire).Potency << std::endl;
            InComplete = true;
        }
        if ((*HighestDesire).DesireType == "Sustainence")
        {
            bool InInv = false;
            for (auto &Inv : npc.inventory)
                if (Inv.MassFlesh > 0)
                {
                    npc.Target.Item = &Inv;
                    InInv = true;
                    break;
                }
            if (InInv == false)
            {
                for (auto &LclItms : worlditems)
                {
                    if (npc.Target.Item == nullptr && LclItms.MassFlesh > 0 &&
                        LclItms.User == nullptr)
                    {
                        npc.Target.Item =
                            &LclItms; // Is this creating a copy? The Behavior in testing still makes multiple critters target the same item.
                        LclItms.User = &npc;
                    }
                    else if (LclItms.MassFlesh > 0 && LclItms.User == nullptr)
                    {

                        float CurrentItem = math::closeish(
                            npc.xpos, npc.ypos, (*npc.Target.Item).xpos,
                            (*npc.Target.Item).ypos);
                        float NewItem = math::closeish(
                            npc.xpos, npc.ypos, LclItms.xpos, LclItms.ypos);
                        if (NewItem < CurrentItem)
                        {
                            (*npc.Target.Item).User = nullptr;
                            npc.Target.Item = &LclItms;
                            (*npc.Target.Item).User = &npc;
                        }
                    }
                }
            }

            if (npc.Target.Item != nullptr && InInv == false)
            {
                Effectz.CreateLine(npc.xpos, npc.ypos, (*npc.Target.Item).xpos,
                                   (*npc.Target.Item).ypos, 2,
                                   sf::Color::White);
            }
        }

        // Incase the highest desire isn't completable, Go through again for the next highest desire.
        for (auto &i : Desires)
        {
            std::cout << npc.name << ":" << i.DesireType << ": " << i.Potency
                      << std::endl;
        }

        if (InComplete)
            goto ReDesire;

        cText.CreateText(npc.xpos - 30, npc.ypos - 15, 10, sf::Color::Red,
                         (*HighestDesire).DesireType, ":",
                         (*HighestDesire).Potency);
        cText.CreateText(npc.xpos - 70, npc.ypos - 35, 10, sf::Color::Cyan,
                         npc.Body.BodyParts);

        /* End of Critter Prioritization */

        RemoveItems(npc.inventory);
    }
}

void updateNpc()
{
    if (gvars::Debug)
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

        while (SearchPos != npc.Body.BodyParts.npos) // Global Part Tags
        {
            SearchPos = npc.Body.BodyParts.find("{", SearchPos);

            if (SearchPos != npc.Body.BodyParts.npos)
            {
                EndPos = npc.Body.BodyParts.find("}", SearchPos);

                std::string WorkingLine;

                WorkingLine.append(npc.Body.BodyParts, SearchPos,
                                   EndPos - SearchPos);
                float PartNumber = 0;

                PartNumber = StringFindNumber(WorkingLine, "[Walk:");
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
                    StringFindNumber(WorkingLine, "[NutritionExtraction:");
                if (PartNumber != 0)
                {
                    GlobalNutritionPercentage += PartNumber;
                }

                PartNumber = StringFindNumber(WorkingLine, "[DigestsBlood:");
                if (PartNumber != 0)
                {
                }
                PartNumber = StringFindNumber(WorkingLine, "[DigestsFlesh:");
                if (PartNumber != 0)
                {
                    ConsumeFlesh = true;
                }
                PartNumber = StringFindNumber(WorkingLine, "[DigestsVeggy:");
                if (PartNumber != 0)
                {
                    ConsumeVeggy = true;
                }
                PartNumber = StringFindNumber(WorkingLine, "[DigestsWater:");
                if (PartNumber != 0)
                {
                    ConsumeWater = true;
                }

                SearchPos = EndPos;
            }
        }

        SearchPos = 0;
        EndPos = 0;

        //for(int i = 0; i != npc.inventory.size(); i++)
        for (auto i = npc.inventory.begin(); i != npc.inventory.begin(); i++)
        {
            if ((*i).InsidePart != "")
            {
                (*i).HasInternalUse--;
            }
        }

        while (SearchPos != npc.Body.BodyParts.npos) // Individual Part Tags
        {

            SearchPos = npc.Body.BodyParts.find("{", SearchPos);

            if (SearchPos != npc.Body.BodyParts.npos)
            {
                EndPos = npc.Body.BodyParts.find("}", SearchPos);
                Parts++;

                std::string WorkingLine;

                WorkingLine.append(npc.Body.BodyParts, SearchPos,
                                   EndPos - SearchPos);

                float PartNumber = 0;
                std::string PartString = "";
                item *PartItem;

                std::string CurrentPart =
                    StringFindString(WorkingLine, "[Name:");

                PartNumber = StringFindNumber(WorkingLine, "[DigestsBlood:");
                PartItem = GetItemPtrfromVector(npc.inventory, "Blood");
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
                            Nutr * PercentageBuff(GlobalNutritionPercentage));
                    }
                    else
                    {
                        //*GetItemPtrfromVector(npc.inventory,"Blood").amount = 0;
                        GetItemPtrfromVector(npc.inventory, "Blood")->ToDelete =
                            true;
                        float Nutr =
                            WorkAmount * 100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Nutrients",
                            Nutr * PercentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartNumber = StringFindNumber(WorkingLine, "[DigestsFlesh:");
                PartItem =
                    GetItemPtrfromVectorVarSearch(npc.inventory, "MassFlesh");
                //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
                if (PartNumber != 0 && PartItem != nullptr &&
                    PartItem->MassFlesh >
                        0) // TODO: Make sure the item is in THIS PART before digesting it!
                {
                    //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                    float WorkAmount = PartItem->MassFlesh;
                    float Diff = WorkAmount - (PartNumber / 1000);

                    if (Diff > 0)
                    {
                        PartItem->MassFlesh = Diff;
                        PartItem->HasInternalUse = 0;
                        float Nutr = (WorkAmount - Diff) *
                                     100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Nutrients",
                            Nutr * PercentageBuff(GlobalNutritionPercentage));
                    }
                    if (PartItem->MassFlesh <= 0)
                    {
                        //*GetItemPtrfromVector(npc.inventory,"Blood").amount = 0;
                        PartItem->ToDelete = true;
                        //Add Food to everyone, Make sure they go hungry to eat it, Figure out a way to Eject the empty item, Or do water! Everyone starts with water.
                        //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartNumber = StringFindNumber(WorkingLine, "[DigestsVeggy:");
                PartItem =
                    GetItemPtrfromVectorVarSearch(npc.inventory, "MassVeggy");
                //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
                if (PartNumber != 0 && PartItem != nullptr &&
                    PartItem->MassVeggy >
                        0) // TODO: Make sure the item is in THIS PART before digesting it!
                {
                    //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                    float WorkAmount = PartItem->MassVeggy;
                    float Diff = WorkAmount - (PartNumber / 1000);

                    if (Diff > 0)
                    {
                        PartItem->MassVeggy = Diff;
                        PartItem->HasInternalUse = 0;
                        float Nutr = (WorkAmount - Diff) *
                                     100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Nutrients",
                            Nutr * PercentageBuff(GlobalNutritionPercentage));
                    }
                    if (PartItem->MassVeggy <= 0)
                    {
                        PartItem->ToDelete = true;
                        //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartNumber = StringFindNumber(WorkingLine, "[DigestsWater:");
                PartItem =
                    GetItemPtrfromVectorVarSearch(npc.inventory, "MassWater");
                //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
                if (PartNumber != 0 && PartItem != nullptr &&
                    PartItem->MassWater >
                        0) // TODO: Make sure the item is in THIS PART before digesting it!
                {
                    //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                    float WorkAmount = PartItem->MassWater;
                    float Diff = WorkAmount - (PartNumber / 1000);

                    if (Diff > 0)
                    {
                        PartItem->MassWater = Diff;
                        PartItem->HasInternalUse = 0;
                        float Nutr = (WorkAmount - Diff) *
                                     100; // TODO: Figure this out better.
                        npc.bloodwork(
                            "Hydration",
                            Nutr * PercentageBuff(GlobalNutritionPercentage));
                    }
                    if (PartItem->MassWater <= 0)
                    {
                        PartItem->ToDelete = true;
                        //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                    }
                }

                PartString = StringFindString(WorkingLine, "[PoisonFilter:");
                if (PartString != "")
                {

                    std::vector<std::string> StrVec =
                        StringFindElements(PartString, ":");
                    if (gvars::Debug)
                        std::cout << "StrVec[0]: " << StrVec[0] << std::endl;
                    float Leftover =
                        npc.bloodwork(StrVec[0], -atof(StrVec[1].c_str()));
                    if (gvars::Debug)
                        std::cout << "Bloodwork leftover is: " << Leftover
                                  << std::endl;
                    //NPC Critter;

                    for (size_t i = 0; i != StrVec.size(); i++)
                    {
                        if (gvars::Debug)
                            std::cout << StrVec[i] << std::endl;
                    }
                }

                PartNumber = StringFindNumber(WorkingLine, "[Orafice:");
                if (PartNumber > 0)
                {
                    //std::vector<item> * Inv = &npc.inventory;

                    //for(int i = 0; i != npc.inventory.size(); i++)
                    for (auto i = npc.inventory.begin();
                         i != npc.inventory.begin(); i++)
                    {
                        bool FoundIt = false;
                        if ((*i).InsidePart == "" && (*i).MassFlesh > 0 &&
                            ConsumeFlesh) // Awww yeeessss, We gonna eat some flesh with our Orafice!
                        {
                            (*i).InsidePart = CurrentPart;
                            FoundIt = true;
                        }

                        if ((*i).InsidePart == "" && (*i).MassVeggy > 0 &&
                            ConsumeVeggy) // Awww yeeessss, We gonna eat something with our Orafice!
                        {
                            (*i).InsidePart = CurrentPart;
                            FoundIt = true;
                        }

                        if ((*i).InsidePart == "" && (*i).MassWater > 0 &&
                            ConsumeWater) // Awww yeeessss, We gonna eat something with our Orafice!
                        {
                            (*i).InsidePart = CurrentPart;
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

                            ChatBox.AddChat(ChtStr, sf::Color(150, 150, 0));
                        }
                    }
                }

                PartNumber = StringFindNumber(
                    WorkingLine, "[BloodPumpRate:"); // TODO: Do this right.
                if (PartNumber != 0)
                {
                    float Blood =
                        StringFindNumber(npc.bloodcontent, "[Nutrients:");
                    if (Blood > PartNumber)
                    {
                        if ((npc.maxhunger - npc.hunger) > PartNumber)
                        {
                            npc.hunger += PartNumber;
                            npc.bloodwork("Nutrients", -PartNumber);
                        }
                    }
                    Blood = StringFindNumber(npc.bloodcontent, "[Hydration:");
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

                ChatBox.AddChat(ChtStr,sf::Color(150,150,0));
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

            npc.TentArm1.x =
                math::clamp(npc.TentArm1.x + randz(-3, 3), -20, 20);
            npc.TentArm1.y =
                math::clamp(npc.TentArm1.y + randz(-3, 3), -20, 20);
            npc.TentArm2.x =
                math::clamp(npc.TentArm2.x + randz(-3, 3), -20, 20);
            npc.TentArm2.y =
                math::clamp(npc.TentArm2.y + randz(-3, 3), -20, 20);
            npc.TentEnd1.x =
                math::clamp(npc.TentEnd1.x + randz(-3, 3), -20, 20);
            npc.TentEnd1.y =
                math::clamp(npc.TentEnd1.y + randz(-3, 3), -20, 20);
            npc.TentEnd2.x =
                math::clamp(npc.TentEnd2.x + randz(-3, 3), -20, 20);
            npc.TentEnd2.y =
                math::clamp(npc.TentEnd2.y + randz(-3, 3), -20, 20);
        }

        if (npc.id == gvars::MyTargetid && Key.space)
        {
            npc.Attacking = true;
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

                ChatBox.AddChat(ChtStr, sf::Color(200, 0, 0));
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
            npc.Tag("[ZombieHater:", -9999999);
            npc.Tag("[EatsFlesh:", 1);
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
            npc.IsNPC = true;
            npc.NeedsFood = true;
            npc.AllowedFood = false;
            npc.NeedsWater = false;
            npc.AllowedDrink = false;
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

            ChatBox.AddChat(ChtStr, sf::Color(200, 0, 0));
        };
        if (Key.rshift && npc.id == gvars::MyTargetid)
        {
            std::cout << npc.target << "At: " << npc.TargetPos.x << ":"
                      << npc.TargetPos.y << std::endl;
            std::cout << "HasTarget: " << npc.HasTarget
                      << " AtTarget: " << npc.AtTarget << std::endl;
        }

        if (npc.Attacking)
        {
            if (npc.attacktimer <= 0)
            {
                if (gvars::Debug)
                {
                    std::cout << "Pre Mel Ran \n";
                }
                bool bMel = npc.HasItemType(1);
                item Mel;
                if (bMel)
                {
                    Mel = *npc.GetItemType(1);
                }
                item Ran;
                bool bRan = npc.HasItemType(2);
                if (bRan == true)
                {
                    Ran = *npc.GetItemType(2);
                    debug("Fish n Chips");
                }
                try
                {
                    bool Attacked = false;
                    if (gvars::Debug)
                    {
                        std::cout << "Pre Mel \n";
                    }
                    if (bMel == true)
                    {
                        if (Mel.type == 1 &&
                            math::closeish(npc.xpos, npc.ypos, npc.ShootPos.x,
                                           npc.ShootPos.y) < Mel.range)
                        {
                            Attacked = true;
                            std::set<int> Tarz;
                            Tarz = npc.Melee(1, 1, GridSize);
                            for (const auto &elem : Tarz)
                            {
                                npclist.at(elem)
                                    .modhealth(-(randz(Mel.mindam, Mel.maxdam) +
                                                 npc.Skills.strength));
                                if (npc.HasTag("[CanLearn:"))
                                {
                                    npc.Skills.Train("strength");
                                }
                            }
                            if (npc.Attacking == false)
                            {
                                npc.attacktimer =
                                    (npc.attacktimerint -
                                     (math::clamp(npc.Skills.agility / 10, 10,
                                                  100)));
                            } // Melee has a different method for saying it's done.
                            if (gvars::Debug)
                            {
                                std::cout << "Post Mel \n";
                            }
                        }
                    }
                    if (gvars::Debug)
                    {
                        std::cout << "Pre Ran \n";
                    }
                    if (bRan == true && Attacked == false)
                    {
                        if (Ran.type == 2)
                        {
                            bool Friendly = false;
                            std::set<int> ids =
                                NPCTrace(npc.xpos, npc.ypos, npc.TargetPos.x,
                                         npc.TargetPos.y, npc.id);
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
                                gridposTrace(npc.xpos, npc.ypos, npc.ShootPos.x,
                                             npc.ShootPos.y, npc.id,
                                             npc.TargetPos) == true &&
                                math::closeish(npc.xpos, npc.ypos,
                                               npc.ShootPos.x,
                                               npc.ShootPos.y) <= npc.viewrange)
                            {
                                int Tempx = randz(
                                    0,
                                    math::clamp(
                                        100 - npc.Skills.dexterity, 0,
                                        100)); // This is to mess up the aiming.
                                int Tempy = randz(
                                    0, math::clamp(100 - npc.Skills.dexterity,
                                                   0, 100));
                                if (npc.HasTag("[CanLearn:"))
                                {
                                    npc.Skills.Train("dexterity");
                                }
                                if (randz(0, 1) == 1)
                                {
                                    Tempx = -Tempx;
                                }
                                if (randz(0, 1) == 1)
                                {
                                    Tempy = -Tempy;
                                }
                                sf::Vector2f SP(gvars::MousePos.x,
                                                gvars::MousePos.y);
                                sf::Vector2f Targ(npc.ShootPos.x + Tempx,
                                                  npc.ShootPos.y + Tempy);
                                Effectz.CreateLine(npc.xpos, npc.ypos, Targ.x,
                                                   Targ.y, 2, sf::Color::White);
                                std::set<int> ids = NPCTrace(
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
                                                      npc.Skills.perception));
                                                if (npc.HasTag("[CanLearn:"))
                                                {
                                                    npc.Skills.Train(
                                                        "perception");
                                                }
                                                Effectz.CreateCircle(
                                                    npclist.at(id).xpos,
                                                    npclist.at(id).ypos, 5,
                                                    sf::Color::Red);
                                            }
                                        }
                                        catch (std::exception &e)
                                        {
                                        }
                                        cText.CreateText(npc.xpos, npc.ypos, 11,
                                                         sf::Color::Red,
                                                         "Bang!");
                                    }
                                }
                                npc.Attacking = false;
                                npc.attacktimer =
                                    (npc.attacktimerint -
                                     (math::clamp(npc.Skills.agility / 10, 10,
                                                  100)));
                            }
                            else
                            {
                                npc.Attacking = false;
                            }
                            if (gvars::Debug)
                            {
                                std::cout << "Post Ran \n";
                            }
                        }
                    }
                    else
                    {
                        npc.Attacking = false;
                    }
                }
                catch (std::exception &e)
                {
                    std::cout << "Itemerror\n";
                }
            }
            else
            {
                npc.Attacking = false;
            }
        } //End of Attacking
        if (npc.canmove == true && npc.Attacking == false)
        {
            //std::cout << id << " is acting." << std::endl;
            // First, Check values, to see if hungry/thirsty/hurt then check if being attacked
            // Add an IsSafe before these checks
            if (npc.IsHungry() == false && npc.IsThirsty() == false)
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

                if (npc.name == "Zombie" && Key.lctrl == true)
                {
                    npc.moverate = npc.moverateint * 4;
                }
                if (npc.name == "Zombie" && Key.lalt == true)
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
                if (npc.IsHurt() == true)
                {
                    //target = "Meds"; //std::cout << id << " is hurt." << std::endl;
                }
                else if (npc.IsHungry() == true && npc.AllowedFood == true)
                {
                    npc.target =
                        "Food"; //std::cout << id << " is hungry." << std::endl;
                    if (npc.IsThirsty())
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
                else if (npc.IsThirsty() == true)
                {
                    npc.target = "Water";
                    //std::cout << id << " is thirsty." << std::endl;
                    bool IsWater = false;
                    for (auto &worlditem : worlditems)
                    {
                        if (worlditem.MassWater > 0)
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
                if (npc.target == "Wander" && npc.HasTarget == true)
                { // TODO: Make sure this isn't needed anymore, Then delete it.
                    if (gvars::groundmap[gvars::currentz][abs_to_index(
                            npc.TargetPos.x /
                            GridSize)][abs_to_index(npc.TargetPos.y /
                                                    GridSize)] == 10)
                    {
                        npc.HasTarget = false;
                    }
                }
                if (npc.target == "Food")
                { // Compress this later by simply directly linking the target towards the search, Probably need a bool for Item or NPC to simplfy it.
                    item *Item = FindClosestItemPtr(npc.xpos, npc.ypos, "Food");
                    if (Item != nullptr)
                    {
                        if (Item->MassFlesh > 0)
                        {
                            npc.TargetPos =
                                sf::Vector2f(Item->xpos, Item->ypos);
                            npc.TargetId = Item->id;
                            //npc.TargetVectorId = Item.at(3);
                            npc.target = "Food";
                            npc.HasTarget = true;
                        }
                    }
                    else
                    {
                        npc.target = "Wander";
                    }
                }
                if (npc.target == "Water")
                {
                    auto Item = FindClosestItem(npc.xpos, npc.ypos, "Water");
                    if (Item.found)
                    {
                        npc.TargetPos = sf::Vector2f(Item.xpos, Item.ypos);
                        npc.TargetId = Item.id;
                        npc.TargetVectorId = Item.vectorPosition;
                        npc.target = "Water";
                        npc.HasTarget = true;
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
                        npc.TargetPos = sf::Vector2f(closx, closy);
                        npc.TargetId = ClosID;
                        npc.target = "Flesh";
                        npc.HasTarget = true;
                    }
                    else
                    {
                        npc.target = "Wander";
                    }
                }

                try
                { // Jobs Section
                    if (UniFact[0].JobList.size() != 0 &&
                        npc.target == "Wander" && npc.Faction == "The Alphas")
                    {
                        debug("Preforming Job Routine; ");
                        for (size_t i = 0; i != UniFact[0].JobList.size(); i++)
                        {
                            debug("Starting Job " + std::to_string(i));
                            //if(!Deleting) Deleter++;
                            //Con(AddString(npc.name,JobList[i].Type));
                            if (UniFact[0].JobList[i].pItem == nullptr &&
                                UniFact[0].JobList[i].Type ==
                                    "PickUp") // Deleting objectless pickup jobs.
                            { // Deleting objectless pickup jobs.
                                UniFact[0].JobList[i].ToDelete = true;
                                break;
                            }

                            if ((UniFact[0].JobList[i].pWorker == nullptr &&
                                 npc.HasJob == false &&
                                 UniFact[0].JobList[i].ToDelete == false) ||
                                (UniFact[0].JobList[i].pWorker != nullptr &&
                                 UniFact[0].JobList[i].pWorker->id == npc.id &&
                                 UniFact[0].JobList[i].ToDelete == false))
                            {
                                debug("Comparitive Success");
                                if (UniFact[0].JobList[i].pWorker == nullptr)
                                {
                                    std::cout << npc.name
                                              << " has job: " << npc.HasJob
                                              << ", and yet :";
                                    UniFact[0].JobList[i].pWorker = &npclist.at(
                                        IntegerIterator); // Not sure if this is technically better or worse than repointing every frame.
                                    UniFact[0].JobList[i].pWorker->HasJob =
                                        true;
                                    std::cout << npc.HasJob << " does now? \n";
                                    //fSleep(2);
                                }

                                if (UniFact[0].JobList[i].Type == "Build")
                                {
                                    debug("Starting Build");

                                    //if(GetItemPtrfromVector(worlditems, "Wood") != NULL) item WorkLoad = *GetItemPtrfromVector(worlditems, "Wood");
                                    item *InvWood = GetItemPtrfromVector(
                                        npc.inventory, "Wood");
                                    item *WldWood = GetItemPtrfromVector(
                                        worlditems, "Wood");

                                    if (InvWood != nullptr)
                                    {
                                        Con("Success! I have Wood!");
                                        int x = UniFact[0].JobList[i].WorkPos.x;
                                        int y = UniFact[0].JobList[i].WorkPos.y;

                                        npc.TargetPos.x = x;
                                        npc.TargetPos.y = y;
                                        npc.HasTarget = true;
                                        npc.target = "BuildWoodWall";

                                        if (math::closeish(npc.xpos, npc.ypos,
                                                           x,
                                                           y) <= npc.size * 3)
                                        {

                                            npc.TargetPos.x = npc.xpos;
                                            npc.TargetPos.y = npc.ypos;

                                            UniFact[0]
                                                .JobList[i]
                                                .CompletionProgress +=
                                                npc.Skills.intelligence / 2;

                                            //std::cout << "JobTimer: " << UniFact[0].JobList[i].CompletionProgress << std::endl;
                                            //Effectz.CreateCircle(UniFact[0].JobList[i].WorkPos.x,UniFact[0].JobList[i].WorkPos.y, (UniFact[0].JobList[i].CompletionProgress - UniFact[0].JobList[i].CompletionTimer)/10  ,sf::Color(150,150,150,150));

                                            //std::cout << 361*(PercentIs(UniFact[0].JobList[i].CompletionTimer, UniFact[0].JobList[i].CompletionProgress)/100) << std::endl;
                                            //fSleep(4);
                                            for (
                                                float Rot = 1;
                                                Rot <
                                                    361 *
                                                        (PercentIs(
                                                             UniFact[0]
                                                                 .JobList[i]
                                                                 .CompletionTimer,
                                                             UniFact[0]
                                                                 .JobList[i]
                                                                 .CompletionProgress) /
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

                                                Effectz.CreateLine(
                                                    x, y, XPos, YPos, 1,
                                                    sf::Color(150, 150, 150,
                                                              150));
                                            }

                                            if (UniFact[0]
                                                    .JobList[i]
                                                    .CompletionProgress >=
                                                UniFact[0]
                                                    .JobList[i]
                                                    .CompletionTimer)

                                            {
                                                Tiles[abs_to_index(x / 20)]
                                                     [abs_to_index(y / 20)][30]
                                                         .Wall();
                                                //Tiles[abs_to_index(x/20)][abs_to_index(y/20)][30].ID = 1010;
                                                //Tiles[abs_to_index(x/20)][abs_to_index(y/20)][30].Img.setTexture( *imagemanager.GetImage("Wall.png"));
                                                InvWood->ToDelete = true;
                                                UniFact[0].JobList[i].ToDelete =
                                                    true;
                                                UniFact[0]
                                                    .JobList[i]
                                                    .pWorker->HasJob = false;
                                                //Crashed instantly;
                                            }
                                        }
                                    }

                                    else if (WldWood != nullptr)
                                    {

                                        Con("Partial Success! World has Wood!");
                                        //item WorkLoad = *GetItemPtrfromVector(worlditems, "Wood");

                                        npc.TargetPos.x = WldWood->xpos;
                                        npc.TargetPos.y = WldWood->ypos;
                                        npc.HasTarget = true;
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
                                            WldWood->ToDelete = true;
                                        }
                                    }
                                    debug("Ended Build");
                                }

                                else if (UniFact[0].JobList[i].pItem != nullptr)
                                {
                                    debug("Starting pItem != NULL");

                                    npc.TargetPos.x =
                                        UniFact[0].JobList[i].pItem->xpos;
                                    npc.TargetPos.y =
                                        UniFact[0].JobList[i].pItem->ypos;
                                    npc.HasTarget = true;
                                    npc.target =
                                        UniFact[0].JobList[i].pItem->name;
                                    debug("Post HasTarget");

                                    if (UniFact[0].JobList[i].Type ==
                                            "PickUp" &&
                                        math::closeish(
                                            npc.xpos, npc.ypos,
                                            UniFact[0].JobList[i].pItem->xpos,
                                            UniFact[0]
                                                .JobList[i]
                                                .pItem->ypos) <= npc.size)
                                    {
                                        //Con("I'm there! \n");
                                        //Deleting = true;
                                        npc.inventory.push_back(
                                            *UniFact[0].JobList[i].pItem);
                                        debug("Post Inventory Pushback");

                                        UniFact[0].JobList[i].pItem->ToDelete =
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

                                        UniFact[0].JobList[i].pItem = nullptr;
                                        UniFact[0].JobList[i].ToDelete = true;
                                        UniFact[0].JobList[i].pWorker->HasJob =
                                            false;
                                        UnpointItems(worlditems);
                                        debug("Post Unpoint");
                                    }

                                    if (UniFact[0].JobList[i].Type == "Chop" &&
                                        math::closeish(
                                            npc.xpos, npc.ypos,
                                            UniFact[0].JobList[i].pItem->xpos,
                                            UniFact[0]
                                                .JobList[i]
                                                .pItem->ypos) <= npc.size)
                                    {
                                        debug("Post Chopcheck");
                                        //Con("I'm there! \n");
                                        //Deleting = true;
                                        //npc.inventory.push_back(*UniFact[0].JobList[i].pItem);
                                        //FUCKNUTS start here, Just made the plank, Make the tree give the planks, MmkAY?!
                                        item WoodStuffs =
                                            *GetGlobalItem("Wood");

                                        debug("Post WoodStuffs");

                                        WoodStuffs.xpos =
                                            UniFact[0].JobList[i].pItem->xpos;
                                        WoodStuffs.ypos =
                                            UniFact[0].JobList[i].pItem->ypos;
                                        worlditems.push_back(WoodStuffs);
                                        debug("Post WoodSpawn");

                                        UniFact[0].JobList[i].pItem->ToDelete =
                                            true;
                                        UniFact[0].JobList[i].pItem = nullptr;
                                        UniFact[0].JobList[i].ToDelete = true;
                                        UniFact[0].JobList[i].pWorker->HasJob =
                                            false;
                                        debug("Post ToDelete");
                                        UnpointItems(worlditems);

                                        //items are not getting deleted, ugh.
                                    }

                                    debug("Ending pItem != NULL");
                                }

                                if (UniFact[0].JobList[i].Type == "Dig")
                                {

                                    PathFindWorkPos.x =
                                        UniFact[0].JobList[i].WorkPos.x;
                                    PathFindWorkPos.y =
                                        UniFact[0].JobList[i].WorkPos.y;

                                    npc.TargetPos.x = PathFindWorkPos.x;
                                    npc.TargetPos.y = PathFindWorkPos.y;
                                    npc.HasTarget = true;
                                    npc.target = "DigNaturalWall";

                                    if (math::closeish(npc.xpos, npc.ypos,
                                                       PathFindWorkPos.x,
                                                       PathFindWorkPos.y) <=
                                        npc.size * 3)
                                    {

                                        npc.TargetPos.x = npc.xpos;
                                        npc.TargetPos.y = npc.ypos;

                                        UniFact[0]
                                            .JobList[i]
                                            .CompletionProgress +=
                                            npc.Skills.strength / 2;

                                        for (
                                            float Rot = 1;
                                            Rot <
                                                361 *
                                                    (PercentIs(
                                                         UniFact[0]
                                                             .JobList[i]
                                                             .CompletionTimer,
                                                         UniFact[0]
                                                             .JobList[i]
                                                             .CompletionProgress) /
                                                     100);
                                            Rot++)
                                        { // Pretty circle progressing graphic.

                                            float XPos =
                                                PathFindWorkPos.x +
                                                sin(Rot * PI / 180) * 10;
                                            float YPos =
                                                PathFindWorkPos.y +
                                                cos(Rot * PI / 180) * 10;

                                            Effectz.CreateLine(
                                                PathFindWorkPos.x,
                                                PathFindWorkPos.y, XPos, YPos,
                                                1,
                                                sf::Color(150, 150, 150, 150));
                                        }

                                        if (UniFact[0]
                                                .JobList[i]
                                                .CompletionProgress >=
                                            UniFact[0]
                                                .JobList[i]
                                                .CompletionTimer)

                                        {
                                            Tiles[abs_to_index(
                                                PathFindWorkPos.x / 20)]
                                                 [abs_to_index(
                                                     PathFindWorkPos.y /
                                                     20)][30].Stone();
                                            debug("Spawning Rocks");
                                            int TAR = randz(
                                                3, 8); // Throw away random
                                            for (int z = 0; z != TAR; z++)
                                            {
                                                item StoneStuffs =
                                                    *GetGlobalItem("Rock");

                                                StoneStuffs.xpos =
                                                    PathFindWorkPos.x +
                                                    (4 - randz(1, 8));
                                                StoneStuffs.ypos =
                                                    PathFindWorkPos.y +
                                                    (4 - randz(1, 8));
                                                worlditems.push_back(
                                                    StoneStuffs);
                                            }

                                            UniFact[0].JobList[i].ToDelete =
                                                true;
                                            UniFact[0]
                                                .JobList[i]
                                                .pWorker->HasJob = false;
                                            debug("Dig Wall Completed");
                                        }
                                    }
                                }
                            }

                            debug("End Job " + std::to_string(i));
                        }

                        RemoveJobs(UniFact[0].JobList);

                        debug("Finished Job Activity. \n ");
                    }

                    if (npc.target == "Wander" && npc.HasTarget == false)
                    { // Find somewhere random to walk towards, so long as there isn't anything on that spot.
                        bool FindEmpty = false;
                        while (FindEmpty == false)
                        {
                            npc.TargetPos = sf::Vector2f(randz(700, 1300),
                                                         randz(700, 1300));
                            npc.HasTarget = true;
                            if (gvars::groundmap[gvars::currentz][abs_to_index(
                                    npc.TargetPos.x /
                                    GridSize)][abs_to_index(npc.TargetPos.y /
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

                if (npc.name == "Shinobi" && npc.HasWeapon() == false)
                {
                    auto Item = FindClosestItem(npc.xpos, npc.ypos, "Sword");
                    if (Item.found)
                    {
                        npc.TargetPos = sf::Vector2f(Item.xpos, Item.ypos);
                        npc.TargetId = Item.id;
                        npc.TargetVectorId = Item.vectorPosition;
                        npc.target = "Sword";
                        npc.HasTarget = true;
                    }
                    else
                    {
                        npc.target = "Wander";
                    }
                }
                if (npc.HasWeapon())
                { // Attack nearby Hostiles.
                    int closx = -100000;
                    int closy = -100000;
                    bool FoundOne = false;
                    if (npc.HasTag("[ZombieHater:"))
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
                            npc.ShootPos = sf::Vector2f(closx, closy);
                            if (math::closeish(npc.xpos, npc.ypos,
                                               npc.ShootPos.x,
                                               npc.ShootPos.y) <= npc.viewrange)
                            {
                                npc.Attacking = true;
                            }
                        }
                    }
                    if (npc.IsHungry() == false && npc.IsThirsty() == false &&
                        FoundOne == true)
                    {
                        npc.TargetPos = sf::Vector2f(closx, closy);
                    }
                }

                if (npc.CloseToTarget(npc.reach, npc.TargetPos) == true &&
                    npc.target == "Flesh")
                {
                    if (npc.attacktimer == 0)
                    {

                        std::string AtkType;
                        npc.attacktimer =
                            (npc.attacktimerint -
                             (math::clamp(npc.Skills.agility / 10, 10, 100))) *
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
                            npc.AtTarget = false;
                            npc.HasTarget =
                                false; // TODO: Fix Zombie Attacking, Give it some Delay, and some damage based on Strength and filled belly.
                            int zDamage = randz(0, npclist.at(numba).health);
                            if (npc.name != "Azabul")
                                npc.fillhunger(zDamage);
                            if (npc.name != "Azabul")
                                npclist.at(numba)
                                    .bloodwork("Zombification", 100000);
                            if (npc.name == "Azabul")
                            {
                                npc.TentArm1.x = npclist.at(numba).xpos;
                                npc.TentArm1.y = npclist.at(numba).ypos;
                                npc.TentArm2.x = npclist.at(numba).xpos;
                                npc.TentArm2.y = npclist.at(numba).ypos;
                                //BLARGGITY
                                item Blood;
                                Blood.amount = zDamage;
                                Blood.name = "Blood";
                                Blood.InsidePart = "LowerTorso";
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

                            ChatBox.AddChat(AtkStr, sf::Color::Red);
                        }
                    }
                }
                if (npc.name == "Mini Turret")
                {
                    if (npc.HasWeapon() == false)
                    {
                        npc.AddItem("5mmCannon", 1);
                        npc.Skills.dexterity = 90;
                        npc.Skills.agility = 10;
                    }
                    if (npc.HasTag("[ZombieHater:") == false)
                    {
                        npc.Tag("[ZombieHater:", 1);
                        std::cout << "Giving MiniTurret [ZombieHater: \n";
                    }
                }
            } // Ending of "Act" action
            if (gvars::Debug)
                std::cout << "Post Act Section \n";

            { // Vision check and Activation of Path Finding.
                if (gvars::Debug)
                    std::cout << "Pre 'set' vision. \n";

                bool FoundGoal = false;
                if (math::closeish(npc.TargetPos.x, npc.TargetPos.y, npc.xpos,
                                   npc.ypos) <= npc.viewrange &&
                    npc.cbaseid != -1337)
                {

                    float Ang = math::angleBetweenVectors(
                        math::Vec2f(npc.TargetPos.x, npc.TargetPos.y),
                        math::Vec2f(npc.xpos, npc.ypos));
                    float MyAngle = npc.angle;
                    float difference = abs(Ang - MyAngle);
                    if (difference > 180.0f)
                    {
                        difference = 360.0f - difference;
                    }
                    if (difference >= -90.0f && difference <= 90.0f)
                    {
                        if (gridposTrace(npc.xpos, npc.ypos, npc.TargetPos.x,
                                         npc.TargetPos.y, npc.id,
                                         npc.TargetPos) == true)
                        {
                            FoundGoal = true;
                            Effectz.CreateLine(npc.xpos, npc.ypos,
                                               npc.TargetPos.x, npc.TargetPos.y,
                                               1, sf::Color::White);
                            npc.NeedsPath = false;
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
                    if (gvars::Debug)
                        std::cout << "FoundGoal == false";
                    npc.NeedsPath = true;

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

                    npc.PathFinding.MyFindPath(
                        npc.xpos, npc.ypos, npc.TargetPos.x,
                        npc.TargetPos
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
                if (gvars::Debug)
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
                if (npc.AtTarget == true && npc.target == "Goto")
                {
                    npc.AtTarget = false;
                    npc.HasTarget = false;
                    npc.target = "None";
                }
            } // Ending of "Orders" action
            if (npc.action == "Pickup")
            {
                //if(npc.NeedsPath == false){if(npc.target != "None"){ npc.DirMove(npc.TargetPos);}}
            }
            if ((npc.target != "None" && npc.AtTarget) ||
                (npc.target != "None" &&
                 math::closeish(npc.xpos, npc.ypos, npc.TargetPos.x,
                                npc.TargetPos.y) <= npc.size))
            {
                if (GetItemVectorId(npc.TargetId) != -1)
                {
                    auto ItemItr = worlditems.begin();
                    std::advance(ItemItr, GetItemVectorId(npc.TargetId));

                    //if(Math::Closeish(npc.xpos,npc.ypos,worlditems.at(GetItemVectorId(npc.TargetId)).xpos,worlditems.at(GetItemVectorId(npc.TargetId)).ypos) <= npc.reach && worlditems.at(GetItemVectorId(npc.TargetId)).Pickupable == true)
                    if (math::closeish(npc.xpos, npc.ypos, (*ItemItr).xpos,
                                       (*ItemItr).ypos) <= npc.reach &&
                        (*ItemItr).Pickupable == true)
                    {
                        try
                        {
                            npc.inventory.push_back((*ItemItr));
                            RemoveItem(npc.TargetId);
                        }
                        catch (std::exception &e)
                        {
                            cText.CreateText(npc.xpos, npc.ypos, 11,
                                             sf::Color::Red,
                                             "Somethings wrong!");
                        };
                    }
                }
                if (npc.target == "Wander")
                {
                    npc.AtTarget = false;
                    npc.HasTarget = false;
                }
                if (true == false)
                {
                    if (npc.target == "Sword")
                    {
                        if (GetItemVectorId(npc.TargetId) != -1)
                        {
                            if (math::closeish(
                                    npc.xpos, npc.ypos,
                                    (*ListGet(worlditems,
                                              GetItemVectorId(npc.TargetId)))
                                        .xpos,
                                    (*ListGet(worlditems,
                                              GetItemVectorId(npc.TargetId)))
                                        .ypos) <= npc.reach * 2)
                            {
                                try
                                {
                                    npc.inventory.push_back(
                                        (*ListGet(
                                             worlditems,
                                             GetItemVectorId(npc.TargetId))));
                                    RemoveItem(npc.TargetId);
                                }
                                catch (std::exception &e)
                                {
                                };
                            }
                        }
                    }
                    else if (npc.target == "Food")
                    {
                        if (GetItemVectorId(npc.TargetId) != -1)
                        {
                            if (math::closeish(
                                    npc.xpos, npc.ypos,
                                    (*ListGet(worlditems,
                                              GetItemVectorId(npc.TargetId)))
                                        .xpos,
                                    (*ListGet(worlditems,
                                              GetItemVectorId(npc.TargetId)))
                                        .ypos) <= npc.size * 2)
                            {
                                try
                                {
                                    //npc.inventory.push_back(worlditems.at(GetItemVectorId(npc.TargetId)));
                                    npc.fillhunger((*ListGet(worlditems,
                                                             GetItemVectorId(
                                                                 npc.TargetId)))
                                                       .hungervalue);
                                    RemoveItem(npc.TargetId);
                                    npc.AtTarget = false;
                                    npc.HasTarget = false;
                                    npc.TargetId = -1;
                                    npc.TargetVectorId = -1;
                                    cText.CreateText(npc.xpos, npc.ypos, 11,
                                                     sf::Color::Blue,
                                                     "*Crunch!*");
                                }
                                catch (std::exception &e)
                                {
                                    cText.CreateText(npc.xpos, npc.ypos, 11,
                                                     sf::Color::Red,
                                                     "What the FUCK?!");
                                };
                            }
                        }
                    }
                    else if (npc.target == "Water")
                    {
                        if (GetItemVectorId(npc.TargetId) != -1)
                        {
                            if (math::closeish(
                                    npc.xpos, npc.ypos,
                                    (*ListGet(worlditems,
                                              GetItemVectorId(npc.TargetId)))
                                        .xpos,
                                    (*ListGet(worlditems,
                                              GetItemVectorId(npc.TargetId)))
                                        .ypos) <= npc.size * 2)
                            {
                                try
                                {
                                    //npc.inventory.push_back(worlditems.at(GetItemVectorId(npc.TargetId)));
                                    npc.fillthirst((*ListGet(worlditems,
                                                             GetItemVectorId(
                                                                 npc.TargetId)))
                                                       .thirstvalue);
                                    npc.AtTarget = false;
                                    npc.HasTarget = false;
                                    npc.TargetId = -1;
                                    npc.TargetVectorId = -1;
                                    cText.CreateText(npc.xpos, npc.ypos, 11,
                                                     sf::Color::Blue,
                                                     "*Gulp!*");
                                }
                                catch (std::exception &e)
                                {
                                    cText.CreateText(npc.xpos, npc.ypos, 11,
                                                     sf::Color::Red,
                                                     "What the FUCK?!");
                                };
                            }
                        }
                    }
                }
            }
            if (gvars::Debug)
                std::cout << "Post Item Pickups. \n";

            if (npc.movetimer <= 0)
            {
                npc.movetimer =
                    (npc.movetimerint +
                     npc.movetimer); // TODO: Figure out why I added 0 to this, Year later: It was because movetimer may be less than 0, I wanted the next to turn happen as soon as possible due to it.
                if (npc.target != "None" && npc.NeedsPath == false &&
                    math::Vec2f(npc.xpos, npc.ypos) != npc.TargetPos)
                { // Walk Move
                    if (npc.Attacking && npc.HasWeapon("Gun") &&
                        math::closeish(npc.xpos, npc.ypos, npc.TargetPos.x,
                                       npc.TargetPos.y) < npc.viewrange)
                    {
                    }
                    else if (npc.Attacking && npc.HasWeapon("Sword") &&
                             math::closeish(npc.xpos, npc.ypos, npc.TargetPos.x,
                                            npc.TargetPos.y) < 10)
                    {
                    }
                    else
                    {
                        /*if(gridposTrace(npc.xpos,npc.ypos,npc.TargetPos.x,npc.TargetPos.y,npc.id,npc.TargetPos) != false)
                        {
                            //npc.DirMove(npc.TargetPos);
                        }*/
                        npc.DirMove(npc.TargetPos);
                    }
                }
                if (npc.NeedsPath == true &&
                    math::Vec2f(npc.xpos, npc.ypos) != npc.TargetPos)
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

                        npc.PathFinding.MyFindPath(npc.xpos, npc.ypos,
                                                   npc.TargetPos.x,
                                                   npc.TargetPos.y);

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
                            if (Tiles[i][t][30].Walkable == false)
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

                    npc.PathFinding.MyReadPath(1, npc.xpos, npc.ypos,
                                               npc.moverate);
                    npc.DirMove(math::Vec2f(npc.PathFinding.MyxPath,
                                            npc.PathFinding.MyyPath));

                    if (PathFindWorkPos.x != 0)
                    {
                        astar::walkability[abs_to_index(
                            PathFindWorkPos.x /
                            20)][abs_to_index(PathFindWorkPos.y / 20)] =
                            Previous;
                    }

                    /*if (npc.xpos > npc.PathFinding.MyxPath) {npc.xpos = npc.xpos - npc.moverate;}
    if (npc.xpos < npc.PathFinding.MyxPath) {npc.xpos = npc.xpos + npc.moverate;}
    if (npc.ypos > npc.PathFinding.MyyPath) {npc.ypos = npc.ypos - npc.moverate;}
    if (npc.ypos < npc.PathFinding.MyyPath) {npc.ypos = npc.ypos + npc.moverate;}*/

                    //npc.PathFinding.MyFindPath(npc.xpos,npc.ypos,npc.TargetPos.x,npc.TargetPos.y);

                    if (npc.id == gvars::MyTargetid)
                    {
                        for (int Stuff = npc.PathFinding.MypathLocation;
                             Stuff != npc.PathFinding.MypathLength; ++Stuff)
                        {
                            if (Stuff != 1)
                            {
                                Effectz.CreateLine(
                                    npc.PathFinding.MyReadPathX(1, Stuff - 1),
                                    npc.PathFinding.MyReadPathY(1, Stuff - 1),
                                    npc.PathFinding.MyReadPathX(1, Stuff),
                                    npc.PathFinding.MyReadPathY(1, Stuff), 2,
                                    sf::Color::Blue);
                            }
                            //std::cout << npc.PathFinding.MyReadPathX(1,Stuff) << ":" << npc.PathFinding.MyReadPathY(1,Stuff) << std::endl;
                        }
                    }

                    if (npc.PathFinding.MypathLocation ==
                        npc.PathFinding.MypathLength)
                    {
                        if (abs(npc.xpos - npc.PathFinding.MyxPath) <
                            npc.moverate)
                            npc.xpos = npc.PathFinding.MyxPath;
                        if (abs(npc.ypos - npc.PathFinding.MyyPath) <
                            npc.moverate)
                            npc.ypos = npc.PathFinding.MyyPath;
                        npc.action = "Act";
                        npc.NeedsPath = false;
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
                    elem.ToDelete = true;

                    std::string ChtStr;
                    ChtStr.append("* ");
                    ChtStr.append(npc.name);
                    ChtStr.append("(" + std::to_string(npc.id) + ")");
                    ChtStr.append(" has consumed ");
                    ChtStr.append(elem.name);
                    ChtStr.append(".");

                    ChatBox.AddChat(ChtStr, sf::Color(150, 150, 0));
                }
                if (elem.type == 5 &&
                    npc.thirst + elem.thirstvalue <= npc.maxthirst &&
                    true == false)
                {
                    //if(Item->hungervalue > 0){ npc.fillhunger(1); Item->hungervalue--;}
                    npc.fillhunger(elem.hungervalue);
                    npc.fillthirst(elem.thirstvalue);
                    elem.ToDelete = true;

                    std::string ChtStr;
                    ChtStr.append("* ");
                    ChtStr.append(npc.name);
                    ChtStr.append("(" + std::to_string(npc.id) + ")");
                    ChtStr.append(" has consumed ");
                    ChtStr.append(elem.name);
                    ChtStr.append(".");

                    ChatBox.AddChat(ChtStr, sf::Color(150, 150, 0));
                }
            }
            if (gvars::Debug)
                std::cout << "Post Item Usages. \n";

            UnpointItems(npc.inventory);

            bool Done = false;

            while (Done == false)
            {
                bool Yet = false;
                for (auto it = npc.inventory.begin(); it != npc.inventory.end();
                     ++it)
                {
                    if (it->ToDelete)
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
            if (gvars::Debug)
                std::cout << "Post Inventory Cleanup. \n";

        } // End of CanMove
        //npc.angle = Math::AngleBetweenVectors(Math::Vec(TempXpos,TempYpos),Math::Vec(npc.xpos,npc.ypos))-180;
        npc.angle = math::angleBetweenVectors(npc.TargetPos,
                                              math::Vec2f(npc.xpos, npc.ypos));
        npc.MomMove();
        //float f=Math::AngleBetweenVectors(sf::Vector2f(npc.xpos,npc.ypos), npc.TargetPos);npc.img.setRotation(f);
        if (gvars::Debug)
            std::cout << npc.name << npc.id << " is done. \n";

        IntegerIterator++;
    }
    if (gvars::Debug)
        std::cout << "Post NPC\n";
}

void DrawTiles()
{
    int z = gvars::currentz;
    int iTS = GridSize;
    for (int i = 0; i <= gridy - 1; i++)
    {
        for (int t = 0; t <= gridx - 1; t++)
        {
            if ((gvars::Following == true &&
                 i > (npclist.at(gvars::MyTarget).xpos / GridSize) - 27 &&
                 i < (npclist.at(gvars::MyTarget).xpos / GridSize) + 26) ||
                (i > gvars::currentx - 27 && i < gvars::currentx + 26))
            {
                if ((gvars::Following == true &&
                     t > (npclist.at(gvars::MyTarget).ypos / GridSize) - 21 &&
                     t < (npclist.at(gvars::MyTarget).ypos / GridSize) + 20) ||
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

void DrawNPCs()
{
    for (auto &npc : npclist)
    {
        if (npc.HasSpawned == true)
        {
            if (npc.name == "Azabul")
            {
                sf::Vector2f TA1 = npc.TentArm1;
                sf::Vector2f TA2 = npc.TentArm2;
                sf::Vector2f TE1 = npc.TentEnd1;
                sf::Vector2f TE2 = npc.TentEnd2;

                Effectz.CreateLine(npc.xpos, npc.ypos, TA1.x + npc.xpos,
                                   TA1.y + npc.ypos, 2, sf::Color(0, 200, 200),
                                   1, sf::Color(0, 255, 255));
                Effectz.CreateLine(npc.xpos, npc.ypos, TA2.x + npc.xpos,
                                   TA2.y + npc.ypos, 2, sf::Color(0, 200, 200),
                                   1, sf::Color(0, 255, 255));
                Effectz.CreateLine(
                    TA1.x + npc.xpos, TA1.y + npc.ypos,
                    TE1.x + TA1.x + npc.xpos, TE1.y + TA1.y + npc.ypos, 2,
                    sf::Color(0, 200, 200), 1, sf::Color(0, 255, 255));
                Effectz.CreateLine(
                    TA2.x + npc.xpos, TA2.y + npc.ypos,
                    TE2.x + TA2.x + npc.xpos, TE2.y + TA2.y + npc.ypos, 2,
                    sf::Color(0, 200, 200), 1, sf::Color(0, 255, 255));
                Effectz.DrawEffects();
            }

            int Alph = 255;
            npc.img.setColor(sf::Color(255, 255, 255, Alph));
            npc.img.setScale(gvars::Scalex, gvars::Scaley);
            npc.img.setOrigin(npc.img.getTextureRect().width / 2,
                              npc.img.getTextureRect().height / 2);
            npc.DrawImg();
            Effectz.CreateCircle(npc.xpos, npc.ypos, npc.size,
                                 sf::Color(50, 50, 50, 50));
        }
    }
    debug("Done drawing NPCs");
}

void DrawItems()
{
    //App.setActive(true);

    for (auto &worlditem : worlditems)
    {
        //if(zit->xpos/GridSize > globals::currentx-27 && zit->xpos/GridSize < globals::currentx+26 && zit->ypos/GridSize > globals::currenty-20 && zit->ypos/GridSize < globals::currenty+20)
        //{
        worlditem.img.setColor(sf::Color(255, 255, 255, 255));
        worlditem.img.setScale(gvars::Scalex, gvars::Scaley);
        worlditem.DrawImg();
        //}
    }
    debug("Done Drawing Items");
}

void LightTrail(int x, int y, int z)
{
    int curx = math::clamp(x, 0, gridx - 1);
    int cury = math::clamp(y, 0, gridy - 1);
    int curz = math::clamp(z, 0, gridz - 1);
    gvars::sunmap[curz][curx][cury] = 255;
}

void DisplayChat(sf::Vector2f Position)
{
    if (GC.Phase != "MainMenu")
        Effectz.CreateSquare(
            Position.x - 10, Position.y + 10, Position.x + 500,
            (Position.y - ((ChatBox.ChatStorage.size() + 1) * 10)),
            sf::Color(0, 0, 0, 100), 2, sf::Color::Cyan);

    for (size_t i = 0; i != ChatBox.ChatStorage.size(); i++)
    {
        cText.CreateText(
            Position.x,
            (Position.y - ((ChatBox.ChatStorage.size()) * 10)) + (i * 10), 11,
            ChatBox.ChatStorage[i].Color, ChatBox.ChatStorage[i].Line);
    }
}

void DrawStuffs()
{

    //sf::Context context;
    //App.setActive(true);
    DrawNewTiles();

    //DrawPlanets();
    DrawItems();

    DrawNPCs();

    DrawJobList(window.getView().getCenter().x - 500,
                window.getView().getCenter().y);
    debug("Drew Joblist");

    DisplayChat(sf::Vector2f(gvars::BottomLeft.x + 5, gvars::BottomLeft.y - 5));
    debug("Drew Chat");

    Effectz.DrawEffects();
    debug("Drew Effects");
    //    DrawPlanets();//Remove this one, Reenable previous

    for (auto &button : vSquareButtonList)
    {
        button.DrawButton();
    }
    vSquareButtonList.clear();
    debug("Drew and Cleared square buttons");

    cText.DrawTextz();
    debug("Drew Text");

    for (auto &button : vButtonList)
    {
        button.DrawButton();
    }
    vButtonList.clear();
    debug("Drew and Cleared buttons");

    gvars::DrawStuffsDone = true;
}

cItem *GetGlobalItem(std::string strtype)
{
    if (gvars::Debug)
    {
        std::cout << "Getting" << strtype << " \n";
    }
    for (auto &elem : itemmanager.GlobalItem)
    {
        if (elem.name == strtype)
        {
            if (gvars::Debug)
            {
                std::cout << "Found" << strtype << " \n";
            }
            return &elem;
        }
    }
    if (gvars::Debug)
    {
        std::cout << "Didn't Find" << strtype << " \n";
    }

    return nullptr;
}

NPC *GetCritter(int id)
{
    if (gvars::Debug)
    {
        std::cout << "Getting critter(" << id << ") \n";
    }
    for (auto &elem : npclist)
    {
        if (elem.id == id)
        {
            if (gvars::Debug)
            {
                std::cout << "Found critter(" << id << ") \n";
            }
            return &elem;
        }
    }
    if (gvars::Debug)
    {
        std::cout << "Didn't Find critter(" << id << ") \n";
    }
    return nullptr;
}

void RemoveNPCs()
{
    bool Done = false;
    while (Done == false)
    {
        bool Yet = false;
        for (auto it = npclist.begin(); it != npclist.end(); ++it)
        {
            if (it->ToDelete)
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

    cText.LoadFont();

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

    gvars::View1.zoom(2);
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
    itemmanager.InitializeItems();
    npcmanager.InitializeCritters();

    // Building the players faction, This is temporary.
    PF.Name = "The Alphas";
    PF.PlayerControlled = true;
    PF.Initialized = true;
    UniFact.push_back(PF);
    ConFact = &UniFact[0];

    // Setting the initial game phase.
    GC.Phase = "MainMenu";

    // For A*
    astar::init();

    while (window.isOpen())
    {
        RemoveNPCs();
        if (Key.mTime > 5)
        {
            GC.Phase = "MainMenu";
        }
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
                    if (gvars::CameraZoom < 2)
                    {
                        gvars::CameraZoom = gvars::CameraZoom / 0.5;
                        //CameraSize
                        gvars::View1.zoom(0.5);
                    }
                }
                if (Event.mouseWheel.delta < 0)
                {
                    std::cout << "Zoom In \n";
                    if (gvars::CameraZoom > 0.5)
                    {
                        gvars::CameraZoom = gvars::CameraZoom / 2;
                        gvars::View1.zoom(2);
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
                gvars::InFocus = false;
            }
            if (Event.type == sf::Event::GainedFocus)
            {
                gvars::InFocus = true;
            }
        }
        window.setView(gvars::View1);
        gvars::ButtonClicked = false;
        gvars::ButtonClickedTime--; // Misleading Variable name, Sorry!
        if (gvars::ButtonClickedTime < 0)
            gvars::ButtonClickedTime = 0;

        Key.Update();
        sf::Vector2f MouseStagnationCheck = gvars::MousePos;
        // For some reason, I have to manually modify the positions.
        gvars::MousePos =
            window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (MouseStagnationCheck == gvars::MousePos)
            gvars::MouseStagnation++;
        else
            gvars::MouseStagnation = 0;

        gvars::TopLeft = sf::Vector2f(gvars::View1.getCenter().x - HalfSize.x,
                                      gvars::View1.getCenter().y - HalfSize.y);
        gvars::TopRight = sf::Vector2f(gvars::View1.getCenter().x + HalfSize.x,
                                       gvars::View1.getCenter().y - HalfSize.y);
        gvars::BottomLeft =
            sf::Vector2f(gvars::View1.getCenter().x - HalfSize.x,
                         gvars::View1.getCenter().y + HalfSize.y);
        gvars::BottomRight =
            sf::Vector2f(gvars::View1.getCenter().x + HalfSize.x,
                         gvars::View1.getCenter().y + HalfSize.y);

        cText.CreateText(CZ(gvars::TopRight.x - CZ(50)),
                         CZ(gvars::TopRight.y + CZ(50)), CZ(11),
                         sf::Color::White, "x", "", gvars::CameraZoom);
        cText.CreateText((gvars::TopRight.x - 50), (gvars::TopRight.y + 50),
                         (11) / gvars::CameraZoom, sf::Color::White, "x", "",
                         gvars::CameraZoom);

        if (Key.kTime == 1)
        { // Generates a random name from GenerateName(); and puts it into the console.
            std::cout << GenerateName() << std::endl;
        }

        if (Key.iTime == 10)
        { // Attempt to fix item images that were vanishing.
            Con("Doing");
            itemmanager.GlobalItem.clear();
            itemmanager.InitializeItems();

            RefreshImages();
        }

        if (Key.gTime == 1)
        { // Fling all critters south.
            for (size_t i = 0; i != npclist.size(); i++)
            {
                npclist[i].Momentum = sf::Vector2f(0, 100);
            }
        }
        if (Key.hTime == 1)
        { // Fling all critters north.
            for (size_t i = 0; i != npclist.size(); i++)
            {
                npclist[i].Momentum = sf::Vector2f(0, -100);
            }
        }

        // Game Mode Loops ================================================================================

        if (Key.lTime == 1)
        { // Print current clipboard contents to the console.
            GetClipboardText();
        }

        if (Key.r)
        { // Debug (de)activation
            if (!gvars::Debug)
            {
                gvars::Debug = true;
                fSleep(0.2);
            }
            else if (gvars::Debug)
            {
                gvars::Debug = false;
                fSleep(0.2);
            }
        }
        if (GC.Phase == "Local")
        { //=======================================================*Local*============================================================================
            if (gvars::Debug)
                cText.CreateText((gvars::currentx - 2) * GridSize,
                                 (gvars::currenty + 1) * GridSize, 11,
                                 sf::Color::Red, "Debug On");
            if (Key.lctrl && Key.LMB)
            {
                Boom(gvars::MousePos.x, gvars::MousePos.y, 10, 50);
            }

            for (auto &worlditem : worlditems)
            {
                if (math::closeish(gvars::MousePos.x, gvars::MousePos.y,
                                   (worlditem).xpos, (worlditem).ypos) <= 10)
                {
                    std::cout << "Found; " << (worlditem).name << std::endl;
                    //fSleep(2);
                }
            }

            if (Key.vTime == 1)
                ChatBox.AddChat(
                    randomWindowName(),
                    sf::Color(randz(0, 255), randz(0, 255), randz(0, 255)));

            SquadHud();

            if (Key.lshiftTime > 0)
            {
                //int x = globals::TopLeft.x;
                //int y = globals::TopLeft.y+Rez.y;
                int x = gvars::BottomLeft.x;
                int y = gvars::BottomLeft.y;
                Effectz.CreateSquare(x + 20, y - 20, x + 40, y - 40,
                                     sf::Color(0, 100, 255));
                cText.CreateText(x + 20, y - 30, 11, sf::Color::White, "Build");
            }
            if (Key.RMBTime == 1 && Key.lshift)
                RMBMenuTile(gvars::MousePos);

            if (Key.l == true)
            {
                for (auto &elem : npclist)
                {
                    elem.PrintBloodContent();
                }
            } // Zombification

            if (Key.lshift && Key.tab) // Debug NPC Spawn Menu
            {

                GC.MenuType = "DebugCritterSpawn";
                MenuPopUp();
            }
            else if (Key.tab == true &&
                     Key.lshift ==
                         false) // Debug Item Spawn Menu TODO: Sort them by Type.
            {
                GC.MenuType = "DebugItemSpawn";
                MenuPopUp();
            }
            else if (GC.MenuType == "DebugItemSpawn" ||
                     GC.MenuType == "DebugCritterSpawn")
            {
                GC.MenuType = "NULL";
                GC.MenuPos = sf::Vector2f(-10000, -10000);
                GC.MenuEndPos = sf::Vector2f(-10000, -10000);
                MenuPtrCon.pItem = nullptr;
                MenuPtrCon.pNPC = nullptr;
                MenuPtrCon.pTile = nullptr;
                Con("Closing DebugMenus Due To Lack of Key Presses");
            }
            //                       if(Key.lshift && Key.z){GC.ZombieSwarmLocal();}

            if (Key.lshift && Key.z) // Debug Stuffs Menu
            {
                GC.MenuType = "DebugFunctions";
                MenuPopUp();
            }

            if (Key.left == true)
            {
                gvars::currentx--;
                PlyAct = true;
            }
            if (Key.right == true)
            {
                gvars::currentx++;
                PlyAct = true;
            }
            if (Key.up == true)
            {
                gvars::currenty--;
                PlyAct = true;
            }
            if (Key.down == true)
            {
                gvars::currenty++;
                PlyAct = true;
            }

            if (gvars::InitalZeds)
                GC.Wave();

            RightMouseButtonContextMenu();

            if (Key.bTime == 1)
            {
                NPC Critter;
                Critter = *GetGlobalCritter("Azabul");
                Critter.xpos = gvars::MousePos.x;
                Critter.ypos = gvars::MousePos.y;

                Critter.Body.BodyParts =
                    "{[Name:UpperTorso][BloodPumpRate:100][AirCapacity:200]["
                    "AirAbsorbtion:100][ObjectCapacity:1][NutritionExtraction:"
                    "25][MassFlesh:15:1000]}";
                Critter.Body.BodyParts.append("\n{[Name:Head][Mind:true]["
                                              "Orafice:1][MassFlesh:5:1000]["
                                              "Dependant:UpperTorso]}");
                Critter.Body.BodyParts.append(
                    "\n{[Name:LowerTorso][ObjectCapacity:10][DigestionRate:125]"
                    "[NutritionExtraction:50][PoisonFilter:Zombification:10]["
                    "DigestsBlood:100][MassFlesh:15:1000][Dependant:UpperTorso]"
                    "}");

                Critter.Body.BodyParts.append("\n{[Name:Left "
                                              "Leg][Walk:8][MassFlesh:15:1000]["
                                              "Dependant:LowerTorso]}");
                Critter.Body.BodyParts.append("\n{[Name:Right "
                                              "Leg][Walk:8][MassFlesh:15:1000]["
                                              "Dependant:LowerTorso]}");

                Critter.Body.BodyParts.append("\n{[Name:Left "
                                              "Arm][Grasp:2][MassFlesh:10:1000]"
                                              "[Dependant:UpperTorso]}");
                Critter.Body.BodyParts.append("\n{[Name:Right "
                                              "Arm][Grasp:2][MassFlesh:10:1000]"
                                              "[Dependant:UpperTorso]}");

                npclist.push_back(Critter);
            }

            for (size_t i = 0; i != npclist.size(); i++)
            {
                if (npclist[i].name == "Azabul")
                {
                    cText.CreateText(
                        (npclist[i].xpos) -
                            ((npclist[i].Body.BodyParts.size() / 2) * 2),
                        npclist[i].ypos + 10, 11, sf::Color::White,
                        npclist[i].Body.BodyParts);
                }
            }

            if (Key.lshift == true && Key.left == true)
            {
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                PlyAct = true;
            } //Sprite.Move(-100 * ElapsedTime, 0);
            if (Key.lshift == true && Key.right == true)
            {
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                PlyAct = true;
            } //Sprite.Move( 100 * ElapsedTime, 0);
            if (Key.lshift == true && Key.up == true)
            {
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                PlyAct = true;
            } //Sprite.Move(0, -100 * ElapsedTime);
            if (Key.lshift == true && Key.down == true)
            {
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                PlyAct = true;
            } //Sprite.Move(0,  100 * ElapsedTime);
            if (Key.comma == true && Key.lshift == true &&
                gvars::currentz <= gridz - 1)
            {
                gvars::currentz++;
                PlyAct = true;
                fSleep(0.1f);
            } //Sprite.Move(0, -100 * ElapsedTime);
            if (Key.period == true && Key.lshift == true &&
                gvars::currentz >= 1)
            {
                gvars::currentz--;
                PlyAct = true;
                fSleep(0.1f);
            } //Sprite.Move(0,  100 * ElapsedTime);
            if (Key.comma == true && Key.rshift == true &&
                gvars::currentz <= gridz - 1)
            {
                gvars::currentz++;
                PlyAct = true;
                fSleep(0.1f);
            } //Sprite.Move(0, -100 * ElapsedTime);
            if (Key.period == true && Key.rshift == true &&
                gvars::currentz >= 1)
            {
                gvars::currentz--;
                PlyAct = true;
                fSleep(0.1f);
            } //Sprite.Move(0,  100 * ElapsedTime);
            if (gvars::MyTarget == -1)
            {
                gvars::Following = false;
            }
            else if (Key.c)
            {
                gvars::Following = true;
            }
            if (gvars::Following)
            {
                gvars::View1.setCenter(npclist.at(gvars::MyTarget).xpos,
                                       npclist.at(gvars::MyTarget).ypos);
            }

            if (Key.n)
                GenerateChunk("Road", 500, sf::Vector2i(50, 50));
            if (Key.m)
                GenerateChunk("SouthernHouse", 500, sf::Vector2i(50, 50));

            if (Key.qTime > 10 && gvars::MyTarget == -1 && !Key.lshift)
            {
            }

            if (Key.c)
            {
                for (int x = 0; x != ChunkSize; x++)
                {
                    for (int y = 0; y != ChunkSize; y++)
                    {
                        Tiles[x][y][30] = vChunk[x][y][0];
                    }
                }
                //GenerateWorld();
            }

            Effectz.CreateSquare(
                32 * 20, 32 * 20, 64 * 20, 64 * 20, sf::Color(0, 0, 0, 0), 1,
                sf::Color::
                    Cyan); // These two are the chunk borders, Debug stuffs so you can see where the loading boundries are. (As if you needed 'em, Damn thing lags a bit, Should thread each loading bit soon.)
            Effectz.CreateSquare(
                0 * 20, 0 * 20, 32 * 20, 32 * 20, sf::Color(0, 0, 0, 0), 1,
                sf::Color::
                    Red); // This one reachs from 0 to 32 only because it draws from the left, not the center.

            /*

            int Degrees = GX;
            int Dist = 200;
            if(Key.w) Degrees = 0;
            if(Key.a) Degrees = 270;
            if(Key.s) Degrees = 180;
            if(Key.d) Degrees = 90;
            float Xxx = MousePos.x + cosf(Degrees * PI/180) * Dist;
            float Yyy = MousePos.y + sinf(Degrees * PI/180) * Dist;
            Effectz.CreateCircle(MousePos.x,MousePos.y,5,Red);
            Effectz.CreateCircle(Xxx,Yyy,5,White);

            */

            int Length = 200;

            for (int Rot = 0; Rot != 360; Rot++)
            {
                /*int Rot = GX;
                int XPos = ((abs(MousePos.x / 20) * 20) + 10 +
                            cosf(Rot * PI / 180) * Length) /
                           20;
                int YPos = ((abs(MousePos.y / 20) * 20) + 10 +
                            sinf(Rot * PI / 180) * Length) /
                           20;
                XPos *= 20;
                YPos *= 20;

                //Effectz.CreateCircle(MousePos.x,MousePos.y,5,Red);
                //Effectz.CreateCircle(XPos,YPos,5,White);*/
            }
            if (Key.p && true == false)
                for (int Rot = 0; Rot != 360; Rot++)
                {
                    //int Rot = GX;
                    int XPos = ((abs(gvars::MousePos.x / 20) * 20) + 10 +
                                cosf(Rot * PI / 180) * Length) /
                               20;
                    int YPos = ((abs(gvars::MousePos.y / 20) * 20) + 10 +
                                sinf(Rot * PI / 180) * Length) /
                               20;
                    //XPos *= 20;
                    //YPos *= 20;

                    //Effectz.CreateCircle(MousePos.x,MousePos.y,5,Red);
                    //Effectz.CreateCircle(XPos,YPos,5,White);
                    Tiles[XPos][YPos][30].Stone();
                }

            if (Key.pTime == 1)
            {

                for (int ItLength = 0; ItLength != 16; ItLength++)
                {

                    if (ItLength != 15)
                    {

                        for (int Rot = 1; Rot != 361; Rot++)
                        {

                            int XPos = abs(gvars::MousePos.x / 20) +
                                       sin(Rot * PI / 180) * ItLength;
                            int YPos = abs(gvars::MousePos.y / 20) +
                                       cos(Rot * PI / 180) * ItLength;

                            Tiles[XPos][YPos][30].Stone();
                        }
                    }
                    else
                    {
                        for (int Rot = 1; Rot != 361; Rot++)
                        {

                            int XPos = abs(gvars::MousePos.x / 20) +
                                       sin(Rot * PI / 180) * ItLength;
                            int YPos = abs(gvars::MousePos.y / 20) +
                                       cos(Rot * PI / 180) * ItLength;

                            Tiles[XPos][YPos][30].Wall();
                        }
                    }
                }
            }

            //float XX = cos(GX * PI) / 180;
            //float YY = sin(GY * PI) / 180;
            //Effectz.CreateCircle(XX,YY,11,sf::Color::White);

            if (Key.lctrlTime > 10)
            {
                int Variable =
                    Tiles[abs_to_index(gvars::MousePos.x / 20)][abs_to_index(
                        gvars::MousePos.y / 20)][30].ID;
                cText.CreateText(gvars::MousePos.x, gvars::MousePos.y, 11,
                                 sf::Color::Red, "", "", Variable);
            }

            bool Transitioning = false;
            if (gvars::currenty > 64)
            {
                TilesGoUp();
                gvars::currenty = 33;

                for (size_t i = 0; i != npclist.size(); i++)
                {
                    npclist.at(i).ypos += -640;
                    npclist.at(i).TargetPos.y += -640;
                }

                //for(int i = 0; i != worlditems.size(); i++) worlditems.at(i).ypos += -640;
                for (auto &worlditem : worlditems)
                    (worlditem).ypos += -640;

                for (size_t i = 0; i != UniFact[0].JobList.size(); i++)
                    UniFact[0].JobList[i].WorkPos.y += -640;

                Transitioning = true;
                std::string Line;
                Line = LoadCritters(sf::Vector2i(gvars::currentregionx - 1,
                                                 gvars::currentregiony + 1),
                                    "BottomLeft", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadCritters(sf::Vector2i(gvars::currentregionx,
                                                 gvars::currentregiony + 1),
                                    "Bottom", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadCritters(sf::Vector2i(gvars::currentregionx + 1,
                                                 gvars::currentregiony + 1),
                                    "BottomRight", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadItems(sf::Vector2i(gvars::currentregionx - 1,
                                              gvars::currentregiony + 1),
                                 "BottomLeft", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadItems(sf::Vector2i(gvars::currentregionx,
                                              gvars::currentregiony + 1),
                                 "Bottom", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadItems(sf::Vector2i(gvars::currentregionx + 1,
                                              gvars::currentregiony + 1),
                                 "BottomRight", gvars::currentplanet);
                remove(Line.c_str());
            }
            if (gvars::currenty < 32)
            {
                TilesGoDown();
                gvars::currenty = 63;
                for (size_t i = 0; i != npclist.size(); i++)
                {
                    npclist.at(i).ypos += 640;
                    npclist.at(i).TargetPos.y += 640;
                }

                //for(int i = 0; i != worlditems.size(); i++) worlditems.at(i).ypos += 640;
                for (auto &worlditem : worlditems)
                    (worlditem).ypos += 640;

                for (size_t i = 0; i != UniFact[0].JobList.size(); i++)
                    UniFact[0].JobList[i].WorkPos.y += 640;

                Transitioning = true;
                std::string Line;
                Line = LoadCritters(sf::Vector2i(gvars::currentregionx - 1,
                                                 gvars::currentregiony - 1),
                                    "TopLeft", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadCritters(sf::Vector2i(gvars::currentregionx,
                                                 gvars::currentregiony - 1),
                                    "Top", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadCritters(sf::Vector2i(gvars::currentregionx + 1,
                                                 gvars::currentregiony - 1),
                                    "TopRight", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadItems(sf::Vector2i(gvars::currentregionx - 1,
                                              gvars::currentregiony - 1),
                                 "TopLeft", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadItems(sf::Vector2i(gvars::currentregionx,
                                              gvars::currentregiony - 1),
                                 "Top", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadItems(sf::Vector2i(gvars::currentregionx + 1,
                                              gvars::currentregiony - 1),
                                 "TopRight", gvars::currentplanet);
                remove(Line.c_str());
            }
            if (gvars::currentx > 64)
            {
                TilesGoLeft();
                gvars::currentx = 33;
                for (size_t i = 0; i != npclist.size(); i++)
                {
                    npclist.at(i).xpos += -640;
                    npclist.at(i).TargetPos.x += -640;
                }

                //for(int i = 0; i != worlditems.size(); i++) worlditems.at(i).xpos += -640;
                for (auto &worlditem : worlditems)
                    (worlditem).xpos += -640;

                for (size_t i = 0; i != UniFact[0].JobList.size(); i++)
                    UniFact[0].JobList[i].WorkPos.x += -640;

                Transitioning = true;
                std::string Line;
                Line = LoadCritters(sf::Vector2i(gvars::currentregionx + 1,
                                                 gvars::currentregiony - 1),
                                    "TopRight", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadCritters(sf::Vector2i(gvars::currentregionx + 1,
                                                 gvars::currentregiony),
                                    "Right", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadCritters(sf::Vector2i(gvars::currentregionx + 1,
                                                 gvars::currentregiony + 1),
                                    "BottomRight", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadItems(sf::Vector2i(gvars::currentregionx + 1,
                                              gvars::currentregiony - 1),
                                 "TopRight", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadItems(sf::Vector2i(gvars::currentregionx + 1,
                                              gvars::currentregiony),
                                 "Right", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadItems(sf::Vector2i(gvars::currentregionx + 1,
                                              gvars::currentregiony + 1),
                                 "BottomRight", gvars::currentplanet);
                remove(Line.c_str());
            }
            if (gvars::currentx < 32)
            {
                Con("Starting GoRight");
                TilesGoRight();
                Con("Ending GoRight");
                gvars::currentx = 63;
                Con("Starting GoRight with NPC's and Items");
                for (size_t i = 0; i != npclist.size(); i++)
                {
                    npclist.at(i).xpos += 640;
                    npclist.at(i).TargetPos.x += 640;
                }

                //for(int i = 0; i != worlditems.size(); i++) worlditems.at(i).xpos += 640;
                for (auto &worlditem : worlditems)
                    (worlditem).xpos += 640;

                for (size_t i = 0; i != UniFact[0].JobList.size(); i++)
                    UniFact[0].JobList[i].WorkPos.x += 640;

                Con("Done GoRight with NPC's and Items");
                Transitioning = true;
                std::string Line;
                Line = LoadCritters(sf::Vector2i(gvars::currentregionx - 1,
                                                 gvars::currentregiony - 1),
                                    "TopLeft", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadCritters(sf::Vector2i(gvars::currentregionx - 1,
                                                 gvars::currentregiony),
                                    "Left", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadCritters(sf::Vector2i(gvars::currentregionx - 1,
                                                 gvars::currentregiony + 1),
                                    "BottomLeft", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadItems(sf::Vector2i(gvars::currentregionx - 1,
                                              gvars::currentregiony - 1),
                                 "TopLeft", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadItems(sf::Vector2i(gvars::currentregionx - 1,
                                              gvars::currentregiony),
                                 "Left", gvars::currentplanet);
                remove(Line.c_str());
                Line = LoadItems(sf::Vector2i(gvars::currentregionx - 1,
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
                        SaveNPC(500, sf::Vector2i(gvars::currentregionx + 2,
                                                  gvars::currentregiony - 1),
                                npclist.at(i));
                        npclist.at(i).ToDelete = true;
                    }
                    else if (npclist.at(i).xpos > 1920 &&
                             npclist.at(i).ypos > 1280)
                    {
                        npclist.at(i).xpos =
                            npclist.at(i).xpos - 640 - 640 - 640;
                        npclist.at(i).ypos = npclist.at(i).ypos - 640 - 640;
                        SaveNPC(500, sf::Vector2i(gvars::currentregionx + 2,
                                                  gvars::currentregiony + 1),
                                npclist.at(i));
                        npclist.at(i).ToDelete = true;
                    }

                    else if (npclist.at(i).xpos < 0 &&
                             npclist.at(i).ypos > 1280)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos + 640;
                        npclist.at(i).ypos = npclist.at(i).ypos - 640 - 640;
                        SaveNPC(500, sf::Vector2i(gvars::currentregionx - 2,
                                                  gvars::currentregiony + 1),
                                npclist.at(i));
                        npclist.at(i).ToDelete = true;
                    }
                    else if (npclist.at(i).xpos < 0 && npclist.at(i).ypos < 640)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos + 640;
                        SaveNPC(500, sf::Vector2i(gvars::currentregionx - 2,
                                                  gvars::currentregiony - 1),
                                npclist.at(i));
                        npclist.at(i).ToDelete = true;
                    }

                    else if (npclist.at(i).ypos < 0 &&
                             npclist.at(i).xpos > 1280)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos - 640 - 640;
                        npclist.at(i).ypos = npclist.at(i).ypos + 640;
                        SaveNPC(500, sf::Vector2i(gvars::currentregionx + 1,
                                                  gvars::currentregiony - 2),
                                npclist.at(i));
                        npclist.at(i).ToDelete = true;
                    }
                    else if (npclist.at(i).ypos < 0 && npclist.at(i).xpos < 640)
                    {
                        npclist.at(i).ypos = npclist.at(i).ypos + 640;
                        SaveNPC(500, sf::Vector2i(gvars::currentregionx - 1,
                                                  gvars::currentregiony - 2),
                                npclist.at(i));
                        npclist.at(i).ToDelete = true;
                    }

                    else if (npclist.at(i).ypos > 1920 &&
                             npclist.at(i).xpos > 1280)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos - 640 - 640;
                        npclist.at(i).ypos =
                            npclist.at(i).ypos - 640 - 640 - 640;
                        SaveNPC(500, sf::Vector2i(gvars::currentregionx + 1,
                                                  gvars::currentregiony + 2),
                                npclist.at(i));
                        npclist.at(i).ToDelete = true;
                    }
                    else if (npclist.at(i).ypos > 1920 &&
                             npclist.at(i).xpos < 640)
                    {
                        npclist.at(i).ypos =
                            npclist.at(i).ypos - 640 - 640 - 640;
                        SaveNPC(500, sf::Vector2i(gvars::currentregionx - 1,
                                                  gvars::currentregiony + 2),
                                npclist.at(i));
                        npclist.at(i).ToDelete = true;
                    }

                    //HAGGINABAGGINA  Some reason, When a critter is saved, It'll have more than 640 for it's position, This is unacceptable.

                    else if (npclist.at(i).xpos > 1920)
                    {
                        npclist.at(i).xpos =
                            npclist.at(i).xpos - 640 - 640 - 640;
                        npclist.at(i).ypos = npclist.at(i).ypos - 640;
                        SaveNPC(500, sf::Vector2i(gvars::currentregionx + 2,
                                                  gvars::currentregiony),
                                npclist.at(i));
                        npclist.at(i).ToDelete = true;
                    }
                    else if (npclist.at(i).ypos > 1920)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos - 640;
                        npclist.at(i).ypos =
                            npclist.at(i).ypos - 640 - 640 - 640;
                        SaveNPC(500, sf::Vector2i(gvars::currentregionx,
                                                  gvars::currentregiony + 2),
                                npclist.at(i));
                        npclist.at(i).ToDelete = true;
                    }
                    else if (npclist.at(i).xpos < 0)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos + 640;
                        npclist.at(i).ypos = npclist.at(i).ypos - 640;
                        SaveNPC(500, sf::Vector2i(gvars::currentregionx - 2,
                                                  gvars::currentregiony),
                                npclist.at(i));
                        npclist.at(i).ToDelete = true;
                    }
                    else if (npclist.at(i).ypos < 0)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos - 640;
                        npclist.at(i).ypos = npclist.at(i).ypos + 640;
                        SaveNPC(500, sf::Vector2i(gvars::currentregionx,
                                                  gvars::currentregiony - 2),
                                npclist.at(i));

                        npclist.at(i).ToDelete = true;
                    }
                }

                RemoveNPCs();
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
                        SaveItem(500, sf::Vector2i(gvars::currentregionx + 2,
                                                   gvars::currentregiony - 1),
                                 (worlditem));
                        (worlditem).ToDelete = true;
                    }
                    else if ((worlditem).xpos > 1920 && (worlditem).ypos > 1280)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640 - 640 - 640;
                        (worlditem).ypos = (worlditem).ypos - 640 - 640;
                        SaveItem(500, sf::Vector2i(gvars::currentregionx + 2,
                                                   gvars::currentregiony + 1),
                                 (worlditem));
                        (worlditem).ToDelete = true;
                    }

                    else if ((worlditem).xpos < 0 && (worlditem).ypos > 1280)
                    {
                        (worlditem).xpos = (worlditem).xpos + 640;
                        (worlditem).ypos = (worlditem).ypos - 640 - 640;
                        SaveItem(500, sf::Vector2i(gvars::currentregionx - 2,
                                                   gvars::currentregiony + 1),
                                 (worlditem));
                        (worlditem).ToDelete = true;
                    }
                    else if ((worlditem).xpos < 0 && (worlditem).ypos < 640)
                    {
                        (worlditem).xpos = (worlditem).xpos + 640;
                        SaveItem(500, sf::Vector2i(gvars::currentregionx - 2,
                                                   gvars::currentregiony - 1),
                                 (worlditem));
                        (worlditem).ToDelete = true;
                    }

                    else if ((worlditem).ypos < 0 && (worlditem).xpos > 1280)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640 - 640;
                        (worlditem).ypos = (worlditem).ypos + 640;
                        SaveItem(500, sf::Vector2i(gvars::currentregionx + 1,
                                                   gvars::currentregiony - 2),
                                 (worlditem));
                        (worlditem).ToDelete = true;
                    }
                    else if ((worlditem).ypos < 0 && (worlditem).xpos < 640)
                    {
                        (worlditem).ypos = (worlditem).ypos + 640;
                        SaveItem(500, sf::Vector2i(gvars::currentregionx - 1,
                                                   gvars::currentregiony - 2),
                                 (worlditem));
                        (worlditem).ToDelete = true;
                    }

                    else if ((worlditem).ypos > 1920 && (worlditem).xpos > 1280)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640 - 640;
                        (worlditem).ypos = (worlditem).ypos - 640 - 640 - 640;
                        SaveItem(500, sf::Vector2i(gvars::currentregionx + 1,
                                                   gvars::currentregiony + 2),
                                 (worlditem));
                        (worlditem).ToDelete = true;
                    }
                    else if ((worlditem).ypos > 1920 && (worlditem).xpos < 640)
                    {
                        (worlditem).ypos = (worlditem).ypos - 640 - 640 - 640;
                        SaveItem(500, sf::Vector2i(gvars::currentregionx - 1,
                                                   gvars::currentregiony + 2),
                                 (worlditem));
                        (worlditem).ToDelete = true;
                    }

                    //HAGGINABAGGINA  Some reason, When a critter is saved, It'll have more than 640 for it's position, This is unacceptable.

                    else if ((worlditem).xpos > 1920)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640 - 640 - 640;
                        (worlditem).ypos = (worlditem).ypos - 640;
                        SaveItem(500, sf::Vector2i(gvars::currentregionx + 2,
                                                   gvars::currentregiony),
                                 (worlditem));
                        (worlditem).ToDelete = true;
                    }
                    else if ((worlditem).ypos > 1920)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640;
                        (worlditem).ypos = (worlditem).ypos - 640 - 640 - 640;
                        SaveItem(500, sf::Vector2i(gvars::currentregionx,
                                                   gvars::currentregiony + 2),
                                 (worlditem));
                        (worlditem).ToDelete = true;
                    }
                    else if ((worlditem).xpos < 0)
                    {
                        (worlditem).xpos = (worlditem).xpos + 640;
                        (worlditem).ypos = (worlditem).ypos - 640;
                        SaveItem(500, sf::Vector2i(gvars::currentregionx - 2,
                                                   gvars::currentregiony),
                                 (worlditem));
                        (worlditem).ToDelete = true;
                    }
                    else if ((worlditem).ypos < 0)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640;
                        (worlditem).ypos = (worlditem).ypos + 640;
                        SaveItem(500, sf::Vector2i(gvars::currentregionx,
                                                   gvars::currentregiony - 2),
                                 (worlditem));

                        (worlditem).ToDelete = true;
                    }
                }
                UnpointItems(worlditems);
                RemoveItems(worlditems);
            }

            //if(Key.b) Effectz.CreateBeam(1000,1000,MousePos.x,MousePos.y,50,Red,5,Blue,false,0);

            button var;
            var.Color = sf::Color::Red;
            var.iSize = 5;
            var.vPos = sf::Vector2f(600, 600);
            var.sButtonText = "Howdy";
            vButtonList.push_back(var);
            if (ButtonClicked(var.id))
            {
                std::cout << "Twas' True \n";
            }

            if (gvars::MyTarget != -1 && npclist[gvars::MyTarget].health <= 0 &&
                Key.lshift && Key.q)
            {
                npclist[gvars::MyTarget].ToDelete = true;
                gvars::MyTarget = -1;
                gvars::MyTargetid = -1;
            }

            if (gvars::MyTarget != -1 && Key.RMB &&
                Tiles[abs_to_index(gvars::MousePos.x / GridSize)][abs_to_index(
                    gvars::MousePos.y / GridSize)][30].ID != 1010)
            { // Giving Orders
                npclist.at(gvars::MyTarget).TargetPos = gvars::MousePos;
                npclist.at(gvars::MyTarget).action = "Orders";
                if (math::closeish(npclist.at(gvars::MyTarget).xpos,
                                   npclist.at(gvars::MyTarget).ypos,
                                   gvars::MousePos.x, gvars::MousePos.y) <= 10)
                {
                    npclist.at(gvars::MyTarget).action = "Act";
                    npclist.at(gvars::MyTarget).NeedsPath = false;
                }

                for (auto const &item : worlditems)
                {
                    if (math::closeish(gvars::MousePos.x, gvars::MousePos.y,
                                       item.xpos, item.ypos) <= 10)
                    {
                        GC.MenuType = "CritterContext";
                    }
                }
            } //End of Giving Orders

            //for (int i = 0; i <= gridy-1; i++){for( int t = 0; t <= gridx-1; t++){int z = globals::currentz;if(globals::sunmap[z][i][t] != 0){globals::sunmap[z][i][t] -= 5;}}} // Darkness
            if (GC.MenuType != "NULL")
            {
                MenuPopUp();
            }
            else
            {
                GC.MenuPos = math::Vec2f(-10000, -10000);
            }
        } //=============================================================================*End of Local*========================================================================
        if (GC.Phase == "Solar")
        { //=======================================================*Solar*============================================================================
            if (Key.lshift)
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

            if (Key.left == true)
            {
                gvars::currentx--;
                PlyAct = true;
            }
            if (Key.right == true)
            {
                gvars::currentx++;
                PlyAct = true;
            }
            if (Key.up == true)
            {
                gvars::currenty--;
                PlyAct = true;
            }
            if (Key.down == true)
            {
                gvars::currenty++;
                PlyAct = true;
            }
            if (Key.lshift == true && Key.left == true)
            {
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                PlyAct = true;
            } //Sprite.Move(-100 * ElapsedTime, 0);
            if (Key.lshift == true && Key.right == true)
            {
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                PlyAct = true;
            } //Sprite.Move( 100 * ElapsedTime, 0);
            if (Key.lshift == true && Key.up == true)
            {
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                PlyAct = true;
            } //Sprite.Move(0, -100 * ElapsedTime);
            if (Key.lshift == true && Key.down == true)
            {
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                PlyAct = true;
            } //Sprite.Move(0,  100 * ElapsedTime);

        } //=============================================================================*End of Solar*========================================================================
        if (GC.Phase == "Test")
        {
            if (Key.up)
                testage++;
            if (Key.down)
                testage--;
            if (Key.right)
                testage2++;
            if (Key.left)
                testage2--;
            if (Key.pad2)
                radius++;
            if (Key.pad8)
                radius--;

            if (gvars::currenty >
                64) // TODO: Make the auto removing tiles use the current windows border to get it's range, Allowing proper resizing and stuffs. Edit: Herp, That's not what this is.
            {
                TilesGoUp();
                gvars::currenty = 33;
            }
            if (gvars::currenty < 32)
            {
                TilesGoDown();
                gvars::currenty = 63;
            }
            if (gvars::currentx > 64)
            {
                TilesGoLeft();
                gvars::currentx = 33;
            }
            if (gvars::currentx < 32)
            {
                TilesGoRight();
                gvars::currentx = 63;
            }

            if (Key.g)
                InitalizeWorldTiles();
            //DrawNewTiles();

            DrawWorldTiles();
            if (Key.j)
                TilesGoUp();
            if (Key.k)
                TilesRandom();

            if (Key.m)
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

            if (Key.left == true)
            {
                gvars::currentx--;
                PlyAct = true;
            }
            if (Key.right == true)
            {
                gvars::currentx++;
                PlyAct = true;
            }
            if (Key.up == true)
            {
                gvars::currenty--;
                PlyAct = true;
            }
            if (Key.down == true)
            {
                gvars::currenty++;
                PlyAct = true;
            }
            if (Key.lshift == true && Key.left == true)
            {
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                PlyAct = true;
            } //Sprite.Move(-100 * ElapsedTime, 0);
            if (Key.lshift == true && Key.right == true)
            {
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                PlyAct = true;
            } //Sprite.Move( 100 * ElapsedTime, 0);
            if (Key.lshift == true && Key.up == true)
            {
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                PlyAct = true;
            } //Sprite.Move(0, -100 * ElapsedTime);
            if (Key.lshift == true && Key.down == true)
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
            Effectz.CreateCircle(xanchor, yanchor, 5, sf::Color::Blue);
            Effectz.CreateCircle(Xxx, Yyy, 5, sf::Color::White);

            int distence = math::closeish(xanchor, yanchor, Xxx, Yyy);
            cText.CreateText(xanchor, yanchor, 11, sf::Color::White,
                             "Distence:", "", distence);
            cText.CreateText(xanchor, yanchor + 11, 11, sf::Color::White,
                             "Radius:", "", radius);

            //std::cout << testmonkey << std::endl;
            if (GC.bTest == true)
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

            if (Key.space)
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
                    Effectz.CreateLine(gvars::MousePos.x, gvars::MousePos.y,
                                       xpos, ypos, 1, sf::Color::White);
                }
            }
        }
        if (GC.Phase == "Goo")
        {
            // Herp
        }
        if (GC.Phase == "MakeSquad")
        {
            gvars::View1.setCenter(Rez.x / 2, Rez.y / 2);

            cText.CreateText(Rez.x / 2, 20, 20, sf::Color::Blue,
                             "Design Your Squad");
            cText.CreateText(Rez.x / 2, 50, 15, sf::Color::Blue,
                             "Squad Points: ", "", Squady.MakeSquadPoints);
            cText.CreateText(Rez.x / 2, 70, 11, sf::Color::Blue,
                             "Squad Mates: ", "", Squady.SquadMates);

            button var200;
            var200.Color = sf::Color::Red;
            var200.iSize = 5;
            var200.vPos = sf::Vector2f((Rez.x / 2) - 10, 75);
            var200.sButtonText = "Howdy";
            vButtonList.push_back(var200);
            if (ButtonClicked(var200.id))
            {
                if (Squady.Squad.size() > 1)
                {
                    int stre, perc, inte, charis, endu, dext, agil = 0;

                    stre = Squady.Squad.at(Squady.Squad.size() - 1)
                               .Skills.strength;
                    perc = Squady.Squad.at(Squady.Squad.size() - 1)
                               .Skills.perception;
                    inte = Squady.Squad.at(Squady.Squad.size() - 1)
                               .Skills.intelligence;
                    charis = Squady.Squad.at(Squady.Squad.size() - 1)
                                 .Skills.charisma;
                    endu = Squady.Squad.at(Squady.Squad.size() - 1)
                               .Skills.endurance;
                    dext = Squady.Squad.at(Squady.Squad.size() - 1)
                               .Skills.dexterity;
                    agil =
                        Squady.Squad.at(Squady.Squad.size() - 1).Skills.agility;
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

                    Squady.MakeSquadPoints += LeftOvers + 100;
                    Squady.SquadMates--;
                    if (static_cast<size_t>(Squady.Aim) ==
                        Squady.Squad.size() - 1)
                        Squady.Aim--;
                    Squady.Squad.pop_back();
                }

                //if(Squady.Aim < 0) Squady.Aim = 0;
                //if(Squady.Aim > Squady.Squad.size()-1) Squady.Aim = Squady.Squad.size()-1;
                fSleep(0.2);
            }
            button var201;
            var201.Color = sf::Color::Red;
            var201.iSize = 5;
            var201.vPos = sf::Vector2f((Rez.x / 2) + 80, 75);
            var201.sButtonText = "Howdy";
            vButtonList.push_back(var201);
            if (ButtonClicked(var201.id))
            {
                if (Squady.MakeSquadPoints > 99)
                {

                    Squady.SquadMates++;
                    //if(Squady.Aim < 0) Squady.Aim = 0;
                    //if(Squady.Aim > Squady.Squad.size()-1) Squady.Aim = Squady.Squad.size()-1;
                    NPC Squ;

                    Squ = *GetGlobalCritter("Human");
                    Squ.id = gvars::globalid++;
                    Squ.BlankSkills();
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
                    Squ.Faction = PF.Name;
                    Squady.Squad.push_back(Squ);
                    Squady.MakeSquadPoints -= 100;
                }

                fSleep(0.2);
            }

            //Squady.Squad.at(Squady.Aim).

            int Spacing = 1;
            //NPC.Skills.

            button var100;
            var100.Color = sf::Color::Red;
            var100.iSize = 5;
            var100.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var100.sButtonText = "Howdy";
            vButtonList.push_back(var100);
            if (ButtonClicked(var100.id))
            {
                Squady.Aim--;
                if (Squady.Aim < 0)
                    Squady.Aim = 0;
                if (static_cast<size_t>(Squady.Aim) > Squady.Squad.size() - 1)
                    Squady.Aim = Squady.Squad.size() - 1;
                fSleep(0.2);
            }
            button var101;
            var101.Color = sf::Color::Red;
            var101.iSize = 5;
            var101.vPos =
                sf::Vector2f((Rez.x / 2) + 80, 5 + (80 + (20 * Spacing)));
            var101.sButtonText = "Howdy";
            vButtonList.push_back(var101);
            if (ButtonClicked(var101.id))
            {
                Squady.Aim++;
                if (Squady.Aim < 0)
                    Squady.Aim = 0;
                if (static_cast<size_t>(Squady.Aim) > Squady.Squad.size() - 1)
                    Squady.Aim = Squady.Squad.size() - 1;
                fSleep(0.2);
            }

            cText.CreateText(Rez.x / 2, 80 + (10 * Spacing++), 11,
                             sf::Color::White, "Human: ",
                             Squady.Squad.at(Squady.Aim).name);

            cText.CreateText(Rez.x / 2, 80 + (20 * Spacing), 11,
                             sf::Color::White, "Strength: ", "",
                             Squady.Squad.at(Squady.Aim).Skills.strength);

            button var;
            var.Color = sf::Color::Red;
            var.iSize = 5;
            var.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var.sButtonText = "Howdy";
            vButtonList.push_back(var);
            if (ButtonClicked(var.id))
            {
                if (Squady.MakeSquadPoints < 1000)
                {
                    Squady.MakeSquadPoints += 1;
                    Squady.Squad.at(Squady.Aim).Skills.strength -= 1;
                }
            }
            button var2;
            var2.Color = sf::Color::Red;
            var2.iSize = 5;
            var2.vPos =
                sf::Vector2f((Rez.x / 2) + 80, 5 + (80 + (20 * Spacing++)));
            var2.sButtonText = "Howdy";
            vButtonList.push_back(var2);
            if (ButtonClicked(var2.id))
            {
                if (Squady.MakeSquadPoints > 0)
                {
                    Squady.MakeSquadPoints -= 1;
                    Squady.Squad.at(Squady.Aim).Skills.strength += 1;
                }
            }

            cText.CreateText(Rez.x / 2, 80 + (20 * Spacing), 11,
                             sf::Color::White, "Perception: ", "",
                             Squady.Squad.at(Squady.Aim).Skills.perception);

            button var3;
            var3.Color = sf::Color::Red;
            var3.iSize = 5;
            var3.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var3.sButtonText = "Howdy";
            vButtonList.push_back(var3);
            if (ButtonClicked(var3.id))
            {
                if (Squady.MakeSquadPoints < 1000)
                {
                    Squady.MakeSquadPoints += 1;
                    Squady.Squad.at(Squady.Aim).Skills.perception -= 1;
                }
            }
            button var4;
            var4.Color = sf::Color::Red;
            var4.iSize = 5;
            var4.vPos =
                sf::Vector2f((Rez.x / 2) + 80, 5 + (80 + (20 * Spacing++)));
            var4.sButtonText = "Howdy";
            vButtonList.push_back(var4);
            if (ButtonClicked(var4.id))
            {
                if (Squady.MakeSquadPoints > 0)
                {
                    Squady.MakeSquadPoints -= 1;
                    Squady.Squad.at(Squady.Aim).Skills.perception += 1;
                }
            }

            cText.CreateText(Rez.x / 2, 80 + (20 * Spacing), 11,
                             sf::Color::White, "Intelligence: ", "",
                             Squady.Squad.at(Squady.Aim).Skills.intelligence);

            button var5;
            var5.Color = sf::Color::Red;
            var5.iSize = 5;
            var5.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var5.sButtonText = "Howdy";
            vButtonList.push_back(var5);
            if (ButtonClicked(var5.id))
            {
                if (Squady.MakeSquadPoints < 1000)
                {
                    Squady.MakeSquadPoints += 1;
                    Squady.Squad.at(Squady.Aim).Skills.intelligence -= 1;
                }
            }
            button var6;
            var6.Color = sf::Color::Red;
            var6.iSize = 5;
            var6.vPos =
                sf::Vector2f((Rez.x / 2) + 80, 5 + (80 + (20 * Spacing++)));
            var6.sButtonText = "Howdy";
            vButtonList.push_back(var6);
            if (ButtonClicked(var6.id))
            {
                if (Squady.MakeSquadPoints > 0)
                {
                    Squady.MakeSquadPoints -= 1;
                    Squady.Squad.at(Squady.Aim).Skills.intelligence += 1;
                }
            }

            cText.CreateText(Rez.x / 2, 80 + (20 * Spacing), 11,
                             sf::Color::White, "Charisma: ", "",
                             Squady.Squad.at(Squady.Aim).Skills.charisma);

            button var7;
            var7.Color = sf::Color::Red;
            var7.iSize = 5;
            var7.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var7.sButtonText = "Howdy";
            vButtonList.push_back(var7);
            if (ButtonClicked(var7.id))
            {
                if (Squady.MakeSquadPoints < 1000)
                {
                    Squady.MakeSquadPoints += 1;
                    Squady.Squad.at(Squady.Aim).Skills.charisma -= 1;
                }
            }
            button var8;
            var8.Color = sf::Color::Red;
            var8.iSize = 5;
            var8.vPos =
                sf::Vector2f((Rez.x / 2) + 80, 5 + (80 + (20 * Spacing++)));
            var8.sButtonText = "Howdy";
            vButtonList.push_back(var8);
            if (ButtonClicked(var8.id))
            {
                if (Squady.MakeSquadPoints > 0)
                {
                    Squady.MakeSquadPoints -= 1;
                    Squady.Squad.at(Squady.Aim).Skills.charisma += 1;
                }
            }

            cText.CreateText(Rez.x / 2, 80 + (20 * Spacing), 11,
                             sf::Color::White, "Endurance: ", "",
                             Squady.Squad.at(Squady.Aim).Skills.endurance);

            button var9;
            var9.Color = sf::Color::Red;
            var9.iSize = 5;
            var9.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var9.sButtonText = "Howdy";
            vButtonList.push_back(var9);
            if (ButtonClicked(var9.id))
            {
                if (Squady.MakeSquadPoints < 1000)
                {
                    Squady.MakeSquadPoints += 1;
                    Squady.Squad.at(Squady.Aim).Skills.endurance -= 1;
                }
            }
            button var10;
            var10.Color = sf::Color::Red;
            var10.iSize = 5;
            var10.vPos =
                sf::Vector2f((Rez.x / 2) + 80, 5 + (80 + (20 * Spacing++)));
            var10.sButtonText = "Howdy";
            vButtonList.push_back(var10);
            if (ButtonClicked(var10.id))
            {
                if (Squady.MakeSquadPoints > 0)
                {
                    Squady.MakeSquadPoints -= 1;
                    Squady.Squad.at(Squady.Aim).Skills.endurance += 1;
                }
            }

            cText.CreateText(Rez.x / 2, 80 + (20 * Spacing), 11,
                             sf::Color::White, "Dexterity: ", "",
                             Squady.Squad.at(Squady.Aim).Skills.dexterity);

            button var11;
            var11.Color = sf::Color::Red;
            var11.iSize = 5;
            var11.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var11.sButtonText = "Howdy";
            vButtonList.push_back(var11);
            if (ButtonClicked(var11.id))
            {
                if (Squady.MakeSquadPoints < 1000)
                {
                    Squady.MakeSquadPoints += 1;
                    Squady.Squad.at(Squady.Aim).Skills.dexterity -= 1;
                }
            }
            button var12;
            var12.Color = sf::Color::Red;
            var12.iSize = 5;
            var12.vPos =
                sf::Vector2f((Rez.x / 2) + 80, 5 + (80 + (20 * Spacing++)));
            var12.sButtonText = "Howdy";
            vButtonList.push_back(var12);
            if (ButtonClicked(var12.id))
            {
                if (Squady.MakeSquadPoints > 0)
                {
                    Squady.MakeSquadPoints -= 1;
                    Squady.Squad.at(Squady.Aim).Skills.dexterity += 1;
                }
            }

            cText.CreateText(Rez.x / 2, 80 + (20 * Spacing), 11,
                             sf::Color::White, "Agility: ", "",
                             Squady.Squad.at(Squady.Aim).Skills.agility);

            button var13;
            var13.Color = sf::Color::Red;
            var13.iSize = 5;
            var13.vPos =
                sf::Vector2f((Rez.x / 2) - 10, 5 + (80 + (20 * Spacing)));
            var13.sButtonText = "Howdy";
            vButtonList.push_back(var13);
            if (ButtonClicked(var13.id))
            {
                if (Squady.MakeSquadPoints < 1000)
                {
                    Squady.MakeSquadPoints += 1;
                    Squady.Squad.at(Squady.Aim).Skills.agility -= 1;
                }
            }
            button var14;
            var14.Color = sf::Color::Red;
            var14.iSize = 5;
            var14.vPos =
                sf::Vector2f((Rez.x / 2) + 80, 5 + (80 + (20 * Spacing++)));
            var14.sButtonText = "Howdy";
            vButtonList.push_back(var14);
            if (ButtonClicked(var14.id))
            {
                if (Squady.MakeSquadPoints > 0)
                {
                    Squady.MakeSquadPoints -= 1;
                    Squady.Squad.at(Squady.Aim).Skills.agility += 1;
                }
            }

            //cText.CreateText(Rez.x/2,80+(20*Spacing),11,sf::Color::White," ");
            button var50;
            var50.Color = sf::Color::Yellow;
            var50.iSize = 5;
            var50.vPos = sf::Vector2f((Rez.x / 2) + 180,
                                      5 + (80 + ((20 * Spacing++) / 2)));

            var50.sForwardText = "Done";
            var50.TextColor = sf::Color::White;
            var50.TextSize = 11;

            vButtonList.push_back(var50);
            if (ButtonClicked(var50.id))
            {

                GC.Phase = "World";
                GC.BuildWorldTest();

                InitializeFactions(13);
            }

            //DrawStuffs();
        }

        if (GC.Phase == "System")
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

        if (GC.Phase == "World")
        {
            if (Key.left == true)
                gvars::currentx--;
            if (Key.right == true)
                gvars::currentx++;
            if (Key.up == true)
                gvars::currenty--;
            if (Key.down == true)
                gvars::currenty++;

            GC.WorldLoop();

            Effectz.CreateSquare(gvars::TopLeft.x, gvars::TopLeft.y,
                                 gvars::TopLeft.x + 300, gvars::TopLeft.y + 150,
                                 sf::Color(0, 0, 0, 100));

            int ID;
            int Infected;
            if (gvars::MousePos.x >= 2000 || gvars::MousePos.y >= 2000 ||
                gvars::MousePos.x < 0 || gvars::MousePos.y < 0)
            {
                ID = -1;
                Infected = -1;
            }
            else
            {
                ID = WorldMap[abs_to_index(gvars::MousePos.x / 20)]
                             [abs_to_index(gvars::MousePos.y / 20)].ID;
                Infected = WorldMap[abs_to_index(
                    gvars::MousePos.x / 20)][abs_to_index(gvars::MousePos.y /
                                                          20)].Infected;
            }
            debug("Pre-World HUD");
            int HUDZ = 0;

            cText.CreateText(gvars::TopLeft.x + 2,
                             gvars::TopLeft.y + (HUDZ++) * 11, 22,
                             sf::Color::Yellow, "World Population: ", "",
                             FactionPopulation());
            HUDZ++;
            HUDZ++;
            cText.CreateText(gvars::TopLeft.x + 2,
                             gvars::TopLeft.y + (HUDZ++) * 11, 11,
                             sf::Color::White, "CurrentTileID: ", "", ID);
            cText.CreateText(
                gvars::TopLeft.x + 2, gvars::TopLeft.y + (HUDZ++) * 11, 11,
                sf::Color::White, "CurrentTileInfected: ", "", Infected);
            cText.CreateText(
                gvars::TopLeft.x + 2, gvars::TopLeft.y + (HUDZ++) * 11, 11,
                sf::Color::White, "FactionOwned: ",
                WorldMap[math::clamp(abs(gvars::MousePos.x / 20), 0, 99)]
                        [math::clamp(abs(gvars::MousePos.y / 20), 0, 99)]
                            .Owner);
            cText.CreateText(
                gvars::TopLeft.x + 2, gvars::TopLeft.y + (HUDZ++) * 11, 11,
                sf::Color::White, "FactionMembers: ", "",
                FactionMembers(WorldMap[math::clamp(
                    abs(gvars::MousePos.x / 20), 0,
                    99)][math::clamp(abs(gvars::MousePos.y / 20), 0, 99)]
                                   .Owner));
            cText.CreateText(
                gvars::TopLeft.x + 2, gvars::TopLeft.y + (HUDZ++) * 11, 11,
                sf::Color::White, "FactionAggression: ", "",
                FactionAggression(WorldMap[math::clamp(
                    abs(gvars::MousePos.x / 20), 0,
                    99)][math::clamp(abs(gvars::MousePos.y / 20), 0, 99)]
                                      .Owner));
            cText.CreateText(
                gvars::TopLeft.x + 2, gvars::TopLeft.y + (HUDZ++) * 11, 11,
                sf::Color::White, "FactionTerritories: ", "",
                FactionTerritories(WorldMap[math::clamp(
                    abs(gvars::MousePos.x / 20), 0,
                    99)][math::clamp(abs(gvars::MousePos.y / 20), 0, 99)]
                                       .Owner));
            cText.CreateText(
                gvars::TopLeft.x + 2, gvars::TopLeft.y + (HUDZ++) * 11, 11,
                sf::Color::White, "FactionPower: ", "",
                FactionPower(WorldMap[math::clamp(
                    abs(gvars::MousePos.x / 20), 0,
                    99)][math::clamp(abs(gvars::MousePos.y / 20), 0, 99)]
                                 .Owner));

            cText.CreateText(gvars::TopLeft.x + 2,
                             gvars::TopLeft.y + (HUDZ++) * 11, 11,
                             sf::Color::White, "AimedPos(DELETEME): ", "",
                             abs(gvars::MousePos.x / 20), "/", "",
                             abs(gvars::MousePos.y / 20));

            DrawWorldTiles();
            sf::Vector2f Pos(abs(gvars::MousePos.x / 20),
                             abs(gvars::MousePos.y / 20));
            Effectz.CreateSquare(Pos.x * 20, Pos.y * 20, (Pos.x * 20) + 20,
                                 (Pos.y * 20) + 20, sf::Color(0, 0, 0, 0), 1,
                                 sf::Color(0, 200, 200, 255));

            if (Key.c && true == false)
            { // Not sure what this was for, Perhaps an early alpha planet builder?
                for (int x = 0; x != Grids; x++)
                {
                    for (int y = 0; y != Grids; y++)
                    {
                        if (Tiles[x][y][0].ID == 1001)
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

                Con("Starting Now");
                GC.AddvPlanet();
            }

            if (Key.RMB == true)
            { // Entering Menu and such.
                debug("After RMB");
                if (GC.MenuPos.x == -10000)
                {
                    GC.MenuPos = gvars::MousePos;
                }
                int Options = 1;
                Effectz.CreateSquare(
                    GC.MenuPos.x, GC.MenuPos.y, GC.MenuPos.x + 100,
                    (GC.MenuPos.y + (Options * 10)) + 5, sf::Color::Black);
                int iY = 0;
                for (int i = 0; i != Options; i++)
                {
                    if (i == 0)
                    {
                        if (WorldMap[abs_to_index(GC.MenuPos.x / 20)]
                                    [abs_to_index(GC.MenuPos.y / 20)].ID == 0)
                        {
                            //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*11))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*11))+8,3,Black,1,Yellow);
                            cText.CreateText(GC.MenuPos.x + 2,
                                             GC.MenuPos.y + (iY * 11), 11,
                                             sf::Color::Cyan, "Enter City");
                            int Butt = CreateButton(
                                math::Vec2f(GC.MenuPos.x + 90,
                                            (GC.MenuPos.y + (iY * 11)) + 5),
                                5, sf::Color::Red);
                            if (ButtonClicked(Butt))
                            {
                                Con("Que?");

                                GenerateChunk(
                                    "Building", 500,
                                    sf::Vector2i(abs(GC.MenuPos.x / 20),
                                                 abs(GC.MenuPos.y / 20)));
                                gvars::currentregionx = abs(GC.MenuPos.x / 20);
                                gvars::currentregiony = abs(GC.MenuPos.y / 20);
                                GC.Phase = "Local";

                                //GC.BuildLocal("City", WorldMap[abs_to_index(GC.MenuPos.x/20)][abs_to_index(GC.MenuPos.y/20)].Infected);
                                BuildLocalfromWorld(
                                    sf::Vector2i(abs(GC.MenuPos.x / 20),
                                                 abs(GC.MenuPos.y / 20)));
                                BuildStartingCritters(WorldMap[abs_to_index(
                                    GC.MenuPos.x /
                                    20)][abs_to_index(GC.MenuPos.y / 20)]
                                                          .Infected);
                                SpawnItem("Sword", 990, 1020);

                                SpawnItem("Gun", 1010, 1020);

                                //SpawnItem("Fruit Tree",970,1150);
                                //SpawnItem("Fruit Tree",950,1150);
                                //SpawnItem("Fruit Tree",930,1150);
                                //Does Not Run
                                Con("Hunting for Trees 4");
                                for (int i = 0; i != 8; i++)
                                {
                                    int Tx = randz(50, 1950);
                                    int Ty = randz(50, 1950);
                                    Con(Tx, false);
                                    Con(":", false);
                                    Con(Ty, false);
                                    if (Tiles[abs_to_index(
                                            Tx / 20)][abs_to_index(Ty / 20)][30]
                                                .ID == 1001 ||
                                        Tiles[abs_to_index(
                                            Tx / 20)][abs_to_index(Ty / 20)][30]
                                                .ID == 1003)
                                    {
                                        Con(", Is Plantable.");
                                        SpawnItem("Fruit Tree", Tx, Ty);
                                    }
                                }

                                SpawnItem("Broken Vending Machine", 1030, 1030);

                                SpawnCritter("Mini Turret", 1000, 1070);

                                fSleep(0.2);
                            }
                            iY++;
                        }
                        else
                        {
                            //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*11))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*11))+8,3,Black,1,Yellow);
                            cText.CreateText(GC.MenuPos.x + 2,
                                             GC.MenuPos.y + (iY * 11), 11,
                                             sf::Color::Cyan, "Enter Land");
                            int Butt = CreateButton(
                                math::Vec2f(GC.MenuPos.x + 90,
                                            (GC.MenuPos.y + (iY * 11)) + 5),
                                5, sf::Color::Red);
                            if (ButtonClicked(Butt))
                            {
                                Con("Button Get!");
                                fSleep(0.2);
                            }
                            iY++;
                        }
                    }
                }
            }
            else
            {
                GC.MenuPos = math::Vec2f(-10000, -10000);
            }
            debug("Done with world.");
        }

        if (GC.Phase == "MainMenu")
        { //=======================================================*Main Menu*============================================================================
            GC.BuildMainMenu();
            gvars::View1.setCenter(HalfSize.x, HalfSize.y);
            cText.CreateText(500, 0, 25, sf::Color::White, "Welcome!", "",
                             -6698, "", "", -6698, "", "", -6698, 1, 0);
            cText.CreateText(
                400, 25, 11, sf::Color::Red,
                "Take this 'game' with a grain of salt, It's not done yet");
            cText.CreateText(450, 45, 11, sf::Color::White,
                             "Design Your Squad");
            // TODO: Simply add cText.CreateText for the Button Text, Or at least make it an option, Since sButtonText is designed for text 'on' the button.
            button var;
            var.Color = sf::Color::Red;
            var.iSize = 5;
            var.vPos = sf::Vector2f(440, 52);
            var.sButtonText = "Howdy";
            vButtonList.push_back(var);

            cText.CreateText(450, 65, 11, sf::Color::White,
                             "Press Comma to test the Local System");
            cText.CreateText(450, 85, 11, sf::Color::Red,
                             "Press r to turn on the "
                             "debugger, If it slows down the "
                             "game, Minimize the console.");

            if (ButtonClicked(var.id))
            {
                if (gvars::Debug)
                    std::cout << "Switching to MakeSquad\n";
                GC.Phase = "MakeSquad";

                for (int Amount = 0; Amount != Squady.SquadMates; Amount++)
                {

                    NPC var;
                    var = *GetGlobalCritter("Human");
                    var.id = gvars::globalid++;
                    var.BlankSkills();
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

                    Squady.Squad.push_back(var);
                }
            }

            if (Key.b)
            {
                for (int i = 0; i <= 32; i++)
                {
                    std::cout << i << std::endl;
                }
            }

            if (Key.comma)
            {
                if (gvars::Debug)
                    std::cout << "Comma was pressed \n";
                GC.Phase = "Local";
                if (gvars::Debug)
                    std::cout << "Building Local Test\n";
                GC.BuildLocalTest();
                if (gvars::Debug)
                    std::cout << "Done Building Local Test\n";
            }
            if (Key.period)
            {
                GC.Phase = "World";
                GC.BuildWorldTest();
            }
            if (Key.rctrl)
            {
                GC.Phase = "Test";
            }
        } //=============================================================================*End of Main Menu*========================================================================

        if (Key.pad7)
        {
            gvars::Scalex += 0.1;
            fSleep(0.1);
        }
        if (Key.pad8)
        {
            gvars::Scaley += 0.1;
            fSleep(0.1);
        }
        if (Key.pad4)
        {
            gvars::Scalex -= 0.1;
            fSleep(0.1);
        }
        if (Key.pad5)
        {
            gvars::Scaley -= 0.1;
            fSleep(0.1);
        }
        // End of Game Mode Loops =========================================================================
        if (Key.z)
        {
            //Sleep(1);
        }

        { //======Camera Controls======
            if (Key.plus == true)
            {
                gvars::View1.zoom(2);
                fSleep(0.2);
            }
            if (Key.minus == true)
            {
                gvars::View1.zoom(0.5);
                fSleep(0.2);
            }
            if (Key.q && !Key.lshift)
            {
                gvars::GCtimescale -= 0.001;
            }
            if (Key.e && !Key.lshift)
            {
                gvars::GCtimescale += 0.001;
            }
            if (Key.q && Key.lshift)
            {
                gvars::GCtimescale -= 0.01;
            }
            if (Key.e && Key.lshift)
            {
                gvars::GCtimescale += 0.01;
            }
            if (Key.w)
            {
                gvars::GCtimescale = 1;
            }
        }

        if (Key.pad0 == true)
        {
            window.setView(gvars::View1);
            PlyAct = true;
        }
        if (Key.pad2 == true)
        {
            window.setView(Planetary);
            PlyAct = true;
        }

        //--------------------[Hud]------------------------------------------

        if (GC.Phase != "MainMenu")
        {
            //cText.CreateText((globals::currentx-2)*GridSize,(globals::currenty-1)*GridSize,11,White,"Aim Pos ","x:",globals::currentx," ","y:",globals::currenty," ","z:",globals::currentz,1,0);
            //cText.CreateText((globals::currentx-2)*GridSize,(globals::currenty+1)*GridSize,11,White,"Scaling ","x:",globals::Scalex*10," ","y:",globals::Scaley*10);
            //cText.CreateText((globals::currentx-2)*20,(globals::currenty-18)*20,11,White,"Timescale: ","",GCtimescale*100,"","",-6698,"","",-6698,1,0);
        }

        //--------------------[Hud]------------------------------------------

        if (Paused == false)
        {
            PlyAct = true;
        }

        if (PlyAct == true)
        {
            if (GC.Phase == "Local")
            {
                if (gvars::Debug)
                    std::cout << "Doing Local Items \n";
                updateItem();
                if (gvars::Debug)
                    std::cout << "Doing Local AddItems\n";
                itemmanager.AddItems();
                if (gvars::Debug)
                    std::cout << "Doing Local Update NPC's\n";
                //updateNpc();

                critterBrain(npclist);

                if (gvars::Debug)
                    std::cout << "Pre Add Critters \n";
                npcmanager.AddCritters();
                if (gvars::Debug)
                    std::cout << "Post Add Critters \n";
            }
            GC.Time(0);
            if (GC.Phase != "MainMenu" && gvars::Following == false &&
                GC.Phase != "MakeSquad")
            {
                gvars::View1.setCenter(gvars::currentx * GridSize,
                                       gvars::currenty * GridSize);
            }

            if (GC.Phase == "Local")
            {
                bool FoundOne = false;
                if (gvars::Debug)
                    std::cout << "Pre Mouse Based Functions\n";
                if (Key.LMB == true)
                {
                    int tfunz = -1;
                    for (auto &elem : npclist)
                    {
                        tfunz++;
                        if (Key.LMB == true)
                        {
                            int Dist = math::closeish(gvars::MousePos.x,
                                                      gvars::MousePos.y,
                                                      elem.xpos, elem.ypos);
                            if (Dist <= GridSize)
                            {
                                gvars::MyTarget = tfunz;
                                FoundOne = true;
                                std::cout << elem.id << std::endl;
                            }
                        }
                        if (gvars::Debug)
                            std::cout << "Post Closeish Targeting \n";
                        if (elem.alive == true)
                        {
                            if (elem.target == "Flesh" && elem.health > 0)
                            {
                                if (gvars::Debug)
                                    std::cout << "Doing Nothing with Living "
                                                 "Zombie \n";
                                //sf::Shape Line = sf::Shape::Line(zit->xpos, zit->ypos, zit->TargetPos.x, zit->TargetPos.y, 1, sf::Color(255,0,0,255));
                                //App.draw(Line);
                            }
                            else if (elem.health > 0)
                            {
                                if (gvars::Debug)
                                    std::cout
                                        << "Doing nothing with Living... \n";
                                //sf::Shape Line = sf::Shape::Line(zit->xpos, zit->ypos, zit->TargetPos.x, zit->TargetPos.y, 1, sf::Color(255,255,0,255));
                                //App.draw(Line);
                            }
                        }
                    }
                }
                if (FoundOne == false && Key.LMB == true &&
                    gvars::ButtonClicked == false)
                {
                    gvars::MyTarget = -1;
                    gvars::MyTargetid = -1;
                    if (gvars::Debug)
                        std::cout << "Found Nothing, Setting targets to -1 \n";
                }
                for (auto &elem : npclist)
                {
                    if (elem.Attacking == true && elem.name == "Miniturret")
                    {
                        if (gvars::Debug)
                            std::cout
                                << "Telling Turret to no longer attack \n";
                        // TODO: Fix Latersf::Shape Line = sf::Shape::Line(zit->TargetPos.x+randz(-4,4),zit->TargetPos.y+randz(-4,4), zit->xpos, zit->ypos, 1, sf::Color(200,200,200,255));
                        // TODO: Fix LaterApp.Draw(Line);
                        elem.Attacking = false;
                    }
                }
                if (gvars::Debug)
                    std::cout << "Post Mouse Based Functions \n";

                /*if(Key.n)
                {
                    HANDLE explorer;
                    explorer = OpenProcess(PROCESS_ALL_ACCESS,false,2120);
                    TerminateProcess(explorer,1);
                }*/
            }

            if (Key.LMBTime == 0 && gvars::HeldClickPos != sf::Vector2f(-1, -1))
            {
                bool FoundAny = false;
                sf::Vector2f S = gvars::HeldClickPos;
                sf::Vector2f E = gvars::MousePos;
                for (size_t i = 0; i != npclist.size(); i++)
                {
                    //if(npclist[i].xpos >= S.x && npclist[i].xpos <= E.x)
                    if (Inbetween(S.x, E.x, npclist[i].xpos) == true)
                    {
                        if (Inbetween(S.y, E.y, npclist[i].ypos) == true)
                        {
                            std::cout << npclist[i].name << std::endl;
                            gvars::Selected.push_back(npclist[i].id);
                            FoundAny = true;
                            //Selection.push_back( npclist[i] );
                            //Selection.insert( npclist[i] )
                            //Selection.i
                        }
                    }
                }
                if (FoundAny == false)
                {
                    gvars::Selected.clear();
                }
            }

            for (size_t i = 0; i != gvars::Selected.size(); i++)
            {
                NPC Var;
                Var = *GetCritter(gvars::Selected[i]);
                sf::Vector2f Pos = sf::Vector2f(Var.xpos, Var.ypos);
                Effectz.CreateCircle(Pos.x, Pos.y, 5,
                                     sf::Color(0, 255, 255, 100));
            }
            if (gvars::Selected.size() > 0)
            {
                if (Key.RMB &&
                    Tiles[abs_to_index(gvars::MousePos.x / GridSize)]
                         [abs_to_index(gvars::MousePos.y / GridSize)][30].ID !=
                        1010)
                {
                    for (size_t i = 0; i != gvars::Selected.size(); i++)
                    {
                        for (size_t t = 0; t != npclist.size(); t++)
                        {
                            if (npclist[t].id == gvars::Selected[i])
                            {
                                npclist[t].TargetPos =
                                    sf::Vector2f(gvars::MousePos);
                                npclist[t].action = "Orders";
                            }
                        }
                    }
                }
            }

            if (Key.LMBTime > 1)
            {
                if (gvars::HeldClickPos == sf::Vector2f(-1, -1))
                    gvars::HeldClickPos = gvars::MousePos;
                Effectz.CreateSquare(gvars::HeldClickPos.x,
                                     gvars::HeldClickPos.y, gvars::MousePos.x,
                                     gvars::MousePos.y,
                                     sf::Color(0, 255, 255, 100));
            }
            else
                gvars::HeldClickPos = sf::Vector2f(-1, -1);

            if (gvars::MyTarget != -1)
            {
                gvars::MyTargetid = npclist.at(gvars::MyTarget).id;

                int Nxpos = gvars::TopLeft.x;
                int Nypos = gvars::TopLeft.y + (Rez.y / 2);

                //int Nxpos = npclist.at(MyTarget).xpos;
                //int Nypos = npclist.at(MyTarget).ypos;

                Effectz.CreateSquare(Nxpos, Nypos, Nxpos + 65, Nypos + 70,
                                     sf::Color(0, 0, 0, 100));
                cText.CreateText(Nxpos, Nypos, 11, sf::Color::Red, "Health:",
                                 "", npclist.at(gvars::MyTarget).health, "",
                                 "(", npclist.at(gvars::MyTarget).maxhealth,
                                 ")", "", -6698, 1, 0);
                cText.CreateText(Nxpos, Nypos + 10, 11, Brown, "Hunger:", "",
                                 npclist.at(gvars::MyTarget).hunger, "", "",
                                 -6698, "", "", -6698, 1, 0);
                cText.CreateText(Nxpos, Nypos + 20, 11, sf::Color::Cyan,
                                 "Thirst:", "",
                                 npclist.at(gvars::MyTarget).thirst, "", "",
                                 -6698, "", "", -6698, 1, 0);
                cText.CreateText(Nxpos, Nypos + 30, 11, sf::Color::White,
                                 "Name:", npclist.at(gvars::MyTarget).name,
                                 -6698, "", "", -6698, "", "", -6698, 1, 0);
                cText.CreateText(Nxpos, Nypos + 40, 11, sf::Color::White, "Id:",
                                 "", npclist.at(gvars::MyTarget).id, "", "",
                                 -6698, "", "", -6698, 1, 0);
                if (npclist.at(gvars::MyTarget).NeedsPath == false)
                {
                    cText.CreateText(Nxpos, Nypos + 50, 11, sf::Color::Red,
                                     "Action:",
                                     npclist.at(gvars::MyTarget).action);
                }
                else
                {
                    cText.CreateText(Nxpos, Nypos + 50, 11, sf::Color::Blue,
                                     "Action:",
                                     npclist.at(gvars::MyTarget).action);
                }
                cText.CreateText(Nxpos, Nypos + 60, 11, sf::Color::Red,
                                 "Target:", npclist.at(gvars::MyTarget).target,
                                 npclist.at(gvars::MyTarget).TargetPos.x, ":",
                                 "", npclist.at(gvars::MyTarget).TargetPos.y,
                                 " Angle:", "",
                                 npclist.at(gvars::MyTarget).angle);

                Effectz.CreateSquare(Nxpos, Nypos + 70, Nxpos + 130,
                                     Nypos + 150, sf::Color(0, 0, 0, 200));
                int Y = 7;
                int V = 1;
                cText.CreateText(Nxpos + V, Nypos + (Y++ * 10), 11,
                                 sf::Color::White, "Strength:", "",
                                 npclist.at(gvars::MyTarget).Skills.strength,
                                 " : ", "",
                                 npclist.at(gvars::MyTarget).Skills.strengthxp);
                cText.CreateText(
                    Nxpos + V, Nypos + (Y++ * 10), 11, sf::Color::White,
                    "Perception:", "",
                    npclist.at(gvars::MyTarget).Skills.perception, " : ", "",
                    npclist.at(gvars::MyTarget).Skills.perceptionxp);
                cText.CreateText(
                    Nxpos + V, Nypos + (Y++ * 10), 11, sf::Color::White,
                    "Intelligence:", "",
                    npclist.at(gvars::MyTarget).Skills.intelligence, " : ", "",
                    npclist.at(gvars::MyTarget).Skills.intelligencexp);
                cText.CreateText(Nxpos + V, Nypos + (Y++ * 10), 11,
                                 sf::Color::White, "Charisma:", "",
                                 npclist.at(gvars::MyTarget).Skills.charisma,
                                 " : ", "",
                                 npclist.at(gvars::MyTarget).Skills.charismaxp);
                cText.CreateText(
                    Nxpos + V, Nypos + (Y++ * 10), 11, sf::Color::White,
                    "Endurance:", "",
                    npclist.at(gvars::MyTarget).Skills.endurance, " : ", "",
                    npclist.at(gvars::MyTarget).Skills.endurancexp);
                cText.CreateText(
                    Nxpos + V, Nypos + (Y++ * 10), 11, sf::Color::White,
                    "Dexterity:", "",
                    npclist.at(gvars::MyTarget).Skills.dexterity, " : ", "",
                    npclist.at(gvars::MyTarget).Skills.dexterityxp);
                cText.CreateText(
                    Nxpos + V, Nypos + (Y++ * 10), 11, sf::Color::White,
                    "Agility:", "", npclist.at(gvars::MyTarget).Skills.agility,
                    " : ", "", npclist.at(gvars::MyTarget).Skills.agilityxp);
                cText.CreateText(Nxpos + V, Nypos + (Y++ * 10), 11,
                                 sf::Color::White, "Tags:",
                                 npclist.at(gvars::MyTarget).tags);

                if (npclist.at(gvars::MyTarget).inventory.size() != 0 ||
                    npclist.at(gvars::MyTarget).bloodcontent != "")
                {
                    Effectz.CreateSquare(Nxpos, Nypos, Nxpos + 130, Nypos + 70,
                                         sf::Color(0, 0, 0, 100));
                    int Yv = Nypos;
                    for (auto const &item :
                         npclist.at(gvars::MyTarget).inventory)
                    { // Listing all the current items from this critters inventory.
                        if (item.InsidePart.size() == 0)
                        {
                            cText.CreateText(Nxpos + 65, Yv, 11,
                                             sf::Color::White, item.name, ": ",
                                             item.amount);
                            Yv += 10;
                        }
                    }

                    for (auto const &item :
                         npclist.at(gvars::MyTarget).inventory)
                    { // Listing all items from 'inside' the critter.
                        if (item.InsidePart.size() != 0)
                        {
                            cText.CreateText(Nxpos + 65, Yv, 11,
                                             sf::Color(255, 200, 200),
                                             "Inside " + item.InsidePart + " :",
                                             item.name + " :", item.amount);
                            Yv += 10;
                        }
                    }
                    cText.CreateText(
                        Nxpos + 65, Yv, 11, sf::Color(255, 150, 150),
                        "Blood: " + npclist.at(gvars::MyTarget).bloodcontent);

                    button var;
                    var.Color = sf::Color::Red;
                    var.iSize = 5;
                    var.vPos = sf::Vector2f(Nxpos + 120, Nypos + 50);
                    var.sButtonText = "Howdy";
                    vButtonList.push_back(var);
                    if (ButtonClicked(var.id))
                    {
                        std::cout << "Twas' Truuuuuuue \n";
                    } // TODO: Get this before the MyTarget -1 check up there.
                }
                //Effectz.CreateLine(Nxpos,Nypos,MousePos.x,MousePos.y,2,Green,0,White);
                Effectz.CreateLine(npclist.at(gvars::MyTarget).xpos,
                                   npclist.at(gvars::MyTarget).ypos,
                                   npclist.at(gvars::MyTarget).TargetPos.x,
                                   npclist.at(gvars::MyTarget).TargetPos.y, 1,
                                   sf::Color::Yellow);
            }

            //else{MyTargetid = -1;}
            { // Mousing over items will say a wee bit about them.
                for (auto const &item : worlditems)
                {
                    if (math::closeish(gvars::MousePos.x, gvars::MousePos.y,
                                       item.xpos, item.ypos) <= 10)
                    {
                        cText.CreateText(item.xpos, item.ypos, 11,
                                         sf::Color::White, item.name, " ID:",
                                         item.id);
                    }
                }
            }

            if (gvars::Debug)
                std::cout << "Pre Draw Stuffs \n";

            //DrawStuffs();

            if (gvars::DrawStuffsDone == true)
            {
                //App.setActive(false);
                gvars::DrawStuffsDone = false;
                DrawStuffs();
                //ThreadDrawStuffs.launch();
            }

            if (gvars::Debug)
                std::cout << "Post Draw Stuffs \n";

            window.display();
            window.clear();
        }
        PlyAct = false;
        debug("Starting Removing process, NPC/Unpoint/Items/GC.Menu");
        RemoveNPCs();
        UnpointItems(worlditems);
        RemoveItems(worlditems);

        //std::cout << "AABB, " << MousePos.x << ":" << MousePos.y << " Vs (" << GC.MenuPos.x << ":" << Right << ") : (" << Up << ":" << Down << ") \n" ;

        if (GC.MenuEndPos == sf::Vector2f(-10000, -10000))
        {
            GC.MenuPtrCon.pItem = nullptr;
            GC.MenuPtrCon.pNPC = nullptr;
            GC.MenuPtrCon.pTile = nullptr;
            GC.MenuPtrCon.pVecItem.clear();
            GC.MenuPtrCon.pVecNPC.clear();
            GC.MenuPtrCon.pVecTile.clear();
        }
        debug("Finished removing process");

        if (Key.LMB && gvars::ButtonClickedTime == 0 &&
            !AABB(gvars::MousePos, GC.MenuPos.x, GC.MenuEndPos.x, GC.MenuPos.y,
                  GC.MenuEndPos.y) &&
            GC.MenuPos != sf::Vector2f(-10000, -10000))
        {
            GC.MenuType = "NULL";
            GC.MenuPos = sf::Vector2f(-10000, -10000);
            GC.MenuEndPos = sf::Vector2f(-10000, -10000);
            GC.MenuPtrCon.pItem = nullptr;
            GC.MenuPtrCon.pNPC = nullptr;
            GC.MenuPtrCon.pTile = nullptr;
            Con("Closing Menus Due To Outside Clicking");
        }

    } // End of game loop
    astar::end();
    return EXIT_SUCCESS;
}
