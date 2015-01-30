#ifndef __TILES_H_INCLUDED__
#define __TILES_H_INCLUDED__

#include <SFML/Graphics.hpp>

const int Grids = 96;

const int ChunkSize = 32;

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
    unsigned int ID;
    int DeathID;
    float Health;
    bool Walkable;
    sf::Sprite Img;
    sf::Color WorldColor;

    void Dirt();
    void RendGrass();
    void Grass();
    void Stone();
    void StoneWall();
    void Wall();
    void WoodFloor();
    void Road(bool Center = false);
    void Door();
    void Lava();
    void Sky();
    void Cake();
    Tile();
};

class xTile
{
public:
    unsigned short ID;
    unsigned short DeathID;
    float Health;
    sf::Texture *Img;
    sf::Color WorldColor;

    xTile();
    void initImage();
};

class Chunk
{
public:
    Tile zTiles[ChunkSize][ChunkSize][ChunkSize];
};

class Planet
{
public:
    std::vector<Chunk> Chunks;
};

class WorldTile
{
public:
    sf::Sprite Img;
    int ID;
    sf::Vector2f GalPos;
    sf::Vector2f RegPos;
    sf::Vector2f LocPos;
    int Infected;
    int Tiles;
    std::string Type;

    std::string Owner;

    class Building
    {
    public:
        sf::Vector2f Pos;
        int Size;
    };

    std::vector<Building> Builds;

    WorldTile();
};

extern Tile Tiles[Grids][Grids][Grids];

extern WorldTile WorldMap[100][100];

extern Tile vChunk[ChunkSize][ChunkSize][ChunkSize]; // 10223616
extern Tile TempTile[Grids][Grids][Grids];
extern xTile xChunk[ChunkSize][ChunkSize][ChunkSize];

void zGenerateChunk(std::string type, int planet, sf::Vector2i Cords,
                    sf::Vector2i Pos = sf::Vector2i(500, 500));
void GenerateChunk(std::string type, int planet, sf::Vector2i Cords,
                   sf::Vector2i Pos = sf::Vector2i(500, 500));
void GenerateWorld(int RegionSize = 100, int PlanetNum = 500);
void SaveMap(int planet, int xcord, int ycord, int xpos, int ypos);
void LoadMap(int planet, int xcord, int ycord, int xpos, int ypos);
void InitalizeWorldTiles();
void DrawWorldTiles();
void InitializeTiles();
void DrawTile(int xpos, int ypos, sf::Texture &Image);
void DrawNewTiles();
void TilesRandom();
void TilesGoUp();
void TilesGoDown();
void TilesGoLeft();
void TilesGoRight();
void BuildLocalfromWorld(sf::Vector2i WorldPos);

#endif // TILES_H_INCLUDED
