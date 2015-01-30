#include "math.h"

#include "util.h"

int math::Closeish(int orix, int oriy, int tarx, int tary)
{
    int Powa = sqrt(pow((tarx - orix), 2) + pow((tary - oriy), 2));
    return Powa;
}

int math::Closeisha(int orix, int oriy, int tarx, int tary)
{ // Original
    int xnum;
    int ynum;
    xnum = orix - tarx;
    if (xnum < 0)
    {
        xnum -= xnum * 2;
    }
    ynum = oriy - tary;
    if (ynum < 0)
    {
        ynum -= ynum * 2;
    }
    int num = xnum + ynum;
    return num;
}

int math::DistanceTrace(int xa, int ya, int xb, int yb)
{ // Highly inaccurate
    int dx = xb - xa, dy = yb - ya, steps;
    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);
    int distance = 0;

    for (int k = 0; k < steps; k++)
    {
        distance++;
    }
    return distance;
}

int math::Closeishxx(int orix, int oriy, int tarx, int tary)
{ // Returns an octogonish type ring of detection, 10% overshooting.
    int Sqr = DistanceTrace(orix, oriy, tarx, tary);
    int Dia = Closeisha(orix, oriy, tarx, tary);
    int Powa = (Sqr + Dia) / 2;
    return Powa;
}

int math::Clamp(int Value, int MinValue, int MaxValue)
{
    if (Value > MaxValue)
    {
        return MaxValue;
    }
    if (Value < MinValue)
    {
        return MinValue;
    }
    else
    {
        return Value;
    }
}

float math::AngleBetweenVectors(sf::Vector2f a, sf::Vector2f b)
{
    //  (180 / PI = 57.3065)
    //return 114.5915f * atan2(b.y - a.y, b.x - a.x);
    return 57.3065f * atan2(b.y - a.y, b.x - a.x);
}

sf::Vector2f math::CircleRandz(int xpos, int ypos, int radius)
{
    float Xxx = 0;      // global
    float Yyy = 0;      // global
    float speeds = 1;   // global
    int xanchor = xpos; // global
    int yanchor = ypos; // global
    float Degrees = 0;
    int Ran = randz(0, 360);
    for (int Turns = -1; Turns != Ran; Turns++)
    {
        Xxx = xanchor + cosf(Degrees * PI / 180) * radius;
        Yyy = yanchor + sinf(Degrees * PI / 180) * radius;
        // Should probably leave speed at 1, If not less.
        Degrees += speeds;
    }
    return sf::Vector2f(Xxx, Yyy);
}
