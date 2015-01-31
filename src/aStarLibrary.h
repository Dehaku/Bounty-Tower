#ifndef ASTARLIBRARY_H_INCLUDED
#define ASTARLIBRARY_H_INCLUDED

namespace astar
{
const int AStarmapWidth = 100, mapHeight = 100, tileSize = 20, numberPeople = 3;
const int notfinished = 0, notStarted = 0; // path-related constants
const int AStarFound = 1, nonexistent = 2;
const int walkable = 0, unwalkable = 1; // walkability array constants

extern char walkability[AStarmapWidth][mapHeight];
//stores length of the AStarFound path for critter
extern int pathLength[numberPeople + 1];
//stores current position along the chosen path for critter
extern int pathLocation[numberPeople + 1];
extern int *pathBank[numberPeople + 1];

//-----------------------------------------------------------------------------
// Name: InitializePathfinder
// Desc: Allocates memory for the pathfinder.
//-----------------------------------------------------------------------------
void InitializePathfinder(void);

//-----------------------------------------------------------------------------
// Name: EndPathfinder
// Desc: Frees memory used by the pathfinder.
//-----------------------------------------------------------------------------
void EndPathfinder(void);

//-----------------------------------------------------------------------------
// Name: FindPath
// Desc: Finds a path using A*
//-----------------------------------------------------------------------------
int FindPath(int pathfinderID, int startingX, int startingY, int targetX,
             int targetY);

//==========================================================
//READ PATH DATA: These functions read the path data and convert
//it to screen pixel coordinates.
void ReadPath(int pathfinderID, int currentX, int currentY, int pixelsPerFrame);

//The following two functions read the raw path data from the pathBank.
//You can call these functions directly and skip the readPath function
//above if you want. Make sure you know what your current pathLocation
//is.

//-----------------------------------------------------------------------------
// Name: ReadPathX
// Desc: Reads the x coordinate of the next path step
//-----------------------------------------------------------------------------
int ReadPathX(int pathfinderID, int pathLocation);

//-----------------------------------------------------------------------------
// Name: ReadPathY
// Desc: Reads the y coordinate of the next path step
//-----------------------------------------------------------------------------
int ReadPathY(int pathfinderID, int pathLocation);
}

#endif // ASTARLIBRARY_H_INCLUDED
