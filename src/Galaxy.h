#ifndef __GALAXY_H_INCLUDED__
#define __GALAXY_H_INCLUDED__



class GalaxyController
{ //
    public:
    std::string Phase;

    float timescale;
    float timescalez;
    float timetilltick;
    int Waves;
    int WaveTimer;
    int TickTimer;
    bool bTest;
    bool Debug;
    sf::Vector2f MenuPos;
    sf::Vector2f MenuEndPos;
    std::string MenuType;
    //std::vector<Planet> vPlanets;

    GalaxyController(){
        Debug = false;
        bTest = false;
        MenuType = "NULL";
        timescale = 10;
        timescalez = 10;
        timetilltick = 0;
        TickTimer = 10;
        WaveTimer = 1000;
        Waves = 1;
    }

    class MenuPointerContainer
    {
        public:
        item * pItem;
        NPC * pNPC;
        Tile * pTile;

        std::vector<item*> pVecItem;
        std::vector<NPC*> pVecNPC;
        std::vector<Tile*> pVecTile;

        MenuPointerContainer()
        {
            pItem = NULL;
            pNPC = NULL;
            pTile = NULL;
        }
    };
    MenuPointerContainer MenuPtrCon;


    void AddvPlanet()
    {
        unsigned short Grid[32][32][32];

        //long long unsigned int Rawr = (sizeof(vChunk)*10000);
        std::cout << (32*32)*32 << std::endl;
        short Cake;

        //std::cout << "Grid: " << (sizeof(vChunk)*1000000) << " vs Tile:" << Rawr << std::endl;
        //std::cout << "Grid: " << (sizeof(Grid)) << " vs Tile:" << sizeof(xTile) << std::endl;

    }


    void Wave(){
        bool FoundAny = false;
        for(std::vector<NPC>::iterator i = npclist.begin(); i != npclist.end(); i++)
        {
            if(i->HasTag("[EatsFlesh:") && i->health > 0)
            {
                FoundAny = true;
            }
        }
        if(FoundAny == false)
        {
            WaveTimer--;
            if(WaveTimer <= 0)
            {
                ZombieSwarmLocal(Waves);
                WaveTimer = 1000;
                Waves++;
            }
            cText.CreateText( ((currentx-2)*GridSize)+1,((currenty-18)*GridSize)+1,11,Black,"Time Till Wave ","",Waves+1,": ","",WaveTimer/10);
            if(WaveTimer/2 < 50 && randz(0,1 == 1))
            {
               cText.CreateText((currentx-2)*GridSize,(currenty-18)*GridSize,11,White,"Time Till Wave ","",Waves+1,": ","",WaveTimer/10);
            }
            else
            {
                cText.CreateText((currentx-2)*GridSize,(currenty-18)*GridSize,11,Red,"Time Till Wave ","",Waves+1,": ","",WaveTimer/10);
            }
        }
    }

    void Time(int Modify){
        if(timetilltick >= TickTimer)
        {
            timetilltick = 0;
            //UpdatePlanet();
        }
        timetilltick += timescalez;
    }

    void BuildLocal(std::string Type = "Test", int ZedAmount = 0){ // TODO: Replace this with a Class containing Critter types and amounts, Probably inventories as well, Or start up premade critters.
        if(Type == "Test")
        {
            if(Debug) std::cout << "Building Lightmap\n";
            int z = currentz;
            for (int i = 0; i != gridy; i++)
            {
                for( int t = 0; t != gridx; t++)
                {
                    //sunmap[z][i][t] = 255;
                }
            }
        // ----------------------------------------
        // 1 dirt 3 grass 6 sand 7 stone 10 wall 20 dirtchannel 52 deepwater 53 lava
        if(Debug) std::cout << "Creating Mass Tiles\n";
            for (int i = 0; i != Grids; i++)
            {
                for( int t = 0; t != Grids; t++)
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
            if(Debug) std::cout << "Placing Dirt\n";
            for (int i = 32; i != 64; i++)
            {
                for( int t = 32; t != 64; t++)
                {
                    Tiles[i][t][30].Dirt();
                }
            }
            if(Debug) std::cout << "Placing Walls\n";
            for (int i = 40; i != 59; i++)
            {
                for( int t = 40; t != 58; t++)
                {
                    Tiles[i][t][30].Wall();
                }
            }
            if(Debug) std::cout << "Placing Stone \n";
            for (int i = 41; i != 58; i++)
            {
                for( int t = 41; t != 57; t++)
                {
                    Tiles[i][t][30].Stone();
                }
            }
            Tiles[40][48][30].Stone();
            Tiles[58][48][30].Stone();
            Tiles[49][40][30].Stone();
            Tiles[49][57][30].Stone();
            //groundmap[6][49][49] = 10;
            //groundmap[6][49][50] = 10;
            //groundmap[6][49][51] = 10;
            //groundmap[6][49][52] = 10;

            // ----------------------------------------

            std::vector<NPC>::iterator zit;
            std::vector<item>::iterator zitz;

            if(true == true)
            {
                int testnum = 0;
                if(Debug){std::cout<<"PreInt\n";}
                try
                {
                    for( int count = 0; count <= 2; count++)
                    {
                        sf::Vector2f vPos = Math.CircleRandz(1000,1000,180);
                        SpawnCritter("Human",vPos.x,vPos.y);
                    }

                    SpawnItem("Sword",1039,1020);

                    SpawnItem("Gun",1059,1020);
                    Con("Hunting for Trees 1");
                    for(int i = 0; i != 5; i++)
                    {
                        int Tx,Ty = randz(0,2000);
                        if(Tiles[abs(Tx/20)][abs(Ty/20)][30].ID == 1001 || Tiles[abs(Tx/20)][abs(Ty/20)][30].ID == 1003)
                        {
                            SpawnItem("Fruit Tree",Tx,Ty);
                        }
                    }
                    //SpawnItem("Fruit Tree",970,1050);
                    //SpawnItem("Fruit Tree",950,1050);
                    //SpawnItem("Fruit Tree",930,1050);

                    SpawnItem("Broken Vending Machine",1150,1050);

                    SpawnCritter("Mini Turret",1000,1000);

                }catch (std::exception& e){std::cout << "Problem during initalization of NPC Spawns. \n";}
                if(Debug){std::cout<<"PostInt\n";}
            }
        }
        if(Type == "City")
        {
            // ----------------------------------------
            // 1 dirt 3 grass 6 sand 7 stone 10 wall 20 dirtchannel 52 deepwater 53 lava
            debug("Creating Mass Tiles");
            for (int i = 0; i != Grids; i++)
            {
                for( int t = 0; t != Grids; t++)
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
            if(Debug) std::cout << "Placing Dirt\n";
            for (int i = 32; i != 64; i++)
            {
                for( int t = 32; t != 64; t++)
                {
                    Tiles[i][t][30].Dirt();
                }
            }
            if(Debug) std::cout << "Placing Walls\n";
            for (int i = 40; i != 59; i++)
            {
                for( int t = 40; t != 58; t++)
                {
                    Tiles[i][t][30].Wall();
                }
            }
            if(Debug) std::cout << "Placing Stone \n";
            for (int i = 41; i != 58; i++)
            {
                for( int t = 41; t != 57; t++)
                {
                    Tiles[i][t][30].Stone();
                }
            }
            Tiles[40][48][30].Stone();
            Tiles[58][48][30].Stone();
            Tiles[49][40][30].Stone();
            Tiles[49][57][30].Stone();

            std::vector<NPC>::iterator zit;
            std::vector<item>::iterator zitz;

            if(true == true)
            {
                int testnum = 0;
                if(Debug){std::cout<<"PreInt\n";}
                try
                {
                    /*for( int count = 0; count <= 2; count++)
                    {
                        sf::Vector2f vPos = Math.CircleRandz(1000,1000,180);
                        SpawnCritter("Human",vPos.x,vPos.y);
                    }*/
                    for( int count = 0; count != Squady.Squad.size(); count++)
                    {
                        sf::Vector2f vPos = Math.CircleRandz(1000,1000,180);
                        //SpawnCritter("Human",vPos.x,vPos.y);
                        Squady.Squad.at(count).xpos = vPos.x;
                        Squady.Squad.at(count).ypos = vPos.y;

                        npcmanager.AddedCritters.push_back(Squady.Squad.at(count));
                    }


                    for(int zeds = 0; zeds != ZedAmount; zeds++)
                    {
                        Con("Starting Zed");
                        sf::Vector2f vPos = Math.CircleRandz(1000,1000,580);
                        SpawnCritter("Zombie",vPos.x,vPos.y);
                        Con("Ending Zed");
                    }

                    SpawnItem("Sword",1039,1020);

                    SpawnItem("Gun",1059,1020);
                    // Does Not Function
                    Con("Hunting for Trees 2");
                    //SpawnItem("Fruit Tree",970,1050);
                    //SpawnItem("Fruit Tree",950,1050);
                    //SpawnItem("Fruit Tree",930,1050);

                    SpawnItem("Broken Vending Machine",1150,1050);

                    SpawnCritter("Mini Turret",1000,1000);

                }catch (std::exception& e){std::cout << "Problem during initalization of NPC Spawns. \n";}
                if(Debug){std::cout<<"PostInt\n";}
            }
        }

        if(Type == "LoadTile")
        {

        }
  }




    void BuildLocalTest(){
        if(Debug) std::cout << "Building Lightmap\n";
        int z = currentz;
        for (int i = 0; i <= gridy-1; i++)
        {
            for( int t = 0; t <= gridx-1; t++)
            {
                //sunmap[z][i][t] = 255;
            }
        }
    // ----------------------------------------
    // 1 dirt 3 grass 6 sand 7 stone 10 wall 20 dirtchannel 52 deepwater 53 lava
    if(Debug) std::cout << "Creating Mass Tiles\n";
        for (int i = 0; i != Grids; i++)
        {
            for( int t = 0; t != Grids; t++)
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
        if(Debug) std::cout << "Placing Dirt\n";
        for (int i = 31; i != 64; i++)
        {
            for( int t = 31; t != 64; t++)
            {
                Tiles[i][t][30].Dirt();
                Con(t);
            }
        }
        if(Debug) std::cout << "Placing Walls\n";
        for (int i = 40; i != 59; i++)
        {
            for( int t = 40; t != 58; t++)
            {
                Tiles[i][t][30].Wall();
            }
        }
        if(Debug) std::cout << "Placing Stone \n";
        for (int i = 41; i != 58; i++)
        {
            for( int t = 41; t != 57; t++)
            {
                Tiles[i][t][30].Stone();
            }
        }
        Tiles[40][48][30].Stone();
        Tiles[58][48][30].Stone();
        Tiles[49][40][30].Stone();
        Tiles[49][57][30].Stone();
        //groundmap[6][49][49] = 10;
        //groundmap[6][49][50] = 10;
        //groundmap[6][49][51] = 10;
        //groundmap[6][49][52] = 10;

    // ----------------------------------------

        std::vector<NPC>::iterator zit;
        std::vector<item>::iterator zitz;

    if(true == true){
    int testnum = 0;
    if(Debug){std::cout<<"PreInt\n";}
    try{
    for( int count = 0; count <= 2; count++)
    {
        sf::Vector2f vPos = Math.CircleRandz(1000,1000,180);
        SpawnCritter("Human",vPos.x,vPos.y);
    }

    SpawnItem("Sword",1039,1020);

    SpawnItem("Gun",1059,1020);
    for(int i = 0; i != 5; i++)
                    //Does not Run
                    Con("Hunting for Trees 3");
    //SpawnItem("Fruit Tree",970,1050);
    //SpawnItem("Fruit Tree",950,1050);
    //SpawnItem("Fruit Tree",930,1050);

    SpawnItem("Broken Vending Machine",1150,1050);

    SpawnCritter("Mini Turret",1000,1000);

    }catch (std::exception& e){std::cout << "Problem during initalization of NPC Spawns. \n";}
    if(Debug){std::cout<<"PostInt\n";}
    }

  }

    void ZombieSwarmLocal(int num = 5){
        std::vector<NPC>::iterator zit;
        if(true == true)
        {
            for( int count = 0; count <= num; count++)
            {
                sf::Vector2f Pos = Math.CircleRandz(1000,1000,500);
                SpawnCritter("Zombie",Pos.x,Pos.y);
            }
        }
    }
/*
    bool BuildSolarTest(){
        int testnum;
        std::vector<planet>::iterator zit;
        testnum = AddPlanet("Sun", 0);
        for(zit = Planets.begin(); zit != Planets.end(); ++zit )
        {
            if(zit->id == testnum)
            {
                // TODO: Fix Laterzit->SetImgSun();
                zit->spawn(0,0,1200,1200);
            }
        }
    }


    bool CreatePlanet(int x, int y, int mass, sf::Vector2f Vel = sf::Vector2f(0,0)){
        std::vector<planet>::iterator zit;
        int testnum = 0;
        testnum = AddPlanet("Planet", 0);
        for(zit = Planets.begin(); zit != Planets.end(); ++zit )
        {
            if(zit->id == testnum)
            {
                zit->mass = 50;
                // TODO: Fix Laterzit->SetImgPlanet();
                // TODO: Fix Laterzit->img.Scale(zit->mass/100,zit->mass/100);
                zit->spawn(0,0,x,y);
            }
        }
    }
*/
    void BuildMainMenu(){
        //Effectz.CreateSquare(-1000,-1000,10000,10000,Cyan);
        sf::Sprite Sprt;
        Sprt.setTexture(*imagemanager.GetImage("Main.png"));

        App.draw(Sprt);
    }

    void BuildTileTest(){// Build the galaxy.
    //for(int gx = 0; gx != 100; gx++){
        // for(int gy = 0; gy != 100; gy++){
            // for(int rx = 0; rx != 100; rx++){
                // for(int ry = 0; ry != 100; ry++){
                    for(int lx = 0; lx != 100; lx++){
                        for(int ly = 0; ly != 100; ly++){
                            Tile T;
                            //T.GalPos = sf::Vector2f(50,50);
                            //T.RegPos = sf::Vector2f(50,50);
                            //T.LocPos = sf::Vector2f(lx,ly);
                            T.ID = randz(0,10);
                            //Tiles.push_back(T);
                        }
                    }

                // }
            // }
        // }
    // }

    std::vector<Tile>::iterator T;

    }

    void BuildWorldTest(){
        debug("Worlding it up. ");
        InitalizeWorldTiles();
        debug("Factioning it up. \n");
        //InitalizeFactions();

    }

    void WorldLoop()
    {
        debug("Starting world loop");
        for(int i = 0; i != UniFact.size(); i++)
        { // Running through factions, Probably needs it's own function.
            debug(AddString("BeginningFaction",UniFact[i].Name));

            if(UniFact[i].Territories.size() != 0 && UniFact[i].Members <= UniFact[i].Territories.size()/2)
            {
                if(randz(1,100) <= UniFact[i].Members*4)
                {
                    UniFact[i].Members++;
                }
            }



            if( (UniFact[i].Aggressiveness * UniFact[i].Members) >= randz(0,100*(UniFact[i].Territories.size()/4) ))
            {// Factions aggression causes them to seek more territory.
                bool TileChange = false;
                for(int t = 0; t != UniFact[i].Territories.size(); t++)
                {
                    sf::Vector2i Short = UniFact[i].Territories[t].WorldTile;
                    if(Short.x > 0 && Short.x < 99 && Short.y > 0 && Short.y < 99)
                    {
                        int Direction = randz(0,3);
                        int X = 0;
                        int Y = 0;
                        if(Direction == 0)
                        {
                             X = 0;
                             Y = -1;
                        }
                        if(Direction == 1)
                        {
                             X = +1;
                             Y = 0;
                        }
                        if(Direction == 2)
                        {
                             X = 0;
                             Y = +1;
                        }
                        if(Direction == 3)
                        {
                             X = -1;
                             Y = 0;
                        }

                            WorldTile * WT = &WorldMap[Short.x+X][Short.y+Y];

                            if(WT->Owner == "")
                            {// Claim it for themselves.
                                WT->Owner = UniFact[i].Name;

                                Territory CT;

                                CT.WorldTile = sf::Vector2i(Short.x+X,Short.y+Y);
                                UniFact[i].Territories.push_back(CT);
                            }
                            else if(WT->Owner != UniFact[i].Name)
                            {// Declare War.
                                std::string Tenant = WT->Owner;

                                for(int z = 0; z != UniFact.size(); z++)
                                {
                                    if(UniFact[z].Name == Tenant)
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

                                        float TenantTechnique = TenantTech + (TenantTech*(PercentIs(UniFact[z].Creativity,35)*0.01));
                                        float InvaderTechnique = InvaderTech + (InvaderTech*(PercentIs(UniFact[i].Creativity,35)*0.01));

                                        float TenantPower = TenantTechnique*(PercentIs(UniFact[z].Members, 60)*0.01);
                                        float InvaderPower = InvaderTechnique*(PercentIs(UniFact[i].Members, 60)*0.01);

                                        TenantPower = TenantPower*randz(7,10);
                                        InvaderPower = InvaderPower*randz(7,10);


                                        //std::cout << "Tenant's Power: " << TenantPower << ", Invader's Power: " << InvaderPower << std::endl;
                                        if(InvaderPower > TenantPower)
                                        {
                                            int Members = UniFact[z].Members;
                                            int Loss = Members/UniFact[z].Territories.size();
                                            //UniFact[z].Members = Loss;
                                            //if(Members > 0) UniFact[i].Members = randz(UniFact[i].Members/16,UniFact[i].Members/4);

                                            WT->Owner = UniFact[i].Name;

                                            Territory CT;

                                            CT.WorldTile = sf::Vector2i(Short.x+X,Short.y+Y);
                                            UniFact[i].Territories.push_back(CT);

                                            for(int k = 0; k != UniFact[z].Territories.size(); k++)
                                            {
                                                if(UniFact[z].Territories[k].WorldTile == sf::Vector2i(Short.x+X,Short.y+Y))
                                                {
                                                    UniFact[z].Territories[k].ToDelete = true;
                                                    TileChange = true;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            //UniFact[i].Members = randz(UniFact[i].Members/4,UniFact[i].Members);
                                            //UniFact[z].Members = randz(UniFact[z].Members/32,UniFact[z].Members/16);
                                        }
                                    }
                                }
                            }
                    }
                }

                if(TileChange)
                {
                    for(int Fact = 0; Fact != UniFact.size(); Fact++)
                    {
                        //std::cout << UniFact[Fact].Name << " Factions: " << UniFact[Fact].Territories.size();
                        CleanTerritories(UniFact[Fact].Territories);
                        //std::cout << ", " << UniFact[Fact].Name << "  Factions: " << UniFact[Fact].Territories.size() << std::endl;
                    }
                }
            }


            if(UniFact[i].Initialized == false)
            { // Spawning and assigning factions their starting territory.
                int x = randz(1,99);
                int y = randz(1,99);
                if(WorldMap[x][y].Owner == "")
                {
                    WorldMap[x][y].Owner = UniFact[i].Name;

                    Territory ClaimedTerritory;

                    ClaimedTerritory.WorldTile = sf::Vector2i(x,y);

                    UniFact[i].Territories.push_back(ClaimedTerritory);

                    UniFact[i].Initialized = true;

                    std::cout << UniFact[i].Name << " has claimed " << x << "/" << y << std::endl;

                }
            }
            debug(AddString("EndingFaction",UniFact[i].Name));
        }
        debug("Ending world loop");
    }
        //Finish playing with the territories, Either add more rules, or add relations, Perhaps reduce aggression by territory count, raise it by members, raise members by territory count, lose members during territory conflict.
};
GalaxyController GC;



#endif // GALAXY_H_INCLUDED
