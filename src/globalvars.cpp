#include "globalvars.h"

#include <iostream>

void debug(std::string Info)
{
    if (gvars::debug)
        std::cout << Info << std::endl;
}

float CZ(float Number)
{
    return Number / gvars::cameraZoom;
}

namespace gvars
{
bool debug = false;
bool inFocus = true;
bool drawStuffsDone = true;
sf::View view1(Center, HalfSize);
int airPressure = 10;
sf::Vector2f mousePos;
int mouseStagnation = 0;
std::vector<int> selected;
float gCtimescale = 1;
int myTarget = -1;
int myTargetid = -1;
float cameraZoom = 1;
int currentx = 50;
int currenty = 50;
int currentz = 6;
int currentregionx = 50;
int currentregiony = 50;
int currentplanet = 500;
int groundmap[gridz][gridy][gridx];
int sunmap[gridz][gridy][gridx];
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

} // namespace gvars
