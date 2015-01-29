#ifndef __FACTIONJOBSECURITY_H_INCLUDED__
#define __FACTIONJOBSECURITY_H_INCLUDED__

#include "Items.h"
#include "JobManager.h"

void RMBMenuItem(item &pItem);
void DigWall(sf::Vector2f Pos);
void RMBMenuTile(sf::Vector2f Pos);
void DrawJobList(int DrawXPos, int DrawYPos);
void RemoveJobs(std::vector<Job> &JobList);

#endif // FACTIONJOBSECURITY_H_INCLUDED
