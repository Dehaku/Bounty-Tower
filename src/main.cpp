#include <cstring>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <thread>
#ifndef GALAXY_LINUX
#include <windows.h>
#endif

#include "Shaders.h"
#include "Items.h"
#include "Faction.h"
#include "Tiles.h"
#include "FactionJobSecurity.h"
#include "Galaxy.h"
#include "Bullet.h"
#include "BountyTower.h"
#include "menus.h"
#include "astar.h"
#include "Textures.h"
#include "Sounds.h"
#include "math.h"
#include "util.h"
#include "globalvars.h"
#include "Networking.h"
#include "PathingController.h"

#include "Camera.h"


#include <SFML/Audio.hpp>
#include "AnimatedSprite.hpp"

#include "micropather.h"
using namespace micropather;

using std::abs;

void drawBeams();

void drawStoredPath(std::vector<Tile *> storedPath)
{
    Vec3 oldPos;
    bool firstRun = true;

    for (auto &i : storedPath)
    {
        Vec3 pathPos = Vec3(i->getPos());
        sf::Color pathColor(255, 255, 255, 100);

        if (!firstRun)
            shapes.createLine((oldPos.x + 1) * GRID_SIZE - (GRID_SIZE/2),
                                (oldPos.y + 1) * GRID_SIZE - (GRID_SIZE/2),
                                (pathPos.x + 1) * GRID_SIZE - (GRID_SIZE/2),
                                (pathPos.y + 1) * GRID_SIZE - (GRID_SIZE/2), 5, pathColor);

        oldPos = pathPos;
        firstRun = false;
    }
}


//-- Prototypes
//

void drawStuffs();

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
sf::RenderWindow window;



class Blob
{
public:
    int sizeX;
    int sizeY;
    int sizeZ;

    int regionX;
    int regionY;


    std::vector<std::vector<std::vector<Tile>>> tiles;

    void resizeGrid(int x, int y, int z)
    {
        tiles.resize(x);

        for(int i = 0; i != x; i++)
        {
            tiles.at(i).resize(y);
        }
        for(int i = 0; i != x; i++)
        {
            for(int t = 0; t != y; t++)
                tiles.at(i).at(t).resize(z);
        }
        sizeX = x;
        sizeY = y;
        sizeZ = z;
    }

    Blob()
    {
        sizeX = 0;
        sizeY = 0;
        sizeZ = 0;
        regionX = 0;
        regionY = 0;
    }
};


class UniverseTiles : public Graph
{
private:
    UniverseTiles(const UniverseTiles &);
    void operator=(const UniverseTiles &);
public:
    std::list<Blob> blobs;


    void NodeToXYZ(void *node, int *x, int *y, int *z)
    {
        Tile *Nodeling = static_cast<Tile *>(node);

        *x = Nodeling->pos.x;
        *y = Nodeling->pos.y;
        *z = Nodeling->pos.z;
    }

    void *XYZToNode(int x, int y, int z, std::vector<std::vector<std::vector<Tile>>> &Tiles)
    {
        return (void *)&(Tiles[x][y][z]);
    }

    int Passable(int nx, int ny, int nz)
    {
        if (nx >= 0 && nx < GRIDS && ny >= 0 && ny < GRIDS &&
            nz >= 0 && nz < CHUNK_SIZE)
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


    MPVector<void *> microPath;
    MicroPather *pather;
    std::vector<Tile *> storedPath;

    UniverseTiles() : pather(0)
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
                shapes.createLine((oldPos.x + 1) * 20 - 10,
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
            sf::Color pathColor(255, 255, 255, 100);

            if (!firstRun)
                shapes.createLine((oldPos.x + 1) * 20 - 10,
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

    int makePath(Vec3 Ori, Vec3 Tar)
    {
        int result = 0;
        if (Passable(Tar.x, Tar.y, Tar.z) == 1)
        {

            float totalCost;

            result = pather->Solve(XYZToNode(Ori.x, Ori.y, Ori.z, tiles),
                                   XYZToNode(Tar.x, Tar.y, Tar.z, tiles), &microPath,
                                   &totalCost);

            if (result == MicroPather::SOLVED)
            {
                unsigned int pathSize = microPath.size();
                for (int i = 0; i != pathSize; i++)
                {
                    storePath(microPath[i]);
                }
            }
        }
        return result;
    }

    void makeTest()
    {
        Blob blobo;
        blobo.resizeGrid(3,3,3);
        for(int x = 0; x != blobo.sizeX; x++)
            for(int y = 0; y != blobo.sizeY; y++)
        {
            blobo.tiles[x][y][0].dirt();
            blobo.tiles[x][y][1].stoneWall();
            blobo.tiles[x][y][2].dirt();
        }
        blobo.regionX = 0;
        blobo.regionY = 0;
        blobs.push_back(blobo);
        blobo.regionX = 2;
        //blobo.regionY = 7;
        blobs.push_back(blobo);
        blobo.regionX = 1;
        blobo.regionY = 0;
        for(int x = 0; x != blobo.sizeX; x++)
            for(int y = 0; y != blobo.sizeY; y++)
        {
            blobo.tiles[x][y][1].dirt();
        }
        blobo.tiles[1][1][0].stoneWall();
        blobs.push_back(blobo);
    }

    void drawTiles()
    {
        int TiSi = GRID_SIZE; // TileSize
        int ReSi = 3*TiSi; // RegionSize, AKA Region Spacing
        for(auto &i : blobs)
        {
            for(int x = 0; x != i.sizeX; x++)
                for(int y = 0; y != i.sizeY; y++)
                    for(int z = 0; z != i.sizeZ; z++)
            {
                int StartX = 200+(i.regionX*ReSi) + x*TiSi;
                int StartY = 200+(i.regionY*ReSi) + y*TiSi;
                int EndX = 200+(i.regionX*ReSi) + (x+1)*TiSi;
                int EndY = 200+(i.regionY*ReSi) + (y+1)*TiSi;
                if(z == 0 && !i.tiles[x][y][z].walkable)
                    shapes.createSquare(StartX+4,StartY+4,EndX-4,EndY-4,sf::Color::Transparent,1,sf::Color::Red);
                if(z == 1 && !i.tiles[x][y][z].walkable)
                    shapes.createSquare(StartX+2,StartY+2,EndX-2,EndY-2,sf::Color::Transparent,1,sf::Color::Green);
                if(z == 2 && !i.tiles[x][y][z].walkable)
                    shapes.createSquare(StartX,StartY,EndX,EndY,sf::Color::Transparent,1,sf::Color::Blue);
            }
        }
    }

    virtual ~UniverseTiles()
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
            StateCost nodeCost = {XYZToNode(N.x, N.y, N.z, tiles), 3};
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
                    StateCost nodeCost = {XYZToNode(nx, ny, nz, tiles), cost[i]};
                    neighbors->push_back(nodeCost);
                }
                else if ( (pass == 1 || pass == 2 || pass == 3) && dz[i] == 0)
                {
                    StateCost nodeCost = {XYZToNode(nx, ny, nz, tiles), cost[i]};
                    neighbors->push_back(nodeCost);
                }
                else if (pass == 3 && dz[i] == -1 && dx[i] == 0 && dy[i] == 0)
                {
                    StateCost nodeCost = {XYZToNode(nx, ny, nz, tiles), cost[i]};
                    neighbors->push_back(nodeCost);
                }
                else if (pass == 2 && dz[i] == 1 && dx[i] == 0 && dy[i] == 0)
                {
                    StateCost nodeCost = {XYZToNode(nx, ny, nz, tiles), cost[i]};
                    neighbors->push_back(nodeCost);
                }
                else
                {
                    StateCost nodeCost = {XYZToNode(nx, ny, nz, tiles), FLT_MAX};
                    neighbors->push_back(nodeCost);
                }
            }
        }
    }

    virtual void PrintStateInfo(void *node)
    {
        int x, y, z;
        NodeToXYZ(node, &x, &y, &z);
        printf("(%d,%d,%d)", x, y, z);
    }

};
UniverseTiles UnyTiles;


void updateItem()
{
    sf::Lock lock(mutex::itemList);
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
                std::cout << item.name << "'s turn! ";
            }
            if (item.produces == true)
            {
                if (gvars::debug)
                {
                    std::cout << item.name << " can produce. ";
                }
                item.prodratetimer++;
                if (item.prodratetimer >= item.prodrate && network::connectedServer == "")
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
                    int z;
                    while (findEmpty == false)
                    {
                        x = randz(item.xpos - 10, item.xpos + 10);
                        y = randz(item.ypos - 10, item.ypos + 10);
                        z = item.zpos;
                        if (tiles[abs_to_index(x / GRID_SIZE)][abs_to_index(
                                y / GRID_SIZE)][abs_to_index(
                                z / GRID_SIZE)].walkable == true)
                        {
                            findEmpty = true;
                        }
                    }
                    if (gvars::debug)
                    {
                        std::cout << "Producing: " << s << " ";
                    }
                    spawnItem(s, x, y);
                    if (gvars::debug)
                    {
                        std::cout << item.name << " has produced: " << s
                                  << " ";
                    }
                }
            }
            if (gvars::debug)
            {
                std::cout << "Done with: " << item.name << " \n";
            }

            if(item.name == "Rune")
            {
                if(randz(1,30) == 30)
                {
                    Orb orb;
                    orb.pos = sf::Vector2f(item.xpos,item.ypos);
                    orb.orbDistance = randz(30,50);
                    orb.orbSpeed = randz(1,5);
                    orb.type = randz(0,3);
                    orb.lifeTime = randz(30,600);
                    Orbs.push_back(orb);
                }
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

                    sf::Packet pack;
                    pack << ident.tilesUpdate;

                    for (int itLength = 0; itLength != 5; itLength++)
                    {



                        for (int rot = 1; rot != 361; rot++)
                        {
                            //int Rot = GX;
                            int itemX = item.xpos / 20, itemY = item.ypos / 20;
                            int XPos = ((itemX) +
                                        cosf(rot * PI / 180) * itLength);
                            int YPos = ((itemY) +
                                        sinf(rot * PI / 180) * itLength);
                            tiles[XPos][YPos][30].stone();

                            pack << gvars::currentregionx << gvars::currentregiony;
                            pack << XPos << YPos << 30;
                            pack << tiles[XPos][YPos][30].id;

                        }
                    }
                    if(network::connectedServer != "")
                    {
                        cliSocket.send(pack);
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
        sf::Lock lock(mutex::npcList);
        //for (me = npclist.begin(); me != npclist.end(); ++me)
        for (auto &me : npclist)
        {
            if (math::closeish(x, y, me.xpos, me.ypos) <= me.reach &&
                me.id != id)
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
            shapes.createCircle(x, y, 1, sf::Color::White);
        }
    }
    if (vectorID.size() == 0)
    {
        vectorID.push_back(-1);
    }

    throw std::runtime_error("npcTrace: couldn't return a value");
}

int getNpcVectorId(int id)
{
    sf::Lock lock(mutex::npcList);
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

entityvectorpointercontainer entityTrace(Vec3 Ori, Vec3 Tar) /* TODO: Improve this to use Z axis as well, or find new formula that can. THERE ARE NONE, GRAAAAH*/
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

    sf::Lock lock(mutex::npcList);
    for (int k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        if (tiles[abs_to_index(x / GRID_SIZE)][abs_to_index(y / GRID_SIZE)][abs_to_index(Ori.z/20)]
                .transparent == false)
        {

            if (inputState.key[Key::Quote])
            {
                shapes.createLine(x, y, Ori.x, Ori.y, 1, sf::Color::Cyan);
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
        EVPC.tiles.insert(&tiles[abs_to_index(x / GRID_SIZE)][abs_to_index(y / GRID_SIZE)][abs_to_index(Ori.z / GRID_SIZE)]);



        if (inputState.key[Key::Quote])
        {
            shapes.createLine(x, y, Ori.x, Ori.y, 1, sf::Color::Blue);
        }
        sf::Vector2f pos(abs_to_index(x / GRID_SIZE),
                         abs_to_index(y / GRID_SIZE));
        sf::Vector2f tar(abs_to_index(Tar.x / GRID_SIZE),
                         abs_to_index(Tar.y / GRID_SIZE));
    }
    return EVPC;
}

void handsOffense(Npc &npc, std::list<Npc> &container, Npc * closEnmy, bool &hasPath, Vec3 &endPos)
{
    for(int i = 0; i != 2; i++)
    { // TODO: GALAXY: Increase the slots iterated through based on the amount of hands the critter has! Multiarmed critters!
        Item * weapon = npc.invSlots[i];

        if(weapon == nullptr)
            continue;

        // Making sure the weapon has the right owner for later pointing.
        weapon->user = &npc;

        // Working the item until it's ready to fire.
        if(!weapon->isReady())
            weapon->trigger();

        // If it's a gun, Let's make sure it's loaded.
        if(weapon->type == 2)
            checkAmmo(npc,container,weapon);

        if(inputState.key[Key::LAlt] && !selectedNPCs.empty() && npc.id == selectedNPCs[0]->id)
        { // Setting up radiuses so we can see how far our weapon can strike.
            if(weapon->type == 2)
                shapes.createCircle(npc.xpos,npc.ypos,weapon->getRange(),sf::Color(255,0,0,50),2,sf::Color::Red);
            if(weapon->type == 1)
                shapes.createCircle(npc.xpos,npc.ypos,weapon->getRange(),sf::Color(0,0,255,50),2,sf::Color::Blue);
            if(weapon->type == 23)
                shapes.createCircle(npc.xpos,npc.ypos,weapon->getRange(),sf::Color(0,255,0,50),2,sf::Color::Green);
        }

        // We check if the target is within range of the current weapon, and if we can actually directly see them.
        bool withinRange = false;
        bool canSee = false;

        if(closEnmy != nullptr)
        {
            if(weapon->type == 2)
            { // Gun Skills!
                Skill * snipeShot = npc.skills.getSkill("Snipe Shot");
                if(snipeShot != nullptr && snipeShot->ranks > 0 && snipeShot->cooldown <= 0 && snipeShot->autouse)
                { // TODO: Figure out how to not use duplicate code here... ugh.

                    withinRange = (math::closeish(npc.xpos,npc.ypos,closEnmy->xpos,closEnmy->ypos) <= weapon->getRange()+(weapon->getRange()*(snipeShot->ranks*0.50)));

                    //Checking for Ammo, else WithinRange = false.
                    if(getItemType(weapon->internalitems,weapon->ammotype) == nullptr)
                        withinRange = false;

                    //Making sure they're in range before doing the raytrace, since it's very slow.
                    if(withinRange && weapon->isReady())
                        canSee = canSeeNpc(npc,*closEnmy);

                    if(withinRange && canSee )
                    {
                        // Firing once the weapon is ready.
                        if(weapon->isReady())
                        {
                            snipeShot->cooldown = snipeShot->cooldownint;
                            weapon->trigger();
                            int damStorage = weapon->maxdam;
                            weapon->maxdam = weapon->maxdam+(weapon->maxdam*(snipeShot->ranks*0.50));
                            std::string Status = weapon->activate(Vec3f(closEnmy->xpos,closEnmy->ypos,closEnmy->zpos));
                            weapon->maxdam = damStorage;
                            AnyDeletes(weapon->internalitems);
                        }
                    }
                    withinRange = false;

                }
            }



            withinRange = (math::closeish(npc.xpos,npc.ypos,closEnmy->xpos,closEnmy->ypos) <= weapon->getRange());

            //Making sure they're in range before doing the raytrace, since it's very slow,
            //Added isReady, No point in checking if we can see them if we can't do anything about it.
            if(withinRange && canSee == false && weapon->isReady())
                canSee = canSeeNpc(npc,*closEnmy);

            if(withinRange && canSee)
            {
                // Making sure we're ready to strike.
                if(weapon->isReady())
                {
                    weapon->trigger();
                    std::string Status = weapon->activate(Vec3f(closEnmy->xpos,closEnmy->ypos,closEnmy->zpos));
                    AnyDeletes(weapon->internalitems);
                }
            }




        }


        if(weapon->type == 2)
        {// Here we do ranged weapon active skills.
            Skill * snipeShot = npc.skills.getSkill("Snipe Shot");
            if(snipeShot != nullptr && snipeShot->ranks > 0 && snipeShot->cooldown <= 0)
            {

                skillKeepInfo * sKI;
                sKI = getSkillKeep();
                if(sKI != nullptr && sKI->user->id == npc.id && sKI->skillName == "Snipe Shot")
                {
                    withinRange = (math::closeish(npc.xpos,npc.ypos,sKI->usePos.x,sKI->usePos.y) <= weapon->getRange()+(weapon->getRange()*(snipeShot->ranks*0.50)));

                    //Checking for Ammo, else WithinRange = false.
                    if(getItemType(weapon->internalitems,weapon->ammotype) == nullptr)
                        withinRange = false;

                    if(withinRange)
                    {
                        shapes.createLine(sKI->user->xpos,sKI->user->ypos,sKI->usePos.x,sKI->usePos.y,1,sf::Color::Cyan);
                        shapes.createCircle(sKI->usePos.x,sKI->usePos.y,15,sf::Color::Cyan);
                    }
                    else
                        shapes.createCircle(sKI->usePos.x,sKI->usePos.y,15,sf::Color::Red);

                    //Do bullet prediction for the player to see the skill in action.
                    {
                        //Maybe not.
                    }


                    // Firing once the weapon is ready, and the user clicks with the skill.
                    //if(weapon->isReady() && inputState.lmbTime == 1 && withinRange)
                    if(inputState.lmbTime == 1 && withinRange)
                    {
                        snipeShot->cooldown = snipeShot->cooldownint;
                        weapon->trigger();
                        int damStorage = weapon->maxdam;
                        weapon->maxdam = weapon->maxdam+(weapon->maxdam*(snipeShot->ranks*0.50));
                        std::string Status = weapon->activate(Vec3f(sKI->usePos.x,sKI->usePos.y,gvars::currentz));
                        weapon->maxdam = damStorage;
                        AnyDeletes(weapon->internalitems);
                    }
                }
            }
        }

        if(weapon->type == 1)
        {// Here we do melee weapon active skills.

            Skill * cleave = npc.skills.getSkill("Cleave");

            if(cleave != nullptr && cleave->ranks > 0 && cleave->cooldown <= 0)
            {

                skillKeepInfo * sKI;
                sKI = getSkillKeep();
                if(sKI != nullptr && sKI->user->id == npc.id && sKI->skillName == "Cleave")
                {
                    npc.desiredViewAngle = gvars::mousePos;

                    //Giving a cleave preview of sorts.
                    sf::Color coloryo(sf::Color::Cyan);
                    coloryo.a = 50;
                    shapes.createCone(npc.getPos2d(), npc.angle, 180, 120, coloryo);

                    if(inputState.lmbTime == 1)
                    { //TODO: Make it so you drag, applying heldPos to sKI, The held position is where the critter will stand, the released position is where it will strike.
                        cleave->cooldown = cleave->cooldownint;

                        sf::Vector2f oriPos(npc.getPos2d());
                        sf::Vector2f offSet = math::angleCalc(oriPos,math::constrainAngle(npc.angle+90),60);

                        shapes.createImageButton(offSet,texturemanager.getTexture("Slash.png"),"",math::constrainAngle(npc.angle+180) );

                        for(auto &npcCleave : npclist)
                        {
                            //Making sure this critter isn't friendly, itself, or already been hit by the attack.
                            if(npcCleave.faction == npc.faction || npcCleave.id == npc.id)
                                continue;

                            //Figuring out if the target is within range, and within an angle.
                            int targetAngle = math::angleBetweenVectors(oriPos,npcCleave.getPos2d());
                            int angleDiff = math::angleDiff(npc.angle,targetAngle);
                            angleDiff = math::constrainAngle(angleDiff-90);
                            int dist = math::closeish(oriPos,npcCleave.getPos2d());

                            if(angleDiff < 90 && angleDiff > -90 && dist <= 120)
                            {
                                //Found someone within range! STRIKING!
                                std::cout << npcCleave.id << "; Someone was hit by the Cleave! \n";
                                int cleaveDamage = weapon->maxdam*(0.75+(cleave->ranks*0.25));
                                npc.dealDamage(&npcCleave,weapon,cleaveDamage);
                            }
                        }
                    }
                }
            }
        } //


    }
}

void assaultDesire(Npc &npc, std::list<Npc> &container, Npc * closEnmy, bool &hasPath, Vec3 &endPos)
{
    if(closEnmy != nullptr && npc.chasePriority == "Assault")
    {
        hasPath = true;
        endPos = Vec3(closEnmy->xpos,closEnmy->ypos,closEnmy->zpos);
    }
    if(npc.chasePriority == "Defend" || npc.isSquaddie && inputState.key[Key::Space])
    {

        if(npc.isSquaddie)
            shapes.createCircle(npc.chaseDefendPos.x,npc.chaseDefendPos.y,npc.chaseRange,sf::Color::Transparent,1,sf::Color::White);

        Npc * closestEnemy = nullptr;
        int closestEnemyDistance = 0;
        for(auto &enemy : npclist)
        {

            //Enemy needs to be alive to be worth a threat, and their faction cannot be ours...
            if(enemy.alive && enemy.faction != npc.faction)
            {
                //Find out if the 'enemy' faction is actually our enemy, If not, continue on!
                if(npc.factionPtr->getFactionRelations(enemy.faction) > -1000)
                    continue;


                int distance = math::distance(npc.chaseDefendPos,enemy.getPos());
                if(distance <= npc.chaseRange)
                {
                    if(closestEnemy == nullptr)
                    {
                        closestEnemy = &enemy;
                        closestEnemyDistance = distance;
                    }
                    else if(distance < closestEnemyDistance)
                    {
                        closestEnemy = &enemy;
                        closestEnemyDistance = distance;
                    }
                    //shapes.createCircle(enemy.xpos,enemy.ypos,10,sf::Color::Red);
                }

            }
        }

        if(closestEnemy != nullptr)
        {

            //shapes.createCircle(closestEnemy->xpos,closestEnemy->ypos,10,sf::Color::Green);
            hasPath = true;
            endPos = Vec3(closestEnemy->xpos,closestEnemy->ypos,closestEnemy->zpos);
        }
        else
        {
            hasPath = true;
            endPos = Vec3(npc.chaseDefendPos.x,npc.chaseDefendPos.y,npc.chaseDefendPos.z);
        }

    }
    debug("0");

    //This makes it so Towerlings defending switches will start chasing once they get hurt.
    if(npc.faction != conFact->name && npc.chasePriority == "Defend")
        if(npc.health != npc.getMaxHealth())
            npc.chasePriority = "Assault";

    handsOffense(npc,container,closEnmy,hasPath,endPos);

    debug("4");
}

void critterPathFind(Npc &npc, std::list<Npc> &container)
{
    Vec3 startPos(npc.xpos/GRID_SIZE,npc.ypos/GRID_SIZE,npc.zpos/GRID_SIZE);

    //Finding out if we're on a walkable tile or not, The pather can't handle starting in a non-walkable tile.
    bool npcWalkable = tiles[abs_to_index(npc.xpos/GRID_SIZE)][abs_to_index(npc.ypos/GRID_SIZE)][abs_to_index(npc.zpos/GRID_SIZE)].walkable;

    //Quick conversion for grid-usage.
    Vec3 npcEndPosGrid = Vec3(npc.endPos.x/GRID_SIZE,npc.endPos.y/GRID_SIZE,npc.endPos.z/GRID_SIZE);

    /*
    if(npc.needsPath)
        npc.hasPath = true;
    */

    bool newPathNeeded = false;
    if(npcEndPosGrid != npc.pathGrid)
        newPathNeeded = true;

    if(npc.needsPath && newPathNeeded && npcWalkable)
    {//If we need a path, and if we're able to walk, Let's get one!

        Vec3 pathPos = npcEndPosGrid;

        //Setting the tile to be walkable for the pathfinder, for jobs/orders that involve a wall.
        bool prevWalkable = tiles[pathPos.x][pathPos.y][pathPos.z].walkable;
        tiles[pathPos.x][pathPos.y][pathPos.z].walkable = true;

        //Acquiring the path.
        int result = pathCon.makePath(startPos, pathPos);
        if(result == MicroPather::NO_SOLUTION)
        {

            npc.storedPath.clear();
            npc.endPos = Vec3();
            npc.hasPath = false;
            npc.needsPath = false;
            chatBox.addChat("There's no way to get there!", sf::Color::White);
        }

        if(!pathCon.storedPath.empty())
        {
            npc.hasPath = true;
            npc.pathGrid = pathPos;
        }
        else
        {
            npc.hasPath = false;
            npc.pathGrid = Vec3();
        }


        //Setting the target tile to it's original walkable state.
        tiles[pathPos.x][pathPos.y][pathPos.z].walkable = prevWalkable;

        //Clearing the old path, and inserting the new one for storage, as long as there IS a new path.
        if( !(result == MicroPather::NO_SOLUTION) )
        {
            npc.storedPath.clear();
            for(auto &i : pathCon.storedPath)
                npc.storedPath.push_back(i);
        }

        //Cleaning the pather's storage for next use.
        pathCon.storedPath.clear();
    }

    //If we're close enough, we don't need any more paths.
    if(math::distance(npcEndPosGrid,startPos) <= npc.size/GRID_SIZE)
        npc.needsPath = false;


    //Wiping our path if we're close enough.
    if(npc.needsPath == false)
        npc.storedPath.clear();

    debug("post hasPath");

    if(!npc.storedPath.empty())
    {//We have a path! Let's do things with it.

        if(inputState.key[Key::LAlt]) // Quick visual of the path.
            drawStoredPath(npc.storedPath);

        //Getting the position of the NEXT tile in our path, since we're on the current one.
        Vec3 Pos(npc.storedPath[1]->getPos());

        //Displaying an inaccurate path trip time
        double pathTime = (((npc.storedPath.size()*GRID_SIZE)*1.2)/npc.moverate)/30;
        std::ostringstream out;
        out << std::setprecision(2) << pathTime;
        std::string pathy = "PathTime: ";
        pathy.append(out.str());
        Vec3 endPathPos(npc.storedPath[npc.storedPath.size()-1]->getPos());
        if(npc.isSquaddie)
            shapes.createText((endPathPos.x)*GRID_SIZE-GRID_SIZE,(endPathPos.y)*GRID_SIZE-GRID_SIZE,10,sf::Color(255,255,255), pathy );

        //Moving to the next position in our path!
        npc.dirMove(sf::Vector2f(Pos.x*GRID_SIZE+(GRID_SIZE/2),Pos.y*GRID_SIZE+(GRID_SIZE/2)));

        //We simply teleport through the Z-axis of our paths.
        if(Pos.z != npc.zpos)
            npc.zpos = Pos.z*GRID_SIZE;

        if(npc.storedPath.size() >= 2)
        {//Pathing through Teleporters!
            if(npc.storedPath[0]->teleporter && npc.storedPath[1]->teleporter)
            {
                npc.xpos = npc.storedPath[0]->telePos.x*GRID_SIZE;
                npc.ypos = npc.storedPath[0]->telePos.y*GRID_SIZE;
                npc.zpos = npc.storedPath[0]->telePos.z*GRID_SIZE;
            }
        }

        //Since we're storing our path, we need to delete the nodes we get close to, so we can keep moving.
        Vec3 posExtended(Pos.x*GRID_SIZE+(GRID_SIZE/2),Pos.y*GRID_SIZE+(GRID_SIZE/2),Pos.z*GRID_SIZE);
        if(math::distance(npc.getPos(),posExtended) <= npc.moverate)
            npc.storedPath.erase(npc.storedPath.begin());

    }


    else if(npc.isSquaddie && npc.storedPath.empty() && math::distance(npc.getPos(),Vec3f(npc.endPos) ) > 5 && npc.endPos.x != 0) // This allows the critter to go to the exact commanded position. (Within 5 pixels)
        npc.dirMove(sf::Vector2f(npc.endPos.x,npc.endPos.y));
    else if(math::distance(npc.getPos(),Vec3f(npc.endPos) ) < 5) // This tells the critter to get rid of their position order once they achieve it.
        npc.endPos.x = 0;

}

void overheadOverlay(Npc &npc)
{
    if(!menus.empty())
        return;
    //Overhead Healthbar.
    float remainingHealth = npc.health / npc.getMaxHealth();
    /*
    if(remainingHealth != 0 && remainingHealth != 1)
    {
        shapes.createSquare(npc.xpos-40,npc.ypos-40, npc.xpos+40, npc.ypos-40+10,sf::Color::Black);
        shapes.createSquare(npc.xpos-(40*remainingHealth),npc.ypos-40, npc.xpos+(40*remainingHealth), npc.ypos-40+8,sf::Color::Red,1,sf::Color::Black);
    }
    */

    if(npc.health != npc.getMaxHealth() && npc.health > 0)
    {
        float healthLow = math::clamp(npc.health,0,1000);
        float healthCap = 1000;
        float healthPercent = healthLow / healthCap;
        shapes.createSquare(npc.xpos-40,npc.ypos-40, npc.xpos+40, npc.ypos-40+10,sf::Color::Black);
        shapes.createSquare(npc.xpos-(40*healthPercent),npc.ypos-40, npc.xpos+(40*healthPercent), npc.ypos-40+8,sf::Color::Red,1,sf::Color::Black);
    }
    if(npc.health != npc.getMaxHealth() && npc.health > 1000) //1k - 10k Yellow bar
    {
        float healthLow = math::clamp(npc.health - 1000,0,10000 - 1000);
        float healthCap = 10000 - 1000;
        float healthPercent = healthLow / healthCap;

        shapes.createSquare(npc.xpos-(40*healthPercent),npc.ypos-40, npc.xpos+(40*healthPercent), npc.ypos-40+8,sf::Color::Yellow,1,sf::Color::Black);
    }
    if(npc.health != npc.getMaxHealth() && npc.health > 10000) //10k - 100k Green bar
    {
        float healthLow = math::clamp(npc.health - 10000,0,100000 - 10000);
        float healthCap = 100000 - 10000;
        float healthPercent = healthLow / healthCap;

        shapes.createSquare(npc.xpos-(40*healthPercent),npc.ypos-40, npc.xpos+(40*healthPercent), npc.ypos-40+8,sf::Color::Green,1,sf::Color::Black);
    }
    if(npc.health != npc.getMaxHealth() && npc.health > 100000) //100k - 1m Blue bar
    {
        float healthLow = math::clamp(npc.health - 100000,0,1000000 - 100000);
        float healthCap = 1000000 - 100000;
        float healthPercent = healthLow / healthCap;

        shapes.createSquare(npc.xpos-(40*healthPercent),npc.ypos-40, npc.xpos+(40*healthPercent), npc.ypos-40+8,sf::Color::Blue,1,sf::Color::Black);
    }

    /*

    if(inputState.key[Key::LAlt])
        if(inputState.key[Key::LShift])
            if(inputState.key[Key::Z].time == 1 || inputState.key[Key::Z].time >= 15)
                npc.modhealth(-random(250,750));


    if(inputState.key[Key::LShift])
        if(inputState.key[Key::LControl])
            if(inputState.key[Key::X].time == 1)
    {
        npc.health = 150000;
        npc.getMaxHealth() = 150000;
    }

    */


    //Overhead Healthtext.
    //int critterHealth = npc.health;
    //shapes.createText(npc.xpos-30,npc.ypos-40,10,sf::Color::White,"Health: " + std::to_string(critterHealth));

    //Overhead Name

    std::string testString;
    if(npc.name != npc.race)
        testString.append(npc.name);

    if(inputState.key[Key::LAlt])
        testString.append(" Health: " + str(static_cast<int>(npc.health)) + "/" + str(static_cast<int>(npc.getMaxHealth())));

    shapes.createText(npc.xpos-30,npc.ypos-55,10,sf::Color::White,testString);

}

Npc * critterDetectNearestEnemy(Npc &npc, std::list<Npc> &container)
{
    std::vector<Npc*> enemyPtrs;
    Npc * closEnmy = nullptr;

    if(bountytower::towerlingassault)
    {
        for (auto &enemys : container)
        {
            if( enemys.factionPtr != nullptr && npc.factionPtr != nullptr && enemys.faction != npc.faction)
            {
                for (auto &i : npc.factionPtr->factRelations)
                {
                    if(enemys.faction == i.faction && i.appeal < 1000 && enemys.alive)
                    {
                        //std::cout << "ZE ENEMY HAS BEEN SPOTTED AT " << enemys.xpos << "/" << enemys.ypos << std::endl;
                        enemyPtrs.push_back(&enemys);
                    }
                }
            }
        }
        for (auto &enemy : enemyPtrs)
        {
            if(npc.isSquaddie)
            {
                bool canSee = canSeeNpcv2(npc,*enemy);

                if(closEnmy == nullptr)
                    closEnmy = enemy;
                else if(math::closeish(npc.xpos,npc.ypos,enemy->xpos,enemy->ypos) <
                        math::closeish(npc.xpos,npc.ypos,closEnmy->xpos,closEnmy->ypos)
                        && canSee
                        )
                {
                    closEnmy = enemy;
                }
            }
            else
            {
                if(closEnmy == nullptr)
                    closEnmy = enemy;
                else if(math::closeish(npc.xpos,npc.ypos,enemy->xpos,enemy->ypos) <
                        math::closeish(npc.xpos,npc.ypos,closEnmy->xpos,closEnmy->ypos)
                        )
                {
                    closEnmy = enemy;
                }
            }
        }
    }
    return closEnmy;
}

void critterBrain(Npc &npc, std::list<Npc> &container)
{

    //Applying momentum movement to critter
    npc.momMove();

    if(!npc.alive)
        return;

    assignItemsUser(npc, container);

    critterSkillRefresh(npc,container);

    critterLevelUp(npc,container);


    if((gvars::framesPassed % 60) == 0)
    {
        float hpRegen = npc.getMaxHealth()*(npc.skills.getRanks("Feral Regeneration")*0.002);
        npc.modhealth(hpRegen);
    }


    npc.container = &container;

    int moveSpeed = npc.moverate;

    if(inputState.key[Key::L] && inputState.key[Key::LShift])
        npc.moverate = 100;


    activeSkills(npc,container);
    workSwitch(npc,container);


    //critterWallCheck(npc,container);

    if(npc.racialAbility == "Telekinetic Field")
    {
        int fieldRange = 300;
        int fieldStrength = 4;
        fieldStrength += cos(fpsKeeper.startTime.getElapsedTime().asSeconds());

        /*
        if(inputState.key[Key::Z])
            fieldStrength += cos(fpsKeeper.startTime.getElapsedTime().asMicroseconds());
        else if(inputState.key[Key::X])
            fieldStrength += cos(fpsKeeper.startTime.getElapsedTime().asMilliseconds());
        else if(inputState.key[Key::C])
            */


        shapes.createCircle(npc.xpos,npc.ypos,fieldRange,sf::Color(100,0,100,25));


        for(auto &enemy : npclist)
        {
            if(!npc.alive)
                continue;
            if(npc.factionPtr->getFactionRelations(enemy.faction) > -1000)
                continue;

            int dist = math::distance(npc.getPos(),enemy.getPos());
            if(dist > fieldRange)
                continue;

            float enemyAngle = math::angleBetweenVectors(npc.getPos2d(),enemy.getPos2d());

            sf::Vector2f fieldPush = math::angleCalc(enemy.getPos2d(),enemyAngle,fieldStrength);

            enemy.momentum += sf::Vector2f(fieldPush.x-enemy.xpos,fieldPush.y-enemy.ypos);
        }

        for(auto &bullet : bullets)
        {
            //Bullet.
            if(bullet.owner == nullptr)
                continue;
            if(npc.factionPtr->getFactionRelations(bullet.owner->faction) > -1000)
                continue;

            int dist = math::distance(npc.getPos(),bullet.pos);
            if(dist > fieldRange)
                continue;

            bullet.speed = math::clamp(bullet.speed-(fieldStrength/2),0,99999999);

        }
    }

    if(npc.faction == "The Titanium Grip")
        scrapPickup(npc,container);
    if(npc.faction == "The Titanium Grip")
        cashPickup(npc,container);

    int alph = 255;
    //npc.img.setColor(sf::Color(255, 255, 255, alph));
    npc.img.setScale(gvars::scalex, gvars::scaley);
    npc.img.setOrigin(npc.img.getTextureRect().width / 2,
    npc.img.getTextureRect().height / 2);
    if (npc.race == "Human")
    {
        npc.img.setScale(0.5, 0.5);
    }


    if(npc.name == "BTBlankBody" && randz(1,1000) == 1000)
        soundmanager.playSound("AngryWallabee.ogg");


    npc.img.setRotation(npc.angle);

    if(onScreen(npc.getPos2d()))
        overheadOverlay(npc);

    runCritterBody(npc);
    debug("Ending Part Loop");
    debug("debug 1", false);

    /*Simulating Hunger/Thirst, Needs to be nerfed/formulated to conditions, I.E. Attributes/Parts/Weather*/
    if( (gvars::framesPassed % 10) == 0)
    {/* Every ten frames, consume nutrients  */
        npc.bloodwork("Nutrients", -1);
        npc.bloodwork("Hydration", -1);
    }


    /* *BodyPart Loop* */

    /* Critter Prioritization */

    Npc * closEnmy = critterDetectNearestEnemy(npc,container);

    if(closEnmy != nullptr)
        npc.desiredViewAngle = closEnmy->getPos2d();
        //npc.targetInfo.npc = closEnmy;

    if(npc.storedPath.size() > 3)
    {//If our path is long enough, Let's have the critter look in the direction their walking.
        Vec3 viewPos(npc.storedPath[3]->getPos());
        viewPos.x = viewPos.x*GRID_SIZE;
        viewPos.y = viewPos.y*GRID_SIZE;
        npc.desiredViewAngle = sf::Vector2f(viewPos.x,viewPos.y);
    }


    /* Critter Vision   */
    critterVision(npc,container);

    critterEquip(npc,container);

    // Method Two, Struct Desires
    std::vector<Desire> desires;

    debug("Declaring and added Desires");
    // Declaring and adding Desires
    {
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

        if(npc.factionPtr != nullptr && npc.factionPtr->jobList.size() != 0)
        {
            for (auto &jobs : npc.factionPtr->jobList)
            {
                if(jobs.pWorker == nullptr)
                {
                    //newDesire.potency = 2+700;
                    newDesire.potency = 50+500;
                    break;
                }
            }
            if(npc.jobPtr != nullptr)
                newDesire.potency += 750;
        }

    }
    desires.push_back(newDesire);
    { //Assault
        newDesire.type = "Assault";
        newDesire.potency = 0;
        //if(bountytower::towerlingassault && npc.faction == "Towerlings")
            newDesire.potency = 700;



    }
    desires.push_back(newDesire);
    }

    debug("debug 6", false);
    /*Causation to Desires*/
    // Get Critters max nutrition, then reduce it by critters nutrients in blood
    float nutrients = npc.maxhunger - npc.bloodwork("Nutrients", 0);
    float hydration = npc.maxthirst - npc.bloodwork("Hydration", 0);

    for (auto &des : desires)
    {
        if (npc.name != "Mini Turret" && des.type == "Sustainence" && GRID_SIZE == 20)
            des.potency += hydration + nutrients;
        if (des.type == "SelfDefense")
        {
            // This line makes the game freeze
            des.potency = 10000;
        }
    }

    debug("debug 7", false);
    // Finding the highest Desire
    bool inComplete;
    Desire *highestDesire = nullptr;
    bool firstIter = true;

ReDesire:
    inComplete = false;
    debug("Finding highest desire");
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

    debug("debug 8", false);
    // Acting on Highest Desire




    debug("Acting on highest Desire:" + (*highestDesire).type);
    if ((*highestDesire).type == "Apathy")
    {// TODO: Add blublublub

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
                Vec3 ItemPos((*npc.targetInfo.item).xpos,(*npc.targetInfo.item).ypos,(*npc.targetInfo.item).zpos);
                Vec3 myPos(npc.xpos,npc.ypos,npc.zpos);
                npc.endPos = Vec3(npc.targetInfo.item->xpos, npc.targetInfo.item->ypos, npc.targetInfo.item->zpos);
                npc.needsPath = true;

                //if (math::closeish(npc.xpos, npc.ypos, ItemPos.x, ItemPos.y) <= npc.size * 3)
                if(math::distance(myPos,ItemPos) <= npc.size*3 && myPos.z/GRID_SIZE == ItemPos.z/GRID_SIZE)
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
        }
    }
    if ((*highestDesire).type == "Work")
        workDesire(npc,container,npc.endPos,npc.needsPath,inComplete,highestDesire);

    if ((*highestDesire).type == "Assault")
        assaultDesire(npc, container, closEnmy, npc.needsPath, npc.endPos);


    debug("Checking inComplete:" + std::to_string(inComplete));
    // Incase the highest desire isn't completable, Go through again for the next highest desire.

    if (inComplete)
        goto ReDesire;

    /*

    if(inputState.key[Key::LAlt])
        shapes.createText(npc.xpos - 30, npc.ypos - 15, 10, sf::Color::Red,
                        (*highestDesire).type, ":", (*highestDesire).potency);

    */


    /* End of Critter Prioritization */



    //Checking and Acquiring path for movement
    critterPathFind(npc,container);

    debug("Removing stuffs");
    if(npc.factionPtr != nullptr)
        removeJobs(npc.factionPtr->jobList);
    removeItems(npc.inventory);
    debug("endCritterbrain2");

    critterPush(npc, container);

    //Setting our movespeed back to normal from Debug speedster hax.
    npc.moverate = moveSpeed;
}

void critterBrain(std::list<Npc> &npcs)
{
    sf::Lock lock(mutex::npcList);

    for (auto &npc : npcs)
    {
        critterBrain(npc, npcs);
    }
    critterPickUp();

}

void drawItems()
{
    for (auto &worlditem : worlditems)
    {
        int itemZ = worlditem.zpos/GRID_SIZE;
        if(aabb(worlditem.xpos,worlditem.ypos,gvars::topLeft.x,gvars::topRight.x,gvars::topLeft.y,gvars::bottomRight.y) && itemZ == gvars::currentz)
        {
            worlditem.img.setColor(sf::Color(255, 255, 255, 255));
            worlditem.img.setScale(gvars::scalex, gvars::scaley);
            worlditem.drawImg();
        }
    }
    debug("Done Drawing Items");
}

void displayChat(sf::Vector2f position)
{
    // restore the default view
    window.setView(gvars::hudView);


    //sf::View * view = &window.getView();
    //view = window.getView();
    if (gCtrl.phase != "MainMenu")
        shapes.createSquare(
            position.x - 10, position.y + 10, position.x + 500,
            (position.y - ((11) * 10)), // (position.y - ((chatBox.chatStorage.size() + 1) * 10)),
            sf::Color(0, 0, 0, 100), 2, sf::Color::Cyan, &gvars::hudView);

    shapes.shapes.back().layer = 300000;

    /*
    for (size_t i = 0; i != chatBox.chatStorage.size(); i++)
    {
        shapes.createText(
            position.x,
            ((position.y - ((chatBox.chatStorage.size()) * 10))-10) + (i * 10), 10,
            chatBox.chatStorage[i].color, chatBox.chatStorage[i].line);
    }
    */
    if(chatBox.chatStorage.size() == 0)
    {
        for(int i = 0; i != 10; i++)
        {
            chatBox.addChat("",sf::Color::Black);
        }
    }
    for (size_t i = chatBox.chatStorage.size(); i != chatBox.chatStorage.size()-10; i--)
    {
        sf::Vector2f drawPos(position.x,((position.y - ((chatBox.chatStorage.size()) * 10))-10) + ((i-1) * 10));
        shapes.createText(drawPos, 10, chatBox.chatStorage[i-1].color, chatBox.chatStorage[i-1].line, &gvars::hudView);
        shapes.shapes.back().layer = 300001;
    }


    if(network::chatting)
    {
        if(gvars::secondSwitch)
            shapes.createText(gvars::bottomLeft.x+10,gvars::bottomLeft.y-15,10,sf::Color::White,"Chat: " + cliCon.chatString);
        else
            shapes.createText(gvars::bottomLeft.x+10,gvars::bottomLeft.y-15,10,sf::Color::White,"Chat: " + cliCon.chatString + "|");
    }

    window.setView(gvars::view1);

}

void displayMouseItem()
{
    if(mouseItem != nullptr)
    {
        sf::Sprite SP;
        sf::Vector2u TexySize = mouseItem->img.getTexture()->getSize();
        SP.setTexture(*mouseItem->img.getTexture());
        //sf::Vector2f rPos(gvars::topLeft.x + vPos.x, gvars::topLeft.y + vPos.y);
        SP.setPosition(gvars::mousePos);
        SP.setOrigin(TexySize.x/2,TexySize.y/2);
        window.draw(SP);
    }
}




void hoverItemHUD()
{
    if(!selectedNPCs.empty() && selectedNPCs[0]->isSquaddie == false)
        return;

    if(mouseItem != nullptr)
        if(selectedNPCs.empty() || selectedNPCs[0]->id != mouseItem->user->id)
            mouseItem = nullptr;

    window.setView(gvars::hudView);
    sf::Vector2f pixelPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), gvars::hudView);

    if(!selectedNPCs.empty() && bountytower::bountytower)
        for (int i = 0; i != 20; i++)
    {
        bool isSecondSlot = false;
        if(selectedNPCs[0]->invSlots[i] != nullptr && selectedNPCs[0]->invSlots[i]->size > 1)
       {
            if(i > 0 && selectedNPCs[0]->invSlots[i-1] != nullptr && selectedNPCs[0]->invSlots[i-1]->id == selectedNPCs[0]->invSlots[i]->id)
                isSecondSlot = true;
        }


        sf::Vector2f vPos(gvars::slotPos[i]);
        bool allowPlace = true;
        bool onSlot = (math::closeish(pixelPos,vPos) <= 20);

        if(onSlot)
        {
            gvars::hovering = true;

            if(!isSecondSlot)
                shapes.createCircle(vPos.x,vPos.y,10,sf::Color(255,255,255,100),0,sf::Color::White,&gvars::hudView);
            else
                shapes.createCircle(vPos.x,vPos.y,10,sf::Color(255,0,0,200),0,sf::Color::White,&gvars::hudView);


            if(mouseItem != nullptr && mouseItem->size > 1 && i != 19)
            {
                sf::Vector2f vPos1(gvars::slotPos[i+1]);
                if(selectedNPCs[0]->invSlots[i+1] != nullptr || i == 1 || i == 19)
                {
                    shapes.createCircle(vPos1.x,vPos1.y,10,sf::Color(255,0,0,200),0,sf::Color::White,&gvars::hudView);
                    allowPlace = false;
                }
                else
                {
                    shapes.createCircle(vPos1.x,vPos1.y,10,sf::Color(255,255,255,100),0,sf::Color::White,&gvars::hudView);
                    allowPlace = true;
                }
            }
        }

        if(inputState.lmbTime == 1 && onSlot && !isSecondSlot)
        {

            if(mouseItem == nullptr && selectedNPCs[0]->invSlots[i] != nullptr)
            {
                mouseItem = selectedNPCs[0]->invSlots[i];
            }
            else if(mouseItem != nullptr)
            {
                if(selectedNPCs[0]->invSlots[i] == nullptr)
                {
                    if(mouseItem->size > 1 && allowPlace)
                    {
                        (*mouseItem->currentSlot) = nullptr;
                        selectedNPCs[0]->invSlots[i] = mouseItem;
                        //selectedNPCs[0]->invSlots[i+1] = mouseItem;
                        mouseItem->currentSlot = &selectedNPCs[0]->invSlots[i];
                        mouseItem = nullptr;
                    }
                    else if(allowPlace)
                    {
                        (*mouseItem->currentSlot) = nullptr;
                        selectedNPCs[0]->invSlots[i] = mouseItem;
                        mouseItem->currentSlot = &selectedNPCs[0]->invSlots[i];
                        mouseItem = nullptr;
                    }
                }
                else
                {
                    Item ** itemBuff = mouseItem->currentSlot;
                    Item * itemPtrBuff = selectedNPCs[0]->invSlots[i];

                    selectedNPCs[0]->invSlots[i] = mouseItem;
                    selectedNPCs[0]->invSlots[i]->currentSlot = &selectedNPCs[0]->invSlots[i];
                    (*itemBuff) = itemPtrBuff;
                    (*(*itemBuff)).currentSlot = (itemBuff);

                    mouseItem = nullptr;
                }
            }
        }
    }
    window.setView(gvars::view1);

    mouseItemFunctions();

}

void drawHudSkills(Npc &npc, sf::Vector2f spritePos)
{
    skillKeepLoop();
    AnyDeletes(skillKeeps);
    int xOffset = 0;
    for(auto &skill : npc.skills.list)
    {
        if(skill.active && skill.ranks > 0)
        {
            sf::Vector2f skillPos(spritePos.x+130+(xOffset*20),spritePos.y);
            //Yes I'm multiplying it by a boolean, so what? It works perfectly here!
            int skillRot = 180+(180*skill.autouse);
            int skillButt = shapes.createImageButton(skillPos,texturemanager.getTexture("ArrowButton.png"),"",skillRot,&gvars::hudView);
            if(skill.cooldown > 0)
            {
                shapes.createText(skillPos,15,sf::Color::White,std::to_string(skill.cooldown/60),&gvars::hudView);
                shapes.shapes.back().layer += 2;
            }


            if(shapes.shapeClicked(skillButt) && skill.cooldown <= 0)
            {
                skillKeepInfo sKI;
                sKI.user = &npc;
                sKI.skillName = skill.name;

                skillKeeps.push_back(sKI);
            }

            if(shapes.shapeHovered(skillButt))
            {
                gvars::hovering = true;
                shapes.createText(skillPos,15,sf::Color::White,skill.name,&gvars::hudView);
                shapes.shapes.back().layer += 2;

                shapes.createText(sf::Vector2f(skillPos.x,skillPos.y+20),15,sf::Color::White,"    " + skill.desc,&gvars::hudView);
                shapes.shapes.back().layer += 2;
                if(inputState.rmbTime == 1)
                {
                    toggle(skill.autouse);
                    if(skill.autouse)
                        chatBox.addChat(skill.name + "'s autouse is now ON.", sf::Color::White);
                    else
                        chatBox.addChat(skill.name + "'s autouse is now OFF.", sf::Color::White);
                }
            }
            xOffset++;
        }
    }
}

void drawSquadHud()
{
    if(!menus.empty())
        return;

    window.setView(gvars::hudView);
    int ydrawPos = 0;

    for(auto &npc : npclist)
    {

        if(npc.faction == "The Titanium Grip" && npc.isSquaddie)
        {

            sf::Vector2f spritePos(5,40+(ydrawPos*60));
            int outLine = 2;
            sf::Color highlightColor = sf::Color::Cyan;

            if(!selectedNPCs.empty())
                for(auto &selected : selectedNPCs)
                    if(npc.id == selected->id)
                    {
                        outLine = 5;
                        highlightColor = sf::Color::White;
                    }


            shapes.createSquare(spritePos.x,spritePos.y,spritePos.x+200,spritePos.y+55,sf::Color(0,50,50,100),outLine,highlightColor,&gvars::hudView);


            if(npc.skillpoints > 0)
            {
                sf::Vector2f skilltextPos = spritePos;
                skilltextPos.x += 205;
                shapes.createText(skilltextPos,10,gvars::cycleGreen,"Unspent Skillpoints!", &gvars::hudView);
            }


            spritePos.x += 2;
            shapes.createText(spritePos,10,sf::Color::White,str(ydrawPos+1) +": " + npc.name, &gvars::hudView);
            shapes.shapes.back().layer += 1;
            spritePos.y += 10;
            shapes.createText(spritePos,10,sf::Color::White,"Health: " + str(static_cast<int>(npc.health)) + "/" + str(static_cast<int>(npc.getMaxHealth())), &gvars::hudView);
            shapes.shapes.back().layer += 1;
            spritePos.y += 1;


            // Health-o-meter!
            float remainingHealth = npc.health / npc.getMaxHealth();
            shapes.createSquare(spritePos.x,spritePos.y+1,spritePos.x+150,spritePos.y+9,sf::Color(0,0,0),1,sf::Color::White,&gvars::hudView);
            shapes.createSquare(spritePos.x,spritePos.y+1,spritePos.x+(150*remainingHealth),spritePos.y+9,sf::Color(255,0,0),0,highlightColor,&gvars::hudView);

            // Level-o-meter!
            spritePos.y += 10;
            shapes.createText(spritePos,10,sf::Color::White,"Level "+str(npc.level) + ", XP: " + str(npc.xp) + "/" +str(nextLevelXpRequired(npc.level)), &gvars::hudView);
            shapes.shapes.back().layer += 1;
            spritePos.y += 1;
            float remainingXP = static_cast<float>(npc.xp) / static_cast<float>(nextLevelXpRequired(npc.level));
            shapes.createSquare(spritePos.x,spritePos.y+1,spritePos.x+150,spritePos.y+9,sf::Color(0,0,0),1,sf::Color::White,&gvars::hudView);
            shapes.createSquare(spritePos.x,spritePos.y+1,spritePos.x+(150*remainingXP),spritePos.y+9,sf::Color::Yellow,0,highlightColor,&gvars::hudView);


            // ???-o-meter!
            spritePos.y += 10;
            //shapes.createText(spritePos,10,sf::Color::White,"???: " + str(npc.health), gvars::hudView);
            spritePos.y += 1;
            //shapes.createSquare(spritePos.x,spritePos.y+1,spritePos.x+150,spritePos.y+9,sf::Color(0,0,0),1,sf::Color::White,&gvars::hudView);
            //shapes.createSquare(spritePos.x,spritePos.y+1,spritePos.x+(150*remainingHealth),spritePos.y+9,sf::Color::Blue,0,highlightColor,&gvars::hudView);


            // Profit!-o-meter!
            spritePos.y += 10;
            //shapes.createText(spritePos,10,sf::Color::White,"???: " + str(npc.health), gvars::hudView);
            spritePos.y += 1;
            //shapes.createSquare(spritePos.x,spritePos.y+1,spritePos.x+150,spritePos.y+9,sf::Color(0,0,0),1,sf::Color::White,&gvars::hudView);
            //shapes.createSquare(spritePos.x,spritePos.y+1,spritePos.x+(150*remainingHealth),spritePos.y+9,sf::Color::Green,0,highlightColor,&gvars::hudView);



            spritePos.x = 170;
            Item * leftHand = npc.invSlots[0];
            if(leftHand != nullptr && leftHand->type == 2)
            {
                Item * ammoLeft = nullptr;
                for(auto &ammo : leftHand->internalitems)
                    ammoLeft = &ammo;

                int ammoCount = 0;
                if(ammoLeft != nullptr)
                    ammoCount = ammoLeft->amount;

                float ammoRemaining = static_cast<float>(ammoCount) / static_cast<float>(leftHand->maxclip);

                // LeftHanded Ammo Remaining
                spritePos.x += 1;
                spritePos.y -= 3;
                shapes.createText(spritePos,10,sf::Color::White,"L", &gvars::hudView);
                shapes.shapes.back().layer += 2;
                spritePos.x -= 1;
                spritePos.y += 3;
                shapes.createSquare(spritePos.x,spritePos.y+8,spritePos.x+10,spritePos.y+8-48,sf::Color::Black,1,sf::Color::White,&gvars::hudView);
                shapes.createSquare(spritePos.x+1,spritePos.y+8,spritePos.x+9,spritePos.y+8-(48*ammoRemaining),sf::Color(200,200,0),0,sf::Color::White,&gvars::hudView);
                shapes.shapes.back().layer += 1;
            }


            Item * rightHand = npc.invSlots[1];
            if(rightHand != nullptr && rightHand->type == 2)
            {// RightHanded Ammo Remaining
                Item * ammoLeft = nullptr;
                for(auto &ammo : rightHand->internalitems)
                    ammoLeft = &ammo;

                int ammoCount = 0;
                if(ammoLeft != nullptr)
                    ammoCount = ammoLeft->amount;

                float ammoRemaining = static_cast<float>(ammoCount) / static_cast<float>(rightHand->maxclip);


                spritePos.x += 21;
                spritePos.y -= 3;
                shapes.createText(spritePos,10,sf::Color::White,"R", &gvars::hudView);
                shapes.shapes.back().layer += 2;
                spritePos.x -= 1;
                spritePos.y += 3;
                shapes.createSquare(spritePos.x,spritePos.y+8,spritePos.x+10,spritePos.y+8-48,sf::Color::Black,1,sf::Color::White,&gvars::hudView);
                shapes.createSquare(spritePos.x+1,spritePos.y+8,spritePos.x+9,spritePos.y+8-(48*ammoRemaining),sf::Color(200,200,0),0,sf::Color::White,&gvars::hudView);
                shapes.shapes.back().layer += 1;
            }




            ydrawPos++;
            spritePos.x = 90;
            drawHudSkills(npc,spritePos);
        }
    }

    window.setView(gvars::view1);
}

void drawEnemyCounterHud()
{
    int counter = getLivingFactionMemberCount("Towerlings");
    sf::Vector2f vPos(screen.x()/1.5,160);
    std::string outPut = "Enemies Remaining: " + std::to_string(counter);
    if(counter > 0)
        shapes.createText(vPos,15,sf::Color::White,outPut,&gvars::hudView);
}


void drawStuffs()
{
    //shapes.createText(15,15,10,sf::Color::White,"Server Port: " + std::to_string(network::mainPort));
    //shapes.createText(15,30,10,sf::Color::White,"Internal Port: " + std::to_string(network::mainPort+23));

    //sf::Vector2f correction = gridEject(gvars::mousePos);
    //shapes.createCircle(correction.x,correction.y,5,sf::Color::Red);

    for(int i = 0; i != peers.connected.size(); i++)
    {
        std::string Text = std::to_string(peers.connected[i].ping) + "Peer: " + peers.connected[i].name;
        shapes.createText(gvars::topRight.x-150,gvars::topRight.y+(i*10)+10,10,sf::Color::Yellow, Text);
    }


    drawNewTiles();
    {
        sf::Lock lock(mutex::itemList);
        drawItems();
    }

    //processLiquid();
    /*
    for(int x = 0; x != GRIDS; x++)
        for(int y = 0; y != GRIDS; y++)
    {
        if(!tiles[x][y][30].liquids.empty())
        {
            int liqAmt = tiles[x][y][30].liquids[0].amount;
            std::string liqName = tiles[x][y][30].liquids[0].name;
            sf::Color liqClr = tiles[x][y][30].liquids[0].color;
            liqClr.a = 100;

            if(math::closeish(x*20,y*20,gvars::mousePos.x,gvars::mousePos.y) <= 100)
            {
                shapes.createSquare(x*20,y*20,(x+1)*20,(y+1)*20,liqClr);
                liqClr.a = 255;
                shapes.createText(x*20+10,y*20+10,10,liqClr,std::to_string(liqAmt));
            }
        }
    }
    */

    /*
    if(inputState.key[Key::L].time == 1)
        for(int x = 0; x != GRIDS; x++)
            for(int y = 0; y != GRIDS; y++)
    {
        if(tiles[x][y][30].liquids.empty())
        {
            Liquid liq;
            liq.amount = randz(1,10);
            liq.name = "Water";
            liq.color = sf::Color::Blue;
            tiles[x][y][30].liquids.push_back(liq);
        }
    }
    */

    {
        sf::Lock lock(mutex::npcList);

        drawSelectedCritterHUD();
        drawNPCs(npclist);
    }

    if(bountytower::towerVictory)
        towerVictory();

    //hoverItemHUD();


    drawJobList(window.getView().getCenter().x - 500,
                window.getView().getCenter().y);
    debug("Drew Joblist");

    if(bountytower::towerLoaded != "")
    {
        drawSquadHud();
        drawEnemyCounterHud();
    }


    //displayChat(sf::Vector2f(gvars::bottomLeft.x + 5, gvars::bottomLeft.y - 5));
    /*
    if(!menus.empty())
    {

    }
    else
    */
    displayChat(sf::Vector2f(760, 115));
    debug("Drew Chat");

    //purtyOrbitals();

    if(inputState.key[Key::F].time == 1)
    {
        Orb orb;
        orb.pos = gvars::mousePos;
        orb.orbDistance = 50;
        orb.orbSpeed = 3;
        orb.type = 3;
        orb.lifeTime = 510;
        Orbs.push_back(orb);
    }

    {
        for(auto &i : Orbs)
        {
            i.updateOrb();
            i.drawOrb(Orbs.size());
        }
        bool finished = false;
        while(finished == false)
        {
            bool Done = true;
            for(std::vector<Orb>::iterator i = Orbs.begin(); i != Orbs.end(); i++)
            {
                finished = false;
                if(i->toDelete)
                {
                    Orbs.erase(i);
                    Done = false;
                    break;
                }
            }
            if(Done == true)
                finished = true;
        }

    }

    displayMouseItem();

    drawBeams();

    shapes.drawShapes();
    AnyDeletes(shapes.shapes);
    //effects.drawEffects();
    debug("Drew Effects");

    if(gvars::transitionLock && gCtrl.phase == "Local")
    {
        sf::Sprite spinner;
        spinner.setTexture(texturemanager.getTexture("Lock.png"));
        int Pos = (gvars::topLeft.x+gvars::topRight.x)/2;
        spinner.setPosition(Pos,gvars::topRight.y+40);
        spinner.setOrigin(10.5f,10.5f);
        window.draw(spinner);
        std::string outText = "Map transition locked, Press Space to toggle.";
        shapes.createText(Pos-(outText.size()*3),gvars::topRight.y+50,10,sf::Color::Cyan,outText);
    }

    //    DrawPlanets();//Remove this one, Reenable previous

    for (auto &button : vSquareButtonList)
    {
        button.draw();
    }
    vSquareButtonList.clear();
    debug("Drew and Cleared square buttons");

    for (auto &button : vImageButtonList)
    {
        button.draw();
    }
    vImageButtonList.clear();

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
            elem.id = gvars::globalid++;

            return &elem;
        }
    }
    if (gvars::debug)
    {
        std::cout << "Didn't Find" << strtype << " \n";
    }

    return nullptr;
}

void testProcess()
{
                /*
                if(inputState.key[Key::N])
                {
                    HANDLE explorer;
                    explorer = OpenProcess(PROCESS_ALL_ACCESS,false,2120);
                    TerminateProcess(explorer,1);
                }
                */
}

//sf::Thread TcpServerThread(&runTcpServer, network::mainPort);
//sf::Thread TcpClientThread(&runTcpClient, network::mainPort+23);

void galaxyLoop()
{

    /*
    if(network::servWait == false)
    {
        debug("Launching Server");
        TcpServerThread.launch();
        network::servWait = true;
    }
    if(network::cliWait == false)
    {
        debug("Launching Client");
        TcpClientThread.launch();
        network::cliWait = true;
    }

    DealPackets();
    if(!clients.empty())
        servCon.updateClients();

    if(inputState.key[Key::P].time == 1 && !network::chatting)
        {
            std::cout << "Sending to " << clients.size() << " clients. \n";
            sf::Packet pack;
            pack << ident.textMessage << randomWindowName();
            tcpSendtoAll(pack);
        }

        if(inputState.key[Key::Q].time == 10 && !network::chatting)
        {
            toggle(gvars::sendGrid);
            std::cout << "sendGrid: " << gvars::sendGrid << std::endl;
        }

    */


}

void galaxySetup()
{
    if (!gvars::defaultFont.loadFromFile("data/fonts/Xolonium-Regular.otf"))
        throw std::runtime_error("Failed to load font!");
    /*

    if (!gvars::defaultFont.loadFromFile("data/fonts/sansation.ttf"))
        throw std::runtime_error("Failed to load font!");

        */


    network::mainPort = randz(23636,65511);

    if (servListener.listen(network::mainPort) != sf::Socket::Done)
    {
        std::cout << "ServListen Error? \n";
    }

    /*
    else
        std::cout << "Server is listening to port " << network::mainPort << ", waiting for connections... " << std::endl;
    */
    selector.add(servListener);


    resizeGrid(GRIDS,GRIDS,CHUNK_SIZE);

    initializeTilePositions();

    window.setFramerateLimit(60); // 0 is unlimited

    gvars::view1.zoom(2);

    if (true == false)
    {   // TODO: Fix this icon crap.
        /*
        sf::Image icon;
        icon.LoadFromFile("gfx/icon.tga");
        App.SetIcon(32,32,icon.GetPixelsPtr());
        if (!Images.LoadFromFile("gfx/Images.tga"))return EXIT_FAILURE;
        sf::Image Aim;
        if (!Aim.LoadFromFile("gfx/Aim.tga"))return EXIT_FAILURE;
        */
    }
    gCtrl.phase = "MainMenu";
    gvars::currentx = 48;
    gvars::currenty = 48;
    gvars::currentz = 30;

    // Building the players faction, This is temporary.
    addInitialFaction();

}

void frames()
{
    gvars::framesPassed++;
    if(gvars::framesPassed >= 4000000)
    {
        std::cout << "Congratulations, The game has ran for four million frames! Frame counter reset. \n";
        std::cout << "Theoretically, The game is running at 30 fps, ";
        std::cout << "Using super computer math, That means the game has ran for at 'least' " << 4000000/60 << " Seconds! Holy shit! \n";
        std::cout << "Do not fear, the game will begin running again after 100 seconds, You can wait, trust me. \n";
        fSleep(100);
        gvars::framesPassed = 0;
    }

    if((gvars::framesPassed % 60) == 0)
    {
        toggle(gvars::secondSwitch);
    }
}

void genericLoop()
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
        gvars::centerScreen = gvars::view1.getCenter();

    if (inputState.key[Key::K].time == 1 && !network::chatting)
        std::cout << generateName() << std::endl;

    if (gCtrl.phase != "MainMenu" && gvars::following == false &&
            gCtrl.phase != "MakeSquad" && gCtrl.phase != "MicroPatherTest")
    {
        gvars::view1.setCenter(gvars::currentx * 20,gvars::currenty * 20);

        renderScreenShake();

    }

    for(auto &bullet : bullets)
    {
        bullet.moveBullet();
    }
    removeBullets();

}

void handleEvents()
{
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
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128 && network::chatting == true)
                {
                    if(event.text.unicode != 8 && event.text.unicode != 13) // 8 = backspace 13 = enter, Thanks to http://www.fileformat.info
                    {
                        std::string TempStr;
                        TempStr = event.text.unicode;
                        cliCon.chatString.append( TempStr );
                    }
                    if(event.text.unicode == 8 && cliCon.chatString.size() != 0)
                    {
                        cliCon.chatString.erase(cliCon.chatString.end()-1);
                    }
                    if(event.text.unicode == 13 && network::client == true)
                    {
                        network::chatting = false;
                        sf::UdpSocket socket;
                        sf::Packet ToSend;
                        std::string SendText;
                        SendText.append(cliCon.name);
                        SendText.append(": ");
                        SendText.append(cliCon.chatString);

                        ToSend << ident.textMessage << SendText;
                        if(ToSend.getDataSize() != 0)
                        {

                            sf::Packet sendpacket;

                            sendpacket << ident.textMessage << SendText;

                            // Send an answer to the server
                            if(cliSocket.send(sendpacket) != sf::Socket::Done)
                            {
                                std::cout << "!= Socket Done (In client sending chat), Apparently! Tell the Devs! \n";
                            }


                            cliCon.chatString.clear();
                        }
                    }

                    if(event.text.unicode == 13 && network::server == true)
                    {
                        network::chatting = false;
                        sf::Packet ToSend;
                        std::string SendText;
                        SendText.append("*Server*");
                        SendText.append(": ");
                        SendText.append(cliCon.chatString);

                        ToSend << ident.textMessage << SendText;
                        if(ToSend.getDataSize() != 0)
                        {

                            for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
                            {
                                if(gvars::debug) std::cout << "Running through clients \n";
                                sf::TcpSocket& client = **it;
                                client.send(ToSend);
                            }
                            cliCon.chatString.clear();
                        }
                    }
                    if(event.text.unicode == 13)
                    {
                        network::chatting = false;
                        std::string firstLetter;
                        firstLetter.append(cliCon.chatString,0,1);
                        std::cout << "(" << firstLetter << ")\n";
                        if(firstLetter == "/")
                            chatCommand(cliCon.chatString);
                        else if(firstLetter != "/" && network::connectedServer != "")
                        {
                            sf::Packet pack;
                            pack << ident.textMessage << network::name + ": " + cliCon.chatString;
                            cliSocket.send(pack);
                        }
                        else
                            chatBox.addChat(cliCon.chatString,sf::Color::White);
                        cliCon.chatString.clear();
                    }

                }
                else if(event.text.unicode == 13 && network::chatting == false)
                {
                    network::chatting = true;
                }
            }

        }
}

float Magnitude(Vec3f v1)
{
	return sqrt(pow(v1.x, 2) + pow(v1.y, 2) + pow(v1.z, 2)); //return the magnitude (pythagoras)
}

void acquireSelectedNPCs()
{
    if(!menus.empty())
        return;

    if (inputState.lmbTime == 0 &&
        gvars::heldClickPos != sf::Vector2f(-1, -1))
    {
        bool foundAny = false;
        sf::Vector2f S = gvars::heldClickPos;
        sf::Vector2f E = gvars::mousePos;
        sf::Lock lock(mutex::npcList);
        for (auto &i : npclist)
        {
            if(i.faction != conFact->name)
                continue;

            if (inbetween(S.x, E.x, i.xpos) == true)
            {
                if (inbetween(S.y, E.y, i.ypos) == true)
                {
                    if(i.alive)
                    {
                        std::cout << i.name << std::endl;
                        //gvars::selected.push_back(i.id);
                        selectedNPCs.push_back(&i);
                        foundAny = true;
                    }

                }
            }
        }
    }

    if (inputState.lmbTime > 1)
    {
        if (gvars::heldClickPos == sf::Vector2f(-1, -1))
            gvars::heldClickPos = gvars::mousePos;
        shapes.createSquare(gvars::heldClickPos.x,gvars::heldClickPos.y, gvars::mousePos.x,gvars::mousePos.y,sf::Color(0, 0, 0, 0),5,sf::Color(0, 255, 255));
        for (auto &npc : npclist)
        {

            if(npc.alive && aabb(npc.getPos2d(),gvars::heldClickPos.x,gvars::mousePos.x,gvars::heldClickPos.y,gvars::mousePos.y))
            {
                shapes.createImageButton(npc.getPos2d(),texturemanager.getTexture("SelectionCircle.png"));
            }
        }

    }
    else
        gvars::heldClickPos = sf::Vector2f(-1, -1);
}

void lmbPress()
{
    if(!menus.empty())
        return;

    hoverItemHUD();
    bool foundOne = false;
    debug("Pre Mouse Based Functions");
    if (inputState.lmb && gvars::hovering == false)
    {
        selectedNPCs.clear();
        sf::Lock lock(mutex::npcList);
        for (auto &elem : npclist)
        {
            if (elem.faction == conFact->name && inputState.lmb)
            {
                int dist = math::closeish(gvars::mousePos.x,
                                            gvars::mousePos.y,
                                            elem.xpos, elem.ypos);
                if (dist <= GRID_SIZE/2)
                {
                    selectedNPCs.push_back(&elem);

                    foundOne = true;
                    return;
                }
            }
        }
    }
    debug("Post Mouse Based Functions");
}

void handlePhase()
{
    if(bountytower::bountytower)
        bountyTowerLoop();

    if (gCtrl.phase == "Local")
    { //=======================================================*Local*============================================================================


        attractNPCs(gvars::mousePos);

        if (gvars::debug)
            shapes.createText((gvars::currentx - 2) * GRID_SIZE,
                                (gvars::currenty + 1) * GRID_SIZE, 11,
                                sf::Color::Red, "Debug On");

        if (inputState.key[Key::V].time == 1 && !network::chatting)
            chatBox.addChat(
                randomWindowName(),
                sf::Color(randz(0, 255), randz(0, 255), randz(0, 255)));
        if (inputState.key[Key::X].time == 10 && !network::chatting)
        {
            for(int x = 0; x != GRIDS; x++)
                for(int y = 0; y != GRIDS; y++)
                    for(int z = 0; z != CHUNK_SIZE; z++)
            {
                int ranNum = randz(0,6);
                if(ranNum == 6)
                    tiles[x][y][z].stoneWall();
                else if(ranNum == 5)
                    tiles[x][y][z].stone();
                else if(ranNum == 4)
                    tiles[x][y][z].grass();
                else
                    tiles[x][y][z].dirt();
                if(randz(0,100) == 100 && y != 0 && y != GRIDS-1)
                {
                    tiles[x][y][z].lava();
                    tiles[x][y-1][z].lava();
                    tiles[x][y+1][z].lava();
                }
            }
        }
        if (inputState.key[Key::Z].time == 10 && !network::chatting && clients.size() != 0)
        {
            sf::Packet pack;
            std::cout << "Packet size: " << sizeof(pack);
            pack << ident.gridUpdate;
            for(int x = 0; x != GRIDS; x++)
                for(int y = 0; y != GRIDS; y++)
                    for(int z = 0; z != CHUNK_SIZE; z++)
            {
                pack << tiles[x][y][z].id;
            }
            std::cout << ", " << sizeof(pack) << std::endl;
            tcpSendtoAll(pack);
        }


        squadHud();

        if (gvars::initalZeds)
            gCtrl.wave();

        rightMouseButtonContextMenu();

        if(inputState.key[Key::V] && !network::chatting)
        {
            for (int x = 0; x != GRIDS; x++)
                for (int y = 0; y != GRIDS; y++)
            {
                //shapes.createText(x*TILE_PIXELS,y*TILE_PIXELS,10,sf::Color::Red,std::to_string(tiles[x][y][gvars::currentz].walkable));
                if(tiles[x][y][gvars::currentz].walkable)
                    shapes.createSquare(x*TILE_PIXELS,y*TILE_PIXELS,x*TILE_PIXELS+TILE_PIXELS,y*TILE_PIXELS+TILE_PIXELS,sf::Color(255,255,0,100));
                else
                    shapes.createSquare(x*TILE_PIXELS,y*TILE_PIXELS,x*TILE_PIXELS+TILE_PIXELS,y*TILE_PIXELS+TILE_PIXELS,sf::Color(255,0,0,100));
            }
        }
        if (inputState.rmbTime == 1 && inputState.key[Key::LShift])
            rmbMenuTile(    Vec3(gvars::mousePos.x,gvars::mousePos.y,gvars::currentz)   );

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


        cameraControls();


        if (selectedNPCs.empty())
        {
            gvars::following = false;
        }
        else if (inputState.key[Key::C] && !network::chatting)
        {
            gvars::following = true;
        }
        if (gvars::following)
        {
            sf::Lock lock(mutex::npcList);

            if(!selectedNPCs.empty())
                gvars::view1.setCenter(selectedNPCs[0]->xpos,
                                   selectedNPCs[0]->ypos);
        }

        shapes.createSquare(
            32 * 20, 32 * 20, 64 * 20, 64 * 20, sf::Color(0, 0, 0, 0), 1,
            sf::Color::
                Cyan); // These two are the chunk borders, Debug stuffs so you can see where the loading boundries are. (As if you needed 'em, Damn thing lags a bit, Should thread each loading bit soon.)
        shapes.createSquare(
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

            /*

            shapes.createText(gvars::mousePos.x, gvars::mousePos.y, 11,
                                sf::Color::Red, "ID: ", "", Variable, ", Pos (" + std::to_string(Pos.x) + "/" + std::to_string(Pos.y) + "/" + std::to_string(Pos.z) + ")");

                                */
        }

        if(!selectedNPCs.empty())
        {
            shapes.createCircle(selectedNPCs[0]->xpos,selectedNPCs[0]->ypos,30,sf::Color(255,255,255,100));
        }

        if(inputState.key[Key::Space].time == 1)
        {
            toggle(gvars::transitionLock);
        }

        bool transitioning = false;
        if (gvars::currenty > 64 && !gvars::transitionLock)
        {
            tilesGoUp();
            gvars::currenty = 33;
            {
                sf::Lock lock(mutex::npcList);

                //for (size_t i = 0; i != npclist.size(); i++)
                for (auto &i : npclist)
                {
                    i.ypos += -640;
                    i.targetPos.y += -640;
                }
            }

            for (auto &worlditem : worlditems)
                (worlditem).ypos += -640;

            for (auto &i : listAt(uniFact,0).jobList)
                i.workPos.y += -640;

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
        if (gvars::currenty < 32 && !gvars::transitionLock)
        {
            tilesGoDown();
            gvars::currenty = 63;
            {
                sf::Lock lock(mutex::npcList);

                //for (size_t i = 0; i != npclist.size(); i++)
                for (auto &i : npclist)
                {
                    i.ypos += 640;
                    i.targetPos.y += 640;
                }
            }

            //for(int i = 0; i != worlditems.size(); i++) worlditems.at(i).ypos += 640;
            for (auto &worlditem : worlditems)
                (worlditem).ypos += 640;

            for (auto &i : listAt(uniFact,0).jobList)
                i.workPos.y += 640;

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
        if (gvars::currentx > 64 && !gvars::transitionLock)
        {
            tilesGoLeft();
            gvars::currentx = 33;
            {

                sf::Lock lock(mutex::npcList);

            //for (size_t i = 0; i != npclist.size(); i++)
            for (auto &i : npclist)
            {
                i.xpos += -640;
                i.targetPos.x += -640;
            }

            }
            //for(int i = 0; i != worlditems.size(); i++) worlditems.at(i).xpos += -640;
            for (auto &worlditem : worlditems)
                (worlditem).xpos += -640;

            for (auto &i : listAt(uniFact,0).jobList)
                i.workPos.x += -640;

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
        if (gvars::currentx < 32 && !gvars::transitionLock)
        {
            con("Starting GoRight");
            tilesGoRight();
            con("Ending GoRight");
            gvars::currentx = 63;
            con("Starting GoRight with NPC's and Items");
            {
                sf::Lock lock(mutex::npcList);

            //for (size_t i = 0; i != npclist.size(); i++)
            for (auto &i : npclist)
            {
                i.xpos += 640;
                i.targetPos.x += 640;
            }

            }
            //for(int i = 0; i != worlditems.size(); i++) worlditems.at(i).xpos += 640;
            for (auto &worlditem : worlditems)
                (worlditem).xpos += 640;

            for (auto &i : listAt(uniFact,0).jobList)
                i.workPos.x += 640;

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

        if (transitioning == true && !gvars::transitionLock)
        {

            {


            sf::Lock lock(mutex::npcList);
            //for (size_t i = 0; i != npclist.size(); i++)
            offloadNpcs();


            }

            removeNPCs(npclist, mutex::npcList);


            offloadItems();

            removeItems(worlditems);
            initializeTilePositions();
        }

        if (gCtrl.menuType != "NULL")
        {
            menuPopUp();
        }
        else
        {
            gCtrl.menuPos = math::Vec2f(-10000, -10000);
        }

        debug("Doing Local Items");
        updateItem();
        debug("Doing Local AddItems");
        itemmanager.addItems();
        debug("Doing Local critterBrain's");
        critterBrain(npclist);
        debug("Pre Add Critters");
        npcmanager.addCritters();
        debug("Post Add Critters");




    } //=============================================================================*End of Local*========================================================================

    /*

    if (gCtrl.phase == "MakeSquad") // Needs a heavy menu overhaul.
    {
        gvars::view1.setCenter(screen.x() / 2, screen.y() / 2);

        shapes.createText(screen.x() / 2, 20, 20, gvars::cycleBlue,
                            "Design Your Squad");
        shapes.createText(screen.x() / 2, 50, 15, sf::Color::Yellow,
                            "Squad Points: ", "", squady.makeSquadPoints);
        shapes.createText(screen.x() / 2, 70, 11, sf::Color::Yellow,
                            "Squad Mates: ", "", squady.squadMates);

        SquareButton var200;
        var200.color = sf::Color::Yellow;
        var200.iSizex = 5;
        var200.iSizey = 5;
        var200.vPos = sf::Vector2f((screen.x() / 2) - 10, 75);
        var200.sButtonText = "Howdy";

        vSquareButtonList.push_back(var200);
        if (squareButtonClicked(var200.id))
        {
            if (squady.squad.size() > 1)
            {
                int stre, perc, inte, charis, endu, dext, agil = 0;

                stre = squady.squad.at(squady.squad.size() - 1)
                           .attributes.strength;
                perc = squady.squad.at(squady.squad.size() - 1)
                           .attributes.perception;
                inte = squady.squad.at(squady.squad.size() - 1)
                           .attributes.intelligence;
                charis = squady.squad.at(squady.squad.size() - 1)
                             .attributes.charisma;
                endu = squady.squad.at(squady.squad.size() - 1)
                           .attributes.endurance;
                dext = squady.squad.at(squady.squad.size() - 1)
                           .attributes.dexterity;
                agil =
                    squady.squad.at(squady.squad.size() - 1).attributes.agility;
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
        var201.vPos = sf::Vector2f((screen.x() / 2) + 150, 75);
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
                squ.faction = conFact->name;

                squady.squad.push_back(squ);
                squady.makeSquadPoints -= 100;
            }

            fSleep(0.2);
        }

        //Squady.Squad.at(Squady.Aim).

        int spacing = 1;
        //NPC.attributes.

        SquareButton var100;
        var100.color = sf::Color::Green;
        var100.iSizex = 5;
        var100.iSizey = 5;
        var100.vPos = sf::Vector2f((screen.x() / 2) - 10,
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
        var101.vPos = sf::Vector2f((screen.x() / 2) + 150,
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

        shapes.createText(screen.x() / 2, 80 + (10 * spacing++), 11,
                            sf::Color::Green, "Human: ",
                            squady.squad.at(squady.aim).name);

        shapes.createText(screen.x() / 2, 80 + (20 * spacing), 11,
                            sf::Color::White, "Strength: ", "",
                            squady.squad.at(squady.aim).attributes.strength);

        SquareButton var;
        var.color = sf::Color::Red;
        var.iSizex = 5;
        var.iSizey = 5;
        var.vPos = sf::Vector2f((screen.x() / 2) - 10,
                                5 + (80 + (20 * spacing)));
        var.sButtonText = "Howdy";
        vSquareButtonList.push_back(var);
        if (squareButtonClicked(var.id))
        {
            if (squady.makeSquadPoints < 1000)
            {
                squady.makeSquadPoints += 1;
                squady.squad.at(squady.aim).attributes.strength -= 1;
            }
        }
        SquareButton var2;
        var2.color = sf::Color::Red;
        var2.iSizex = 5;
        var2.iSizey = 5;
        var2.vPos = sf::Vector2f((screen.x() / 2) + 150,
                                 5 + (80 + (20 * spacing++)));
        var2.sButtonText = "Howdy";
        vSquareButtonList.push_back(var2);
        if (squareButtonClicked(var2.id))
        {
            if (squady.makeSquadPoints > 0)
            {
                squady.makeSquadPoints -= 1;
                squady.squad.at(squady.aim).attributes.strength += 1;
            }
        }

        shapes.createText(screen.x() / 2, 80 + (20 * spacing), 11,
                            sf::Color::White, "Perception: ", "",
                            squady.squad.at(squady.aim).attributes.perception);

        SquareButton var3;
        var3.color = sf::Color::Red;
        var3.iSizex = 5;
        var3.iSizey = 5;
        var3.vPos = sf::Vector2f((screen.x() / 2) - 10,
                                 5 + (80 + (20 * spacing)));
        var3.sButtonText = "Howdy";
        vSquareButtonList.push_back(var3);
        if (squareButtonClicked(var3.id))
        {
            if (squady.makeSquadPoints < 1000)
            {
                squady.makeSquadPoints += 1;
                squady.squad.at(squady.aim).attributes.perception -= 1;
            }
        }
        SquareButton var4;
        var4.color = sf::Color::Red;
        var4.iSizex = 5;
        var4.iSizey = 5;
        var4.vPos = sf::Vector2f((screen.x() / 2) + 150,
                                 5 + (80 + (20 * spacing++)));
        var4.sButtonText = "Howdy";
        vSquareButtonList.push_back(var4);
        if (squareButtonClicked(var4.id))
        {
            if (squady.makeSquadPoints > 0)
            {
                squady.makeSquadPoints -= 1;
                squady.squad.at(squady.aim).attributes.perception += 1;
            }
        }

        shapes.createText(
            screen.x() / 2, 80 + (20 * spacing), 11, sf::Color::White,
            "Intelligence: ", "",
            squady.squad.at(squady.aim).attributes.intelligence);

        SquareButton var5;
        var5.color = sf::Color::Red;
        var5.iSizex = 5;
        var5.iSizey = 5;
        var5.vPos = sf::Vector2f((screen.x() / 2) - 10,
                                 5 + (80 + (20 * spacing)));
        var5.sButtonText = "Howdy";
        vSquareButtonList.push_back(var5);
        if (squareButtonClicked(var5.id))
        {
            if (squady.makeSquadPoints < 1000)
            {
                squady.makeSquadPoints += 1;
                squady.squad.at(squady.aim).attributes.intelligence -= 1;
            }
        }
        SquareButton var6;
        var6.color = sf::Color::Red;
        var6.iSizex = 5;
        var6.iSizey = 5;
        var6.vPos = sf::Vector2f((screen.x() / 2) + 150,
                                 5 + (80 + (20 * spacing++)));
        var6.sButtonText = "Howdy";
        vSquareButtonList.push_back(var6);
        if (squareButtonClicked(var6.id))
        {
            if (squady.makeSquadPoints > 0)
            {
                squady.makeSquadPoints -= 1;
                squady.squad.at(squady.aim).attributes.intelligence += 1;
            }
        }

        shapes.createText(screen.x() / 2, 80 + (20 * spacing), 11,
                            sf::Color::White, "Charisma: ", "",
                            squady.squad.at(squady.aim).attributes.charisma);

        SquareButton var7;
        var7.color = sf::Color::Red;
        var7.iSizex = 5;
        var7.iSizey = 5;
        var7.vPos = sf::Vector2f((screen.x() / 2) - 10,
                                 5 + (80 + (20 * spacing)));
        var7.sButtonText = "Howdy";
        vSquareButtonList.push_back(var7);
        if (squareButtonClicked(var7.id))
        {
            if (squady.makeSquadPoints < 1000)
            {
                squady.makeSquadPoints += 1;
                squady.squad.at(squady.aim).attributes.charisma -= 1;
            }
        }
        SquareButton var8;
        var8.color = sf::Color::Red;
        var8.iSizex = 5;
        var8.iSizey = 5;
        var8.vPos = sf::Vector2f((screen.x() / 2) + 150,
                                 5 + (80 + (20 * spacing++)));
        var8.sButtonText = "Howdy";
        vSquareButtonList.push_back(var8);
        if (squareButtonClicked(var8.id))
        {
            if (squady.makeSquadPoints > 0)
            {
                squady.makeSquadPoints -= 1;
                squady.squad.at(squady.aim).attributes.charisma += 1;
            }
        }

        shapes.createText(screen.x() / 2, 80 + (20 * spacing), 11,
                            sf::Color::White, "Endurance: ", "",
                            squady.squad.at(squady.aim).attributes.endurance);

        SquareButton var9;
        var9.color = sf::Color::Red;
        var9.iSizex = 5;
        var9.iSizey = 5;
        var9.vPos = sf::Vector2f((screen.x() / 2) - 10,
                                 5 + (80 + (20 * spacing)));
        var9.sButtonText = "Howdy";
        vSquareButtonList.push_back(var9);
        if (squareButtonClicked(var9.id))
        {
            if (squady.makeSquadPoints < 1000)
            {
                squady.makeSquadPoints += 1;
                squady.squad.at(squady.aim).attributes.endurance -= 1;
            }
        }
        SquareButton var10;
        var10.color = sf::Color::Red;
        var10.iSizex = 5;
        var10.iSizey = 5;
        var10.vPos = sf::Vector2f((screen.x() / 2) + 150,
                                  5 + (80 + (20 * spacing++)));
        var10.sButtonText = "Howdy";
        vSquareButtonList.push_back(var10);
        if (squareButtonClicked(var10.id))
        {
            if (squady.makeSquadPoints > 0)
            {
                squady.makeSquadPoints -= 1;
                squady.squad.at(squady.aim).attributes.endurance += 1;
            }
        }

        shapes.createText(screen.x() / 2, 80 + (20 * spacing), 11,
                            sf::Color::White, "Dexterity: ", "",
                            squady.squad.at(squady.aim).attributes.dexterity);

        SquareButton var11;
        var11.color = sf::Color::Red;
        var11.iSizex = 5;
        var11.iSizey = 5;
        var11.vPos = sf::Vector2f((screen.x() / 2) - 10,
                                  5 + (80 + (20 * spacing)));
        var11.sButtonText = "Howdy";
        vSquareButtonList.push_back(var11);
        if (squareButtonClicked(var11.id))
        {
            if (squady.makeSquadPoints < 1000)
            {
                squady.makeSquadPoints += 1;
                squady.squad.at(squady.aim).attributes.dexterity -= 1;
            }
        }
        SquareButton var12;
        var12.color = sf::Color::Red;
        var12.iSizex = 5;
        var12.iSizey = 5;
        var12.vPos = sf::Vector2f((screen.x() / 2) + 150,
                                  5 + (80 + (20 * spacing++)));
        var12.sButtonText = "Howdy";
        vSquareButtonList.push_back(var12);
        if (squareButtonClicked(var12.id))
        {
            if (squady.makeSquadPoints > 0)
            {
                squady.makeSquadPoints -= 1;
                squady.squad.at(squady.aim).attributes.dexterity += 1;
            }
        }

        shapes.createText(screen.x() / 2, 80 + (20 * spacing), 11,
                            sf::Color::White, "Agility: ", "",
                            squady.squad.at(squady.aim).attributes.agility);

        SquareButton var13;
        var13.color = sf::Color::Red;
        var13.iSizex = 5;
        var13.iSizey = 5;
        var13.vPos = sf::Vector2f((screen.x() / 2) - 10,
                                  5 + (80 + (20 * spacing)));
        var13.sButtonText = "Howdy";
        vSquareButtonList.push_back(var13);
        if (squareButtonClicked(var13.id))
        {
            if (squady.makeSquadPoints < 1000)
            {
                squady.makeSquadPoints += 1;
                squady.squad.at(squady.aim).attributes.agility -= 1;
            }
        }
        SquareButton var14;
        var14.color = sf::Color::Red;
        var14.iSizex = 5;
        var14.iSizey = 5;
        var14.vPos = sf::Vector2f((screen.x() / 2) + 150,
                                  5 + (80 + (20 * spacing++)));
        var14.sButtonText = "Howdy";
        vSquareButtonList.push_back(var14);
        if (squareButtonClicked(var14.id))
        {
            if (squady.makeSquadPoints > 0)
            {
                squady.makeSquadPoints -= 1;
                squady.squad.at(squady.aim).attributes.agility += 1;
            }
        }

        //cText.CreateText(screen.x()/2,80+(20*Spacing),11,sf::Color::White," ");
        SquareButton var50;
        var50.color = sf::Color::Yellow;
        var50.iSizex = 5;
        var50.iSizey = 5;
        var50.vPos = sf::Vector2f((screen.x() / 2) + 180,
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

    }

    if (gCtrl.phase == "World")
    {
        cameraControls();

        gCtrl.worldLoop();

        shapes.createSquare(gvars::topLeft.x, gvars::topLeft.y,
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

        shapes.createText(gvars::topLeft.x + 2,
                            gvars::topLeft.y + (hudz++) * 11, 22,
                            sf::Color::Yellow, "World Population: ", "",
                            factionPopulation());
        hudz++;
        hudz++;
        shapes.createText(gvars::topLeft.x + 2,
                            gvars::topLeft.y + (hudz++) * 11, 11,
                            sf::Color::White, "CurrentTileID: ", "", id);
        shapes.createText(
            gvars::topLeft.x + 2, gvars::topLeft.y + (hudz++) * 11, 11,
            sf::Color::White, "CurrentTileInfected: ", "", infected);
        shapes.createText(
            gvars::topLeft.x + 2, gvars::topLeft.y + (hudz++) * 11, 11,
            sf::Color::White, "FactionOwned: ",
            worldMap[math::clamp(abs(gvars::mousePos.x / 20), 0, 99)]
                    [math::clamp(abs(gvars::mousePos.y / 20), 0, 99)]
                        .owner);
        shapes.createText(
            gvars::topLeft.x + 2, gvars::topLeft.y + (hudz++) * 11, 11,
            sf::Color::White, "FactionMembers: ", "",
            factionMembers(worldMap[math::clamp(
                abs(gvars::mousePos.x / 20), 0,
                99)][math::clamp(abs(gvars::mousePos.y / 20), 0, 99)]
                               .owner));
        shapes.createText(
            gvars::topLeft.x + 2, gvars::topLeft.y + (hudz++) * 11, 11,
            sf::Color::White, "FactionAggression: ", "",
            factionAggression(worldMap[math::clamp(
                abs(gvars::mousePos.x / 20), 0,
                99)][math::clamp(abs(gvars::mousePos.y / 20), 0, 99)]
                                  .owner));
        shapes.createText(
            gvars::topLeft.x + 2, gvars::topLeft.y + (hudz++) * 11, 11,
            sf::Color::White, "FactionTerritories: ", "",
            factionTerritories(worldMap[math::clamp(
                abs(gvars::mousePos.x / 20), 0,
                99)][math::clamp(abs(gvars::mousePos.y / 20), 0, 99)]
                                   .owner));
        shapes.createText(
            gvars::topLeft.x + 2, gvars::topLeft.y + (hudz++) * 11, 11,
            sf::Color::White, "FactionPower: ", "",
            factionPower(worldMap[math::clamp(
                abs(gvars::mousePos.x / 20), 0,
                99)][math::clamp(abs(gvars::mousePos.y / 20), 0, 99)]
                             .owner));

        shapes.createText(gvars::topLeft.x + 2,
                            gvars::topLeft.y + (hudz++) * 11, 11,
                            sf::Color::White, "AimedPos(DELETEME): ", "",
                            abs(gvars::mousePos.x / 20), "/", "",
                            abs(gvars::mousePos.y / 20));

        drawWorldTiles();
        int xMouse(gvars::mousePos.x / 20);
        int yMouse(gvars::mousePos.y / 20);
        sf::Vector2f pos(xMouse, yMouse);
        shapes.createSquare(pos.x * 20, pos.y * 20, (pos.x * 20) + 20,
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
        shapes.createText(500, 0, 25, sf::Color::White, "Welcome!", "",
                            -6698, "", "", -6698, "", "", -6698, 1, 0);
        shapes.createText(
            450, 25, 11, sf::Color::Red,
            "Take this 'game' with a grain of salt, It's not done yet");
        shapes.createText(450, 45, 11, gvars::cycleGreen,
                            "Design Your Squad");

        shapes.createText(450, 60, 11, sf::Color::Red,
                            "Press 'm' to view MicroPather basic test");

        // TODO: Simply add cText.CreateText for the Button Text, Or at least make it an option, Since sButtonText is designed for text 'on' the button.
        SquareButton var;
        var.color = gvars::cycleRed;
        var.iSizex = 10;
        var.iSizey = 6;

        var.vPos = sf::Vector2f(440, 52);
        var.sButtonText = "Howdy";
        vSquareButtonList.push_back(var);

        shapes.createText(450, 75, 11, sf::Color::Red,
                            "Press r to turn on the "
                            "debugger, If it slows down the "
                            "game, Minimize the console.");
        shapes.createText(395, 755, 10, sf::Color::White,
                            "Pathfinding: MicroPather by Lee Thomason");








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



        if (inputState.key[Key::Comma] && !network::chatting)
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
        if (inputState.key[Key::Period] && !network::chatting)
        {
            gCtrl.phase = "World";
            gCtrl.buildWorldTest();
        }
        if (inputState.key[Key::RControl] && !network::chatting)
        {
            gCtrl.phase = "Test";
        }
    } //=============================================================================*End of Main Menu*========================================================================

        */

}

void cleanMenu()
{
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
}




struct BeamAttribute
{
    sf::Color color;
    int damage;
    BeamAttribute()
    {
        color = sf::Color(0,0,0);
        damage = 0;
    }
};

class Beam
{
public:
    std::vector<BeamAttribute> attributes;
    sf::Vector2f startPos;
    sf::Vector2f endPos;
    double angle;
    int lifeTime;
    bool toDelete;
    unsigned long long id;
    sf::Color getColor()
    {
        sf::Color color(0,0,0);
        for(auto &atr : attributes)
        {
            color += atr.color;
        }
        return color;
    }
    Beam()
    {
        angle = 0;
        lifeTime = 0;
        toDelete = false;
        id = gvars::globalid++;
    }
};

std::list<Beam> beams;

void drawBeams()
{
    for(auto &beam : beams)
    {
        sf::Color color(0,0,0);
        int damage = 0;
        for(auto &atr : beam.attributes)
        {
            damage += atr.damage;
            color += atr.color;
        }

        shapes.createLine(beam.startPos.x,beam.startPos.y,beam.endPos.x,beam.endPos.y,3,color);
        int angle = beam.angle;
        shapes.createText(beam.startPos.x,beam.startPos.y,10,color,std::to_string(angle));
        shapes.createText(beam.endPos.x,beam.endPos.y,10,color,std::to_string(angle));


    }
}

std::vector<sf::Vector2i> traceTrail(int xa,int ya,int xb,int yb)
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

    std::vector<sf::Vector2i> returns;
    for (int k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        returns.push_back(sf::Vector2i(x,y));
    }
    return returns;
}

void beamTestLoop()
{
    //if(inputState.key[Key::Space].time == 1)
    {
        beams.clear();
        {
            Beam beam;
            beam.startPos = sf::Vector2f(25,200);
            //beam.endPos = sf::Vector2f(100,100);
            beam.endPos = gvars::mousePos;
        //beam.startPos = sf::Vector2f(100,100);
        //beam.endPos = sf::Vector2f(25,25);
        beam.angle = math::angleBetweenVectors(beam.startPos,beam.endPos);
        BeamAttribute BA;
        BA.damage = 5;
        BA.color = sf::Color(0,150,0);
        beam.attributes.push_back(BA);
        BA.damage = 3;
        BA.color = sf::Color(0,0,150);
        beam.attributes.push_back(BA);
        beams.push_back(beam);
        }
        {
            Beam beam;
        beam.startPos = sf::Vector2f(40,20);
        beam.endPos = sf::Vector2f(40,400);
        //beam.startPos = sf::Vector2f(100,40);
        //beam.endPos = sf::Vector2f(20,40);
        beam.angle = math::angleBetweenVectors(beam.startPos,beam.endPos);
        BeamAttribute BA;
        BA.damage = 5;
        BA.color = sf::Color(150,0,0);
        beam.attributes.push_back(BA);
        beams.push_back(beam);
        }
    }
    int Beams = 0;

    for( auto &beam : beams)
        {
            std::vector<sf::Vector2i> trail = traceTrail(beam.startPos.x,beam.startPos.y,beam.endPos.x,beam.endPos.y);
            /*
            if(Beams == 0)
            {

                std::cout << "trail length: " << trail.size() << std::endl;
                for(int i = 0; i != trail.size(); i++)
                {
                    std::cout << i << ": " << trail[i].x << "/" << trail[i].y << std::endl;
                }
                std::cout << "trail length: " << trail.size() << std::endl;
                fSleep(5);

            }
            */



            for( auto &otherbeam : beams)
            {
                if(beam.id != otherbeam.id)
                {
                    std::vector<sf::Vector2i> othertrail = traceTrail(otherbeam.startPos.x,otherbeam.startPos.y,otherbeam.endPos.x,otherbeam.endPos.y);


                    for(int i = 0; i != trail.size(); i++)
                        for(int t = 0; t != othertrail.size(); t++)
                    {
                        if(trail[i].x == othertrail[t].x && trail[i].y == othertrail[t].y)
                        {
                            sf::Color color = beam.getColor() + otherbeam.getColor();
                            shapes.createCircle(trail[i].x,trail[i].y,10,color);

                            //int angle = beam.angle + otherbeam.angle;
                            //int angle = math::constrainAngle( (beam.angle + otherbeam.angle) / 2);
                            int angle = math::constrainAngle( math::angleDiff(beam.angle,otherbeam.angle));
                            angle = math::constrainAngle(beam.angle + (angle/2) );

                            shapes.createText(trail[i].x,trail[i].y,10,color,std::to_string(angle));
                            sf::Vector2f xxPos(trail[i].x,trail[i].y);
                            sf::Vector2f vPos = math::angleCalc(xxPos,math::constrainAngle(angle),100);
                            shapes.createCircle(vPos.x,vPos.y,10,color);
                            std::string outPut = std::to_string(beam.id) + ": a: " + std::to_string(angle);
                            shapes.createText(vPos.x,vPos.y,10,color,outPut);
                            shapes.createLine(trail[i].x,trail[i].y,vPos.x,vPos.y,3,color);

                            if(inputState.key[Key::Z])
                            {
                                fSleep(1);
                            }

                        }
                    }
                }
            }
            Beams++;
        }

}

void newItemstuffs()
{
    return;
    std::cout << "Kaboom! \n";
    makeItems(itemlist,1);
    printItems(itemlist);
    std::cout << "itemlist maxdam: " << totalDamageofItems(itemlist) << std::endl;

    for( auto &item : itemlist)
    {
        std::cout << "====== \n";
        itemPtrVector IPV;
        IPV = makeItems(item.internalitems,3);
        for ( auto &internal : IPV.ptrs)
        {
            makeItems(internal->internalitems,3);
            std::cout << "X \n";
            printItems(internal->internalitems);
            std::cout << "X \n";
        }
        printItems(item.internalitems);
        std::cout << "====== \n";
    }
    std::cout << "Giga: " << totalDamageofItemsInternalized(itemlist) << std::endl;
}

void playThemeTrack()
{
    //gvars::soundVolume = 0;
    gvars::musicVolume = 50;

    playMusic("LimetoeLonelyCowboy.ogg");

    /*
    int ranNum = randz(1,2);
    if(ranNum == 1)
        playMusic("Jalandhar.ogg");
    else if(ranNum == 2)
        playMusic("Neo Western.ogg");
        */

}


void critDamages(float damage, critScore crit)
{
    std::cout << "\n===Crit Damages===\n";
    std::cout << "damage = " << damage << std::endl;
    std::cout << "Norm Crit Chance/Damage: " << crit.normalCritChance << " : " << crit.normalDamageMultiplier << std::endl;
    //std::cout <<
    std::cout << "Gamma Crit Chance/Damage: " << crit.gammaCritChance << " : " << crit.gammaDamageMultiplier << std::endl;
    std::cout << "Iota Crit Chance/Damage: " << crit.iotaCritChance << " : " << crit.iotaDamageMultiplier << std::endl;

    std::cout << "Mu Roll/Damage: " << randz(1,crit.muCritChance) << "/" << damage*crit.muDamageMultiplier << std::endl;
    std::cout << "================== \n";
    int dogEatDog = 0;
    for(int x = 0; x != 4; x++)
    {
        int TopDog = 0;
        for(int i = 0; i != 25; i++)
        {
            //int roll = randz(1,10000000);
            int roll = random(1,crit.iotaCritChance);
            if(roll > TopDog)
                TopDog = roll;
            std::cout << "iota Roll/Damage: " << roll << "/" << damage*crit.muDamageMultiplier << std::endl;
        }
        std::cout << "Highest Roll: " << TopDog << std::endl;
    }
    std::cout << "Ultimate High: " << dogEatDog << std::endl;



    std::cout << "random(5,500): " << random(5,500) << std::endl;
    std::cout << "random(5,500): " << random(5,500) << std::endl;
    std::cout << "random(5,500): " << random(5,500) << std::endl;
    std::cout << "random(5,500): " << random(5,500) << std::endl;


}

void Line( float x1, float y1, float x2, float y2, sf::Color color )
{
        // Bresenham's line algorithm
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if(steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if(x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    const int maxX = (int)x2;

    for(int x=(int)x1; x<maxX; x++)
    {
        if(steep)
        {
            //SetPixel(y,x, color);
            shapes.createCircle(y,x,1,color);
        }
        else
        {
            shapes.createCircle(x,y,1,color);
        }

        error -= dy;
        if(error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}


void detectLine( float x1, float y1, float x2, float y2)
{
        // Bresenham's line algorithm
        int lineAmount = 0;
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if(steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if(x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    const int maxX = (int)x2;

    for(int x=(int)x1; x<maxX; x++)
    {
        if(steep)
        {
            //SetPixel(y,x, color);
            if(tiles[abs_to_index(y/GRID_SIZE)][abs_to_index(x/GRID_SIZE)][gvars::currentz].walkable)
                shapes.createCircle(y,x,1,sf::Color(0,0,255));
            else
                shapes.createCircle(y,x,1,sf::Color(255,0,0));


            lineAmount++;



            //shapes.createCircle(y,x,1,color);
        }
        else
        {
            if(tiles[abs_to_index(x/GRID_SIZE)][abs_to_index(y/GRID_SIZE)][gvars::currentz].walkable)
                shapes.createCircle(x,y,1,sf::Color(0,0,255));
            else
                shapes.createCircle(x,y,1,sf::Color(255,0,0));

            lineAmount++;
        }

        error -= dy;
        if(error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
    std::cout << "cycles: " << lineAmount << std::endl;
}

void detectLineGrid( float x1, float y1, float x2, float y2)
{
        // Bresenham's line algorithm
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if(steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if(x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    const int maxX = (int)x2;

    for(int x=(int)x1; x<maxX; x++)
    {
        if(steep)
        {
            //SetPixel(y,x, color);
            int sqXStart = (x*GRID_SIZE-(GRID_SIZE/2))+(GRID_SIZE/2);
            int sqYStart = y*GRID_SIZE-(GRID_SIZE/2)+(GRID_SIZE/2);
            int sqXEnd = x*GRID_SIZE+(GRID_SIZE/2)+(GRID_SIZE/2);
            int sqYEnd = y*GRID_SIZE+(GRID_SIZE/2)+(GRID_SIZE/2);

            if(tiles[y][x][gvars::currentz].walkable)
                shapes.createSquare(sqYStart,sqXStart,sqYEnd,sqXEnd,sf::Color(0,0,255,100));
            else
                shapes.createSquare(sqYStart,sqXStart,sqYEnd,sqXEnd,sf::Color(255,0,0,100));



            //shapes.createCircle(y,x,1,color);
        }
        else
        {
            int sqXStart = x*GRID_SIZE-(GRID_SIZE/2)+(GRID_SIZE/2);
            int sqYStart = y*GRID_SIZE-(GRID_SIZE/2)+(GRID_SIZE/2);
            int sqXEnd = x*GRID_SIZE+(GRID_SIZE/2)+(GRID_SIZE/2);
            int sqYEnd = y*GRID_SIZE+(GRID_SIZE/2)+(GRID_SIZE/2);

            if(tiles[x][y][gvars::currentz].walkable)
                shapes.createSquare(sqXStart,sqYStart,sqXEnd,sqYEnd,sf::Color(0,0,255,100));
            else
                shapes.createSquare(sqXStart,sqYStart,sqXEnd,sqYEnd,sf::Color(255,0,0,100));
        }

        error -= dy;
        if(error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}






int weightChance()
{
    randomWeightList.clear();
    randomWeightList.addEntry("Pistol",50);
    randomWeightList.addEntry("Shotgun",25);
    randomWeightList.addEntry("Minigun",10);
    randomWeightList.addEntry("Harpoon Cannon",5);


    int ran = randomWeightList.getRandomSlot();
    return ran;
}

void pauseMenu()
{

    if(inputState.key[Key::R].time == 1)
    {
        //int itemChosen = weightChance();
        randomWeightList.clear();



        for(auto &item : itemmanager.globalItems)
        {
            if(item.type == 2)
                randomWeightList.addEntry(item.name, item.rarity);
        }



        //std::cout << "Random Item: " << randomWeightList.entries[itemChosen].name << std::endl;
        std::string randomItem = randomWeightList.getRandomName();
        std::cout << "Random Item: " << randomItem << std::endl;
        std::cout << "Chance: " << randomWeightList.getChance(randomItem) << std::endl;
    }

    if(inputState.key[Key::F1].time == 1)
    {
        std::cout << "Old Size: " << gvars::hudView.getSize().x << "/" << gvars::hudView.getSize().y << std::endl;
    }
    if(inputState.key[Key::F2].time == 1)
    {
        std::cout << "Old Size: " << gvars::view1.getSize().x << "/" << gvars::view1.getSize().y << std::endl;
    }


    if(inputState.key[Key::F4].time == 1)
        std::cout << "Resolution: " << window.getSize().x << "/" << window.getSize().y << std::endl;



    if(inputState.key[Key::P].time == 1)
    {
        bool gamePaused = true;

        window.setView(gvars::hudView);

        sf::Vector2f tSize(screen.x()/2-100,screen.y()/2);
        shapes.createText(tSize,20,sf::Color::White,"Game Paused, Press P to resume.",&gvars::hudView);
        shapes.drawShapes();

        textList.drawTextz();
        window.display();

        while(gamePaused)
        {

            sf::Event event;
            while (window.pollEvent(event))
            {
                std::cout << "event: " << event.type << std::endl;
                if (event.type == sf::Event::TextEntered)
                {
                    std::cout << "Key Pressed: " << event.text.unicode << std::endl;
                    if (event.text.unicode == 112)//20?)
                    {
                        gamePaused = false;
                    }
                }
            }
        window.setView(gvars::view1);
        }
    }
}



void onStart()
{
    gvars::hudView = window.getDefaultView();

    int towerDiff = 10;
    int towerFloors = 5;
    int currentFloor = 4;
    std::cout << "currentFloor/towerFloors/towerDiff: " << currentFloor << "/" << towerFloors << "/" << towerDiff << std::endl;
    std::cout << "Floor Difficulty: " << getFloorDifficulty(currentFloor,towerFloors,towerDiff) << std::endl;
}




int main()
{
    //srand(clock());
    srand(time(NULL));

    soundmanager.init();
    initializeMusic();
    playThemeTrack();


    texturemanager.init();
    animationmanager.init();
    shadermanager.setupShaders();

    itemmanager.initializeItems();
    npcmanager.initializeCritters();


    galaxySetup();
    bountyTowerSetup();

    window.create(sf::VideoMode(1280, 720, 32), randomWindowName());
    window.setVerticalSyncEnabled(true);

    textList.loadFont();

    soundmanager.playSound("Startup.wav");
    newItemstuffs();

    onStart();

    while (window.isOpen())
    {
        shadermanager.shaderLoop();

        if(inputState.lmbTime == 1 && inputState.key[Key::LShift])
            shadermanager.setShockwave(gvars::mousePos);


        if(inputState.rmb && inputState.key[Key::LShift])
        {

            sf::View * view;
            view = &gvars::view1;

            Shape shape;
            shape.shape = shape.Line;
            shape.duration = 30;
            shape.startPos = sf::Vector2f(500,500);
            shape.endPos = gvars::mousePos + sf::Vector2f(500,500);

            shape.size = 30;


            //shape.maincolor = sf::Color::Red;
            shape.maincolor = sf::Color::White;
            shape.texture = &texturemanager.getTexture("BTGrass.png");
            //shape.outline = 3;
            //shape.seccolor = sf::Color::Red;

            shape.drawView = &gvars::view1;

            shapes.shapes.push_back(shape);

            shapes.createLine(gvars::mousePos.x, gvars::mousePos.y, gvars::mousePos.x + 200, gvars::mousePos.y + 200, 5, sf::Color::Green);
        }

        pauseMenu();
        fpsKeeper.calcFPS();

        gvars::hovering = false;
        if (inputState.key[Key::RControl] && inputState.key[Key::R].time == 1 && !network::chatting)
            toggle(gvars::debug);


        frames();
        handleEvents();

        genericLoop();
        galaxyLoop();
        //beamTestLoop();

        handlePhase();

        acquireSelectedNPCs();
        selectedNPCprocess();


        debug("Pre Draw Stuffs");
        hoverItemIDdisplay();
        drawStuffs();

        lmbPress();

        window.display();
        window.clear();
        debug("Post Draw Stuffs");

        debug("Starting Removing process, NPC/Unpoint/Items/GC.Menu");

        removeNPCs(npclist, mutex::npcList);
        removeItems(worlditems);
        soundmanager.cleanSounds();
        cleanMenu();
    } // End of game loop

    //TcpServerThread.terminate();
    //TcpClientThread.terminate();

    return EXIT_SUCCESS;
}
