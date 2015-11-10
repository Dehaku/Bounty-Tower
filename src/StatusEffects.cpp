
#include "StatusEffects.h"

StatusEffect::StatusEffect()
{
    duration = 0;
    AffectsCarrierFaction = false;
    AffectsCarrierEnemies = false;
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
