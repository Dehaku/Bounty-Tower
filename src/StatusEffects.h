#ifndef STATUSEFFECTS_H_INCLUDED
#define STATUSEFFECTS_H_INCLUDED

#include <string>
#include <SFML/Graphics.hpp>

class StatusComponent
{
public:

};

class StatusEffect
{
public:
    std::string name;
    int duration;
    int potency;

    sf::Color color;
    sf::Texture * icon;
    sf::Texture * critterEffect;

    bool toDelete;

    StatusEffect();
};

#endif // STATUSEFFECTS_H_INCLUDED
