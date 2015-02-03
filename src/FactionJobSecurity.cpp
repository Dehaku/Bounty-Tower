#include "FactionJobSecurity.h"

#include "Faction.h"
#include "Tiles.h"
#include "util.h"
#include "globalvars.h"

#include <iostream>

using std::abs;

void rmbMenuItem(Item &pItem)
{
    /*
    std::cout << pItem.name << std::endl;
    Job job;
    job.Name = "PickUpItem";
    job.Type = "PickUp";
    job.pItem = &pItem;
    JobList.push_back(job);
    */
    for (size_t i = 0; i != uniFact.size(); i++)
    {
        if (uniFact[i].playerControlled)
        {
            Job job;
            job.pItem = &pItem;
            if (job.pItem->name == "Fruit Tree")
            {
                job.name = "ChopDownTree";
                job.type = "Chop";
            }
            else
            {
                job.name = "PickUpItem";
                job.type = "PickUp";
            }

            uniFact[i].jobList.push_back(job);
        }
    }
}

void digWall(sf::Vector2f Pos)
{
    for (size_t i = 0; i != uniFact.size(); i++)
    {
        if (uniFact[i].playerControlled)
        {
            Job job;
            std::cout << "Tile clicked: "
                      << Tiles[abs_to_index(Pos.x / 20)][abs_to_index(
                             Pos.y / 20)][6].ID << std::endl;
            if (Tiles[abs_to_index(Pos.x / 20)][abs_to_index(Pos.y / 20)][30]
                    .ID == 1008)
            {
                job.name = "DigNaturalWall";
                job.type = "Dig";
                job.workPos.x = (abs(Pos.x / 20) * 20) + 10;
                job.workPos.y = (abs(Pos.y / 20) * 20) + 10;
            }
            else
            {

                return;
            }

            uniFact[i].jobList.push_back(job);
        }
    }
}

void rmbMenuTile(sf::Vector2f Pos)
{
    for (size_t i = 0; i != uniFact.size(); i++)
    {
        if (uniFact[i].playerControlled)
        {
            Job job;
            //job.pItem = &pItem;
            //globals::groundmap[abs_to_index(Pos.x/20)][abs_to_index(Pos.y/20)]
            Con("Dafuqe \n");
            if (Tiles[abs_to_index(Pos.x / 20)][abs_to_index(Pos.y / 20)][30]
                    .ID != 1010)
            {
                job.name = "BuildWoodWall";
                job.type = "Build";
                job.workPos.x = (abs(Pos.x / 20) * 20) + 10;
                job.workPos.y = (abs(Pos.y / 20) * 20) + 10;
                Con("Dafuqe2 \n");
            }
            else
            {
                Con("Dafuqe3 \n");
                return;
            }

            Con("Dafuqe4 \n");
            uniFact[i].jobList.push_back(job);
            Con("Dafuqe5 \n");
        }
    }
}

void drawJobList(int DrawXPos, int DrawYPos)
{
    int YVariance = 1;
    for (size_t i = 0; i != uniFact.size(); i++)
    {
        if (uniFact[i].playerControlled)
        {
            for (size_t t = 0; t != uniFact[i].jobList.size(); t++)
            {

                //cText.CreateText(DrawXPos,DrawYPos+(YVariance*10),11,sf::Color::Yellow,AddString(UniFact[i].JobList[t].Type," "),UniFact[i].JobList[t].Name);

                if (uniFact[i].jobList[t].pItem != nullptr &&
                    uniFact[i].jobList[t].pWorker != nullptr)
                {
                    debug("First condition");
                    debug("CritterName");
                    debug("ItemName");

                    cText.CreateText(
                        DrawXPos, DrawYPos + (YVariance * 10), 11,
                        sf::Color::Yellow, uniFact[i].jobList[t].pWorker->name +
                                               uniFact[i].jobList[t].name + " ",
                        uniFact[i]
                            .jobList[t]
                            .pItem->name); // ,JobList.at(i).pItem.name
                }
                else if (uniFact[i].jobList[t].pItem != nullptr)
                {
                    debug("Second condition");
                    cText.CreateText(
                        DrawXPos, DrawYPos + (YVariance * 10), 11,
                        sf::Color::Yellow, uniFact[i].jobList[t].name + " ",
                        uniFact[i]
                            .jobList[t]
                            .pItem->name); // ,JobList.at(i).pItem.name
                }
                else if (uniFact[i].jobList[t].pWorker != nullptr)
                {
                    debug("Third condition");
                    cText.CreateText(DrawXPos, DrawYPos + (YVariance * 10), 11,
                                     sf::Color::Yellow,
                                     uniFact[i].jobList[t].pWorker->name +
                                         uniFact[i].jobList[t].type + " ",
                                     uniFact[i].jobList[t].name);
                }
                else
                {
                    debug("Fourth condition");
                    cText.CreateText(DrawXPos, DrawYPos + (YVariance * 10), 11,
                                     sf::Color::Yellow,
                                     uniFact[i].jobList[t].type + " ",
                                     uniFact[i].jobList[t].name);
                }

                YVariance++;
            }
        }
        //cText.CreateText(DrawXPos,DrawYPos+(YVariance*10),11,sf::Color::Yellow,AddString(JobList[i].Name," "),JobList[i].pItem->name); // ,JobList.at(i).pItem.name
        //std::cout << JobList[i].pItem->name << std::endl;
    }
}

void removeJobs(std::vector<Job> &JobList)
{
    //debug("Removing Jobs")
    bool Done = false;
    while (Done == false)
    {
        bool Yet = false;
        for (size_t i = 0; i != JobList.size(); i++)
        {
            if (JobList.at(i).toDelete == true)
            {
                JobList.erase(JobList.begin() + i);
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
