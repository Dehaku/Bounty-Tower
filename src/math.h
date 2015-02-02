#ifndef __MATH_H_INCLUDED__
#define __MATH_H_INCLUDED__

#include <SFML/System.hpp>

const double PI = 3.141;
const double G = 6.67;

namespace math
{
int closeish(int orix, int oriy, int tarx, int tary);
int closeisha(int orix, int oriy, int tarx, int tary);
int distanceTrace(int xa, int ya, int xb, int yb);
int closeishxx(int orix, int oriy, int tarx, int tary);
int closeishS(int orix, int tarx);
int clamp(int Value, int MinValue, int MaxValue);
float angleBetweenVectors(sf::Vector2f a, sf::Vector2f b);
sf::Vector2f circleRandz(int xpos, int ypos, int radius);
using Vec2f = sf::Vector2f;
}

#endif
