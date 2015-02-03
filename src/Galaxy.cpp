#include "Galaxy.h"

#include "Faction.h"
#include "Textures.h"
#include "globalvars.h"
#include "math.h"
#include "util.h"

#include <iostream>

GalaxyController gCtrl;

extern sf::RenderWindow window;

GalaxyController::GalaxyController()
    : timescale(10), timescalez(10), timetilltick(0), waves(1), waveTimer(1000),
      tickTimer(10), bTest(false), myDebug(false), menuType("NULL")
{
}

void GalaxyController::addvPlanet()
{

    //long long unsigned int Rawr = (sizeof(vChunk)*10000);
    std::cout << (32 * 32) * 32 << std::endl;

    //std::cout << "Grid: " << (sizeof(vChunk)*1000000) << " vs Tile:" << Rawr << std::endl;
    //std::cout << "Grid: " << (sizeof(Grid)) << " vs Tile:" << sizeof(xTile) << std::endl;
}

void GalaxyController::wave()
{
    bool FoundAny = false;
    for (auto &elem : npclist)
    {
        if (elem.hasTag("[EatsFlesh:") && elem.health > 0)
        {
            FoundAny = true;
        }
    }
    if (FoundAny == false)
    {
        waveTimer--;
        if (waveTimer <= 0)
        {
            zombieSwarmLocal(waves);
            waveTimer = 1000;
            waves++;
        }
        textList.createText(((gvars::currentx - 2) * GridSize) + 1,
                            ((gvars::currenty - 18) * GridSize) + 1, 11,
                            sf::Color::Black, "Time Till Wave ", "", waves + 1,
                            ": ", "", waveTimer / 10);
        if (waveTimer / 2 < 50 && randz(0, 1) == 1)
        {
            textList.createText((gvars::currentx - 2) * GridSize,
                                (gvars::currenty - 18) * GridSize, 11,
                                sf::Color::White, "Time Till Wave ", "",
                                waves + 1, ": ", "", waveTimer / 10);
        }
        else
        {
            textList.createText((gvars::currentx - 2) * GridSize,
                                (gvars::currenty - 18) * GridSize, 11,
                                sf::Color::Red, "Time Till Wave ", "",
                                waves + 1, ": ", "", waveTimer / 10);
        }
    }
}

void GalaxyController::time(int /*Modify*/)
{
    if (timetilltick >= tickTimer)
    {
        timetilltick = 0;
        //UpdatePlanet();
    }
    timetilltick += timescalez;
}

void GalaxyController::buildLocal(std::string Type, int ZedAmount)
{ // TODO: Replace this with a Class containing Critter types and amounts, Probably inventories as well, Or start up premade critters.
    if (Type == "Test")
    {
        if (myDebug)
            std::cout << "Building Lightmap\n";
        for (int i = 0; i != gridy; i++)
        {
            for (int t = 0; t != gridx; t++)
            {
                //globals::sunmap[z][i][t] = 255;
            }
        }
        // ----------------------------------------
        // 1 dirt 3 grass 6 sand 7 stone 10 wall 20 dirtchannel 52 deepwater 53 lava
        if (myDebug)
            std::cout << "Creating Mass Tiles\n";
        for (int i = 0; i != Grids; i++)
        {
            for (int t = 0; t != Grids; t++)
            {
                tiles[i][t][30].id = 3;
                tiles[i][t][30].grass();
                tiles[i][t][29].id = 1;
                tiles[i][t][28].id = 1;
                tiles[i][t][27].id = 7;
                tiles[i][t][26].id = 7;
                tiles[i][t][25].id = 7;
                tiles[i][t][24].id = 53;
            }
        }
        if (myDebug)
            std::cout << "Placing Dirt\n";
        for (int i = 32; i != 64; i++)
        {
            for (int t = 32; t != 64; t++)
            {
                tiles[i][t][30].dirt();
            }
        }
        if (myDebug)
            std::cout << "Placing Walls\n";
        for (int i = 40; i != 59; i++)
        {
            for (int t = 40; t != 58; t++)
            {
                tiles[i][t][30].wall();
            }
        }
        if (myDebug)
            std::cout << "Placing Stone \n";
        for (int i = 41; i != 58; i++)
        {
            for (int t = 41; t != 57; t++)
            {
                tiles[i][t][30].stone();
            }
        }
        tiles[40][48][30].stone();
        tiles[58][48][30].stone();
        tiles[49][40][30].stone();
        tiles[49][57][30].stone();
        //globals::groundmap[6][49][49] = 10;
        //globals::groundmap[6][49][50] = 10;
        //globals::groundmap[6][49][51] = 10;
        //globals::groundmap[6][49][52] = 10;

        // ----------------------------------------

        {
            if (myDebug)
            {
                std::cout << "PreInt\n";
            }
            try
            {
                for (int count = 0; count <= 2; count++)
                {
                    sf::Vector2f vPos = math::circleRandz(1000, 1000, 180);
                    spawnCritter("Human", vPos.x, vPos.y);
                }

                spawnItem("Sword", 1039, 1020);

                spawnItem("Gun", 1059, 1020);
                Con("Hunting for Trees 1");
                for (int i = 0; i != 5; i++)
                {
                    int Tx = randz(0, 2000), Ty = randz(0, 2000);
                    if (tiles[abs_to_index(Tx / 20)][abs_to_index(Ty / 20)][30]
                                .id == 1001 ||
                        tiles[abs_to_index(Tx / 20)][abs_to_index(Ty / 20)][30]
                                .id == 1003)
                    {
                        spawnItem("Fruit Tree", Tx, Ty);
                    }
                }
                //SpawnItem("Fruit Tree",970,1050);
                //SpawnItem("Fruit Tree",950,1050);
                //SpawnItem("Fruit Tree",930,1050);

                spawnItem("Broken Vending Machine", 1150, 1050);

                spawnCritter("Mini Turret", 1000, 1000);
            }
            catch (std::exception &e)
            {
                std::cout << "Problem during initalization of NPC Spawns. \n";
            }
            if (myDebug)
            {
                std::cout << "PostInt\n";
            }
        }
    }
    if (Type == "City")
    {
        // ----------------------------------------
        // 1 dirt 3 grass 6 sand 7 stone 10 wall 20 dirtchannel 52 deepwater 53 lava
        debug("Creating Mass Tiles");
        for (int i = 0; i != Grids; i++)
        {
            for (int t = 0; t != Grids; t++)
            {
                tiles[i][t][30].id = 3;
                tiles[i][t][30].grass();
                tiles[i][t][29].id = 1;
                tiles[i][t][28].id = 1;
                tiles[i][t][27].id = 7;
                tiles[i][t][26].id = 7;
                tiles[i][t][25].id = 7;
                tiles[i][t][24].id = 53;
            }
        }
        if (myDebug)
            std::cout << "Placing Dirt\n";
        for (int i = 32; i != 64; i++)
        {
            for (int t = 32; t != 64; t++)
            {
                tiles[i][t][30].dirt();
            }
        }
        if (myDebug)
            std::cout << "Placing Walls\n";
        for (int i = 40; i != 59; i++)
        {
            for (int t = 40; t != 58; t++)
            {
                tiles[i][t][30].wall();
            }
        }
        if (myDebug)
            std::cout << "Placing Stone \n";
        for (int i = 41; i != 58; i++)
        {
            for (int t = 41; t != 57; t++)
            {
                tiles[i][t][30].stone();
            }
        }
        tiles[40][48][30].stone();
        tiles[58][48][30].stone();
        tiles[49][40][30].stone();
        tiles[49][57][30].stone();

        {
            if (myDebug)
            {
                std::cout << "PreInt\n";
            }
            try
            {
                /*for( int count = 0; count <= 2; count++)
                    {
                        sf::Vector2f vPos = Math::CircleRandz(1000,1000,180);
                        SpawnCritter("Human",vPos.x,vPos.y);
                    }*/
                for (size_t count = 0; count != squady.squad.size(); count++)
                {
                    sf::Vector2f vPos = math::circleRandz(1000, 1000, 180);
                    //SpawnCritter("Human",vPos.x,vPos.y);
                    squady.squad.at(count).xpos = vPos.x;
                    squady.squad.at(count).ypos = vPos.y;

                    npcmanager.addedCritters.push_back(squady.squad.at(count));
                }

                for (int zeds = 0; zeds != ZedAmount; zeds++)
                {
                    Con("Starting Zed");
                    sf::Vector2f vPos = math::circleRandz(1000, 1000, 580);
                    spawnCritter("Zombie", vPos.x, vPos.y);
                    Con("Ending Zed");
                }

                spawnItem("Sword", 1039, 1020);

                spawnItem("Gun", 1059, 1020);
                // Does Not Function
                Con("Hunting for Trees 2");
                //SpawnItem("Fruit Tree",970,1050);
                //SpawnItem("Fruit Tree",950,1050);
                //SpawnItem("Fruit Tree",930,1050);

                spawnItem("Broken Vending Machine", 1150, 1050);

                spawnCritter("Mini Turret", 1000, 1000);
            }
            catch (std::exception &e)
            {
                std::cout << "Problem during initalization of NPC Spawns. \n";
            }
            if (myDebug)
            {
                std::cout << "PostInt\n";
            }
        }
    }

    if (Type == "LoadTile")
    {
    }
}

void GalaxyController::buildLocalTest()
{
    if (myDebug)
        std::cout << "Building Lightmap\n";
    for (int i = 0; i <= gridy - 1; i++)
    {
        for (int t = 0; t <= gridx - 1; t++)
        {
            //globals::sunmap[z][i][t] = 255;
        }
    }
    // ----------------------------------------
    // 1 dirt 3 grass 6 sand 7 stone 10 wall 20 dirtchannel 52 deepwater 53 lava
    if (myDebug)
        std::cout << "Creating Mass Tiles\n";
    for (int i = 0; i != Grids; i++)
    {
        for (int t = 0; t != Grids; t++)
        {
            tiles[i][t][30].id = 3;
            //Tiles[i][t][30].Grass();
            tiles[i][t][29].id = 1;
            tiles[i][t][28].id = 1;
            tiles[i][t][27].id = 7;
            tiles[i][t][26].id = 7;
            tiles[i][t][25].id = 7;
            tiles[i][t][24].id = 53;
        }
    }
    if (myDebug)
        std::cout << "Placing Dirt\n";
    for (int i = 31; i != 64; i++)
    {
        for (int t = 31; t != 64; t++)
        {
            tiles[i][t][30].dirt();
            Con(t);
        }
    }
    if (myDebug)
        std::cout << "Placing Walls\n";
    for (int i = 40; i != 59; i++)
    {
        for (int t = 40; t != 58; t++)
        {
            tiles[i][t][30].wall();
        }
    }
    if (myDebug)
        std::cout << "Placing Stone \n";
    for (int i = 41; i != 58; i++)
    {
        for (int t = 41; t != 57; t++)
        {
            tiles[i][t][30].stone();
        }
    }
    tiles[40][48][30].stone();
    tiles[58][48][30].stone();
    tiles[49][40][30].stone();
    tiles[49][57][30].stone();
    //globals::groundmap[6][49][49] = 10;
    //globals::groundmap[6][49][50] = 10;
    //globals::groundmap[6][49][51] = 10;
    //globals::groundmap[6][49][52] = 10;

    // ----------------------------------------

    {
        if (myDebug)
        {
            std::cout << "PreInt\n";
        }
        try
        {
            for (int count = 0; count <= 2; count++)
            {
                sf::Vector2f vPos = math::circleRandz(1000, 1000, 180);
                spawnCritter("Human", vPos.x, vPos.y);
            }

            spawnItem("Sword", 1039, 1020);

            spawnItem("Gun", 1059, 1020);
            for (int i = 0; i != 5; i++)
                //Does not Run
                Con("Hunting for Trees 3");
            //SpawnItem("Fruit Tree",970,1050);
            //SpawnItem("Fruit Tree",950,1050);
            //SpawnItem("Fruit Tree",930,1050);

            spawnItem("Broken Vending Machine", 1150, 1050);

            spawnCritter("Mini Turret", 1000, 1000);
        }
        catch (std::exception &e)
        {
            std::cout << "Problem during initalization of NPC Spawns. \n";
        }
        if (myDebug)
        {
            std::cout << "PostInt\n";
        }
    }
}

void GalaxyController::zombieSwarmLocal(int num)
{
    {
        for (int count = 0; count <= num; count++)
        {
            sf::Vector2f Pos = math::circleRandz(1000, 1000, 500);
            spawnCritter("Zombie", Pos.x, Pos.y);
        }
    }
}

void GalaxyController::buildMainMenu()
{
    //Effectz.CreateSquare(-1000,-1000,10000,10000,Cyan);
    sf::Sprite Sprt;
    Sprt.setTexture(texturemanager.getTexture("Main.png"));

    window.draw(Sprt);
}

void GalaxyController::buildTileTest()
{ // Build the galaxy.
    //for(int gx = 0; gx != 100; gx++){
    // for(int gy = 0; gy != 100; gy++){
    // for(int rx = 0; rx != 100; rx++){
    // for(int ry = 0; ry != 100; ry++){
    for (int lx = 0; lx != 100; lx++)
    {
        for (int ly = 0; ly != 100; ly++)
        {
            Tile T;
            //T.GalPos = sf::Vector2f(50,50);
            //T.RegPos = sf::Vector2f(50,50);
            //T.LocPos = sf::Vector2f(lx,ly);
            T.id = randz(0, 10);
            //Tiles.push_back(T);
        }
    }
}

void GalaxyController::buildWorldTest()
{
    debug("Worlding it up. ");
    initalizeWorldTiles();
    debug("Factioning it up. \n");
    //InitalizeFactions();
}

void GalaxyController::worldLoop()
{
    debug("Starting world loop");
    for (size_t i = 0; i != uniFact.size(); i++)
    { // Running through factions, Probably needs it's own function.
        debug("BeginningFaction" + uniFact[i].name);

        if (uniFact[i].initialized == false)
        { // Spawning and assigning factions their starting territory.

            int x = randz(1, 99);
            int y = randz(1, 99);
            if (worldMap[x][y].owner == "")
            {
                worldMap[x][y].owner = uniFact[i].name;

                Territory ClaimedTerritory;

                ClaimedTerritory.worldTile = sf::Vector2i(x, y);

                uniFact[i].territories.push_back(ClaimedTerritory);

                uniFact[i].initialized = true;

                std::cout << uniFact[i].name << " has claimed " << x << "/" << y
                          << std::endl;
            }
        }

        if (uniFact[i].territories.size() != 0 &&
            static_cast<size_t>(uniFact[i].members) <=
                uniFact[i].territories.size() / 2)
        {
            if (randz(1, 100) <= uniFact[i].members * 4)
            {
                uniFact[i].members++;
            }
        }

        if ((uniFact[i].aggressiveness * uniFact[i].members) >=
            randz(0, 100 * (uniFact[i].territories.size() / 4)))
        { // Factions aggression causes them to seek more territory.
            bool TileChange = false;

            if (uniFact[i].territories.empty())
                continue;
            if (uniFact[i].members <=
                0) /* Don't ask how it can be in the negatives. */
                continue;

            int ForMembers = uniFact[i].members;
            for (int t = 0; t != ForMembers; t++)
            {
                int Territ = randz(0, uniFact[i].territories.size() - 1);

                sf::Vector2i Short = uniFact[i].territories[Territ].worldTile;

                if (AABB(Short, 1, 98, 1, 98))
                {
                    int Direction = randz(0, 3);
                    int X = 0;
                    int Y = 0;
                    if (Direction == 0)
                    {
                        X = 0;
                        Y = -1;
                    }
                    if (Direction == 1)
                    {
                        X = +1;
                        Y = 0;
                    }
                    if (Direction == 2)
                    {
                        X = 0;
                        Y = +1;
                    }
                    if (Direction == 3)
                    {
                        X = -1;
                        Y = 0;
                    }

                    WorldTile *WT = &worldMap[Short.x + X][Short.y + Y];

                    if (WT->owner == "")
                    { // Claim it for themselves.
                        WT->owner = uniFact[i].name;

                        Territory CT;

                        CT.worldTile = sf::Vector2i(Short.x + X, Short.y + Y);
                        uniFact[i].territories.push_back(CT);
                    }
                    else if (WT->owner != uniFact[i].name)
                    { // Declare War.
                        std::string Tenant = WT->owner;

                        for (size_t z = 0; z != uniFact.size(); z++)
                        {
                            if (uniFact[z].name == Tenant)
                            {
                                float TenantTech = 0;
                                float InvaderTech = 0;

                                TenantTech += uniFact[z].techMedical;
                                TenantTech += uniFact[z].techWeaponryBlunt;
                                TenantTech += uniFact[z].techWeaponryEnergy;
                                TenantTech += uniFact[z].techWeaponryExplosive;
                                TenantTech += uniFact[z].techWeaponryMass;
                                TenantTech += uniFact[z].techWeaponrySharp;

                                InvaderTech += uniFact[i].techMedical;
                                InvaderTech += uniFact[i].techWeaponryBlunt;
                                InvaderTech += uniFact[i].techWeaponryEnergy;
                                InvaderTech += uniFact[i].techWeaponryExplosive;
                                InvaderTech += uniFact[i].techWeaponryMass;
                                InvaderTech += uniFact[i].techWeaponrySharp;

                                float TenantTechnique =
                                    TenantTech +
                                    (TenantTech *
                                     (PercentIs(uniFact[z].creativity, 35) *
                                      0.01));
                                float InvaderTechnique =
                                    InvaderTech +
                                    (InvaderTech *
                                     (PercentIs(uniFact[i].creativity, 35) *
                                      0.01));

                                float TenantPower =
                                    TenantTechnique *
                                    (PercentIs(uniFact[z].members, 60) * 0.01);
                                float InvaderPower =
                                    InvaderTechnique *
                                    (PercentIs(uniFact[i].members, 60) * 0.01);

                                TenantPower = TenantPower * randz(7, 10);
                                InvaderPower = InvaderPower * randz(7, 10);

                                //std::cout << "Tenant's Power: " << TenantPower << ", Invader's Power: " << InvaderPower << std::endl;
                                if (InvaderPower > TenantPower)
                                {
                                    //UniFact[z].Members = Loss;
                                    //if(Members > 0) UniFact[i].Members = randz(UniFact[i].Members/16,UniFact[i].Members/4);
                                    if (uniFact[z].members > 0)
                                        uniFact[z].members -= 1;

                                    WT->owner = uniFact[i].name;

                                    Territory CT;

                                    CT.worldTile =
                                        sf::Vector2i(Short.x + X, Short.y + Y);
                                    uniFact[i].territories.push_back(CT);

                                    for (size_t k = 0;
                                         k != uniFact[z].territories.size();
                                         k++)
                                    {
                                        if (uniFact[z]
                                                .territories[k]
                                                .worldTile ==
                                            sf::Vector2i(Short.x + X,
                                                         Short.y + Y))
                                        {
                                            uniFact[z].territories[k].toDelete =
                                                true;
                                            TileChange = true;
                                        }
                                    }
                                }
                                else
                                {
                                    if (uniFact[i].members > 0)
                                        uniFact[i].members -= 1;
                                    //UniFact[i].Members = randz(UniFact[i].Members/4,UniFact[i].Members);
                                    //UniFact[z].Members = randz(UniFact[z].Members/32,UniFact[z].Members/16);
                                }
                            }
                        }
                    }
                }
            }

            if (TileChange)
            {
                for (size_t Fact = 0; Fact != uniFact.size(); Fact++)
                {
                    //std::cout << UniFact[Fact].Name << " Factions: " << UniFact[Fact].Territories.size();
                    cleanTerritories(uniFact[Fact].territories);
                    //std::cout << ", " << UniFact[Fact].Name << "  Factions: " << UniFact[Fact].Territories.size() << std::endl;
                }
            }
        }

        debug("EndingFaction" + uniFact[i].name);
    }
    debug("Ending world loop");
}

GalaxyController::MenuPointerContainer::MenuPointerContainer()
{
    pItem = nullptr;
    pNPC = nullptr;
    pTile = nullptr;
}
