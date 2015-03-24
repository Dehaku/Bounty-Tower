#ifndef __EFFECTS_H_INCLUDED__
#define __EFFECTS_H_INCLUDED__

#include <SFML/Graphics.hpp>
#include "math.h"
#include "InputState.h"
#include "globalvars.h"

/* Need to Make
Kamehame styled beam
*/

class Effects
{
public:
    class Line
    {
    public:
        sf::Vector2f startPos;
        sf::Vector2f endPos;
        int size;
        int duration;
        bool fading;
        int faderate;
        int outline;
        sf::Color maincolor;
        sf::Color seccolor;

        void draw();
    };
    class Circle
    {
    public:
        sf::Vector2f pos;
        int size;
        bool outline;
        sf::Color maincolor;
        sf::Color seccolor;

        void draw();
    };
    class Square
    {
    public:
        sf::Vector2f startPos;
        sf::Vector2f endPos;
        bool outline;
        sf::Color maincolor;
        sf::Color seccolor;

        void draw();
    };

    class Beam
    {
    public:
        sf::Vector2f startPos;
        sf::Vector2f endPos;
        int size;
        int duration;
        bool fading;
        int faderate;
        int outline;
        sf::Color maincolor;
        sf::Color seccolor;

        void draw();
    };
    std::vector<Circle> circles;
    std::vector<Line> lines;
    std::vector<Beam> beams;
    std::vector<Square> squares;
    std::vector<sf::ConvexShape> polygons;

    void createLine(int sxpos, int sypos, int expos, int eypos, int size,
                    sf::Color mainColor, float outline = 0,
                    sf::Color secondaryColor = sf::Color(0, 0, 0),
                    bool fades = false, int fadesrate = 255);

    void createBeam(int sxpos, int sypos, int expos, int eypos, int size,
                    sf::Color mainColor, float outline = 0,
                    sf::Color secondaryColor = sf::Color(0, 0, 0),
                    bool fades = false, int fadesrate = 255);

    void createSquare(int sxpos, int sypos, int expos, int eypos,
                      sf::Color mainColor, float outline = 0,
                      sf::Color secondaryColor = sf::Color(0, 0, 0));

    void createCircle(int xpos, int ypos, int Size, sf::Color MainColor,
                      float Outline = 0,
                      sf::Color SecondaryColor = sf::Color(0, 0, 0));

    void drawEffects();
};
extern Effects effects;

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

void purtyOrbitals();

#endif // EFFECTS_H_INCLUDED
