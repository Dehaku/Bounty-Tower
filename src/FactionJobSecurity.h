#ifndef __FACTIONJOBSECURITY_H_INCLUDED__
#define __FACTIONJOBSECURITY_H_INCLUDED__

#include "Items.h"
#include "JobManager.h"

void rmbMenuItem(Item &pItem);
void digWall(sf::Vector2f Pos);
void rmbMenuTile(sf::Vector2f Pos);
void drawJobList(int DrawXPos, int DrawYPos);
void removeJobs(std::vector<Job> &JobList);

#endif // FACTIONJOBSECURITY_H_INCLUDED
