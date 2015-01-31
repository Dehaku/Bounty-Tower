#include "Tiles.h"
#include "Effects.h"
#include "Faction.h"
#include "filesystemUtils.hpp"
#include "Textures.h"
#include "math.h"
#include "util.h"
#include "globalvars.h"

#include <sstream>
#include <fstream>
#include <iostream>

Tile Tiles[Grids][Grids][Grids];
WorldTile WorldMap[100][100];
Tile vChunk[ChunkSize][ChunkSize][ChunkSize]; // 10223616
Tile TempTile[Grids][Grids][Grids];
xTile xChunk[ChunkSize][ChunkSize][ChunkSize];

extern sf::RenderWindow App;

void zGenerateChunk(std::string type, int planet, sf::Vector2i Cords,
                    sf::Vector2i /*Pos*/)
{

    Con("Queso?");

    std::string Deb = "Creating Chunk: ";
    Deb.append(type);
    debug(Deb);

    if (type == "Building")
    {

        for (int i = 0; i != ChunkSize; i++)
        {
            for (int t = 0; t != ChunkSize; t++)
            {
                for (int x = 0; x != ChunkSize; x++)
                {
                    if (x < 30)
                        vChunk[i][t][x].Stone();
                    if (x > 30)
                        vChunk[i][t][x].Sky();
                }
            }
        }

        for (int i = 0; i != ChunkSize; i++)
        {
            for (int t = 0; t != ChunkSize; t++)
            {
                vChunk[i][t][0].Stone();
            }
        }
        for (int i = 10; i != 21; i++)
        {
            for (int t = 10; t != 21; t++)
            {
                vChunk[i][t][0].Wall();
            }
        }
        for (int i = 11; i != 20; i++)
        {
            for (int t = 11; t != 20; t++)
            {
                vChunk[i][t][0].Stone();
            }
        }
        vChunk[15][20][0].Door();
        for (int i = 0; i != ChunkSize; i++)
        {
            for (int t = 0; t != ChunkSize; t++)
            {
                for (int x = 0; x != ChunkSize; x++)
                {
                    vChunk[i][t][x].Stone();
                }
            }
        }
    }

    using namespace std; // Start Saving Map
    debug("Saving current map to file...");
    string line("data/maps/map");
    stringstream convert;
    convert << planet;
    convert << Cords.x;
    convert << Cords.y;
    line.append(convert.str());
    string ending(".map");
    line.append(ending);
    cout << line << endl;
    ofstream outputFile(line.c_str());
    //outputFile << biometype << endl;
    debug("Writting map to file...");
    for (int i = 0; i != ChunkSize; i++)
    {
        for (int t = 0; t != ChunkSize; t++)
        {
            for (int x = 0; x != ChunkSize; x++)
            {
                outputFile << vChunk[i][t][0].ID << " ";
            }
            outputFile << endl;
        }
        outputFile << endl;
    }
}

void GenerateChunk(std::string type, int planet, sf::Vector2i Cords,
                   sf::Vector2i /*Pos*/)
{
    //std::string Deb = "Creating Chunk: ";
    //Deb.append(type);
    //debug(Deb);

    if (type == "Building")
    {
        for (int i = 0; i != ChunkSize; i++)
        {
            for (int t = 0; t != ChunkSize; t++)
            {
                for (int x = 0; x != ChunkSize; x++)
                {
                    if (x < 30)
                        vChunk[i][t][x].Stone();
                    if (x == 30)
                        vChunk[i][t][x].Dirt();
                    if (x > 30)
                        vChunk[i][t][x].Sky();
                    if (i >= 10 && i < 21 && x == 30)
                    {
                        if (t >= 10 && t < 21)
                        {
                            vChunk[i][t][x].Wall();
                        }

                        if (i >= 11 && i < 20)
                        {
                            if (t >= 11 && t < 20)
                            {
                                vChunk[i][t][x].Stone();
                            }
                        }
                    }
                }
            }
        }

        /*for(int i = 10; i != 21; i++)
        {
            for(int t = 10; t != 21; t++)
            {
                vChunk[i][t][0].Wall();
            }
        }
        for(int i = 11; i != 20; i++)
        {
            for(int t = 11; t != 20; t++)
            {
                vChunk[i][t][0].Stone();
            }
        }*/
        vChunk[15][20][30].Door();
    }

    else if (type == "SouthernHouse")
    {

        for (int i = 0; i != ChunkSize; i++)
        {
            for (int t = 0; t != ChunkSize; t++)
            {
                for (int x = 0; x != ChunkSize; x++)
                {
                    if (x < 30)
                        vChunk[i][t][x].Stone();
                    if (x > 30)
                        vChunk[i][t][x].Sky();
                }
            }
        }

        for (int i = 0; i != ChunkSize; i++)
        {
            for (int t = 0; t != ChunkSize; t++)
            {
                vChunk[i][t][0].Grass();
            }
        }
        for (int i = 10; i != 21; i++)
        {
            for (int t = 5; t != 21; t++)
            {
                vChunk[i][t][0].Wall();
            }
        }
        for (int i = 11; i != 20; i++)
        {
            for (int t = 11; t != 20; t++)
            {
                vChunk[i][t][0].WoodFloor();
            }
        }
        for (int i = 11; i != 20; i++)
        {
            for (int t = 6; t != 10; t++)
            {
                vChunk[i][t][0].WoodFloor();
            }
        }
        vChunk[15][20][0].Door();
        vChunk[11][10][0].Door();
    }

    else if (type == "Road")
    {

        for (int i = 0; i != ChunkSize; i++)
        {
            for (int t = 0; t != ChunkSize; t++)
            {
                for (int x = 0; x != ChunkSize; x++)
                {
                    if (x < 30)
                        vChunk[i][t][x].Stone();
                    if (x > 30)
                        vChunk[i][t][x].Sky();
                }
            }
        }

        for (int i = 0; i != ChunkSize; i++)
        {
            for (int t = 0; t != ChunkSize; t++)
            {
                vChunk[i][t][30].Grass();
            }
        }
        for (int i = 11; i != 23; i++)
        {
            for (int t = 0; t != ChunkSize; t++)
            {
                vChunk[i][t][30].Road();
            }
        }
        for (int i = 16; i != 18; i++)
        {
            for (int t = 0; t != ChunkSize; t++)
            {
                vChunk[i][t][30].Road(true);
            }
        }
    }

    else if (type == "Spire")
    {

        for (int i = 0; i != ChunkSize; i++)
        {
            for (int t = 0; t != ChunkSize; t++)
            {
                for (int x = 0; x != ChunkSize; x++)
                {
                    if (x < 30)
                        vChunk[i][t][x].Stone();
                    if (x > 30)
                        vChunk[i][t][x].Sky();
                }
            }
        }

        for (int i = 0; i != ChunkSize; i++)
        {
            for (int t = 0; t != ChunkSize; t++)
            {
                vChunk[i][t][30].Grass();
                if (randz(1, 4) == 1)
                    vChunk[i][t][30].Stone();
            }
        }
        for (int i = 4; i != 28; i++)
        {
            for (int t = 1; t != 28; t++)
            {
                //vChunk[i][t][30].Road();
                if (randz(1, 100) == 1)
                {
                    vChunk[i - 1][t - 1][30].Stone();
                    vChunk[i - 1][t + 1][30].Stone();
                    vChunk[i + 1][t - 1][30].Stone();
                    vChunk[i + 1][t + 1][30].Stone();
                    vChunk[i][t + 1][30].Stone();
                    vChunk[i][t - 1][30].Stone();
                    vChunk[i + 1][t][30].Stone();
                    vChunk[i - 1][t][30].Stone();
                    vChunk[i][t][30].StoneWall();
                }
                else if (randz(1, 10) == 1)
                {
                    vChunk[i][t][30].Stone();
                }
                else if (randz(1, 20) == 300)
                {
                    //int xsize = randz(i,i-Math::Clamp(32,0,Math::i) );
                    //int ysize = randz(5,29);
                }
                if (randz(1, 10000) == 1)
                {
                    vChunk[i - 1][t - 1][30].Stone();
                    vChunk[i - 1][t + 1][30].Stone();
                    vChunk[i + 1][t - 1][30].Stone();
                    vChunk[i + 1][t + 1][30].Stone();
                    vChunk[i][t + 1][30].Stone();
                    vChunk[i][t - 1][30].Stone();
                    vChunk[i + 1][t][30].Stone();
                    vChunk[i - 1][t][30].Stone();
                    vChunk[i][t][30].Lava();
                }
            }
        }
    }

    else if (type == "Plains")
    {

        for (int i = 0; i != ChunkSize; i++)
        {
            for (int t = 0; t != ChunkSize; t++)
            {
                for (int x = 0; x != ChunkSize; x++)
                {
                    if (x < 30)
                        vChunk[i][t][x].Stone();
                    if (x == 30)
                        vChunk[i][t][x].Grass();
                    if (x > 30)
                        vChunk[i][t][x].Sky();
                }
            }
        }
    }

    else
    {
        std::cout << type << " is not a known Chunk, You best not enter the "
                             "area it exists. Or lack there of." << std::endl;
    }

    using namespace std; // Start Saving Map
    //debug("Saving current map to file...");
    string line("data/maps/Planet");
    stringstream convert;
    convert << planet;
    line.append(convert.str());
    convert.clear();
    galaxy_mkdir(line);
    line.append("/map");
    convert << "x";
    convert << Cords.x;
    convert << "y";
    convert << Cords.y;
    line.append(convert.str());
    string ending(".map");
    line.append(ending);
    //cout << line << endl;
    // 162,176:1057
    // 5.38,16.0:65
    if (true == false)
    {
        sf::Image image;
        image.create(ChunkSize * 100, ChunkSize * 100, sf::Color(0, 0, 0, 255));
        int XX = 0;
        int YY = 0;
        Con("Post Image");

        for (int i = Cords.x * ChunkSize;
             i != (Cords.x * ChunkSize) + ChunkSize; i++)
        {

            for (int t = Cords.y * ChunkSize;
                 t != (Cords.y * ChunkSize) + ChunkSize; t++)
            {
                image.setPixel(
                    i, t,
                    vChunk[abs_to_index(XX)][abs_to_index(YY)][0].WorldColor);
                YY++;
            }
            YY = 0;
            XX++;
        }

        Con("Saving Image");
        /*
    bool Doing = true;
    sf::Texture Imgr;
    Imgr.loadFromImage(image);
    sf::Sprite sImgr;
    sImgr.setTexture(Imgr);

    while(Doing == true)
    {
        Effectz.CreateSquare(0,0,1000,1000,sf::Color::Black);
        Effectz.DrawEffects();
        sImgr.setPosition(-800,-700);
        App.draw(sImgr);
        App.display();
    }
    */
        image.saveToFile(".//data//maps//map5005050.png");
    }

    ofstream outputFile(line.c_str());
    //outputFile << biometype << endl;
    //debug("Writting map to file...");
    for (int x = 0; x != ChunkSize; x++)
    {
        auto ID = vChunk[0][0][x].ID;
        bool Same = true;
        for (int i = 0; i != ChunkSize; i++)
        {
            for (int t = 0; t != ChunkSize; t++)
            {
                if (vChunk[i][t][x].ID != ID)
                {
                    Same = false;
                    //Con("Same = false;");
                }
            }
        }

        if (Same)
        {
            outputFile << "[";
            outputFile << x;
            outputFile << ",";
            outputFile << ID;
            outputFile << "]" << endl;
        }
        else
        {
            for (int i = 0; i != ChunkSize; i++)
            {
                for (int t = 0; t != ChunkSize; t++)
                {
                    outputFile << vChunk[i][t][x].ID << " ";
                }
                outputFile << endl;
            }
            outputFile << endl;
        }
    }
}

void GenerateWorld(int RegionSize, int PlanetNum)
{

    for (int x = 0; x != RegionSize; x++)
    {
        for (int y = 0; y != RegionSize; y++)
        {
            int RanFor = randz(0, 10);

            if (RanFor == 10)
                GenerateChunk("Building", PlanetNum, sf::Vector2i(x, y));
            else if (RanFor == 9)
                GenerateChunk("SouthernHouse", PlanetNum, sf::Vector2i(x, y));
            else if (RanFor == 8)
                GenerateChunk("Road", PlanetNum, sf::Vector2i(x, y));
            else if (RanFor <= 7)
                GenerateChunk("Building", PlanetNum, sf::Vector2i(x, y));

            //GenerateChunk("Building",PlanetNum,x,y);
        }
    }

    //GenerateChunk("Building",500,sf::Vector2i(abs(GC.MenuPos.x/20),abs(GC.MenuPos.y/20)));
}

void SaveMap(int planet, int /*xcord*/, int /*ycord*/, int xpos, int ypos)
{
    using namespace std; // Start to Save Map
    cout << "Saving current map to file...\n";
    string line("saves/maps/map");
    stringstream convert;
    convert << planet;
    convert << xpos;
    convert << ypos;
    line.append(convert.str());
    string ending(".txt");
    line.append(ending);
    cout << line << endl;
    ofstream outputFile(line.c_str());
    //outputFile << biometype << endl;
    cout << "Writting map to file...\n";
    for (int i = 0; i != Grids; i++)
    {
        for (int t = 0; t != Grids; t++)
        {
            outputFile << Tiles[i][t][30].ID << endl;
        }
    }
}

void LoadMap(int planet, int xcord, int ycord, int /*xpos*/, int /*ypos*/)
{
    using namespace std;
    fstream File; // Start to Load Map
    string newline("saves/maps/map");
    stringstream newconvert;
    newconvert << planet;
    newconvert << xcord;
    newconvert << ycord;
    newline.append(newconvert.str());
    string newending(".txt");
    newline.append(newending);

    File.open(newline.c_str(), fstream::in);
    debug("looking for file...");

    if (File.is_open())
    {
        cout << "found file: " << newline << endl;
        cout << "Read string to file...\n";
        //File >> biometype;
        for (int i = 0; i != Grids; i++)
        {
            for (int t = 0; t != Grids; t++)
            {
                File >> Tiles[i][t][30].ID;
            }
        }
        File.close();
    } // End to Load Map
}

void InitalizeWorldTiles()
{
    sf::Texture Txtr;
    sf::Sprite Sprt;
    std::cout << "Texture SizeOf: " << sizeof(Txtr)
              << ", Sprite: " << sizeof(Sprt) << std::endl;
    int Cities = 0;
    int Plains = 0;
    int Spires = 0;

    for (int i = 0; i != 100; i++)
    { // Generating the tiles.
        for (int t = 0; t != 100; t++)
        {
            WorldMap[i][t].ID = randz(0, 10);
            if (WorldMap[i][t].ID == WorldTileType::City)
            {
                WorldMap[i][t].ID = WorldTileType::City;
                WorldMap[i][t].Img.setTexture(
                    texturemanager.getTexture("City.png"));
                if (gvars::InitalZeds)
                    WorldMap[i][t].Infected = randz(0, 20);
                else
                    WorldMap[i][t].Infected = 0;
                WorldMap[i][t].Type = "Building";
                Cities++;
            }
            else if (WorldMap[i][t].ID == WorldTileType::Spire)
            {
                WorldMap[i][t].ID = WorldTileType::Spire;
                WorldMap[i][t].Img.setTexture(
                    texturemanager.getTexture("Spire.png"));
                WorldMap[i][t].Infected = 0;
                WorldMap[i][t].Type = "Spire";
                Spires++;
            }
            else
            {
                WorldMap[i][t].ID = WorldTileType::Grass;
                WorldMap[i][t].Img.setTexture(
                    texturemanager.getTexture("Grass.png"));
                WorldMap[i][t].Infected = 0;
                WorldMap[i][t].Type = "Plains";
                Plains++;
            }
        }
    }
    std::cout << "Cities/Spires/Plains: " << Cities << "/" << Spires << "/"
              << Plains << std::endl;
}

void DrawWorldTiles()
{
    debug("Drawing World Tiles");
    for (int i = 0; i != 100; i++)
    {
        for (int t = 0; t != 100; t++)
        {

            if (AABB(i * 20, t * 20, gvars::TopLeft.x - 20, gvars::TopRight.x,
                     gvars::TopLeft.y - 20, gvars::BottomRight.y))
            {

                WorldMap[i][t].Img.setPosition(i * 20, t * 20);

                App.draw(WorldMap[i][t].Img);
                if (WorldMap[i][t].Infected > 0)
                {
                    Effectz.CreateSquare(
                        (i * 20) + 1, (t * 20) + 1, ((i * 20) + 20) - 1,
                        ((t * 20) + 20) - 1, sf::Color(0, 0, 0, 0), 1,
                        sf::Color(
                            255, 0, 0,
                            math::Clamp(10 * WorldMap[i][t].Infected, 0, 255)));
                }
                if (WorldMap[i][t].Owner != "")
                {
                    for (size_t z = 0; z != UniFact.size(); z++)
                    {
                        if (WorldMap[i][t].Owner == UniFact[z].Name)
                        {
                            sf::Color Prim = UniFact[z].PrimaryColor;
                            Prim.a = Prim.a / 1.3;
                            sf::Color Seco = UniFact[z].SecondaryColor;
                            Seco.a = Seco.a / 1.3;
                            Effectz.CreateSquare(
                                (i * 20) + 1, (t * 20) + 1, ((i * 20) + 20) - 1,
                                ((t * 20) + 20) - 1, Prim, 10, Seco);
                        }
                    }
                }
            }
        }
    }
    debug("Done Drawing");
}

void InitializeTiles()
{ // TODO: Make a Tile Manager to hold the presets of these, Then do Tiles[i][t] = TileManager.at(ect)
    for (int i = 0; i != Grids; i++)
    {
        for (int t = 0; t != Grids; t++)
        {
            Tiles[i][t][30].Sky();
        }
    }
    for (int i = 0; i != Grids; i++)
    {
        Tiles[i][20][30].Dirt();
    }
    for (int i = 0; i != Grids; i++)
    {
        for (int t = 21; t != Grids; t++)
        {
            Tiles[i][t][30].Stone();
        }
    }
}

void DrawTile(int xpos, int ypos, sf::Texture &Image)
{
    Con("1stBacon");
    sf::Sprite Imgr;
    Con("2ndBacon");
    Imgr.setTexture(Image);
    Con("Bacon");
    Imgr.setPosition(xpos, ypos);
    Con("Bacon");
    App.draw(Imgr);
}

void DrawNewTiles()
{
    for (int i = 0; i != Grids; i++)
    {
        for (int t = 0; t != Grids; t++)
        {
            if (AABB(i * 20, t * 20, gvars::TopLeft.x - 20, gvars::TopRight.x,
                     gvars::TopLeft.y - 20, gvars::BottomRight.y))
            {
                Tiles[i][t][30].Img.setPosition(i * 20, t * 20);
                App.draw(Tiles[i][t][30].Img);
            }
        }
    }
    debug("Done Drawing New Tiles");
}

void TilesRandom()
{
    for (int i = 0; i != Grids; i++)
    {
        for (int t = 0; t != Grids; t++)
        {
            int Ran = randz(0, 2);
            if (Ran == 0)
            {
                Tiles[i][t][30].ID = 10;
                //                Tiles[i][t][30].Img.setTexture( *imagemanager.GetImage("Underground.png"));
            }
            if (Ran == 1)
            {
                Tiles[i][t][30].ID = 20;
                //                Tiles[i][t][30].Img.setTexture( *imagemanager.GetImage("Ground.bmp"));
            }
            if (Ran == 2)
            {
                Tiles[i][t][30].ID = 30;
                //                Tiles[i][t][30].Img.setTexture( *imagemanager.GetImage("Sky.bmp"));
            }
        }
    }
}

void TilesGoUp()
{
    std::cout << "Going Up \n";
    for (int i = 0; i != Grids; i++)
    {
        for (int t = 0; t != Grids; t++)
        {
            for (int x = 0; x != Grids; x++)
            {
                TempTile[i][t][x] = Tiles[i][t][x];
            }
        }
    }
    //[ ][ ][ ]
    //[^][^][^]
    //[^][^][^]

    for (int i = 0; i != Grids; i++)
    {
        for (int t = 32; t != Grids; t++)
        {
            Tiles[i][t - 32][30] = TempTile[i][t][30];
        }
    }

    debug("Starting");
    GenerateChunk(
        WorldMap[gvars::currentregionx - 1][gvars::currentregiony + 2].Type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx - 1, gvars::currentregiony + 2));
    for (int x = 0; x != ChunkSize; x++)
    {
        for (int y = 0 + 32 + 32; y != ChunkSize + 32 + 32; y++)
        {
            Tiles[x][y][30] = vChunk[x][y - 32 - 32][30];
        }
    }
    debug("Starting 2");
    GenerateChunk(
        WorldMap[gvars::currentregionx][gvars::currentregiony + 2].Type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx, gvars::currentregiony + 2));
    for (int x = 0 + 32; x != ChunkSize + 32; x++)
    {
        for (int y = 0 + 32 + 32; y != ChunkSize + 32 + 32; y++)
        {
            //std::cout << "X:" << x << "Y:" << y << ":" << vChunk[x][y][30].ID;

            //if(y == ChunkSize+32-1) std::cout << std::endl << "=============================" << std::endl;
            Tiles[x][y][30] = vChunk[x - 32][y - 32 - 32][30];
        }
    }
    debug("Starting 3");
    GenerateChunk(
        WorldMap[gvars::currentregionx + 1][gvars::currentregiony + 2].Type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx + 1, gvars::currentregiony + 2));
    for (int x = 0 + 32 + 32; x != ChunkSize + 32 + 32; x++)
    {
        for (int y = 0 + 32 + 32; y != ChunkSize + 32 + 32; y++)
        {
            Tiles[x][y][30] = vChunk[x - 32 - 32][y - 32 - 32][30];
        }
    }

    debug("Done");
    gvars::currentregiony++;
}

void TilesGoDown()
{
    std::cout << "Going Down \n";
    for (int i = 0; i != Grids; i++)
    {
        for (int t = 0; t != Grids; t++)
        {
            for (int x = 0; x != Grids; x++)
            {
                TempTile[i][t][x] = Tiles[i][t][x];
            }
        }
    }
    //[v][v][v]
    //[v][v][v]
    //[ ][ ][ ]

    for (int i = 0; i != Grids; i++)
    {
        for (int t = 0; t != Grids - 32; t++)
        {
            Tiles[i][t + 32][30] = TempTile[i][t][30];
        }
    }

    debug("Starting");
    GenerateChunk(
        WorldMap[gvars::currentregionx - 1][gvars::currentregiony - 2].Type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx - 1, gvars::currentregiony - 2));
    for (int x = 0; x != ChunkSize; x++)
    {
        for (int y = 0; y != ChunkSize; y++)
        {
            Tiles[x][y][30] = vChunk[x][y][30];
        }
    }
    debug("Starting 2");
    GenerateChunk(
        WorldMap[gvars::currentregionx][gvars::currentregiony - 2].Type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx, gvars::currentregiony - 2));
    for (int x = 0 + 32; x != ChunkSize + 32; x++)
    {
        for (int y = 0; y != ChunkSize; y++)
        {
            //std::cout << "X:" << x << "Y:" << y << ":" << vChunk[x][y][30].ID;

            //if(y == ChunkSize+32-1) std::cout << std::endl << "=============================" << std::endl;
            Tiles[x][y][30] = vChunk[x - 32][y][30];
        }
    }
    debug("Starting 3");
    GenerateChunk(
        WorldMap[gvars::currentregionx + 1][gvars::currentregiony - 2].Type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx + 1, gvars::currentregiony - 2));
    for (int x = 0 + 32 + 32; x != ChunkSize + 32 + 32; x++)
    {
        for (int y = 0; y != ChunkSize; y++)
        {
            Tiles[x][y][30] = vChunk[x - 32 - 32][y][30];
        }
    }

    debug("Done");
    gvars::currentregiony--;
}

void TilesGoLeft()
{
    std::cout << "Going Left \n";
    //[][][] = [][][]
    //[][][] = [][][]
    //[][][] = [][][]

    for (int i = 0; i != Grids; i++)
    {
        for (int t = 0; t != Grids; t++)
        {
            for (int x = 0; x != Grids; x++)
            {
                TempTile[i][t][x] = Tiles[i][t][x];
            }
        }
    }
    //[][<][<]
    //[][<][<]
    //[][<][<]

    for (int i = 32; i != Grids; i++)
    {
        for (int t = 0; t != Grids; t++)
        {
            Tiles[i - 32][t][30] = TempTile[i][t][30];
        }
    }
    debug("Starting");
    GenerateChunk(
        WorldMap[gvars::currentregionx + 2][gvars::currentregiony - 1].Type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx + 2, gvars::currentregiony - 1));
    for (int x = 0 + 32 + 32; x != ChunkSize + 32 + 32; x++)
    {
        for (int y = 0; y != ChunkSize; y++)
        {
            Tiles[x][y][30] = vChunk[x - 32 - 32][y][30];
        }
    }
    debug("Starting 2");
    GenerateChunk(
        WorldMap[gvars::currentregionx + 2][gvars::currentregiony].Type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx + 2, gvars::currentregiony));
    for (int x = 0 + 32 + 32; x != ChunkSize + 32 + 32; x++)
    {

        for (int y = 0 + 32; y != ChunkSize + 32; y++)
        {
            //std::cout << "X:" << x << "Y:" << y << ":" << vChunk[x][y][30].ID;

            //if(y == ChunkSize+32-1) std::cout << std::endl << "=============================" << std::endl;
            Tiles[x][y][30] = vChunk[x - 32 - 32][y - 32][30];
        }
    }
    debug("Starting 3");
    GenerateChunk(
        WorldMap[gvars::currentregionx + 2][gvars::currentregiony + 1].Type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx + 2, gvars::currentregiony + 1));
    for (int x = 0 + 32 + 32; x != ChunkSize + 32 + 32; x++)
    {
        for (int y = 0 + 32 + 32; y != ChunkSize + 32 + 32; y++)
        {
            Tiles[x][y][30] = vChunk[x - 32 - 32][y - 32 - 32][30];
        }
    }

    debug("Done");
    gvars::currentregionx++;
}

void TilesGoRight()
{
    Con("Going Right");
    //Tile TempTile[Grids][Grids][Grids];
    //[][][] = [][][]
    //[][][] = [][][]
    //[][][] = [][][]

    for (int i = 0; i != Grids; i++)
    {
        for (int t = 0; t != Grids; t++)
        {
            for (int x = 0; x != Grids; x++)
            {
                TempTile[i][t][x] = Tiles[i][t][x];
            }
        }
    }
    //[>][>][]
    //[>][>][]
    //[>][>][]

    for (int i = 0; i != Grids - 32; i++)
    {
        for (int t = 0; t != Grids; t++)
        {
            Tiles[i + 32][t][30] = TempTile[i][t][30];
        }
    }

    debug("Starting");
    GenerateChunk(
        WorldMap[gvars::currentregionx - 2][gvars::currentregiony - 1].Type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx - 2, gvars::currentregiony - 1));
    for (int x = 0; x != ChunkSize; x++)
    {
        for (int y = 0; y != ChunkSize; y++)
        {
            Tiles[x][y][30] = vChunk[x][y][30];
        }
    }
    debug("Starting 2");
    GenerateChunk(
        WorldMap[gvars::currentregionx - 2][gvars::currentregiony].Type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx - 2, gvars::currentregiony));
    for (int x = 0; x != ChunkSize; x++)
    {

        for (int y = 0 + 32; y != ChunkSize + 32; y++)
        {
            //std::cout << "X:" << x << "Y:" << y << ":" << vChunk[x][y][30].ID;

            //if(y == ChunkSize+32-1) std::cout << std::endl << "=============================" << std::endl;
            Tiles[x][y][30] = vChunk[x][y - 32][30];
        }
    }
    debug("Starting 3");
    GenerateChunk(
        WorldMap[gvars::currentregionx - 2][gvars::currentregiony + 1].Type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx - 2, gvars::currentregiony + 1));
    for (int x = 0; x != ChunkSize; x++)
    {
        for (int y = 0 + 32 + 32; y != ChunkSize + 32 + 32; y++)
        {
            Tiles[x][y][30] = vChunk[x][y - 32 - 32][30];
        }
    }

    LoadCritters(
        sf::Vector2i(gvars::currentregionx - 2, gvars::currentregiony + 1),
        "Left");

    debug("Done");
    gvars::currentregionx--;

    /*for(int i = Grids; i != 31; i--)
    {
        for(int t = 0; t != Grids; t++)
        {
            // Save Tile Stuffs
            Tiles[i][t][30] = Tiles[i-32][t][30];
            // Load Tile Stuffs
            int Ran = -1; //randz(0,2);
            if(Ran == 0)
            {
                Tiles[i-32][t][30].ID = 10;
//                Tiles[i-32][t][30].Img.setTexture( *imagemanager.GetImage("Underground.png"));
            }
            if(Ran == 1)
            {
                Tiles[i-32][t][30].ID = 20;
//                Tiles[i-32][t][30].Img.setTexture( *imagemanager.GetImage("Ground.bmp"));
            }
            if(Ran == 2)
            {
                Tiles[i-32][t][30].ID = 30;
//                Tiles[i-32][t][30].Img.setTexture( *imagemanager.GetImage("Sky.bmp"));
            }
        }
    }*/
}

void BuildLocalfromWorld(sf::Vector2i WorldPos)
{
    debug("Starting BLfW");
    GenerateChunk(WorldMap[WorldPos.x - 1][WorldPos.y - 1].Type,
                  gvars::currentplanet,
                  sf::Vector2i(WorldPos.x - 1, WorldPos.y - 1));
    for (int x = 0; x != ChunkSize; x++)
    {
        for (int y = 0; y != ChunkSize; y++)
        {
            Tiles[x][y][30] = vChunk[x][y][30];
        }
    }
    debug("Starting 2");
    GenerateChunk(WorldMap[WorldPos.x - 1][WorldPos.y].Type,
                  gvars::currentplanet,
                  sf::Vector2i(WorldPos.x - 1, WorldPos.y));
    for (int x = 0; x != ChunkSize; x++)
    {

        for (int y = 0 + 32; y != ChunkSize + 32; y++)
        {
            //std::cout << "X:" << x << "Y:" << y << ":" << vChunk[x][y][30].ID;

            //if(y == ChunkSize+32-1) std::cout << std::endl << "=============================" << std::endl;
            Tiles[x][y][30] = vChunk[x][y - 32][30];
        }
    }
    debug("Starting 3");
    GenerateChunk(WorldMap[WorldPos.x - 1][WorldPos.y + 1].Type,
                  gvars::currentplanet,
                  sf::Vector2i(WorldPos.x - 1, WorldPos.y + 1));
    for (int x = 0; x != ChunkSize; x++)
    {
        for (int y = 0 + 32 + 32; y != ChunkSize + 32 + 32; y++)
        {
            Tiles[x][y][30] = vChunk[x][y - 32 - 32][30];
        }
    }

    GenerateChunk(WorldMap[WorldPos.x + 1][WorldPos.y - 1].Type,
                  gvars::currentplanet,
                  sf::Vector2i(WorldPos.x + 1, WorldPos.y - 1));
    for (int x = 0 + 32 + 32; x != ChunkSize + 32 + 32; x++)
    {
        for (int y = 0; y != ChunkSize; y++)
        {
            Tiles[x][y][30] = vChunk[x - 32 - 32][y][30];
        }
    }
    debug("Starting 2");
    GenerateChunk(WorldMap[WorldPos.x + 1][WorldPos.y].Type,
                  gvars::currentplanet,
                  sf::Vector2i(WorldPos.x + 1, WorldPos.y));
    for (int x = 0 + 32 + 32; x != ChunkSize + 32 + 32; x++)
    {

        for (int y = 0 + 32; y != ChunkSize + 32; y++)
        {
            //std::cout << "X:" << x << "Y:" << y << ":" << vChunk[x][y][30].ID;

            //if(y == ChunkSize+32-1) std::cout << std::endl << "=============================" << std::endl;
            Tiles[x][y][30] = vChunk[x - 32 - 32][y - 32][30];
        }
    }
    debug("Starting 3");
    GenerateChunk(WorldMap[WorldPos.x + 1][WorldPos.y + 1].Type,
                  gvars::currentplanet,
                  sf::Vector2i(WorldPos.x + 1, WorldPos.y + 1));
    for (int x = 0 + 32 + 32; x != ChunkSize + 32 + 32; x++)
    {
        for (int y = 0 + 32 + 32; y != ChunkSize + 32 + 32; y++)
        {
            Tiles[x][y][30] = vChunk[x - 32 - 32][y - 32 - 32][30];
        }
    }

    GenerateChunk(WorldMap[WorldPos.x][WorldPos.y + 1].Type,
                  gvars::currentplanet,
                  sf::Vector2i(WorldPos.x, WorldPos.y + 1));
    for (int x = 0 + 32; x != ChunkSize + 32; x++)
    {
        for (int y = 0 + 32 + 32; y != ChunkSize + 32 + 32; y++)
        {
            //std::cout << "X:" << x << "Y:" << y << ":" << vChunk[x][y][30].ID;

            //if(y == ChunkSize+32-1) std::cout << std::endl << "=============================" << std::endl;
            Tiles[x][y][30] = vChunk[x - 32][y - 32 - 32][30];
        }
    }

    GenerateChunk(WorldMap[WorldPos.x][WorldPos.y - 1].Type,
                  gvars::currentplanet,
                  sf::Vector2i(WorldPos.x, WorldPos.y - 1));
    for (int x = 0 + 32; x != ChunkSize + 32; x++)
    {
        for (int y = 0; y != ChunkSize; y++)
        {
            //std::cout << "X:" << x << "Y:" << y << ":" << vChunk[x][y][30].ID;
            //if(y == ChunkSize+32-1) std::cout << std::endl << "=============================" << std::endl;
            Tiles[x][y][30] = vChunk[x - 32][y][30];
        }
    }

    GenerateChunk(WorldMap[WorldPos.x][WorldPos.y].Type, gvars::currentplanet,
                  sf::Vector2i(WorldPos.x, WorldPos.y));
    for (int x = 0 + 32; x != ChunkSize + 32; x++)
    {
        for (int y = 0 + 32; y != ChunkSize + 32; y++)
        {
            //std::cout << "X:" << x << "Y:" << y << ":" << vChunk[x][y][30].ID;
            //if(y == ChunkSize+32-1) std::cout << std::endl << "=============================" << std::endl;
            Tiles[x][y][30] = vChunk[x - 32][y - 32][30];
        }
    }

    debug("Done");
}

void Tile::Dirt()
{ // 1003
    ID = 1003;
    WorldColor = sf::Color(100, 100, 0);
    Img.setTexture(texturemanager.getTexture("Dirt.png"));
}

void Tile::RendGrass()
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

void Tile::Grass()
{ // 1001
    ID = 1001;
    WorldColor = sf::Color(0, 150, 0);
    Img.setTexture(texturemanager.getTexture("Grass.png"));
    //RendGrass();
}

void Tile::Stone()
{ // 1007
    ID = 1007;
    WorldColor = sf::Color(150, 150, 150);
    Walkable = true;
    Img.setTexture(texturemanager.getTexture("Underground.png"));
}

void Tile::StoneWall()
{ // 1008
    ID = 1008;
    WorldColor = sf::Color(150, 150, 150);
    Walkable = false;
    Img.setTexture(texturemanager.getTexture("StoneWall.png"));
}

void Tile::Wall()
{ // 1010
    ID = 1010;
    WorldColor = sf::Color(100, 100, 100);
    Img.setTexture(texturemanager.getTexture("Wall.png"));
    Health = 200;
    DeathID = 1007;
    Walkable = false;
}

void Tile::WoodFloor()
{ // 1030
    ID = 1030;
    WorldColor = sf::Color(150, 150, 0);
    Img.setTexture(texturemanager.getTexture("WoodFloor.png"));
}

void Tile::Road(bool Center)
{ // 1050
    ID = 1050;
    WorldColor = sf::Color(0, 0, 0);
    if (Center)
        Img.setTexture(texturemanager.getTexture("MiddleRoad.png"));
    if (!Center)
        Img.setTexture(texturemanager.getTexture("Black.png"));
}

void Tile::Door()
{ // 1100
    ID = 1100;
    WorldColor = sf::Color(255, 0, 0);
    Img.setTexture(texturemanager.getTexture("Door.png"));
    Walkable = true;
    Health = 15;
    DeathID = 7;
}

void Tile::Lava()
{ // 1337
    ID = 1337;
    WorldColor = sf::Color(255, 100, 0);
    Img.setTexture(texturemanager.getTexture("Lava.png"));
}

void Tile::Sky()
{ // 1700
    ID = 1700;
    WorldColor = sf::Color(150, 150, 150);
    Img.setTexture(texturemanager.getTexture("Sky.png"));
}

void Tile::Cake()
{
    ID = 1100;
    WorldColor = sf::Color(255, 0, 0);
    Img.setTexture(texturemanager.getTexture("Door.png"));
    Health = 15;
    DeathID = 7;
}

Tile::Tile() : ID{}
{
    WorldColor = sf::Color(0, 0, 0, 255);
    Health = 10;
    DeathID = ID;
    Walkable = true;
}

xTile::xTile() : ID{}, Img{nullptr}
{
    WorldColor = sf::Color(0, 0, 0, 255);
    Health = 10;
    DeathID = ID;
}

void xTile::initImage()
{
    Img = &texturemanager.getTexture("City.png");
}

WorldTile::WorldTile() : ID{}
{
    Infected = 0;
    Tiles = 32;
}
