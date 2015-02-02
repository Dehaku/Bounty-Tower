#include "Effects.h"

#include <cmath>

extern sf::RenderWindow window;

Effects effects;

void Effects::Line::draw()
{
    /*//sf::Shape Line = sf::Shape::Line(StartPos.x,StartPos.y,EndPos.x,EndPos.y, size, maincolor, outline, seccolor);
    sf::Vertex Line[2] = {StartPos,EndPos};
    //Line.setFillColor(maincolor);
    //maincolor.a -= faderate;
    //if(maincolor.a < 0){maincolor.a = 0;}
    App.draw(Line,2,sf::Lines);*/

    sf::RectangleShape rectangle;

    int Length =
        sqrt(pow((endPos.x - startPos.x), 2) + pow((endPos.y - startPos.y), 2));
    rectangle.setSize(sf::Vector2f(Length, size));
    //rectangle.setSize(sf::Vector2f(StartPos.x-EndPos.x,StartPos.y-EndPos.y));
    float Angle =
        57.3065f * atan2(endPos.y - startPos.y, endPos.x - startPos.x);
    rectangle.setRotation(Angle);

    rectangle.setOrigin(0, size / 2);

    rectangle.setFillColor(maincolor);
    rectangle.setOutlineColor(seccolor);
    rectangle.setOutlineThickness(outline);
    rectangle.setPosition(startPos.x, startPos.y);
    window.draw(rectangle);
}

void Effects::Circle::draw()
{
    //sf::Shape Circle = sf::Shape::Circle(Pos.x,Pos.y,size, maincolor, outline, seccolor);
    sf::CircleShape Circle;
    Circle.setRadius(size);
    Circle.setFillColor(maincolor);
    Circle.setOutlineThickness(outline);
    Circle.setOutlineColor(seccolor);
    Circle.setPosition(pos.x, pos.y);
    Circle.setOrigin(size, size);
    window.draw(Circle);
}

void Effects::Square::draw()
{
    //sf::Shape Square = sf::Shape::Rectangle(StartPos.x,StartPos.y,EndPos.x,EndPos.y, maincolor, outline, seccolor);
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(100, 50));
    rectangle.setSize(
        sf::Vector2f(startPos.x - endPos.x, startPos.y - endPos.y));
    rectangle.setFillColor(maincolor);
    rectangle.setOutlineColor(seccolor);
    rectangle.setOutlineThickness(outline);
    rectangle.setPosition(endPos.x, endPos.y);
    window.draw(rectangle);
}

void Effects::Beam::draw()
{
    //sf::Shape Line = sf::Shape::Line(StartPos.x,StartPos.y,EndPos.x,EndPos.y, size, maincolor, outline, seccolor);
    sf::Vertex Line[2] = {startPos, endPos};
    //Line.setFillColor(maincolor);
    //maincolor.a -= faderate;
    //if(maincolor.a < 0){maincolor.a = 0;}
    window.draw(Line, 2, sf::Lines);

    sf::RectangleShape rectangle;

    int Length =
        sqrt(pow((endPos.x - startPos.x), 2) + pow((endPos.y - startPos.y), 2));
    rectangle.setSize(sf::Vector2f(Length, size));
    //rectangle.setSize(sf::Vector2f(StartPos.x-EndPos.x,StartPos.y-EndPos.y));
    float Angle =
        57.3065f * atan2(endPos.y - startPos.y, endPos.x - startPos.x);
    rectangle.setRotation(Angle);

    rectangle.setOrigin(0, size / 2);

    rectangle.setFillColor(maincolor);
    rectangle.setOutlineColor(seccolor);
    rectangle.setOutlineThickness(outline);
    rectangle.setPosition(startPos.x, startPos.y);
    window.draw(rectangle);
}

void Effects::createLine(int Sxpos, int Sypos, int Expos, int Eypos, int Size,
                         sf::Color MainColor, float Outline,
                         sf::Color SecondaryColor, bool Fades, int Fadesrate)
{
    Line evar;
    evar.startPos = sf::Vector2f(Sxpos, Sypos);
    evar.endPos = sf::Vector2f(Expos, Eypos);
    evar.size = Size;
    evar.fading = Fades;
    evar.faderate = Fadesrate;
    evar.outline = Outline;
    evar.maincolor = MainColor;
    evar.seccolor = SecondaryColor;
    lines.push_back(evar);
}

void Effects::createBeam(int Sxpos, int Sypos, int Expos, int Eypos, int Size,
                         sf::Color MainColor, float Outline,
                         sf::Color SecondaryColor, bool Fades, int Fadesrate)
{
    Beam evar;
    evar.startPos = sf::Vector2f(Sxpos, Sypos);
    evar.endPos = sf::Vector2f(Expos, Eypos);
    evar.size = Size;
    evar.fading = Fades;
    evar.faderate = Fadesrate;
    evar.outline = Outline;
    evar.maincolor = MainColor;
    evar.seccolor = SecondaryColor;
    beams.push_back(evar);
}

void Effects::createSquare(int Sxpos, int Sypos, int Expos, int Eypos,
                           sf::Color MainColor, float Outline,
                           sf::Color SecondaryColor)
{
    Square evar;
    evar.startPos = sf::Vector2f(Sxpos, Sypos);
    evar.endPos = sf::Vector2f(Expos, Eypos);
    evar.outline = Outline;
    evar.maincolor = MainColor;
    evar.seccolor = SecondaryColor;
    squares.push_back(evar);
}

void Effects::createCircle(int xpos, int ypos, int Size, sf::Color MainColor,
                           float Outline, sf::Color SecondaryColor)
{
    Circle evar;
    evar.pos = sf::Vector2f(xpos, ypos);
    evar.size = Size;
    evar.outline = Outline;
    evar.maincolor = MainColor;
    evar.seccolor = SecondaryColor;
    circles.push_back(evar);
}

void Effects::drawEffects()
{
    {
        int remover = 0;
        for (auto &elem : circles)
        {
            elem.draw();
            remover++;
            //if(effectz->fading == true && effectz->maincolor.a <= 0){removerz = remover;}
        }
        circles.clear();
    }

    for (auto &elem : squares)
    {
        elem.draw();
    }
    squares.clear();

    for (auto &elem : lines)
    {
        elem.draw();
    }
    lines.clear();

    for (auto &elem : beams)
    {
        elem.draw();
    }
    beams.clear();
}
