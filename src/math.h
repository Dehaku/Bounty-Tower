#ifndef __MATH_H_INCLUDED__
#define __MATH_H_INCLUDED__

#include <SFML/System.hpp>

namespace math
{
int Closeish(int orix, int oriy, int tarx, int tary);
int Closeisha(int orix, int oriy, int tarx, int tary);
int DistanceTrace(int xa, int ya, int xb, int yb);
int Closeishxx(int orix, int oriy, int tarx, int tary);
int Clamp(int Value, int MinValue, int MaxValue);
float AngleBetweenVectors(sf::Vector2f a, sf::Vector2f b);
sf::Vector2f CircleRandz(int xpos, int ypos, int radius);
using Vec2f = sf::Vector2f;
}

#endif
