
#include "SaveLoad.h"

void saveGame(std::string profileName)
{

    std::string line("data/saves/" + profileName);

    galaxy_mkdir(line);
    line.append("/save");
    line.append(".crit");

    std::string invLine("data/saves/" + profileName);
    invLine.append("/save");
    invLine.append(".inv");
    std::ofstream outputFileItems(invLine.c_str());


    { // General/Misc data.
        std::string miscLine("data/saves/" + profileName);
        miscLine.append("/save");
        miscLine.append(".misc");
        std::ofstream outputFileMisc(miscLine.c_str());

        outputFileMisc << "[Cash:" << conFact->credits << "]"
        << "[GameBeaten:" << bountytower::gameBeaten << "]";
    }



    std::ofstream outputFile(line.c_str());
    for(auto squaddie : Squaddies)
    {

        Npc critter = *squaddie;
        if(!critter.functional())
            continue;

        outputFile << "[name:" << critter.name << "]"
               << "[race:" << critter.race << "]"
               << "[xpos:" << critter.xpos << "]"
               << "[ypos:" << critter.ypos << "]"
               << "[zpos:" << critter.zpos << "]"
               << "[level:" << critter.level << "]"
               << "[xp:" << critter.xp << "]"
               << "[slots:" << critter.additionalSlots << "]"
               << "[strength:" << critter.attributes.strength << "]"
               << "[perception:" << critter.attributes.perception << "]"
               << "[intelligence:" << critter.attributes.intelligence << "]"
               << "[charisma:" << critter.attributes.charisma << "]"
               << "[endurance:" << critter.attributes.endurance << "]"
               << "[dexterity:" << critter.attributes.dexterity << "]"
               << "[agility:" << critter.attributes.agility << "]"
               << "[health:" << critter.health << "]"
               << "[angle:" << critter.angle << "]"
               << "[cbaseid:" << critter.cbaseid << "]"
               << "[maxhealth:" << critter.maxhealth << "]"
               << "[skillpoints:" << critter.skillpoints << "]"
               << "{Tags:" << critter.tags << "}";

               for(auto skill : critter.skills.list)
               {
                    if(skill.ranks == 0)
                        continue;
                    outputFile << "[Skill:" << skill.name << ":" << skill.ranks << "]";
               }

               outputFile << std::endl;

        if(critter.inventory.empty())
            continue;

        std::ofstream file;
        file.open(invLine.c_str(), std::fstream::in | std::fstream::ate);
        if(file.is_open())
        {
            for(auto item : critter.inventory)
            {
                file << "[owner:" << critter.name << "]"
                << makeStringFromItem(item);
                file << std::endl;

                for(auto invItem : item.internalitems)
                {
                    file << "[InternalItem]"
                    << makeStringFromItem(invItem);
                    file << std::endl;
                }
            }
        }
    }
    chatBox.addChat("(Profile1) Game Saved!",sf::Color::White);
}

void loadGame(std::string profileName)
{
    std::cout << "Loading Game Profile: " + profileName + "!\n";

    { // Loading generic/misc data.
        std::ifstream input("data/saves/" + profileName + "/save.misc");
        if (!input.is_open())
            return;

        con("Misc file is open.");

        while (input.good())
        {
            std::string line;
            getline(input, line);
            conFact->credits = stringFindNumber(line,"Cash:");
            bountytower::gameBeaten = booleanize(stringFindNumber(line, "[GameBeaten:"));
        }
    }

    for(auto squaddie : Squaddies)
        squaddie->toDelete = true;
    Squaddies.clear();

    std::ifstream input("data/saves/" + profileName + "/save.crit");
    if (!input.is_open())
        return;

    con("Critter File is open.");

    while (input.good())
    {
        std::string line;
        getline(input, line);
        Npc critter;

        critter.name = "Debuggery";
        std::string critterName = stringFindString(line, "[name:");
        if(critterName == "")
        { // This is an invalid line, Next!
            std::cout << "This line doesn't have a name! Invalid. \n";
            continue;
        }
        critter.race = stringFindString(line, "[race:");

        // This should save some manual work.
        critter = *getGlobalCritter(critter.race);
        critter.name = critterName;
        critter.skillpoints = stringFindNumber(line, "[skillpoints:");
        critter.xpos = stringFindNumber(line, "[xpos:");
        critter.ypos = stringFindNumber(line, "[ypos:");
        critter.zpos = stringFindNumber(line, "[zpos:");
        critter.level = stringFindNumber(line, "[level:");
        critter.xp = stringFindNumber(line, "[xp:");
        critter.additionalSlots = stringFindNumber(line, "[slots:");
        critter.attributes.strength = stringFindNumber(line, "[strength:");
        critter.attributes.perception = stringFindNumber(line, "[perception:");
        critter.attributes.intelligence = stringFindNumber(line, "[intelligence:");
        critter.attributes.charisma = stringFindNumber(line, "[charisma:");
        critter.attributes.endurance = stringFindNumber(line, "[endurance:");
        critter.attributes.dexterity = stringFindNumber(line, "[dexterity:");
        critter.cbaseid = stringFindNumber(line, "[cbaseid:");
        critter.tags = stringFindChaos(line,"{Tags:","}");
        std::cout << "Tags: " << critter.tags << std::endl;
        //status.auraAffectsAllies = booleanize(stringFindNumber(line, "[AuraAllies:"));

        con("Loading Skills");
        std::vector<std::string> skills = stringFindVectorChaos(line,"[Skill:","]");
        for(auto &skillLine : skills)
        {
            std::vector<std::string> skillBits = stringFindElements(skillLine);
            for(auto &skill : critter.skills.list)
                if(skillBits[0] == skill.name)
                    skill.ranks = std::stoi(skillBits[1]);
        }

        critter.health = critter.getMaxHealth();
        critter.isSquaddie = true;
        critter.faction = conFact->name;
        critter.factionPtr = conFact;
        critter.chasePriority = "Hold Position";
        std::cout << "Loaded " << critter.name << " the " << critter.race << std::endl;
        npclist.push_back(critter);
    }

    AnyDeletes(npclist);
    Squaddies.clear();
    for(auto &npc : npclist)
        if(npc.faction == "The Titanium Grip" && npc.isSquaddie)
            Squaddies.push_back(&npc);

    std::ifstream itemInput("data/saves/" + profileName + "/save.inv");
    if (!itemInput.is_open())
        return;

    Item * lastItem;
    while (itemInput.good())
    {
        std::string line;
        getline(itemInput, line);

        Item item;

        item.name = "Debuggery";
        std::string itemName = stringFindString(line, "[name:");
        if(itemName == "")
        { // This is an invalid line, Next!
            std::cout << "This line doesn't have a name! Invalid. \n";
            continue;
        }
        //con("Getting item: " + itemName);
        //item = *getGlobalItem(itemName);

        item = makeItemFromString(line);

        //Clearing status effects so that loading a preexisting item doesn't dupe the statuses.
        item.statusEffects.clear();
        item.statusEffectsInflict.clear();
        item.statusEffectsCarried.clear();

        item.amount = stringFindNumber(line, "[amount:");

        std::vector<std::string> statusEffects = stringFindVectorChaos(line,"{StatusEffect:","}");
        std::vector<std::string> statusEffectInflicts = stringFindVectorChaos(line,"{StatusEffectInflict:","}");
        std::vector<std::string> statusEffectCarried = stringFindVectorChaos(line,"{StatusEffectCarried:","}");

        con("Status Effects");
        for(auto &statusString : statusEffects)
        {
            StatusEffect status;
            status.name = stringFindString(statusString, "[Name:");
            status.rank = stringFindString(statusString, "[Rank:");
            status.duration = stringFindNumber(statusString, "[Duration:");
            status.auraAffectsAllies = booleanize(stringFindNumber(statusString, "[AuraAllies:"));
            status.auraAffectsEnemies = booleanize(stringFindNumber(statusString, "[AuraEnemies:"));
            status.auraAffectsNeutrals = booleanize(stringFindNumber(statusString, "[AuraNeutrals:"));

            std::vector<std::string> aspectStrings = stringFindVectorChaos(statusString, "[Aspect:","]");
            for(auto &aspectString : aspectStrings)
            {
                StatusAspect aspect;

                std::vector<std::string> aspectBits = stringFindElements(aspectString);
                for(int i = 0; i != 3; i++)
                {
                    if(i == 0)
                        aspect.name = std::stoi(aspectBits[i]);
                    if(i == 1)
                        aspect.potency = std::stoi(aspectBits[i]);
                    if(i == 2)
                        aspect.type = aspectBits[i];
                }
                status.aspects.push_back(aspect);
            }
            con(item.name + " is infused with " + status.name + "!");
            item.statusEffects.push_back(status);
        }

        con("Status Effect Inflicting");
        for(auto &statusString : statusEffectInflicts)
        {
            StatusEffect status;
            status.name = stringFindString(statusString, "[Name:");
            status.rank = stringFindString(statusString, "[Rank:");
            status.duration = stringFindNumber(statusString, "[Duration:");
            status.auraAffectsAllies = booleanize(stringFindNumber(statusString, "[AuraAllies:"));
            status.auraAffectsEnemies = booleanize(stringFindNumber(statusString, "[AuraEnemies:"));
            status.auraAffectsNeutrals = booleanize(stringFindNumber(statusString, "[AuraNeutrals:"));

            std::vector<std::string> aspectStrings = stringFindVectorChaos(statusString, "[Aspect:","]");
            for(auto &aspectString : aspectStrings)
            {
                StatusAspect aspect;

                std::vector<std::string> aspectBits = stringFindElements(aspectString);
                for(int i = 0; i != 3; i++)
                {
                    if(i == 0)
                        aspect.name = std::stoi(aspectBits[i]);
                    if(i == 1)
                        aspect.potency = std::stoi(aspectBits[i]);
                    if(i == 2)
                        aspect.type = aspectBits[i];
                }
                status.aspects.push_back(aspect);
            }
            con(item.name + " is infused with inflicting " + status.name + "!");
            item.statusEffectsInflict.push_back(status);
        }

        con("Status Effect Carried");
        for(auto &statusString : statusEffectCarried)
        {
            StatusEffect status;
            status.name = stringFindString(statusString, "[Name:");
            status.rank = stringFindString(statusString, "[Rank:");
            status.duration = stringFindNumber(statusString, "[Duration:");
            status.auraAffectsAllies = booleanize(stringFindNumber(statusString, "[AuraAllies:"));
            status.auraAffectsEnemies = booleanize(stringFindNumber(statusString, "[AuraEnemies:"));
            status.auraAffectsNeutrals = booleanize(stringFindNumber(statusString, "[AuraNeutrals:"));

            std::vector<std::string> aspectStrings = stringFindVectorChaos(statusString, "[Aspect:","]");
            for(auto &aspectString : aspectStrings)
            {
                StatusAspect aspect;

                std::vector<std::string> aspectBits = stringFindElements(aspectString);
                for(int i = 0; i != 3; i++)
                {
                    if(i == 0)
                        aspect.name = std::stoi(aspectBits[i]);
                    if(i == 1)
                        aspect.potency = std::stoi(aspectBits[i]);
                    if(i == 2)
                        aspect.type = aspectBits[i];
                }
                status.aspects.push_back(aspect);
            }
            con(item.name + " is infused with carrying " + status.name + "!");
            item.statusEffectsCarried.push_back(status);
        }




        std::string itemOwner = stringFindString(line, "[owner:");
        if(itemOwner != "")
        {
            con("Looking for " + itemOwner);
            for(auto &squaddie : Squaddies)
            {
                if(squaddie->name == itemOwner)
                {
                    con(itemOwner + " found, adding " + item.name);
                    squaddie->inventory.push_back(item);
                    lastItem = &squaddie->inventory.back();
                }
            }
        }

        if(line.find("[InternalItem]") != std::string::npos)
        {
            con("Internal Item, Shoving into " + lastItem->name);
            lastItem->internalitems.push_back(item);
        }
    }

    con("Game's loaded, You're good to go recollect some bounties!");
    //chatBox.addChat(" Game's loaded, You're good to go recollect some bounties",sf::Color::White);
}
