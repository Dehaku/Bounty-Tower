#ifndef __GLOBALS_H_INCLUDED__
#define __GLOBALS_H_INCLUDED__

#include <SFML/Graphics.hpp>

#include "defs.h"

float cz(float number);
void debug(std::string info, bool endline = true);


namespace mutex
{
    extern sf::Mutex npcList;
    extern sf::Mutex itemList;
}

namespace gvars
{
extern std::string version;
extern unsigned int framesPassed;
extern bool debug;
extern bool inFocus;
extern bool drawStuffsDone;
extern sf::View view1;
extern int airPressure;
extern sf::Vector2f mousePos;
extern int mouseStagnation;
extern std::vector<int> selected;
extern float gCtimescale;
extern int myTarget;
extern int myTargetid;
extern float cameraZoom;
extern int currentx;
extern int currenty;
extern int currentz;
extern int currentregionx;
extern int currentregiony;
extern int currentplanet;
extern int groundmap[GRID_Z][GRID_Y][GRID_X];
extern int sunmap[GRID_Z][GRID_Y][GRID_X];
extern bool following;
extern bool buttonClicked;
extern int buttonClickedTime;
extern sf::Vector2f heldClickPos;
extern int glbbtn;
extern float scalex;
extern float scaley;
extern unsigned long long globalid;
extern sf::Vector2f topLeft;
extern sf::Vector2f topRight;
extern sf::Vector2f bottomLeft;
extern sf::Vector2f bottomRight;
extern sf::Vector2f centerScreen;
extern std::string genString;
extern sf::Vector2i cords;
extern bool initalZeds;
extern float timeScale;
extern sf::Color cycleRed;
extern sf::Color cycleGreen;
extern sf::Color cycleBlue;
extern bool cycleGrowth;
extern float constantRotation;
extern bool secondSwitch;
extern bool sendGrid;
extern bool workingNpcList;
extern bool transitionLock;
extern int soundVolume;
extern int musicVolume;
extern int vocalVolume;
}

#endif // GLOBALS_H_INCLUDED
