#ifndef STATUSEFFECTS_H_INCLUDED
#define STATUSEFFECTS_H_INCLUDED

#include <string>
#include <SFML/Graphics.hpp>

#include "util.h"
#include "Textures.h"

extern std::vector<std::string> aspectNum;
int getAspectNum(std::string aspect);

class StatusAspect
{
public:
    int name;
    std::string type;
    float potency;
    enum AspectTypes
    { // WARNING, THESE MUST BE IN THE SAME ORDER AS THE DEFINITIONS IN 'void insertAspects()' OR THE FILE WILL GIVE BROKEN EFFECTS.
        // Carrier = Entity/Critter/Item this aspect resides on.
        // Damages: Blunt, Pierce, Slash, Fire, Poison, Cold, Electricity, Energy(Positive/Negitive Inclusive)
        ConditionHealth, // Type: Above, Below
        // Potency acts as Percentage. This status effect will only apply if it's (Type) the carrier's health value.
        ConditionLife, // Type: Dead, Alive.
        // Potency, Simply acts as On/Off. This status effect will only apply if the carrier is (Type).
        ConditionMana, // Type: Above, Below
        // Potency acts as Percentage. This status effect will only apply if it's (Type) the carrier's mana value.
        ConditionOnItemUse, // Type: Type of item used.
        // Potency, Simply acts as an On/Off
        ConditionOnDeath, //
        // Potency, Simply acts as an On/Off
        ConditionOnHit, //
        // Potency, Simply acts as an On/Off
        ConditionOnHitByType, // Type: Damages
        // Potency, Simply acts as an On/Off
        ConditionReceivedDamage, // Type: Damages. (Leave Blank for All)
        // Potency, Acts as health, Once it hits 0, this condition is true.
        ConditionTimeDelay, //
        // Potency, The amount of frames must pass before this Condition is true.
        ConditionNearbyUnit, // Type: Ally, Enemy, Neutral.
        // Potency defines the range they must be within.
        ConditionUnitCount, // Type: Ally, Enemy, Neutral.
        // Potency defines the amount of units that must exist of this type.
        ActionSpeed, // Type: Attack, Cast, Reload, Switch Work.
        // Potency acts as Percentage.
        AffectDamage, // Type: Damages
        // Potency acts as Percentage.
        AffectHealth, // Type: Damages
        // Potency heals in the positives, harms in the negitives.
        Armor, // Type: Damages
        // Potency acts as Percentage. (Reduces incoming damage.)
        Freeze,
        // Disables all actions, Potency acts as health for this Aspect.
        MoveSpeed,
        // Potency directly applies to variable.
        Sleep,
        // Disables all actions, Potency acts as health for this Aspect.
        Stun,
        // Disables Item Usage. (This includes attacking/casting)
        ManaRegen,
        // Potency acts as Percentage.
        ManaBurn,
        // Potency directly applies to variable.
        Attribute, // Type: Critter Attributes.
        // Potency directly applies to variable.
        Thorns, // Type: Damages.
        // Deals damage to those who Melee the Carrier., Potency is damage amount, Type sets damage type.
        AmmoCost, // Type: Ammo Type.
        // Potency acts as percentage, For every 100%, Automatically costs one bullet, Remainder is random chance.
        Accuracy,
        // Potency directly applies to variable. Item Spread, in this case.
        Immunity, // Type: Damages, Momentum.
        // Potency, Simply acts as an On/Off
        StatusEffectImmunity, // Type: Status Effect Name.
        // Potency, Simply acts as an On/Off.
        ApplyMomentum, // Only works as Aura or on Weapon.
        // Potency pushes when positive, pulls when negitive.
        MomentumSensitivity,
        // Potency acts as Percentage.
        CauseExplosion, // Type: Item Type. // should be turned into a conditional somehow.
        // Potency is damage applied. This hurts everyone, Even Carrier.
        AffectVisionRange,
        // Potency directly applies to variable.
        ItemFind,
        // Potency acts as Percentage.
        ItemDropRate,
        // Potency acts as Percentage.
        XPModifier, // This is applied to all XP the carrier receives.
        // Potency acts as Percentage.
        DeathXPModifier, // This is applied to the XP given out when the Carrier dies.
        // Potency acts as Percentage.
        Mark, // Type: (Phoenix, Dragon, Monkey, Eagle, Mole)
        // Potency, Simply acts as an On/Off.
        SpawnCreature, // Type: Direct Race Name. Creatures will be hostile to Everyone.
        // Potency ???
        SpawnItem, // Type: Direct Item Name.
        // Potency indicates amount of stack to spawn, Does not over rule Stacksize.
        Revive,
        // Potency acts as a lives counter.
        DisableDeath,
        // Potency, Simply acts as an On/Off.
        AutoDodge, // Type: Damages. This aspect causes the carrier to automatically avoid a source of damage equal to it's type.
        // Potency, Acts as a counter, 5 Potency, Five automatic dodges.
        ChangeRace // Type: Direct Race Name.
        // Potency, Acts as a duration, If set to -1, It lasts for the duration of the status effect.




    };

    bool toDelete;
};

class StatusEffect
{
public:
    std::string name;
    std::string rank;
    int duration;

    int auraRadius; // This, When above 0, Doesn't affect the carrier, but applies it to everyone within it's radius, for 1 duration, and with 0 auraRadius.
    bool auraAffectsAllies;
    bool auraAffectsEnemies;
    bool auraAffectsNeutrals;


    sf::Color color;
    sf::Texture const * icon;
    sf::Texture const * critterEffect;

    std::vector<StatusAspect> aspects;
    void addAspect(int name, int potency, std::string type = "");

    bool toDelete;

    StatusEffect();
};

class GlobalStatusEffects
{
public:
    std::vector<StatusEffect> statusEffects;

    StatusEffect getStatusEffect(std::string statusEffect);

    void initializeStatusEffects();
};

extern GlobalStatusEffects globalStatusEffects;

#endif // STATUSEFFECTS_H_INCLUDED
