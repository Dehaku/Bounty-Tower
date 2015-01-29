#ifndef __GALAXY_H_INCLUDED__
#define __GALAXY_H_INCLUDED__

#include "Items.h"
#include "Tiles.h"

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

class GalaxyController
{
public:
    std::string Phase;

    float timescale;
    float timescalez;
    float timetilltick;
    int Waves;
    int WaveTimer;
    int TickTimer;
    bool bTest;
    bool Debug;
    sf::Vector2f MenuPos;
    sf::Vector2f MenuEndPos;
    std::string MenuType;
    //std::vector<Planet> vPlanets;

    GalaxyController();

    class MenuPointerContainer
    {
    public:
        item *pItem;
        NPC *pNPC;
        Tile *pTile;

        std::vector<item *> pVecItem;
        std::vector<NPC *> pVecNPC;
        std::vector<Tile *> pVecTile;

        MenuPointerContainer();
    };
    MenuPointerContainer MenuPtrCon;

    void AddvPlanet();

    void Wave();

    void Time(int Modify);

    void BuildLocal(std::string Type = "Test", int ZedAmount = 0);

    void BuildLocalTest();

    void ZombieSwarmLocal(int num = 5);
    /*
    bool BuildSolarTest(){
        int testnum;
        std::vector<planet>::iterator zit;
        testnum = AddPlanet("Sun", 0);
        for(zit = Planets.begin(); zit != Planets.end(); ++zit )
        {
            if(zit->id == testnum)
            {
                // TODO: Fix Laterzit->SetImgSun();
                zit->spawn(0,0,1200,1200);
            }
        }
    }


    bool CreatePlanet(int x, int y, int mass, sf::Vector2f Vel = sf::Vector2f(0,0)){
        std::vector<planet>::iterator zit;
        int testnum = 0;
        testnum = AddPlanet("Planet", 0);
        for(zit = Planets.begin(); zit != Planets.end(); ++zit )
        {
            if(zit->id == testnum)
            {
                zit->mass = 50;
                // TODO: Fix Laterzit->SetImgPlanet();
                // TODO: Fix Laterzit->img.Scale(zit->mass/100,zit->mass/100);
                zit->spawn(0,0,x,y);
            }
        }
    }
*/
    void BuildMainMenu();

    void BuildTileTest();

    void BuildWorldTest();

    void WorldLoop();
    //Finish playing with the territories, Either add more rules, or add relations, Perhaps reduce aggression by territory count, raise it by members, raise members by territory count, lose members during territory conflict.
};
extern GalaxyController GC;

#endif // GALAXY_H_INCLUDED
