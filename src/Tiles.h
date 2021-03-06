#ifndef __TILES_H_INCLUDED__
#define __TILES_H_INCLUDED__

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>


#include "defs.h"
#include "Shaders.h"

const int GRIDS = 96;

const int CHUNK_SIZE = 32;

bool isInBounds(sf::Vector2f vPos);

class WorldTileType
{
public:
    enum
    {
        City,
        Spire,
        Grass,
        Road,
        River,
        Ocean
    };
};

class Liquid
{
public:
    std::string name;
    int amount;
    sf::Color color;
};

class Tile
{
public:
    unsigned int id;
    int deathID;
    bool transparent;
    float health;
    bool walkable;
    bool goesUp;
    bool goesDown;
    Vec3 pos;
    bool teleporter;
    Vec3 telePos;
    sf::Sprite img;
    sf::Color worldColor;
    std::string state;
    float workProgress;
    int workGoal;

    std::vector<Liquid> liquids;

    void setPos(int x, int y, int z);
    Vec3 getPos();

    void dirt();
    void rendGrass();
    void grass();
    void stone();
    void stoneWall();
    void wall();
    void woodFloor();
    void stairsUp();
    void stairsDown();
    void road(bool center = false);
    void door();
    void lava();
    void baseRune();
    void sky();
    void cake();
    void BTstone();

    void BTwater();
    void BTgrass();
    void BTwall();
    void BTwallFake();
    void BTdoor();
    void BTlava();
    void BTsky();

    void BTstairs();
    void BTelevatordoor();
    void BTelevator();
    void BTbossrug();
    void BTswitch();
    void setTilebyID(int ID);
    void teleportPad(Vec3 teleportPosition);
    void BTgoodietile();
    void BTairdroptile();
    Tile();
};

class XTile
{
public:
    unsigned short id;
    unsigned short deathID;
    float health;
    sf::Texture *img;
    sf::Color worldColor;

    XTile();
    void initImage();
};

class Chunk
{
public:
    Tile zTiles[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
};

class Planet
{
public:
    std::vector<Chunk> chunks;
};

class WorldTile
{
public:
    sf::Sprite img;
    int id;
    sf::Vector2f galPos;
    sf::Vector2f regPos;
    sf::Vector2f locPos;
    int infected;
    int tiles;
    std::string type;

    std::string owner;

    class Building
    {
    public:
        sf::Vector2f pos;
        int size;
    };

    std::vector<Building> builds;

    WorldTile();
};

//extern Tile tiles[GRIDS][GRIDS][CHUNK_SIZE];
extern std::vector<std::vector<std::vector<Tile>>> tiles;

extern WorldTile worldMap[100][100];

extern Tile vChunk[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]; // 10223616
extern Tile tempTile[GRIDS][GRIDS][CHUNK_SIZE];
extern XTile xChunk[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

void zGenerateChunk(std::string type, int planet, sf::Vector2i cords,
                    sf::Vector2i pos = sf::Vector2i(500, 500));
void generateChunk(std::string type, int planet, sf::Vector2i cords,
                   sf::Vector2i pos = sf::Vector2i(500, 500));
void generateWorld(int regionSize = 100, int planetNum = 500);
void saveMap(int planet, int xcord, int ycord, int xpos, int ypos);
void loadMap(int planet, int xcord, int ycord, int xpos, int ypos);
void initalizeWorldTiles();
void drawWorldTiles();
void initializeTiles();
void initializeTilePositions();
void drawTile(int xpos, int ypos, sf::Texture &image);
void drawNewTiles();
void tilesRandom();
void tilesGoUp();
void tilesGoDown();
void tilesGoLeft();
void tilesGoRight();
void buildLocalfromWorld(sf::Vector2i worldPos);

void networkGridUpdate(sf::Packet pack);

void processLiquid();
void resizeGrid(int x, int y, int z);
sf::Vector2f gridEject(sf::Vector2f position);
std::string tileFace(int xpos, int ypos, int zpos, int size, std::vector<std::vector<std::vector<Tile>>> &Tiles);
bool gridTrace(sf::Vector2f Ori, sf::Vector2f Tar);


#endif // TILES_H_INCLUDED
