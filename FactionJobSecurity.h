#ifndef __FACTIONJOBSECURITY_H_INCLUDED__
#define __FACTIONJOBSECURITY_H_INCLUDED__

// *#include "Faction.h"
// *#include "JobManager.h"
// *#include "Tiles.h"

#include "Game.h"





void RMBMenuItem(item &pItem){
    /*
    std::cout << pItem.name << std::endl;
    Job job;
    job.Name = "PickUpItem";
    job.Type = "PickUp";
    job.pItem = &pItem;
    JobList.push_back(job);
    */
    for(int i = 0; i != UniFact.size(); i++)
    {
        if(UniFact[i].PlayerControlled)
        {
            Job job;
            job.pItem = &pItem;
            if(job.pItem->name == "Fruit Tree")
            {
                job.Name = "ChopDownTree";
                job.Type = "Chop";
            }
            else
            {
                job.Name = "PickUpItem";
                job.Type = "PickUp";
            }

            UniFact[i].JobList.push_back(job);
        }
    }
}

void DigWall(sf::Vector2f Pos)
{
    for(int i = 0; i != UniFact.size(); i++)
    {
        if(UniFact[i].PlayerControlled)
        {
            Job job;
            std::cout << "Tile clicked: " << Tiles[abs(Pos.x/20)][abs(Pos.y/20)][6].ID << std::endl;
            if(Tiles[abs(Pos.x/20)][abs(Pos.y/20)][30].ID == 1008)
            {
                job.Name = "DigNaturalWall";
                job.Type = "Dig";
                job.WorkPos.x = (abs(Pos.x/20)*20)+10;
                job.WorkPos.y = (abs(Pos.y/20)*20)+10;

            }
            else
            {

                return;
            }


            UniFact[i].JobList.push_back(job);

        }
    }
}


void RMBMenuTile(sf::Vector2f Pos)
{
    for(int i = 0; i != UniFact.size(); i++)
    {
        if(UniFact[i].PlayerControlled)
        {
            Job job;
            //job.pItem = &pItem;
            //groundmap[abs(Pos.x/20)][abs(Pos.y/20)]
            Con("Dafuqe \n");
            if(Tiles[abs(Pos.x/20)][abs(Pos.y/20)][30].ID != 1010)
            {
                job.Name = "BuildWoodWall";
                job.Type = "Build";
                job.WorkPos.x = (abs(Pos.x/20)*20)+10;
                job.WorkPos.y = (abs(Pos.y/20)*20)+10;
                Con("Dafuqe2 \n");
            }
            else
            {
                Con("Dafuqe3 \n");
                return;
            }

            Con("Dafuqe4 \n");
            UniFact[i].JobList.push_back(job);
            Con("Dafuqe5 \n");
        }
    }
}




void DrawJobList(int DrawXPos, int DrawYPos)
{
    int YVariance = 1;
    for(int i = 0; i != UniFact.size(); i++)
    {
        if(UniFact[i].PlayerControlled)
        {
            for(int t = 0; t != UniFact[i].JobList.size(); t++)
            {

                //cText.CreateText(DrawXPos,DrawYPos+(YVariance*10),11,sf::Color::Yellow,AddString(UniFact[i].JobList[t].Type," "),UniFact[i].JobList[t].Name);

                if(UniFact[i].JobList[t].pItem != NULL && UniFact[i].JobList[t].pWorker != NULL)
                {
                    debug("First condition");
                    std::string Name = UniFact[i].JobList[t].pWorker->name;
                    debug("CritterName");
                    Name = UniFact[i].JobList[t].pItem->name;
                    debug("ItemName");

                    cText.CreateText(DrawXPos,DrawYPos+(YVariance*10),11,sf::Color::Yellow,AddString(UniFact[i].JobList[t].pWorker->name, UniFact[i].JobList[t].Name," "),UniFact[i].JobList[t].pItem->name); // ,JobList.at(i).pItem.name
                }
                else if(UniFact[i].JobList[t].pItem != NULL)
                {
                    debug("Second condition");
                    cText.CreateText(DrawXPos,DrawYPos+(YVariance*10),11,sf::Color::Yellow,AddString(UniFact[i].JobList[t].Name," "),UniFact[i].JobList[t].pItem->name); // ,JobList.at(i).pItem.name
                }
                else if(UniFact[i].JobList[t].pWorker != NULL)
                {
                    debug("Third condition");
                    cText.CreateText(DrawXPos,DrawYPos+(YVariance*10),11,sf::Color::Yellow,AddString(UniFact[i].JobList[t].pWorker->name,UniFact[i].JobList[t].Type," "),UniFact[i].JobList[t].Name);
                }
                else
                {
                    debug("Fourth condition");
                    cText.CreateText(DrawXPos,DrawYPos+(YVariance*10),11,sf::Color::Yellow,AddString(UniFact[i].JobList[t].Type," "),UniFact[i].JobList[t].Name);
                }


                YVariance++;
            }
        }
        //cText.CreateText(DrawXPos,DrawYPos+(YVariance*10),11,sf::Color::Yellow,AddString(JobList[i].Name," "),JobList[i].pItem->name); // ,JobList.at(i).pItem.name
        //std::cout << JobList[i].pItem->name << std::endl;
    }
}



void RemoveJobs(std::vector<Job> &JobList)
{
    //debug("Removing Jobs")
    bool Done = false;
    while(Done == false)
    {
        bool Yet = false;
        for(int i = 0; i != JobList.size(); i++)
        {
            if(JobList.at(i).ToDelete == true)
            {
                JobList.erase(JobList.begin()+i);
                Yet = true;
                break;
            }
        }
        if(Yet == false)
        {
            Done = true;
        }
    }
}



#endif // FACTIONJOBSECURITY_H_INCLUDED
