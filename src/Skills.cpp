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


    skill.tree = "Melee";

    skill.name = "Sunder";
    skill.desc = "Passive - 20% chance per rank to destroy an opponents held weapon, Max ranks causes the item to simply be dropped on the floor.";
    list.push_back(skill);

    skill.name = "Backstab";
    skill.desc = "Passive - Deal 20% more damge per rank when striking an opponent from the back with a melee weapon.";
    list.push_back(skill);

    skill.name = "Cleave";
    skill.desc = "Active - Activate to Swing a melee weapon for 75% damage in a 90 degree arc.";
    list.push_back(skill);

    skill.name = "Onslaught";
    skill.desc = "Active - Activate to do a number of quick attacks equal to the ranks in this skill.";
    list.push_back(skill);

    skill.name = "Hamstring";
    skill.desc = "Active - Perform an attack that immobalizes the target permanently.";
    list.push_back(skill);

    skill.name = "Charge";
    skill.desc = "Active - Dashes in a straight line, damaging all enemies in the path with your weapon's damage, Knockback included.";
    skill.duration = 30*2;
    // Dev Note: Use a momentum movement for simplicity, prevent damage on the charger using the Charge skill state.
    list.push_back(skill);
    skill.duration = 0;


    skill.tree = "Ranged";

    skill.name = "Ricochet";
    skill.desc = "Passive - 20% additional chance per rank for projectiles to ricochet off of hard surfaces, Overflow spills into penetration value.";
    list.push_back(skill);

    skill.name = "Penetration";
    skill.desc = "Passive - Each rank of this skill increases the penetration value of a projectile by 20%";
    list.push_back(skill);

    skill.name = "Copper Bullet";
    skill.desc = "Passive - The first bullet in a magazine deals 10% extra base damage per rank, stacks with other 'Bullet' skills if the position aligns.";
    list.push_back(skill);

    skill.name = "Silver Bullet";
    skill.desc = "Passive - The middle-most bullet in a magazine deals 20% extra base damage per rank, stacks with other 'Bullet' skills if the position aligns.";
    list.push_back(skill);

    skill.name = "Gold Bullet";
    skill.desc = "Passive - The last bullet in a magazine deals 30% extra base damage per rank, stacks with other 'Bullet' skills if the position aligns.";
    list.push_back(skill);

    skill.name = "Lead Bullet";
    skill.desc = "Passive - Each bullet has a 20% chance per rank to poison an enemy for 5% of it's max health per second for ten seconds.";
    list.push_back(skill);

    skill.name = "Miscounted Shot";
    skill.desc = "Passive - Each bullet has a 5% chance per rank to not consume any ammo.";
    list.push_back(skill);

    skill.name = "Snipe Shot";
    skill.desc = "Active - Fires a single bullet with 50% additional Range and Damage per rank, When aiming, displays a line of travel, including richochet potential and penetration.";
    list.push_back(skill);

    skill.name = "Short Burst";
    skill.desc = "Active - Firerate increased by 20% per rank for 3 seconds with perfect accuracy.";
    list.push_back(skill);

    skill.name = "Long Burst";
    skill.desc = "Active - Firerate increased by 20% per rank for ten seconds, recoil is worsened by 50% for the duration.";
    skill.duration = 30*10;
    list.push_back(skill);
    skill.duration = 0;


    skill.tree = "Magic";

    skill.name = "Mana Regen";
    skill.desc = "Passive - Gain an additional 20% mana regeneration for your devices per rank.";
    list.push_back(skill);

    skill.name = "Explosive Conjuration";
    skill.desc = "Passive - Gain a 10% chance for magical projectiles/attacks to explode per rank.";
    list.push_back(skill);

    skill.name = "Influencial Potential";
    skill.desc = "Passive - Gain a 5% chance for magical projectiles/attacks to charm minions per rank.";
    list.push_back(skill);


    skill.tree = "Engineering";

    skill.name = "Lemme help with that";
    skill.desc = "Passive - 10% chance per rank to prevent allies item durability loss and prevent weapon jamming.";
    list.push_back(skill);

    skill.name = "Lucky Scavenger";
    skill.desc = "Passive - 20% chance per rank to find money or ammo while picking up scrap.";
    list.push_back(skill);

    skill.name = "Explosive Deconstruction";
    skill.desc = "Passive - Upon constructions death, detonates with a scaled explosion based on constructions health and cost.";
    list.push_back(skill);

    skill.name = "Practical Problems";
    skill.desc = "Passive - Doubles the weapon slots of the turret. Sometimes, You just need more gun.";
    skill.ranksmax = 1;
    list.push_back(skill);
    skill.ranksmax = 5;

    skill.name = "Quality Built";
    skill.desc = "Passive - Increases construction base damage and range by 20% per rank.";
    list.push_back(skill);

    skill.name = "Dynomo Construction";
    skill.desc = "Active - Increases build rate to 400% for ten seconds.";
    list.push_back(skill);

    skill.name = "Turret Overdrive";
    skill.desc = "Active - Causes constructions to go into overdrive mode, dealing extra damage for one second per rank.";
    list.push_back(skill);




}
