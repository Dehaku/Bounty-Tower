#ifndef __DEFS_H_INCLUDED__
#define __DEFS_H_INCLUDED__

#include <SFML/Graphics.hpp>

//const sf::Vector2f RESOLUTION(1024, 768);
const sf::Vector2f RESOLUTION(1280, 720);
const sf::Vector2f CENTER(500, 500);
const sf::Vector2f HALF_SIZE(RESOLUTION.x / 2, RESOLUTION.y / 2);
const int GRID_X = 100;
const int GRID_Y = 100;
const int GRID_Z = 10;
const int GRID_SIZE = 60;
const int TILE_PIXELS = 20;
const sf::Color BROWN(100, 100, 0, 255);

struct Desire
{
    std::string type;
    float potency;
};

struct Vec3
{
    int x;
    int y;
    int z;
    Vec3(int First = 0, int Second = 0, int Third = 0)
    {
        x = First;
        y = Second;
        z = Third;
    }

    operator == (Vec3 a)
    {
      if(a.x == x && a.y == y && a.z == z)
         return true;
      else
         return false;
    }

    operator != (Vec3 a)
    {
      if(a.x != x || a.y != y || a.z != z)
         return true;
      else
         return false;
    }

    };

struct Vec3f
{
    float x;
    float y;
    float z;
    Vec3f(float First = 0, float Second = 0, float Third = 0)
    {
        x = First;
        y = Second;
        z = Third;
    }
    Vec3f(Vec3 altVec)
    {
        x = altVec.x;
        y = altVec.y;
        z = altVec.z;
    }

};

#endif // __DEFS_H_INCLUDED__
