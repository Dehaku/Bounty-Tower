#ifndef __FACTIONJOBSECURITY_H_INCLUDED__
#define __FACTIONJOBSECURITY_H_INCLUDED__

#include "Items.h"
#include "JobManager.h"

void rmbMenuItem(Item &pItem);
void digWall(Vec3 pos);
void rmbMenuTile(Vec3 pos);
void drawJobList(int x, int y);
void removeJobs(std::list<Job> &jobList);

#endif // FACTIONJOBSECURITY_H_INCLUDED
