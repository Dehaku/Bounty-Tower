
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

    std::ofstream outputFile(line.c_str());
    for(auto squaddie : Squaddies)
    {
        Npc critter = *squaddie;

        outputFile << "[name:" << critter.name << "]"
               << "[race:" << critter.race << "]"
               << "[xpos:" << critter.xpos << "]"
               << "[ypos:" << critter.ypos << "]"
               << "[zpos:" << critter.zpos << "]"
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
                "[name:" << item.name << "]"
                "[amount:" << item.amount << "]";
                for(auto status : item.statusEffects)
                {
                    file << "{StatusEffect:"
                    << "[Name:" << status.name << "]"
                    << "[Duration:" << status.duration << "]"
                    << "[AuraRadius:" << status.auraRadius << "]"

                    << "[AuraAllies:" << status.auraAffectsAllies << "]"
                    << "[AuraEnemies:" << status.auraAffectsEnemies << "]"
                    << "[AuraNeutrals:" << status.auraAffectsNeutrals << "]";
                    for(auto aspect : status.aspects)
                        file << "[Aspect:" << aspect.name << ":" << aspect.potency << ":" << aspect.type << "]" ;
                    file << "}";
                }
                for(auto status : item.statusEffectsInflict)
                {
                    file << "{StatusEffectInflict:"
                    << "[Name:" << status.name << "]"
                    << "[Duration:" << status.duration << "]"
                    << "[AuraRadius:" << status.auraRadius << "]"

                    << "[AuraAllies:" << status.auraAffectsAllies << "]"
                    << "[AuraEnemies:" << status.auraAffectsEnemies << "]"
                    << "[AuraNeutrals:" << status.auraAffectsNeutrals << "]";
                    for(auto aspect : status.aspects)
                        file << "[Aspect:" << aspect.name << ":" << aspect.potency << ":" << aspect.type << "]" ;
                    file << "}";
                }
                file << std::endl;
                for(auto invItem : item.internalitems)
                {
                    file << "[InternalItem]"
                    "[name:" << invItem.name << "]"
                    "[amount:" << invItem.amount << "]";
                    for(auto status : invItem.statusEffects)
                    {
                        file << "{StatusEffect:"
                        << "[Name:" << status.name << "]"
                        << "[Duration:" << status.duration << "]"
                        << "[AuraRadius:" << status.auraRadius << "]"

                        << "[AuraAllies:" << status.auraAffectsAllies << "]"
                        << "[AuraEnemies:" << status.auraAffectsEnemies << "]"
                        << "[AuraNeutrals:" << status.auraAffectsNeutrals << "]";
                        for(auto aspect : status.aspects)
                            file << "[Aspect:" << aspect.name << ":" << aspect.potency << ":" << aspect.type << "]" ;
                        file << "}";
                    }
                    for(auto status : invItem.statusEffectsInflict)
                    {
                        file << "{StatusEffectInflict:"
                        << "[Name:" << status.name << "]"
                        << "[Duration:" << status.duration << "]"
                        << "[AuraRadius:" << status.auraRadius << "]"

                        << "[AuraAllies:" << status.auraAffectsAllies << "]"
                        << "[AuraEnemies:" << status.auraAffectsEnemies << "]"
                        << "[AuraNeutrals:" << status.auraAffectsNeutrals << "]";
                        for(auto aspect : status.aspects)
                            file << "[Aspect:" << aspect.name << ":" << aspect.potency << ":" << aspect.type << "]" ;
                        file << "}";
                    }
                    file << std::endl;
                }
            }
        }








        /*
        std::ofstream outputFileItems(invLine.c_str());

        for (auto i = critter.inventory.begin(); i != critter.inventory.end();
             i++)
        {
            outputFile << "[" << (*i).name << "]";
        }
        outputFile << "";

        */

    }





    /*

    std::ofstream file;
    con("looking for file...");
    file.open(line.c_str(), std::fstream::in | std::fstream::ate);
    if (file.is_open())
    {
        con("Problem? 3");
        file << "[name:" << critter.name << "]"
             << "[race:" << critter.race << "]"
             << "[xpos:" << critter.xpos << "]"
             << "[ypos:" << critter.ypos << "]"
             << "[strength:" << critter.attributes.strength << "]"
             << "[perception:" << critter.attributes.perception << "]"
             << "[intelligence:" << critter.attributes.intelligence << "]"
             << "[charisma:" << critter.attributes.charisma << "]"
             << "[endurance:" << critter.attributes.endurance << "]"
             << "[dexterity:" << critter.attributes.dexterity << "]"
             << "[agility:" << critter.attributes.agility << "]"
             << "[health:" << critter.health << "]"
             //<< "[action:" << critter.action << "]"
             << "[angle:" << critter.angle << "]"
             << "[thirst:" << critter.thirst << "]"
             << "[hunger:" << critter.hunger << "]"
             << "[strengthxp:" << critter.attributes.strengthxp << "]"
             << "[perceptionxp:" << critter.attributes.perceptionxp << "]"
             << "[intelligencexp:" << critter.attributes.intelligencexp << "]"
             << "[charismaxp:" << critter.attributes.charismaxp << "]"
             << "[endurancexp:" << critter.attributes.endurancexp << "]"
             << "[dexterityxp:" << critter.attributes.dexterityxp << "]"
             << "[agilityxp:" << critter.attributes.agilityxp << "]"
             << "[cbaseid:" << critter.cbaseid << "]"
             << "[maxhealth:" << critter.maxhealth << "]"
             << "{Tags:" << critter.tags << "}"
             << "{Blood:" << critter.bloodcontent << "}"
             << "{Items:";
        con("Problem? 4");

        //for(int i = 0; i != Critter.inventory.size(); i++)
        for (auto i = critter.inventory.begin(); i != critter.inventory.begin();
             i++)
        {
            file << "[" << (*i).name << "]";
        }
        file << "}";
        file << std::endl;
        con("Problem? 5");
        //Con("Added", false);
        //Con(object.name);
        file.close();
    } // End to Load Map

    */
    /*debug("v-Adding Tags-v");
                debug(StringFindChaos(line,"{Tags:","}"));
                debug("^-Added Tags-^");
                Critter.tags.append(StringFindChaos(line,"{Tags:","}"));
                std::set<std::string> Items;
                std::string List;
                List.append(StringFindChaos(line,"{Items:","}"));
                Items = StringFindSetChaos(List,"[","]");
                for(std::set<std::string>::iterator i = Items.begin(); i != Items.end(); i++)
                {
                    std::string OogaBooga = *i;
                    char *a=new char[OogaBooga.size()+1];a[OogaBooga.size()]=0;memcpy(a,OogaBooga.c_str(),OogaBooga.size());
                    if(Debug){ std::cout << "Pre Critter.AddItem \n";}
                    Critter.AddItem(a,1);
                    if(Debug){ std::cout << "*Post Critter.AddItem* \n";}
                }*/

}

void loadGame(std::string profileName)
{
    for(auto squaddie : Squaddies)
        squaddie->toDelete = true;

    Squaddies.clear();

    std::cout << "Loading Game Profile: " + profileName + "!\n";
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
        critter.attributes.strength = stringFindNumber(line, "[strength:");
        critter.attributes.perception = stringFindNumber(line, "[perception:");
        critter.attributes.intelligence = stringFindNumber(line, "[intelligence:");
        critter.attributes.charisma = stringFindNumber(line, "[charisma:");
        critter.attributes.endurance = stringFindNumber(line, "[endurance:");
        critter.attributes.dexterity = stringFindNumber(line, "[dexterity:");
        critter.cbaseid = stringFindNumber(line, "[cbaseid:");
        critter.tags = stringFindChaos(line,"{Tags:","}");
        std::cout << "Tags: " << critter.tags << std::endl;
        critter.health = critter.getMaxHealth();
        //status.auraAffectsAllies = booleanize(stringFindNumber(line, "[AuraAllies:"));



        /*
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
        */

        critter.isSquaddie = true;
        critter.faction = conFact->name;
        critter.factionPtr = conFact;
        std::cout << "Loaded " << critter.name << " the " << critter.race << std::endl;
        npclist.push_back(critter);
        //setupSquadHotKeySelection();

    }
    Squaddies.clear();
    for(auto &npc : npclist)
        if(npc.faction == "The Titanium Grip" && npc.isSquaddie)
            Squaddies.push_back(&npc);

    std::ifstream itemInput("data/saves/" + profileName + "/save.inv");
    if (!itemInput.is_open())
        return;

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
        con("Getting item: " + itemName);
        item = *getGlobalItem(itemName);
        item.amount = stringFindNumber(line, "[amount:");




        std::string itemOwner = stringFindString(line, "[owner:");

        if(itemOwner != "")
        {
            con("Looking for " + itemOwner);
            for(auto &squaddie : Squaddies)
            {
                con(itemOwner + "/" + squaddie->name);
                if(squaddie->name == itemOwner)
                {
                    con(itemOwner + " found, adding " + item.name);
                    squaddie->inventory.push_back(item);
                }
                else
                    con(squaddie->name + " isn't it. ");
            }
        }
    }


}
