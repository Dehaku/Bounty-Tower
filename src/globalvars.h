#ifndef __GLOBALS_H_INCLUDED__
#define __GLOBALS_H_INCLUDED__

#include <SFML/Graphics.hpp>

#include "defs.h"

float CZ(float number);
void debug(std::string info);

namespace gvars
{
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
extern int groundmap[gridz][gridy][gridx];
extern int sunmap[gridz][gridy][gridx];
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
extern std::string genString;
extern sf::Vector2i cords;
extern bool initalZeds;
extern float timeScale;
extern sf::Color cycleRed;
extern sf::Color cycleGreen;
extern sf::Color cycleBlue;
extern bool cycleGrowth;
}

#endif // GLOBALS_H_INCLUDED
