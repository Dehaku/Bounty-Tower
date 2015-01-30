#ifndef __GLOBALS_H_INCLUDED__
#define __GLOBALS_H_INCLUDED__

#include <SFML/Graphics.hpp>

#include "defs.h"

float CZ(float Number);
void debug(std::string Info);

extern bool Debug;
extern bool InFocus;
extern bool DrawStuffsDone;
extern sf::View View1;
extern int AirPressure;
extern float GX;
extern float GY;
extern sf::Vector2f MousePos;
extern int MouseStagnation;
extern std::vector<int> Selected;
extern float GCtimescale;
extern int MyTarget;
extern int MyTargetid;
extern float CameraZoom;
extern int currentx;
extern int currenty;
extern int currentz;
extern int currentregionx;
extern int currentregiony;
extern int currentplanet;
extern int groundmap[gridz][gridy][gridx];
extern int smellmap[gridz][gridy][gridx];

namespace globals
{
extern int sunmap[gridz][gridy][gridx];
extern bool Following;
extern bool ButtonClicked;
extern int ButtonClickedTime;
extern sf::Vector2f HeldClickPos;
extern int glbbtn;
extern float Scalex;
extern float Scaley;
extern unsigned long long globalid;
extern sf::Vector2f TopLeft;
extern sf::Vector2f TopRight;
extern sf::Vector2f BottomLeft;
extern sf::Vector2f BottomRight;
extern std::string GenString;
extern sf::Vector2i Cords;
extern bool InitalZeds;
extern float TimeScale;
}

#endif // GLOBALS_H_INCLUDED
