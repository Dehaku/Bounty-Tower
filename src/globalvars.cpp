#include "globalvars.h"

#include <iostream>

void debug(std::string info, bool endline)
{
    if (gvars::debug)
    {
        std::cout << info;
        if(endline)
            std::cout << std::endl;
    }

}

float cz(float number)
{
    return number / gvars::cameraZoom;
}

namespace mutex
{
    sf::Mutex npcList;
    sf::Mutex itemList;
}

namespace gvars
{
std::string version = "0.0.1.0";
unsigned int framesPassed = 0;
bool debug = false;
bool inFocus = true;
bool drawStuffsDone = true;
sf::View view1(CENTER, HALF_SIZE);
int airPressure = 2;
sf::Vector2f mousePos;
int mouseStagnation = 0;
std::vector<int> selected;
float gCtimescale = 1;
int myTarget = -1;
int myTargetid = -1;
float cameraZoom = 1;
int currentx = 50;
int currenty = 50;
int currentz = 30;
int currentregionx = 50;
int currentregiony = 50;
int currentplanet = 500;
int groundmap[GRID_Z][GRID_Y][GRID_X];
int sunmap[GRID_Z][GRID_Y][GRID_X];
bool following = false;
bool buttonClicked = false;
int buttonClickedTime = 0;
sf::Vector2f heldClickPos;
int glbbtn = 100;
float scalex = 1;
float scaley = 1;
unsigned long long globalid = 100;
sf::Vector2f topLeft;
sf::Vector2f topRight;
sf::Vector2f bottomLeft;
sf::Vector2f bottomRight;
std::string genString;
sf::Vector2i cords;
bool initalZeds = false;
float timeScale = 1;

sf::Color cycleRed(255, 255, 255);
sf::Color cycleGreen(255, 255, 255);
sf::Color cycleBlue(255, 255, 255);
bool cycleGrowth = false;

float constantRotation = 0;

bool secondSwitch = false;

bool sendGrid = false;

bool workingNpcList = false;

} // namespace gvars
