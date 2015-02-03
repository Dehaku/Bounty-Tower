#ifndef __TILES_H_INCLUDED__
#define __TILES_H_INCLUDED__

#include <SFML/Graphics.hpp>

const int GRIDS = 96;

const int CHUNK_SIZE = 32;

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

class Tile
{
public:
    unsigned int id;
    int deathID;
    float health;
    bool walkable;
    sf::Sprite img;
    sf::Color worldColor;

    void dirt();
    void rendGrass();
    void grass();
    void stone();
    void stoneWall();
    void wall();
    void woodFloor();
    void road(bool center = false);
    void door();
    void lava();
    void sky();
    void cake();
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

extern Tile tiles[GRIDS][GRIDS][GRIDS];

extern WorldTile worldMap[100][100];

extern Tile vChunk[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]; // 10223616
extern Tile tempTile[GRIDS][GRIDS][GRIDS];
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
void drawTile(int xpos, int ypos, sf::Texture &image);
void drawNewTiles();
void tilesRandom();
void tilesGoUp();
void tilesGoDown();
void tilesGoLeft();
void tilesGoRight();
void buildLocalfromWorld(sf::Vector2i worldPos);

#endif // TILES_H_INCLUDED
