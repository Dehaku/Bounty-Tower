#ifndef __GLOBALS_H_INCLUDED__
#define __GLOBALS_H_INCLUDED__

#include <SFML/Graphics.hpp>

#include "defs.h"

extern bool Debug;
extern bool InFocus;
extern bool DrawStuffsDone;

extern std::vector<std::string> chatboxlist;

class NPC;
extern std::vector<NPC> *Selection;

extern sf::Vector2f RMBHeldPos;

extern sf::Vector2f HalfSize;
extern sf::View View1;

extern int AirPressure;

extern float GX;
extern float GY;

extern sf::Vector2f MousePos;
extern int MouseStagnation;

extern sf::Sprite NewTile;

extern bool AllowedToAttack; // Set this to each NPC

extern std::vector<int> Selected;

extern float GCtimescale;

extern int MyTarget;
extern int MyTargetid;

extern float CameraZoom;

float CZ(float Number);

extern int currentx;
extern int currenty;
extern int currentz;

extern int currentregionx;
extern int currentregiony;
extern int currentplanet;

extern int groundmap[gridz][gridy][gridx];
extern int smellmap[gridz][gridy][gridx];
extern int sunmap[gridz][gridy][gridx];

enum class skills
{
    ag,
    ch,
    in,
    de,
    st,
    pe,
    en,
    wi
};

class Globes
{
public:
    bool Following;
    bool ButtonClicked;
    int ButtonClickedTime;
    sf::Vector2f HeldClickPos;
    int glbbtn;
    float Scalex;
    float Scaley;
    unsigned long long globalid;
    sf::Vector2f TopLeft;
    sf::Vector2f TopRight;
    sf::Vector2f BottomLeft;
    sf::Vector2f BottomRight;
    std::string GenString;
    sf::Vector2i Cords;
    bool InitalZeds;
    float TimeScale;

    Globes();
};
extern Globes Globals;

void debug(std::string Info);

#endif // GLOBALS_H_INCLUDED
