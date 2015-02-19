#include <cstring>
#ifndef GALAXY_LINUX
#include <windows.h>
#endif
#include "Items.h"
#include "Faction.h"
#include "Tiles.h"
#include "FactionJobSecurity.h"
#include "Galaxy.h"
#include "menus.h"
#include "astar.h"
#include "Textures.h"
#include "math.h"
#include "util.h"
#include "globalvars.h"

#define USE_PATHER

#ifdef USE_PATHER

#include "micropather.h"
using namespace micropather;
#endif

using std::abs;

template <typename T> T &listAt(std::list<T> &list, size_t index)
{
    auto it = list.begin();
    std::advance(it, index);
    return *it;
}


const int worldSizeX = 32;
const int worldSizeY = 32;
const int worldSizeZ = 3;

class cTile
{
public:
    Vec3 position;
    int type;
    bool teleporter;
    Vec3 telePos;
    void setPos(int x, int y, int z)
    {
        position = Vec3(x, y, z);
    }
    Vec3 getPos()
    {
        return position;
    }
    cTile()
    {
        type = 0;
        teleporter = false;
        telePos = Vec3(-1, -1, -1);
    }
};

cTile grid[worldSizeX][worldSizeY][worldSizeZ];

class worldclass
#ifdef USE_PATHER
    : public Graph
#endif
{
private:
    worldclass(const worldclass &);
    void operator=(const worldclass &);

public:
    void NodeToXYZ(void *node, int *x, int *y, int *z)
    {
        cTile *Nodeling = static_cast<cTile *>(node);

        *x = Nodeling->position.x;
        *y = Nodeling->position.y;
        *z = Nodeling->position.z;
    }

    void *XYZToNode(int x, int y, int z)
    {
        return (void *)&(grid[x][y][z]);
    }

    int Passable(int nx, int ny, int nz)
    {
        if (nx >= 0 && nx < worldSizeX && ny >= 0 && ny < worldSizeY &&
            nz >= 0 && nz < worldSizeZ)
        {
            if (grid[nx][ny][nz].type == 0)
                return 1;
            if (grid[nx][ny][nz].type == 2)
                return 2;
            if (grid[nx][ny][nz].type == 3)
                return 3;
            if (grid[nx][ny][nz].type == 10)
                return 1;
        }
        return 0;
    }

    void toggleTeleporter()
    {
        if (grid[5][1][0].type == 10)
        {
            grid[5][1][0].type = 0;
            grid[5][1][0].teleporter = false;
            grid[5][1][0].telePos = Vec3(-1, -1, -1);

            grid[20][30][2].type = 0;
            grid[20][30][2].teleporter = false;
            grid[20][30][2].telePos = Vec3(-1, -1, -1);
        }
        else
        {
            grid[5][1][0].type = 10;
            grid[5][1][0].teleporter = true;
            grid[5][1][0].telePos = Vec3(20, 30, 2);
            grid[20][30][2].type = 10;
            grid[20][30][2].teleporter = true;
            grid[20][30][2].telePos = Vec3(5, 1, 0);
        }
    }

#ifdef USE_PATHER

    MPVector<void *> microPath;
    MicroPather *pather;
    std::vector<cTile *> storedPath;

    worldclass() : pather(0)
    {
        pather = new MicroPather(
            this, 20); // Use a very small memory block to stress the pather
    }

    void drawPath()
    {
        unsigned int k;
        unsigned int pathSize = microPath.size();
        Vec3 oldPos;
        for (k = 0; k < pathSize; ++k)
        {
            Vec3 pathPos;
            NodeToXYZ(microPath[k], &pathPos.x, &pathPos.y, &pathPos.z);
            sf::Color pathColor(0, 0, 0);
            if (pathPos.z == 0)
                pathColor.r = 255;
            if (pathPos.z == 1)
                pathColor.g = 255;
            if (pathPos.z == 2)
                pathColor.b = 255;

            if (k != 0)
                effects.createLine((oldPos.x + 1) * 20 - 10,
                                   (oldPos.y + 1) * 20 - 10,
                                   (pathPos.x + 1) * 20 - 10,
                                   (pathPos.y + 1) * 20 - 10, 5, pathColor);

            oldPos = pathPos;
        }
    }

    void drawStoredPath()
    {
        Vec3 oldPos;
        bool firstRun = true;

        for (auto &i : storedPath)
        {
            Vec3 pathPos;
            pathPos = Vec3(i->getPos());
            sf::Color pathColor(0, 0, 0);
            if (pathPos.z == 0)
                pathColor.r = 255;
            if (pathPos.z == 1)
                pathColor.g = 255;
            if (pathPos.z == 2)
                pathColor.b = 255;

            if (!firstRun)
                effects.createLine((oldPos.x + 1) * 20 - 10,
                                   (oldPos.y + 1) * 20 - 10,
                                   (pathPos.x + 1) * 20 - 10,
                                   (pathPos.y + 1) * 20 - 10, 5, pathColor);

            oldPos = pathPos;
            firstRun = false;
        }
        storedPath.clear();
    }

    void storePath(void *node)
    {
        cTile *Nodeling = static_cast<cTile *>(node);
        storedPath.push_back(Nodeling);
    }

    int makePath(Vec3 Ori, Vec3 Tar)
    {
        int result = 0;
        if (Passable(Tar.x, Tar.y, Tar.z) == 1)
        {
#ifdef USE_PATHER
            float totalCost;

            result = pather->Solve(XYZToNode(Ori.x, Ori.y, Ori.z),
                                   XYZToNode(Tar.x, Tar.y, Tar.z), &microPath,
                                   &totalCost);

            if (result == MicroPather::SOLVED)
            {
                unsigned int pathSize = microPath.size();
                for (int i = 0; i != pathSize; i++)
                {
                    storePath(microPath[i]);
                }
            }

//printf( "Pather returned %d\n", result );

#endif
        }
        return result;
    }

    virtual ~worldclass()
    {
        delete pather;
    }

    virtual float LeastCostEstimate(void *nodeStart, void *nodeEnd)
    {
        int xStart, yStart, zStart, xEnd, yEnd, zEnd;
        NodeToXYZ(nodeStart, &xStart, &yStart, &zStart);
        NodeToXYZ(nodeEnd, &xEnd, &yEnd, &zEnd);

        double d = sqrt(pow(xEnd - xStart, 2) + pow(yEnd - yStart, 2) +
                        pow(zEnd - zStart, 2));
        /* Compute the minimum path cost using distance measurement. It is possible
		   to compute the exact minimum path using the fact that you can move only
		   on a straight line or on a diagonal, and this will yield a better result.
		*/
        return d;
    }

    virtual void AdjacentCost(void *node,
                              micropather::MPVector<StateCost> *neighbors)
    {
        int x, y, z;
        NodeToXYZ(node, &x, &y, &z);
        //const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
        //const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
        //const int dx[26] = { -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1};
        //const int dy[26] = { -1, -1, -1, 0, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, 0, 1, 1, 1};
        //const int dz[26] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1};

        const int dx[10] = {0, -1, 0, 1, -1, 1, -1, 0, 1, 0};
        const int dy[10] = {0, -1, -1, -1, 0, 0, 1, 1, 1, 0};
        const int dz[10] = {-1, 0, 0, 0, 0, 0, 0, 0, 0, 1};

        //const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };
        float One = 1;
        float Two = 1.41f;
        float Three = 1.44f;
        const float cost[10] = {One, Two, One, Two, One,
                                One, Two, One, Two, One};

        cTile *Nodeling = static_cast<cTile *>(node);

        if (Nodeling->teleporter)
        {
            Vec3 N(Nodeling->telePos);
            StateCost nodeCost = {XYZToNode(N.x, N.y, N.z), 3};
            neighbors->push_back(nodeCost);
        }

        for (int i = 0; i < 10; ++i)
        {

            int nx = x + dx[i];
            int ny = y + dy[i];
            int nz = z + dz[i];

            //void* targetTile = XYZToNode(nx,ny,nz);

            int pass = Passable(nx, ny, nz);
            if (pass > 0)
            {
                //std::cout << dz[i];
                if (pass == 1 && dz[i] == 0)
                {
                    // Normal floor
                    StateCost nodeCost = {XYZToNode(nx, ny, nz), cost[i]};
                    neighbors->push_back(nodeCost);
                }
                else if (pass == 3 && dz[i] == -1 && dx[i] == 0 && dy[i] == 0)
                {
                    StateCost nodeCost = {XYZToNode(nx, ny, nz), cost[i]};
                    neighbors->push_back(nodeCost);
                }
                else if (pass == 2 && dz[i] == 1 && dx[i] == 0 && dy[i] == 0)
                {
                    StateCost nodeCost = {XYZToNode(nx, ny, nz), cost[i]};
                    neighbors->push_back(nodeCost);
                }
                else
                {
                    // Normal floor
                    StateCost nodeCost = {XYZToNode(nx, ny, nz), FLT_MAX};
                    neighbors->push_back(nodeCost);
                }
            }
        }
    }

    virtual void
    AdjacentCostPureFlight(void *node,
                           micropather::MPVector<StateCost> *neighbors)
    {
        int x, y, z;
        NodeToXYZ(node, &x, &y, &z);
        //const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
        //const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
        const int dx[26] = {-1, 0,  1, -1, 0,  1, -1, 0,  1, -1, 0,  1, -1,
                            1,  -1, 0, 1,  -1, 0, 1,  -1, 0, 1,  -1, 0, 1};
        const int dy[26] = {-1, -1, -1, 0, 0,  0,  1,  1, 1, -1, -1, -1, 0,
                            0,  1,  1,  1, -1, -1, -1, 0, 0, 0,  1,  1,  1};
        const int dz[26] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0,
                            0,  0,  0,  0,  1,  1,  1,  1,  1,  1, 1, 1, 1};
        //const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };
        float One = 1;
        float Two = 1.41f;
        float Three = 1.44f;
        const float cost[26] = {Three, Two,   Three, Two,   One,  Two,   Three,
                                Two,   Three, Two,   One,   Two,  One,   One,
                                Two,   One,   Two,   Three, Two,  Three, Two,
                                One,   Two,   Three, Two,   Three};

        for (int i = 0; i < 26; ++i)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            int nz = z + dz[i];

            int pass = Passable(nx, ny, nz);
            if (pass > 0)
            {
                if (pass == 1)
                {
                    // Normal floor
                    StateCost nodeCost = {XYZToNode(nx, ny, nz), cost[i]};
                    neighbors->push_back(nodeCost);
                }
                else
                {
                    // Normal floor
                    StateCost nodeCost = {XYZToNode(nx, ny, nz), FLT_MAX};
                    neighbors->push_back(nodeCost);
                }
            }
        }
    }

    virtual void PrintStateInfo(void *node)
    {
        int x, y, z;
        NodeToXYZ(node, &x, &y, &z);
        printf("(%d,%d,%d)", x, y, &z);
    }

#endif
};
worldclass world;

class PathingController
#ifdef USE_PATHER
    : public Graph
#endif
{
private:
    PathingController(const PathingController &);
    void operator=(const PathingController &);

public:
    void NodeToXYZ(void *node, int *x, int *y, int *z)
    {
        Tile *Nodeling = static_cast<Tile *>(node);

        *x = Nodeling->pos.x;
        *y = Nodeling->pos.y;
        *z = Nodeling->pos.z;
    }

    void *XYZToNode(int x, int y, int z)
    {
        return (void *)&(tiles[x][y][z]);
    }

    int Passable(int nx, int ny, int nz)
    {
        if (nx >= 0 && nx < GRIDS && ny >= 0 && ny < GRIDS &&
            nz >= 0 && nz < GRIDS)
        {

            if (tiles[nx][ny][nz].goesDown && tiles[nx][ny][nz].goesUp)
                return 4;
            if (tiles[nx][ny][nz].goesDown)
                return 3; // return 2;
            if (tiles[nx][ny][nz].goesUp)
                return 2; // return 3;
            if (tiles[nx][ny][nz].walkable)
                return 1;
        }
        return 0;
    }

#ifdef USE_PATHER

    MPVector<void *> microPath;
    MicroPather *pather;
    std::vector<Tile *> storedPath;
    std::vector<Tile *> storedRPath;

    PathingController() : pather(0)
    {
        pather = new MicroPather(
            this, 20); // Use a very small memory block to stress the pather
    }

    void drawPath()
    {
        unsigned int k;
        unsigned int pathSize = microPath.size();
        Vec3 oldPos;
        for (k = 0; k < pathSize; ++k)
        {
            Vec3 pathPos;
            NodeToXYZ(microPath[k], &pathPos.x, &pathPos.y, &pathPos.z);
            sf::Color pathColor(0, 0, 0);
            if (pathPos.z == 0)
                pathColor.r = 255;
            if (pathPos.z == 1)
                pathColor.g = 255;
            if (pathPos.z == 2)
                pathColor.b = 255;

            if (k != 0)
                effects.createLine((oldPos.x + 1) * 20 - 10,
                                   (oldPos.y + 1) * 20 - 10,
                                   (pathPos.x + 1) * 20 - 10,
                                   (pathPos.y + 1) * 20 - 10, 5, pathColor);

            oldPos = pathPos;
        }
    }

    void drawStoredPath()
    {
        Vec3 oldPos;
        bool firstRun = true;

        for (auto &i : storedPath)
        {
            Vec3 pathPos;
            pathPos = Vec3(i->getPos());
            sf::Color pathColor(0, 0, 0);
            if (pathPos.z == 0)
                pathColor.r = 255;
            if (pathPos.z == 1)
                pathColor.g = 255;
            if (pathPos.z == 2)
                pathColor.b = 255;

            if (!firstRun)
                effects.createLine((oldPos.x + 1) * 20 - 10,
                                   (oldPos.y + 1) * 20 - 10,
                                   (pathPos.x + 1) * 20 - 10,
                                   (pathPos.y + 1) * 20 - 10, 5, pathColor);

            oldPos = pathPos;
            firstRun = false;
        }

    }

    void drawStoredPathRainbow()
    {
        Vec3 oldPos;
        bool firstRun = true;

        for (auto &i : storedRPath)
        {
            Vec3 pathPos;
            pathPos = Vec3(i->getPos());
            sf::Color pathColor(randz(2,253), randz(2,253), randz(2,253));


            if (!firstRun)
                effects.createLine((oldPos.x + 1) * 20 - 10,
                                   (oldPos.y + 1) * 20 - 10,
                                   (pathPos.x + 1) * 20 - 10,
                                   (pathPos.y + 1) * 20 - 10, 5, pathColor);

            oldPos = pathPos;
            firstRun = false;
        }

    }

    void storePath(void *node)
    {
        Tile *Nodeling = static_cast<Tile *>(node);
        storedPath.push_back(Nodeling);
    }
    void storeRPath(void *node)
    {
        Tile *Nodeling = static_cast<Tile *>(node);
        storedRPath.push_back(Nodeling);
    }

    int makePath(Vec3 Ori, Vec3 Tar)
    {
        int result = 0;
        if (Passable(Tar.x, Tar.y, Tar.z) == 1)
        {
#ifdef USE_PATHER
            float totalCost;

            result = pather->Solve(XYZToNode(Ori.x, Ori.y, Ori.z),
                                   XYZToNode(Tar.x, Tar.y, Tar.z), &microPath,
                                   &totalCost);

            if (result == MicroPather::SOLVED)
            {
                unsigned int pathSize = microPath.size();
                for (int i = 0; i != pathSize; i++)
                {
                    storePath(microPath[i]);
                }
            }
            unsigned int pathSize = microPath.size();
            for (int i = 0; i != pathSize; i++)
            {
                storeRPath(microPath[i]);
            }



#endif
        }
        return result;
    }

    virtual ~PathingController()
    {
        delete pather;
    }

    virtual float LeastCostEstimate(void *nodeStart, void *nodeEnd)
    {
        int xStart, yStart, zStart, xEnd, yEnd, zEnd;
        NodeToXYZ(nodeStart, &xStart, &yStart, &zStart);
        NodeToXYZ(nodeEnd, &xEnd, &yEnd, &zEnd);

        double d = sqrt(pow(xEnd - xStart, 2) + pow(yEnd - yStart, 2) +
                        pow(zEnd - zStart, 2));
        /* Compute the minimum path cost using distance measurement. It is possible
		   to compute the exact minimum path using the fact that you can move only
		   on a straight line or on a diagonal, and this will yield a better result.
		*/
        return d;
    }

    virtual void AdjacentCost(void *node,
                              micropather::MPVector<StateCost> *neighbors)
    {
        int x, y, z;
        NodeToXYZ(node, &x, &y, &z);
        //const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
        //const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
        //const int dx[26] = { -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1};
        //const int dy[26] = { -1, -1, -1, 0, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, 0, 1, 1, 1};
        //const int dz[26] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1};

        const int dx[10] = {0, -1, 0, 1, -1, 1, -1, 0, 1, 0};
        const int dy[10] = {0, -1, -1, -1, 0, 0, 1, 1, 1, 0};
        const int dz[10] = {-1, 0, 0, 0, 0, 0, 0, 0, 0, 1};

        //const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };
        float One = 1;
        float Two = 1.41f;
        float Three = 1.44f;
        const float cost[10] = {One, Two, One, Two, One,
                                One, Two, One, Two, One};

        Tile *Nodeling = static_cast<Tile *>(node);

        if (Nodeling->teleporter)
        {
            Vec3 N(Nodeling->telePos);
            StateCost nodeCost = {XYZToNode(N.x, N.y, N.z), 3};
            neighbors->push_back(nodeCost);
        }

        for (int i = 0; i < 10; ++i)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            int nz = z + dz[i];

            int pass = Passable(nx, ny, nz);
            if (pass > 0)
            {
                if (pass == 4)
                {
                    StateCost nodeCost = {XYZToNode(nx, ny, nz), cost[i]};
                    neighbors->push_back(nodeCost);
                }
                else if ( (pass == 1 || pass == 2 || pass == 3) && dz[i] == 0)
                {
                    StateCost nodeCost = {XYZToNode(nx, ny, nz), cost[i]};
                    neighbors->push_back(nodeCost);
                }
                else if (pass == 3 && dz[i] == -1 && dx[i] == 0 && dy[i] == 0)
                {
                    StateCost nodeCost = {XYZToNode(nx, ny, nz), cost[i]};
                    neighbors->push_back(nodeCost);
                }
                else if (pass == 2 && dz[i] == 1 && dx[i] == 0 && dy[i] == 0)
                {
                    StateCost nodeCost = {XYZToNode(nx, ny, nz), cost[i]};
                    neighbors->push_back(nodeCost);
                }
                else
                {
                    StateCost nodeCost = {XYZToNode(nx, ny, nz), FLT_MAX};
                    neighbors->push_back(nodeCost);
                }
            }
        }
    }

    virtual void
    AdjacentCostPureFlight(void *node,
                           micropather::MPVector<StateCost> *neighbors)
    {
        int x, y, z;
        NodeToXYZ(node, &x, &y, &z);
        //const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
        //const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
        const int dx[26] = {-1, 0,  1, -1, 0,  1, -1, 0,  1, -1, 0,  1, -1,
                            1,  -1, 0, 1,  -1, 0, 1,  -1, 0, 1,  -1, 0, 1};
        const int dy[26] = {-1, -1, -1, 0, 0,  0,  1,  1, 1, -1, -1, -1, 0,
                            0,  1,  1,  1, -1, -1, -1, 0, 0, 0,  1,  1,  1};
        const int dz[26] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0,
                            0,  0,  0,  0,  1,  1,  1,  1,  1,  1, 1, 1, 1};
        //const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };
        float One = 1;
        float Two = 1.41f;
        float Three = 1.44f;
        const float cost[26] = {Three, Two,   Three, Two,   One,  Two,   Three,
                                Two,   Three, Two,   One,   Two,  One,   One,
                                Two,   One,   Two,   Three, Two,  Three, Two,
                                One,   Two,   Three, Two,   Three};

        for (int i = 0; i < 26; ++i)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            int nz = z + dz[i];

            int pass = Passable(nx, ny, nz);
            if (pass > 0)
            {
                if (pass == 1)
                {
                    // Normal floor
                    StateCost nodeCost = {XYZToNode(nx, ny, nz), cost[i]};
                    neighbors->push_back(nodeCost);
                }
                else
                {
                    // Normal floor
                    StateCost nodeCost = {XYZToNode(nx, ny, nz), FLT_MAX};
                    neighbors->push_back(nodeCost);
                }
            }
        }
    }

    virtual void PrintStateInfo(void *node)
    {
        int x, y, z;
        NodeToXYZ(node, &x, &y, &z);
        printf("(%d,%d,%d)", x, y, &z);
    }

#endif
};
PathingController pathCon;




//-- Prototypes
//

void drawStuffs();
int getItemVectorId(int id);

std::string getClipboardText()
{
#ifdef GALAXY_LINUX
    return "Not implemented.";
#else
    // Try opening the clipboard
    if (!OpenClipboard(NULL))
        con("ClipError1");

    // Get handle of clipboard object for ANSI text
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == NULL)
        con("ClipError1");

    // Lock the handle to get the actual text pointer
    char *pszText = static_cast<char *>(GlobalLock(hData));
    if (pszText == NULL)
        con("ClipError1");

    // Save text in a string class instance
    std::string text(pszText);
    con("Your Clipboard Contains: ", false);
    con(text);

    // Release the lock
    GlobalUnlock(hData);

    // Release the clipboard
    CloseClipboard();

    return text;
#endif
}

// Create the main rendering window
std::string randomWindowName()
{
    std::vector<std::string> const names{
        "Zombificational Settlementation Experimentation", "Galaxy",
        "Ye Old Settlement, Ye New Galaxy",
        "The first Galaxy had Joey, Just saying",
        "First comes the universe, then comes Galaxy"};

    return names[randz(0, names.size() - 1)];
}

sf::RenderWindow window;

void unpointItems(std::list<Item> &items)
{

    //for(int i = 0; i != Items.size(); i++)
    for (auto i = items.begin(); i != items.begin(); i++)
    {
        if ((*i).toDelete)
        {
            std::cout << "JobList size: " << uniFact[0].jobList.size()
                      << std::endl;

            for (size_t t = 0; t != uniFact[0].jobList.size(); t++)
            {
                std::cout << "Job Point: " << &uniFact[0].jobList[t].pItem
                          << "/" << uniFact[0].jobList[t].pItem
                          << ", Item Point: " << &(*i) << "/" << &(*i)
                          << std::endl;
                fSleep(1);

                if (uniFact[0].jobList[t].pItem != nullptr &&
                    (&(*i)) != nullptr)
                {

                    std::cout << "ID! ";
                    try
                    {
                        std::cout << uniFact[0].jobList[t].pItem->id;
                    }
                    catch (std::exception &e)
                    {
                        std::cout << "Something went wrong in in here\n";
                    }
                    std::cout << uniFact[0].jobList[t].pItem->id;
                    std::cout << " ; ";
                    int id1 = uniFact[0].jobList[t].pItem->id;
                    std::cout << "ID 2! ";
                    std::cout << (*i).id;
                    std::cout << " ; ";
                    int id2 = (*i).id;
                    std::cout << "ID's: " << id1 << ":" << id2 << std::endl;

                    if (id1 ==
                        id2) //if(UniFact[0].JobList[t].pItem != NULL && &(*i) != NULL && UniFact[0].JobList[t].pItem == &(*i)) //if(ID1 == ID2)
                    {
                        std::cout << "Match! \n";
                        uniFact[0].jobList[t].pItem = nullptr;
                        //fSleep(1);
                    }
                }
            }
        }
    }
}

bool removeItem(int id)
{
    int tempInt = 0;
    std::list<Item>::iterator location;
    for (auto it = worlditems.begin(); it != worlditems.end(); ++it)
    {
        if (it->id == id)
        {
            location = it;
            tempInt = 1;
        }
    }
    if (tempInt == 1)
    {
        worlditems.erase(location);
        return true;
    }
    else if (tempInt == 0)
    {
        return false;
    }
    throw std::runtime_error("Couldn't return anything sensible");
}

void updateItem()
{
    if (gvars::debug)
    {
        std::cout << "Pre Item \n";
    }

    for (auto &item : worlditems)
    {
        try
        {
            if (gvars::debug)
            {
                std::cout << item.name << "'s turn! \n";
            }
            if (item.produces == true)
            {
                if (gvars::debug)
                {
                    std::cout << item.name << " can produce. \n";
                }
                item.prodratetimer++;
                if (item.prodratetimer >= item.prodrate)
                {
                    item.prodratetimer = 0;
                    std::string s;
                    s = item.produce;
                    auto a = new char[s.size() + 1];
                    a[s.size()] = 0;
                    memcpy(a, s.c_str(), s.size());
                    bool findEmpty = false;
                    int x;
                    int y;
                    while (findEmpty == false)
                    {
                        x = randz(item.xpos - 10, item.xpos + 10);
                        y = randz(item.ypos - 10, item.ypos + 10);
                        if (tiles[abs_to_index(x / GRID_SIZE)][abs_to_index(
                                y / GRID_SIZE)][30].walkable == true)
                        {
                            findEmpty = true;
                        }
                    }
                    if (gvars::debug)
                    {
                        std::cout << "Producing: " << s << " \n";
                    }
                    spawnItem(s, x, y);
                    if (gvars::debug)
                    {
                        std::cout << item.name << " has produced: " << s
                                  << " \n";
                    }
                }
            }
            if (gvars::debug)
            {
                std::cout << "Done with: " << item.name << " \n";
            }

            if (gvars::debug)
            {
                std::cout << "Acting on Missile \n";
            }
            if (item.targetPos != sf::Vector2f(-1, -1))
            {
                item.xpos = (item.xpos + item.targetPos.x) / 2;
                item.ypos = (item.ypos + item.targetPos.y) / 2;
                if (item.age >= 10)
                {
                    item.toDelete = true;
                    boom(item.xpos, item.ypos, randz(item.mindam, item.maxdam),
                         item.range);

                    for (int itLength = 0; itLength != 5; itLength++)
                    {
                        for (int rot = 1; rot != 361; rot++)
                        {
                            //int Rot = GX;
                            int XPos = ((abs(item.xpos / 20)) +
                                        cosf(rot * PI / 180) * itLength);
                            int YPos = ((abs(item.ypos / 20)) +
                                        sinf(rot * PI / 180) * itLength);
                            //XPos *= 20;
                            //YPos *= 20;

                            //Effectz.CreateCircle(MousePos.x,MousePos.y,5,Red);
                            //Effectz.CreateCircle(XPos,YPos,5,White);
                            tiles[XPos][YPos][30].stone();
                        }
                    }

                    con("Boom!");
                }
            }
            item.age = item.age + 1;
            if (gvars::debug)
            {
                std::cout << "Post Item \n";
            }
        }
        catch (std::exception &e)
        {
            std::cout << "Problem with an Item in Update Item \n";
        }
    }
}

std::vector<int> nngTrace(int xa, int ya, int xb, int yb, int id,
                          std::vector<int> /*exceptions*/)
{ //.at(0) = Item/NPC(23/69) .at(1) = id
    int dx = xb - xa, dy = yb - ya, steps;
    float xIncrement, yIncrement, x = xa, y = ya;
    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);
    xIncrement = dx / (float)steps;
    yIncrement = dy / (float)steps;
    std::vector<int> vectorID;
    for (int k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        if (inputState.key[Key::G])
        {
            effects.createCircle(x, y, 1, sf::Color::Blue);
        }
        int count = 0;

        { // Merely doing this so I can reuse the same code, but for items, Hehe.
            std::vector<Npc>::iterator me;

            for (me = npclist.begin(); me != npclist.end(); ++me)
            {
                if (math::closeish(x, y, me->xpos, me->ypos) <= me->reach &&
                    me->id != id)
                {

                    std::vector<int>::iterator vec;
                    try
                    {
                        bool exists = false;
                        for (vec = vectorID.begin(); vec != vectorID.end();
                             ++vec)
                        {
                            if (*vec == me->id)
                            {
                                exists = true;
                            }
                        }
                        if (exists == false)
                        {
                            vectorID.push_back(69);
                            vectorID.push_back(getNpcVectorId(me->id));
                        }
                    }
                    catch (std::exception &e)
                    {
                        std::cout << "VectError\n";
                    }

                } //23 = Item, 69 = NPC
                count++;
            }
        }
        std::list<Item>::iterator me;

        for (me = worlditems.begin(); me != worlditems.end(); ++me)
        {
            if (math::closeish(x, y, me->xpos, me->ypos) <= 10 && me->id != id)
            {
                //Making sure not to constantly add the same
                //try{if( VectorID.at(Count-1) != Me->id){ VectorID.push_back(23); VectorID.push_back( GetItemVectorId(Me->id)); }} catch (std::exception& e){}

            } //23 = Item, 69 = NPC
            count++;
        }

        if (inputState.key[Key::Period])
        {
            effects.createCircle(x, y, 1, sf::Color::White);
        }
    }
    if (vectorID.size() == 0)
    {
        vectorID.push_back(-1);
    }
    return vectorID;
}


struct entityvectorpointercontainer
{
    std::set<Item*> items;
    std::set<Npc*> npcs;
    std::set<Tile*> tiles;
};


entityvectorpointercontainer entityTrace(Vec3 Ori, Vec3 Tar) /* TODO: Improve this to use Z axis as well, or find new formula that can. */
{ // Looking in a straight line for a specific spot, Walls block vision.

    entityvectorpointercontainer EVPC;

    float dx = Tar.x - Ori.x, dy = Tar.y - Ori.y, steps;
    float xIncrement, yIncrement, x = Ori.x, y = Ori.y;
    if (abs_to_index(dx) > abs_to_index(dy))
        steps = abs_to_index(dx);
    else
        steps = abs_to_index(dy);
    xIncrement = dx / (float)steps;
    yIncrement = dy / (float)steps;

    for (int k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        if (tiles[abs_to_index(x / GRID_SIZE)][abs_to_index(y / GRID_SIZE)][abs_to_index(Ori.z/20)]
                .transparent == false)
        {

            if (inputState.key[Key::Quote])
            {
                effects.createLine(x, y, Ori.x, Ori.y, 1, sf::Color::Cyan);
            }
            break;

        } // Stops the trace if it hits a wall.

        for (auto &items : worlditems)
        {
            if (items.xpos == x && items.ypos == y && items.zpos == Ori.z)
                EVPC.items.insert(&items);
        }
        for (auto &npcs : npclist)
        {
            //if(npcs.xpos == x && npcs.ypos == y && npcs.zpos == Ori.z)
            if (math::closeish(npcs.xpos,npcs.ypos,x,y) <= npcs.size && npcs.zpos == Ori.z)
                EVPC.npcs.insert(&npcs);
        }
        EVPC.tiles.insert(&tiles[abs_to_index(x / GRID_SIZE)][abs_to_index(y / GRID_SIZE)][abs_to_index(Ori.z/20)]);



        if (inputState.key[Key::Quote])
        {
            effects.createLine(x, y, Ori.x, Ori.y, 1, sf::Color::Blue);
        }
        sf::Vector2f pos(abs_to_index(x / GRID_SIZE),
                         abs_to_index(y / GRID_SIZE));
        sf::Vector2f tar(abs_to_index(Tar.x / GRID_SIZE),
                         abs_to_index(Tar.y / GRID_SIZE));
    }
    return EVPC;
}




bool gridTrace(sf::Vector2f Ori, sf::Vector2f Tar)
{ // Looking in a straight line for a specific spot, Walls block vision.

    float dx = Tar.x - Ori.x, dy = Tar.y - Ori.y, steps;
    float xIncrement, yIncrement, x = Ori.x, y = Ori.y;
    if (abs_to_index(dx) > abs_to_index(dy))
        steps = abs_to_index(dx);
    else
        steps = abs_to_index(dy);
    xIncrement = dx / (float)steps;
    yIncrement = dy / (float)steps;

    for (int k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        if (tiles[abs_to_index(x / GRID_SIZE)][abs_to_index(y / GRID_SIZE)][30]
                .transparent == false)
        {

            if (inputState.key[Key::Quote])
            {
                effects.createLine(x, y, Ori.x, Ori.y, 1, sf::Color::Cyan);
            }
            //std::cout << "Shoulda Broke. " << std::endl;
            break;

        } // Stops the trace if it hits a wall.
        //std::cout << "Stuffs " << std::endl;
        sf::Vector2f pos(abs_to_index(x / GRID_SIZE),
                         abs_to_index(y / GRID_SIZE));
        sf::Vector2f tar(abs_to_index(Tar.x / GRID_SIZE),
                         abs_to_index(Tar.y / GRID_SIZE));
        if (pos == tar)
        {
            return true;
        } // Returns true and stops searching.

        if (inputState.key[Key::Quote])
        {
            effects.createLine(x, y, Ori.x, Ori.y, 1, sf::Color::Blue);
        }
    }
    return false; // Returns false if the target was never found.
}



bool gridposTrace(int xa, int ya, int xb, int yb, int id, sf::Vector2f target)
{ // Looking in a straight line for a specific spot, Walls block vision.

    int dx = xb - xa, dy = yb - ya, steps;
    float xIncrement, yIncrement, x = xa, y = ya;
    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);
    xIncrement = dx / (float)steps;
    yIncrement = dy / (float)steps;

    for (int k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        if (tiles[abs_to_index(x / GRID_SIZE)][abs_to_index(y / GRID_SIZE)][30]
                .id == 1010)
        {
            if (inputState.key[Key::Period] && id == gvars::myTargetid)
            {
                effects.createLine(x, y, xa, ya, 1, sf::Color::Blue);
            }
            //std::cout << "Shoulda Broke. " << std::endl;
            break;

        } // Stops the trace if it hits a wall.
        //std::cout << "Stuffs " << std::endl;
        sf::Vector2f pos(abs(x / GRID_SIZE), abs(y / GRID_SIZE));
        sf::Vector2f tar(abs(target.x / GRID_SIZE), abs(target.y / GRID_SIZE));
        if (pos == tar)
        {
            return true;
        } // Returns true and stops searching.
        if (inputState.key[Key::Period] && id == gvars::myTargetid)
        {
            effects.createLine(x, y, xa, ya, 1, sf::Color::Blue);
        }
    }
    return false; // Returns false if the target was never found.
}

std::vector<int> npcTrace(int xa, int ya, int xb, int yb, int id,
                          std::vector<int> /*exceptions = std::vector<int>()*/)
{

    int dx = xb - xa, dy = yb - ya, steps;
    float xIncrement, yIncrement, x = xa, y = ya;
    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);
    xIncrement = dx / (float)steps;
    yIncrement = dy / (float)steps;
    std::vector<int> vectorID;

    for (int k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        bool kill = false;
        std::vector<Npc>::iterator me;
        int count = 0;
        for (me = npclist.begin(); me != npclist.end(); ++me)
        {
            if (math::closeish(x, y, me->xpos, me->ypos) <= me->reach &&
                me->id != id)
            {
                vectorID.push_back(count);
                kill = true;
            }
            count++;
        }
        if (kill)
            return vectorID;

        if (inputState.key[Key::H])
        {
            effects.createCircle(x, y, 1, sf::Color::White);
        }
    }
    if (vectorID.size() == 0)
    {
        vectorID.push_back(-1);
    }

    throw std::runtime_error("npcTrace: couldn't return a value");
}

int getItemVectorId(int id)
{
    int index = 0;
    for (auto const &item : worlditems)
    {
        if (item.id == id)
        {
            return index;
        }
        index++;
    }
    return -1;
}

int getNpcVectorId(int id)
{
    int index = 0;
    for (auto const &npc : npclist)
    {
        if (npc.id == id)
        {
            return index;
        }
        index++;
    }
    return -1;
}

bool removeNPC(char * /*NPCname*/, int /*Id*/)
{
    int tempInt = 0;
    std::vector<Npc>::iterator location;
    for (auto it = npclist.begin(); it != npclist.end(); ++it)
    {
        if (it->health <= 0 || it->hasSpawned == false || it->alive == false)
        {
            location = it;
            tempInt = 1;
        }
    }
    if (tempInt == 1)
    {
        npclist.erase(location);
        return true;
    }
    else if (tempInt == 0)
    {
        return false;
    }
    throw std::runtime_error("RemoveNPC: Couldn't return anything sensible.");
}

struct ItemFindResult
{
    bool found;
    int xpos;
    int ypos;
    int id;
    int vectorPosition;
};

ItemFindResult findClosestItem(int orix, int oriy, std::string tarItem,
                               int /*Gxpos*/ = 0, int /*Gypos*/ = 0,
                               int /*Rxpos*/ = 0, int /*Rypos*/ = 0)
{
    // This will be difficult, go through all the items, and find the closest one.
    int closx =
        -1000000; // Make sure the default starting number is far beyond being the closest one.
    int closy = -1000000;
    int closid;
    int closVect;
    int vectPos = 0;
    bool first = true;
    for (auto const &item : worlditems)
    {
        if (first == true)
        {
            if (item.name == tarItem)
            {
                closx = item.xpos;
                closy = item.ypos;
                closid = item.id;
                closVect = vectPos;
                first = false;
            }
        }
        else
        {
            if (item.name == tarItem)
            {
                int one = math::closeish(orix, oriy, item.xpos, item.ypos);
                int two = math::closeish(orix, oriy, closx, closy);
                if (one < two)
                {
                    closx = item.xpos;
                    closy = item.ypos;
                    closid = item.id;
                    closVect = vectPos;
                }
            }
        }
        vectPos++;
    }
    if (first != true)
    {
        return {true, closx, closy, closid, closVect};
    }
    return {false, 0, 0, 0, 0};
}

std::set<int> npcList(int exceptions = -1)
{
    if (gvars::debug)
    {
        std::cout << "Pre npcList \n";
    }
    std::set<int> returns;

    for (auto const &npc : npclist)
    {
        if (gvars::debug)
        {
            std::cout << "For NpcList \n";
        }
        if (npc.id != exceptions)
        {
            if (gvars::debug)
            {
                std::cout << "Post exception NpcList \n";
            }
            returns.insert(getNpcVectorId(npc.id));
            if (gvars::debug)
            {
                std::cout << "Post Returns NpcList \n";
            }
        }
    }
    if (gvars::debug)
    {
        std::cout << "Post For NpcList \n";
    }
    if (!returns.empty())
    {
        return returns;
    }
    throw std::runtime_error("NpcList: Couldn't return anything!");
}

void runCritterBody(Npc &npc)
{
    /*  BodyPart Loop
        First, Run through the bodyparts finding the 'global' tags, like
            Nutrient Extraction and such.
        Second, Run through each individual part running through all
            the local tags.
    */

    short int parts = 0;
    size_t searchPos = 0;
    size_t endPos = 0;

    debug("Debug: Beginning Part Loop for" + npc.name);

    //  Global Part Tag Variables

    float partsWalkSpeed = 0;
    float globalNutritionPercentage = 100;

    //  *   Global Part Tag Variables   *

    while (searchPos != npc.body.bodyParts.npos) // Global Part Tags
    {
        searchPos = npc.body.bodyParts.find("{", searchPos);

        if (searchPos != npc.body.bodyParts.npos)
        {
            endPos = npc.body.bodyParts.find("}", searchPos);

            std::string workingLine;

            workingLine.append(npc.body.bodyParts, searchPos,
                               endPos - searchPos);
            float partNumber = 0;

            partNumber = stringFindNumber(workingLine, "[Walk:");
            if (partNumber != 0)
            {

                if (partsWalkSpeed != 0)
                {
                    partsWalkSpeed = partNumber;
                }
                else
                {
                    partsWalkSpeed += (partNumber * 0.5);
                }
            }

            partNumber = stringFindNumber(workingLine, "[NutritionExtraction:");
            if (partNumber != 0)
            {
                globalNutritionPercentage += partNumber;
            }

            partNumber = stringFindNumber(workingLine, "[DigestsBlood:");
            if (partNumber != 0)
            {
            }
            partNumber = stringFindNumber(workingLine, "[DigestsFlesh:");
            if (partNumber != 0)
            {
                npc.consumeFlesh = true;
            }
            partNumber = stringFindNumber(workingLine, "[DigestsVeggy:");
            if (partNumber != 0)
            {
                npc.consumeVeggy = true;
            }
            partNumber = stringFindNumber(workingLine, "[DigestsWater:");
            if (partNumber != 0)
            {
                npc.consumeWater = true;
            }

            searchPos = endPos;
        }
    }

    searchPos = 0;

    for (auto &elem : npc.inventory)
    {
        if ((elem).insidePart != "")
        {
            (elem).hasInternalUse--;
        }
    }

    while (searchPos != npc.body.bodyParts.npos) // Individual Part Tags
    {

        searchPos = npc.body.bodyParts.find("{", searchPos);

        if (searchPos != npc.body.bodyParts.npos)
        {
            endPos = npc.body.bodyParts.find("}", searchPos);
            parts++;

            std::string workingLine;

            workingLine.append(npc.body.bodyParts, searchPos,
                               endPos - searchPos);

            float partNumber = 0;
            std::string partString = "";
            Item *partItem;

            std::string currentPart = stringFindString(workingLine, "[Name:");

            partNumber = stringFindNumber(workingLine, "[DigestsBlood:");
            partItem = getItemPtrFromVector(npc.inventory, "Blood");
            if (partNumber != 0 && partItem != nullptr)
            {

                float workAmount = partItem->amount;
                float diff = workAmount - (partNumber / 1000);

                if (diff > 0)
                {
                    partItem->amount = diff;
                    float Nutr = (workAmount - diff) *
                                 100; // TODO: Figure this out better.
                    npc.bloodwork(
                        "Nutrients",
                        Nutr * percentageBuff(globalNutritionPercentage));
                }
                else
                {
                    //*GetItemPtrfromVector(npc.inventory,"Blood").amount = 0;
                    getItemPtrFromVector(npc.inventory, "Blood")->toDelete =
                        true;
                    float nutr =
                        workAmount * 100; // TODO: Figure this out better.
                    npc.bloodwork(
                        "Nutrients",
                        nutr * percentageBuff(globalNutritionPercentage));
                }
            }

            partNumber = stringFindNumber(workingLine, "[DigestsFlesh:");
            partItem =
                getItemPtrfromVectorVarSearch(npc.inventory, "MassFlesh");
            //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
            if (partNumber != 0 && partItem != nullptr &&
                partItem->massFlesh >
                    0) // TODO: Make sure the item is in THIS PART before digesting it!
            {
                //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                float workAmount = partItem->massFlesh;
                float diff = workAmount - (partNumber / 1000);

                if (diff > 0)
                {
                    partItem->massFlesh = diff;
                    partItem->hasInternalUse = 0;
                    float Nutr = (workAmount - diff) *
                                 100; // TODO: Figure this out better.
                    npc.bloodwork(
                        "Nutrients",
                        Nutr * percentageBuff(globalNutritionPercentage));
                }
                if (partItem->massFlesh <= 0)
                {
                    //*GetItemPtrfromVector(npc.inventory,"Blood").amount = 0;
                    partItem->toDelete = true;
                    //Add Food to everyone, Make sure they go hungry to eat it, Figure out a way to Eject the empty item, Or do water! Everyone starts with water.
                    //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                }
            }

            partNumber = stringFindNumber(workingLine, "[DigestsVeggy:");
            partItem =
                getItemPtrfromVectorVarSearch(npc.inventory, "MassVeggy");
            //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
            if (partNumber != 0 && partItem != nullptr &&
                partItem->massVeggy >
                    0) // TODO: Make sure the item is in THIS PART before digesting it!
            {
                //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                float workAmount = partItem->massVeggy;
                float diff = workAmount - (partNumber / 1000);

                if (diff > 0)
                {
                    partItem->massVeggy = diff;
                    partItem->hasInternalUse = 0;
                    float nutr = (workAmount - diff) *
                                 100; // TODO: Figure this out better.
                    npc.bloodwork(
                        "Nutrients",
                        nutr * percentageBuff(globalNutritionPercentage));
                }
                if (partItem->massVeggy <= 0)
                {
                    partItem->toDelete = true;
                    //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                }
            }

            partNumber = stringFindNumber(workingLine, "[DigestsWater:");
            partItem =
                getItemPtrfromVectorVarSearch(npc.inventory, "MassWater");
            //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
            if (partNumber != 0 && partItem != nullptr &&
                partItem->massWater >
                    0) // TODO: Make sure the item is in THIS PART before digesting it!
            {
                //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                float workAmount = partItem->massWater;
                float diff = workAmount - (partNumber / 1000);

                if (diff > 0)
                {
                    partItem->massWater = diff;
                    partItem->hasInternalUse = 0;
                    float nutr = (workAmount - diff) *
                                 100; // TODO: Figure this out better.
                    npc.bloodwork(
                        "Hydration",
                        nutr * percentageBuff(globalNutritionPercentage));
                }
                if (partItem->massWater <= 0)
                {
                    partItem->toDelete = true;
                    //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                }
            }

            partString = stringFindString(workingLine, "[PoisonFilter:");
            if (partString != "")
            {

                std::vector<std::string> strVec =
                    stringFindElements(partString, ":");
                if (gvars::debug)
                    std::cout << "StrVec[0]: " << strVec[0] << std::endl;
                float leftover =
                    npc.bloodwork(strVec[0], -atof(strVec[1].c_str()));
                if (gvars::debug)
                    std::cout << "Bloodwork leftover is: " << leftover
                              << std::endl;
                //NPC Critter;

                for (size_t i = 0; i != strVec.size(); i++)
                {
                    if (gvars::debug)
                        std::cout << strVec[i] << std::endl;
                }
            }

            partNumber = stringFindNumber(workingLine, "[Orafice:");
            if (partNumber > 0)
            {
                //std::vector<item> * Inv = &npc.inventory;

                //for(int i = 0; i != npc.inventory.size(); i++)
                for (auto i = npc.inventory.begin(); i != npc.inventory.end();
                     i++)
                {
                    bool foundIt = false;
                    if ((*i).insidePart == "" && (*i).massFlesh > 0 &&
                        npc.consumeFlesh) // Awww yeeessss, We gonna eat some flesh with our Orafice!
                    {
                        (*i).insidePart = currentPart;
                        foundIt = true;
                    }

                    if ((*i).insidePart == "" && (*i).massVeggy > 0 &&
                        npc.consumeVeggy) // Awww yeeessss, We gonna eat something with our Orafice!
                    {
                        (*i).insidePart = currentPart;
                        foundIt = true;
                    }

                    if ((*i).insidePart == "" && (*i).massWater > 0 &&
                        npc.consumeWater) // Awww yeeessss, We gonna eat something with our Orafice!
                    {
                        (*i).insidePart = currentPart;
                        foundIt = true;
                    }

                    if (foundIt)
                    {
                        std::string chtStr;
                        chtStr.append("* ");
                        chtStr.append(npc.name);
                        chtStr.append("(" + std::to_string(npc.id) + ")");
                        chtStr.append(" has inserted ");
                        chtStr.append((*i).name);
                        chtStr.append(" into their ");
                        chtStr.append(currentPart);
                        chtStr.append("'s Orafice(");
                        chtStr.append(std::to_string(partNumber));
                        chtStr.append(").");

                        chatBox.addChat(chtStr, sf::Color(150, 150, 0));
                    }
                }
            }

            partNumber = stringFindNumber(
                workingLine, "[BloodPumpRate:"); // TODO: Do this right.
            if (partNumber != 0)
            {
                float blood = stringFindNumber(npc.bloodcontent, "[Nutrients:");
                if (blood > partNumber)
                {
                    if ((npc.maxhunger - npc.hunger) > partNumber)
                    {
                        npc.hunger += partNumber;
                        npc.bloodwork("Nutrients", -partNumber);
                    }
                }
                blood = stringFindNumber(npc.bloodcontent, "[Hydration:");
                if (blood > partNumber)
                {
                    if ((npc.maxthirst - npc.thirst) > partNumber)
                    {
                        npc.thirst += partNumber;
                        npc.bloodwork("Hydration", -partNumber);
                    }
                }
            }
            searchPos = endPos;
        }
    }
}

void critterBrain(Npc &npc, std::vector<Npc> &container)
{
    int critterZ = npc.zpos/20;
    textList.createText(npc.xpos,npc.ypos,10,sf::Color::White,"ZPos:","",npc.zpos," /","",critterZ);
    runCritterBody(npc);
    debug("Debug: Ending Part Loop");

    /*Simulating Hunger/Thirst, Needs to be nerfed/formulated to conditions, I.E. Attributes/Parts/Weather*/
    npc.bloodwork("Nutrients", -1);
    npc.bloodwork("Hydration", -1);

    /* *BodyPart Loop* */
    /* Critter Vision   */
    const sf::Vector2f npcPos(npc.xpos, npc.ypos);
    //sf::Vector2f targetPos(gvars::mousePos);
    /* Running through the critters pointers to see which one is valid, Then setting it to aim at it. */
    sf::Vector2f * targetPos = &npc.desiredViewAngle;
    if (npc.targetInfo.item != nullptr)
        *targetPos = sf::Vector2f(npc.targetInfo.item->xpos,npc.targetInfo.item->ypos);
    else if (npc.targetInfo.npc != nullptr)
        *targetPos = sf::Vector2f(npc.targetInfo.npc->xpos,npc.targetInfo.npc->ypos);
    else if (npc.targetInfo.tile != nullptr)
    {
        Vec3 tilePos(npc.targetInfo.tile->getPos());
        *targetPos = sf::Vector2f(tilePos.x,tilePos.y);
    }
    else if (randz(1,100) == 100)
        /* No target? Occasionally choose a nearby direction to look at. */
        *targetPos = sf::Vector2f(npc.xpos+randz(-15,15),npc.ypos+randz(-15,15));

    /*  Do the angle math to figure out which direction we need to turn */
    const float npcAngle = math::constrainAngle(npc.angle);
    const float targetAngle =
        math::constrainAngle(math::angleBetweenVectors(npcPos, *targetPos) - 90);
    /*  Turn towards it's target!    */
    if (math::angleDiff(npcAngle, targetAngle) > npc.turnSpeed)
        npc.angle += npc.turnSpeed;
    else if (math::angleDiff(npcAngle, targetAngle) < -npc.turnSpeed)
        npc.angle -= npc.turnSpeed;
    else
        npc.angle = targetAngle;

    const int endAngle = -(npc.angle - (npc.viewangle / 2));
    const int startAngle = -(npc.angle - (-npc.viewangle / 2));

    /*Drawing Vision*/
    sf::ConvexShape shape;
    int pointCounter = 1;
    shape.setPointCount(pointCounter);
    shape.setPoint(0, sf::Vector2f(0, 0));

    for (int rot = startAngle; rot != endAngle; rot++)
    {
        float xPos = npc.xpos + sinf(rot * PI / 180) * npc.viewrange;
        float yPos = npc.ypos + cosf(rot * PI / 180) * npc.viewrange;

        entityvectorpointercontainer EVPC;

        //EVPC = entityTrace(Vec3(npcPos.x,npcPos.y,npc.zpos), Vec3(xPos, yPos, npc.zpos));

        //std::cout << npc.name << "'s EVPC: " << EVPC.items.size() << "/" << EVPC.npcs.size() << "/" << EVPC.tiles.size() << std::endl;

        for (auto &i : EVPC.items)
        {
            effects.createCircle(i->xpos,i->ypos,5,sf::Color::Black);
        }
        for (auto &i : EVPC.tiles)
        {
            Vec3 Pos = i->getPos();
            effects.createCircle(Pos.x*20,Pos.y*20,5,sf::Color::Black);
        }


        if (rot == startAngle)
        {
            pointCounter++;
            shape.setPointCount(pointCounter);
            shape.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - npc.xpos, yPos - npc.ypos));
        }

        int CenterPoint = (startAngle + endAngle) / 2;
        if (rot == CenterPoint)
        {
            pointCounter++;
            shape.setPointCount(pointCounter);
            shape.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - npc.xpos, yPos - npc.ypos));
            bool failedAlign = true;
            if (npc.angle == targetAngle)
            {
                if (gridTrace(npcPos, *targetPos))
                {
                    effects.createLine(npc.xpos, npc.ypos, xPos, yPos, 1,
                                       sf::Color::Cyan);
                    failedAlign = false;
                }
            }
            if (failedAlign)
                effects.createLine(npc.xpos, npc.ypos, xPos, yPos, 1,
                                   sf::Color::Red);
        }
        if ((rot % 10) == 0 && rot != 0)
        {
            pointCounter++;
            shape.setPointCount(pointCounter);
            shape.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - npc.xpos, yPos - npc.ypos));
        }
        if (rot == endAngle - 1)
        {
            pointCounter++;
            shape.setPointCount(pointCounter);
            shape.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - npc.xpos, yPos - npc.ypos));
        }
    }

    sf::Color filling(sf::Color::Yellow);
    filling.a = (50);
    shape.setFillColor(filling);
    shape.setOutlineColor(sf::Color::Yellow);
    shape.setOutlineThickness(1);
    shape.setPosition(npcPos);
    effects.polygons.push_back(shape);

    /* Critter Prioritization */
    // Method Two, Struct Desires
    struct Desire
    {
        std::string type;
        float potency;
    };
    std::vector<Desire> desires;

    // Declaring and adding Desires
    Desire newDesire;
    { //Sustainence
        newDesire.type = "Sustainence";
        newDesire.potency = 0;
    }
    desires.push_back(newDesire);
    { //Apathy
        newDesire.type = "Apathy";
        newDesire.potency = 100;
    }
    desires.push_back(newDesire);
    { //SelfDefense
        newDesire.type = "SelfDefense";
        newDesire.potency = 0;
    }
    desires.push_back(newDesire);
    { //Social
        newDesire.type = "Social";
        newDesire.potency = 0;
    }
    desires.push_back(newDesire);
    { //Work
        newDesire.type = "Work";
        newDesire.potency = 0;
    }
    desires.push_back(newDesire);

    /*Causation to Desires*/
    // Get Critters max nutrition, then reduce it by critters nutrients in blood
    float nutrients = npc.maxhunger - npc.bloodwork("Nutrients", 0);
    float hydration = npc.maxthirst - npc.bloodwork("Hydration", 0);

    for (auto &sus : desires)
    {
        if (sus.type == "Sustainence")
            sus.potency += hydration + nutrients;
        if (sus.type == "SelfDefense")
        {
            // This line makes the game freeze
            sus.potency = 10000;
        }
    }

    // Finding the highest Desire
    bool inComplete;
    Desire *highestDesire = nullptr;
    bool firstIter = true;

ReDesire:
    inComplete = false;

    for (auto &i : desires)
    {
        if (firstIter)
        {
            highestDesire = &i;
            firstIter = false;
        }
        if (i.potency > (*highestDesire).potency)
            highestDesire = &i;
    }

    if (highestDesire == nullptr)
    {
        throw std::runtime_error("critterBrain: Something went wrong!");
    }

    // Acting on Highest Desire
    if ((*highestDesire).type == "Apathy")
    {
        effects.createCircle(npc.xpos, npc.ypos, 11, sf::Color::Red);
    }
    if ((*highestDesire).type == "SelfDefense")
    {
        (*highestDesire).potency = 0;
        inComplete = true;
    }
    if ((*highestDesire).type == "Sustainence")
    {
        bool inInv = false;
        for (auto &inv : npc.inventory)
            if (inv.massFlesh > 0)
            {
                //npc.Target.item = &Inv;
                inInv = true;
                break;
            }
        if (inInv == false)
        {
            for (auto &lclItms : worlditems)
            {
                if (npc.targetInfo.item == nullptr && lclItms.massFlesh > 0 &&
                    lclItms.user == nullptr)
                {
                    npc.targetInfo.item =
                        &lclItms; // Is this creating a copy? The Behavior in testing still makes multiple critters target the same item.
                    lclItms.user = &npc;
                }
                else if (lclItms.massFlesh > 0 && lclItms.user == nullptr)
                {

                    float CurrentItem = math::closeish(
                        npc.xpos, npc.ypos, (*npc.targetInfo.item).xpos,
                        (*npc.targetInfo.item).ypos);
                    float NewItem = math::closeish(npc.xpos, npc.ypos,
                                                   lclItms.xpos, lclItms.ypos);
                    if (NewItem < CurrentItem)
                    {
                        (*npc.targetInfo.item).user = nullptr;
                        npc.targetInfo.item = &lclItms;
                        (*npc.targetInfo.item).user = &npc;
                    }
                }
            }
            if (npc.targetInfo.item != nullptr)
            {
                sf::Vector2f ItemPos((*npc.targetInfo.item).xpos,
                                     (*npc.targetInfo.item).ypos);
            /*    npc.dirMove(sf::Vector2f((*npc.targetInfo.item).xpos,
                                         (*npc.targetInfo.item).ypos)); */

                if (math::closeish(npc.xpos, npc.ypos, ItemPos.x, ItemPos.y) <=
                    npc.size * 2)
                {
                    Item *tar = npc.targetInfo.item;
                    npc.targetInfo.item = nullptr;
                    (*tar).user = nullptr;
                    npc.inventory.push_back((*tar));
                    (*tar).toDelete = true;
                }
            }
        }

        if (npc.targetInfo.item != nullptr && inInv == false)
        {
            effects.createLine(npc.xpos, npc.ypos, (*npc.targetInfo.item).xpos,
                               (*npc.targetInfo.item).ypos, 2,
                               sf::Color::White);
        }
    }

    // Incase the highest desire isn't completable, Go through again for the next highest desire.
    if (inComplete)
        goto ReDesire;

    textList.createText(npc.xpos - 30, npc.ypos - 15, 10, sf::Color::Red,
                        (*highestDesire).type, ":", (*highestDesire).potency);
    textList.createText(npc.xpos - 70, npc.ypos - 35, 10, sf::Color::Cyan,
                        npc.body.bodyParts);

    /* End of Critter Prioritization */

    Vec3 startPos(npc.xpos/20,npc.ypos/20,npc.zpos/20);
    //Vec3 endPos(46, 46, 29);
    Vec3 endPos(gvars::mousePos.x/20, gvars::mousePos.y/20, gvars::currentz);

    int result = pathCon.makePath(startPos, endPos);
    pathCon.drawStoredPath();

    if(npc.targetInfo.item != nullptr)
    {
            std::cout << "Not null, ";
            Vec3 startPos(npc.xpos/20,npc.ypos/20,npc.zpos/20);
            Vec3 endPos(npc.targetInfo.item->xpos/20, npc.targetInfo.item->ypos/20, npc.targetInfo.item->zpos/20);
            //int result = pathCon.makePath(startPos, endPos);
            std::cout << "result: " << result << " path size: " << pathCon.storedPath.size() << std::endl;

            //world.DrawPath();
            pathCon.drawStoredPath();
    }
    if(!pathCon.storedPath.empty())
    {
        Vec3 Pos(pathCon.storedPath[1]->getPos());


        npc.dirMove(sf::Vector2f(Pos.x*20,Pos.y*20));
        pathCon.storedPath.clear();
    }
    else if(npc.targetInfo.item != nullptr && pathCon.storedPath.size() == 1 || npc.targetInfo.item != nullptr && pathCon.storedPath.size() == 2)
    {
        npc.dirMove(sf::Vector2f((*npc.targetInfo.item).xpos,
                                         (*npc.targetInfo.item).ypos));
    }


    removeItems(npc.inventory);
}

void critterBrain(std::vector<Npc> &npcs)
{

    //Vec3 startPos(35,35,30);
    //Vec3 endPos(46, 46, 30);

    Vec3 startPos(48,44,29);
    Vec3 endPos(44, 44, 29);

    if(inputState.key[Key::K].time > 1)
        endPos = Vec3(50,50,30);
    if(inputState.key[Key::L].time > 1)
        endPos = Vec3(50,50,29);
    //Vec3 endPos(gvars::mousePos.x/20, gvars::mousePos.y/20, gvars::currentz);

    int result = pathCon.makePath(startPos, endPos);
    pathCon.drawStoredPathRainbow();
    pathCon.storedPath.clear();
    pathCon.storedRPath.clear();
    std::cout << result << ", Is the test. \n";


    for (auto &npc : npcs)
    {
        critterBrain(npc, npcs);
    }
}

void updateNpc(Npc &npc, int integerIterator)
{
    // BodyPart Loop
    // First, Run through the bodyparts finding the 'global' tags, like Nutrient Extraction and such.
    // Second, Run through each individual part running through all the local tags.

    short int parts = 0;
    size_t searchPos = 0;
    size_t endPos = 0;

    debug("Debug: Beginning Part Loop for" + npc.name);

    //  Global Part Tag Variables

    float partsWalkSpeed = 0;
    float globalNutritionPercentage = 100;
    bool consumeFlesh = false;
    bool consumeVeggy = false;
    bool consumeWater = false;

    //  *   Global Part Tag Variables   *

    while (searchPos != npc.body.bodyParts.npos) // Global Part Tags
    {
        searchPos = npc.body.bodyParts.find("{", searchPos);

        if (searchPos != npc.body.bodyParts.npos)
        {
            endPos = npc.body.bodyParts.find("}", searchPos);

            std::string workingLine;

            workingLine.append(npc.body.bodyParts, searchPos,
                               endPos - searchPos);
            float partNumber = 0;

            partNumber = stringFindNumber(workingLine, "[Walk:");
            if (partNumber != 0)
            {

                if (partsWalkSpeed != 0)
                {
                    partsWalkSpeed = partNumber;
                }
                else
                {
                    partsWalkSpeed += (partNumber * 0.5);
                }
            }

            partNumber = stringFindNumber(workingLine, "[NutritionExtraction:");
            if (partNumber != 0)
            {
                globalNutritionPercentage += partNumber;
            }

            partNumber = stringFindNumber(workingLine, "[DigestsBlood:");
            if (partNumber != 0)
            {
            }
            partNumber = stringFindNumber(workingLine, "[DigestsFlesh:");
            if (partNumber != 0)
            {
                consumeFlesh = true;
            }
            partNumber = stringFindNumber(workingLine, "[DigestsVeggy:");
            if (partNumber != 0)
            {
                consumeVeggy = true;
            }
            partNumber = stringFindNumber(workingLine, "[DigestsWater:");
            if (partNumber != 0)
            {
                consumeWater = true;
            }

            searchPos = endPos;
        }
    }

    searchPos = 0;

    //for(int i = 0; i != npc.inventory.size(); i++)
    for (auto i = npc.inventory.begin(); i != npc.inventory.begin(); i++)
    {
        if ((*i).insidePart != "")
        {
            (*i).hasInternalUse--;
        }
    }

    while (searchPos != npc.body.bodyParts.npos) // Individual Part Tags
    {

        searchPos = npc.body.bodyParts.find("{", searchPos);

        if (searchPos != npc.body.bodyParts.npos)
        {
            endPos = npc.body.bodyParts.find("}", searchPos);
            parts++;

            std::string workingLine;

            workingLine.append(npc.body.bodyParts, searchPos,
                               endPos - searchPos);

            float partNumber = 0;
            std::string partString = "";
            Item *partItem;

            std::string currentPart = stringFindString(workingLine, "[Name:");

            partNumber = stringFindNumber(workingLine, "[DigestsBlood:");
            partItem = getItemPtrFromVector(npc.inventory, "Blood");
            if (partNumber != 0 && partItem != nullptr)
            {

                float workAmount = partItem->amount;
                float diff = workAmount - (partNumber / 1000);

                if (diff > 0)
                {
                    partItem->amount = diff;
                    float Nutr = (workAmount - diff) *
                                 100; // TODO: Figure this out better.
                    npc.bloodwork(
                        "Nutrients",
                        Nutr * percentageBuff(globalNutritionPercentage));
                }
                else
                {
                    //*GetItemPtrfromVector(npc.inventory,"Blood").amount = 0;
                    getItemPtrFromVector(npc.inventory, "Blood")->toDelete =
                        true;
                    float nutr =
                        workAmount * 100; // TODO: Figure this out better.
                    npc.bloodwork(
                        "Nutrients",
                        nutr * percentageBuff(globalNutritionPercentage));
                }
            }

            partNumber = stringFindNumber(workingLine, "[DigestsFlesh:");
            partItem =
                getItemPtrfromVectorVarSearch(npc.inventory, "MassFlesh");
            //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
            if (partNumber != 0 && partItem != nullptr &&
                partItem->massFlesh >
                    0) // TODO: Make sure the item is in THIS PART before digesting it!
            {
                //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                float workAmount = partItem->massFlesh;
                float diff = workAmount - (partNumber / 1000);

                if (diff > 0)
                {
                    partItem->massFlesh = diff;
                    partItem->hasInternalUse = 0;
                    float Nutr = (workAmount - diff) *
                                 100; // TODO: Figure this out better.
                    npc.bloodwork(
                        "Nutrients",
                        Nutr * percentageBuff(globalNutritionPercentage));
                }
                if (partItem->massFlesh <= 0)
                {
                    //*GetItemPtrfromVector(npc.inventory,"Blood").amount = 0;
                    partItem->toDelete = true;
                    //Add Food to everyone, Make sure they go hungry to eat it, Figure out a way to Eject the empty item, Or do water! Everyone starts with water.
                    //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                }
            }

            partNumber = stringFindNumber(workingLine, "[DigestsVeggy:");
            partItem =
                getItemPtrfromVectorVarSearch(npc.inventory, "MassVeggy");
            //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
            if (partNumber != 0 && partItem != nullptr &&
                partItem->massVeggy >
                    0) // TODO: Make sure the item is in THIS PART before digesting it!
            {
                //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                float workAmount = partItem->massVeggy;
                float diff = workAmount - (partNumber / 1000);

                if (diff > 0)
                {
                    partItem->massVeggy = diff;
                    partItem->hasInternalUse = 0;
                    float Nutr = (workAmount - diff) *
                                 100; // TODO: Figure this out better.
                    npc.bloodwork(
                        "Nutrients",
                        Nutr * percentageBuff(globalNutritionPercentage));
                }
                if (partItem->massVeggy <= 0)
                {
                    partItem->toDelete = true;
                    //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                }
            }

            partNumber = stringFindNumber(workingLine, "[DigestsWater:");
            partItem =
                getItemPtrfromVectorVarSearch(npc.inventory, "MassWater");
            //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
            if (partNumber != 0 && partItem != nullptr &&
                partItem->massWater >
                    0) // TODO: Make sure the item is in THIS PART before digesting it!
            {
                //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                float workAmount = partItem->massWater;
                float diff = workAmount - (partNumber / 1000);

                if (diff > 0)
                {
                    partItem->massWater = diff;
                    partItem->hasInternalUse = 0;
                    float Nutr = (workAmount - diff) *
                                 100; // TODO: Figure this out better.
                    npc.bloodwork(
                        "Hydration",
                        Nutr * percentageBuff(globalNutritionPercentage));
                }
                if (partItem->massWater <= 0)
                {
                    partItem->toDelete = true;
                    //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                }
            }

            partString = stringFindString(workingLine, "[PoisonFilter:");
            if (partString != "")
            {

                std::vector<std::string> strVec =
                    stringFindElements(partString, ":");
                if (gvars::debug)
                    std::cout << "StrVec[0]: " << strVec[0] << std::endl;
                float Leftover =
                    npc.bloodwork(strVec[0], -atof(strVec[1].c_str()));
                if (gvars::debug)
                    std::cout << "Bloodwork leftover is: " << Leftover
                              << std::endl;
                //NPC Critter;

                for (size_t i = 0; i != strVec.size(); i++)
                {
                    if (gvars::debug)
                        std::cout << strVec[i] << std::endl;
                }
            }

            partNumber = stringFindNumber(workingLine, "[Orafice:");
            if (partNumber > 0)
            {
                //std::vector<item> * Inv = &npc.inventory;

                //for(int i = 0; i != npc.inventory.size(); i++)
                for (auto i = npc.inventory.begin(); i != npc.inventory.begin();
                     i++)
                {
                    bool foundIt = false;
                    if ((*i).insidePart == "" && (*i).massFlesh > 0 &&
                        consumeFlesh) // Awww yeeessss, We gonna eat some flesh with our Orafice!
                    {
                        (*i).insidePart = currentPart;
                        foundIt = true;
                    }

                    if ((*i).insidePart == "" && (*i).massVeggy > 0 &&
                        consumeVeggy) // Awww yeeessss, We gonna eat something with our Orafice!
                    {
                        (*i).insidePart = currentPart;
                        foundIt = true;
                    }

                    if ((*i).insidePart == "" && (*i).massWater > 0 &&
                        consumeWater) // Awww yeeessss, We gonna eat something with our Orafice!
                    {
                        (*i).insidePart = currentPart;
                        foundIt = true;
                    }

                    if (foundIt)
                    {
                        std::string chtStr;
                        chtStr.append("* ");
                        chtStr.append(npc.name);
                        chtStr.append("(" + std::to_string(npc.id) + ")");
                        chtStr.append(" has inserted ");
                        chtStr.append((*i).name);
                        chtStr.append(" into their ");
                        chtStr.append(currentPart);
                        chtStr.append("'s Orafice(");
                        chtStr.append(std::to_string(partNumber));
                        chtStr.append(").");

                        chatBox.addChat(chtStr, sf::Color(150, 150, 0));
                    }
                }
            }

            partNumber = stringFindNumber(
                workingLine, "[BloodPumpRate:"); // TODO: Do this right.
            if (partNumber != 0)
            {
                float blood = stringFindNumber(npc.bloodcontent, "[Nutrients:");
                if (blood > partNumber)
                {
                    if ((npc.maxhunger - npc.hunger) > partNumber)
                    {
                        npc.hunger += partNumber;
                        npc.bloodwork("Nutrients", -partNumber);
                    }
                }
                blood = stringFindNumber(npc.bloodcontent, "[Hydration:");
                if (blood > partNumber)
                {
                    if ((npc.maxthirst - npc.thirst) > partNumber)
                    {
                        npc.thirst += partNumber;
                        npc.bloodwork("Hydration", -partNumber);
                    }
                }
            }

            searchPos = endPos;
        }
    }

    // TODO: Can't really use this until the critter brain rewrite.
    /*
    for(int i = 0; i != npc.inventory.size(); i++)
    {
        //std::cout << "Freakkin: " << npc.inventory[i].HasInternalUse << std::endl;
        if(npc.inventory[i].HasInternalUse <= -100)
        {
            //TODO: Ejection code, Just duplicate the item onto the ground with the same coords, eh? Also add a chatbox text that a critter did this.

            item ExpelItem = npc.inventory[i];
            ExpelItem.xpos = npc.xpos;
            ExpelItem.ypos = npc.ypos;
            ExpelItem.InsidePart = "";
            ExpelItem.HasInternalUse = 0;
            worlditems.push_back(ExpelItem);
            npc.inventory[i].ToDelete = true;

            std::string ChtStr;
            ChtStr.append("* ");
            ChtStr.append(npc.name);
            ChtStr.append(AddString("(",std::to_string(npc.id),")"));
            ChtStr.append(" has ejected ");
            ChtStr.append(npc.inventory[i].name);
            ChtStr.append(" from their ");
            ChtStr.append(npc.inventory[i].InsidePart);
            ChtStr.append(".");

            chatBox.AddChat(ChtStr,sf::Color(150,150,0));
        }
    }
    */

    debug("Debug: Ending Part Loop");
    // *BodyPart Loop*

    sf::Vector2f pathFindWorkPos(0, 0);

    float tempXpos = npc.xpos;
    float tempYpos = npc.ypos;
    npc.movetimer -= npc.movetimerrate;
    npc.attacktimer--;
    if (npc.attacktimer < 0)
    {
        npc.attacktimer = 0;
    }

    if (npc.name == "Zombie")
    {
    }

    if (npc.name == "Azabul")
    {

        npc.tentArm1.x = math::clamp(npc.tentArm1.x + randz(-3, 3), -20, 20);
        npc.tentArm1.y = math::clamp(npc.tentArm1.y + randz(-3, 3), -20, 20);
        npc.tentArm2.x = math::clamp(npc.tentArm2.x + randz(-3, 3), -20, 20);
        npc.tentArm2.y = math::clamp(npc.tentArm2.y + randz(-3, 3), -20, 20);
        npc.tentEnd1.x = math::clamp(npc.tentEnd1.x + randz(-3, 3), -20, 20);
        npc.tentEnd1.y = math::clamp(npc.tentEnd1.y + randz(-3, 3), -20, 20);
        npc.tentEnd2.x = math::clamp(npc.tentEnd2.x + randz(-3, 3), -20, 20);
        npc.tentEnd2.y = math::clamp(npc.tentEnd2.y + randz(-3, 3), -20, 20);
    }

    if (npc.id == gvars::myTargetid && inputState.key[Key::Space])
    {
        npc.attacking = true;
    }
    npc.hungertimer--;
    if (npc.hungertimer <= 0)
    {
        npc.hungertimer = npc.hungertimerint;
        npc.hunger -= npc.hungerrate;
        if (npc.hunger < 0)
        {
            npc.hunger = 0;
        }
    }
    npc.thirsttimer--;
    if (npc.thirsttimer <= 0)
    {
        npc.thirsttimer = npc.thirsttimerint;
        npc.thirst -= npc.thirstrate;
        if (npc.thirst < 0)
        {
            npc.thirst = 0;
        }
    }
    npc.regentimer--;
    if (npc.regentimer <= 0 && npc.health < npc.maxhealth && npc.health > 0)
    {
        npc.regentimer = npc.regentimerint;
        npc.health += npc.regenrate;
    }
    if (npc.health <= 0)
    {
        if (npc.canmove == true)
        {
            std::string chtStr;
            chtStr.append("* ");
            chtStr.append(npc.name);
            chtStr.append("(" + std::to_string(npc.id) + ")");
            chtStr.append(" has died! ");

            chatBox.addChat(chtStr, sf::Color(200, 0, 0));
        }
        npc.canmove = false;
    }
    if (npc.health > 0)
    {
        npc.canmove = true;
    }
    if (npc.health > 0 && npc.boolblood("[Zombification:") &&
        npc.name != "Zombie")
    {
        npc.bloodwork("Zombification", -0.01);
        npc.modhealth(-0.01);
        npc.fillhunger(-0.01);
    }
    if (npc.health <= 0 && npc.boolblood("[Zombification:") &&
        npc.name != "Zombie")
    {
        npc.bloodwork("Zombification", -9999999);
        npc.tag("[ZombieHater:", -9999999);
        npc.tag("[EatsFlesh:", 1);
        std::string oldName = npc.name;
        npc.name = "Zombie";
        npc.cbaseid = 666333;
        std::string imagery = "Zombie.tga";
        for (auto const &image : texturemanager.textures)
        {
            if (image.name == imagery)
            {
                npc.img.setTexture(image.texture);
                npc.img.setOrigin(npc.img.getTextureRect().height / 2,
                                  npc.img.getTextureRect().width / 2);
            }
        }
        npc.isNPC = true;
        npc.needsFood = true;
        npc.allowedFood = false;
        npc.needsWater = false;
        npc.allowedDrink = false;
        npc.canmove = true;
        npc.alive = true;
        npc.maxhealth = 200;
        npc.health = npc.maxhealth;
        npc.breathtimerint = 0;
        npc.hungertimerint = 0;
        npc.thirsttimerint = 0;
        npc.thirst = 0; //randz(10,600);
        npc.breathtimer = 2;
        npc.hungertimer = 0;
        npc.thirsttimer = 0;
        npc.maxhunger = 1000000;
        npc.maxthirst = 1000000;
        npc.target = "Flesh";
        npc.action = "Act";
        //npc.moverateint = 1;
        //npc.moverate = 1;
        npc.movetimerrate = 400;

        std::string chtStr;
        chtStr.append("* ");
        chtStr.append(oldName);
        chtStr.append("(" + std::to_string(npc.id) + ")");
        chtStr.append(" has been zombified by Zombification! ");

        chatBox.addChat(chtStr, sf::Color(200, 0, 0));
    };
    if (inputState.key[Key::RShift] && npc.id == gvars::myTargetid)
    {
        std::cout << npc.target << "At: " << npc.targetPos.x << ":"
                  << npc.targetPos.y << std::endl;
        std::cout << "HasTarget: " << npc.hasTarget
                  << " AtTarget: " << npc.atTarget << std::endl;
    }

    if (npc.attacking)
    {
        if (npc.attacktimer <= 0)
        {
            if (gvars::debug)
            {
                std::cout << "Pre Mel Ran \n";
            }
            bool bMel = npc.hasItemType(1);
            Item mel;
            if (bMel)
            {
                mel = *npc.getItemType(1);
            }
            Item ran;
            bool bRan = npc.hasItemType(2);
            if (bRan == true)
            {
                ran = *npc.getItemType(2);
                debug("Fish n Chips");
            }
            try
            {
                bool attacked = false;
                if (gvars::debug)
                {
                    std::cout << "Pre Mel \n";
                }
                if (bMel == true)
                {
                    if (mel.type == 1 &&
                        math::closeish(npc.xpos, npc.ypos, npc.shootPos.x,
                                       npc.shootPos.y) < mel.range)
                    {
                        attacked = true;
                        std::set<int> tarz;
                        tarz = npc.melee(1, 1, GRID_SIZE);
                        for (const auto &elem : tarz)
                        {
                            npclist.at(elem)
                                .modhealth(-(randz(mel.mindam, mel.maxdam) +
                                             npc.skills.strength));
                            if (npc.hasTag("[CanLearn:"))
                            {
                                npc.skills.Train("strength");
                            }
                        }
                        if (npc.attacking == false)
                        {
                            npc.attacktimer =
                                (npc.attacktimerint -
                                 (math::clamp(npc.skills.agility / 10, 10,
                                              100)));
                        } // Melee has a different method for saying it's done.
                        if (gvars::debug)
                        {
                            std::cout << "Post Mel \n";
                        }
                    }
                }
                if (gvars::debug)
                {
                    std::cout << "Pre Ran \n";
                }
                if (bRan == true && attacked == false)
                {
                    if (ran.type == 2)
                    {
                        bool friendly = false;
                        std::set<int> ids =
                            npcTrace(npc.xpos, npc.ypos, npc.targetPos.x,
                                     npc.targetPos.y, npc.id);
                        if (!ids.empty())
                        {
                            try
                            {
                                for (const auto &id : ids)
                                {
                                    if (npclist.at(id).cbaseid == npc.cbaseid)
                                    {
                                        friendly = true;
                                    }
                                }
                            }
                            catch (std::exception &e)
                            {
                            }
                        }
                        if (friendly == false &&
                            gridposTrace(npc.xpos, npc.ypos, npc.shootPos.x,
                                         npc.shootPos.y, npc.id,
                                         npc.targetPos) == true &&
                            math::closeish(npc.xpos, npc.ypos, npc.shootPos.x,
                                           npc.shootPos.y) <= npc.viewrange)
                        {
                            int Tempx = randz(
                                0, math::clamp(
                                       100 - npc.skills.dexterity, 0,
                                       100)); // This is to mess up the aiming.
                            int Tempy =
                                randz(0, math::clamp(100 - npc.skills.dexterity,
                                                     0, 100));
                            if (npc.hasTag("[CanLearn:"))
                            {
                                npc.skills.Train("dexterity");
                            }
                            if (randz(0, 1) == 1)
                            {
                                Tempx = -Tempx;
                            }
                            if (randz(0, 1) == 1)
                            {
                                Tempy = -Tempy;
                            }
                            sf::Vector2f sp(gvars::mousePos.x,
                                            gvars::mousePos.y);
                            sf::Vector2f targ(npc.shootPos.x + Tempx,
                                              npc.shootPos.y + Tempy);
                            effects.createLine(npc.xpos, npc.ypos, targ.x,
                                               targ.y, 2, sf::Color::White);
                            std::set<int> ids = npcTrace(
                                npc.xpos, npc.ypos, targ.x, targ.y, npc.id);
                            if (!ids.empty())
                            {
                                for (size_t Ta = 0; Ta != ids.size(); Ta++)
                                {
                                    try
                                    {
                                        for (const auto &id : ids)
                                        {
                                            npclist.at(id).modhealth(
                                                -(randz(ran.mindam,
                                                        ran.maxdam) +
                                                  npc.skills.perception));
                                            if (npc.hasTag("[CanLearn:"))
                                            {
                                                npc.skills.Train("perception");
                                            }
                                            effects.createCircle(
                                                npclist.at(id).xpos,
                                                npclist.at(id).ypos, 5,
                                                sf::Color::Red);
                                        }
                                    }
                                    catch (std::exception &e)
                                    {
                                    }
                                    textList.createText(npc.xpos, npc.ypos, 11,
                                                        sf::Color::Red,
                                                        "Bang!");
                                }
                            }
                            npc.attacking = false;
                            npc.attacktimer =
                                (npc.attacktimerint -
                                 (math::clamp(npc.skills.agility / 10, 10,
                                              100)));
                        }
                        else
                        {
                            npc.attacking = false;
                        }
                        if (gvars::debug)
                        {
                            std::cout << "Post Ran \n";
                        }
                    }
                }
                else
                {
                    npc.attacking = false;
                }
            }
            catch (std::exception &e)
            {
                std::cout << "Itemerror\n";
            }
        }
        else
        {
            npc.attacking = false;
        }
    } //End of Attacking
    if (npc.canmove == true && npc.attacking == false)
    {
        //std::cout << id << " is acting." << std::endl;
        // First, Check values, to see if hungry/thirsty/hurt then check if being attacked
        // Add an IsSafe before these checks
        if (npc.isHungry() == false && npc.isThirsty() == false)
        {
            //npc.moverate = npc.moverateint;
            npc.moverate = partsWalkSpeed;
        }
        //else if(npc.IsThirsty() == false && npc.IsHungry() == true){npc.moverate = npc.moverateint/2;if(npc.moverate <= 0){npc.moverate=1;}}
        //else if(npc.IsHungry() == false  && npc.IsThirsty() == true){npc.moverate = npc.moverateint/2;if(npc.moverate <= 0){npc.moverate=1;}}
        //else if(npc.IsHungry() == true  && npc.IsThirsty() == true){npc.moverate = npc.moverateint/4;if(npc.moverate <= 0){npc.moverate=1;}}

        if (npc.name == "Zombie")
        {
            if (npc.name == "Zombie" && npc.hunger > 0)
            {
                npc.moverate = npc.moverateint * 2;
            }
            if (npc.name == "Zombie" && npc.hunger >= 0)
            {
                npc.target = "Flesh";
            }

            if (npc.name == "Zombie" && inputState.key[Key::LControl] == true)
            {
                npc.moverate = npc.moverateint * 4;
            }
            if (npc.name == "Zombie" && inputState.key[Key::LAlt] == true)
            {
                npc.moverate = 4;
            }
            float shake = 0.2;
            int bumz = randz(1, 10);
            if (bumz == 1)
            {
                npc.ypos += shake;
            }
            if (bumz == 2)
            {
                npc.xpos += shake;
            }
            if (bumz == 3)
            {
                npc.ypos -= shake;
            }
            if (bumz == 4)
            {
                npc.xpos -= shake;
            }
        }

        if (npc.action == "Act")
        { // Start of "Act" action.
            if (npc.isHurt() == true)
            {
                //target = "Meds"; //std::cout << id << " is hurt." << std::endl;
            }
            else if (npc.isHungry() == true && npc.allowedFood == true)
            {
                npc.target =
                    "Food"; //std::cout << id << " is hungry." << std::endl;
                if (npc.isThirsty())
                {
                    bool IsFood = false;
                    for (auto &worlditem : worlditems)
                    {
                        if (worlditem.type == 4)
                        {
                            IsFood = true;
                        }
                    }
                    if (IsFood == false)
                    {
                        npc.target = "Water";
                    }
                }
            }
            else if (npc.target == "Food" && npc.hunger < npc.maxhunger)
            {
            }
            else if (npc.target == "Flesh" && npc.name == "Zombie")
            {
            }
            else if (npc.isThirsty() == true)
            {
                npc.target = "Water";
                //std::cout << id << " is thirsty." << std::endl;
                bool IsWater = false;
                for (auto &worlditem : worlditems)
                {
                    if (worlditem.massWater > 0)
                    {
                        IsWater = true;
                    }
                }
                if (IsWater == false)
                {
                    npc.target = "Wander";
                }
            }
            else if (npc.target == "Water" && npc.thirst < npc.maxthirst)
            {
            }
            else
            {
                npc.target = "Wander";
                //std::cout << id << " is fine." << std::endl;
            }

            if (npc.target == "Food")
            { // Compress this later by simply directly linking the target towards the search, Probably need a bool for Item or NPC to simplfy it.
                Item *item = findClosestItemPtr(npc.xpos, npc.ypos, "Food");
                if (item != nullptr)
                {
                    if (item->massFlesh > 0)
                    {
                        npc.targetPos = sf::Vector2f(item->xpos, item->ypos);
                        npc.targetId = item->id;
                        //npc.TargetVectorId = Item.at(3);
                        npc.target = "Food";
                        npc.hasTarget = true;
                    }
                }
                else
                {
                    npc.target = "Wander";
                }
            }
            if (npc.target == "Water")
            {
                auto item = findClosestItem(npc.xpos, npc.ypos, "Water");
                if (item.found)
                {
                    npc.targetPos = sf::Vector2f(item.xpos, item.ypos);
                    npc.targetId = item.id;
                    npc.targetVectorId = item.vectorPosition;
                    npc.target = "Water";
                    npc.hasTarget = true;
                }
                else
                {
                    npc.target = "Wander";
                }
            }

            if (npc.name == "Azabul")
                npc.target = "Flesh";
            if (npc.target == "Flesh")
            {
                int closx = -100000;
                int closy = -100000;
                int closID = 0;
                bool first = true;
                for (auto &elem : npclist)
                {
                    //if(first == true){ if(zitz->cbaseid != npc.cbaseid && zitz->health > 0){
                    if (first == true)
                    {
                        if (elem.cbaseid == 110110 && elem.health > 0)
                        {
                            closx = elem.xpos;
                            closy = elem.ypos;
                            first = false;
                        }
                    }
                    // else { if(zitz->cbaseid != npc.cbaseid && zitz->health > 0) {
                    else
                    {
                        if (elem.cbaseid == 110110 && elem.health > 0)
                        {
                            int one = math::closeish(npc.xpos, npc.ypos,
                                                     elem.xpos, elem.ypos);
                            int two = math::closeish(npc.xpos, npc.ypos, closx,
                                                     closy);
                            if (one < two)
                            {
                                closx = elem.xpos;
                                closy = elem.ypos;
                                closID = elem.id;
                            }
                        }
                    }
                }
                if (closx == -100000)
                {
                    npc.target = "Wander";
                }
                if (first == false)
                {
                    npc.targetPos = sf::Vector2f(closx, closy);
                    npc.targetId = closID;
                    npc.target = "Flesh";
                    npc.hasTarget = true;
                }
                else
                {
                    npc.target = "Wander";
                }
            }

            try
            { // Jobs Section
                if (uniFact[0].jobList.size() != 0 && npc.target == "Wander" &&
                    npc.Faction == "The Alphas")
                {
                    debug("Preforming Job Routine; ");
                    for (size_t i = 0; i != uniFact[0].jobList.size(); i++)
                    {
                        debug("Starting Job " + std::to_string(i));
                        //if(!Deleting) Deleter++;
                        //Con(AddString(npc.name,JobList[i].Type));
                        if (uniFact[0].jobList[i].pItem == nullptr &&
                            uniFact[0].jobList[i].type ==
                                "PickUp") // Deleting objectless pickup jobs.
                        {                 // Deleting objectless pickup jobs.
                            uniFact[0].jobList[i].toDelete = true;
                            break;
                        }

                        if ((uniFact[0].jobList[i].pWorker == nullptr &&
                             npc.hasJob == false &&
                             uniFact[0].jobList[i].toDelete == false) ||
                            (uniFact[0].jobList[i].pWorker != nullptr &&
                             uniFact[0].jobList[i].pWorker->id == npc.id &&
                             uniFact[0].jobList[i].toDelete == false))
                        {
                            debug("Comparitive Success");
                            if (uniFact[0].jobList[i].pWorker == nullptr)
                            {
                                std::cout << npc.name
                                          << " has job: " << npc.hasJob
                                          << ", and yet :";
                                uniFact[0].jobList[i].pWorker = &npclist.at(
                                    integerIterator); // Not sure if this is technically better or worse than repointing every frame.
                                uniFact[0].jobList[i].pWorker->hasJob = true;
                                std::cout << npc.hasJob << " does now? \n";
                                //fSleep(2);
                            }

                            if (uniFact[0].jobList[i].type == "Build")
                            {
                                debug("Starting Build");

                                //if(GetItemPtrfromVector(worlditems, "Wood") != NULL) item WorkLoad = *GetItemPtrfromVector(worlditems, "Wood");
                                Item *invWood =
                                    getItemPtrFromVector(npc.inventory, "Wood");
                                Item *wldWood =
                                    getItemPtrFromVector(worlditems, "Wood");

                                if (invWood != nullptr)
                                {
                                    con("Success! I have Wood!");
                                    int x = uniFact[0].jobList[i].workPos.x;
                                    int y = uniFact[0].jobList[i].workPos.y;

                                    npc.targetPos.x = x;
                                    npc.targetPos.y = y;
                                    npc.hasTarget = true;
                                    npc.target = "BuildWoodWall";

                                    if (math::closeish(npc.xpos, npc.ypos, x,
                                                       y) <= npc.size * 3)
                                    {

                                        npc.targetPos.x = npc.xpos;
                                        npc.targetPos.y = npc.ypos;

                                        uniFact[0]
                                            .jobList[i]
                                            .completionProgress +=
                                            npc.skills.intelligence / 2;

                                        //std::cout << "JobTimer: " << UniFact[0].JobList[i].CompletionProgress << std::endl;
                                        //Effectz.CreateCircle(UniFact[0].JobList[i].WorkPos.x,UniFact[0].JobList[i].WorkPos.y, (UniFact[0].JobList[i].CompletionProgress - UniFact[0].JobList[i].CompletionTimer)/10  ,sf::Color(150,150,150,150));

                                        //std::cout << 361*(PercentIs(UniFact[0].JobList[i].CompletionTimer, UniFact[0].JobList[i].CompletionProgress)/100) << std::endl;
                                        //fSleep(4);
                                        for (
                                            float rot = 1;
                                            rot <
                                                361 *
                                                    (percentIs(
                                                         uniFact[0]
                                                             .jobList[i]
                                                             .completionTimer,
                                                         uniFact[0]
                                                             .jobList[i]
                                                             .completionProgress) /
                                                     100);
                                            rot++)
                                        {
                                            //std::cout << "Rot: " << Rot << std::endl;

                                            float xPos =
                                                x + sin(rot * PI / 180) * 10;
                                            float yPos =
                                                y + cos(rot * PI / 180) * 10;

                                            effects.createLine(
                                                x, y, xPos, yPos, 1,
                                                sf::Color(150, 150, 150, 150));
                                        }

                                        if (uniFact[0]
                                                .jobList[i]
                                                .completionProgress >=
                                            uniFact[0]
                                                .jobList[i]
                                                .completionTimer)

                                        {
                                            tiles[abs_to_index(x / 20)]
                                                 [abs_to_index(y / 20)][30]
                                                     .wall();
                                            //Tiles[abs_to_index(x/20)][abs_to_index(y/20)][30].ID = 1010;
                                            //Tiles[abs_to_index(x/20)][abs_to_index(y/20)][30].Img.setTexture( *imagemanager.GetImage("Wall.png"));
                                            invWood->toDelete = true;
                                            uniFact[0].jobList[i].toDelete =
                                                true;
                                            uniFact[0]
                                                .jobList[i]
                                                .pWorker->hasJob = false;
                                            //Crashed instantly;
                                        }
                                    }
                                }

                                else if (wldWood != nullptr)
                                {

                                    con("Partial Success! World has Wood!");
                                    //item WorkLoad = *GetItemPtrfromVector(worlditems, "Wood");

                                    npc.targetPos.x = wldWood->xpos;
                                    npc.targetPos.y = wldWood->ypos;
                                    npc.hasTarget = true;
                                    npc.target = wldWood->name;

                                    //Con(WorkLoad.id);

                                    debug("Post wood targeting, Pre "
                                          "Close-Ish function");

                                    if (math::closeish(
                                            npc.xpos, npc.ypos, wldWood->xpos,
                                            wldWood->ypos) <= npc.size)
                                    {
                                        //Con("I'm there! \n");
                                        //Deleting = true;
                                        debug("It's close!");
                                        npc.inventory.push_back(*wldWood);
                                        wldWood->toDelete = true;
                                    }
                                }
                                debug("Ended Build");
                            }

                            else if (uniFact[0].jobList[i].pItem != nullptr)
                            {
                                debug("Starting pItem != NULL");

                                npc.targetPos.x =
                                    uniFact[0].jobList[i].pItem->xpos;
                                npc.targetPos.y =
                                    uniFact[0].jobList[i].pItem->ypos;
                                npc.hasTarget = true;
                                npc.target = uniFact[0].jobList[i].pItem->name;
                                debug("Post HasTarget");

                                if (uniFact[0].jobList[i].type == "PickUp" &&
                                    math::closeish(
                                        npc.xpos, npc.ypos,
                                        uniFact[0].jobList[i].pItem->xpos,
                                        uniFact[0].jobList[i].pItem->ypos) <=
                                        npc.size)
                                {
                                    //Con("I'm there! \n");
                                    //Deleting = true;
                                    npc.inventory.push_back(
                                        *uniFact[0].jobList[i].pItem);
                                    debug("Post Inventory Pushback");

                                    uniFact[0].jobList[i].pItem->toDelete =
                                        true;
                                    /*
                            for(std::list<item>::iterator v = worlditems.begin(); v != worlditems.end(); v++ )
                            {
                                if((*v).id == UniFact[0].JobList[i].pItem->id)
                                {
                                    v->ToDelete = true;
                                    std::cout << "Confirmed. \n";
                                    fSleep(1);
                                }
                            }
                            */

                                    uniFact[0].jobList[i].pItem = nullptr;
                                    uniFact[0].jobList[i].toDelete = true;
                                    uniFact[0].jobList[i].pWorker->hasJob =
                                        false;
                                    unpointItems(worlditems);
                                    debug("Post Unpoint");
                                }

                                if (uniFact[0].jobList[i].type == "Chop" &&
                                    math::closeish(
                                        npc.xpos, npc.ypos,
                                        uniFact[0].jobList[i].pItem->xpos,
                                        uniFact[0].jobList[i].pItem->ypos) <=
                                        npc.size)
                                {
                                    debug("Post Chopcheck");
                                    //Con("I'm there! \n");
                                    //Deleting = true;
                                    //npc.inventory.push_back(*UniFact[0].JobList[i].pItem);
                                    //FUCKNUTS start here, Just made the plank, Make the tree give the planks, MmkAY?!
                                    Item woodStuffs = *getGlobalItem("Wood");

                                    debug("Post WoodStuffs");

                                    woodStuffs.xpos =
                                        uniFact[0].jobList[i].pItem->xpos;
                                    woodStuffs.ypos =
                                        uniFact[0].jobList[i].pItem->ypos;
                                    worlditems.push_back(woodStuffs);
                                    debug("Post WoodSpawn");

                                    uniFact[0].jobList[i].pItem->toDelete =
                                        true;
                                    uniFact[0].jobList[i].pItem = nullptr;
                                    uniFact[0].jobList[i].toDelete = true;
                                    uniFact[0].jobList[i].pWorker->hasJob =
                                        false;
                                    debug("Post ToDelete");
                                    unpointItems(worlditems);

                                    //items are not getting deleted, ugh.
                                }

                                debug("Ending pItem != NULL");
                            }

                            if (uniFact[0].jobList[i].type == "Dig")
                            {

                                pathFindWorkPos.x =
                                    uniFact[0].jobList[i].workPos.x;
                                pathFindWorkPos.y =
                                    uniFact[0].jobList[i].workPos.y;

                                npc.targetPos.x = pathFindWorkPos.x;
                                npc.targetPos.y = pathFindWorkPos.y;
                                npc.hasTarget = true;
                                npc.target = "DigNaturalWall";

                                if (math::closeish(
                                        npc.xpos, npc.ypos, pathFindWorkPos.x,
                                        pathFindWorkPos.y) <= npc.size * 3)
                                {

                                    npc.targetPos.x = npc.xpos;
                                    npc.targetPos.y = npc.ypos;

                                    uniFact[0].jobList[i].completionProgress +=
                                        npc.skills.strength / 2;

                                    for (float rot = 1;
                                         rot <
                                             361 *
                                                 (percentIs(
                                                      uniFact[0]
                                                          .jobList[i]
                                                          .completionTimer,
                                                      uniFact[0]
                                                          .jobList[i]
                                                          .completionProgress) /
                                                  100);
                                         rot++)
                                    { // Pretty circle progressing graphic.

                                        float xPos = pathFindWorkPos.x +
                                                     sin(rot * PI / 180) * 10;
                                        float yPos = pathFindWorkPos.y +
                                                     cos(rot * PI / 180) * 10;

                                        effects.createLine(
                                            pathFindWorkPos.x,
                                            pathFindWorkPos.y, xPos, yPos, 1,
                                            sf::Color(150, 150, 150, 150));
                                    }

                                    if (uniFact[0]
                                            .jobList[i]
                                            .completionProgress >=
                                        uniFact[0].jobList[i].completionTimer)

                                    {
                                        tiles[abs_to_index(pathFindWorkPos.x /
                                                           20)]
                                             [abs_to_index(pathFindWorkPos.y /
                                                           20)][30].stone();
                                        debug("Spawning Rocks");
                                        int tar =
                                            randz(3, 8); // Throw away random
                                        for (int z = 0; z != tar; z++)
                                        {
                                            Item stoneStuffs =
                                                *getGlobalItem("Rock");

                                            stoneStuffs.xpos =
                                                pathFindWorkPos.x +
                                                (4 - randz(1, 8));
                                            stoneStuffs.ypos =
                                                pathFindWorkPos.y +
                                                (4 - randz(1, 8));
                                            worlditems.push_back(stoneStuffs);
                                        }

                                        uniFact[0].jobList[i].toDelete = true;
                                        uniFact[0].jobList[i].pWorker->hasJob =
                                            false;
                                        debug("Dig Wall Completed");
                                    }
                                }
                            }
                        }

                        debug("End Job " + std::to_string(i));
                    }

                    removeJobs(uniFact[0].jobList);

                    debug("Finished Job Activity. \n ");
                }

                if (npc.target == "Wander" && npc.hasTarget == false)
                { // Find somewhere random to walk towards, so long as there isn't anything on that spot.
                    bool findEmpty = false;
                    while (findEmpty == false)
                    {
                        npc.targetPos =
                            sf::Vector2f(randz(700, 1300), randz(700, 1300));
                        npc.hasTarget = true;
                        if (gvars::groundmap[gvars::currentz][abs_to_index(
                                npc.targetPos.x /
                                GRID_SIZE)][abs_to_index(npc.targetPos.y /
                                                         GRID_SIZE)] != 10)
                        {
                            findEmpty = true;
                        }
                    }
                }
            }
            catch (std::exception &e)
            {
                std::cout << "Something went wrong in NPC Job Routine \n";
            }

            if (npc.name == "Shinobi" && npc.hasWeapon() == false)
            {
                auto item = findClosestItem(npc.xpos, npc.ypos, "Sword");
                if (item.found)
                {
                    npc.targetPos = sf::Vector2f(item.xpos, item.ypos);
                    npc.targetId = item.id;
                    npc.targetVectorId = item.vectorPosition;
                    npc.target = "Sword";
                    npc.hasTarget = true;
                }
                else
                {
                    npc.target = "Wander";
                }
            }
            if (npc.hasWeapon())
            { // Attack nearby Hostiles.
                int closx = -100000;
                int closy = -100000;
                bool foundOne = false;
                if (npc.hasTag("[ZombieHater:"))
                {
                    bool first = true;
                    for (auto &elem : npclist)
                    {
                        if (first == true)
                        {
                            if (elem.cbaseid == 666333 && elem.health > 0)
                            {
                                closx = elem.xpos;
                                closy = elem.ypos;
                                first = false;
                                foundOne = true;
                            }
                        }
                        else
                        {
                            if (elem.cbaseid == 666333 && elem.health > 0)
                            {
                                int one = math::closeish(npc.xpos, npc.ypos,
                                                         elem.xpos, elem.ypos);
                                int two = math::closeish(npc.xpos, npc.ypos,
                                                         closx, closy);
                                if (one < two)
                                {
                                    closx = elem.xpos;
                                    closy = elem.ypos;
                                }
                            }
                        }
                    }
                    if (first == false)
                    {
                        npc.shootPos = sf::Vector2f(closx, closy);
                        if (math::closeish(npc.xpos, npc.ypos, npc.shootPos.x,
                                           npc.shootPos.y) <= npc.viewrange)
                        {
                            npc.attacking = true;
                        }
                    }
                }
                if (npc.isHungry() == false && npc.isThirsty() == false &&
                    foundOne == true)
                {
                    npc.targetPos = sf::Vector2f(closx, closy);
                }
            }

            if (npc.closeToTarget(npc.reach, npc.targetPos) == true &&
                npc.target == "Flesh")
            {
                if (npc.attacktimer == 0)
                {

                    std::string atkType;
                    npc.attacktimer =
                        (npc.attacktimerint -
                         (math::clamp(npc.skills.agility / 10, 10, 100))) *
                        4;
                    int numba = -1;
                    int numbaz = -1;
                    int num;
                    bool foundit = false;
                    for (auto &elem : npclist)
                    {
                        if (foundit == false)
                        {
                            numba++;
                            if (math::closeishS(elem.xpos, npc.xpos) <=
                                    npc.reach &&
                                math::closeishS(elem.ypos, npc.ypos) <=
                                    npc.reach &&
                                elem.target != "Flesh")
                            {
                                numbaz++;
                                num = elem.id;
                                foundit = true;
                            }
                        }
                    }
                    if (foundit == true && npclist.at(numba).id == num)
                    {
                        npc.atTarget = false;
                        npc.hasTarget =
                            false; // TODO: Fix Zombie Attacking, Give it some Delay, and some damage based on Strength and filled belly.
                        int zDamage = randz(0, npclist.at(numba).health);
                        if (npc.name != "Azabul")
                            npc.fillhunger(zDamage);
                        if (npc.name != "Azabul")
                            npclist.at(numba)
                                .bloodwork("Zombification", 100000);
                        if (npc.name == "Azabul")
                        {
                            npc.tentArm1.x = npclist.at(numba).xpos;
                            npc.tentArm1.y = npclist.at(numba).ypos;
                            npc.tentArm2.x = npclist.at(numba).xpos;
                            npc.tentArm2.y = npclist.at(numba).ypos;
                            //BLARGGITY
                            Item blood;
                            blood.amount = zDamage;
                            blood.name = "Blood";
                            blood.insidePart = "LowerTorso";
                            npc.inventory.push_back(blood);
                            atkType = " has drunk from ";
                        }
                        npclist.at(numba).modhealth(-zDamage);

                        std::string atkStr;
                        atkStr.append("* ");
                        atkStr.append(npc.name);
                        atkStr.append("(" + std::to_string(npc.id) + ")");
                        if (atkType != "")
                            atkStr.append(" has biten ");
                        else
                            atkStr.append(atkType);
                        atkStr.append(npclist.at(numba).name);
                        atkStr.append(
                            "(" + std::to_string(npclist.at(numba).id) + ")");
                        atkStr.append(", dealing ");
                        atkStr.append(std::to_string(zDamage));
                        atkStr.append(" damage.");

                        chatBox.addChat(atkStr, sf::Color::Red);
                    }
                }
            }
            if (npc.name == "Mini Turret")
            {
                if (npc.hasWeapon() == false)
                {
                    npc.addItem("5mmCannon", 1);
                    npc.skills.dexterity = 90;
                    npc.skills.agility = 10;
                }
                if (npc.hasTag("[ZombieHater:") == false)
                {
                    npc.tag("[ZombieHater:", 1);
                    std::cout << "Giving MiniTurret [ZombieHater: \n";
                }
            }
        } // Ending of "Act" action
        if (gvars::debug)
            std::cout << "Post Act Section \n";

        { // Vision check and Activation of Path Finding.
            if (gvars::debug)
                std::cout << "Pre 'set' vision. \n";

            bool foundGoal = false;
            if (math::closeish(npc.targetPos.x, npc.targetPos.y, npc.xpos,
                               npc.ypos) <= npc.viewrange &&
                npc.cbaseid != -1337)
            {

                float ang = math::angleBetweenVectors(
                    math::Vec2f(npc.targetPos.x, npc.targetPos.y),
                    math::Vec2f(npc.xpos, npc.ypos));
                float myAngle = npc.angle;
                float difference = abs(ang - myAngle);
                if (difference > 180.0f)
                {
                    difference = 360.0f - difference;
                }
                if (difference >= -90.0f && difference <= 90.0f)
                {
                    if (gridposTrace(npc.xpos, npc.ypos, npc.targetPos.x,
                                     npc.targetPos.y, npc.id,
                                     npc.targetPos) == true)
                    {
                        foundGoal = true;
                        effects.createLine(npc.xpos, npc.ypos, npc.targetPos.x,
                                           npc.targetPos.y, 1,
                                           sf::Color::White);
                        npc.needsPath = false;
                    }
                }
            }
            gvars::sunmap[gvars::currentz][abs_to_index(
                npc.xpos / GRID_SIZE)][abs_to_index(npc.ypos / GRID_SIZE)] =
                255;
            for (int i = 0; i <= GRID_Y - 1; i++)
            { // Vision Stuffs;
                for (int t = 0; t <= GRID_X - 1; t++)
                {
                    int z = gvars::currentz;
                    if (gvars::sunmap[z][i][t] != -1)
                    {
                        if (math::closeish((i * GRID_SIZE) + 10,
                                           (t * GRID_SIZE) + 10, npc.xpos,
                                           npc.ypos) <= npc.viewrange &&
                            npc.cbaseid != -1337)
                        {

                            float ang = math::angleBetweenVectors(
                                math::Vec2f((i * GRID_SIZE) + 10,
                                            (t * GRID_SIZE) + 10),
                                math::Vec2f(npc.xpos, npc.ypos));
                            float myAngle = npc.angle;
                            float difference = abs(ang - myAngle);
                            if (difference > 180.0f)
                            {
                                difference = 360.0f - difference;
                            }
                            if (difference >= -90.0f && difference <= 90.0f)
                            {
                                /*if(gridposTrace(npc.xpos,npc.ypos,(i*GridSize)+10,(t*GridSize)+10,npc.id,Math::Vec((i*GridSize)+10,(t*GridSize)+10)) == true)
                                    {
                                         globals::sunmap[z][i][t] = 255;
                                         //Effectz.CreateCircle((i*20)+10,(t*20)+10,2,White);
                                    }*/
                            }
                        }
                    }
                }
            }
            if (foundGoal == false && npc.cbaseid != -1337)
            {
                if (gvars::debug)
                    std::cout << "FoundGoal == false";
                npc.needsPath = true;

                int previous = -1;

                if (pathFindWorkPos.x != 0)
                {
                    previous = astar::walkability[abs_to_index(
                        pathFindWorkPos.x /
                        20)][abs_to_index(pathFindWorkPos.y / 20)];
                    astar::walkability[abs_to_index(pathFindWorkPos.x / 20)]
                                      [abs_to_index(pathFindWorkPos.y / 20)] =
                                          astar::WALKABLE;
                }

                npc.pathFinding.myFindPath(
                    npc.xpos, npc.ypos, npc.targetPos.x,
                    npc.targetPos
                        .y); // TODO: This causes a crash for some reason.

                if (pathFindWorkPos.x != 0)
                {
                    astar::walkability[abs_to_index(
                        pathFindWorkPos.x /
                        20)][abs_to_index(pathFindWorkPos.y / 20)] = previous;
                }
            }
            if (true == false)
            {
                /*try{

    sList = NpcList(npc.id);
    std::set<int>::iterator iList;

    for(iList = sList.begin(); iList != sList.end(); iList++){
        float NpcPosX = npclist.at(*iList).xpos;
        float NpcPosY = npclist.at(*iList).ypos;
        if( Math::Closeish(npc.TargetPos.x,npc.TargetPos.y,npc.xpos,npc.ypos) <= npc.viewrange ){
            float Ang = Math::AngleBetweenVectors(Math::Vec(npc.TargetPos.x,npc.TargetPos.y),Math::Vec(npc.xpos,npc.ypos));
            if(Ang >= npc.angle-90 && Ang <= npc.angle+90){

                std::cout << npc.name << npc.id << " is facing " << npclist.at(*iList).name <<npclist.at(*iList).id << std::endl;
                Effectz.CreateLine(npc.xpos,npc.ypos,npc.TargetPos.x,npc.TargetPos.y,1,White);
                if(gridposTrace(npc.xpos,npc.ypos,npc.TargetPos.x,npc.TargetPos.y,npc.id,npc.TargetPos) == true){ FoundGoal = true; }
            }

        }
    }


}catch (std::exception& e){}*/
            }
            if (gvars::debug)
                std::cout << "Post 'set' vision. \n";
        }

        if (npc.action ==
            "Orders") // Vision correction currently disabled to preserve laaaaaag.
        {             // Start of "Orders action.
            /*for (int i = 0; i <= gridy-1; i++)
            {
                for( int t = 0; t <= gridx-1; t++)
                {
                    int z = globals::currentz;
                    if(globals::sunmap[z][i][t] != -1)
                    {
                        if( Math::Closeish((i*GridSize)+10,(t*GridSize)+10,npc.xpos,npc.ypos) <= npc.viewrange && npc.cbaseid != -1337)
                        {
                            float Ang = Math::AngleBetweenVectors(Math::Vec((i*GridSize)+10,(t*GridSize)+10),Math::Vec(npc.xpos,npc.ypos));
                            float MyAngle = npc.angle;
                            float difference = abs(Ang - MyAngle);
                            if (difference > 180.0f)
                            {
                                difference = 360.0f - difference;
                            }
                            if(difference >= -90.0f && difference <= 90.0f)
                            {
                                if(gridposTrace(npc.xpos,npc.ypos,(i*GridSize)+10,(t*GridSize)+10,npc.id,Math::Vec((i*GridSize)+10,(t*GridSize)+10)) == true)
                                {
                                    globals::sunmap[z][i][t] = 255;
                                    //Effectz.CreateCircle((i*20)+10,(t*20)+10,2,White);
                                }
                            }
                        }
                    }
                }
            }*/
            if (npc.target == "Goto")
            {
            }
            //if(npc.NeedsPath == false){if(npc.target != "None"){ npc.DirMove(npc.TargetPos);}}
            if (npc.atTarget == true && npc.target == "Goto")
            {
                npc.atTarget = false;
                npc.hasTarget = false;
                npc.target = "None";
            }
        } // Ending of "Orders" action
        if (npc.action == "Pickup")
        {
            //if(npc.NeedsPath == false){if(npc.target != "None"){ npc.DirMove(npc.TargetPos);}}
        }
        if ((npc.target != "None" && npc.atTarget) ||
            (npc.target != "None" &&
             math::closeish(npc.xpos, npc.ypos, npc.targetPos.x,
                            npc.targetPos.y) <= npc.size))
        {
            if (getItemVectorId(npc.targetId) != -1)
            {
                auto itemItr = worlditems.begin();
                std::advance(itemItr, getItemVectorId(npc.targetId));

                //if(Math::Closeish(npc.xpos,npc.ypos,worlditems.at(GetItemVectorId(npc.TargetId)).xpos,worlditems.at(GetItemVectorId(npc.TargetId)).ypos) <= npc.reach && worlditems.at(GetItemVectorId(npc.TargetId)).Pickupable == true)
                if (math::closeish(npc.xpos, npc.ypos, (*itemItr).xpos,
                                   (*itemItr).ypos) <= npc.reach &&
                    (*itemItr).pickupable == true)
                {
                    try
                    {
                        npc.inventory.push_back((*itemItr));
                        removeItem(npc.targetId);
                    }
                    catch (std::exception &e)
                    {
                        textList.createText(npc.xpos, npc.ypos, 11,
                                            sf::Color::Red,
                                            "Somethings wrong!");
                    };
                }
            }
            if (npc.target == "Wander")
            {
                npc.atTarget = false;
                npc.hasTarget = false;
            }
            if (true == false)
            {
                if (npc.target == "Sword")
                {
                    if (getItemVectorId(npc.targetId) != -1)
                    {
                        if (math::closeish(
                                npc.xpos, npc.ypos,
                                (listAt(worlditems,
                                        getItemVectorId(npc.targetId))).xpos,
                                (listAt(worlditems,
                                        getItemVectorId(npc.targetId))).ypos) <=
                            npc.reach * 2)
                        {
                            try
                            {
                                npc.inventory.push_back(
                                    (listAt(worlditems,
                                            getItemVectorId(npc.targetId))));
                                removeItem(npc.targetId);
                            }
                            catch (std::exception &e)
                            {
                            };
                        }
                    }
                }
                else if (npc.target == "Food")
                {
                    if (getItemVectorId(npc.targetId) != -1)
                    {
                        if (math::closeish(
                                npc.xpos, npc.ypos,
                                (listAt(worlditems,
                                        getItemVectorId(npc.targetId))).xpos,
                                (listAt(worlditems,
                                        getItemVectorId(npc.targetId))).ypos) <=
                            npc.size * 2)
                        {
                            try
                            {
                                //npc.inventory.push_back(worlditems.at(GetItemVectorId(npc.TargetId)));
                                npc.fillhunger(
                                    (listAt(worlditems,
                                            getItemVectorId(npc.targetId)))
                                        .hungervalue);
                                removeItem(npc.targetId);
                                npc.atTarget = false;
                                npc.hasTarget = false;
                                npc.targetId = -1;
                                npc.targetVectorId = -1;
                                textList.createText(npc.xpos, npc.ypos, 11,
                                                    sf::Color::Blue,
                                                    "*Crunch!*");
                            }
                            catch (std::exception &e)
                            {
                                textList.createText(npc.xpos, npc.ypos, 11,
                                                    sf::Color::Red,
                                                    "What the FUCK?!");
                            };
                        }
                    }
                }
                else if (npc.target == "Water")
                {
                    if (getItemVectorId(npc.targetId) != -1)
                    {
                        if (math::closeish(
                                npc.xpos, npc.ypos,
                                (listAt(worlditems,
                                        getItemVectorId(npc.targetId))).xpos,
                                (listAt(worlditems,
                                        getItemVectorId(npc.targetId))).ypos) <=
                            npc.size * 2)
                        {
                            try
                            {
                                //npc.inventory.push_back(worlditems.at(GetItemVectorId(npc.TargetId)));
                                npc.fillthirst(
                                    (listAt(worlditems,
                                            getItemVectorId(npc.targetId)))
                                        .thirstvalue);
                                npc.atTarget = false;
                                npc.hasTarget = false;
                                npc.targetId = -1;
                                npc.targetVectorId = -1;
                                textList.createText(npc.xpos, npc.ypos, 11,
                                                    sf::Color::Blue, "*Gulp!*");
                            }
                            catch (std::exception &e)
                            {
                                textList.createText(npc.xpos, npc.ypos, 11,
                                                    sf::Color::Red,
                                                    "What the FUCK?!");
                            };
                        }
                    }
                }
            }
        }
        if (gvars::debug)
            std::cout << "Post Item Pickups. \n";

        if (npc.movetimer <= 0)
        {
            npc.movetimer =
                (npc.movetimerint +
                 npc.movetimer); // TODO: Figure out why I added 0 to this, Year later: It was because movetimer may be less than 0, I wanted the next to turn happen as soon as possible due to it.
            if (npc.target != "None" && npc.needsPath == false &&
                math::Vec2f(npc.xpos, npc.ypos) != npc.targetPos)
            { // Walk Move
                if (npc.attacking && npc.hasWeapon("Gun") &&
                    math::closeish(npc.xpos, npc.ypos, npc.targetPos.x,
                                   npc.targetPos.y) < npc.viewrange)
                {
                }
                else if (npc.attacking && npc.hasWeapon("Sword") &&
                         math::closeish(npc.xpos, npc.ypos, npc.targetPos.x,
                                        npc.targetPos.y) < 10)
                {
                }
                else
                {
                    /*if(gridposTrace(npc.xpos,npc.ypos,npc.TargetPos.x,npc.TargetPos.y,npc.id,npc.TargetPos) != false)
                    {
                        //npc.DirMove(npc.TargetPos);
                    }*/
                    npc.dirMove(npc.targetPos);
                }
            }
            if (npc.needsPath == true &&
                math::Vec2f(npc.xpos, npc.ypos) != npc.targetPos)
            { // Acting on Path Finding.
                if (randz(0, 20) < 3)
                {
                    int previous = -1;
                    if (pathFindWorkPos.x != 0)
                    {
                        previous = astar::walkability[abs_to_index(
                            pathFindWorkPos.x /
                            20)][abs_to_index(pathFindWorkPos.y / 20)];
                        astar::walkability[abs_to_index(
                            pathFindWorkPos.x /
                            20)][abs_to_index(pathFindWorkPos.y / 20)] =
                            astar::WALKABLE;
                    }

                    npc.pathFinding.myFindPath(
                        npc.xpos, npc.ypos, npc.targetPos.x, npc.targetPos.y);

                    if (pathFindWorkPos.x != 0)
                    {
                        astar::walkability[abs_to_index(
                            pathFindWorkPos.x /
                            20)][abs_to_index(pathFindWorkPos.y / 20)] =
                            previous;
                    }
                }
                for (int i = 0; i != GRIDS; i++)
                {
                    for (int t = 0; t != GRIDS; t++)
                    {
                        // Add an Item Check here later to see if it blocks movement
                        if (tiles[i][t][30].walkable == false)
                        {
                            astar::walkability[i][t] = astar::UNWALKABLE;
                        }
                        else
                        {
                            astar::walkability[i][t] = astar::WALKABLE;
                        }
                        if (astar::walkability[i][t] > 1)
                            astar::walkability[i][t] = 0;
                    }
                }

                int previous = -1;

                if (pathFindWorkPos.x != 0)
                {
                    previous = astar::walkability[abs_to_index(
                        pathFindWorkPos.x /
                        20)][abs_to_index(pathFindWorkPos.y / 20)];
                    astar::walkability[abs_to_index(pathFindWorkPos.x / 20)]
                                      [abs_to_index(pathFindWorkPos.y / 20)] =
                                          astar::WALKABLE;
                }

                npc.pathFinding.myReadPath(1, npc.xpos, npc.ypos, npc.moverate);
                npc.dirMove(math::Vec2f(npc.pathFinding.myxPath,
                                        npc.pathFinding.myyPath));

                if (pathFindWorkPos.x != 0)
                {
                    astar::walkability[abs_to_index(
                        pathFindWorkPos.x /
                        20)][abs_to_index(pathFindWorkPos.y / 20)] = previous;
                }

                /*if (npc.xpos > npc.pathFinding.MyxPath) {npc.xpos = npc.xpos - npc.moverate;}
if (npc.xpos < npc.pathFinding.MyxPath) {npc.xpos = npc.xpos + npc.moverate;}
if (npc.ypos > npc.pathFinding.MyyPath) {npc.ypos = npc.ypos - npc.moverate;}
if (npc.ypos < npc.pathFinding.MyyPath) {npc.ypos = npc.ypos + npc.moverate;}*/

                //npc.pathFinding.MyFindPath(npc.xpos,npc.ypos,npc.TargetPos.x,npc.TargetPos.y);

                if (npc.id == gvars::myTargetid)
                {
                    for (int stuff = npc.pathFinding.mypathLocation;
                         stuff != npc.pathFinding.mypathLength; ++stuff)
                    {
                        if (stuff != 1)
                        {
                            effects.createLine(
                                npc.pathFinding.myReadPathX(1, stuff - 1),
                                npc.pathFinding.myReadPathY(1, stuff - 1),
                                npc.pathFinding.myReadPathX(1, stuff),
                                npc.pathFinding.myReadPathY(1, stuff), 2,
                                sf::Color::Blue);
                        }
                        //std::cout << npc.pathFinding.MyReadPathX(1,Stuff) << ":" << npc.pathFinding.MyReadPathY(1,Stuff) << std::endl;
                    }
                }

                if (npc.pathFinding.mypathLocation ==
                    npc.pathFinding.mypathLength)
                {
                    if (abs(npc.xpos - npc.pathFinding.myxPath) < npc.moverate)
                        npc.xpos = npc.pathFinding.myxPath;
                    if (abs(npc.ypos - npc.pathFinding.myyPath) < npc.moverate)
                        npc.ypos = npc.pathFinding.myyPath;
                    npc.action = "Act";
                    npc.needsPath = false;
                }

                //npc.TargetPos.x = xPath[1];
                //npc.TargetPos.y = yPath[1];
            }
        }
        for (auto &elem : npc.inventory)
        {
            if (elem.type == 4 &&
                npc.hunger + elem.hungervalue <= npc.maxhunger && true == false)
            {
                //if(Item->hungervalue > 0){ npc.fillhunger(1); Item->hungervalue--;}
                npc.fillhunger(elem.hungervalue);
                npc.fillthirst(elem.thirstvalue);
                elem.toDelete = true;

                std::string chtStr;
                chtStr.append("* ");
                chtStr.append(npc.name);
                chtStr.append("(" + std::to_string(npc.id) + ")");
                chtStr.append(" has consumed ");
                chtStr.append(elem.name);
                chtStr.append(".");

                chatBox.addChat(chtStr, sf::Color(150, 150, 0));
            }
            if (elem.type == 5 &&
                npc.thirst + elem.thirstvalue <= npc.maxthirst && true == false)
            {
                //if(Item->hungervalue > 0){ npc.fillhunger(1); Item->hungervalue--;}
                npc.fillhunger(elem.hungervalue);
                npc.fillthirst(elem.thirstvalue);
                elem.toDelete = true;

                std::string chtStr;
                chtStr.append("* ");
                chtStr.append(npc.name);
                chtStr.append("(" + std::to_string(npc.id) + ")");
                chtStr.append(" has consumed ");
                chtStr.append(elem.name);
                chtStr.append(".");

                chatBox.addChat(chtStr, sf::Color(150, 150, 0));
            }
        }
        if (gvars::debug)
            std::cout << "Post Item Usages. \n";

        unpointItems(npc.inventory);

        bool done = false;

        while (done == false)
        {
            bool yet = false;
            for (auto it = npc.inventory.begin(); it != npc.inventory.end();
                 ++it)
            {
                if (it->toDelete)
                {
                    std::cout << it->name << " to be deleted. \n";
                    npc.inventory.erase(it);
                    yet = true;
                    break;
                }
            }
            if (yet == false)
            {
                done = true;
            }
        }
        if (gvars::debug)
            std::cout << "Post Inventory Cleanup. \n";

    } // End of CanMove
    //npc.angle = Math::AngleBetweenVectors(Math::Vec(TempXpos,TempYpos),Math::Vec(npc.xpos,npc.ypos))-180;
    npc.angle = math::angleBetweenVectors(npc.targetPos,
                                          math::Vec2f(npc.xpos, npc.ypos));
    npc.momMove();
    //float f=Math::AngleBetweenVectors(sf::Vector2f(npc.xpos,npc.ypos), npc.TargetPos);npc.img.setRotation(f);
    if (gvars::debug)
        std::cout << npc.name << npc.id << " is done. \n";
}

void updateNpcs()
{
    if (gvars::debug)
    {
        std::cout << "Pre NPC\n";
    }
    int integerIterator = 0;
    for (auto &npc : npclist)
    {
        updateNpc(npc, integerIterator);
        integerIterator++;
    }
    if (gvars::debug)
        std::cout << "Post NPC\n";
}

void drawTiles()
{
    int z = gvars::currentz;
    int its = GRID_SIZE;
    for (int i = 0; i <= GRID_Y - 1; i++)
    {
        for (int t = 0; t <= GRID_X - 1; t++)
        {
            if ((gvars::following == true &&
                 i > (npclist.at(gvars::myTarget).xpos / GRID_SIZE) - 27 &&
                 i < (npclist.at(gvars::myTarget).xpos / GRID_SIZE) + 26) ||
                (i > gvars::currentx - 27 && i < gvars::currentx + 26))
            {
                if ((gvars::following == true &&
                     t > (npclist.at(gvars::myTarget).ypos / GRID_SIZE) - 21 &&
                     t < (npclist.at(gvars::myTarget).ypos / GRID_SIZE) + 20) ||
                    (t > gvars::currenty - 21 && t < gvars::currenty + 20))
                {
                    sf::Sprite tile;
                    if (gvars::groundmap[gvars::currentz][i][t] == 1)
                    { // dirt
                        tile.setTexture(texturemanager.getTexture("Dirt.bmp"));
                        tile.setColor(
                            sf::Color(255, 255, 255, gvars::sunmap[z][i][t]));
                        tile.setPosition(i * its, t * its);
                        window.draw(tile);
                    }
                    if (gvars::groundmap[gvars::currentz][i][t] == 3)
                    { // grass
                        tile.setTexture(texturemanager.getTexture("Grass.bmp"));
                        tile.setColor(
                            sf::Color(255, 255, 255, gvars::sunmap[z][i][t]));
                        tile.setPosition(i * its, t * its);
                        window.draw(tile);
                    }
                    if (gvars::groundmap[gvars::currentz][i][t] == 7)
                    { // stone
                        tile.setTexture(texturemanager.getTexture("Stone.bmp"));
                        tile.setColor(
                            sf::Color(255, 255, 255, gvars::sunmap[z][i][t]));
                        tile.setPosition(i * its, t * its);
                        window.draw(tile);
                    }
                    if (gvars::groundmap[gvars::currentz][i][t] == 20)
                    { // water
                        tile.setTexture(texturemanager.getTexture("Water.bmp"));
                        tile.setColor(
                            sf::Color(255, 255, 255, gvars::sunmap[z][i][t]));
                        tile.setPosition(i * its, t * its);
                        window.draw(tile);
                    }
                    if (gvars::groundmap[gvars::currentz][i][t] == 53)
                    { // lava
                        tile.setTexture(texturemanager.getTexture("Lava.bmp"));
                        tile.setColor(
                            sf::Color(255, 255, 255, gvars::sunmap[z][i][t]));
                        tile.setPosition(i * its, t * its);
                        window.draw(tile);
                    }
                    if (gvars::groundmap[gvars::currentz][i][t] == 52)
                    { // deepwater
                        tile.setTexture(
                            texturemanager.getTexture("DeepWater.bmp"));
                        tile.setColor(
                            sf::Color(255, 255, 255, gvars::sunmap[z][i][t]));
                        tile.setPosition(i * its, t * its);
                        window.draw(tile);
                    }
                }
            }
        }
    }
}

void drawNPCs()
{
    for (auto &npc : npclist)
    {
        if (npc.hasSpawned == true)
        {
            int CritterZ = npc.zpos/20;
            if(aabb(npc.xpos,npc.ypos,gvars::topLeft.x,gvars::topRight.x,gvars::topLeft.y,gvars::bottomRight.y) && CritterZ == gvars::currentz)
            {

                if (npc.name == "Azabul")
            {
                sf::Vector2f ta1 = npc.tentArm1;
                sf::Vector2f ta2 = npc.tentArm2;
                sf::Vector2f te1 = npc.tentEnd1;
                sf::Vector2f te2 = npc.tentEnd2;

                effects.createLine(npc.xpos, npc.ypos, ta1.x + npc.xpos,
                                   ta1.y + npc.ypos, 2, sf::Color(0, 200, 200),
                                   1, sf::Color(0, 255, 255));
                effects.createLine(npc.xpos, npc.ypos, ta2.x + npc.xpos,
                                   ta2.y + npc.ypos, 2, sf::Color(0, 200, 200),
                                   1, sf::Color(0, 255, 255));
                effects.createLine(
                    ta1.x + npc.xpos, ta1.y + npc.ypos,
                    te1.x + ta1.x + npc.xpos, te1.y + ta1.y + npc.ypos, 2,
                    sf::Color(0, 200, 200), 1, sf::Color(0, 255, 255));
                effects.createLine(
                    ta2.x + npc.xpos, ta2.y + npc.ypos,
                    te2.x + ta2.x + npc.xpos, te2.y + ta2.y + npc.ypos, 2,
                    sf::Color(0, 200, 200), 1, sf::Color(0, 255, 255));
                effects.drawEffects();
            }

            int alph = 255;
            npc.img.setColor(sf::Color(255, 255, 255, alph));
            npc.img.setScale(gvars::scalex, gvars::scaley);
            npc.img.setOrigin(npc.img.getTextureRect().width / 2,
                              npc.img.getTextureRect().height / 2);
            if (npc.race == "Human")
            {
                npc.img.setScale(0.5, 0.5);
                npc.img.setRotation(npc.angle);
            }

            npc.drawImg();
            effects.createCircle(npc.xpos, npc.ypos, npc.size,
                                 sf::Color(50, 50, 50, 50));


            }



        }
    }
    debug("Done drawing NPCs");
}

void drawItems()
{
    //App.setActive(true);

    for (auto &worlditem : worlditems)
    {
        int itemZ = worlditem.zpos/20;
        if(aabb(worlditem.xpos,worlditem.ypos,gvars::topLeft.x,gvars::topRight.x,gvars::topLeft.y,gvars::bottomRight.y) && itemZ == gvars::currentz)
        {
            worlditem.img.setColor(sf::Color(255, 255, 255, 255));
            worlditem.img.setScale(gvars::scalex, gvars::scaley);
            worlditem.drawImg();
        }
    }
    debug("Done Drawing Items");
}

void lightTrail(int x, int y, int z)
{
    int curx = math::clamp(x, 0, GRID_X - 1);
    int cury = math::clamp(y, 0, GRID_Y - 1);
    int curz = math::clamp(z, 0, GRID_Z - 1);
    gvars::sunmap[curz][curx][cury] = 255;
}

void displayChat(sf::Vector2f position)
{
    if (gCtrl.phase != "MainMenu")
        effects.createSquare(
            position.x - 10, position.y + 10, position.x + 500,
            (position.y - ((chatBox.chatStorage.size() + 1) * 10)),
            sf::Color(0, 0, 0, 100), 2, sf::Color::Cyan);

    for (size_t i = 0; i != chatBox.chatStorage.size(); i++)
    {
        textList.createText(
            position.x,
            (position.y - ((chatBox.chatStorage.size()) * 10)) + (i * 10), 11,
            chatBox.chatStorage[i].color, chatBox.chatStorage[i].line);
    }
}

void drawStuffs()
{

    //sf::Context context;
    //App.setActive(true);
    drawNewTiles();

    //DrawPlanets();
    drawItems();

    drawNPCs();

    drawJobList(window.getView().getCenter().x - 500,
                window.getView().getCenter().y);
    debug("Drew Joblist");

    displayChat(sf::Vector2f(gvars::bottomLeft.x + 5, gvars::bottomLeft.y - 5));
    debug("Drew Chat");

    effects.drawEffects();
    debug("Drew Effects");
    //    DrawPlanets();//Remove this one, Reenable previous

    for (auto &button : vSquareButtonList)
    {
        button.draw();
    }
    vSquareButtonList.clear();
    debug("Drew and Cleared square buttons");

    textList.drawTextz();
    debug("Drew Text");

    for (auto &button : vButtonList)
    {
        button.draw();
    }
    vButtonList.clear();
    debug("Drew and Cleared buttons");

    gvars::drawStuffsDone = true;
}

Item *getGlobalItem(std::string strtype)
{
    if (gvars::debug)
    {
        std::cout << "Getting" << strtype << " \n";
    }
    for (auto &elem : itemmanager.globalItems)
    {
        if (elem.name == strtype)
        {
            if (gvars::debug)
            {
                std::cout << "Found" << strtype << " \n";
            }
            return &elem;
        }
    }
    if (gvars::debug)
    {
        std::cout << "Didn't Find" << strtype << " \n";
    }

    return nullptr;
}

Npc *getCritter(int id)
{
    if (gvars::debug)
    {
        std::cout << "Getting critter(" << id << ") \n";
    }
    for (auto &elem : npclist)
    {
        if (elem.id == id)
        {
            if (gvars::debug)
            {
                std::cout << "Found critter(" << id << ") \n";
            }
            return &elem;
        }
    }
    if (gvars::debug)
    {
        std::cout << "Didn't Find critter(" << id << ") \n";
    }
    return nullptr;
}

void removeNPCs()
{
    bool done = false;
    while (done == false)
    {
        bool yet = false;
        for (auto it = npclist.begin(); it != npclist.end(); ++it)
        {
            if (it->toDelete)
            {
                std::cout << it->name << " to be deleted. \n";
                npclist.erase(it);
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


void buildMicroPatherTest()
{
    for (int x = 0; x != worldSizeX; x++)
        for (int y = 0; y != worldSizeY; y++)
            for (int z = 0; z != worldSizeZ; z++)
            {
                grid[x][y][z].setPos(x, y, z);
                grid[x][y][z].type = 0;
                if (randz(0, 10) == 10)
                    grid[x][y][z].type = 1;
            }

    grid[16][16][0].type = 0;
    grid[16][16][1].type = 2;

    grid[5][1][0].type = 10;
    grid[5][1][0].teleporter = true;
    grid[5][1][0].telePos = Vec3(20, 30, 2);
    grid[20][30][2].type = 10;
    grid[20][30][2].teleporter = true;
    grid[20][30][2].telePos = Vec3(5, 1, 0);

    grid[16][8][1].type = 0;
    grid[16][8][2].type = 2;

    grid[0][0][0].type = 0;
    grid[worldSizeX - 1][worldSizeY - 1][worldSizeZ - 1].type = 0;
}


int main()
{
    srand(clock());
    /* Perhaps have both Upstairs and Downstairs as the same thing? would this work? How to deal with the 'recieving' position.
            I.E. What if there's a staircase that leads up, but on the upper level, there's a wall where the stairs lead.*/
    /* 0 = open, 1 = wall, 2 = upstairs, 3 = downstairs */

    buildMicroPatherTest();

    initializeTilePositions();

    window.create(sf::VideoMode(RESOLUTION.x, RESOLUTION.y, 32),
                  randomWindowName());

    textList.loadFont();

    bool paused = false;
    //Debug = true;

    bool plyAct = false;
    window.setFramerateLimit(30); // 0 is unlimited
    sf::View planetary(CENTER, HALF_SIZE);

    window.setVerticalSyncEnabled(true);

    // Various temporary variables used for testing.
    int testage = 0;
    int testage2 = 0;
    float xxx = 0;                      // global
    float yyy = 0;                      // global
    int speeds = 1;                     // global
    int xanchor = 0;                    // global
    int yanchor = 0;                    // global
    float degrees = randz(.0f, 359.0f); // global
    int radius = 200;

    gvars::view1.zoom(2);
    if (true == false)
    {   // TODO: Fix this icon crap.
        /*sf::Image icon;
    icon.LoadFromFile("gfx/icon.tga");
    App.SetIcon(32,32,icon.GetPixelsPtr());
    if (!Images.LoadFromFile("gfx/Images.tga"))return EXIT_FAILURE;
    sf::Image Aim;
    if (!Aim.LoadFromFile("gfx/Aim.tga"))return EXIT_FAILURE;*/
    }

    texturemanager.init();
    itemmanager.initializeItems();
    npcmanager.initializeCritters();

    // Building the players faction, This is temporary.
    g_pf.name = "The Alphas";
    g_pf.playerControlled = true;
    g_pf.initialized = true;
    uniFact.push_back(g_pf);
    conFact = &uniFact[0];

    // Setting the initial game phase.
    gCtrl.phase = "MainMenu";
    //gCtrl.phase = "MicroPatherTest";

    // For A*
    astar::init();

    while (window.isOpen())
    {
        if (gvars::cycleGrowth)
        {
            gvars::cycleRed.g++;
            gvars::cycleRed.b++;
            gvars::cycleGreen.r++;
            gvars::cycleGreen.b++;
            gvars::cycleBlue.r++;
            gvars::cycleBlue.g++;
            if (gvars::cycleRed.g >= 255)
                gvars::cycleGrowth = false;
        }
        else
        {
            gvars::cycleRed.g--;
            gvars::cycleRed.b--;
            gvars::cycleGreen.r--;
            gvars::cycleGreen.b--;
            gvars::cycleBlue.r--;
            gvars::cycleBlue.g--;
            if (gvars::cycleRed.g <= 0)
                gvars::cycleGrowth = true;
        }

        gvars::constantRotation++;
        if(gvars::constantRotation > 359)
            gvars::constantRotation = 0;

        removeNPCs();
        sf::Event event;
        while (window.pollEvent(event))
        {
            inputState.updateFromEvent(event);
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseWheelMoved)
            {
                if (event.mouseWheel.delta > 0)
                {
                    std::cout << "Zoom Out \n";
                    if (gvars::cameraZoom < 2)
                    {
                        gvars::cameraZoom = gvars::cameraZoom / 0.5;
                        //CameraSize
                        gvars::view1.zoom(0.5);
                    }
                }
                if (event.mouseWheel.delta < 0)
                {
                    std::cout << "Zoom In \n";
                    if (gvars::cameraZoom > 0.5)
                    {
                        gvars::cameraZoom = gvars::cameraZoom / 2;
                        gvars::view1.zoom(2);
                    }
                }

                //Develop the scaling camera, Probably using .setsize instead of zoom.
                //http://www.sfml-dev.org/tutorials/2.0/graphics-view.php
            }

            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                //sf::FloatRect visibleArea(0, 0, Event.size.width, Event.size.height);
                //View1.setViewport(visibleArea);
                //App.setView(sf::View(visibleArea));
            }

            if (event.type == sf::Event::LostFocus)
            {
                gvars::inFocus = false;
            }
            if (event.type == sf::Event::GainedFocus)
            {
                gvars::inFocus = true;
            }
        }
        window.setView(gvars::view1);
        gvars::buttonClicked = false;
        gvars::buttonClickedTime--; // Misleading Variable name, Sorry!
        if (gvars::buttonClickedTime < 0)
            gvars::buttonClickedTime = 0;

        inputState.update();
        sf::Vector2f mouseStagnationCheck = gvars::mousePos;

        gvars::mousePos =
            window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (mouseStagnationCheck == gvars::mousePos)
            gvars::mouseStagnation++;
        else
            gvars::mouseStagnation = 0;

        gvars::topLeft = sf::Vector2f(gvars::view1.getCenter().x - HALF_SIZE.x,
                                      gvars::view1.getCenter().y - HALF_SIZE.y);
        gvars::topRight =
            sf::Vector2f(gvars::view1.getCenter().x + HALF_SIZE.x,
                         gvars::view1.getCenter().y - HALF_SIZE.y);
        gvars::bottomLeft =
            sf::Vector2f(gvars::view1.getCenter().x - HALF_SIZE.x,
                         gvars::view1.getCenter().y + HALF_SIZE.y);
        gvars::bottomRight =
            sf::Vector2f(gvars::view1.getCenter().x + HALF_SIZE.x,
                         gvars::view1.getCenter().y + HALF_SIZE.y);

        textList.createText(cz(gvars::topRight.x - cz(50)),
                            cz(gvars::topRight.y + cz(50)), cz(11),
                            sf::Color::White, "x", "", gvars::cameraZoom);
        textList.createText((gvars::topRight.x - 50), (gvars::topRight.y + 50),
                            (11) / gvars::cameraZoom, sf::Color::White, "x", "",
                            gvars::cameraZoom);

        if (inputState.key[Key::K].time == 1)
        { // Generates a random name from GenerateName(); and puts it into the console.
            std::cout << generateName() << std::endl;
        }

        if (inputState.key[Key::G].time == 1)
        { // Fling all critters south.
            for (auto &i : npclist)
            {
                i.momentum = sf::Vector2f(0, 100);
            }
        }
        if (inputState.key[Key::H].time == 1)
        { // Fling all critters north.
            for (auto &i : npclist)
            {
                i.momentum = sf::Vector2f(0, -100);
            }
        }

        // Game Mode Loops ================================================================================

        if (inputState.key[Key::R])
        { // Debug (de)activation
            if (!gvars::debug)
            {
                gvars::debug = true;
                fSleep(0.2);
            }
            else if (gvars::debug)
            {
                gvars::debug = false;
                fSleep(0.2);
            }
        }

        if (gCtrl.phase == "MicroPatherTest")
        {
            if (inputState.key[Key::Left])
            {
                gvars::currentx--;
                plyAct = true;
            }
            if (inputState.key[Key::Right])
            {
                gvars::currentx++;
                plyAct = true;
            }
            if (inputState.key[Key::Up])
            {
                gvars::currenty--;
                plyAct = true;
            }
            if (inputState.key[Key::Down])
            {
                gvars::currenty++;
                plyAct = true;
            }

            for (int x = 0; x != worldSizeX; x++)
                for (int y = 0; y != worldSizeY; y++)
                    for (int z = 0; z != worldSizeZ; z++)
                    {
                        sf::Color wallColor(0, 0, 0);
                        if (z == 0)
                            wallColor.r = 150;
                        else if (z == 1)
                            wallColor.g = 150;
                        else //if (z == 2)
                            wallColor.b = 150;
                        if (grid[x][y][z].type == 10)
                            wallColor = sf::Color(255, 0, 255);

                        if (grid[x][y][z].type == 1 || grid[x][y][z].type == 10)
                            effects.createSquare(x * 20, y * 20, (x + 1) * 20,
                                                 (y + 1) * 20, wallColor);
                    }
            /* Adding a second loop to make sure the yellow squares are on top of the display, Temporary only. */
            for (int x = 0; x != worldSizeX; x++)
                for (int y = 0; y != worldSizeY; y++)
                    for (int z = 0; z != worldSizeZ; z++)
                    {
                        if (grid[x][y][z].type == 2)
                            effects.createSquare(
                                (x * 20) + 2, (y * 20) + 2, ((x + 1) * 20) - 2,
                                ((y + 1) * 20) - 2, sf::Color::Yellow, 4,
                                sf::Color::White);
                    }

            Vec3 startPos(0, 0, 0);
            Vec3 endPos(worldSizeX - 1, worldSizeY - 1, worldSizeZ - 1);
            world.makePath(startPos, endPos);

            //world.DrawPath();
            world.drawStoredPath();

            textList.createText(700, 100, 10, sf::Color::White,
                                "Total Tile Movement: ", "",
                                world.microPath.size());
            textList.createText(700, 120, 10, sf::Color::White,
                                "Toggle Teleporters: [Space]");
            textList.createText(700, 140, 10, sf::Color::White,
                                "Randomize Tiles: [Tab]");

            if (inputState.key[Key::Space].time == 1)
            {
                world.toggleTeleporter();
                world.pather->Reset();
            }
            if (inputState.key[Key::Tab].time == 1)
            {
                buildMicroPatherTest();
                world.pather->Reset();
            }
        }

        if (gCtrl.phase == "Local")
        { //=======================================================*Local*============================================================================
            if (gvars::debug)
                textList.createText((gvars::currentx - 2) * GRID_SIZE,
                                    (gvars::currenty + 1) * GRID_SIZE, 11,
                                    sf::Color::Red, "Debug On");

            if (inputState.key[Key::V].time == 1)
                chatBox.addChat(
                    randomWindowName(),
                    sf::Color(randz(0, 255), randz(0, 255), randz(0, 255)));

            squadHud();

            if (inputState.key[Key::LShift].time > 0)
            {
                int x = gvars::bottomLeft.x;
                int y = gvars::bottomLeft.y;
                effects.createSquare(x + 20, y - 20, x + 40, y - 40,
                                     sf::Color(0, 100, 255));
                textList.createText(x + 20, y - 30, 11, sf::Color::White,
                                    "Build");
            }
            if (inputState.rmbTime == 1 && inputState.key[Key::LShift])
                rmbMenuTile(gvars::mousePos);

            if (inputState.key[Key::LShift] &&
                inputState.key[Key::Tab]) // Debug NPC Spawn Menu
            {

                gCtrl.menuType = "DebugCritterSpawn";
                menuPopUp();
            }
            else if (inputState.key[Key::Tab] == true &&
                     inputState.key[Key::LShift] ==
                         false) // Debug Item Spawn Menu TODO: Sort them by Type.
            {
                gCtrl.menuType = "DebugItemSpawn";
                menuPopUp();
            }
            else if (gCtrl.menuType == "DebugItemSpawn" ||
                     gCtrl.menuType == "DebugCritterSpawn")
            {
                gCtrl.menuType = "NULL";
                gCtrl.menuPos = sf::Vector2f(-10000, -10000);
                gCtrl.menuEndPos = sf::Vector2f(-10000, -10000);
                menuPtrCon.pItem = nullptr;
                menuPtrCon.pNPC = nullptr;
                menuPtrCon.pTile = nullptr;
                con("Closing DebugMenus Due To Lack of Key Presses");
            }
            //                       if(Key.lshift && Key.z){GC.ZombieSwarmLocal();}

            if (inputState.key[Key::LShift] &&
                inputState.key[Key::Z]) // Debug Stuffs Menu
            {
                gCtrl.menuType = "DebugFunctions";
                menuPopUp();
            }

            if (inputState.key[Key::Left])
            {
                gvars::currentx--;
                plyAct = true;
            }
            if (inputState.key[Key::Right])
            {
                gvars::currentx++;
                plyAct = true;
            }
            if (inputState.key[Key::Up])
            {
                gvars::currenty--;
                plyAct = true;
            }
            if (inputState.key[Key::Down])
            {
                gvars::currenty++;
                plyAct = true;
            }

            if (gvars::initalZeds)
                gCtrl.wave();

            rightMouseButtonContextMenu();

            if (inputState.key[Key::LShift] == true &&
                inputState.key[Key::Left])
            {
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                plyAct = true;
            } //Sprite.Move(-100 * ElapsedTime, 0);
            if (inputState.key[Key::LShift] == true &&
                inputState.key[Key::Right])
            {
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                plyAct = true;
            } //Sprite.Move( 100 * ElapsedTime, 0);
            if (inputState.key[Key::LShift] == true && inputState.key[Key::Up])
            {
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                plyAct = true;
            } //Sprite.Move(0, -100 * ElapsedTime);
            if (inputState.key[Key::LShift] == true &&
                inputState.key[Key::Down])
            {
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                plyAct = true;
            } //Sprite.Move(0,  100 * ElapsedTime);
            if (inputState.key[Key::Comma] == true &&
                inputState.key[Key::LShift] == true &&
                gvars::currentz <= GRIDS - 1)
            {
                gvars::currentz++;
                plyAct = true;
                fSleep(0.1f);
            } //Sprite.Move(0, -100 * ElapsedTime);
            if (inputState.key[Key::Period] == true &&
                inputState.key[Key::LShift] == true && gvars::currentz >= 1)
            {
                gvars::currentz--;
                plyAct = true;
                fSleep(0.1f);
            } //Sprite.Move(0,  100 * ElapsedTime);
            if (inputState.key[Key::Comma] == true &&
                inputState.key[Key::RShift] == true &&
                gvars::currentz <= GRIDS - 1)
            {
                gvars::currentz++;
                plyAct = true;
                fSleep(0.1f);
            } //Sprite.Move(0, -100 * ElapsedTime);
            if (inputState.key[Key::Period] == true &&
                inputState.key[Key::RShift] == true && gvars::currentz >= 1)
            {
                gvars::currentz--;
                plyAct = true;
                fSleep(0.1f);
            } //Sprite.Move(0,  100 * ElapsedTime);
            if (gvars::myTarget == -1)
            {
                gvars::following = false;
            }
            else if (inputState.key[Key::C])
            {
                gvars::following = true;
            }
            if (gvars::following)
            {
                gvars::view1.setCenter(npclist.at(gvars::myTarget).xpos,
                                       npclist.at(gvars::myTarget).ypos);
            }

            effects.createSquare(
                32 * 20, 32 * 20, 64 * 20, 64 * 20, sf::Color(0, 0, 0, 0), 1,
                sf::Color::
                    Cyan); // These two are the chunk borders, Debug stuffs so you can see where the loading boundries are. (As if you needed 'em, Damn thing lags a bit, Should thread each loading bit soon.)
            effects.createSquare(
                0 * 20, 0 * 20, 32 * 20, 32 * 20, sf::Color(0, 0, 0, 0), 1,
                sf::Color::
                    Red); // This one reachs from 0 to 32 only because it draws from the left, not the center.

            if (inputState.key[Key::LControl].time > 10)
            {
                int Variable =
                    tiles[abs_to_index(gvars::mousePos.x / 20)][abs_to_index(
                        gvars::mousePos.y / 20)][gvars::currentz].id;
                Vec3 Pos = tiles[abs_to_index(gvars::mousePos.x / 20)][abs_to_index(
                        gvars::mousePos.y / 20)][gvars::currentz].getPos();

                textList.createText(gvars::mousePos.x, gvars::mousePos.y, 11,
                                    sf::Color::Red, "ID: ", "", Variable, ", Pos (" + std::to_string(Pos.x) + "/" + std::to_string(Pos.y) + "/" + std::to_string(Pos.z) + ")");
            }

            bool transitioning = false;
            if (gvars::currenty > 64)
            {
                tilesGoUp();
                gvars::currenty = 33;

                for (size_t i = 0; i != npclist.size(); i++)
                {
                    npclist.at(i).ypos += -640;
                    npclist.at(i).targetPos.y += -640;
                }

                for (auto &worlditem : worlditems)
                    (worlditem).ypos += -640;

                for (size_t i = 0; i != uniFact[0].jobList.size(); i++)
                    uniFact[0].jobList[i].workPos.y += -640;

                transitioning = true;
                std::string line;
                line = loadCritters(sf::Vector2i(gvars::currentregionx - 1,
                                                 gvars::currentregiony + 1),
                                    "BottomLeft", gvars::currentplanet);
                remove(line.c_str());
                line = loadCritters(sf::Vector2i(gvars::currentregionx,
                                                 gvars::currentregiony + 1),
                                    "Bottom", gvars::currentplanet);
                remove(line.c_str());
                line = loadCritters(sf::Vector2i(gvars::currentregionx + 1,
                                                 gvars::currentregiony + 1),
                                    "BottomRight", gvars::currentplanet);
                remove(line.c_str());
                line = loadItems(sf::Vector2i(gvars::currentregionx - 1,
                                              gvars::currentregiony + 1),
                                 "BottomLeft", gvars::currentplanet);
                remove(line.c_str());
                line = loadItems(sf::Vector2i(gvars::currentregionx,
                                              gvars::currentregiony + 1),
                                 "Bottom", gvars::currentplanet);
                remove(line.c_str());
                line = loadItems(sf::Vector2i(gvars::currentregionx + 1,
                                              gvars::currentregiony + 1),
                                 "BottomRight", gvars::currentplanet);
                remove(line.c_str());
            }
            if (gvars::currenty < 32)
            {
                tilesGoDown();
                gvars::currenty = 63;
                for (size_t i = 0; i != npclist.size(); i++)
                {
                    npclist.at(i).ypos += 640;
                    npclist.at(i).targetPos.y += 640;
                }

                //for(int i = 0; i != worlditems.size(); i++) worlditems.at(i).ypos += 640;
                for (auto &worlditem : worlditems)
                    (worlditem).ypos += 640;

                for (size_t i = 0; i != uniFact[0].jobList.size(); i++)
                    uniFact[0].jobList[i].workPos.y += 640;

                transitioning = true;
                std::string line;
                line = loadCritters(sf::Vector2i(gvars::currentregionx - 1,
                                                 gvars::currentregiony - 1),
                                    "TopLeft", gvars::currentplanet);
                remove(line.c_str());
                line = loadCritters(sf::Vector2i(gvars::currentregionx,
                                                 gvars::currentregiony - 1),
                                    "Top", gvars::currentplanet);
                remove(line.c_str());
                line = loadCritters(sf::Vector2i(gvars::currentregionx + 1,
                                                 gvars::currentregiony - 1),
                                    "TopRight", gvars::currentplanet);
                remove(line.c_str());
                line = loadItems(sf::Vector2i(gvars::currentregionx - 1,
                                              gvars::currentregiony - 1),
                                 "TopLeft", gvars::currentplanet);
                remove(line.c_str());
                line = loadItems(sf::Vector2i(gvars::currentregionx,
                                              gvars::currentregiony - 1),
                                 "Top", gvars::currentplanet);
                remove(line.c_str());
                line = loadItems(sf::Vector2i(gvars::currentregionx + 1,
                                              gvars::currentregiony - 1),
                                 "TopRight", gvars::currentplanet);
                remove(line.c_str());
            }
            if (gvars::currentx > 64)
            {
                tilesGoLeft();
                gvars::currentx = 33;
                for (size_t i = 0; i != npclist.size(); i++)
                {
                    npclist.at(i).xpos += -640;
                    npclist.at(i).targetPos.x += -640;
                }

                //for(int i = 0; i != worlditems.size(); i++) worlditems.at(i).xpos += -640;
                for (auto &worlditem : worlditems)
                    (worlditem).xpos += -640;

                for (size_t i = 0; i != uniFact[0].jobList.size(); i++)
                    uniFact[0].jobList[i].workPos.x += -640;

                transitioning = true;
                std::string line;
                line = loadCritters(sf::Vector2i(gvars::currentregionx + 1,
                                                 gvars::currentregiony - 1),
                                    "TopRight", gvars::currentplanet);
                remove(line.c_str());
                line = loadCritters(sf::Vector2i(gvars::currentregionx + 1,
                                                 gvars::currentregiony),
                                    "Right", gvars::currentplanet);
                remove(line.c_str());
                line = loadCritters(sf::Vector2i(gvars::currentregionx + 1,
                                                 gvars::currentregiony + 1),
                                    "BottomRight", gvars::currentplanet);
                remove(line.c_str());
                line = loadItems(sf::Vector2i(gvars::currentregionx + 1,
                                              gvars::currentregiony - 1),
                                 "TopRight", gvars::currentplanet);
                remove(line.c_str());
                line = loadItems(sf::Vector2i(gvars::currentregionx + 1,
                                              gvars::currentregiony),
                                 "Right", gvars::currentplanet);
                remove(line.c_str());
                line = loadItems(sf::Vector2i(gvars::currentregionx + 1,
                                              gvars::currentregiony + 1),
                                 "BottomRight", gvars::currentplanet);
                remove(line.c_str());
            }
            if (gvars::currentx < 32)
            {
                con("Starting GoRight");
                tilesGoRight();
                con("Ending GoRight");
                gvars::currentx = 63;
                con("Starting GoRight with NPC's and Items");
                for (size_t i = 0; i != npclist.size(); i++)
                {
                    npclist.at(i).xpos += 640;
                    npclist.at(i).targetPos.x += 640;
                }

                //for(int i = 0; i != worlditems.size(); i++) worlditems.at(i).xpos += 640;
                for (auto &worlditem : worlditems)
                    (worlditem).xpos += 640;

                for (size_t i = 0; i != uniFact[0].jobList.size(); i++)
                    uniFact[0].jobList[i].workPos.x += 640;

                con("Done GoRight with NPC's and Items");
                transitioning = true;
                std::string line;
                line = loadCritters(sf::Vector2i(gvars::currentregionx - 1,
                                                 gvars::currentregiony - 1),
                                    "TopLeft", gvars::currentplanet);
                remove(line.c_str());
                line = loadCritters(sf::Vector2i(gvars::currentregionx - 1,
                                                 gvars::currentregiony),
                                    "Left", gvars::currentplanet);
                remove(line.c_str());
                line = loadCritters(sf::Vector2i(gvars::currentregionx - 1,
                                                 gvars::currentregiony + 1),
                                    "BottomLeft", gvars::currentplanet);
                remove(line.c_str());
                line = loadItems(sf::Vector2i(gvars::currentregionx - 1,
                                              gvars::currentregiony - 1),
                                 "TopLeft", gvars::currentplanet);
                remove(line.c_str());
                line = loadItems(sf::Vector2i(gvars::currentregionx - 1,
                                              gvars::currentregiony),
                                 "Left", gvars::currentplanet);
                remove(line.c_str());
                line = loadItems(sf::Vector2i(gvars::currentregionx - 1,
                                              gvars::currentregiony + 1),
                                 "BottomLeft", gvars::currentplanet);
                remove(line.c_str());
            }

            if (transitioning == true)
            {
                for (size_t i = 0; i != npclist.size(); i++)
                {

                    if (npclist.at(i).xpos > 1920 && npclist.at(i).ypos < 640)
                    {
                        npclist.at(i).xpos =
                            npclist.at(i).xpos - 640 - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx + 2,
                                                  gvars::currentregiony - 1),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }
                    else if (npclist.at(i).xpos > 1920 &&
                             npclist.at(i).ypos > 1280)
                    {
                        npclist.at(i).xpos =
                            npclist.at(i).xpos - 640 - 640 - 640;
                        npclist.at(i).ypos = npclist.at(i).ypos - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx + 2,
                                                  gvars::currentregiony + 1),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }

                    else if (npclist.at(i).xpos < 0 &&
                             npclist.at(i).ypos > 1280)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos + 640;
                        npclist.at(i).ypos = npclist.at(i).ypos - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx - 2,
                                                  gvars::currentregiony + 1),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }
                    else if (npclist.at(i).xpos < 0 && npclist.at(i).ypos < 640)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos + 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx - 2,
                                                  gvars::currentregiony - 1),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }

                    else if (npclist.at(i).ypos < 0 &&
                             npclist.at(i).xpos > 1280)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos - 640 - 640;
                        npclist.at(i).ypos = npclist.at(i).ypos + 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx + 1,
                                                  gvars::currentregiony - 2),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }
                    else if (npclist.at(i).ypos < 0 && npclist.at(i).xpos < 640)
                    {
                        npclist.at(i).ypos = npclist.at(i).ypos + 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx - 1,
                                                  gvars::currentregiony - 2),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }

                    else if (npclist.at(i).ypos > 1920 &&
                             npclist.at(i).xpos > 1280)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos - 640 - 640;
                        npclist.at(i).ypos =
                            npclist.at(i).ypos - 640 - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx + 1,
                                                  gvars::currentregiony + 2),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }
                    else if (npclist.at(i).ypos > 1920 &&
                             npclist.at(i).xpos < 640)
                    {
                        npclist.at(i).ypos =
                            npclist.at(i).ypos - 640 - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx - 1,
                                                  gvars::currentregiony + 2),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }

                    //HAGGINABAGGINA  Some reason, When a critter is saved, It'll have more than 640 for it's position, This is unacceptable.

                    else if (npclist.at(i).xpos > 1920)
                    {
                        npclist.at(i).xpos =
                            npclist.at(i).xpos - 640 - 640 - 640;
                        npclist.at(i).ypos = npclist.at(i).ypos - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx + 2,
                                                  gvars::currentregiony),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }
                    else if (npclist.at(i).ypos > 1920)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos - 640;
                        npclist.at(i).ypos =
                            npclist.at(i).ypos - 640 - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx,
                                                  gvars::currentregiony + 2),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }
                    else if (npclist.at(i).xpos < 0)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos + 640;
                        npclist.at(i).ypos = npclist.at(i).ypos - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx - 2,
                                                  gvars::currentregiony),
                                npclist.at(i));
                        npclist.at(i).toDelete = true;
                    }
                    else if (npclist.at(i).ypos < 0)
                    {
                        npclist.at(i).xpos = npclist.at(i).xpos - 640;
                        npclist.at(i).ypos = npclist.at(i).ypos + 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx,
                                                  gvars::currentregiony - 2),
                                npclist.at(i));

                        npclist.at(i).toDelete = true;
                    }
                }

                removeNPCs();
                /*
                for(int i = 0; i != worlditems.size(); i++)
                {
                   if(worlditems.at(i).xpos > 1920)
                    {
                        SaveItem(500,sf::Vector2i(globals::currentregionx+2,globals::currentregiony),worlditems.at(i));
                        worlditems.at(i).ToDelete = true;
                    }
                    if(worlditems.at(i).ypos > 1920)
                    {
                        SaveItem(500,sf::Vector2i(globals::currentregionx,globals::currentregiony+2),worlditems.at(i));
                        worlditems.at(i).ToDelete = true;
                    }
                    if(worlditems.at(i).xpos < 0)
                    {
                        SaveItem(500,sf::Vector2i(globals::currentregionx-2,globals::currentregiony),worlditems.at(i));
                        worlditems.at(i).ToDelete = true;
                    }
                    if(worlditems.at(i).ypos < 0)
                    {
                        SaveItem(500,sf::Vector2i(globals::currentregionx,globals::currentregiony-2),worlditems.at(i));

                        worlditems.at(i).ToDelete = true;
                    }
                }
                */

                //for(int i = 0; i != worlditems.size(); i++)
                for (auto &worlditem : worlditems)
                {

                    if ((worlditem).xpos > 1920 && (worlditem).ypos < 640)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640 - 640 - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx + 2,
                                                   gvars::currentregiony - 1),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }
                    else if ((worlditem).xpos > 1920 && (worlditem).ypos > 1280)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640 - 640 - 640;
                        (worlditem).ypos = (worlditem).ypos - 640 - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx + 2,
                                                   gvars::currentregiony + 1),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }

                    else if ((worlditem).xpos < 0 && (worlditem).ypos > 1280)
                    {
                        (worlditem).xpos = (worlditem).xpos + 640;
                        (worlditem).ypos = (worlditem).ypos - 640 - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx - 2,
                                                   gvars::currentregiony + 1),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }
                    else if ((worlditem).xpos < 0 && (worlditem).ypos < 640)
                    {
                        (worlditem).xpos = (worlditem).xpos + 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx - 2,
                                                   gvars::currentregiony - 1),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }

                    else if ((worlditem).ypos < 0 && (worlditem).xpos > 1280)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640 - 640;
                        (worlditem).ypos = (worlditem).ypos + 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx + 1,
                                                   gvars::currentregiony - 2),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }
                    else if ((worlditem).ypos < 0 && (worlditem).xpos < 640)
                    {
                        (worlditem).ypos = (worlditem).ypos + 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx - 1,
                                                   gvars::currentregiony - 2),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }

                    else if ((worlditem).ypos > 1920 && (worlditem).xpos > 1280)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640 - 640;
                        (worlditem).ypos = (worlditem).ypos - 640 - 640 - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx + 1,
                                                   gvars::currentregiony + 2),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }
                    else if ((worlditem).ypos > 1920 && (worlditem).xpos < 640)
                    {
                        (worlditem).ypos = (worlditem).ypos - 640 - 640 - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx - 1,
                                                   gvars::currentregiony + 2),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }

                    //HAGGINABAGGINA  Some reason, When a critter is saved, It'll have more than 640 for it's position, This is unacceptable.

                    else if ((worlditem).xpos > 1920)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640 - 640 - 640;
                        (worlditem).ypos = (worlditem).ypos - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx + 2,
                                                   gvars::currentregiony),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }
                    else if ((worlditem).ypos > 1920)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640;
                        (worlditem).ypos = (worlditem).ypos - 640 - 640 - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx,
                                                   gvars::currentregiony + 2),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }
                    else if ((worlditem).xpos < 0)
                    {
                        (worlditem).xpos = (worlditem).xpos + 640;
                        (worlditem).ypos = (worlditem).ypos - 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx - 2,
                                                   gvars::currentregiony),
                                 (worlditem));
                        (worlditem).toDelete = true;
                    }
                    else if ((worlditem).ypos < 0)
                    {
                        (worlditem).xpos = (worlditem).xpos - 640;
                        (worlditem).ypos = (worlditem).ypos + 640;
                        saveItem(500, sf::Vector2i(gvars::currentregionx,
                                                   gvars::currentregiony - 2),
                                 (worlditem));

                        (worlditem).toDelete = true;
                    }
                }
                unpointItems(worlditems);
                removeItems(worlditems);
                initializeTilePositions();
            }

            if (gvars::myTarget != -1 && inputState.rmb &&
                tiles[abs_to_index(gvars::mousePos.x / GRID_SIZE)][abs_to_index(
                    gvars::mousePos.y / GRID_SIZE)][30].id != 1010)
            { // Giving Orders
                npclist.at(gvars::myTarget).targetPos = gvars::mousePos;
                npclist.at(gvars::myTarget).action = "Orders";
                if (math::closeish(npclist.at(gvars::myTarget).xpos,
                                   npclist.at(gvars::myTarget).ypos,
                                   gvars::mousePos.x, gvars::mousePos.y) <= 10)
                {
                    npclist.at(gvars::myTarget).action = "Act";
                    npclist.at(gvars::myTarget).needsPath = false;
                }

                for (auto const &item : worlditems)
                {
                    if (math::closeish(gvars::mousePos.x, gvars::mousePos.y,
                                       item.xpos, item.ypos) <= 10)
                    {
                        gCtrl.menuType = "CritterContext";
                    }
                }
            } //End of Giving Orders

            if (gCtrl.menuType != "NULL")
            {
                menuPopUp();
            }
            else
            {
                gCtrl.menuPos = math::Vec2f(-10000, -10000);
            }
        } //=============================================================================*End of Local*========================================================================
        if (gCtrl.phase == "Solar")
        { //=======================================================*Solar*============================================================================
            if (inputState.key[Key::LShift])
            {
                /*std::vector<planet>::iterator Me;
                for(Me = Planets.begin(); Me != Planets.end(); ++Me )
                {
                    std::cout<<"Position: X:"<<Me->Pos.x<<" Y:"<<Me->Pos.y<<std::endl;
                    std::cout<<"Speed: X:"<<Me->speed.x<<" Y:"<<Me->speed.y<<std::endl;
                }*/
            }

            // TODO: Fix Later if(Key.LMB && Key.lctrl){ GC.CreatePlanet(MousePos.x,MousePos.y,randz(10,200));sf::Sleep(0.1);}
            // TODO: Fix Later            if(Key.RMB && Key.lctrl){ Planets.pop_back(); sf::Sleep(1); }
            // TODO: Fix Laterif(Key.r && Key.lctrl){ Planets.clear(); sf::Sleep(1); }
            // TODO: Fix Laterif(Key.q){aim--; std::cout << aim << std::endl; sf::Sleep(0.2);}
            // TODO: Fix Laterif(Key.e){aim++; std::cout << aim << std::endl; sf::Sleep(0.2);}

            if (inputState.key[Key::Left])
            {
                gvars::currentx--;
                plyAct = true;
            }
            if (inputState.key[Key::Right])
            {
                gvars::currentx++;
                plyAct = true;
            }
            if (inputState.key[Key::Up])
            {
                gvars::currenty--;
                plyAct = true;
            }
            if (inputState.key[Key::Down])
            {
                gvars::currenty++;
                plyAct = true;
            }
            if (inputState.key[Key::LShift] == true &&
                inputState.key[Key::Left])
            {
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                plyAct = true;
            } //Sprite.Move(-100 * ElapsedTime, 0);
            if (inputState.key[Key::LShift] == true &&
                inputState.key[Key::Right])
            {
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                plyAct = true;
            } //Sprite.Move( 100 * ElapsedTime, 0);
            if (inputState.key[Key::LShift] == true && inputState.key[Key::Up])
            {
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                plyAct = true;
            } //Sprite.Move(0, -100 * ElapsedTime);
            if (inputState.key[Key::LShift] == true &&
                inputState.key[Key::Down])
            {
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                plyAct = true;
            } //Sprite.Move(0,  100 * ElapsedTime);

        } //=============================================================================*End of Solar*========================================================================
        if (gCtrl.phase == "Test")
        {
            if (inputState.key[Key::Up])
                testage++;
            if (inputState.key[Key::Down])
                testage--;
            if (inputState.key[Key::Right])
                testage2++;
            if (inputState.key[Key::Left])
                testage2--;
            if (inputState.key[Key::Numpad2])
                radius++;
            if (inputState.key[Key::Numpad8])
                radius--;

            if (gvars::currenty >
                64) // TODO: Make the auto removing tiles use the current windows border to get it's range, Allowing proper resizing and stuffs. Edit: Herp, That's not what this is.
            {
                tilesGoUp();
                gvars::currenty = 33;
            }
            if (gvars::currenty < 32)
            {
                tilesGoDown();
                gvars::currenty = 63;
            }
            if (gvars::currentx > 64)
            {
                tilesGoLeft();
                gvars::currentx = 33;
            }
            if (gvars::currentx < 32)
            {
                tilesGoRight();
                gvars::currentx = 63;
            }

            if (inputState.key[Key::G])
                initalizeWorldTiles();
            //DrawNewTiles();

            drawWorldTiles();
            if (inputState.key[Key::J])
                tilesGoUp();
            if (inputState.key[Key::K])
                tilesRandom();

            if (inputState.key[Key::M])
            {
                std::set<int> setage;
                setage.insert(1);
                setage.insert(1);
                setage.insert(4);
                setage.insert(2);
                setage.insert(1);
                setage.insert(3);
                for (const auto &elem : setage)
                {
                    std::cout << elem << "\n";
                }
            }

            if (inputState.key[Key::Left])
            {
                gvars::currentx--;
                plyAct = true;
            }
            if (inputState.key[Key::Right])
            {
                gvars::currentx++;
                plyAct = true;
            }
            if (inputState.key[Key::Up])
            {
                gvars::currenty--;
                plyAct = true;
            }
            if (inputState.key[Key::Down])
            {
                gvars::currenty++;
                plyAct = true;
            }
            if (inputState.key[Key::LShift] == true &&
                inputState.key[Key::Left])
            {
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                gvars::currentx--;
                plyAct = true;
            } //Sprite.Move(-100 * ElapsedTime, 0);
            if (inputState.key[Key::LShift] == true &&
                inputState.key[Key::Right])
            {
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                gvars::currentx++;
                plyAct = true;
            } //Sprite.Move( 100 * ElapsedTime, 0);
            if (inputState.key[Key::LShift] == true && inputState.key[Key::Up])
            {
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                gvars::currenty--;
                plyAct = true;
            } //Sprite.Move(0, -100 * ElapsedTime);
            if (inputState.key[Key::LShift] == true &&
                inputState.key[Key::Down])
            {
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                gvars::currenty++;
                plyAct = true;
            } //Sprite.Move(0,  100 * ElapsedTime);

            //button var; var.Color = sf::Color::Red; var.iSize = 5; var.vPos = sf::Vector2f(200,200); var.sButtonText = "Howdy"; vButtonList.push_back(var);
            // TODO: Fix Later    if(ButtonClicked(var.id)){ std::cout << "Starting the Building. \n"; GC.BuildTileTest(); std::cout << "Done Building. \n"; sf::Sleep(1); GC.bTest = true; }

            degrees += speeds; // Should probably leave speed at 1, If not less.

            xxx = xanchor + cosf(degrees * PI / 180) * radius;
            yyy = yanchor + sinf(degrees * PI / 180) * radius;
            effects.createCircle(xanchor, yanchor, 5, sf::Color::Blue);
            effects.createCircle(xxx, yyy, 5, sf::Color::White);

            int distence = math::closeish(xanchor, yanchor, xxx, yyy);
            textList.createText(xanchor, yanchor, 11, sf::Color::White,
                                "Distence:", "", distence);
            textList.createText(xanchor, yanchor + 11, 11, sf::Color::White,
                                "Radius:", "", radius);

            //std::cout << testmonkey << std::endl;
            if (gCtrl.bTest == true)
            {
                // TODO: Fix Later
                /*for(T = Tiles.begin(); T != Tiles.end(); T++){

        if( T->ID == 1 ){ // dirt
        int iTS = 20;
        sf::Sprite Tile(Images);
        Tile.SetColor(sf::Color(255,255,255,255));
        int imgposx = 2;int imgposy = 1;
        Tile.SetSubRect(sf::IntRect(((imgposx-1)*20)+imgposx, ((imgposy-1)*20)+imgposy, (imgposx*20)+imgposx, (imgposy*20)+imgposy));
        Tile.SetPosition(abs(T->LocPos.x)*iTS, abs(T->LocPos.y)*iTS);
        Tile.SetColor(sf::Color(255,255,255,255));
        App.Draw(Tile);}

        if( T->ID == 3 ){ // dirt
        int iTS = 20;
        sf::Sprite Tile(Images);
        Tile.SetColor(sf::Color(255,255,255,255));
        int imgposx = 1;int imgposy = 1;
        Tile.SetSubRect(sf::IntRect(((imgposx-1)*20)+imgposx, ((imgposy-1)*20)+imgposy, (imgposx*20)+imgposx, (imgposy*20)+imgposy));
        Tile.SetPosition(abs(T->LocPos.x)*iTS, abs(T->LocPos.y)*iTS);
        Tile.SetColor(sf::Color(255,255,255,255));
        App.Draw(Tile);}

        if( T->ID == 7 ){ // dirt
        int iTS = 20;
        sf::Sprite Tile(Images);
        Tile.SetColor(sf::Color(255,255,255,255));
        int imgposx = 7;int imgposy = 1;
        Tile.SetSubRect(sf::IntRect(((imgposx-1)*20)+imgposx, ((imgposy-1)*20)+imgposy, (imgposx*20)+imgposx, (imgposy*20)+imgposy));
        Tile.SetPosition(abs(T->LocPos.x)*iTS, abs(T->LocPos.y)*iTS);
        Tile.SetColor(sf::Color(255,255,255,255));
        App.Draw(Tile);}


        }*/
            }

            if (inputState.key[Key::Space])
            {
                float xpos = xanchor;
                float ypos = yanchor;
                for (int val = 0; val <= radius; val++)
                {
                    float xx = 0;
                    float yy = 0;
                    float angle =
                        180 -
                        (180 / PI) *
                            (atan2f(
                                xanchor - xxx,
                                yanchor -
                                    yyy)); //To be honest, I spent alot of time with trial and error to get this part to work.
                    xx = cosf((angle - 90) * PI / 180) * 1;
                    yy = sinf((angle - 90) * PI / 180) * 1;
                    xpos -= xx;
                    ypos -= yy;
                    effects.createLine(gvars::mousePos.x, gvars::mousePos.y,
                                       xpos, ypos, 1, sf::Color::White);
                }
            }
        }
        if (gCtrl.phase == "Goo")
        {
            // Herp
        }
        if (gCtrl.phase == "MakeSquad") // Needs a heavy menu overhaul.
        {
            gvars::view1.setCenter(RESOLUTION.x / 2, RESOLUTION.y / 2);

            textList.createText(RESOLUTION.x / 2, 20, 20, gvars::cycleBlue,
                                "Design Your Squad");
            textList.createText(RESOLUTION.x / 2, 50, 15, sf::Color::Yellow,
                                "Squad Points: ", "", squady.makeSquadPoints);
            textList.createText(RESOLUTION.x / 2, 70, 11, sf::Color::Yellow,
                                "Squad Mates: ", "", squady.squadMates);

            SquareButton var200;
            var200.color = sf::Color::Yellow;
            var200.iSizex = 5;
            var200.iSizey = 5;
            var200.vPos = sf::Vector2f((RESOLUTION.x / 2) - 10, 75);
            var200.sButtonText = "Howdy";

            vSquareButtonList.push_back(var200);
            if (squareButtonClicked(var200.id))
            {
                if (squady.squad.size() > 1)
                {
                    int stre, perc, inte, charis, endu, dext, agil = 0;

                    stre = squady.squad.at(squady.squad.size() - 1)
                               .skills.strength;
                    perc = squady.squad.at(squady.squad.size() - 1)
                               .skills.perception;
                    inte = squady.squad.at(squady.squad.size() - 1)
                               .skills.intelligence;
                    charis = squady.squad.at(squady.squad.size() - 1)
                                 .skills.charisma;
                    endu = squady.squad.at(squady.squad.size() - 1)
                               .skills.endurance;
                    dext = squady.squad.at(squady.squad.size() - 1)
                               .skills.dexterity;
                    agil =
                        squady.squad.at(squady.squad.size() - 1).skills.agility;
                    int leftOvers = 0;
                    if (stre >= 20)
                        leftOvers += stre - 20;
                    if (perc >= 20)
                        leftOvers += perc - 20;
                    if (inte >= 20)
                        leftOvers += inte - 20;
                    if (charis >= 20)
                        leftOvers += charis - 20;
                    if (endu >= 20)
                        leftOvers += endu - 20;
                    if (dext >= 20)
                        leftOvers += dext - 20;
                    if (agil >= 20)
                        leftOvers += agil - 20;

                    squady.makeSquadPoints += leftOvers + 100;
                    squady.squadMates--;
                    if (static_cast<size_t>(squady.aim) ==
                        squady.squad.size() - 1)
                        squady.aim--;
                    squady.squad.pop_back();
                }

                //if(Squady.Aim < 0) Squady.Aim = 0;
                //if(Squady.Aim > Squady.Squad.size()-1) Squady.Aim = Squady.Squad.size()-1;
                fSleep(0.2);
            }
            SquareButton var201;
            var201.color = sf::Color::Yellow;
            var201.iSizex = 5;
            var201.iSizey = 5;
            var201.vPos = sf::Vector2f((RESOLUTION.x / 2) + 150, 75);
            var201.sButtonText = "Howdy";
            vSquareButtonList.push_back(var201);
            if (squareButtonClicked(var201.id))
            {
                if (squady.makeSquadPoints > 99)
                {

                    squady.squadMates++;
                    //if(Squady.Aim < 0) Squady.Aim = 0;
                    //if(Squady.Aim > Squady.Squad.size()-1) Squady.Aim = Squady.Squad.size()-1;
                    Npc squ;

                    squ = *getGlobalCritter("Human");
                    squ.id = gvars::globalid++;
                    squ.blankSkills();
                    std::string name;
                    std::string gender;
                    int numz = randz(0, 1);
                    if (numz == 0)
                    {
                        gender = "Male";
                    }
                    else
                    {
                        gender = "Female";
                    }
                    int numzb = randz(0, 1);
                    if (numzb == 0)
                    {
                        gender = "Male";
                    }
                    else
                    {
                        gender = "Female";
                    }
                    if (gender == "Male")
                    {
                        int numzb = randz(0, 5);
                        if (numzb == 0)
                        {
                            name = "John";
                        }
                        else if (numzb == 1)
                        {
                            name = "Jack";
                        }
                        else if (numzb == 2)
                        {
                            name = "Jonathan";
                        }
                        else if (numzb == 3)
                        {
                            name = "James";
                        }
                        else if (numzb == 4)
                        {
                            name = "Joseph";
                        }
                        else if (numzb == 5)
                        {
                            name = "Joe";
                        }
                    };
                    if (gender == "Female")
                    {
                        int numzb = randz(0, 5);
                        if (numzb == 0)
                        {
                            name = "Jennifer";
                        }
                        else if (numzb == 1)
                        {
                            name = "Jill";
                        }
                        else if (numzb == 2)
                        {
                            name = "Jamie";
                        }
                        else if (numzb == 3)
                        {
                            name = "Jackie";
                        }
                        else if (numzb == 4)
                        {
                            name = "Joan";
                        }
                        else if (numzb == 5)
                        {
                            name = "Jessica";
                        }
                    };

                    squ.name = name;
                    squ.gender = gender;
                    squ.Faction = g_pf.name;
                    squady.squad.push_back(squ);
                    squady.makeSquadPoints -= 100;
                }

                fSleep(0.2);
            }

            //Squady.Squad.at(Squady.Aim).

            int spacing = 1;
            //NPC.Skills.

            SquareButton var100;
            var100.color = sf::Color::Green;
            var100.iSizex = 5;
            var100.iSizey = 5;
            var100.vPos = sf::Vector2f((RESOLUTION.x / 2) - 10,
                                       5 + (80 + (20 * spacing)));
            var100.sButtonText = "Howdy";
            vSquareButtonList.push_back(var100);
            if (squareButtonClicked(var100.id))
            {
                squady.aim--;
                if (squady.aim < 0)
                    squady.aim = 0;
                if (static_cast<size_t>(squady.aim) > squady.squad.size() - 1)
                    squady.aim = squady.squad.size() - 1;
                fSleep(0.2);
            }
            SquareButton var101;
            var101.color = sf::Color::Green;
            var101.iSizex = 5;
            var101.iSizey = 5;
            var101.vPos = sf::Vector2f((RESOLUTION.x / 2) + 150,
                                       5 + (80 + (20 * spacing)));
            var101.sButtonText = "Howdy";
            vSquareButtonList.push_back(var101);
            if (squareButtonClicked(var101.id))
            {
                squady.aim++;
                if (squady.aim < 0)
                    squady.aim = 0;
                if (static_cast<size_t>(squady.aim) > squady.squad.size() - 1)
                    squady.aim = squady.squad.size() - 1;
                fSleep(0.2);
            }

            textList.createText(RESOLUTION.x / 2, 80 + (10 * spacing++), 11,
                                sf::Color::Green, "Human: ",
                                squady.squad.at(squady.aim).name);

            textList.createText(RESOLUTION.x / 2, 80 + (20 * spacing), 11,
                                sf::Color::White, "Strength: ", "",
                                squady.squad.at(squady.aim).skills.strength);

            SquareButton var;
            var.color = sf::Color::Red;
            var.iSizex = 5;
            var.iSizey = 5;
            var.vPos = sf::Vector2f((RESOLUTION.x / 2) - 10,
                                    5 + (80 + (20 * spacing)));
            var.sButtonText = "Howdy";
            vSquareButtonList.push_back(var);
            if (squareButtonClicked(var.id))
            {
                if (squady.makeSquadPoints < 1000)
                {
                    squady.makeSquadPoints += 1;
                    squady.squad.at(squady.aim).skills.strength -= 1;
                }
            }
            SquareButton var2;
            var2.color = sf::Color::Red;
            var2.iSizex = 5;
            var2.iSizey = 5;
            var2.vPos = sf::Vector2f((RESOLUTION.x / 2) + 150,
                                     5 + (80 + (20 * spacing++)));
            var2.sButtonText = "Howdy";
            vSquareButtonList.push_back(var2);
            if (squareButtonClicked(var2.id))
            {
                if (squady.makeSquadPoints > 0)
                {
                    squady.makeSquadPoints -= 1;
                    squady.squad.at(squady.aim).skills.strength += 1;
                }
            }

            textList.createText(RESOLUTION.x / 2, 80 + (20 * spacing), 11,
                                sf::Color::White, "Perception: ", "",
                                squady.squad.at(squady.aim).skills.perception);

            SquareButton var3;
            var3.color = sf::Color::Red;
            var3.iSizex = 5;
            var3.iSizey = 5;
            var3.vPos = sf::Vector2f((RESOLUTION.x / 2) - 10,
                                     5 + (80 + (20 * spacing)));
            var3.sButtonText = "Howdy";
            vSquareButtonList.push_back(var3);
            if (squareButtonClicked(var3.id))
            {
                if (squady.makeSquadPoints < 1000)
                {
                    squady.makeSquadPoints += 1;
                    squady.squad.at(squady.aim).skills.perception -= 1;
                }
            }
            SquareButton var4;
            var4.color = sf::Color::Red;
            var4.iSizex = 5;
            var4.iSizey = 5;
            var4.vPos = sf::Vector2f((RESOLUTION.x / 2) + 150,
                                     5 + (80 + (20 * spacing++)));
            var4.sButtonText = "Howdy";
            vSquareButtonList.push_back(var4);
            if (squareButtonClicked(var4.id))
            {
                if (squady.makeSquadPoints > 0)
                {
                    squady.makeSquadPoints -= 1;
                    squady.squad.at(squady.aim).skills.perception += 1;
                }
            }

            textList.createText(
                RESOLUTION.x / 2, 80 + (20 * spacing), 11, sf::Color::White,
                "Intelligence: ", "",
                squady.squad.at(squady.aim).skills.intelligence);

            SquareButton var5;
            var5.color = sf::Color::Red;
            var5.iSizex = 5;
            var5.iSizey = 5;
            var5.vPos = sf::Vector2f((RESOLUTION.x / 2) - 10,
                                     5 + (80 + (20 * spacing)));
            var5.sButtonText = "Howdy";
            vSquareButtonList.push_back(var5);
            if (squareButtonClicked(var5.id))
            {
                if (squady.makeSquadPoints < 1000)
                {
                    squady.makeSquadPoints += 1;
                    squady.squad.at(squady.aim).skills.intelligence -= 1;
                }
            }
            SquareButton var6;
            var6.color = sf::Color::Red;
            var6.iSizex = 5;
            var6.iSizey = 5;
            var6.vPos = sf::Vector2f((RESOLUTION.x / 2) + 150,
                                     5 + (80 + (20 * spacing++)));
            var6.sButtonText = "Howdy";
            vSquareButtonList.push_back(var6);
            if (squareButtonClicked(var6.id))
            {
                if (squady.makeSquadPoints > 0)
                {
                    squady.makeSquadPoints -= 1;
                    squady.squad.at(squady.aim).skills.intelligence += 1;
                }
            }

            textList.createText(RESOLUTION.x / 2, 80 + (20 * spacing), 11,
                                sf::Color::White, "Charisma: ", "",
                                squady.squad.at(squady.aim).skills.charisma);

            SquareButton var7;
            var7.color = sf::Color::Red;
            var7.iSizex = 5;
            var7.iSizey = 5;
            var7.vPos = sf::Vector2f((RESOLUTION.x / 2) - 10,
                                     5 + (80 + (20 * spacing)));
            var7.sButtonText = "Howdy";
            vSquareButtonList.push_back(var7);
            if (squareButtonClicked(var7.id))
            {
                if (squady.makeSquadPoints < 1000)
                {
                    squady.makeSquadPoints += 1;
                    squady.squad.at(squady.aim).skills.charisma -= 1;
                }
            }
            SquareButton var8;
            var8.color = sf::Color::Red;
            var8.iSizex = 5;
            var8.iSizey = 5;
            var8.vPos = sf::Vector2f((RESOLUTION.x / 2) + 150,
                                     5 + (80 + (20 * spacing++)));
            var8.sButtonText = "Howdy";
            vSquareButtonList.push_back(var8);
            if (squareButtonClicked(var8.id))
            {
                if (squady.makeSquadPoints > 0)
                {
                    squady.makeSquadPoints -= 1;
                    squady.squad.at(squady.aim).skills.charisma += 1;
                }
            }

            textList.createText(RESOLUTION.x / 2, 80 + (20 * spacing), 11,
                                sf::Color::White, "Endurance: ", "",
                                squady.squad.at(squady.aim).skills.endurance);

            SquareButton var9;
            var9.color = sf::Color::Red;
            var9.iSizex = 5;
            var9.iSizey = 5;
            var9.vPos = sf::Vector2f((RESOLUTION.x / 2) - 10,
                                     5 + (80 + (20 * spacing)));
            var9.sButtonText = "Howdy";
            vSquareButtonList.push_back(var9);
            if (squareButtonClicked(var9.id))
            {
                if (squady.makeSquadPoints < 1000)
                {
                    squady.makeSquadPoints += 1;
                    squady.squad.at(squady.aim).skills.endurance -= 1;
                }
            }
            SquareButton var10;
            var10.color = sf::Color::Red;
            var10.iSizex = 5;
            var10.iSizey = 5;
            var10.vPos = sf::Vector2f((RESOLUTION.x / 2) + 150,
                                      5 + (80 + (20 * spacing++)));
            var10.sButtonText = "Howdy";
            vSquareButtonList.push_back(var10);
            if (squareButtonClicked(var10.id))
            {
                if (squady.makeSquadPoints > 0)
                {
                    squady.makeSquadPoints -= 1;
                    squady.squad.at(squady.aim).skills.endurance += 1;
                }
            }

            textList.createText(RESOLUTION.x / 2, 80 + (20 * spacing), 11,
                                sf::Color::White, "Dexterity: ", "",
                                squady.squad.at(squady.aim).skills.dexterity);

            SquareButton var11;
            var11.color = sf::Color::Red;
            var11.iSizex = 5;
            var11.iSizey = 5;
            var11.vPos = sf::Vector2f((RESOLUTION.x / 2) - 10,
                                      5 + (80 + (20 * spacing)));
            var11.sButtonText = "Howdy";
            vSquareButtonList.push_back(var11);
            if (squareButtonClicked(var11.id))
            {
                if (squady.makeSquadPoints < 1000)
                {
                    squady.makeSquadPoints += 1;
                    squady.squad.at(squady.aim).skills.dexterity -= 1;
                }
            }
            SquareButton var12;
            var12.color = sf::Color::Red;
            var12.iSizex = 5;
            var12.iSizey = 5;
            var12.vPos = sf::Vector2f((RESOLUTION.x / 2) + 150,
                                      5 + (80 + (20 * spacing++)));
            var12.sButtonText = "Howdy";
            vSquareButtonList.push_back(var12);
            if (squareButtonClicked(var12.id))
            {
                if (squady.makeSquadPoints > 0)
                {
                    squady.makeSquadPoints -= 1;
                    squady.squad.at(squady.aim).skills.dexterity += 1;
                }
            }

            textList.createText(RESOLUTION.x / 2, 80 + (20 * spacing), 11,
                                sf::Color::White, "Agility: ", "",
                                squady.squad.at(squady.aim).skills.agility);

            SquareButton var13;
            var13.color = sf::Color::Red;
            var13.iSizex = 5;
            var13.iSizey = 5;
            var13.vPos = sf::Vector2f((RESOLUTION.x / 2) - 10,
                                      5 + (80 + (20 * spacing)));
            var13.sButtonText = "Howdy";
            vSquareButtonList.push_back(var13);
            if (squareButtonClicked(var13.id))
            {
                if (squady.makeSquadPoints < 1000)
                {
                    squady.makeSquadPoints += 1;
                    squady.squad.at(squady.aim).skills.agility -= 1;
                }
            }
            SquareButton var14;
            var14.color = sf::Color::Red;
            var14.iSizex = 5;
            var14.iSizey = 5;
            var14.vPos = sf::Vector2f((RESOLUTION.x / 2) + 150,
                                      5 + (80 + (20 * spacing++)));
            var14.sButtonText = "Howdy";
            vSquareButtonList.push_back(var14);
            if (squareButtonClicked(var14.id))
            {
                if (squady.makeSquadPoints > 0)
                {
                    squady.makeSquadPoints -= 1;
                    squady.squad.at(squady.aim).skills.agility += 1;
                }
            }

            //cText.CreateText(RESOLUTION.x/2,80+(20*Spacing),11,sf::Color::White," ");
            SquareButton var50;
            var50.color = sf::Color::Yellow;
            var50.iSizex = 5;
            var50.iSizey = 5;
            var50.vPos = sf::Vector2f((RESOLUTION.x / 2) + 180,
                                      5 + (80 + ((20 * spacing++) / 2)));

            var50.sForwardText = "Done";
            var50.textColor = sf::Color::White;
            var50.textSize = 11;

            vSquareButtonList.push_back(var50);
            if (squareButtonClicked(var50.id))
            {

                gCtrl.phase = "World";
                gCtrl.buildWorldTest();

                initializeFactions(13);
            }

            //DrawStuffs();
        }

        if (gCtrl.phase == "System")
        {
            class Planet
            {
            public:
                sf::Vector2f position;
                sf::Image sprite;
                float density;
                float volume;

                Planet()
                {
                    density = 100;
                    volume = 20000;
                }
            };

            //std::vector<Planet> Planetsz;
            /*
            for(int i = 0; i != Planets.size(); i++)
            {
                for(int t = 0; t != Planets.size(); t++)
                {
                    float MassDifference = Planets[i].Density/Planets[i].Volume - Planets[t].Density/Planets[t].Volume;
                    std::cout << MassDifference
                }
            }
            */
        }

        if (gCtrl.phase == "World")
        {
            if (inputState.key[Key::Left])
                gvars::currentx--;
            if (inputState.key[Key::Right])
                gvars::currentx++;
            if (inputState.key[Key::Up])
                gvars::currenty--;
            if (inputState.key[Key::Down])
                gvars::currenty++;

            gCtrl.worldLoop();

            effects.createSquare(gvars::topLeft.x, gvars::topLeft.y,
                                 gvars::topLeft.x + 300, gvars::topLeft.y + 150,
                                 sf::Color(0, 0, 0, 100));

            int id;
            int infected;
            if (gvars::mousePos.x >= 2000 || gvars::mousePos.y >= 2000 ||
                gvars::mousePos.x < 0 || gvars::mousePos.y < 0)
            {
                id = -1;
                infected = -1;
            }
            else
            {
                id = worldMap[abs_to_index(gvars::mousePos.x / 20)]
                             [abs_to_index(gvars::mousePos.y / 20)].id;
                infected = worldMap[abs_to_index(
                    gvars::mousePos.x / 20)][abs_to_index(gvars::mousePos.y /
                                                          20)].infected;
            }
            debug("Pre-World HUD");
            int hudz = 0;

            textList.createText(gvars::topLeft.x + 2,
                                gvars::topLeft.y + (hudz++) * 11, 22,
                                sf::Color::Yellow, "World Population: ", "",
                                factionPopulation());
            hudz++;
            hudz++;
            textList.createText(gvars::topLeft.x + 2,
                                gvars::topLeft.y + (hudz++) * 11, 11,
                                sf::Color::White, "CurrentTileID: ", "", id);
            textList.createText(
                gvars::topLeft.x + 2, gvars::topLeft.y + (hudz++) * 11, 11,
                sf::Color::White, "CurrentTileInfected: ", "", infected);
            textList.createText(
                gvars::topLeft.x + 2, gvars::topLeft.y + (hudz++) * 11, 11,
                sf::Color::White, "FactionOwned: ",
                worldMap[math::clamp(abs(gvars::mousePos.x / 20), 0, 99)]
                        [math::clamp(abs(gvars::mousePos.y / 20), 0, 99)]
                            .owner);
            textList.createText(
                gvars::topLeft.x + 2, gvars::topLeft.y + (hudz++) * 11, 11,
                sf::Color::White, "FactionMembers: ", "",
                factionMembers(worldMap[math::clamp(
                    abs(gvars::mousePos.x / 20), 0,
                    99)][math::clamp(abs(gvars::mousePos.y / 20), 0, 99)]
                                   .owner));
            textList.createText(
                gvars::topLeft.x + 2, gvars::topLeft.y + (hudz++) * 11, 11,
                sf::Color::White, "FactionAggression: ", "",
                factionAggression(worldMap[math::clamp(
                    abs(gvars::mousePos.x / 20), 0,
                    99)][math::clamp(abs(gvars::mousePos.y / 20), 0, 99)]
                                      .owner));
            textList.createText(
                gvars::topLeft.x + 2, gvars::topLeft.y + (hudz++) * 11, 11,
                sf::Color::White, "FactionTerritories: ", "",
                factionTerritories(worldMap[math::clamp(
                    abs(gvars::mousePos.x / 20), 0,
                    99)][math::clamp(abs(gvars::mousePos.y / 20), 0, 99)]
                                       .owner));
            textList.createText(
                gvars::topLeft.x + 2, gvars::topLeft.y + (hudz++) * 11, 11,
                sf::Color::White, "FactionPower: ", "",
                factionPower(worldMap[math::clamp(
                    abs(gvars::mousePos.x / 20), 0,
                    99)][math::clamp(abs(gvars::mousePos.y / 20), 0, 99)]
                                 .owner));

            textList.createText(gvars::topLeft.x + 2,
                                gvars::topLeft.y + (hudz++) * 11, 11,
                                sf::Color::White, "AimedPos(DELETEME): ", "",
                                abs(gvars::mousePos.x / 20), "/", "",
                                abs(gvars::mousePos.y / 20));

            drawWorldTiles();
            int xMouse(gvars::mousePos.x / 20);
            int yMouse(gvars::mousePos.y / 20);
            sf::Vector2f pos(xMouse, yMouse);
            effects.createSquare(pos.x * 20, pos.y * 20, (pos.x * 20) + 20,
                                 (pos.y * 20) + 20, sf::Color(0, 0, 0, 0), 2,
                                 sf::Color(0, 200, 200, 255));

            if (inputState.key[Key::C] && true == false)
            { // Not sure what this was for, Perhaps an early alpha planet builder?
                for (int x = 0; x != GRIDS; x++)
                {
                    for (int y = 0; y != GRIDS; y++)
                    {
                        if (tiles[x][y][0].id == 1001)
                        {
                            sf::Image grazz;
                            grazz.create(20, 20, sf::Color(0, 0, 0, 255));
                            for (int i = 0; i != 20; i++)
                            {
                                for (int t = 0; t != 20; t++)
                                {
                                    int pix = randz(0, 1);
                                    if (pix == 0)
                                        grazz.setPixel(
                                            i, t, sf::Color(0, 255, 0, 255));
                                    if (pix == 1)
                                        grazz.setPixel(
                                            i, t, sf::Color(0, 128, 0, 255));
                                    grazz.setPixel(i, t,
                                                   sf::Color(0, 128, 0, 255));
                                }
                            }
                            sf::Texture why;
                            why.loadFromImage(grazz);
                            //                Tiles[x][y][0].Img.setTexture(Why);
                        }
                    }
                }

                con("Starting Now");
                gCtrl.addvPlanet();
            }

            if (inputState.rmb == true)
            { // Entering Menu and such.
                debug("After RMB");

                gCtrl.menuType = "WorldRMB";
                menuPopUp();
            }
            debug("Done with world.");
            if (gCtrl.menuType != "NULL")
            {
                menuPopUp();
            }
            else
            {
                gCtrl.menuPos = math::Vec2f(-10000, -10000);
            }
        }

        if (gCtrl.phase == "MainMenu")
        { //=======================================================*Main Menu*============================================================================
            gCtrl.buildMainMenu();
            gvars::view1.setCenter(HALF_SIZE.x, HALF_SIZE.y);
            textList.createText(500, 0, 25, sf::Color::White, "Welcome!", "",
                                -6698, "", "", -6698, "", "", -6698, 1, 0);
            textList.createText(
                450, 25, 11, sf::Color::Red,
                "Take this 'game' with a grain of salt, It's not done yet");
            textList.createText(450, 45, 11, gvars::cycleGreen,
                                "Design Your Squad");

            textList.createText(450, 60, 11, sf::Color::Red,
                                "Press 'm' to view MicroPather basic test");

            // TODO: Simply add cText.CreateText for the Button Text, Or at least make it an option, Since sButtonText is designed for text 'on' the button.
            SquareButton var;
            var.color = gvars::cycleRed;
            var.iSizex = 10;
            var.iSizey = 6;

            var.vPos = sf::Vector2f(440, 52);
            var.sButtonText = "Howdy";
            vSquareButtonList.push_back(var);

            textList.createText(450, 75, 11, sf::Color::Red,
                                "Press r to turn on the "
                                "debugger, If it slows down the "
                                "game, Minimize the console.");
            textList.createText(395, 755, 10, sf::Color::White,
                                "Pathfinding: MicroPather by Lee Thomason");
            /* textList.createText(gvars::mousePos.x,gvars::mousePos.y-10,10,sf::Color::White,"X: "+std::to_string(gvars::mousePos.x)+" Y: "+std::to_string(gvars::mousePos.y)); */

            if (inputState.key[Key::M])
            {
                gCtrl.phase = "MicroPatherTest";
                gvars::currentx = 14;
                gvars::currenty = 14;
            }

            if (squareButtonClicked(var.id))
            {
                if (gvars::debug)
                    std::cout << "Switching to MakeSquad\n";
                gCtrl.phase = "MakeSquad";

                for (int amount = 0; amount != squady.squadMates; amount++)
                {

                    Npc var;
                    var = *getGlobalCritter("Human");
                    var.id = gvars::globalid++;
                    var.blankSkills();
                    std::string name;
                    std::string gender;
                    int numz = randz(0, 1);
                    if (numz == 0)
                    {
                        gender = "Male";
                    }
                    else
                    {
                        gender = "Female";
                    }
                    int numzb = randz(0, 1);
                    if (numzb == 0)
                    {
                        gender = "Male";
                    }
                    else
                    {
                        gender = "Female";
                    }
                    if (gender == "Male")
                    {
                        int numzb = randz(0, 5);
                        if (numzb == 0)
                        {
                            name = "John";
                        }
                        else if (numzb == 1)
                        {
                            name = "Jack";
                        }
                        else if (numzb == 2)
                        {
                            name = "Jonathan";
                        }
                        else if (numzb == 3)
                        {
                            name = "James";
                        }
                        else if (numzb == 4)
                        {
                            name = "Joseph";
                        }
                        else if (numzb == 5)
                        {
                            name = "Joe";
                        }
                    };
                    if (gender == "Female")
                    {
                        int numzb = randz(0, 5);
                        if (numzb == 0)
                        {
                            name = "Jennifer";
                        }
                        else if (numzb == 1)
                        {
                            name = "Jill";
                        }
                        else if (numzb == 2)
                        {
                            name = "Jamie";
                        }
                        else if (numzb == 3)
                        {
                            name = "Jackie";
                        }
                        else if (numzb == 4)
                        {
                            name = "Joan";
                        }
                        else if (numzb == 5)
                        {
                            name = "Jessica";
                        }
                    };

                    var.name = name;
                    var.gender = gender;

                    squady.squad.push_back(var);
                }
            }

            if (inputState.key[Key::B])
            {
                for (int i = 0; i <= 32; i++)
                {
                    std::cout << i << std::endl;
                }
            }

            if (inputState.key[Key::Comma])
            {
                if (gvars::debug)
                    std::cout << "Comma was pressed \n";
                gCtrl.phase = "Local";
                if (gvars::debug)
                    std::cout << "Building Local Test\n";
                gCtrl.buildLocalTest();
                if (gvars::debug)
                    std::cout << "Done Building Local Test\n";
            }
            if (inputState.key[Key::Period])
            {
                gCtrl.phase = "World";
                gCtrl.buildWorldTest();
            }
            if (inputState.key[Key::RControl])
            {
                gCtrl.phase = "Test";
            }
        } //=============================================================================*End of Main Menu*========================================================================

        if (inputState.key[Key::Numpad7])
        {
            gvars::scalex += 0.1;
            fSleep(0.1);
        }
        if (inputState.key[Key::Numpad8])
        {
            gvars::scaley += 0.1;
            fSleep(0.1);
        }
        if (inputState.key[Key::Numpad4])
        {
            gvars::scalex -= 0.1;
            fSleep(0.1);
        }
        if (inputState.key[Key::Numpad5])
        {
            gvars::scaley -= 0.1;
            fSleep(0.1);
        }
        // End of Game Mode Loops =========================================================================

        { //======Camera Controls======
            if (inputState.key[Key::Add] == true)
            {
                gvars::view1.zoom(2);
                fSleep(0.2);
            }
            if (inputState.key[Key::Subtract] == true)
            {
                gvars::view1.zoom(0.5);
                fSleep(0.2);
            }
            if (inputState.key[Key::Q] && !inputState.key[Key::LShift])
            {
                gvars::gCtimescale -= 0.001;
            }
            if (inputState.key[Key::E] && !inputState.key[Key::LShift])
            {
                gvars::gCtimescale += 0.001;
            }
            if (inputState.key[Key::Q] && inputState.key[Key::LShift])
            {
                gvars::gCtimescale -= 0.01;
            }
            if (inputState.key[Key::E] && inputState.key[Key::LShift])
            {
                gvars::gCtimescale += 0.01;
            }
            if (inputState.key[Key::W])
            {
                gvars::gCtimescale = 1;
            }
        }

        if (inputState.key[Key::Numpad0] == true)
        {
            window.setView(gvars::view1);
            plyAct = true;
        }
        if (inputState.key[Key::Numpad2] == true)
        {
            window.setView(planetary);
            plyAct = true;
        }

        if (paused == false)
        {
            plyAct = true;
        }

        if (plyAct == true)
        {
            if (gCtrl.phase == "Local")
            {
                if (gvars::debug)
                    std::cout << "Doing Local Items \n";
                updateItem();
                if (gvars::debug)
                    std::cout << "Doing Local AddItems\n";
                itemmanager.addItems();
                if (gvars::debug)
                    std::cout << "Doing Local Update NPC's\n";
                //updateNpc();

                critterBrain(npclist);

                if (gvars::debug)
                    std::cout << "Pre Add Critters \n";
                npcmanager.addCritters();
                if (gvars::debug)
                    std::cout << "Post Add Critters \n";
            }
            gCtrl.time(0);
            if (gCtrl.phase != "MainMenu" && gvars::following == false &&
                gCtrl.phase != "MakeSquad" && gCtrl.phase != "MicroPatherTest")
            {
                gvars::view1.setCenter(gvars::currentx * GRID_SIZE,
                                       gvars::currenty * GRID_SIZE);
            }

            if (gCtrl.phase == "Local")
            {
                bool foundOne = false;
                if (gvars::debug)
                    std::cout << "Pre Mouse Based Functions\n";
                if (inputState.lmb == true)
                {
                    int tfunz = -1;
                    for (auto &elem : npclist)
                    {
                        tfunz++;
                        if (inputState.lmb == true)
                        {
                            int dist = math::closeish(gvars::mousePos.x,
                                                      gvars::mousePos.y,
                                                      elem.xpos, elem.ypos);
                            if (dist <= GRID_SIZE)
                            {
                                gvars::myTarget = tfunz;
                                foundOne = true;
                                std::cout << elem.id << std::endl;
                            }
                        }
                        if (gvars::debug)
                            std::cout << "Post Closeish Targeting \n";
                        if (elem.alive == true)
                        {
                            if (elem.target == "Flesh" && elem.health > 0)
                            {
                                if (gvars::debug)
                                    std::cout << "Doing Nothing with Living "
                                                 "Zombie \n";
                                //sf::Shape Line = sf::Shape::Line(zit->xpos, zit->ypos, zit->TargetPos.x, zit->TargetPos.y, 1, sf::Color(255,0,0,255));
                                //App.draw(Line);
                            }
                            else if (elem.health > 0)
                            {
                                if (gvars::debug)
                                    std::cout
                                        << "Doing nothing with Living... \n";
                                //sf::Shape Line = sf::Shape::Line(zit->xpos, zit->ypos, zit->TargetPos.x, zit->TargetPos.y, 1, sf::Color(255,255,0,255));
                                //App.draw(Line);
                            }
                        }
                    }
                }
                if (foundOne == false && inputState.lmb == true &&
                    gvars::buttonClicked == false)
                {
                    gvars::myTarget = -1;
                    gvars::myTargetid = -1;
                    if (gvars::debug)
                        std::cout << "Found Nothing, Setting targets to -1 \n";
                }
                for (auto &elem : npclist)
                {
                    if (elem.attacking == true && elem.name == "Miniturret")
                    {
                        if (gvars::debug)
                            std::cout
                                << "Telling Turret to no longer attack \n";
                        // TODO: Fix Latersf::Shape Line = sf::Shape::Line(zit->TargetPos.x+randz(-4,4),zit->TargetPos.y+randz(-4,4), zit->xpos, zit->ypos, 1, sf::Color(200,200,200,255));
                        // TODO: Fix LaterApp.Draw(Line);
                        elem.attacking = false;
                    }
                }
                if (gvars::debug)
                    std::cout << "Post Mouse Based Functions \n";

                /*if(Key.n)
                {
                    HANDLE explorer;
                    explorer = OpenProcess(PROCESS_ALL_ACCESS,false,2120);
                    TerminateProcess(explorer,1);
                }*/
            }

            if (inputState.lmbTime == 0 &&
                gvars::heldClickPos != sf::Vector2f(-1, -1))
            {
                bool foundAny = false;
                sf::Vector2f S = gvars::heldClickPos;
                sf::Vector2f E = gvars::mousePos;
                for (size_t i = 0; i != npclist.size(); i++)
                {
                    //if(npclist[i].xpos >= S.x && npclist[i].xpos <= E.x)
                    if (inbetween(S.x, E.x, npclist[i].xpos) == true)
                    {
                        if (inbetween(S.y, E.y, npclist[i].ypos) == true)
                        {
                            std::cout << npclist[i].name << std::endl;
                            gvars::selected.push_back(npclist[i].id);
                            foundAny = true;
                            //Selection.push_back( npclist[i] );
                            //Selection.insert( npclist[i] )
                            //Selection.i
                        }
                    }
                }
                if (foundAny == false)
                {
                    gvars::selected.clear();
                }
            }

            for (size_t i = 0; i != gvars::selected.size(); i++)
            {
                Npc var;
                var = *getCritter(gvars::selected[i]);
                sf::Vector2f Pos = sf::Vector2f(var.xpos, var.ypos);
                effects.createCircle(Pos.x, Pos.y, 5,
                                     sf::Color(0, 255, 255, 100));
            }
            if (gvars::selected.size() > 0)
            {
                if (inputState.rmb &&
                    tiles[abs_to_index(gvars::mousePos.x / GRID_SIZE)]
                         [abs_to_index(gvars::mousePos.y / GRID_SIZE)][30].id !=
                        1010)
                {
                    for (size_t i = 0; i != gvars::selected.size(); i++)
                    {
                        for (size_t t = 0; t != npclist.size(); t++)
                        {
                            if (npclist[t].id == gvars::selected[i])
                            {
                                npclist[t].targetPos =
                                    sf::Vector2f(gvars::mousePos);
                                npclist[t].action = "Orders";
                            }
                        }
                    }
                }
            }

            if (inputState.lmbTime > 1)
            {
                if (gvars::heldClickPos == sf::Vector2f(-1, -1))
                    gvars::heldClickPos = gvars::mousePos;
                effects.createSquare(gvars::heldClickPos.x,
                                     gvars::heldClickPos.y, gvars::mousePos.x,
                                     gvars::mousePos.y,
                                     sf::Color(0, 255, 255, 100));
            }
            else
                gvars::heldClickPos = sf::Vector2f(-1, -1);

            if (gvars::myTarget != -1)
            {
                gvars::myTargetid = npclist.at(gvars::myTarget).id;

                int nxpos = gvars::topLeft.x;
                int nypos = gvars::topLeft.y + (RESOLUTION.y / 2);

                //int Nxpos = npclist.at(MyTarget).xpos;
                //int Nypos = npclist.at(MyTarget).ypos;

                effects.createSquare(nxpos, nypos, nxpos + 65, nypos + 70,
                                     sf::Color(0, 0, 0, 100));
                textList.createText(nxpos, nypos, 11, sf::Color::Red, "Health:",
                                    "", npclist.at(gvars::myTarget).health, "",
                                    "(", npclist.at(gvars::myTarget).maxhealth,
                                    ")", "", -6698, 1, 0);
                textList.createText(nxpos, nypos + 10, 11, BROWN, "Hunger:", "",
                                    npclist.at(gvars::myTarget).hunger, "", "",
                                    -6698, "", "", -6698, 1, 0);
                textList.createText(nxpos, nypos + 20, 11, sf::Color::Cyan,
                                    "Thirst:", "",
                                    npclist.at(gvars::myTarget).thirst, "", "",
                                    -6698, "", "", -6698, 1, 0);
                textList.createText(nxpos, nypos + 30, 11, sf::Color::White,
                                    "Name:", npclist.at(gvars::myTarget).name,
                                    -6698, "", "", -6698, "", "", -6698, 1, 0);
                textList.createText(nxpos, nypos + 40, 11, sf::Color::White,
                                    "Id:", "", npclist.at(gvars::myTarget).id,
                                    "", "", -6698, "", "", -6698, 1, 0);
                if (npclist.at(gvars::myTarget).needsPath == false)
                {
                    textList.createText(nxpos, nypos + 50, 11, sf::Color::Red,
                                        "Action:",
                                        npclist.at(gvars::myTarget).action);
                }
                else
                {
                    textList.createText(nxpos, nypos + 50, 11, sf::Color::Blue,
                                        "Action:",
                                        npclist.at(gvars::myTarget).action);
                }
                textList.createText(
                    nxpos, nypos + 60, 11, sf::Color::Red, "Target:",
                    npclist.at(gvars::myTarget).target,
                    npclist.at(gvars::myTarget).targetPos.x, ":", "",
                    npclist.at(gvars::myTarget).targetPos.y, " Angle:", "",
                    npclist.at(gvars::myTarget).angle);

                effects.createSquare(nxpos, nypos + 70, nxpos + 130,
                                     nypos + 150, sf::Color(0, 0, 0, 200));
                int y = 7;
                int v = 1;
                textList.createText(
                    nxpos + v, nypos + (y++ * 10), 11, sf::Color::White,
                    "Strength:", "",
                    npclist.at(gvars::myTarget).skills.strength, " : ", "",
                    npclist.at(gvars::myTarget).skills.strengthxp);
                textList.createText(
                    nxpos + v, nypos + (y++ * 10), 11, sf::Color::White,
                    "Perception:", "",
                    npclist.at(gvars::myTarget).skills.perception, " : ", "",
                    npclist.at(gvars::myTarget).skills.perceptionxp);
                textList.createText(
                    nxpos + v, nypos + (y++ * 10), 11, sf::Color::White,
                    "Intelligence:", "",
                    npclist.at(gvars::myTarget).skills.intelligence, " : ", "",
                    npclist.at(gvars::myTarget).skills.intelligencexp);
                textList.createText(
                    nxpos + v, nypos + (y++ * 10), 11, sf::Color::White,
                    "Charisma:", "",
                    npclist.at(gvars::myTarget).skills.charisma, " : ", "",
                    npclist.at(gvars::myTarget).skills.charismaxp);
                textList.createText(
                    nxpos + v, nypos + (y++ * 10), 11, sf::Color::White,
                    "Endurance:", "",
                    npclist.at(gvars::myTarget).skills.endurance, " : ", "",
                    npclist.at(gvars::myTarget).skills.endurancexp);
                textList.createText(
                    nxpos + v, nypos + (y++ * 10), 11, sf::Color::White,
                    "Dexterity:", "",
                    npclist.at(gvars::myTarget).skills.dexterity, " : ", "",
                    npclist.at(gvars::myTarget).skills.dexterityxp);
                textList.createText(
                    nxpos + v, nypos + (y++ * 10), 11, sf::Color::White,
                    "Agility:", "", npclist.at(gvars::myTarget).skills.agility,
                    " : ", "", npclist.at(gvars::myTarget).skills.agilityxp);
                textList.createText(nxpos + v, nypos + (y++ * 10), 11,
                                    sf::Color::White, "Tags:",
                                    npclist.at(gvars::myTarget).tags);

                if (npclist.at(gvars::myTarget).inventory.size() != 0 ||
                    npclist.at(gvars::myTarget).bloodcontent != "")
                {
                    effects.createSquare(nxpos, nypos, nxpos + 130, nypos + 70,
                                         sf::Color(0, 0, 0, 100));
                    int yv = nypos;
                    for (auto const &item :
                         npclist.at(gvars::myTarget).inventory)
                    { // Listing all the current items from this critters inventory.
                        if (item.insidePart.size() == 0)
                        {
                            textList.createText(nxpos + 65, yv, 11,
                                                sf::Color::White, item.name,
                                                ": ", item.amount);
                            yv += 10;
                        }
                    }

                    for (auto const &item :
                         npclist.at(gvars::myTarget).inventory)
                    { // Listing all items from 'inside' the critter.
                        if (item.insidePart.size() != 0)
                        {
                            textList.createText(
                                nxpos + 65, yv, 11, sf::Color(255, 200, 200),
                                "Inside " + item.insidePart + " :",
                                item.name + " :", item.amount);
                            yv += 10;
                        }
                    }
                    textList.createText(
                        nxpos + 65, yv, 11, sf::Color(255, 150, 150),
                        "Blood: " + npclist.at(gvars::myTarget).bloodcontent);

                    Button var;
                    var.color = sf::Color::Red;
                    var.iSize = 5;
                    var.vPos = sf::Vector2f(nxpos + 120, nypos + 50);
                    var.sButtonText = "Howdy";
                    vButtonList.push_back(var);
                    if (buttonClicked(var.id))
                    {
                        std::cout << "Twas' Truuuuuuue \n";
                    } // TODO: Get this before the MyTarget -1 check up there.
                }
                //Effectz.CreateLine(Nxpos,Nypos,MousePos.x,MousePos.y,2,Green,0,White);
                effects.createLine(npclist.at(gvars::myTarget).xpos,
                                   npclist.at(gvars::myTarget).ypos,
                                   npclist.at(gvars::myTarget).targetPos.x,
                                   npclist.at(gvars::myTarget).targetPos.y, 1,
                                   sf::Color::Yellow);
            }

            //else{MyTargetid = -1;}
            { // Mousing over items will say a wee bit about them.
                for (auto const &item : worlditems)
                {
                    if (math::closeish(gvars::mousePos.x, gvars::mousePos.y,
                                       item.xpos, item.ypos) <= 10)
                    {
                        textList.createText(item.xpos, item.ypos, 11,
                                            sf::Color::White, item.name, " ID:",
                                            item.id);
                    }
                }
            }

            if (gvars::debug)
                std::cout << "Pre Draw Stuffs \n";

            //DrawStuffs();

            if (gvars::drawStuffsDone == true)
            {
                //App.setActive(false);
                gvars::drawStuffsDone = false;
                drawStuffs();
                //ThreadDrawStuffs.launch();
            }

            if (gvars::debug)
                std::cout << "Post Draw Stuffs \n";

            window.display();
            window.clear();
        }
        plyAct = false;
        debug("Starting Removing process, NPC/Unpoint/Items/GC.Menu");
        removeNPCs();
        unpointItems(worlditems);
        removeItems(worlditems);

        if (gCtrl.menuEndPos == sf::Vector2f(-10000, -10000))
        {
            gCtrl.menuPtrCon.pItem = nullptr;
            gCtrl.menuPtrCon.pNPC = nullptr;
            gCtrl.menuPtrCon.pTile = nullptr;
            gCtrl.menuPtrCon.pVecItem.clear();
            gCtrl.menuPtrCon.pVecNPC.clear();
            gCtrl.menuPtrCon.pVecTile.clear();
        }
        debug("Finished removing process");

        if (inputState.lmb && gvars::buttonClickedTime == 0 &&
            !aabb(gvars::mousePos, gCtrl.menuPos.x, gCtrl.menuEndPos.x,
                  gCtrl.menuPos.y, gCtrl.menuEndPos.y) &&
            gCtrl.menuPos != sf::Vector2f(-10000, -10000))
        {
            gCtrl.menuType = "NULL";
            gCtrl.menuPos = sf::Vector2f(-10000, -10000);
            gCtrl.menuEndPos = sf::Vector2f(-10000, -10000);
            gCtrl.menuPtrCon.pItem = nullptr;
            gCtrl.menuPtrCon.pNPC = nullptr;
            gCtrl.menuPtrCon.pTile = nullptr;
            con("Closing Menus Due To Outside Clicking");
        }

    } // End of game loop
    astar::end();
    return EXIT_SUCCESS;
}
