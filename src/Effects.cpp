#include "Effects.h"

#include <cmath>

extern sf::RenderWindow window;

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
            shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Cyan);
            //textList.createText(Pos.x,Pos.y,10,sf::Color::White,std::to_string(lifeTime) + "/" + std::to_string(toDelete));
        }
        else if(type == 1)
        {
            sf::Vector2f Pos = math::angleCalc(pos,orbRot,(orbSpacing % 360));
            shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Red);
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
            shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Yellow);
        }
        else if(type == 3)
        {
            sf::Vector2f Pos = math::angleCalc(pos,orbRot,(sin(orbRot * PI / 180) * 45) + (cos(orbRot * PI / 180)*45) );
            shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Green);
        }

    }


Shape::Shape()
{
    shape = Circle;
    size = 0;
    outline = 0;
    maincolor = sf::Color::Transparent;
    seccolor = sf::Color::Transparent;
    //drawView = window.getView();
    drawView = &gvars::view1;

    texture = nullptr;

    fades = false;
    duration = 0;
    toDelete = false;
}

void Shapes::createLine(int sxpos, int sypos, int expos, int eypos, int size,
                         sf::Color mainColor, float outline,
                         sf::Color secondaryColor, sf::View * drawView)
{
    Shape evar;
    evar.shape = Shape::Line;
    evar.startPos = sf::Vector2f(sxpos, sypos);
    evar.endPos = sf::Vector2f(expos, eypos);
    evar.size = size;
    evar.outline = outline;
    evar.maincolor = mainColor;
    evar.seccolor = secondaryColor;
    evar.drawView = drawView;
    shapes.push_back(evar);
}

void Shapes::createSquare(int sxpos, int sypos, int expos, int eypos,
                           sf::Color mainColor, float outline,
                           sf::Color secondaryColor, sf::View * drawView)
{
    Shape evar;
    evar.shape = Shape::Square;
    evar.startPos = sf::Vector2f(sxpos, sypos);
    evar.endPos = sf::Vector2f(expos, eypos);
    evar.outline = outline;
    evar.maincolor = mainColor;
    evar.seccolor = secondaryColor;
    evar.drawView = drawView;
    shapes.push_back(evar);
}

void Shapes::createCircle(int xpos, int ypos, int size, sf::Color mainColor,
                           float outline, sf::Color secondaryColor, sf::View * drawView)
{
    Shape evar;
    evar.shape = Shape::Circle;
    evar.startPos = sf::Vector2f(xpos, ypos);
    evar.size = size;
    evar.outline = outline;
    evar.maincolor = mainColor;
    evar.seccolor = secondaryColor;
    evar.drawView = drawView;
    shapes.push_back(evar);
}

void Shapes::createCone(sf::Vector2f vPos, int angle, int angleRadius, int depth, sf::Color color)
{

    const int endAngle = -(angle - (angleRadius / 2));
    const int startAngle = -(angle - (-angleRadius / 2));

    /*Drawing Vision*/
    sf::ConvexShape cone;
    int pointCounter = 1;
    cone.setPointCount(pointCounter);
    cone.setPoint(0, sf::Vector2f(0, 0));
    for (int rot = startAngle; rot != endAngle; rot++)
    {
        float xPos = vPos.x + sinf(rot * PI / 180) * depth;
        float yPos = vPos.y + cosf(rot * PI / 180) * depth;


        /* Placing the points to make the visible vision visibility visible */
        if (rot == startAngle)
        {
            pointCounter++;
            cone.setPointCount(pointCounter);
            cone.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - vPos.x, yPos - vPos.y));
        }

        int CenterPoint = (startAngle + endAngle) / 2;
        if (rot == CenterPoint)
        {
            pointCounter++;
            cone.setPointCount(pointCounter);
            cone.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - vPos.x, yPos - vPos.y));

        }
        if ((rot % 10) == 0 && rot != 0)
        {
            pointCounter++;
            cone.setPointCount(pointCounter);
            cone.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - vPos.x, yPos - vPos.y));
        }
        if (rot == endAngle - 1)
        {
            pointCounter++;
            cone.setPointCount(pointCounter);
            cone.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - vPos.x, yPos - vPos.y));
        }
    }

    //sf::Color filling(color);
    //filling.a = (50);
    cone.setFillColor(color);
    cone.setOutlineColor(color);
    cone.setOutlineThickness(1);
    cone.setPosition(vPos);
    polygons.push_back(cone);

}

Shapes shapes;



void Shapes::drawShapes()
{
    for(auto &shape : shapes)
    {
        sf::View oldView = window.getView();
        window.setView(*shape.drawView);

        if(shape.shape == shape.Square)
        {
            sf::RectangleShape rectangle;
            rectangle.setSize( shape.startPos - shape.endPos);
            rectangle.setFillColor(shape.maincolor);
            rectangle.setOutlineColor(shape.seccolor);
            rectangle.setOutlineThickness(shape.outline);
            rectangle.setPosition(shape.endPos.x, shape.endPos.y);

            if(shape.fades)
            {
                int alpha = 10 * shape.duration;
                shape.maincolor.a = math::clamp(alpha,0,255);
                shape.seccolor.a = shape.maincolor.a;
            }

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

            if(shape.fades)
            {
                int alpha = 10 * shape.duration;
                shape.maincolor.a = math::clamp(alpha,0,255);
                shape.seccolor.a = shape.maincolor.a;
            }

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

            if(shape.fades)
            {
                int alpha = 10 * shape.duration;
                shape.maincolor.a = math::clamp(alpha,0,255);
                shape.seccolor.a = shape.maincolor.a;
            }

            if(shape.texture != nullptr)
                rectangle.setTexture(shape.texture);

            window.draw(rectangle);
        }

        window.setView(oldView);
        shape.duration--;
        if(shape.duration <= 0)
            shape.toDelete = true;
    }
    for (auto &elem : polygons)
    {
        window.draw(elem);
    }
    polygons.clear();
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
        shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Cyan);
    }
    for(int i = 0; i != orbs; i++)
    {
        sf::Vector2f Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),(orbSpacing % 360));
        shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Red);
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
        shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Yellow);
    }
    for(int i = 0; i != orbs; i++)
    {
        sf::Vector2f Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),(cos(orbRot * PI / 180) * 45) + (cos(orbRot * PI / 180)*45) );
        shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Green);
    }
    for(int i = 0; i != orbs; i++)
    {
        sf::Vector2f Pos = math::angleCalc(gvars::mousePos,orbRot+(orbSpacing*i),30+(cos(orbRot * PI / 180)*15) );
        shapes.createCircle(Pos.x,Pos.y,3,sf::Color::White,1,sf::Color::Blue);
    }
}



