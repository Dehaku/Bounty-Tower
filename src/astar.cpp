#include "astar.h"

#include <stdexcept>
#include <cstdlib>

namespace astar
{
int onClosedList = 10;
char walkability[MAP_WIDTH][MAP_HEIGHT];
//1 dimensional array holding ID# of open list items
int openList[MAP_WIDTH * MAP_HEIGHT + 2];
// 2 dimensional array used to record
// whether a cell is on the open list or on the closed list.
int whichList[MAP_WIDTH + 1][MAP_HEIGHT + 1];
//1d array stores the x location of an item on the open list
int openX[MAP_WIDTH * MAP_HEIGHT + 2];
//1d array stores the y location of an item on the open list
int openY[MAP_WIDTH * MAP_HEIGHT + 2];
//2d array to store parent of each cell (x)
int parentX[MAP_WIDTH + 1][MAP_HEIGHT + 1];
//2d array to store parent of each cell (y)
int parentY[MAP_WIDTH + 1][MAP_HEIGHT + 1];
//1d array to store F cost of a cell on the open list
int fcost[MAP_WIDTH * MAP_HEIGHT + 2];
//2d array to store G cost for each cell.
int gcost[MAP_WIDTH + 1][MAP_HEIGHT + 1];
//1d array to store H cost of a cell on the open list
int hcost[MAP_WIDTH * MAP_HEIGHT + 2];
int pathLength[NUMBER_OF_PEOPLE + 1];
int pathLocation[NUMBER_OF_PEOPLE + 1];
int *pathBank[NUMBER_OF_PEOPLE + 1];
//Path reading variables
int pathStatus[NUMBER_OF_PEOPLE + 1];
int xPath[NUMBER_OF_PEOPLE + 1];
int yPath[NUMBER_OF_PEOPLE + 1];

void init()
{
    for (auto &elem : pathBank)
        elem = (int *)malloc(4);
}

void end()
{
    for (auto &elem : pathBank)
    {
        free(elem);
    }
}

int findPath(int pathfinderID, int startingX, int startingY, int targetX,
             int targetY)
{
    //* std::cout << "Path One";
    int onOpenList = 0, parentXval = 0, parentYval = 0, a = 0, b = 0, m = 0,
        u = 0, v = 0, temp = 0, corner = 0, numberOfOpenListItems = 0,
        addedGCost = 0, tempGcost = 0, path = 0, tempx, pathX, pathY,
        cellPosition, newOpenListItemID = 0;

    //1. Convert location data (in pixels) to coordinates in the walkability array.
    int startX = startingX / TILE_SIZE;
    int startY = startingY / TILE_SIZE;
    targetX = targetX / TILE_SIZE;
    targetY = targetY / TILE_SIZE;

    //2.Quick Path Checks: Under the some circumstances no path needs to
    //	be generated ...

    //	If starting location and target are in the same location...
    if (startX == targetX && startY == targetY &&
        pathLocation[pathfinderID] > 0)
        return FOUND;
    if (startX == targetX && startY == targetY &&
        pathLocation[pathfinderID] == 0)
        return NONEXISTENT;

    //	If target square is unwalkable, return that it's a nonexistent path.
    if (walkability[targetX][targetY] == UNWALKABLE)
        goto noPath;
    //* std::cout << "Path Two";
    //3.Reset some variables that need to be cleared
    if (onClosedList > 1000000) //reset whichList occasionally
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            for (int y = 0; y < MAP_HEIGHT; y++)
                whichList[x][y] = 0;
        }
        onClosedList = 10;
    }
    onClosedList =
        onClosedList +
        2; //changing the values of onOpenList and onClosed list is faster than redimming whichList() array
    onOpenList = onClosedList - 1;
    pathLength[pathfinderID] = NOT_STARTED;   //i.e, = 0
    pathLocation[pathfinderID] = NOT_STARTED; //i.e, = 0
    gcost[startX][startY] = 0; //reset starting square's G value to 0

    //4.Add the starting location to the open list of squares to be checked.
    numberOfOpenListItems = 1;
    openList[1] =
        1; //assign it as the top (and currently only) item in the open list, which is maintained as a binary heap (explained below)
    openX[1] = startX;
    openY[1] = startY;

    //5.Do the following until a path is AStarFound or deemed nonexistent.
    //* std::cout << "Path Three";
    do
    {

        //6.If the open list is not empty, take the first cell off of the list.
        //	This is the lowest F cost cell on the open list.

        if (numberOfOpenListItems != 0)
        {

            //7. Pop the first item off the open list.
            parentXval = openX[openList[1]];
            parentYval =
                openY[openList[1]]; //record cell coordinates of the item
            whichList[parentXval][parentYval] =
                onClosedList; //add the item to the closed list

            //	Open List = Binary Heap: Delete this item from the open list, which
            //  is maintained as a binary heap. For more information on binary heaps, see:
            //	http://www.policyalmanac.org/games/binaryHeaps.htm
            numberOfOpenListItems = numberOfOpenListItems -
                                    1; //reduce number of open list items by 1

            //	Delete the top item in binary heap and reorder the heap, with the lowest F cost item rising to the top.
            openList[1] =
                openList[numberOfOpenListItems +
                         1]; //move the last item in the heap up to slot #1
            v = 1;

            //	Repeat the following until the new item in slot #1 sinks to its proper spot in the heap.
            do
            {
                u = v;
                if (2 * u + 1 <= numberOfOpenListItems) //if both children exist
                {

                    //Check if the F cost of the parent is greater than each child.
                    //Select the lowest of the two children.
                    if (fcost[openList[u]] >= fcost[openList[2 * u]])
                        v = 2 * u;
                    if (fcost[openList[v]] >= fcost[openList[2 * u + 1]])
                        v = 2 * u + 1;
                }
                else
                {
                    if (2 * u <=
                        numberOfOpenListItems) //if only child #1 exists
                    {
                        //Check if the F cost of the parent is greater than child #1
                        if (fcost[openList[u]] >= fcost[openList[2 * u]])
                            v = 2 * u;
                    }
                }

                if (u != v) //if parent's F is > one of its children, swap them
                {
                    temp = openList[u];
                    openList[u] = openList[v];
                    openList[v] = temp;
                }
                else
                    break; //otherwise, exit loop

            } while (false == false); //(!Key.p);//reorder the binary heap

            //7.Check the adjacent squares. (Its "children" -- these path children
            //	are similar, conceptually, to the binary heap children mentioned
            //	above, but don't confuse them. They are different. Path children
            //	are portrayed in Demo 1 with grey pointers pointing toward
            //	their parents.) Add these adjacent child squares to the open list
            //	for later consideration if appropriate (see various if statements
            //	below).

            for (b = parentYval - 1; b <= parentYval + 1; b++)
            {
                for (a = parentXval - 1; a <= parentXval + 1; a++)
                {

                    //	If not off the map (do this first to avoid array out-of-bounds errors)
                    if (a != -1 && b != -1 && a != MAP_WIDTH && b != MAP_HEIGHT)
                    {

                        //	If not already on the closed list (items on the closed list have
                        //	already been considered and can now be ignored).
                        if (whichList[a][b] != onClosedList)
                        {

                            //	If not a wall/obstacle square.
                            if (walkability[a][b] != UNWALKABLE)
                            {

                                //	Don't cut across corners
                                corner = WALKABLE;
                                if (a == parentXval - 1)
                                {
                                    if (b == parentYval - 1)
                                    {
                                        if (walkability[parentXval -
                                                        1][parentYval] ==
                                                UNWALKABLE ||
                                            walkability[parentXval][parentYval -
                                                                    1] ==
                                                UNWALKABLE)
                                            corner = UNWALKABLE;
                                    }
                                    else if (b == parentYval + 1)
                                    {
                                        if (walkability[parentXval][parentYval +
                                                                    1] ==
                                                UNWALKABLE ||
                                            walkability[parentXval -
                                                        1][parentYval] ==
                                                UNWALKABLE)
                                            corner = UNWALKABLE;
                                    }
                                }
                                else if (a == parentXval + 1)
                                {
                                    if (b == parentYval - 1)
                                    {
                                        if (walkability[parentXval][parentYval -
                                                                    1] ==
                                                UNWALKABLE ||
                                            walkability[parentXval +
                                                        1][parentYval] ==
                                                UNWALKABLE)
                                            corner = UNWALKABLE;
                                    }
                                    else if (b == parentYval + 1)
                                    {
                                        if (walkability[parentXval +
                                                        1][parentYval] ==
                                                UNWALKABLE ||
                                            walkability[parentXval][parentYval +
                                                                    1] ==
                                                UNWALKABLE)
                                            corner = UNWALKABLE;
                                    }
                                }
                                if (corner == WALKABLE)
                                {

                                    //	If not already on the open list, add it to the open list.
                                    if (whichList[a][b] != onOpenList)
                                    {

                                        //Create a new open list item in the binary heap.
                                        newOpenListItemID =
                                            newOpenListItemID +
                                            1; //each new item has a unique ID #
                                        m = numberOfOpenListItems + 1;
                                        openList[m] =
                                            newOpenListItemID; //place the new open list item (actually, its ID#) at the bottom of the heap
                                        openX[newOpenListItemID] = a;
                                        openY[newOpenListItemID] =
                                            b; //record the x and y coordinates of the new item

                                        //Figure out its G cost
                                        if (abs(a - parentXval) == 1 &&
                                            abs(b - parentYval) == 1)
                                            addedGCost =
                                                14; //cost of going to diagonal squares
                                        else
                                            addedGCost =
                                                10; //cost of going to non-diagonal squares
                                        gcost[a][b] =
                                            gcost[parentXval][parentYval] +
                                            addedGCost;

                                        //Figure out its H and F costs and parent
                                        hcost[openList[m]] =
                                            10 * (abs(a - targetX) +
                                                  abs(b - targetY));
                                        fcost[openList[m]] =
                                            gcost[a][b] + hcost[openList[m]];
                                        parentX[a][b] = parentXval;
                                        parentY[a][b] = parentYval;

                                        //Move the new open list item to the proper place in the binary heap.
                                        //Starting at the bottom, successively compare to parent items,
                                        //swapping as needed until the item finds its place in the heap
                                        //or bubbles all the way to the top (if it has the lowest F cost).
                                        while (
                                            m !=
                                            1) //While item hasn't bubbled to the top (m=1)
                                        {
                                            //Check if child's F cost is < parent's F cost. If so, swap them.
                                            if (fcost[openList[m]] <=
                                                fcost[openList[m / 2]])
                                            {
                                                temp = openList[m / 2];
                                                openList[m / 2] = openList[m];
                                                openList[m] = temp;
                                                m = m / 2;
                                            }
                                            else
                                                break;
                                        }
                                        numberOfOpenListItems =
                                            numberOfOpenListItems +
                                            1; //add one to the number of items in the heap

                                        //Change whichList to show that the new item is on the open list.
                                        whichList[a][b] = onOpenList;
                                    }

                                    //8.If adjacent cell is already on the open list, check to see if this
                                    //	path to that cell from the starting location is a better one.
                                    //	If so, change the parent of the cell and its G and F costs.
                                    else //If whichList(a,b) = onOpenList
                                    {

                                        //Figure out the G cost of this possible new path
                                        if (abs(a - parentXval) == 1 &&
                                            abs(b - parentYval) == 1)
                                            addedGCost =
                                                14; //cost of going to diagonal tiles
                                        else
                                            addedGCost =
                                                10; //cost of going to non-diagonal tiles
                                        tempGcost =
                                            gcost[parentXval][parentYval] +
                                            addedGCost;

                                        //If this path is shorter (G cost is lower) then change
                                        //the parent cell, G cost and F cost.
                                        if (tempGcost <
                                            gcost[a][b]) //if G cost is less,
                                        {
                                            parentX[a][b] =
                                                parentXval; //change the square's parent
                                            parentY[a][b] = parentYval;
                                            gcost[a][b] =
                                                tempGcost; //change the G cost

                                            //Because changing the G cost also changes the F cost, if
                                            //the item is on the open list we need to change the item's
                                            //recorded F cost and its position on the open list to make
                                            //sure that we maintain a properly ordered open list.
                                            for (
                                                int x = 1;
                                                x <= numberOfOpenListItems;
                                                x++) //look for the item in the heap
                                            {
                                                if (openX[openList[x]] == a &&
                                                    openY[openList[x]] ==
                                                        b) //item AStarFound
                                                {
                                                    fcost[openList[x]] =
                                                        gcost[a][b] +
                                                        hcost
                                                            [openList
                                                                 [x]]; //change the F cost

                                                    //See if changing the F score bubbles the item up from it's current location in the heap
                                                    m = x;
                                                    while (
                                                        m !=
                                                        1) //While item hasn't bubbled to the top (m=1)
                                                    {
                                                        //Check if child is < parent. If so, swap them.
                                                        if (fcost[openList[m]] <
                                                            fcost[openList[m /
                                                                           2]])
                                                        {
                                                            temp =
                                                                openList[m / 2];
                                                            openList[m / 2] =
                                                                openList[m];
                                                            openList[m] = temp;
                                                            m = m / 2;
                                                        }
                                                        else
                                                            break;
                                                    }
                                                    break; //exit for x = loop
                                                } //If openX(openList(x)) = a
                                            } //For x = 1 To numberOfOpenListItems
                                        }     //If tempGcost < Gcost(a,b)

                                    } //else If whichList(a,b) = onOpenList
                                }     //If not cutting a corner
                            }         //If not a wall/obstacle square.
                        }             //If not already on the closed list
                    }                 //If not off the map
                } //for (a = parentXval-1; a <= parentXval+1; a++){
            }     //for (b = parentYval-1; b <= parentYval+1; b++){

        } //if (numberOfOpenListItems != 0)

        //9.If open list is empty then there is no path.
        else
        {
            path = NONEXISTENT;
            break;
        }

        //If target is added to open list then path has been AStarFound.
        if (whichList[targetX][targetY] == onOpenList)
        {
            path = FOUND;
            break;
        }

    } while (1); //Do until path is AStarFound or deemed nonexistent
    //* std::cout << "Path Ender";

    //10.Save the path if it exists.
    if (path == FOUND)
    {
        //* std::cout << "Path AStarFound";

        //a.Working backwards from the target to the starting location by checking
        //	each cell's parent, figure out the length of the path.
        pathX = targetX;
        pathY = targetY;
        do
        {
            //Look up the parent of the current cell.
            tempx = parentX[pathX][pathY];
            pathY = parentY[pathX][pathY];
            pathX = tempx;

            //Figure out the path length
            //* std::cout << "Path Length";
            pathLength[pathfinderID] = pathLength[pathfinderID] + 1;
        } while (pathX != startX || pathY != startY);

        //b.Resize the data bank to the right size in bytes
        pathBank[pathfinderID] =
            (int *)realloc(pathBank[pathfinderID],
                           pathLength[pathfinderID] * 8); // AStarFound IT!

        //c. Now copy the path information over to the databank. Since we are
        //	working backwards from the target to the start location, we copy
        //	the information to the data bank in reverse order. The result is
        //	a properly ordered set of path data, from the first step to the
        //	last.
        pathX = targetX;
        pathY = targetY;
        cellPosition = pathLength[pathfinderID] * 2; //start at the end
        //* std::cout << "Path Doin";
        do
        {
            cellPosition = cellPosition - 2; //work backwards 2 integers
            pathBank[pathfinderID][cellPosition] = pathX;
            pathBank[pathfinderID][cellPosition + 1] = pathY;
            //* std::cout << "Path Bankin";
            //d.Look up the parent of the current cell.
            tempx = parentX[pathX][pathY];
            pathY = parentY[pathX][pathY];
            pathX = tempx;
            //* std::cout << "Path Returning";

            //e.If we have reached the starting square, exit the loop.
        } while (pathX != startX || pathY != startY);

        //11.Read the first path step into xPath/yPath arrays
        readPath(pathfinderID, startingX, startingY, 1);
        //* std::cout << "Path Reading";
    }
    //* std::cout << "returning Path ;" << path << "\n";
    //* std::cout << "Likes to crash here. \n";
    return path;

//13.If there is no path to the selected target, set the pathfinder's
//	xPath and yPath equal to its current location and return that the
//	path is nonexistent.
//* std::cout << "Path None";
noPath:
    //* std::cout << "Path Nonening \n";
    xPath[pathfinderID] = startingX;
    yPath[pathfinderID] = startingY;
    return NONEXISTENT;
}

void readPath(int pathfinderID, int currentX, int currentY, int pixelsPerFrame)
{
    /*
;	Note on PixelsPerFrame: The need for this parameter probably isn't
;	that obvious, so a little explanation is in order. This
;	parameter is used to determine if the pathfinder has gotten close
;	enough to the center of a given path square to warrant looking up
;	the next step on the path.
;
;	This is needed because the speed of certain sprites can
;	make reaching the exact center of a path square impossible.
;	In Demo #2, the chaser has a velocity of 3 pixels per frame. Our
;	tile size is 50 pixels, so the center of a tile will be at location
;	25, 75, 125, etc. Some of these are not evenly divisible by 3, so
;	our pathfinder has to know how close is close enough to the center.
;	It calculates this by seeing if the pathfinder is less than
;	pixelsPerFrame # of pixels from the center of the square.

;	This could conceivably cause problems if you have a *really* fast
;	sprite and/or really small tiles, in which case you may need to
;	adjust the formula a bit. But this should almost never be a problem
;	for games with standard sized tiles and normal speeds. Our smiley
;	in Demo #4 moves at a pretty fast clip and it isn't even close
;	to being a problem.
*/

    int id = pathfinderID; //redundant, but makes the following easier to read

    //If a path has been AStarFound for the pathfinder	...
    if (pathStatus[id] == FOUND)
    {

        //If path finder is just starting a new path or has reached the
        //center of the current path square (and the end of the path
        //hasn't been reached), look up the next path square.
        if (pathLocation[id] < pathLength[id])
        {
            //if just starting or if close enough to center of square
            if (pathLocation[id] == 0 ||
                (abs(currentX - xPath[id]) < pixelsPerFrame &&
                 abs(currentY - yPath[id]) < pixelsPerFrame))
                pathLocation[id] = pathLocation[id] + 1;
        }

        //Read the path data.
        xPath[id] = readPathX(id, pathLocation[id]);
        yPath[id] = readPathY(id, pathLocation[id]);

        //If the center of the last path square on the path has been
        //reached then reset.
        if (pathLocation[id] == pathLength[id])
        {
            if (abs(currentX - xPath[id]) < pixelsPerFrame &&
                abs(currentY - yPath[id]) <
                    pixelsPerFrame) //if close enough to center of square
                pathStatus[id] = NOT_STARTED;
        }
    }

    //If there is no path for this pathfinder, simply stay in the current
    //location.
    else
    {
        xPath[id] = currentX;
        yPath[id] = currentY;
    }
}

int readPathX(int pathfinderID, int pathLocation)
{
    if (pathLocation <= pathLength[pathfinderID])
    {

        //Read coordinate from bank
        int x = pathBank[pathfinderID][pathLocation * 2 - 2];

        //Adjust the coordinates so they align with the center
        //of the path square (optional). This assumes that you are using
        //sprites that are centered -- i.e., with the midHandle command.
        //Otherwise you will want to adjust this.
        x = TILE_SIZE * x + .5 * TILE_SIZE;
        return x;
    }
    throw std::runtime_error("ReadPathX: Couldn't return a meaningful value!");
}

int readPathY(int pathfinderID, int pathLocation)
{
    if (pathLocation <= pathLength[pathfinderID])
    {

        //Read coordinate from bank
        int y = pathBank[pathfinderID][pathLocation * 2 - 1];

        //Adjust the coordinates so they align with the center
        //of the path square (optional). This assumes that you are using
        //sprites that are centered -- i.e., with the midHandle command.
        //Otherwise you will want to adjust this.
        y = TILE_SIZE * y + .5 * TILE_SIZE;
        return y;
    }
    throw std::runtime_error("ReadPathY: Couldn't return a meaningful value!");
}

} // namespace astar
