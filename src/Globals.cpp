#include "Globals.h"

#include <iostream>

bool Debug = false;
bool InFocus = true;
bool DrawStuffsDone = true;
sf::View View1(Center, HalfSize);

int AirPressure = 10;

float GX = 0;
float GY = 0;

sf::Vector2f MousePos;
int MouseStagnation = 0;

bool AllowedToAttack = true; // Set this to each NPC

std::vector<int> Selected;

float GCtimescale = 1;

int MyTarget = -1;
int MyTargetid = -1;

float CameraZoom = 1;

//sf::Vector2i CameraSize = Rez;

float CZ(float Number)
{
    return Number / CameraZoom;
}

int currentx = 50;
int currenty = 50;
int currentz = 6;

int currentregionx = 50;
int currentregiony = 50;
int currentplanet = 500;

int groundmap[gridz][gridy][gridx];
int smellmap[gridz][gridy][gridx];
int sunmap[gridz][gridy][gridx];

void debug(std::string Info)
{
    if (Debug)
        std::cout << Info << std::endl;
}

Globes::Globes() : GenString{}
{
    globalid = 100;
    glbbtn = 100;
    Scalex = 1;
    Scaley = 1;
    ButtonClicked = false;
    ButtonClickedTime = 0;
    Following = false;
    InitalZeds = false;
    TimeScale = 1;
}
Globes Globals;
