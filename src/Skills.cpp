#include "Skills.h"



Skills::Skills()
{
    Skill skill;
    skill.level = 0;
    skill.ranks = 0;
    skill.ranksmax = 5;
    skill.durationint = 0;
    skill.duration = 0;
    skill.cost = 1;
    skill.cooldownint = 30*10;
    skill.cooldown = 0;
    skill.autouse = true;
    skill.active = false; // Splits Passive skills from Active skills.
    skill.on = false; // Means it's currently going.


    skill.tree = "Melee";

    skill.name = "Batter Up";
    skill.desc = "Passive - Damage*Rank is applied as force to your enemies.";
    skill.level = 0;
    skill.active = false;
    list.push_back(skill);

    skill.name = "Cleave";
    skill.desc = "Active - Activate to Swing a melee weapon for 75% damage in a 180 degree arc \nRanks grant 25% more damage";
    skill.level = 0;
    skill.active = true;
    list.push_back(skill);


    /*



    skill.name = "Rambo Style";
    skill.desc = "Passive - Allows use of one large weapon in each hand, ranks reduce damage and accuracy penalties.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Power Attack";
    skill.desc = "Active - Melee attack deals increased damage.";
    skill.active = true;
    list.push_back(skill);

    skill.name = "Grand Slam";
    skill.desc = "Active - Extreme Knockback attack, Turns target into a projectile.";
    skill.active = true;
    list.push_back(skill);

    skill.name = "Tremor";
    skill.desc = "Active - Slams the ground, dealing damage to all nearby.";
    skill.active = true;
    list.push_back(skill);

    skill.name = "Sunder";
    skill.desc = "Passive - 20% chance per rank to destroy an opponents held weapon, Max ranks causes the item to simply be dropped on the floor.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Shadowstep";
    skill.desc = "Passive - While engaged in melee, teleport behind target.";
    //Useful by itself, and with backstab. (Critters need to turn around to aim and attack, so it grants some time)
    skill.active = false;
    list.push_back(skill);

    skill.name = "Backstab";
    skill.desc = "Passive - Deal 20% more damge per rank when striking an opponent from the back with a melee weapon.";
    skill.active = false;
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

    skill.name = "Reflective Coating";
    skill.desc = "Passive - Projectiles that strike your shield are reflected, ranks increase accuracy to reflect to enemy.";
    skill.active = false;
    list.push_back(skill);

    */


    skill.tree = "Ranger";

    /*
    skill.name = "Offensive Reloading";
    skill.desc = "Passive - When reloading, you flick the mag/shell at nearby enemies to stun them, AoE causing stun. Ranks increase range and duration of stun.";
    skill.active = false;
    list.push_back(skill);

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

    skill.name = "Two Handed Luck";
    skill.desc = "Passive - Randomly get a few seconds of no recoil while firing one weapon two handed, or two individual weapons.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Battle Roar";
    skill.desc = "Active - Surge of Adrenaline causes creature to have 0 recoil for the duration.";
    skill.active = true;
    list.push_back(skill);

    */

    skill.name = "Bronze Bullet";
    skill.desc = "Passive - The first bullet in a magazine deals 10% extra base damage per rank \nstacks with other 'Bullet' skills if the position aligns.";
    skill.level = 0;
    skill.active = false;
    list.push_back(skill);

    skill.name = "Miscounted Shot";
    skill.desc = "Passive - Each bullet has a 5% chance per rank to not consume any ammo.";
    skill.level = 0;
    skill.active = false;
    list.push_back(skill);

    skill.name = "Silver Bullet";
    skill.desc = "Passive - The middle-most bullet in a magazine deals 20% extra base damage per rank \nstacks with other 'Bullet' skills if the position aligns.";
    skill.level = 1;
    skill.active = false;
    list.push_back(skill);

    skill.name = "Gold Bullet";
    skill.desc = "Passive - The last bullet in a magazine deals 30% extra base damage per rank \nstacks with other 'Bullet' skills if the position aligns.";
    skill.level = 2;
    skill.active = false;
    list.push_back(skill);

    skill.name = "Snipe Shot";
    skill.desc = "Active - Fires a single bullet with 50% additional Range and Damage per rank.";// When aiming, displays a line of travel, including richochet potential and penetration.";
    skill.level = 3;
    skill.active = true;
    list.push_back(skill);

    /*


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

    skill.name = "Spray and Pray";
    skill.desc = "Active - fires in an arc (rank) times";
    skill.active = true;
    list.push_back(skill);

    skill.name = "Deft Hands";
    skill.desc = "Passive - Increases reload speed and item swap speed.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Unlucky Frag";
    skill.desc = "Passive - Bullets have a (rank)% chance to fragment upon collision, sending short lived bullets out.";
    skill.active = false;
    list.push_back(skill);

    */


    skill.tree = "Magic";

    skill.name = "(NON-FUNCTIONAL) Mana Regen";
    skill.desc = "Passive - Gain an additional 20% mana regeneration for your devices per rank.";
    skill.active = false;
    list.push_back(skill);
    /*
    skill.name = "Mana Regen";
    skill.desc = "Passive - Gain an additional 20% mana regeneration for your devices per rank.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Mana Surge";
    skill.desc = "Active - For [Rank*2] seconds, Spells cost no mana.";
    skill.active = true;
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

    skill.name = "Elemental Basics";
    skill.desc = "Passive - 10% per skill rank to apply elemental effect to enemies.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Elemental Mastery";
    skill.desc = "Passive - 20% Additional Elemental Damage + 10% per skill rank to apply elemental effect to enemies. (Stacks with previous)";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Metamagic: Bouncing Spell";
    skill.desc = "Active - Spells richochet [Rank] times for [Rank*5] Seconds";
    skill.active = true;
    list.push_back(skill);

    skill.name = "Metamagic: Piercing Spell";
    skill.desc = "Active - Spells penetrate [Rank] enemies for [Rank*5] Seconds";
    skill.active = true;
    list.push_back(skill);

    skill.name = "Metamagic: Enhance Spell";
    skill.desc = "Active - For [Rank*2] seconds, Spells have double effects.";
    skill.active = true;
    list.push_back(skill);

    skill.name = "Metamagic: MultiSpell";
    skill.desc = "Active - For [Rank*2] seconds, Spells have a one second cooldown.";
    skill.active = true;
    list.push_back(skill);


    */


    skill.tree = "Engineer";

    skill.name = "Ammo Construction";
    skill.desc = "Active - Produces 5 ammo per rank for 30 scrap, Cannot produce ammo beyond cap.\n(LeftHand priority) ";
    skill.level = 0;
    skill.active = true;
    skill.autouse = false;
    list.push_back(skill);

    skill.name = "Turret Construction";
    skill.desc = "Active - For 30 scrap, Creates a friendly turret that deals 25*rank damage.";
    skill.level = 0;
    skill.active = true;
    skill.autouse = false;
    list.push_back(skill);

    skill.name = "Logistics Expert";
    skill.desc = "Passive - Increases the rate switches are fixed. Intelligence is applied to work speed per \nrank.";
    skill.level = 1;
    skill.active = false;
    list.push_back(skill);

    skill.name = "Lucky Scavenger";
    skill.desc = "Passive - 20% chance per rank to find money or ammo while picking up scrap.";
    skill.level = 1;
    skill.active = false;
    skill.autouse = true;
    list.push_back(skill);

    skill.name = "Explosive Deconstruction";
    skill.ranksmax = 1;
    skill.desc = "Passive - Upon constructions death, detonates with a scaled explosion based on \nconstructions health and cost.";
    skill.level = 2;
    skill.active = false;
    list.push_back(skill);
    skill.ranksmax = 5;



    skill.name = "Quality Built";
    skill.desc = "Passive - Increases construction base damage and range by 20% per rank.";
    skill.level = 2;
    skill.active = false;
    list.push_back(skill);

    skill.name = "Practical Problems";
    skill.desc = "Passive - Doubles the weapon slots of the turret. Sometimes, You just need more gun.";
    skill.level = 3;
    skill.ranksmax = 1;
    skill.active = false;
    list.push_back(skill);
    skill.ranksmax = 5;

    /*

    skill.name = "Deconstruct";
    skill.desc = "Active - Allows reclaimation of parts from machines you've made, or broken enemy machines.";
    skill.ranksmax = 1;
    skill.active = true;
    list.push_back(skill);
    skill.ranksmax = 5;

    skill.name = "Shield Generator Construction";
    skill.desc = "Active - Allows construction of a placable shield generator, The shield takes the damage of those inside it's protection. Shield Health: [Rank*1000]";
    skill.active = true;
    skill.autouse = false;
    list.push_back(skill);

    skill.name = "Switch Bot Construction";
    skill.desc = "Active - Allows construction of a cheap little bot that can be slapped onto a switch to work on it for you.";
    skill.active = true;
    skill.autouse = false;
    list.push_back(skill);

    skill.name = "Drone: Medi-Bot";
    skill.desc = "Active - Builds a small hovering bot that heals allies.";
    skill.active = true;
    skill.autouse = false;
    list.push_back(skill);

    skill.name = "Drone: Merc-Bot";
    skill.desc = "Active - Builds a small hovering bot that has a random weapon based on rank, assaults enemies.";
    skill.active = true;
    skill.autouse = false;
    list.push_back(skill);

    skill.name = "Drone: Mach-Helper";
    skill.desc = "Active - Builds a small hovering bot that speeds around, picking up equipment, Ranks increase speed and inventory slots.";
    skill.active = true;
    skill.autouse = false;
    list.push_back(skill);

    skill.name = "Barricade Construction";
    skill.desc = "Active - Builds a fortification to block enemy fire/progression, Ranks add spikes and other lethal bits";
    skill.active = true;
    skill.autouse = false;
    list.push_back(skill);

    skill.name = "Lemme help with that";
    skill.desc = "Passive - 10% chance per rank to prevent allies item durability loss and prevent weapon jamming.";
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
    */

    skill.name = "Undead Dread";
    skill.desc = "Passive - 40% of missing health percentage is added to damage per rank.";
    skill.level = 0;
    skill.active = false;
    list.push_back(skill);

    skill.name = "Vampirism";
    skill.desc = "Passive - Gain (Rank)% Life Steal on damage done to enemies.";
    skill.level = 1;
    skill.active = false;
    list.push_back(skill);

    skill.name = "Feral Regeneration";
    skill.desc = "Passive - Creature gains 0.2% (total health per rank) health regeneration per second.";
    skill.level = 1;
    // Certain creatures will start with this at max ranks, like trolls.
    skill.active = false;
    list.push_back(skill);

    skill.name = "Feral Body";
    skill.desc = "Passive - Creature gains 10% Damage Resistence per Rank";
    skill.level = 2;
    skill.active = false;
    list.push_back(skill);





    /*

    skill.name = "Incite Rage";
    skill.desc = "Active - Causes enemies within an AoE to use twice the ammo, but gaining 30% additional damage, for 2 seconds per rank.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Undeath";
    skill.desc = "Passive - Missing health percentage is added to attributes.";
    skill.active = false;
    list.push_back(skill);

    skill.name = "Demonic Bounty";
    skill.desc = "Passive - Kills gain temporary buffs to stats, attack speed, damage, reload speed, ect";
    skill.active = false;
    list.push_back(skill);

    skill.name = "(Epic)Blaze of Glory";
    skill.desc = "Active - Gain invincibility for a time, Causes a detonation once the invicibility wears off. (Ranks reduce explosive damage taken?)";
    skill.active = true;
    list.push_back(skill);

    skill.name = "(Epic)Soulbind";
    skill.desc = "Passive - Two squadmembers that have this ability can bind to eachother. \n" +
	" While bound, neither can die unless BOTH do, but any excess damage is moved to the other.";
    skill.active = false;
    list.push_back(skill);

    */



    skill.tree = "Paragon";
    skill.ranksmax = 99999;

    skill.name = "(NON-FUNCTIONAL) Perfect Health";
    skill.desc = "Passive - Gain 10% base health per rank.";
    skill.active = false;
    list.push_back(skill);

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

Skill * Skills::getSkill(std::string skillName)
{
    for(auto &skill : list)
    {
        if(skill.name == skillName)
            return &skill;
    }
    return nullptr;
}

