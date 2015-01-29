#ifndef __EFFECTS_H_INCLUDED__
#define __EFFECTS_H_INCLUDED__

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

/* Need to Make
Kamehame styled beam
*/

class Effects
{
public:
    float AngleBetweenVectors(sf::Vector2f a, sf::Vector2f b);
    int Closeish(int orix, int oriy, int tarx, int tary);
    class cLine
    {
    public:
        sf::Vector2f StartPos;
        sf::Vector2f EndPos;
        int size;
        int duration;
        bool fading;
        int faderate;
        int outline;
        sf::Color maincolor;
        sf::Color seccolor;

        void DrawMe();
    };
    class cCircle
    {
    public:
        sf::Vector2f Pos;
        int size;
        bool outline;
        sf::Color maincolor;
        sf::Color seccolor;

        void DrawMe();
    };
    class cSquare
    {
    public:
        sf::Vector2f StartPos;
        sf::Vector2f EndPos;
        bool outline;
        sf::Color maincolor;
        sf::Color seccolor;

        void DrawMe();
    };

    class cBeam
    {
    public:
        sf::Vector2f StartPos;
        sf::Vector2f EndPos;
        int size;
        int duration;
        bool fading;
        int faderate;
        int outline;
        sf::Color maincolor;
        sf::Color seccolor;

        void DrawMe();
    };
    std::vector<cCircle> Circles;
    std::vector<cLine> Lines;
    std::vector<cBeam> Beams;
    std::vector<cSquare> Squares;

    void CreateLine(int Sxpos, int Sypos, int Expos, int Eypos, int Size,
                    sf::Color MainColor, float Outline = 0,
                    sf::Color SecondaryColor = sf::Color(0, 0, 0),
                    bool Fades = false, int Fadesrate = 255);

    void CreateBeam(int Sxpos, int Sypos, int Expos, int Eypos, int Size,
                    sf::Color MainColor, float Outline = 0,
                    sf::Color SecondaryColor = sf::Color(0, 0, 0),
                    bool Fades = false, int Fadesrate = 255);

    void CreateSquare(int Sxpos, int Sypos, int Expos, int Eypos,
                      sf::Color MainColor, float Outline = 0,
                      sf::Color SecondaryColor = sf::Color(0, 0, 0));

    void CreateCircle(int xpos, int ypos, int Size, sf::Color MainColor,
                      float Outline = 0,
                      sf::Color SecondaryColor = sf::Color(0, 0, 0));

    void DrawEffects();
};
extern Effects Effectz;

#endif // EFFECTS_H_INCLUDED
