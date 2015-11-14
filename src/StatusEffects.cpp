
#include "StatusEffects.h"

#include <sstream>
#include <fstream>
#include <cstring>
#include <iostream>

std::vector<std::string> aspectNum;

GlobalStatusEffects globalStatusEffects;

void insertAspects()
{ // WARNING, THESE MUST BE IN THE SAME ORDER AS THE DEFINITIONS IN 'enum AspectTypes' OR THE FILE WILL GIVE BROKEN EFFECTS.
    std::string aspectName;
    aspectName = "ConditionHealth";
    aspectNum.push_back(aspectName);
    aspectName = "ConditionLife";
    aspectNum.push_back(aspectName);
    aspectName = "ConditionMana";
    aspectNum.push_back(aspectName);

    aspectName = "ConditionOnItemUse";
    aspectNum.push_back(aspectName);
    aspectName = "ConditionOnDeath";
    aspectNum.push_back(aspectName);
    aspectName = "ConditionOnHit";
    aspectNum.push_back(aspectName);
    aspectName = "ConditionOnHitByType";
    aspectNum.push_back(aspectName);
    aspectName = "ConditionReceivedDamage";
    aspectNum.push_back(aspectName);
    aspectName = "ConditionTimeDelay";
    aspectNum.push_back(aspectName);

    aspectName = "ConditionNearbyUnit";
    aspectNum.push_back(aspectName);
    aspectName = "ConditionUnitCount";
    aspectNum.push_back(aspectName);
    aspectName = "ActionSpeed";
    aspectNum.push_back(aspectName);
    aspectName = "AffectDamage";
    aspectNum.push_back(aspectName);
    aspectName = "AffectHealth";
    aspectNum.push_back(aspectName);
    aspectName = "Armor";
    aspectNum.push_back(aspectName);
    aspectName = "Freeze";
    aspectNum.push_back(aspectName);
    aspectName = "MoveSpeed";
    aspectNum.push_back(aspectName);
    aspectName = "Sleep";
    aspectNum.push_back(aspectName);
    aspectName = "Stun";
    aspectNum.push_back(aspectName);
    aspectName = "ManaRegen";
    aspectNum.push_back(aspectName);
    aspectName = "ManaBurn";
    aspectNum.push_back(aspectName);
    aspectName = "Attribute";
    aspectNum.push_back(aspectName);

    aspectName = "Thorns";
    aspectNum.push_back(aspectName);
    aspectName = "AmmoCost";
    aspectNum.push_back(aspectName);
    aspectName = "Accuracy";
    aspectNum.push_back(aspectName);
    aspectName = "Immunity";
    aspectNum.push_back(aspectName);
    aspectName = "StatusEffectImmunity";
    aspectNum.push_back(aspectName);
    aspectName = "ApplyMomentum";
    aspectNum.push_back(aspectName);
    aspectName = "MomentumSensitivity";
    aspectNum.push_back(aspectName);
    aspectName = "CauseExplosion";
    aspectNum.push_back(aspectName);
    aspectName = "AffectVisionRange";
    aspectNum.push_back(aspectName);
    aspectName = "ItemFind";
    aspectNum.push_back(aspectName);
    aspectName = "ItemDropRate";
    aspectNum.push_back(aspectName);
    aspectName = "XPModifier";
    aspectNum.push_back(aspectName);
    aspectName = "DeathXPModifier";
    aspectNum.push_back(aspectName);
    aspectName = "Mark";
    aspectNum.push_back(aspectName);
    aspectName = "SpawnCreature";
    aspectNum.push_back(aspectName);
    aspectName = "SpawnItem";
    aspectNum.push_back(aspectName);
    aspectName = "Revive";
    aspectNum.push_back(aspectName);
    aspectName = "DisableDeath";
    aspectNum.push_back(aspectName);
    aspectName = "AutoDodge";
    aspectNum.push_back(aspectName);
    aspectName = "ChangeRace";
    aspectNum.push_back(aspectName);
}

int getAspectNum(std::string aspect)
{
    for(int i = 0; i != aspectNum.size(); i++)
    {
        if(aspectNum[i] == aspect)
            return i;
    }
    return 0;
}


void GlobalStatusEffects::initializeStatusEffects()
{
    insertAspects();

    std::cout << "Adding global status effects from file.\n";
    std::ifstream input("data/statusEffects.txt");
    if (!input.is_open())
        return;

    while (input.good())
    {
        std::string line;
        getline(input, line);
        StatusEffect status;

        status.critterEffect = nullptr;
        status.icon = nullptr;

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
            for (auto &image : texturemanager.textures)
                if (image.name == overlayName)
                    status.critterEffect = &image.texture;

        std::vector<std::string> aspects = stringFindVectorChaos(line,"[Aspect:","]");

        for(auto &aspect : aspects)
        {
            std::cout << "Found: " << aspect << std::endl;
            std::vector<std::string> components = stringFindElements(aspect,":");
            for(int i = 0; i != components.size(); i++)
            {
                if(i == 0)
                    std::cout << "Name: " << components[i] << std::endl;
                if(i == 1)
                    std::cout << "Potency: " << components[i] << std::endl;
                if(i == 2)
                    std::cout << "Type: " << components[i] << std::endl;
            }

            StatusAspect SA;
            for(int i = 0; i != components.size(); i++)
            {
                if(i == 0)
                {
                    for(int t = 0; t != aspectNum.size(); t++)
                    {
                        if(aspectNum[t] != components[i])
                            continue;
                        SA.name = t;
                        break;
                    }
                }
                if(i == 1)
                    SA.potency = std::stoi(components[i]);
                if(i == 2)
                    SA.type = components[i];
            }

            status.aspects.push_back(SA);
            std::cout << "New Aspect: " << aspectNum[SA.name] << ": " << SA.name << ", Pot: " << SA.potency << ", Type: " << SA.type << std::endl;

        }






        std::cout << "Adding Status Effect: " << status.name << std::endl;
        statusEffects.push_back(status);
    }
}

StatusEffect GlobalStatusEffects::getStatusEffect(std::string statusEffect)
{
    for(auto &status : statusEffects)
        if(status.name == statusEffect)
            return status;

    throw std::runtime_error("There is no " + statusEffect + ".");
}

StatusEffect::StatusEffect()
{
    critterEffect = nullptr;
    icon = nullptr;

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


