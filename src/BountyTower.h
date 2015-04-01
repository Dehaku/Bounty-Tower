#ifndef BOUNTYTOWER_H_INCLUDED
#define BOUNTYTOWER_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "Textures.h"
#include "util.h"
#include "Galaxy.h"
#include "globalvars.h"
#include "InputState.h"
#include "menus.h"
#include "Camera.h"
#include "math.h"
#include "Tiles.h"

extern sf::RenderWindow window;

void bountyTowerSetup();
void bountyTowerLoop();

void buildTower(std::string towerName);

class Tower
{
public:
    sf::Texture *tex;
    std::string name;
    int difficulty;
    int minioncount;
    std::string bountyTarget;
    int bountyPay;
    int floors;
    Tower();
};

extern std::vector<Tower> towers;

#endif // BOUNTYTOWER_H_INCLUDED
