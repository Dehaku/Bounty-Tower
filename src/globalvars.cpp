#include "globalvars.h"

#include <iostream>
#include "InputState.h"
#include "Networking.h"

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

void assignSlotPos()
{
    for( int i = 0; i != 20; i++)
    {
        sf::Vector2f vPos;
            //Left slots
            if(i == 0)
                vPos = sf::Vector2f(608,657);
            if(i == 1)
                vPos = sf::Vector2f(670,657);

            if(i == 2)
                vPos = sf::Vector2f(548, 674); // 764?
            if(i == 4)
                vPos = sf::Vector2f(485, 674);
            if(i == 6)
                vPos = sf::Vector2f(422, 674);
            if(i == 8)
                vPos = sf::Vector2f(359, 674);
            if(i == 10)
                vPos = sf::Vector2f(296, 674);
            if(i == 12)
                vPos = sf::Vector2f(233, 674);
            if(i == 14)
                vPos = sf::Vector2f(170, 674);
            if(i == 16)
                vPos = sf::Vector2f(107, 674);
            if(i == 18)
                vPos = sf::Vector2f(44, 674);


            // Right slots
            if(i == 3)
                vPos = sf::Vector2f(730, 674);
            if(i == 5)
                vPos = sf::Vector2f(793, 674);
            if(i == 7)
                vPos = sf::Vector2f(858, 674);
            if(i == 9)
                vPos = sf::Vector2f(919, 674);
            if(i == 11)
                vPos = sf::Vector2f(982, 674);
            if(i == 13)
                vPos = sf::Vector2f(1045, 674);
            if(i == 15)
                vPos = sf::Vector2f(1108, 674);
            if(i == 17)
                vPos = sf::Vector2f(1171, 674);
            if(i == 19)
                vPos = sf::Vector2f(1234, 674);
        gvars::slotPos.push_back(vPos);
    }

}

namespace gvars
{
bool hovering = false;
std::vector<sf::Vector2f> slotPos;
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
sf::Vector2f centerScreen;
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

bool transitionLock = true;

int soundVolume = 50;
int musicVolume = 50;
int vocalVolume = 50;

} // namespace gvars
