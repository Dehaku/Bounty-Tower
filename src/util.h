#ifndef __UTIL_H_INCLUDED__
#define __UTIL_H_INCLUDED__

#include <string>
#include <vector>
#include <set>
#include <chrono>
#include <iostream>

#include <SFML/System.hpp>

void fSleep(float time);

template <typename T> void con(T arg, bool endLine = true)
{
    std::cout << arg;
    if (endLine)
    {
        std::cout << std::endl;
    }
}

bool booleanize(int num);
bool inbetween(float first, float second, float number);
int randz(int minValue, int maxValue);
int random(int minValue, int maxValue);
bool aabb(int pointx, int pointy, int left, int right, int up, int down);
bool aabb(sf::Vector2f point, int left, int right, int up, int down);
bool aabb(sf::Vector2i point, int left, int right, int up, int down);

/// Get the absolute of a numeric value,
/// converted to size_t, suitable for indexing
template <typename T> size_t abs_to_index(T value)
{
    return static_cast<size_t>(std::abs(value));
}

int stringFindNumber(std::string stringy, std::string term);
std::string stringFindString(std::string stringy, std::string term);
std::string stringFindStringNpos(std::string stringy, std::string term);
std::vector<std::string> stringFindElements(std::string source,
                                            std::string seperater = ":");
std::vector<float> numericSplitter(float received, float splitVariable = 100);
float percentageBuff(float received);
std::string stringFindChaos(std::string stringy, std::string term,
                            std::string ending);
std::set<std::string> stringFindSetChaos(std::string stringy, std::string term,
                                         std::string ending);
std::vector<std::string> stringFindVectorChaos(std::string stringy, std::string term,
                                         std::string ending);
float percentIs(float value, float percentage);

std::string generateName(int minLength = 2, int maxLength = 10);
bool toggle(bool &boolean);

#endif // __UTIL_H_INCLUDED__
