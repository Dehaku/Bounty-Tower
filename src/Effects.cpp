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

    int length =
        sqrt(pow((endPos.x - startPos.x), 2) + pow((endPos.y - startPos.y), 2));
    rectangle.setSize(sf::Vector2f(length, size));
    //rectangle.setSize(sf::Vector2f(StartPos.x-EndPos.x,StartPos.y-EndPos.y));
    float angle =
        57.3065f * atan2(endPos.y - startPos.y, endPos.x - startPos.x);
    rectangle.setRotation(angle);

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
    sf::CircleShape circle;
    circle.setRadius(size);
    circle.setFillColor(maincolor);
    circle.setOutlineThickness(outline);
    circle.setOutlineColor(seccolor);
    circle.setPosition(pos.x, pos.y);
    circle.setOrigin(size, size);
    window.draw(circle);
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
    sf::Vertex line[2] = {startPos, endPos};
    //Line.setFillColor(maincolor);
    //maincolor.a -= faderate;
    //if(maincolor.a < 0){maincolor.a = 0;}
    window.draw(line, 2, sf::Lines);

    sf::RectangleShape rectangle;

    int length =
        sqrt(pow((endPos.x - startPos.x), 2) + pow((endPos.y - startPos.y), 2));
    rectangle.setSize(sf::Vector2f(length, size));
    //rectangle.setSize(sf::Vector2f(StartPos.x-EndPos.x,StartPos.y-EndPos.y));
    float angle =
        57.3065f * atan2(endPos.y - startPos.y, endPos.x - startPos.x);
    rectangle.setRotation(angle);

    rectangle.setOrigin(0, size / 2);

    rectangle.setFillColor(maincolor);
    rectangle.setOutlineColor(seccolor);
    rectangle.setOutlineThickness(outline);
    rectangle.setPosition(startPos.x, startPos.y);
    window.draw(rectangle);
}

void Effects::createLine(int sxpos, int sypos, int expos, int eypos, int size,
                         sf::Color mainColor, float outline,
                         sf::Color secondaryColor, bool fades, int fadesrate)
{
    Line evar;
    evar.startPos = sf::Vector2f(sxpos, sypos);
    evar.endPos = sf::Vector2f(expos, eypos);
    evar.size = size;
    evar.fading = fades;
    evar.faderate = fadesrate;
    evar.outline = outline;
    evar.maincolor = mainColor;
    evar.seccolor = secondaryColor;
    lines.push_back(evar);
}

void Effects::createBeam(int sxpos, int sypos, int expos, int eypos, int size,
                         sf::Color mainColor, float outline,
                         sf::Color secondaryColor, bool fades, int fadesrate)
{
    Beam evar;
    evar.startPos = sf::Vector2f(sxpos, sypos);
    evar.endPos = sf::Vector2f(expos, eypos);
    evar.size = size;
    evar.fading = fades;
    evar.faderate = fadesrate;
    evar.outline = outline;
    evar.maincolor = mainColor;
    evar.seccolor = secondaryColor;
    beams.push_back(evar);
}

void Effects::createSquare(int sxpos, int sypos, int expos, int eypos,
                           sf::Color mainColor, float outline,
                           sf::Color secondaryColor)
{
    Square evar;
    evar.startPos = sf::Vector2f(sxpos, sypos);
    evar.endPos = sf::Vector2f(expos, eypos);
    evar.outline = outline;
    evar.maincolor = mainColor;
    evar.seccolor = secondaryColor;
    squares.push_back(evar);
}

void Effects::createCircle(int xpos, int ypos, int size, sf::Color mainColor,
                           float outline, sf::Color secondaryColor)
{
    Circle evar;
    evar.pos = sf::Vector2f(xpos, ypos);
    evar.size = size;
    evar.outline = outline;
    evar.maincolor = mainColor;
    evar.seccolor = secondaryColor;
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
    for (auto &elem : polygons)
    {
        window.draw(elem);
    }
    polygons.clear();
}
