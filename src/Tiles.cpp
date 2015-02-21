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

Tile tiles[GRIDS][GRIDS][GRIDS];
WorldTile worldMap[100][100];
Tile vChunk[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]; // 10223616
Tile tempTile[GRIDS][GRIDS][GRIDS];
XTile xChunk[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

extern sf::RenderWindow window;

void zGenerateChunk(std::string type, int planet, sf::Vector2i cords,
                    sf::Vector2i /*Pos*/)
{

    con("Queso?");

    std::string deb = "Creating Chunk: ";
    deb.append(type);
    debug(deb);

    if (type == "Building")
    {

        for (int i = 0; i != CHUNK_SIZE; i++)
        {
            for (int t = 0; t != CHUNK_SIZE; t++)
            {
                for (int x = 0; x != CHUNK_SIZE; x++)
                {
                    if (x < 30)
                        vChunk[i][t][x].stone();
                    if (x > 30)
                        vChunk[i][t][x].sky();
                }
            }
        }

        for (int i = 0; i != CHUNK_SIZE; i++)
        {
            for (int t = 0; t != CHUNK_SIZE; t++)
            {
                vChunk[i][t][0].stone();
            }
        }
        for (int i = 10; i != 21; i++)
        {
            for (int t = 10; t != 21; t++)
            {
                vChunk[i][t][0].wall();
            }
        }
        for (int i = 11; i != 20; i++)
        {
            for (int t = 11; t != 20; t++)
            {
                vChunk[i][t][0].stone();
            }
        }
        vChunk[15][20][0].door();
        for (int i = 0; i != CHUNK_SIZE; i++)
        {
            for (int t = 0; t != CHUNK_SIZE; t++)
            {
                for (int x = 0; x != CHUNK_SIZE; x++)
                {
                    vChunk[i][t][x].stone();
                }
            }
        }
    }

    using namespace std; // Start Saving Map
    debug("Saving current map to file...");
    string line("data/maps/map");
    stringstream convert;
    convert << planet;
    convert << cords.x;
    convert << cords.y;
    line.append(convert.str());
    string ending(".map");
    line.append(ending);
    cout << line << endl;
    ofstream outputFile(line.c_str());
    //outputFile << biometype << endl;
    debug("Writting map to file...");
    for (int i = 0; i != CHUNK_SIZE; i++)
    {
        for (int t = 0; t != CHUNK_SIZE; t++)
        {
            for (int x = 0; x != CHUNK_SIZE; x++)
            {
                outputFile << vChunk[i][t][0].id << " ";
            }
            outputFile << endl;
        }
        outputFile << endl;
    }
}

void generateChunk(std::string type, int planet, sf::Vector2i cords,
                   sf::Vector2i /*Pos*/)
{

    if (type == "Building")
    {
        for (int i = 0; i != CHUNK_SIZE; i++)
        {
            for (int t = 0; t != CHUNK_SIZE; t++)
            {
                for (int x = 0; x != CHUNK_SIZE; x++)
                {
                    if (x < 30)
                        vChunk[i][t][x].stoneWall();
                    if (x == 30)
                        vChunk[i][t][x].dirt();
                    if (x > 30)
                        vChunk[i][t][x].sky();
                    if (i >= 10 && i < 21 && x == 30)
                    {
                        if (t >= 10 && t < 21)
                        {
                            vChunk[i][t][x].wall();
                        }

                        if (i >= 11 && i < 20)
                        {
                            if (t >= 11 && t < 20)
                            {
                                vChunk[i][t][x].stone();
                            }
                        }
                    }

                    if (i >= 10 && i < 21 && x == 29)
                    {
                        if (t >= 10 && t < 21)
                        {
                            vChunk[i][t][x].wall();
                        }

                        if (i >= 11 && i < 20)
                        {
                            if (t >= 11 && t < 20)
                            {
                                vChunk[i][t][x].stone();
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
        vChunk[15][20][30].door();
        vChunk[15][12][30].stairsDown();
        vChunk[15][12][29].stairsUp();
        //vChunk[11][15][30].teleportPad(Vec3(21,15,30));
        //vChunk[21][15][30].teleportPad(Vec3(11,15,30));
    }

    else if (type == "SouthernHouse")
    {

        for (int i = 0; i != CHUNK_SIZE; i++)
        {
            for (int t = 0; t != CHUNK_SIZE; t++)
            {
                for (int x = 0; x != CHUNK_SIZE; x++)
                {
                    if (x < 30)
                        vChunk[i][t][x].stone();
                    if (x > 30)
                        vChunk[i][t][x].sky();
                }
            }
        }

        for (int i = 0; i != CHUNK_SIZE; i++)
        {
            for (int t = 0; t != CHUNK_SIZE; t++)
            {
                vChunk[i][t][0].grass();
            }
        }
        for (int i = 10; i != 21; i++)
        {
            for (int t = 5; t != 21; t++)
            {
                vChunk[i][t][0].wall();
            }
        }
        for (int i = 11; i != 20; i++)
        {
            for (int t = 11; t != 20; t++)
            {
                vChunk[i][t][0].woodFloor();
            }
        }
        for (int i = 11; i != 20; i++)
        {
            for (int t = 6; t != 10; t++)
            {
                vChunk[i][t][0].woodFloor();
            }
        }
        vChunk[15][20][0].door();
        vChunk[11][10][0].door();
    }

    else if (type == "Road")
    {

        for (int i = 0; i != CHUNK_SIZE; i++)
        {
            for (int t = 0; t != CHUNK_SIZE; t++)
            {
                for (int x = 0; x != CHUNK_SIZE; x++)
                {
                    if (x < 30)
                        vChunk[i][t][x].stone();
                    if (x > 30)
                        vChunk[i][t][x].sky();
                }
            }
        }

        for (int i = 0; i != CHUNK_SIZE; i++)
        {
            for (int t = 0; t != CHUNK_SIZE; t++)
            {
                vChunk[i][t][30].grass();
            }
        }
        for (int i = 11; i != 23; i++)
        {
            for (int t = 0; t != CHUNK_SIZE; t++)
            {
                vChunk[i][t][30].road();
            }
        }
        for (int i = 16; i != 18; i++)
        {
            for (int t = 0; t != CHUNK_SIZE; t++)
            {
                vChunk[i][t][30].road(true);
            }
        }
    }

    else if (type == "Spire")
    {

        for (int i = 0; i != CHUNK_SIZE; i++)
        {
            for (int t = 0; t != CHUNK_SIZE; t++)
            {
                for (int x = 0; x != CHUNK_SIZE; x++)
                {
                    if (x < 30)
                        vChunk[i][t][x].stoneWall();
                    if (x > 30)
                        vChunk[i][t][x].sky();
                }
            }
        }

        for (int i = 0; i != CHUNK_SIZE; i++)
        {
            for (int t = 0; t != CHUNK_SIZE; t++)
            {
                vChunk[i][t][30].grass();
                if (randz(1, 4) == 1)
                    vChunk[i][t][30].stone();
            }
        }
        for (int i = 4; i != 28; i++)
        {
            for (int t = 1; t != 28; t++)
            {
                //vChunk[i][t][30].Road();
                if (randz(1, 100) == 1)
                {
                    vChunk[i - 1][t - 1][30].stone();
                    vChunk[i - 1][t + 1][30].stone();
                    vChunk[i + 1][t - 1][30].stone();
                    vChunk[i + 1][t + 1][30].stone();
                    vChunk[i][t + 1][30].stone();
                    vChunk[i][t - 1][30].stone();
                    vChunk[i + 1][t][30].stone();
                    vChunk[i - 1][t][30].stone();
                    vChunk[i][t][30].stoneWall();
                }
                else if (randz(1, 10) == 1)
                {
                    vChunk[i][t][30].stone();
                }
                else if (randz(1, 20) == 300)
                {
                    //int xsize = randz(i,i-Math::Clamp(32,0,Math::i) );
                    //int ysize = randz(5,29);
                }
                if (randz(1, 10000) == 1)
                {
                    vChunk[i - 1][t - 1][30].stone();
                    vChunk[i - 1][t + 1][30].stone();
                    vChunk[i + 1][t - 1][30].stone();
                    vChunk[i + 1][t + 1][30].stone();
                    vChunk[i][t + 1][30].stone();
                    vChunk[i][t - 1][30].stone();
                    vChunk[i + 1][t][30].stone();
                    vChunk[i - 1][t][30].stone();
                    vChunk[i][t][30].lava();
                }
            }
        }
    }

    else if (type == "Plains")
    {

        for (int i = 0; i != CHUNK_SIZE; i++)
        {
            for (int t = 0; t != CHUNK_SIZE; t++)
            {
                for (int x = 0; x != CHUNK_SIZE; x++)
                {
                    if (x < 25)
                        vChunk[i][t][x].stoneWall();
                    else if (x < 30)
                        vChunk[i][t][x].stoneWall(); // Replace with DirtWall when it exists.
                    if (x == 30)
                        vChunk[i][t][x].grass();
                    if (x > 30)
                        vChunk[i][t][x].sky();
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
    convert << cords.x;
    convert << "y";
    convert << cords.y;
    line.append(convert.str());
    string ending(".map");
    line.append(ending);
    //cout << line << endl;
    // 162,176:1057
    // 5.38,16.0:65
    if (true == false)
    {
        sf::Image image;
        image.create(CHUNK_SIZE * 100, CHUNK_SIZE * 100,
                     sf::Color(0, 0, 0, 255));
        int XX = 0;
        int YY = 0;
        con("Post Image");

        for (int i = cords.x * CHUNK_SIZE;
             i != (cords.x * CHUNK_SIZE) + CHUNK_SIZE; i++)
        {

            for (int t = cords.y * CHUNK_SIZE;
                 t != (cords.y * CHUNK_SIZE) + CHUNK_SIZE; t++)
            {
                image.setPixel(
                    i, t,
                    vChunk[abs_to_index(XX)][abs_to_index(YY)][0].worldColor);
                YY++;
            }
            YY = 0;
            XX++;
        }

        con("Saving Image");
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
    for (int x = 0; x != CHUNK_SIZE; x++)
    {
        auto id = vChunk[0][0][x].id;
        bool same = true;
        for (int i = 0; i != CHUNK_SIZE; i++)
        {
            for (int t = 0; t != CHUNK_SIZE; t++)
            {
                if (vChunk[i][t][x].id != id)
                {
                    same = false;
                    //Con("Same = false;");
                }
            }
        }

        if (same)
        {
            outputFile << "[";
            outputFile << x;
            outputFile << ",";
            outputFile << id;
            outputFile << "]" << endl;
        }
        else
        {
            for (int i = 0; i != CHUNK_SIZE; i++)
            {
                for (int t = 0; t != CHUNK_SIZE; t++)
                {
                    outputFile << vChunk[i][t][x].id << " ";
                }
                outputFile << endl;
            }
            outputFile << endl;
        }
    }
}

void generateWorld(int regionSize, int planetNum)
{

    for (int x = 0; x != regionSize; x++)
    {
        for (int y = 0; y != regionSize; y++)
        {
            int ranFor = randz(0, 10);

            if (ranFor == 10)
                generateChunk("Building", planetNum, sf::Vector2i(x, y));
            else if (ranFor == 9)
                generateChunk("SouthernHouse", planetNum, sf::Vector2i(x, y));
            else if (ranFor == 8)
                generateChunk("Road", planetNum, sf::Vector2i(x, y));
            else if (ranFor <= 7)
                generateChunk("Building", planetNum, sf::Vector2i(x, y));

            //GenerateChunk("Building",PlanetNum,x,y);
        }
    }

    //GenerateChunk("Building",500,sf::Vector2i(abs(GC.MenuPos.x/20),abs(GC.MenuPos.y/20)));
}

void saveMap(int planet, int /*xcord*/, int /*ycord*/, int xpos, int ypos)
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
    for (int i = 0; i != GRIDS; i++)
    {
        for (int t = 0; t != GRIDS; t++)
        {
            outputFile << tiles[i][t][30].id << endl;
        }
    }
}

void loadMap(int planet, int xcord, int ycord, int /*xpos*/, int /*ypos*/)
{
    using namespace std;
    fstream file; // Start to Load Map
    string newline("saves/maps/map");
    stringstream newconvert;
    newconvert << planet;
    newconvert << xcord;
    newconvert << ycord;
    newline.append(newconvert.str());
    string newending(".txt");
    newline.append(newending);

    file.open(newline.c_str(), fstream::in);
    debug("looking for file...");

    if (file.is_open())
    {
        cout << "found file: " << newline << endl;
        cout << "Read string to file...\n";
        //File >> biometype;
        for (int i = 0; i != GRIDS; i++)
        {
            for (int t = 0; t != GRIDS; t++)
            {
                file >> tiles[i][t][30].id;
            }
        }
        file.close();
    } // End to Load Map
}

void initalizeWorldTiles()
{
    sf::Texture txtr;
    sf::Sprite sprt;
    std::cout << "Texture SizeOf: " << sizeof(txtr)
              << ", Sprite: " << sizeof(sprt) << std::endl;
    int Cities = 0;
    int Plains = 0;
    int Spires = 0;

    for (int i = 0; i != 100; i++)
    { // Generating the tiles.
        for (int t = 0; t != 100; t++)
        {
            worldMap[i][t].id = randz(0, 10);
            if (worldMap[i][t].id == WorldTileType::City)
            {
                worldMap[i][t].id = WorldTileType::City;
                worldMap[i][t].img.setTexture(
                    texturemanager.getTexture("City.png"));
                if (gvars::initalZeds)
                    worldMap[i][t].infected = randz(0, 20);
                else
                    worldMap[i][t].infected = 0;
                worldMap[i][t].type = "Building";
                Cities++;
            }
            else if (worldMap[i][t].id == WorldTileType::Spire)
            {
                worldMap[i][t].id = WorldTileType::Spire;
                worldMap[i][t].img.setTexture(
                    texturemanager.getTexture("Spire.png"));
                worldMap[i][t].infected = 0;
                worldMap[i][t].type = "Spire";
                Spires++;
            }
            else
            {
                worldMap[i][t].id = WorldTileType::Grass;
                worldMap[i][t].img.setTexture(
                    texturemanager.getTexture("Grass.png"));
                worldMap[i][t].infected = 0;
                worldMap[i][t].type = "Plains";
                Plains++;
            }
        }
    }
    std::cout << "Cities/Spires/Plains: " << Cities << "/" << Spires << "/"
              << Plains << std::endl;
}

void drawWorldTiles()
{
    debug("Drawing World Tiles");
    for (int i = 0; i != 100; i++)
    {
        for (int t = 0; t != 100; t++)
        {

            if (aabb(i * 20, t * 20, gvars::topLeft.x - 20, gvars::topRight.x,
                     gvars::topLeft.y - 20, gvars::bottomRight.y))
            {

                worldMap[i][t].img.setPosition(i * 20, t * 20);

                window.draw(worldMap[i][t].img);
                if (worldMap[i][t].infected > 0)
                {
                    effects.createSquare(
                        (i * 20) + 1, (t * 20) + 1, ((i * 20) + 20) - 1,
                        ((t * 20) + 20) - 1, sf::Color(0, 0, 0, 0), 1,
                        sf::Color(
                            255, 0, 0,
                            math::clamp(10 * worldMap[i][t].infected, 0, 255)));
                }
                if (worldMap[i][t].owner != "")
                {
                    for (auto &z : uniFact)
                    {
                        if (worldMap[i][t].owner == z.name)
                        {
                            sf::Color prim = z.primaryColor;
                            prim.a = prim.a / 1.3;
                            sf::Color seco = z.secondaryColor;
                            seco.a = seco.a / 1.3;
                            effects.createSquare(
                                (i * 20) + 1, (t * 20) + 1, ((i * 20) + 20) - 1,
                                ((t * 20) + 20) - 1, prim, 10, seco);
                        }
                    }
                }
            }
        }
    }
    debug("Done Drawing");
}

void initializeTiles()
{ // TODO: Make a Tile Manager to hold the presets of these, Then do Tiles[i][t] = TileManager.at(ect)
    for (int i = 0; i != GRIDS; i++)
    {
        for (int t = 0; t != GRIDS; t++)
        {
            tiles[i][t][30].sky();
        }
    }
    for (int i = 0; i != GRIDS; i++)
    {
        tiles[i][20][30].dirt();
    }
    for (int i = 0; i != GRIDS; i++)
    {
        for (int t = 21; t != GRIDS; t++)
        {
            tiles[i][t][30].stone();
        }
    }
}

void initializeTilePositions()
{
    for (int x = 0; x != GRIDS; x++)
        for (int y = 0; y != GRIDS; y++)
            for (int z = 0; z != CHUNK_SIZE; z++)
                tiles[x][y][z].setPos(x,y,z);
}

void drawTile(int xpos, int ypos, sf::Texture &image)
{
    con("1stBacon");
    sf::Sprite imgr;
    con("2ndBacon");
    imgr.setTexture(image);
    con("Bacon");
    imgr.setPosition(xpos, ypos);
    con("Bacon");
    window.draw(imgr);
}

void drawNewTiles()
{
    for (int i = 0; i != GRIDS; i++)
    {
        for (int t = 0; t != GRIDS; t++)
        {
            if (aabb(i * 20, t * 20, gvars::topLeft.x - 20, gvars::topRight.x,
                     gvars::topLeft.y - 20, gvars::bottomRight.y))
            {
                tiles[i][t][gvars::currentz].img.setPosition(i * 20, t * 20);
                window.draw(tiles[i][t][gvars::currentz].img);
                if(tiles[i][t][gvars::currentz].teleporter)
                {
                    sf::Sprite spinner;
                    spinner.setTexture(texturemanager.getTexture("Sphere.png"));
                    spinner.setRotation(gvars::constantRotation);
                    spinner.setPosition(i*20+10,t*20+10);
                    spinner.setOrigin(10.5f,10.5f);
                    sf::Color spin = spinner.getColor();
                    spin.a = 127;
                    spinner.setColor(spin);
                    window.draw(spinner);
                }
            }
        }
    }
    debug("Done Drawing New Tiles");
}

void tilesRandom()
{
    for (int i = 0; i != GRIDS; i++)
    {
        for (int t = 0; t != GRIDS; t++)
        {
            int ran = randz(0, 2);
            if (ran == 0)
            {
                tiles[i][t][30].id = 10;
                //                Tiles[i][t][30].Img.setTexture( *imagemanager.GetImage("Underground.png"));
            }
            if (ran == 1)
            {
                tiles[i][t][30].id = 20;
                //                Tiles[i][t][30].Img.setTexture( *imagemanager.GetImage("Ground.bmp"));
            }
            if (ran == 2)
            {
                tiles[i][t][30].id = 30;
                //                Tiles[i][t][30].Img.setTexture( *imagemanager.GetImage("Sky.bmp"));
            }
        }
    }
}

void tilesGoUp()
{
    std::cout << "Going Up \n";
    for (int i = 0; i != GRIDS; i++)
    {
        for (int t = 0; t != GRIDS; t++)
        {
            for (int x = 0; x != GRIDS; x++)
            {
                tempTile[i][t][x] = tiles[i][t][x];
            }
        }
    }
    //[ ][ ][ ]
    //[^][^][^]
    //[^][^][^]

    for (int i = 0; i != GRIDS; i++)
    {
        for (int t = 32; t != GRIDS; t++)
        {
            tiles[i][t - 32][30] = tempTile[i][t][30];
        }
    }

    debug("Starting");
    generateChunk(
        worldMap[gvars::currentregionx - 1][gvars::currentregiony + 2].type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx - 1, gvars::currentregiony + 2));
    for (int x = 0; x != CHUNK_SIZE; x++)
    {
        for (int y = 0 + 32 + 32; y != CHUNK_SIZE + 32 + 32; y++)
        {
            tiles[x][y][30] = vChunk[x][y - 32 - 32][30];
        }
    }
    debug("Starting 2");
    generateChunk(
        worldMap[gvars::currentregionx][gvars::currentregiony + 2].type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx, gvars::currentregiony + 2));
    for (int x = 0 + 32; x != CHUNK_SIZE + 32; x++)
    {
        for (int y = 0 + 32 + 32; y != CHUNK_SIZE + 32 + 32; y++)
        {
            //std::cout << "X:" << x << "Y:" << y << ":" << vChunk[x][y][30].ID;

            //if(y == ChunkSize+32-1) std::cout << std::endl << "=============================" << std::endl;
            tiles[x][y][30] = vChunk[x - 32][y - 32 - 32][30];
        }
    }
    debug("Starting 3");
    generateChunk(
        worldMap[gvars::currentregionx + 1][gvars::currentregiony + 2].type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx + 1, gvars::currentregiony + 2));
    for (int x = 0 + 32 + 32; x != CHUNK_SIZE + 32 + 32; x++)
    {
        for (int y = 0 + 32 + 32; y != CHUNK_SIZE + 32 + 32; y++)
        {
            tiles[x][y][30] = vChunk[x - 32 - 32][y - 32 - 32][30];
        }
    }

    debug("Done");
    gvars::currentregiony++;
}

void tilesGoDown()
{
    std::cout << "Going Down \n";
    for (int i = 0; i != GRIDS; i++)
    {
        for (int t = 0; t != GRIDS; t++)
        {
            for (int x = 0; x != GRIDS; x++)
            {
                tempTile[i][t][x] = tiles[i][t][x];
            }
        }
    }
    //[v][v][v]
    //[v][v][v]
    //[ ][ ][ ]

    for (int i = 0; i != GRIDS; i++)
    {
        for (int t = 0; t != GRIDS - 32; t++)
        {
            tiles[i][t + 32][30] = tempTile[i][t][30];
        }
    }

    debug("Starting");
    generateChunk(
        worldMap[gvars::currentregionx - 1][gvars::currentregiony - 2].type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx - 1, gvars::currentregiony - 2));
    for (int x = 0; x != CHUNK_SIZE; x++)
    {
        for (int y = 0; y != CHUNK_SIZE; y++)
        {
            tiles[x][y][30] = vChunk[x][y][30];
        }
    }
    debug("Starting 2");
    generateChunk(
        worldMap[gvars::currentregionx][gvars::currentregiony - 2].type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx, gvars::currentregiony - 2));
    for (int x = 0 + 32; x != CHUNK_SIZE + 32; x++)
    {
        for (int y = 0; y != CHUNK_SIZE; y++)
        {
            //std::cout << "X:" << x << "Y:" << y << ":" << vChunk[x][y][30].ID;

            //if(y == ChunkSize+32-1) std::cout << std::endl << "=============================" << std::endl;
            tiles[x][y][30] = vChunk[x - 32][y][30];
        }
    }
    debug("Starting 3");
    generateChunk(
        worldMap[gvars::currentregionx + 1][gvars::currentregiony - 2].type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx + 1, gvars::currentregiony - 2));
    for (int x = 0 + 32 + 32; x != CHUNK_SIZE + 32 + 32; x++)
    {
        for (int y = 0; y != CHUNK_SIZE; y++)
        {
            tiles[x][y][30] = vChunk[x - 32 - 32][y][30];
        }
    }

    debug("Done");
    gvars::currentregiony--;
}

void tilesGoLeft()
{
    std::cout << "Going Left \n";
    //[][][] = [][][]
    //[][][] = [][][]
    //[][][] = [][][]

    for (int i = 0; i != GRIDS; i++)
    {
        for (int t = 0; t != GRIDS; t++)
        {
            for (int x = 0; x != GRIDS; x++)
            {
                tempTile[i][t][x] = tiles[i][t][x];
            }
        }
    }
    //[][<][<]
    //[][<][<]
    //[][<][<]

    for (int i = 32; i != GRIDS; i++)
    {
        for (int t = 0; t != GRIDS; t++)
        {
            tiles[i - 32][t][30] = tempTile[i][t][30];
        }
    }
    debug("Starting");
    generateChunk(
        worldMap[gvars::currentregionx + 2][gvars::currentregiony - 1].type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx + 2, gvars::currentregiony - 1));
    for (int x = 0 + 32 + 32; x != CHUNK_SIZE + 32 + 32; x++)
    {
        for (int y = 0; y != CHUNK_SIZE; y++)
        {
            tiles[x][y][30] = vChunk[x - 32 - 32][y][30];
        }
    }
    debug("Starting 2");
    generateChunk(
        worldMap[gvars::currentregionx + 2][gvars::currentregiony].type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx + 2, gvars::currentregiony));
    for (int x = 0 + 32 + 32; x != CHUNK_SIZE + 32 + 32; x++)
    {

        for (int y = 0 + 32; y != CHUNK_SIZE + 32; y++)
        {
            //std::cout << "X:" << x << "Y:" << y << ":" << vChunk[x][y][30].ID;

            //if(y == ChunkSize+32-1) std::cout << std::endl << "=============================" << std::endl;
            tiles[x][y][30] = vChunk[x - 32 - 32][y - 32][30];
        }
    }
    debug("Starting 3");
    generateChunk(
        worldMap[gvars::currentregionx + 2][gvars::currentregiony + 1].type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx + 2, gvars::currentregiony + 1));
    for (int x = 0 + 32 + 32; x != CHUNK_SIZE + 32 + 32; x++)
    {
        for (int y = 0 + 32 + 32; y != CHUNK_SIZE + 32 + 32; y++)
        {
            tiles[x][y][30] = vChunk[x - 32 - 32][y - 32 - 32][30];
        }
    }

    debug("Done");
    gvars::currentregionx++;
}

void tilesGoRight()
{
    con("Going Right");
    //Tile TempTile[Grids][Grids][Grids];
    //[][][] = [][][]
    //[][][] = [][][]
    //[][][] = [][][]

    for (int i = 0; i != GRIDS; i++)
    {
        for (int t = 0; t != GRIDS; t++)
        {
            for (int x = 0; x != GRIDS; x++)
            {
                tempTile[i][t][x] = tiles[i][t][x];
            }
        }
    }
    //[>][>][]
    //[>][>][]
    //[>][>][]

    for (int i = 0; i != GRIDS - 32; i++)
    {
        for (int t = 0; t != GRIDS; t++)
        {
            tiles[i + 32][t][30] = tempTile[i][t][30];
        }
    }

    debug("Starting");
    generateChunk(
        worldMap[gvars::currentregionx - 2][gvars::currentregiony - 1].type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx - 2, gvars::currentregiony - 1));
    for (int x = 0; x != CHUNK_SIZE; x++)
    {
        for (int y = 0; y != CHUNK_SIZE; y++)
        {
            tiles[x][y][30] = vChunk[x][y][30];
        }
    }
    debug("Starting 2");
    generateChunk(
        worldMap[gvars::currentregionx - 2][gvars::currentregiony].type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx - 2, gvars::currentregiony));
    for (int x = 0; x != CHUNK_SIZE; x++)
    {

        for (int y = 0 + 32; y != CHUNK_SIZE + 32; y++)
        {
            //std::cout << "X:" << x << "Y:" << y << ":" << vChunk[x][y][30].ID;

            //if(y == ChunkSize+32-1) std::cout << std::endl << "=============================" << std::endl;
            tiles[x][y][30] = vChunk[x][y - 32][30];
        }
    }
    debug("Starting 3");
    generateChunk(
        worldMap[gvars::currentregionx - 2][gvars::currentregiony + 1].type,
        gvars::currentplanet,
        sf::Vector2i(gvars::currentregionx - 2, gvars::currentregiony + 1));
    for (int x = 0; x != CHUNK_SIZE; x++)
    {
        for (int y = 0 + 32 + 32; y != CHUNK_SIZE + 32 + 32; y++)
        {
            tiles[x][y][30] = vChunk[x][y - 32 - 32][30];
        }
    }

    loadCritters(
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

void buildLocalfromWorld(sf::Vector2i worldPos)
{
    int worldX[9] = {-1,0,1,-1,0,1,-1,0,1};
    int worldY[9] = {-1,-1,-1,0,0,0,1,1,1};
    int gridX[9]  = {0,32,64,0,32,64,0,32,64};
    int gridY[9]  = {0,0,0,32,32,32,64,64,64};

    for (int i = 0; i != 9; i++)
    {
        generateChunk(worldMap[worldPos.x + worldX[i]][worldPos.y + worldY[i]].type,
                  gvars::currentplanet,
                  sf::Vector2i(worldPos.x + worldX[i], worldPos.y + worldY[i]));
        for (int x = 0 + gridX[i]; x != CHUNK_SIZE + gridX[i]; x++)
            for (int y = 0 + gridY[i]; y != CHUNK_SIZE + gridY[i]; y++)
                for (int z = 0; z != CHUNK_SIZE; z++)
                    tiles[x][y][z] = vChunk[x - gridX[i]][y - gridY[i]][z];
    }
    debug("bLfW Done");
}

void Tile::setPos(int x, int y, int z)
{
    pos = Vec3(x, y, z);
}

Vec3 Tile::getPos()
{
    return pos;
}

void Tile::dirt()
{ // 1003
    id = 1003;
    worldColor = sf::Color(100, 100, 0);
    img.setTexture(texturemanager.getTexture("Dirt.png"));
}

void Tile::rendGrass()
{ // 1001
    id = 1001;
    worldColor = sf::Color(0, 150, 0);
    sf::Image grazz;
    grazz.create(20, 20, sf::Color(0, 0, 0, 255));
    for (int i = 0; i != 20; i++)
    {
        for (int t = 0; t != 20; t++)
        {
            int pix = randz(0, 3);
            if (pix <= 2)
                grazz.setPixel(i, t, sf::Color(0, 128, 0, 255));
            if (pix == 3)
                grazz.setPixel(i, t, sf::Color(0, 255, 0, 255));
            //Grazz.setPixel(i,t,sf::Color(0,128,0,255));
        }
    }
    //sf::Texture Why;
    //Texture.loadFromImage(Grazz);
    //Img.setTexture(Texture);
    //Img.setTexture(Why);
    //Img.setTexture( *imagemanager.GetImage("Grass.png"));
}

void Tile::grass()
{ // 1001
    id = 1001;
    worldColor = sf::Color(0, 150, 0);
    img.setTexture(texturemanager.getTexture("Grass.png"));
    //RendGrass();
}

void Tile::stone()
{ // 1007
    id = 1007;
    worldColor = sf::Color(150, 150, 150);
    transparent = true;
    walkable = true;
    img.setTexture(texturemanager.getTexture("Underground.png"));
}

void Tile::stoneWall()
{ // 1008
    id = 1008;
    worldColor = sf::Color(150, 150, 150);
    transparent = false;
    walkable = false;
    img.setTexture(texturemanager.getTexture("StoneWall.png"));
}

void Tile::wall()
{ // 1010
    id = 1010;
    worldColor = sf::Color(100, 100, 100);
    img.setTexture(texturemanager.getTexture("Wall.png"));
    transparent = false;
    health = 200;
    deathID = 1007;
    walkable = false;
}

void Tile::woodFloor()
{ // 1030
    id = 1030;
    worldColor = sf::Color(150, 150, 0);
    img.setTexture(texturemanager.getTexture("WoodFloor.png"));
    transparent = true;
    walkable = true;
}

void Tile::stairsUp()
{ // 1031
    id = 1031;
    worldColor = sf::Color(150, 150, 0);
    img.setTexture(texturemanager.getTexture("Stairs.png"));
    transparent = true;
    walkable = true;
    goesUp = true;
    goesDown = true;
}

void Tile::stairsDown()
{ // 1032
    id = 1032;
    worldColor = sf::Color(150, 150, 0);
    img.setTexture(texturemanager.getTexture("Stairs.png"));
    transparent = true;
    walkable = true;
    goesDown = true;
    goesUp = true;
}

void Tile::road(bool center)
{ // 1050
    id = 1050;
    worldColor = sf::Color(0, 0, 0);
    if (center)
        img.setTexture(texturemanager.getTexture("MiddleRoad.png"));
    if (!center)
        img.setTexture(texturemanager.getTexture("Black.png"));
}

void Tile::door()
{ // 1100
    id = 1100;
    worldColor = sf::Color(255, 0, 0);
    img.setTexture(texturemanager.getTexture("Door.png"));
    transparent = true;
    walkable = true;
    health = 15;
    deathID = 7;
}

void Tile::lava()
{ // 1337
    id = 1337;
    worldColor = sf::Color(255, 100, 0);
    img.setTexture(texturemanager.getTexture("Lava.png"));
}

void Tile::sky()
{ // 1700
    id = 1700;
    worldColor = sf::Color(150, 150, 150);
    img.setTexture(texturemanager.getTexture("Sky.png"));
}

void Tile::cake()
{
    id = 1100;
    worldColor = sf::Color(255, 0, 0);
    img.setTexture(texturemanager.getTexture("Door.png"));
    health = 15;
    deathID = 7;
}

void Tile::teleportPad(Vec3 teleportPosition)
{ // 2000
    id = 2000;
    worldColor = sf::Color(255, 0, 255);
    img.setTexture(texturemanager.getTexture("TeleporterPad.png"));
    teleporter = true;
    telePos = teleportPosition;
}

Tile::Tile() : id{}
{
    worldColor = sf::Color(0, 0, 0, 255);
    health = 10;
    transparent = true;
    deathID = id;
    walkable = true;
    goesUp = false;
    goesDown = false;
    teleporter = false;
    telePos = Vec3(-1,-1,-1);
}

XTile::XTile() : id{}, img{nullptr}
{
    worldColor = sf::Color(0, 0, 0, 255);
    health = 10;
    deathID = id;
}

void XTile::initImage()
{
    img = &texturemanager.getTexture("City.png");
}

WorldTile::WorldTile() : id{}
{
    infected = 0;
    tiles = 32;
}
