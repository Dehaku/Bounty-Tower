#ifndef __UTIL_H_INCLUDED__
#define __UTIL_H_INCLUDED__

#include <string>
#include <vector>
#include <set>
#include <iostream>

#include <SFML/System.hpp>

void fSleep(float Time);

template <typename T> void con(T arg, bool endLine = true)
{
    std::cout << arg;
    if (endLine)
    {
        std::cout << std::endl;
    }
}

bool booleanize(int Num);
bool inbetween(float First, float Second, float Number);
int randz(int minValue, int maxValue);
bool aabb(int Pointx, int Pointy, int Left, int Right, int Up, int Down);
bool aabb(sf::Vector2f Point, int Left, int Right, int Up, int Down);
bool aabb(sf::Vector2i Point, int Left, int Right, int Up, int Down);

/// Get the absolute of a numeric value,
/// converted to size_t, suitable for indexing
template <typename T> size_t abs_to_index(T value)
{
    return static_cast<size_t>(std::abs(value));
}

int stringFindNumber(std::string Stringy, std::string Term);
std::string stringFindString(std::string Stringy, std::string Term);
std::string stringFindStringNpos(std::string Stringy, std::string Term);
std::vector<std::string> stringFindElements(std::string Source,
                                            std::string Seperater = ":");
std::vector<float> numericSplitter(float Received, float SplitVariable = 100);
float percentageBuff(float Received);
std::string stringFindChaos(std::string Stringy, std::string Term,
                            std::string Ending);
std::set<std::string> stringFindSetChaos(std::string Stringy, std::string Term,
                                         std::string Ending);
float percentIs(float Value, float Percentage);

std::string generateName(int MinLength = 2, int MaxLength = 10);
bool toggle(bool &Boolean);

#endif // __UTIL_H_INCLUDED__
