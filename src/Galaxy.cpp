#include "Galaxy.h"

#include "Faction.h"
#include "Textures.h"
#include "globalvars.h"
#include "math.h"
#include "util.h"

#include <iostream>

GalaxyController GC;

extern sf::RenderWindow App;

GalaxyController::GalaxyController()
    : timescale(10), timescalez(10), timetilltick(0), Waves(1), WaveTimer(1000),
      TickTimer(10), bTest(false), Debug(false), MenuType("NULL")
{
}

void GalaxyController::AddvPlanet()
{

    //long long unsigned int Rawr = (sizeof(vChunk)*10000);
    std::cout << (32 * 32) * 32 << std::endl;

    //std::cout << "Grid: " << (sizeof(vChunk)*1000000) << " vs Tile:" << Rawr << std::endl;
    //std::cout << "Grid: " << (sizeof(Grid)) << " vs Tile:" << sizeof(xTile) << std::endl;
}

void GalaxyController::Wave()
{
    bool FoundAny = false;
    for (auto &elem : npclist)
    {
        if (elem.HasTag("[EatsFlesh:") && elem.health > 0)
        {
            FoundAny = true;
        }
    }
    if (FoundAny == false)
    {
        WaveTimer--;
        if (WaveTimer <= 0)
        {
            ZombieSwarmLocal(Waves);
            WaveTimer = 1000;
            Waves++;
        }
        cText.CreateText(((gvars::currentx - 2) * GridSize) + 1,
                         ((gvars::currenty - 18) * GridSize) + 1, 11,
                         sf::Color::Black, "Time Till Wave ", "", Waves + 1,
                         ": ", "", WaveTimer / 10);
        if (WaveTimer / 2 < 50 && randz(0, 1) == 1)
        {
            cText.CreateText((gvars::currentx - 2) * GridSize,
                             (gvars::currenty - 18) * GridSize, 11,
                             sf::Color::White, "Time Till Wave ", "", Waves + 1,
                             ": ", "", WaveTimer / 10);
        }
        else
        {
            cText.CreateText((gvars::currentx - 2) * GridSize,
                             (gvars::currenty - 18) * GridSize, 11,
                             sf::Color::Red, "Time Till Wave ", "", Waves + 1,
                             ": ", "", WaveTimer / 10);
        }
    }
}

void GalaxyController::Time(int /*Modify*/)
{
    if (timetilltick >= TickTimer)
    {
        timetilltick = 0;
        //UpdatePlanet();
    }
    timetilltick += timescalez;
}

void GalaxyController::BuildLocal(std::string Type, int ZedAmount)
{ // TODO: Replace this with a Class containing Critter types and amounts, Probably inventories as well, Or start up premade critters.
    if (Type == "Test")
    {
        if (Debug)
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
        if (Debug)
            std::cout << "Creating Mass Tiles\n";
        for (int i = 0; i != Grids; i++)
        {
            for (int t = 0; t != Grids; t++)
            {
                Tiles[i][t][30].ID = 3;
                Tiles[i][t][30].Grass();
                Tiles[i][t][29].ID = 1;
                Tiles[i][t][28].ID = 1;
                Tiles[i][t][27].ID = 7;
                Tiles[i][t][26].ID = 7;
                Tiles[i][t][25].ID = 7;
                Tiles[i][t][24].ID = 53;
            }
        }
        if (Debug)
            std::cout << "Placing Dirt\n";
        for (int i = 32; i != 64; i++)
        {
            for (int t = 32; t != 64; t++)
            {
                Tiles[i][t][30].Dirt();
            }
        }
        if (Debug)
            std::cout << "Placing Walls\n";
        for (int i = 40; i != 59; i++)
        {
            for (int t = 40; t != 58; t++)
            {
                Tiles[i][t][30].Wall();
            }
        }
        if (Debug)
            std::cout << "Placing Stone \n";
        for (int i = 41; i != 58; i++)
        {
            for (int t = 41; t != 57; t++)
            {
                Tiles[i][t][30].Stone();
            }
        }
        Tiles[40][48][30].Stone();
        Tiles[58][48][30].Stone();
        Tiles[49][40][30].Stone();
        Tiles[49][57][30].Stone();
        //globals::groundmap[6][49][49] = 10;
        //globals::groundmap[6][49][50] = 10;
        //globals::groundmap[6][49][51] = 10;
        //globals::groundmap[6][49][52] = 10;

        // ----------------------------------------

        {
            if (Debug)
            {
                std::cout << "PreInt\n";
            }
            try
            {
                for (int count = 0; count <= 2; count++)
                {
                    sf::Vector2f vPos = math::CircleRandz(1000, 1000, 180);
                    SpawnCritter("Human", vPos.x, vPos.y);
                }

                SpawnItem("Sword", 1039, 1020);

                SpawnItem("Gun", 1059, 1020);
                Con("Hunting for Trees 1");
                for (int i = 0; i != 5; i++)
                {
                    int Tx = randz(0, 2000), Ty = randz(0, 2000);
                    if (Tiles[abs_to_index(Tx / 20)][abs_to_index(Ty / 20)][30]
                                .ID == 1001 ||
                        Tiles[abs_to_index(Tx / 20)][abs_to_index(Ty / 20)][30]
                                .ID == 1003)
                    {
                        SpawnItem("Fruit Tree", Tx, Ty);
                    }
                }
                //SpawnItem("Fruit Tree",970,1050);
                //SpawnItem("Fruit Tree",950,1050);
                //SpawnItem("Fruit Tree",930,1050);

                SpawnItem("Broken Vending Machine", 1150, 1050);

                SpawnCritter("Mini Turret", 1000, 1000);
            }
            catch (std::exception &e)
            {
                std::cout << "Problem during initalization of NPC Spawns. \n";
            }
            if (Debug)
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
                Tiles[i][t][30].ID = 3;
                Tiles[i][t][30].Grass();
                Tiles[i][t][29].ID = 1;
                Tiles[i][t][28].ID = 1;
                Tiles[i][t][27].ID = 7;
                Tiles[i][t][26].ID = 7;
                Tiles[i][t][25].ID = 7;
                Tiles[i][t][24].ID = 53;
            }
        }
        if (Debug)
            std::cout << "Placing Dirt\n";
        for (int i = 32; i != 64; i++)
        {
            for (int t = 32; t != 64; t++)
            {
                Tiles[i][t][30].Dirt();
            }
        }
        if (Debug)
            std::cout << "Placing Walls\n";
        for (int i = 40; i != 59; i++)
        {
            for (int t = 40; t != 58; t++)
            {
                Tiles[i][t][30].Wall();
            }
        }
        if (Debug)
            std::cout << "Placing Stone \n";
        for (int i = 41; i != 58; i++)
        {
            for (int t = 41; t != 57; t++)
            {
                Tiles[i][t][30].Stone();
            }
        }
        Tiles[40][48][30].Stone();
        Tiles[58][48][30].Stone();
        Tiles[49][40][30].Stone();
        Tiles[49][57][30].Stone();

        {
            if (Debug)
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
                for (size_t count = 0; count != Squady.Squad.size(); count++)
                {
                    sf::Vector2f vPos = math::CircleRandz(1000, 1000, 180);
                    //SpawnCritter("Human",vPos.x,vPos.y);
                    Squady.Squad.at(count).xpos = vPos.x;
                    Squady.Squad.at(count).ypos = vPos.y;

                    npcmanager.AddedCritters.push_back(Squady.Squad.at(count));
                }

                for (int zeds = 0; zeds != ZedAmount; zeds++)
                {
                    Con("Starting Zed");
                    sf::Vector2f vPos = math::CircleRandz(1000, 1000, 580);
                    SpawnCritter("Zombie", vPos.x, vPos.y);
                    Con("Ending Zed");
                }

                SpawnItem("Sword", 1039, 1020);

                SpawnItem("Gun", 1059, 1020);
                // Does Not Function
                Con("Hunting for Trees 2");
                //SpawnItem("Fruit Tree",970,1050);
                //SpawnItem("Fruit Tree",950,1050);
                //SpawnItem("Fruit Tree",930,1050);

                SpawnItem("Broken Vending Machine", 1150, 1050);

                SpawnCritter("Mini Turret", 1000, 1000);
            }
            catch (std::exception &e)
            {
                std::cout << "Problem during initalization of NPC Spawns. \n";
            }
            if (Debug)
            {
                std::cout << "PostInt\n";
            }
        }
    }

    if (Type == "LoadTile")
    {
    }
}

void GalaxyController::BuildLocalTest()
{
    if (Debug)
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
    if (Debug)
        std::cout << "Creating Mass Tiles\n";
    for (int i = 0; i != Grids; i++)
    {
        for (int t = 0; t != Grids; t++)
        {
            Tiles[i][t][30].ID = 3;
            //Tiles[i][t][30].Grass();
            Tiles[i][t][29].ID = 1;
            Tiles[i][t][28].ID = 1;
            Tiles[i][t][27].ID = 7;
            Tiles[i][t][26].ID = 7;
            Tiles[i][t][25].ID = 7;
            Tiles[i][t][24].ID = 53;
        }
    }
    if (Debug)
        std::cout << "Placing Dirt\n";
    for (int i = 31; i != 64; i++)
    {
        for (int t = 31; t != 64; t++)
        {
            Tiles[i][t][30].Dirt();
            Con(t);
        }
    }
    if (Debug)
        std::cout << "Placing Walls\n";
    for (int i = 40; i != 59; i++)
    {
        for (int t = 40; t != 58; t++)
        {
            Tiles[i][t][30].Wall();
        }
    }
    if (Debug)
        std::cout << "Placing Stone \n";
    for (int i = 41; i != 58; i++)
    {
        for (int t = 41; t != 57; t++)
        {
            Tiles[i][t][30].Stone();
        }
    }
    Tiles[40][48][30].Stone();
    Tiles[58][48][30].Stone();
    Tiles[49][40][30].Stone();
    Tiles[49][57][30].Stone();
    //globals::groundmap[6][49][49] = 10;
    //globals::groundmap[6][49][50] = 10;
    //globals::groundmap[6][49][51] = 10;
    //globals::groundmap[6][49][52] = 10;

    // ----------------------------------------

    {
        if (Debug)
        {
            std::cout << "PreInt\n";
        }
        try
        {
            for (int count = 0; count <= 2; count++)
            {
                sf::Vector2f vPos = math::CircleRandz(1000, 1000, 180);
                SpawnCritter("Human", vPos.x, vPos.y);
            }

            SpawnItem("Sword", 1039, 1020);

            SpawnItem("Gun", 1059, 1020);
            for (int i = 0; i != 5; i++)
                //Does not Run
                Con("Hunting for Trees 3");
            //SpawnItem("Fruit Tree",970,1050);
            //SpawnItem("Fruit Tree",950,1050);
            //SpawnItem("Fruit Tree",930,1050);

            SpawnItem("Broken Vending Machine", 1150, 1050);

            SpawnCritter("Mini Turret", 1000, 1000);
        }
        catch (std::exception &e)
        {
            std::cout << "Problem during initalization of NPC Spawns. \n";
        }
        if (Debug)
        {
            std::cout << "PostInt\n";
        }
    }
}

void GalaxyController::ZombieSwarmLocal(int num)
{
    {
        for (int count = 0; count <= num; count++)
        {
            sf::Vector2f Pos = math::CircleRandz(1000, 1000, 500);
            SpawnCritter("Zombie", Pos.x, Pos.y);
        }
    }
}

void GalaxyController::BuildMainMenu()
{
    //Effectz.CreateSquare(-1000,-1000,10000,10000,Cyan);
    sf::Sprite Sprt;
    Sprt.setTexture(*texturemanager.getTexture("Main.png"));

    App.draw(Sprt);
}

void GalaxyController::BuildTileTest()
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
            T.ID = randz(0, 10);
            //Tiles.push_back(T);
        }
    }
}

void GalaxyController::BuildWorldTest()
{
    debug("Worlding it up. ");
    InitalizeWorldTiles();
    debug("Factioning it up. \n");
    //InitalizeFactions();
}

void GalaxyController::WorldLoop()
{
    debug("Starting world loop");
    for (size_t i = 0; i != UniFact.size(); i++)
    { // Running through factions, Probably needs it's own function.
        debug("BeginningFaction" + UniFact[i].Name);

        if (UniFact[i].Initialized == false)
        { // Spawning and assigning factions their starting territory.

            int x = randz(1, 99);
            int y = randz(1, 99);
            if (WorldMap[x][y].Owner == "")
            {
                WorldMap[x][y].Owner = UniFact[i].Name;

                Territory ClaimedTerritory;

                ClaimedTerritory.WorldTile = sf::Vector2i(x, y);

                UniFact[i].Territories.push_back(ClaimedTerritory);

                UniFact[i].Initialized = true;

                std::cout << UniFact[i].Name << " has claimed " << x << "/" << y
                          << std::endl;
            }
        }

        if (UniFact[i].Territories.size() != 0 &&
            static_cast<size_t>(UniFact[i].Members) <=
                UniFact[i].Territories.size() / 2)
        {
            if (randz(1, 100) <= UniFact[i].Members * 4)
            {
                UniFact[i].Members++;
            }
        }

        if ((UniFact[i].Aggressiveness * UniFact[i].Members) >=
            randz(0, 100 * (UniFact[i].Territories.size() / 4)))
        { // Factions aggression causes them to seek more territory.
            bool TileChange = false;

            if (UniFact[i].Territories.empty())
                continue;
            if (UniFact[i].Members <=
                0) /* Don't ask how it can be in the negatives. */
                continue;

            int ForMembers = UniFact[i].Members;
            for (int t = 0; t != ForMembers; t++)
            {
                int Territ = randz(0, UniFact[i].Territories.size() - 1);

                sf::Vector2i Short = UniFact[i].Territories[Territ].WorldTile;

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

                    WorldTile *WT = &WorldMap[Short.x + X][Short.y + Y];

                    if (WT->Owner == "")
                    { // Claim it for themselves.
                        WT->Owner = UniFact[i].Name;

                        Territory CT;

                        CT.WorldTile = sf::Vector2i(Short.x + X, Short.y + Y);
                        UniFact[i].Territories.push_back(CT);
                    }
                    else if (WT->Owner != UniFact[i].Name)
                    { // Declare War.
                        std::string Tenant = WT->Owner;

                        for (size_t z = 0; z != UniFact.size(); z++)
                        {
                            if (UniFact[z].Name == Tenant)
                            {
                                float TenantTech = 0;
                                float InvaderTech = 0;

                                TenantTech += UniFact[z].TechMedical;
                                TenantTech += UniFact[z].TechWeaponryBlunt;
                                TenantTech += UniFact[z].TechWeaponryEnergy;
                                TenantTech += UniFact[z].TechWeaponryExplosive;
                                TenantTech += UniFact[z].TechWeaponryMass;
                                TenantTech += UniFact[z].TechWeaponrySharp;

                                InvaderTech += UniFact[i].TechMedical;
                                InvaderTech += UniFact[i].TechWeaponryBlunt;
                                InvaderTech += UniFact[i].TechWeaponryEnergy;
                                InvaderTech += UniFact[i].TechWeaponryExplosive;
                                InvaderTech += UniFact[i].TechWeaponryMass;
                                InvaderTech += UniFact[i].TechWeaponrySharp;

                                float TenantTechnique =
                                    TenantTech +
                                    (TenantTech *
                                     (PercentIs(UniFact[z].Creativity, 35) *
                                      0.01));
                                float InvaderTechnique =
                                    InvaderTech +
                                    (InvaderTech *
                                     (PercentIs(UniFact[i].Creativity, 35) *
                                      0.01));

                                float TenantPower =
                                    TenantTechnique *
                                    (PercentIs(UniFact[z].Members, 60) * 0.01);
                                float InvaderPower =
                                    InvaderTechnique *
                                    (PercentIs(UniFact[i].Members, 60) * 0.01);

                                TenantPower = TenantPower * randz(7, 10);
                                InvaderPower = InvaderPower * randz(7, 10);

                                //std::cout << "Tenant's Power: " << TenantPower << ", Invader's Power: " << InvaderPower << std::endl;
                                if (InvaderPower > TenantPower)
                                {
                                    //UniFact[z].Members = Loss;
                                    //if(Members > 0) UniFact[i].Members = randz(UniFact[i].Members/16,UniFact[i].Members/4);
                                    if (UniFact[z].Members > 0)
                                        UniFact[z].Members -= 1;

                                    WT->Owner = UniFact[i].Name;

                                    Territory CT;

                                    CT.WorldTile =
                                        sf::Vector2i(Short.x + X, Short.y + Y);
                                    UniFact[i].Territories.push_back(CT);

                                    for (size_t k = 0;
                                         k != UniFact[z].Territories.size();
                                         k++)
                                    {
                                        if (UniFact[z]
                                                .Territories[k]
                                                .WorldTile ==
                                            sf::Vector2i(Short.x + X,
                                                         Short.y + Y))
                                        {
                                            UniFact[z].Territories[k].ToDelete =
                                                true;
                                            TileChange = true;
                                        }
                                    }
                                }
                                else
                                {
                                    if (UniFact[i].Members > 0)
                                        UniFact[i].Members -= 1;
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
                for (size_t Fact = 0; Fact != UniFact.size(); Fact++)
                {
                    //std::cout << UniFact[Fact].Name << " Factions: " << UniFact[Fact].Territories.size();
                    CleanTerritories(UniFact[Fact].Territories);
                    //std::cout << ", " << UniFact[Fact].Name << "  Factions: " << UniFact[Fact].Territories.size() << std::endl;
                }
            }
        }

        debug("EndingFaction" + UniFact[i].Name);
    }
    debug("Ending world loop");
}

GalaxyController::MenuPointerContainer::MenuPointerContainer()
{
    pItem = nullptr;
    pNPC = nullptr;
    pTile = nullptr;
}
