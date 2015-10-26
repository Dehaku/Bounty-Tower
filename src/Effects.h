#ifndef __EFFECTS_H_INCLUDED__
#define __EFFECTS_H_INCLUDED__

#include <SFML/Graphics.hpp>
#include "math.h"
#include "InputState.h"
#include "globalvars.h"
#include "Shaders.h"


class Shape
{
public:

    enum
    {
        Circle,
        Square,
        Line
    };

    int shape;
    sf::Texture * texture;

    sf::Vector2f startPos;
    sf::Vector2f endPos;
    int size;
    int outline;
    int rotation;
    sf::Color maincolor;
    sf::Color seccolor;
    sf::View * drawView;


    int duration;
    bool fades;
    bool toDelete;

    Shape();
};

class Shapes
{
public:
    void createLine(int sxpos, int sypos, int expos, int eypos, int size,
                    sf::Color mainColor, float outline = 0,
                    sf::Color secondaryColor = sf::Color(0, 0, 0),
                    sf::View * drawView = &gvars::view1);

    void createSquare(int sxpos, int sypos, int expos, int eypos,
                      sf::Color mainColor, float outline = 0,
                      sf::Color secondaryColor = sf::Color(0, 0, 0),
                      sf::View * drawView = &gvars::view1);

    void createCircle(int xpos, int ypos, int Size, sf::Color MainColor,
                      float Outline = 0,
                      sf::Color SecondaryColor = sf::Color(0, 0, 0),
                      sf::View * drawView = &gvars::view1);

    std::vector<Shape> shapes;
    std::vector<sf::ConvexShape> polygons;

    void createCone(sf::Vector2f vPos, int angle, int angleRadius, int depth, sf::Color color);

    void drawShapes();
};
extern Shapes shapes;

class Orb
{
public:
    int orbRot;
    int orbs;
    int orbSpeed;
    int orbDistance;
    sf::Vector2f pos;
    unsigned int lifeTime;
    bool toDelete;
    int type;
    void updateOrb();
    void drawOrb(int totalOrbs);
    Orb();
};
extern std::vector<Orb> Orbs;


class EffectStorer
{
public:
    sf::Vector2f startPos;
    sf::Vector2f endPos;
    int size;
    int outline;
    bool fades;
    sf::Color maincolor;
    sf::Color seccolor;
    sf::View drawView;

    int duration;
    std::string effect;
    bool toDelete;

    EffectStorer();
    EffectStorer(std::string Effect, int Duration, sf::Vector2f StartPos);
};

void drawStoredEffects();

extern std::vector<EffectStorer> effectsStorage;

void purtyOrbitals();

#endif // EFFECTS_H_INCLUDED
