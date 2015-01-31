#include "math.h"

#include "util.h"

int math::closeish(int orix, int oriy, int tarx, int tary)
{
    return sqrt(pow((tarx - orix), 2) + pow((tary - oriy), 2));
}

int math::closeisha(int orix, int oriy, int tarx, int tary)
{
    int xnum = orix - tarx;
    if (xnum < 0)
    {
        xnum -= xnum * 2;
    }
    int ynum = oriy - tary;
    if (ynum < 0)
    {
        ynum -= ynum * 2;
    }
    return xnum + ynum;
}

// NOTE: Highly inaccurate
int math::distanceTrace(int xa, int ya, int xb, int yb)
{
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

// Returns an octogonish type ring of detection, 10% overshooting.
int math::closeishxx(int orix, int oriy, int tarx, int tary)
{
    int Sqr = distanceTrace(orix, oriy, tarx, tary);
    int Dia = closeisha(orix, oriy, tarx, tary);
    int Powa = (Sqr + Dia) / 2;
    return Powa;
}

int math::CloseishS(int orix, int tarx)
{
    int xnum;
    xnum = orix - tarx;
    int num = xnum;
    if (num < 0)
    {
        num -= num * 2;
    }
    return num;
}

int math::clamp(int Value, int MinValue, int MaxValue)
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

float math::angleBetweenVectors(sf::Vector2f a, sf::Vector2f b)
{
    //  (180 / PI = 57.3065)
    //return 114.5915f * atan2(b.y - a.y, b.x - a.x);
    return 57.3065f * atan2(b.y - a.y, b.x - a.x);
}

sf::Vector2f math::circleRandz(int xpos, int ypos, int radius)
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
