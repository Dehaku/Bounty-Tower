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

    sf::View oldView = window.getView();
    window.setView(drawView);
    window.draw(rectangle);
    window.setView(oldView);
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

    sf::View oldView = window.getView();
    window.setView(drawView);
    window.draw(circle);
    window.setView(oldView);
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

    sf::View oldView = window.getView();
    window.setView(drawView);
    window.draw(rectangle);
    window.setView(oldView);
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
                         sf::Color secondaryColor, sf::View drawView, bool fades, int fadesrate)
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
    evar.drawView = drawView;
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
                           sf::Color secondaryColor, sf::View drawView)
{
    Square evar;
    evar.startPos = sf::Vector2f(sxpos, sypos);
    evar.endPos = sf::Vector2f(expos, eypos);
    evar.outline = outline;
    evar.maincolor = mainColor;
    evar.seccolor = secondaryColor;
    evar.drawView = drawView;
    squares.push_back(evar);
}

void Effects::createCircle(int xpos, int ypos, int size, sf::Color mainColor,
                           float outline, sf::Color secondaryColor, sf::View drawView)
{
    Circle evar;
    evar.pos = sf::Vector2f(xpos, ypos);
    evar.size = size;
    evar.outline = outline;
    evar.maincolor = mainColor;
    evar.seccolor = secondaryColor;
    evar.drawView = drawView;
    circles.push_back(evar);
}

void Effects::createCone(sf::Vector2f vPos, int angle, int angleRadius, int depth, sf::Color color)
{

    const int endAngle = -(angle - (angleRadius / 2));
    const int startAngle = -(angle - (-angleRadius / 2));

    /*Drawing Vision*/
    sf::ConvexShape shape;
    int pointCounter = 1;
    shape.setPointCount(pointCounter);
    shape.setPoint(0, sf::Vector2f(0, 0));
    for (int rot = startAngle; rot != endAngle; rot++)
    {
        float xPos = vPos.x + sinf(rot * PI / 180) * depth;
        float yPos = vPos.y + cosf(rot * PI / 180) * depth;


        /* Placing the points to make the visible vision visibility visible */
        if (rot == startAngle)
        {
            pointCounter++;
            shape.setPointCount(pointCounter);
            shape.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - vPos.x, yPos - vPos.y));
        }

        int CenterPoint = (startAngle + endAngle) / 2;
        if (rot == CenterPoint)
        {
            pointCounter++;
            shape.setPointCount(pointCounter);
            shape.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - vPos.x, yPos - vPos.y));

        }
        if ((rot % 10) == 0 && rot != 0)
        {
            pointCounter++;
            shape.setPointCount(pointCounter);
            shape.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - vPos.x, yPos - vPos.y));
        }
        if (rot == endAngle - 1)
        {
            pointCounter++;
            shape.setPointCount(pointCounter);
            shape.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - vPos.x, yPos - vPos.y));
        }
    }

    //sf::Color filling(color);
    //filling.a = (50);
    shape.setFillColor(color);
    shape.setOutlineColor(color);
    shape.setOutlineThickness(1);
    shape.setPosition(vPos);
    effects.polygons.push_back(shape);

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

int orbRot = 0;
int orbs = 1;
int orbSpeed = 2;

Orb::Orb()
{
    orbRot = 0;
    orbs = 1;
    orbSpeed = 2;
    lifeTime = 0;
    toDelete = false;
    type = 0;
}

void Orb::updateOrb()
    {
        if(lifeTime <= 0)
            toDelete = true;
        lifeTime--;

        orbRot += orbSpeed;
    }

void Orb::drawOrb(int totalOrbs = 1)
    {

        int orbSpacing = 360/totalOrbs;
        if(type == 0)
        {
            sf::Vector2f Pos = math::angleCalc(pos,orbRot,orbDistance);
            effects.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Cyan);
            //textList.createText(Pos.x,Pos.y,10,sf::Color::White,std::to_string(lifeTime) + "/" + std::to_string(toDelete));
        }
        else if(type == 1)
        {
            sf::Vector2f Pos = math::angleCalc(pos,orbRot,(orbSpacing % 360));
            effects.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Red);
        }
        else if(type == 2)
        {
            sf::Vector2f Pos;
            if(orbRot < 360)
                Pos = math::angleCalc(pos,orbRot,(-(orbRot-180) + 180));
            if(orbRot < 270)
                Pos = math::angleCalc(pos,orbRot,(orbRot-180));
            if(orbRot < 180)
                Pos = math::angleCalc(pos,orbRot,(-orbRot + 180));
            if(orbRot < 90)
                Pos = math::angleCalc(pos,orbRot,(orbRot));
            effects.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Yellow);
        }
        else if(type == 3)
        {
            sf::Vector2f Pos = math::angleCalc(pos,orbRot,(sin(orbRot * PI / 180) * 45) + (cos(orbRot * PI / 180)*45) );
            effects.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Green);
        }

    }


std::vector<EffectStorer> effectsStorage;

EffectStorer::EffectStorer()
{
    size = 0;
    outline = 0;
    maincolor = sf::Color::Transparent;
    seccolor = sf::Color::Transparent;
    drawView = window.getView();

    fades = false;
    duration = 1;
    toDelete = false;
}

EffectStorer::EffectStorer(std::string Effect, int Duration, sf::Vector2f StartPos)
{
    effect = Effect;
    duration = Duration;
    startPos = StartPos;

    size = 0;
    outline = 0;
    maincolor = sf::Color::Transparent;
    seccolor = sf::Color::Transparent;
    drawView = window.getView();

    fades = false;
    toDelete = false;
}

void drawStoredEffects()
{
    for(auto &effect : effectsStorage)
    {
        if(effect.effect == "Circle")
        {
            Effects::Circle evar;
            evar.pos = effect.startPos;
            evar.size = effect.size;
            evar.outline = effect.outline;
            evar.maincolor = effect.maincolor;
            evar.seccolor = effect.seccolor;
            evar.drawView = effect.drawView;

            if(effect.fades)
            {
                int alpha = 10 * effect.duration;
                evar.maincolor.a = math::clamp(alpha,0,255);
                evar.seccolor.a = evar.maincolor.a;
            }

            effects.circles.push_back(evar);
        }
        else if(effect.effect == "Square")
        {
            Effects::Square evar;
            evar.startPos = effect.startPos;
            evar.endPos = effect.endPos;
            evar.outline = effect.outline;
            evar.maincolor = effect.maincolor;
            evar.seccolor = effect.seccolor;
            evar.drawView = effect.drawView;

            if(effect.fades)
            {
                int alpha = 10 * effect.duration;
                evar.maincolor.a = math::clamp(alpha,0,255);
                evar.seccolor.a = evar.maincolor.a;
            }

            effects.squares.push_back(evar);
        }
        else if(effect.effect == "Line")
        {
            Effects::Line evar;
            evar.startPos = effect.startPos;
            evar.endPos = effect.endPos;
            evar.size = effect.size;
            evar.outline = effect.outline;
            evar.maincolor = effect.maincolor;
            evar.seccolor = effect.seccolor;
            evar.drawView = effect.drawView;

            if(effect.fades)
            {
                int alpha = 10 * effect.duration;
                evar.maincolor.a = math::clamp(alpha,0,255);
                evar.seccolor.a = evar.maincolor.a;
            }

            effects.lines.push_back(evar);
        }

        effect.duration--;
        if(effect.duration <= 0)
            effect.toDelete = true;
    }
}


Shape::Shape()
{
    shape = Circle;
    size = 0;
    outline = 0;
    maincolor = sf::Color::Transparent;
    seccolor = sf::Color::Transparent;
    drawView = window.getView();

    texture = nullptr;

    fades = false;
    duration = 0;
    toDelete = false;
}

Shapes shapes;

void Shapes::drawShapes()
{
    for(auto &shape : shapes)
    {
        sf::View oldView = window.getView();
        window.setView(shape.drawView);

        if(shape.shape == shape.Square)
        {
            sf::RectangleShape rectangle;
            rectangle.setSize( shape.startPos - shape.endPos);
            rectangle.setFillColor(shape.maincolor);
            rectangle.setOutlineColor(shape.seccolor);
            rectangle.setOutlineThickness(shape.outline);
            rectangle.setPosition(shape.endPos.x, shape.endPos.y);
            if(shape.texture != nullptr)
                rectangle.setTexture(shape.texture);

            window.draw(rectangle);
        }
        else if(shape.shape == shape.Circle)
        {
            sf::CircleShape circle;
            circle.setRadius(shape.size);
            circle.setFillColor(shape.maincolor);
            circle.setOutlineThickness(shape.outline);
            circle.setOutlineColor(shape.seccolor);
            circle.setPosition(shape.startPos.x, shape.startPos.y);
            circle.setOrigin(shape.size, shape.size);
            if(shape.texture != nullptr)
                circle.setTexture(shape.texture);

            window.draw(circle);
        }
        else if(shape.shape == shape.Line)
        {
            sf::RectangleShape rectangle;

            int length =
                sqrt(pow((shape.endPos.x - shape.startPos.x), 2) + pow((shape.endPos.y - shape.startPos.y), 2));
            rectangle.setSize(sf::Vector2f(length, shape.size));

            float angle =
                57.3065f * atan2(shape.endPos.y - shape.startPos.y, shape.endPos.x - shape.startPos.x);
            rectangle.setRotation(angle);

            rectangle.setOrigin(0, shape.size / 2);

            rectangle.setFillColor(shape.maincolor);
            rectangle.setOutlineColor(shape.seccolor);
            rectangle.setOutlineThickness(shape.outline);
            rectangle.setPosition(shape.startPos.x, shape.startPos.y);
            if(shape.texture != nullptr)
                rectangle.setTexture(shape.texture);

            window.draw(rectangle);
        }

        window.setView(oldView);
        shape.duration--;
        if(shape.duration <= 0)
            shape.toDelete = true;
    }
}

std::vector<Orb> Orbs;



void purtyOrbitals()
{
    orbRot += orbSpeed;
    if(orbRot >= 360)
        orbRot = 0;
    if(inputState.key[Key::I].time == 1)
        orbSpeed++;
    if(inputState.key[Key::K].time == 1)
        orbSpeed--;
    if(inputState.key[Key::L].time == 1)
        orbs++;
    if(inputState.key[Key::J].time == 1)
        orbs--;
    if(orbs < 1)
        orbs = 1;

    int orbSpacing = 360/orbs;
    for(int i = 0; i != orbs; i++)
    {
        sf::Vector2f Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),30);
        effects.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Cyan);
    }
    for(int i = 0; i != orbs; i++)
    {
        sf::Vector2f Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),(orbSpacing % 360));
        effects.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Red);
    }
    for(int i = 0; i != orbs; i++)
    {
        sf::Vector2f Pos;
        if(orbRot < 360)
            Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),(-(orbRot-180) + 180));
        if(orbRot < 270)
            Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),(orbRot-180));
        if(orbRot < 180)
            Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),(-orbRot + 180));
        if(orbRot < 90)
            Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),(orbRot));
        effects.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Yellow);
    }
    for(int i = 0; i != orbs; i++)
    {
        sf::Vector2f Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),(cos(orbRot * PI / 180) * 45) + (cos(orbRot * PI / 180)*45) );
        effects.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Green);
    }
    for(int i = 0; i != orbs; i++)
    {
        sf::Vector2f Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),30+(cos(orbRot * PI / 180)*15) );
        effects.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Blue);
    }
}



