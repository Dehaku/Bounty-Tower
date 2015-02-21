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
    for (auto &i : uniFact)
    {
        if (i.playerControlled)
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

            i.jobList.push_back(job);
        }
    }
}

void digWall(sf::Vector2f pos)
{
    for (auto &i : uniFact)
    {
        if (i.playerControlled)
        {
            Job job;
            std::cout << "Tile clicked: "
                      << tiles[abs_to_index(pos.x / 20)][abs_to_index(
                             pos.y / 20)][6].id << std::endl;
            if (tiles[abs_to_index(pos.x / 20)][abs_to_index(pos.y / 20)][30]
                    .id == 1008)
            {
                job.name = "DigNaturalWall";
                job.type = "Dig";
                job.workPos.x = (abs(pos.x / 20) * 20) + 10;
                job.workPos.y = (abs(pos.y / 20) * 20) + 10;
            }
            else
            {

                return;
            }

            i.jobList.push_back(job);
        }
    }
}

void rmbMenuTile(sf::Vector2f pos)
{
    for (auto &i : uniFact)
    {
        if (i.playerControlled)
        {
            Job job;
            //job.pItem = &pItem;
            //globals::groundmap[abs_to_index(Pos.x/20)][abs_to_index(Pos.y/20)]
            if (tiles[abs_to_index(pos.x / 20)][abs_to_index(pos.y / 20)][30]
                    .id != 1010)
            {
                job.name = "BuildWoodWall";
                job.type = "Build";
                job.workPos.x = (abs(pos.x / 20) * 20) + 10;
                job.workPos.y = (abs(pos.y / 20) * 20) + 10;
            }
            else
            {
                return;
            }

            i.jobList.push_back(job);
            std::cout << "Inserted job into " << i.name << std::endl;
        }
    }
}

void drawJobList(int x, int y)
{
    int YVariance = 1;
    for (auto &i : uniFact)
    {
        if (i.playerControlled)
        {
            for (auto &t : i.jobList)
            {

                //cText.CreateText(DrawXPos,DrawYPos+(YVariance*10),11,sf::Color::Yellow,AddString(UniFact[i].JobList[t].Type," "),UniFact[i].JobList[t].Name);

                if (t.pItem != nullptr &&
                    t.pWorker != nullptr)
                {
                    debug("First condition");
                    debug("CritterName");
                    debug("ItemName");

                    textList.createText(
                        x, y + (YVariance * 10), 11, sf::Color::Yellow,
                        t.pWorker->name +
                            t.name + " ",
                        t.pItem->name); // ,JobList.at(i).pItem.name
                }
                else if (t.pItem != nullptr)
                {
                    debug("Second condition");
                    textList.createText(
                        x, y + (YVariance * 10), 11, sf::Color::Yellow,
                        t.name + " ",
                        t.pItem->name); // ,JobList.at(i).pItem.name
                }
                else if (t.pWorker != nullptr)
                {
                    debug("Third condition");
                    textList.createText(x, y + (YVariance * 10), 11,
                                        sf::Color::Yellow,
                                        t.pWorker->name +
                                            t.type + " ",
                                        t.name);
                }
                else
                {
                    debug("Fourth condition");
                    textList.createText(x, y + (YVariance * 10), 11,
                                        sf::Color::Yellow,
                                        t.type + " ",
                                        t.name);
                }

                YVariance++;
            }
        }
        //cText.CreateText(DrawXPos,DrawYPos+(YVariance*10),11,sf::Color::Yellow,AddString(JobList[i].Name," "),JobList[i].pItem->name); // ,JobList.at(i).pItem.name
        //std::cout << JobList[i].pItem->name << std::endl;
    }
}

void removeJobs(std::vector<Job> &jobList)
{
    //debug("Removing Jobs")
    bool done = false;
    while (done == false)
    {
        bool yet = false;
        for (size_t i = 0; i != jobList.size(); i++)
        {
            if (jobList.at(i).toDelete == true)
            {
                jobList.erase(jobList.begin() + i);
                yet = true;
                break;
            }
        }
        if (yet == false)
        {
            done = true;
        }
    }
}
