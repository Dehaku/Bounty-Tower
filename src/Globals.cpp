#include "Globals.h"

#include <iostream>

void debug(std::string Info)
{
    if (Debug)
        std::cout << Info << std::endl;
}

float CZ(float Number)
{
    return Number / globals::CameraZoom;
}

bool Debug = false;
bool InFocus = true;
bool DrawStuffsDone = true;
sf::View View1(Center, HalfSize);
int AirPressure = 10;
sf::Vector2f MousePos;
int MouseStagnation = 0;
std::vector<int> Selected;
float GCtimescale = 1;
int MyTarget = -1;
int MyTargetid = -1;

namespace globals
{
float CameraZoom = 1;
int currentx = 50;
int currenty = 50;
int currentz = 6;
int currentregionx = 50;
int currentregiony = 50;
int currentplanet = 500;
int groundmap[gridz][gridy][gridx];
int sunmap[gridz][gridy][gridx];
bool Following = false;
bool ButtonClicked = false;
int ButtonClickedTime = 0;
sf::Vector2f HeldClickPos;
int glbbtn = 100;
float Scalex = 1;
float Scaley = 1;
unsigned long long globalid = 100;
sf::Vector2f TopLeft;
sf::Vector2f TopRight;
sf::Vector2f BottomLeft;
sf::Vector2f BottomRight;
std::string GenString;
sf::Vector2i Cords;
bool InitalZeds = false;
float TimeScale = 1;
}
