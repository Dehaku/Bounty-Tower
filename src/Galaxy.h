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
    std::string phase;

    float timescale;
    float timescalez;
    float timetilltick;
    int waves;
    int waveTimer;
    int tickTimer;
    bool bTest;
    bool myDebug;
    sf::Vector2f menuPos;
    sf::Vector2f menuEndPos;
    std::string menuType;

    GalaxyController();

    class MenuPointerContainer
    {
    public:
        Item *pItem;
        Npc *pNPC;
        Tile *pTile;

        std::vector<Item *> pVecItem;
        std::vector<Npc *> pVecNPC;
        std::vector<Tile *> pVecTile;

        MenuPointerContainer();
    };
    MenuPointerContainer menuPtrCon;

    void addvPlanet();
    void wave();
    void time(int Modify);
    void buildLocal(std::string Type = "Test", int ZedAmount = 0);
    void buildLocalTest();
    void zombieSwarmLocal(int num = 5);
    void buildMainMenu();
    void buildTileTest();
    void buildWorldTest();
    void worldLoop();
};
extern GalaxyController gCtrl;

#endif // GALAXY_H_INCLUDED
