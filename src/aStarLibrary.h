#ifndef ASTARLIBRARY_H_INCLUDED
#define ASTARLIBRARY_H_INCLUDED

//#include "keys.h"
#include <iostream>

const int AStarmapWidth = 100, mapHeight = 100, tileSize = 20, numberPeople = 3;
const int notfinished = 0, notStarted = 0; // path-related constants
const int AStarFound = 1, nonexistent = 2;
const int walkable = 0, unwalkable = 1; // walkability array constants

extern int onClosedList;

extern char walkability[AStarmapWidth][mapHeight];
//1 dimensional array holding ID# of open list items
extern int openList[AStarmapWidth * mapHeight + 2];
// 2 dimensional array used to record
// whether a cell is on the open list or on the closed list.
extern int whichList[AStarmapWidth + 1][mapHeight + 1];

//1d array stores the x location of an item on the open list
extern int openX[AStarmapWidth * mapHeight + 2];
//1d array stores the y location of an item on the open list
extern int openY[AStarmapWidth * mapHeight + 2];
//2d array to store parent of each cell (x)
extern int parentX[AStarmapWidth + 1][mapHeight + 1];
//2d array to store parent of each cell (y)
extern int parentY[AStarmapWidth + 1][mapHeight + 1];
//1d array to store F cost of a cell on the open list
extern int Fcost[AStarmapWidth * mapHeight + 2];
//2d array to store G cost for each cell.
extern int Gcost[AStarmapWidth + 1][mapHeight + 1];
//1d array to store H cost of a cell on the open list
extern int Hcost[AStarmapWidth * mapHeight + 2];
//stores length of the AStarFound path for critter
extern int pathLength[numberPeople + 1];
//stores current position along the chosen path for critter
extern int pathLocation[numberPeople + 1];
extern int *pathBank[numberPeople + 1];

//Path reading variables
extern int pathStatus[numberPeople + 1];
extern int xPath[numberPeople + 1];
extern int yPath[numberPeople + 1];

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

#endif // ASTARLIBRARY_H_INCLUDED
