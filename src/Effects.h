#ifndef __EFFECTS_H_INCLUDED__
#define __EFFECTS_H_INCLUDED__

#include <SFML/Graphics.hpp>

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

    void createLine(int Sxpos, int Sypos, int Expos, int Eypos, int Size,
                    sf::Color MainColor, float Outline = 0,
                    sf::Color SecondaryColor = sf::Color(0, 0, 0),
                    bool Fades = false, int Fadesrate = 255);

    void createBeam(int Sxpos, int Sypos, int Expos, int Eypos, int Size,
                    sf::Color MainColor, float Outline = 0,
                    sf::Color SecondaryColor = sf::Color(0, 0, 0),
                    bool Fades = false, int Fadesrate = 255);

    void createSquare(int Sxpos, int Sypos, int Expos, int Eypos,
                      sf::Color MainColor, float Outline = 0,
                      sf::Color SecondaryColor = sf::Color(0, 0, 0));

    void createCircle(int xpos, int ypos, int Size, sf::Color MainColor,
                      float Outline = 0,
                      sf::Color SecondaryColor = sf::Color(0, 0, 0));

    void drawEffects();
};
extern Effects effects;

#endif // EFFECTS_H_INCLUDED
