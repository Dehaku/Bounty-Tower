#include "Effects.h"

#include <cmath>

extern sf::RenderWindow App;

Effects Effectz;

void Effects::cLine::DrawMe()
{
    /*//sf::Shape Line = sf::Shape::Line(StartPos.x,StartPos.y,EndPos.x,EndPos.y, size, maincolor, outline, seccolor);
    sf::Vertex Line[2] = {StartPos,EndPos};
    //Line.setFillColor(maincolor);
    //maincolor.a -= faderate;
    //if(maincolor.a < 0){maincolor.a = 0;}
    App.draw(Line,2,sf::Lines);*/

    sf::RectangleShape rectangle;

    int Length =
        sqrt(pow((EndPos.x - StartPos.x), 2) + pow((EndPos.y - StartPos.y), 2));
    rectangle.setSize(sf::Vector2f(Length, size));
    //rectangle.setSize(sf::Vector2f(StartPos.x-EndPos.x,StartPos.y-EndPos.y));
    float Angle =
        57.3065f * atan2(EndPos.y - StartPos.y, EndPos.x - StartPos.x);
    rectangle.setRotation(Angle);

    rectangle.setOrigin(0, size / 2);

    rectangle.setFillColor(maincolor);
    rectangle.setOutlineColor(seccolor);
    rectangle.setOutlineThickness(outline);
    rectangle.setPosition(StartPos.x, StartPos.y);
    App.draw(rectangle);
}

void Effects::cCircle::DrawMe()
{
    //sf::Shape Circle = sf::Shape::Circle(Pos.x,Pos.y,size, maincolor, outline, seccolor);
    sf::CircleShape Circle;
    Circle.setRadius(size);
    Circle.setFillColor(maincolor);
    Circle.setOutlineThickness(outline);
    Circle.setOutlineColor(seccolor);
    Circle.setPosition(Pos.x, Pos.y);
    Circle.setOrigin(size, size);
    App.draw(Circle);
}

void Effects::cSquare::DrawMe()
{
    //sf::Shape Square = sf::Shape::Rectangle(StartPos.x,StartPos.y,EndPos.x,EndPos.y, maincolor, outline, seccolor);
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(100, 50));
    rectangle.setSize(
        sf::Vector2f(StartPos.x - EndPos.x, StartPos.y - EndPos.y));
    rectangle.setFillColor(maincolor);
    rectangle.setOutlineColor(seccolor);
    rectangle.setOutlineThickness(outline);
    rectangle.setPosition(EndPos.x, EndPos.y);
    App.draw(rectangle);
}

void Effects::cBeam::DrawMe()
{
    //sf::Shape Line = sf::Shape::Line(StartPos.x,StartPos.y,EndPos.x,EndPos.y, size, maincolor, outline, seccolor);
    sf::Vertex Line[2] = {StartPos, EndPos};
    //Line.setFillColor(maincolor);
    //maincolor.a -= faderate;
    //if(maincolor.a < 0){maincolor.a = 0;}
    App.draw(Line, 2, sf::Lines);

    sf::RectangleShape rectangle;

    int Length =
        sqrt(pow((EndPos.x - StartPos.x), 2) + pow((EndPos.y - StartPos.y), 2));
    rectangle.setSize(sf::Vector2f(Length, size));
    //rectangle.setSize(sf::Vector2f(StartPos.x-EndPos.x,StartPos.y-EndPos.y));
    float Angle =
        57.3065f * atan2(EndPos.y - StartPos.y, EndPos.x - StartPos.x);
    rectangle.setRotation(Angle);

    rectangle.setOrigin(0, size / 2);

    rectangle.setFillColor(maincolor);
    rectangle.setOutlineColor(seccolor);
    rectangle.setOutlineThickness(outline);
    rectangle.setPosition(StartPos.x, StartPos.y);
    App.draw(rectangle);
}

void Effects::CreateLine(int Sxpos, int Sypos, int Expos, int Eypos, int Size,
                         sf::Color MainColor, float Outline,
                         sf::Color SecondaryColor, bool Fades, int Fadesrate)
{
    cLine evar;
    evar.StartPos = sf::Vector2f(Sxpos, Sypos);
    evar.EndPos = sf::Vector2f(Expos, Eypos);
    evar.size = Size;
    evar.fading = Fades;
    evar.faderate = Fadesrate;
    evar.outline = Outline;
    evar.maincolor = MainColor;
    evar.seccolor = SecondaryColor;
    Lines.push_back(evar);
}

void Effects::CreateBeam(int Sxpos, int Sypos, int Expos, int Eypos, int Size,
                         sf::Color MainColor, float Outline,
                         sf::Color SecondaryColor, bool Fades, int Fadesrate)
{
    cBeam evar;
    evar.StartPos = sf::Vector2f(Sxpos, Sypos);
    evar.EndPos = sf::Vector2f(Expos, Eypos);
    evar.size = Size;
    evar.fading = Fades;
    evar.faderate = Fadesrate;
    evar.outline = Outline;
    evar.maincolor = MainColor;
    evar.seccolor = SecondaryColor;
    Beams.push_back(evar);
}

void Effects::CreateSquare(int Sxpos, int Sypos, int Expos, int Eypos,
                           sf::Color MainColor, float Outline,
                           sf::Color SecondaryColor)
{
    cSquare evar;
    evar.StartPos = sf::Vector2f(Sxpos, Sypos);
    evar.EndPos = sf::Vector2f(Expos, Eypos);
    evar.outline = Outline;
    evar.maincolor = MainColor;
    evar.seccolor = SecondaryColor;
    Squares.push_back(evar);
}

void Effects::CreateCircle(int xpos, int ypos, int Size, sf::Color MainColor,
                           float Outline, sf::Color SecondaryColor)
{
    cCircle evar;
    evar.Pos = sf::Vector2f(xpos, ypos);
    evar.size = Size;
    evar.outline = Outline;
    evar.maincolor = MainColor;
    evar.seccolor = SecondaryColor;
    Circles.push_back(evar);
}

void Effects::DrawEffects()
{
    {
        int remover = 0;
        for (auto &elem : Circles)
        {
            elem.DrawMe();
            remover++;
            //if(effectz->fading == true && effectz->maincolor.a <= 0){removerz = remover;}
        }
        Circles.clear();
    }

    for (auto &elem : Squares)
    {
        elem.DrawMe();
    }
    Squares.clear();

    for (auto &elem : Lines)
    {
        elem.DrawMe();
    }
    Lines.clear();

    for (auto &elem : Beams)
    {
        elem.DrawMe();
    }
    Beams.clear();
}
