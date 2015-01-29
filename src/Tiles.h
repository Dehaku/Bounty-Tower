#ifndef __TILES_H_INCLUDED__
#define __TILES_H_INCLUDED__

#include <SFML/Graphics.hpp>
#include "Images.h"
#include "Globals.h"

const int Grids = 96;

const int ChunkSize = 32;

class Tile
{
public:
    unsigned int ID;
    int DeathID;
    float Health;
    bool Walkable;
    sf::Sprite Img;
    sf::Texture Texture;
    sf::Color WorldColor;

    void Dirt()
    { // 1003
        ID = 1003;
        WorldColor = sf::Color(100, 100, 0);
        Img.setTexture(*imagemanager.GetImage("Dirt.png"));
    }

    void RendGrass()
    { // 1001
        ID = 1001;
        WorldColor = sf::Color(0, 150, 0);
        sf::Image Grazz;
        Grazz.create(20, 20, sf::Color(0, 0, 0, 255));
        for (int i = 0; i != 20; i++)
        {
            for (int t = 0; t != 20; t++)
            {
                int Pix = randz(0, 3);
                if (Pix <= 2)
                    Grazz.setPixel(i, t, sf::Color(0, 128, 0, 255));
                if (Pix == 3)
                    Grazz.setPixel(i, t, sf::Color(0, 255, 0, 255));
                //Grazz.setPixel(i,t,sf::Color(0,128,0,255));
            }
        }
        //sf::Texture Why;
        //Texture.loadFromImage(Grazz);
        //Img.setTexture(Texture);
        //Img.setTexture(Why);
        //Img.setTexture( *imagemanager.GetImage("Grass.png"));
    }

    void Grass()
    { // 1001
        ID = 1001;
        WorldColor = sf::Color(0, 150, 0);
        Img.setTexture(*imagemanager.GetImage("Grass.png"));
        //RendGrass();
    }

    void Stone()
    { // 1007
        ID = 1007;
        WorldColor = sf::Color(150, 150, 150);
        Walkable = true;
        Img.setTexture(*imagemanager.GetImage("Underground.png"));
    }

    void StoneWall()
    { // 1008
        ID = 1008;
        WorldColor = sf::Color(150, 150, 150);
        Walkable = false;
        Img.setTexture(*imagemanager.GetImage("StoneWall.png"));
    }

    void Wall()
    { // 1010
        ID = 1010;
        WorldColor = sf::Color(100, 100, 100);
        Img.setTexture(*imagemanager.GetImage("Wall.png"));
        Health = 200;
        DeathID = 1007;
        Walkable = false;
    }

    void WoodFloor()
    { // 1030
        ID = 1030;
        WorldColor = sf::Color(150, 150, 0);
        Img.setTexture(*imagemanager.GetImage("WoodFloor.png"));
    }

    void Road(bool Center = false)
    { // 1050
        ID = 1050;
        WorldColor = sf::Color(0, 0, 0);
        if (Center)
            Img.setTexture(*imagemanager.GetImage("MiddleRoad.png"));
        if (!Center)
            Img.setTexture(*imagemanager.GetImage("Black.png"));
    }

    void Door()
    { // 1100
        ID = 1100;
        WorldColor = sf::Color(255, 0, 0);
        Img.setTexture(*imagemanager.GetImage("Door.png"));
        Walkable = true;
        Health = 15;
        DeathID = 7;
    }

    void Lava()
    { // 1337
        ID = 1337;
        WorldColor = sf::Color(255, 100, 0);
        Img.setTexture(*imagemanager.GetImage("Lava.png"));
    }

    void Sky()
    { // 1700
        ID = 1700;
        WorldColor = sf::Color(150, 150, 150);
        Img.setTexture(*imagemanager.GetImage("Sky.png"));
    }

    void Cake()
    {
        ID = 1100;
        WorldColor = sf::Color(255, 0, 0);
        Img.setTexture(*imagemanager.GetImage("Door.png"));
        Health = 15;
        DeathID = 7;
    }

    Tile()
    {
        WorldColor = sf::Color(0, 0, 0, 255);
        Health = 10;
        DeathID = ID;
        Walkable = true;
    }
};

class xTile
{
public:
    unsigned short ID;
    unsigned short DeathID;
    float Health;
    sf::Texture *Img;
    sf::Color WorldColor;

    xTile()
    {
        WorldColor = sf::Color(0, 0, 0, 255);
        Health = 10;
        DeathID = ID;
    }
    void initImage()
    {
        Img = imagemanager.GetImage("City.png");
    }
};

class Chunk
{
public:
    Tile zTiles[ChunkSize][ChunkSize][ChunkSize];
    Chunk()
    {
    }
};

class Planet
{
public:
    std::vector<Chunk> Chunks;
    Planet()
    {
    }
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

    WorldTile()
    {
        Infected = 0;
        Tiles = 32;
    }
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
