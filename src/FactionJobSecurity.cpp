#include "FactionJobSecurity.h"

#include "Faction.h"
#include "Tiles.h"
#include "util.h"
#include "globalvars.h"

#include <iostream>

using std::abs;

template <typename T> T &listAt(std::list<T> &list, size_t index)
{
    auto it = list.begin();
    std::advance(it, index);
    return *it;
}

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

void rmbMenuTile(Vec3 pos)
{
    for (auto &i : uniFact)
    {
        if (i.playerControlled)
        {
            Job job;
            //job.pItem = &pItem;
            //globals::groundmap[abs_to_index(Pos.x/20)][abs_to_index(Pos.y/20)]
            if (tiles[abs_to_index(pos.x / 20)][abs_to_index(pos.y / 20)][abs_to_index(pos.z / 20)]
                    .id != 1010)
            {
                job.name = "BuildWoodWall";
                job.type = "Build";
                job.workPos.x = (abs(pos.x / 20) * 20) + 10;
                job.workPos.y = (abs(pos.y / 20) * 20) + 10;
                job.workPos.z = (abs(pos.z / 20) * 20) + 10;
            }
            else
            {
                return;
            }

            i.jobList.push_back(job);
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


                sf::Vector2f pos(t.workPos.x,t.workPos.y);
                effects.createSquare(pos.x-5,pos.y-5,pos.x+5,pos.y+5,sf::Color::Transparent,1,sf::Color::Cyan);
                textList.createText(pos.x-(t.name.size()*3),pos.y+10,10,sf::Color::Cyan,t.name);
                textList.createText(pos.x-(t.errorReason.size()*3),pos.y+20,10,sf::Color::Red,t.errorReason);

                YVariance++;
            }
        }
        //cText.CreateText(DrawXPos,DrawYPos+(YVariance*10),11,sf::Color::Yellow,AddString(JobList[i].Name," "),JobList[i].pItem->name); // ,JobList.at(i).pItem.name
        //std::cout << JobList[i].pItem->name << std::endl;
    }
}

void removeJobs(std::list<Job> &jobList)
{
    //debug("Removing Jobs")
    bool done = false;
    while (done == false)
    {
        bool yet = false;
        std::list<Job>::iterator Iter = jobList.begin();
        for (size_t i = 0; i != jobList.size(); i++)
        {

            if (listAt(jobList,i).toDelete == true)
            {
                jobList.erase(Iter);
                yet = true;
                break;
            }
            Iter++;
        }
        if (yet == false)
        {
            done = true;
        }
    }
}
