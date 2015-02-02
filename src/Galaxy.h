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

    GalaxyController();

    class MenuPointerContainer
    {
    public:
        Item *pItem;
        NPC *pNPC;
        Tile *pTile;

        std::vector<Item *> pVecItem;
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
    void BuildMainMenu();
    void BuildTileTest();
    void BuildWorldTest();
    void WorldLoop();
};
extern GalaxyController GC;

#endif // GALAXY_H_INCLUDED
