#ifndef __FACTIONJOBSECURITY_H_INCLUDED__
#define __FACTIONJOBSECURITY_H_INCLUDED__

#include "Items.h"
#include "JobManager.h"

void rmbMenuItem(Item &pItem);
void digWall(sf::Vector2f pos);
void rmbMenuTile(sf::Vector2f pos);
void drawJobList(int x, int y);
void removeJobs(std::vector<Job> &jobList);

#endif // FACTIONJOBSECURITY_H_INCLUDED
