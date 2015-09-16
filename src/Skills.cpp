#include "Skills.h"



Skills::Skills()
{
    Skill skill;
    skill.level = 1;
    skill.ranks = 0;
    skill.ranksmax = 5;
    skill.duration = 0;
    skill.cost = 1;
    skill.cooldownint = 30*10;
    skill.cooldown = 0;
    skill.autouse = true;
    skill.active = false;


    skill.tree = "Melee";

    skill.name = "Knockback";
    skill.desc = "Passive - 5% of damage is applied as force, per rank";
    skill.active = false;
    list.push_back(skill);

    /*
    skill.name = "Sunder";
    skill.desc = "Passive - 20% chance per rank to destroy an opponents held weapon, Max ranks causes the item to simply be dropped on the floor.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Backstab";
    skill.desc = "Passive - Deal 20% more damge per rank when striking an opponent from the back with a melee weapon.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Cleave";
    skill.desc = "Active - Activate to Swing a melee weapon for 75% damage in a 90 degree arc.";
    skill.active = true;
    list.push_back(skill);

    skill.name = "Onslaught";
    skill.desc = "Active - Activate to do a number of quick attacks equal to the ranks in this skill.";
    skill.active = true;
    list.push_back(skill);

    skill.name = "Hamstring";
    skill.desc = "Active - Perform an attack that immobalizes the target for ten seconds per rank.";
    skill.active = true;
    list.push_back(skill);

    skill.name = "Charge";
    skill.desc = "Active - Dashes in a straight line, damaging all enemies in the path with your weapon's damage, Knockback included.";
    skill.duration = 30*2;
    skill.active = true;
    // Dev Note: Use a momentum movement for simplicity, prevent damage on the charger using the Charge skill state.
    list.push_back(skill);
    skill.duration = 0;
    */


    skill.tree = "Ranger";

    /*
    skill.name = "Ricochet";
    skill.desc = "Passive - 20% additional chance per rank for projectiles to ricochet off of hard surfaces, Overflow spills into penetration value.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Penetration";
    skill.desc = "Passive - Each rank of this skill increases the penetration value of a projectile by 20%";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Lead Bullet";
    skill.desc = "Passive - Each bullet has a 20% chance per rank to poison an enemy for 5% of it's max health per second for ten seconds.";
    skill.active = false;
    list.push_back(skill);
    */

    skill.name = "Bronze Bullet";
    skill.desc = "Passive - The first bullet in a magazine deals 10% extra base damage per rank, stacks with other 'Bullet' skills if the position aligns.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Silver Bullet";
    skill.desc = "Passive - The middle-most bullet in a magazine deals 20% extra base damage per rank, stacks with other 'Bullet' skills if the position aligns.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Gold Bullet";
    skill.desc = "Passive - The last bullet in a magazine deals 30% extra base damage per rank, stacks with other 'Bullet' skills if the position aligns.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Miscounted Shot";
    skill.desc = "Passive - Each bullet has a 5% chance per rank to not consume any ammo.";
    skill.active = false;
    list.push_back(skill);

    /*
    skill.name = "Snipe Shot";
    skill.desc = "Active - Fires a single bullet with 50% additional Range and Damage per rank, When aiming, displays a line of travel, including richochet potential and penetration.";
    skill.active = true;
    list.push_back(skill);

    skill.name = "Short Burst";
    skill.desc = "Active - Firerate increased by 20% per rank for 3 seconds with perfect accuracy.";
    skill.active = true;
    list.push_back(skill);

    skill.name = "Long Burst";
    skill.desc = "Active - Firerate increased by 20% per rank for ten seconds, recoil is worsened by 50% for the duration.";
    skill.duration = 30*10;
    skill.active = true;
    list.push_back(skill);
    skill.duration = 0;
    */


    skill.tree = "Magic";

    /*
    skill.name = "Mana Regen";
    skill.desc = "Passive - Gain an additional 20% mana regeneration for your devices per rank.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Explosive Conjuration";
    skill.desc = "Passive - Gain a 10% chance for magical projectiles/attacks to explode per rank.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Explosive Richochet";
    skill.desc = "Passive - Gain a 60 radius for magical projectiles/attacks to explode per rank, each time they richochet.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Influencial Potential";
    skill.desc = "Passive - Gain a 5% chance for magical projectiles/attacks to charm minions per rank.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Elemental Atunement";
    skill.desc = "Passive - Gain 5% elemental resistence and chance against their status effects.";
    skill.active = false;
    list.push_back(skill);


    */


    skill.tree = "Engineer";

    /*
    skill.name = "Turret Construction";
    skill.desc = "Active - Allows construction of up to total ranks of turrets.";
    skill.active = true;
    skill.autouse = false;
    list.push_back(skill);
    */

    skill.name = "Lucky Scavenger";
    skill.desc = "Passive - 20% chance per rank to find money or ammo while picking up scrap.";
    skill.active = false;
    skill.autouse = true;
    list.push_back(skill);

    /*
    skill.name = "Lemme help with that";
    skill.desc = "Passive - 10% chance per rank to prevent allies item durability loss and prevent weapon jamming.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Explosive Deconstruction";
    skill.desc = "Passive - Upon constructions death, detonates with a scaled explosion based on constructions health and cost.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Practical Problems";
    skill.desc = "Passive - Doubles the weapon slots of the turret. Sometimes, You just need more gun.";
    skill.ranksmax = 1;
    skill.active = false;
    list.push_back(skill);
    skill.ranksmax = 5;

    skill.name = "Quality Built";
    skill.desc = "Passive - Increases construction base damage and range by 20% per rank.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Dynomo Construction";
    skill.desc = "Active - Increases build rate to 400% for ten seconds.";
    skill.active = true;
    list.push_back(skill);

    skill.name = "Turret Overdrive";
    skill.desc = "Active - Causes constructions to go into overdrive mode, dealing double damage for one second per rank.";
    skill.active = true;
    list.push_back(skill);
    */


    skill.tree = "Monster";

    /*
    skill.name = "Feral Weaponry";
    skill.desc = "Active - Causes creature to go berzerk, striking rapidly with claws, fangs, tail, dealing damage based on rank.";
    skill.active = true;
    list.push_back(skill);

    skill.name = "Feral Regeneration";
    skill.desc = "Passive - Creature gains 1% (total health per rank) health regeneration.";
    // Certain creatures will start with this at max ranks, like trolls.
    skill.active = false;
    list.push_back(skill);

    */
    skill.name = "Feral Body";
    skill.desc = "Passive - Creature gains 10% Damage Resistence per Rank";
    skill.active = false;
    list.push_back(skill);



    skill.tree = "Paragon";
    skill.ranksmax = 99999;

    /*
    skill.name = "Perfect Health";
    skill.desc = "Passive - Gain 10% base health per rank.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "True Beam";
    skill.desc = "Active - Fires a beam that cannot be dodged, dealing (All Attribute) Damage per Rank";
    skill.active = true;
    list.push_back(skill);
    */



}

int Skills::getRanks(std::string skillName)
{
    for(auto &skill : list)
    {
        if(skill.name == skillName)
            return skill.ranks;
    }
    return 0;
}
