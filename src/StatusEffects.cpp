
#include "StatusEffects.h"

#include <sstream>
#include <fstream>
#include <cstring>
#include <iostream>

GlobalStatusEffects globalStatusEffects;

void GlobalStatusEffects::initializeStatusEffects()
{
    std::cout << "Adding global status effects from file.\n";
    std::ifstream input("data/statusEffects.txt");
    if (!input.is_open())
        return;

    while (input.good())
    {
        std::string line;
        getline(input, line);
        StatusEffect status;
        status.name = "Debuggery";
        std::string statusName = stringFindString(line, "[Name:");
        if(statusName == "")
        { // This is an invalid line, Next!
            std::cout << "This line doesn't have a name! Invalid. \n";
            continue;
        }

        status.name = statusName;

        status.duration = stringFindNumber(line, "[Duration:");

        status.auraRadius = stringFindNumber(line, "[AuraRadius:");
        status.auraAffectsAllies = booleanize(stringFindNumber(line, "[AuraAllies:"));
        status.auraAffectsEnemies = booleanize(stringFindNumber(line, "[AuraEnemies:"));
        status.auraAffectsNeutrals = booleanize(stringFindNumber(line, "[AuraNeutrals:"));

        std::string iconName = stringFindString(line, "[IconImage:");
        if(iconName != "")
            for (auto const &image : texturemanager.textures)
                if (image.name == iconName)
                    status.icon = &image.texture;

        std::string overlayName = stringFindString(line, "[OverlayImage:");
        if(overlayName != "")
            for (auto const &image : texturemanager.textures)
                if (image.name == overlayName)
                    status.critterEffect = &image.texture;

        std::cout << "Adding Status Effect: " << status.name << std::endl;
        statusEffects.push_back(status);
    }
}

StatusEffect::StatusEffect()
{
    duration = 0;
    auraRadius = 0;

    auraAffectsAllies = false;
    auraAffectsEnemies = false;
    auraAffectsNeutrals = false;
    //potency = 0;

    toDelete = false;
}

void StatusEffect::addAspect(int name, int potency, std::string type)
{
    StatusAspect aspect;
    aspect.name = name;
    aspect.potency = potency;
    aspect.type = type;
    aspect.toDelete = false;

    aspects.push_back(aspect);
}


