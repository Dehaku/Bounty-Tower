#include "BountyTower.h"

template <typename T> T &listAt(std::list<T> &list, size_t index)
{
    auto it = list.begin();
    std::advance(it, index);
    return *it;
}

template <typename T> void AnyDeletes(std::list<T> &list)
{

    //auto it = list.begin();
    //std::advance(it, index);
    /*
    for (auto &listings : list)
    {
        if(listings.toDelete)
        {
            std::cout << "To be deleted! \n";
        }
    }
    */
    bool done = false;
    while (done == false)
    {
        bool yet = false;
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            //std::cout << it->name << ",'s toDelete: " << it->toDelete << std::endl;
            if (it->toDelete)
            {
                list.erase(it);
                yet = true;
                break;
            }
        }
        if (yet == false)
        {
            done = true;
        }
    }

    //return;
}

void equipStarters()
{
    int currentCritter = 0;
    for(auto &member : npclist)
    {
        if(currentCritter == 0)
        {
            Item item = *getGlobalItem("Baton");
            member.inventory.push_back(item);
        }
        if(currentCritter == 1)
        {
            Item item = *getGlobalItem("Sword");
            member.inventory.push_back(item);
        }
        if(currentCritter == 2)
        {
            Item item = *getGlobalItem("Gun");
            Item bullet;
            bullet = *getGlobalItem("5.56mm");
            bullet.amount = 30;

            item.internalitems.push_back(bullet);
            member.inventory.push_back(item);
        }

        if(currentCritter == 3)
        {
            Item item = *getGlobalItem("Sniper");
            Item bullet;
            bullet = *getGlobalItem("5.56mm");
            bullet.amount = 30;

            item.internalitems.push_back(bullet);
            member.inventory.push_back(item);
        }
        currentCritter++;
    }
}

void bountyTowerSetup()
{
    assignSlotPos();
    gCtrl.phase = "Lobby";
    window.setFramerateLimit(60); // 0 is unlimited
    //UnyTiles.makeTest();
    window.setView(gvars::view1);
    gvars::currentx = 32;
    gvars::currenty = 18;
    gvars::currentz = 1;
    {
        int Amt = randz(5,30);
        for(int i = 0; i != Amt; i++)
        {
            Tower tower;
            towers.push_back(tower);
        }
    }

    Faction * FactPtr;

    FactPtr = addFaction("The Titanium Grip");
    std::cout << FactPtr->name << " added. \n";

    conFact = &listAt(uniFact,0);
    conFact->playerControlled = false;
    conFact = &listAt(uniFact,1);

    FactionRelation factR;

    factR.faction = "Towerlings";
    factR.appeal = -1337;

    conFact->factRelations.push_back(factR);

    addMembers(4,"The Titanium Grip");
    equipStarters();


    FactPtr = addFaction("Towerlings");
    std::cout << FactPtr->name << " added. \n";
    factR.faction = "The Titanium Grip";
    FactPtr->factRelations.push_back(factR);


    //gCtrl.menuType = "BTTowers";
    //menuPopUp();

    //gvars::debug = true;
}

int elevateElevatorInhabitants()
{
    int AmountRaised = 0;
    for(auto &npc : npclist)
    {
        if(tiles[abs_to_index(npc.xpos/GRID_SIZE)][abs_to_index(npc.ypos/GRID_SIZE)][gvars::currentz].id == 3202)
        {
            std::cout << npc.name << " has arrisen. \n";
            npc.zpos += GRID_SIZE;
            AmountRaised++;
        }
    }

    std::cout << "Arrisen: " << AmountRaised << std::endl;
    return AmountRaised;
}

class debugTileTracker
{
public:
    Tile useTile;
    sf::Texture tex;
    debugTileTracker()
    {
        useTile.id = -1;
    }
};
debugTileTracker debugTileKeeper;

void tileWipe()
{
    for(int x = 0; x != GRIDS; x++)
        for(int y = 0; y != GRIDS; y++)
            for(int z = 0; z != CHUNK_SIZE; z++)
    {
        tiles[x][y][z].BTgrass();
    }
}

void debugTileMode()
{
    int menuEdgeL = gvars::centerScreen.x+300;
    int menuEdgeU = gvars::topLeft.y;
    effects.createSquare(menuEdgeL,gvars::topLeft.y,gvars::topRight.x,gvars::bottomLeft.y,sf::Color::Black);
    int mouseX = gvars::mousePos.x/GRID_SIZE;
    int mouseY = gvars::mousePos.y/GRID_SIZE;


    if(debugTileKeeper.useTile.id != -1 && aabb(mouseX,mouseY,0,GRIDS-1,0,GRIDS-1))
    {
        debugTileKeeper.tex = *debugTileKeeper.useTile.img.getTexture();
        sf::Vector2f vPos((gvars::mousePos.x/GRID_SIZE)*GRID_SIZE,(gvars::mousePos.y/GRID_SIZE)*GRID_SIZE);
        createImageButton(vPos,debugTileKeeper.tex);

        if(inputState.lmbTime > 5)
        {
            //tiles[gvars::mousePos.x/GRID_SIZE][gvars::mousePos.y/GRID_SIZE][gvars::currentz] = debugTileKeeper.useTile;
            tiles[gvars::mousePos.x/GRID_SIZE][gvars::mousePos.y/GRID_SIZE][gvars::currentz].setTilebyID(debugTileKeeper.useTile.id);
        }


    }
    if(inputState.key[Key::Space]) // Colors all walkable tiles
    for(int x = 0; x != GRIDS; x++)
        for(int y = 0; y != GRIDS; y++)
    {
        if(tiles[x][y][gvars::currentz].walkable)
        {
            effects.createSquare(x*GRID_SIZE,y*GRID_SIZE,(x+1)*GRID_SIZE,(y+1)*GRID_SIZE,sf::Color(0,255,255,100));
        }
    }

    if(inputState.key[Key::H])
    {// Identifies currently aimed Tile
        std::cout << "Tile(ID/Walkable/transparent/health/deathID/goesUp/GoesDown/State): "
        << std::endl
        << tiles[gvars::mousePos.x/GRID_SIZE][gvars::mousePos.y/GRID_SIZE][gvars::currentz].id
        << "/"
        << tiles[gvars::mousePos.x/GRID_SIZE][gvars::mousePos.y/GRID_SIZE][gvars::currentz].walkable
        << "/"
        << tiles[gvars::mousePos.x/GRID_SIZE][gvars::mousePos.y/GRID_SIZE][gvars::currentz].transparent
        << "/"
        << tiles[gvars::mousePos.x/GRID_SIZE][gvars::mousePos.y/GRID_SIZE][gvars::currentz].health
        << "/"
        << tiles[gvars::mousePos.x/GRID_SIZE][gvars::mousePos.y/GRID_SIZE][gvars::currentz].deathID
        << "/"
        << tiles[gvars::mousePos.x/GRID_SIZE][gvars::mousePos.y/GRID_SIZE][gvars::currentz].goesUp
        << "/"
        << tiles[gvars::mousePos.x/GRID_SIZE][gvars::mousePos.y/GRID_SIZE][gvars::currentz].goesDown
        << "/"
        << tiles[gvars::mousePos.x/GRID_SIZE][gvars::mousePos.y/GRID_SIZE][gvars::currentz].state
        << std::endl;
    }

    if(inputState.key[Key::LShift] && inputState.key[Key::LControl] && inputState.key[Key::O])
    { // Saves map
        if(inputState.key[Key::O].time == 30)
        {
            std::string outPut = "Hold for another ten seconds to save the current map layout, This cannot be undone. \n";
            std::cout << outPut;
            std::cout << outPut;
            std::cout << outPut;
            chatBox.addChat(outPut,sf::Color::Red);
            chatBox.addChat(outPut,sf::Color::Red);
            chatBox.addChat(outPut,sf::Color::Red);
            soundmanager.playSound("Startup.wav");
        }
        if(inputState.key[Key::O].time == 300)
        {
            saveMap(636,0,0,50,50);
            soundmanager.playSound("Startup.wav");
            std::string outPut = "The current map has been saved! \n";
            std::cout << outPut;
            chatBox.addChat(outPut,sf::Color::Red);
        }
    }

    if(inputState.key[Key::LShift] && inputState.key[Key::LControl] && inputState.key[Key::P])
    { // Loads map
        if(inputState.key[Key::P].time == 30)
        {
            std::string outPut = "Hold for another ten seconds to load the current map layout, This cannot be undone. \n";
            std::cout << outPut;
            std::cout << outPut;
            std::cout << outPut;
            chatBox.addChat(outPut,sf::Color::Blue);
            chatBox.addChat(outPut,sf::Color::Blue);
            chatBox.addChat(outPut,sf::Color::Blue);
            soundmanager.playSound("Startup.wav");
        }
        if(inputState.key[Key::P].time == 300)
        {
            tileWipe();
            loadMap(636,0,0,50,50);
            soundmanager.playSound("Startup.wav");
            std::string outPut = "The current map has been loaded! \n";
            std::cout << outPut;
            chatBox.addChat(outPut,sf::Color::Blue);
        }
    }


    if(inputState.key[Key::G].time == 10)
    {
        tileWipe();
    }

    int wallButt = createImageButton(sf::Vector2f(menuEdgeL+30,menuEdgeU+30),texturemanager.getTexture("FMTwallcheat2.png"));
    if(imageButtonClicked(wallButt))
        debugTileKeeper.useTile.BTwall();

    int floorButt = createImageButton(sf::Vector2f(menuEdgeL+90,menuEdgeU+30),texturemanager.getTexture("FMTtile1.png"));
    if(imageButtonClicked(floorButt))
        debugTileKeeper.useTile.BTstone();

    int fakewallButt = createImageButton(sf::Vector2f(menuEdgeL+30+60+60,menuEdgeU+30),texturemanager.getTexture("FMTwallcheat.png"));
    if(imageButtonClicked(fakewallButt))
        debugTileKeeper.useTile.BTwallFake();

    //
    int grassButt = createImageButton(sf::Vector2f(menuEdgeL+30,menuEdgeU+30+60),texturemanager.getTexture("BTGrass.png"));
    if(imageButtonClicked(grassButt))
        debugTileKeeper.useTile.BTgrass();
    int waterButt = createImageButton(sf::Vector2f(menuEdgeL+30+60,menuEdgeU+30+60),texturemanager.getTexture("BTWater.png"));
    if(imageButtonClicked(waterButt))
        debugTileKeeper.useTile.BTwater();
    int skyButt = createImageButton(sf::Vector2f(menuEdgeL+30+60+60,menuEdgeU+30+60),texturemanager.getTexture("BTSky.png"));
    if(imageButtonClicked(skyButt))
        debugTileKeeper.useTile.BTsky();

    int switchButt = createImageButton(sf::Vector2f(menuEdgeL+30,menuEdgeU+30+60+60),texturemanager.getTexture("Switchboard.png"));
    if(imageButtonClicked(switchButt))
        debugTileKeeper.useTile.BTswitch();

    int stairButt = createImageButton(sf::Vector2f(menuEdgeL+30+60,menuEdgeU+30+60+60),texturemanager.getTexture("Stairs.png"));
    if(imageButtonClicked(stairButt))
        debugTileKeeper.useTile.BTstairs();

    int doorButt = createImageButton(sf::Vector2f(menuEdgeL+30+60+60,menuEdgeU+30+60+60),texturemanager.getTexture("BTDoorTile.png"));
    if(imageButtonClicked(doorButt))
        debugTileKeeper.useTile.BTdoor();

    int elevatorButt = createImageButton(sf::Vector2f(menuEdgeL+30+60,menuEdgeU+30+60+60+60),texturemanager.getTexture("BTElevator.png"));
    if(imageButtonClicked(elevatorButt))
        debugTileKeeper.useTile.BTelevator();

    int elevatordoorButt = createImageButton(sf::Vector2f(menuEdgeL+30+60+60,menuEdgeU+30+60+60+60),texturemanager.getTexture("BTElevatorDoor.png"));
    if(imageButtonClicked(elevatordoorButt))
        debugTileKeeper.useTile.BTelevatordoor();




}

void towerMenu()
{
    //Lock camera in a reasonable position.
    gvars::currentx = 32;
    gvars::currenty = 18;
    //Screen position defaults
    int xPart = RESOLUTION.x/5;
    int yPart = RESOLUTION.y/3;
    int xMinus = xPart/2;
    int yMinus = yPart/2;

    //Build the modern fantasy tower.
    Tower fantasyTower;
    {
        fantasyTower.bountyPay = 2000;
        fantasyTower.bountyTarget = "Trifecta";
        fantasyTower.floors = 5;
        fantasyTower.difficulty = 10;
        fantasyTower.minioncount = 100;
        fantasyTower.name = "FantasyModern";
        fantasyTower.tex = &texturemanager.getTexture("FantasyModern.png");
    }
    //Place a button for the tower.
    sf::Vector2f vPos((-xMinus)+xPart,(-yMinus)+yPart);
    int fanTowButt = createImageButton(vPos,*fantasyTower.tex);
    if(imageButtonClicked(fanTowButt))
    {


        bountytower::towerLoaded = fantasyTower.name;
        buildTower(fantasyTower.name);

        loadMap(636,0,0,50,50);

        int xview = (96*60)/20;
        gvars::currentx = xview/2;
        gvars::currenty = xview;

        for(auto &npc : npclist)
            npc.momentum = sf::Vector2f(0,0);
    }
    if(imageButtonHovered(fanTowButt))
    {
        int butty = createImageButton(gvars::mousePos,texturemanager.getTexture("MercRogueFem.png"));
        std::string textOut = "Tower: " + fantasyTower.name + ", (" + std::to_string(fantasyTower.difficulty) + "/" +
                                            std::to_string(fantasyTower.minioncount) + ") , (Difficulty/Minions).";
        textList.createText(gvars::mousePos.x,gvars::mousePos.y,15,sf::Color::Red,textOut);
    }
    //Draw some info about the thing.
    {
        std::string textOut = " Tower: " + fantasyTower.name +
        "\n Minions: " + std::to_string(fantasyTower.minioncount) +
        "\n Difficulty: " + std::to_string(fantasyTower.difficulty);
        textList.createText(vPos.x+50,vPos.y-25,15,sf::Color::Red,textOut);
    }

    effects.createCircle((-xMinus)+xPart,(-yMinus)+yPart,50,gvars::cycleBlue);
    effects.createCircle((-xMinus)+xPart*2,(-yMinus)+yPart,50,gvars::cycleBlue);
    effects.createCircle((-xMinus)+xPart*3,(-yMinus)+yPart,50,gvars::cycleBlue);
    effects.createCircle((-xMinus)+xPart*4,(-yMinus)+yPart,50,gvars::cycleBlue);
    effects.createCircle((-xMinus)+xPart*5,(-yMinus)+yPart,50,gvars::cycleRed);

    effects.createCircle((-xMinus)+xPart,(-yMinus)+yPart*2,50,gvars::cycleBlue);
    effects.createCircle((-xMinus)+xPart*2,(-yMinus)+yPart*2,50,gvars::cycleBlue);
    effects.createCircle((-xMinus)+xPart*3,(-yMinus)+yPart*2,50,gvars::cycleBlue);
    effects.createCircle((-xMinus)+xPart*4,(-yMinus)+yPart*2,50,gvars::cycleBlue);
    effects.createCircle((-xMinus)+xPart*5,(-yMinus)+yPart*2,50,gvars::cycleRed);

    effects.createCircle((-xMinus)+xPart,(-yMinus)+yPart*3,50,gvars::cycleBlue);
    effects.createCircle((-xMinus)+xPart*2,(-yMinus)+yPart*3,50,gvars::cycleBlue);
    effects.createCircle((-xMinus)+xPart*3,(-yMinus)+yPart*3,50,gvars::cycleBlue);
    effects.createCircle((-xMinus)+xPart*4,(-yMinus)+yPart*3,50,gvars::cycleBlue);
    effects.createCircle((-xMinus)+xPart*5,(-yMinus)+yPart*3,50,gvars::cycleRed);


}



class baseMenu
{
public:
    bool toDelete;
    std::string name;
    sf::Vector2f Pos;
    Npc *npc;

    baseMenu();

};

baseMenu::baseMenu()
{
    toDelete = false;
}

std::list<baseMenu> menus;

void squaddieMenu(Npc &npc)
{
    std::cout << "This squaddie has, Health: " << npc.health << ", items: " << npc.inventory.size() << std::endl;
    baseMenu sMenu;
    sMenu.name = "Squaddie Menu";
    sMenu.Pos = sf::Vector2f(RESOLUTION.x/2,RESOLUTION.y/2);
    sMenu.npc = &npc;
    menus.push_back(sMenu);
}

void drawMenus()
{
    window.setView(window.getDefaultView());

    for(auto &menu : menus)
    {
        sf::RectangleShape Rec;
        Rec.setOutlineColor(sf::Color::White);
        Rec.setOutlineThickness(5);

        Rec.setFillColor(sf::Color(200,200,100));
        Rec.setSize(sf::Vector2f(RESOLUTION.x-200,RESOLUTION.y-200));
        Rec.setOrigin(Rec.getSize().x/2,Rec.getSize().y/2);
        Rec.setPosition(menu.Pos);
        window.draw(Rec);

        if(menu.name == "Squaddie Menu")
        {
            Npc *npc = menu.npc;
            sf::Text text = createText(npc->name);
            text.setPosition(150,200);
            text.setColor(sf::Color::Black);
            window.draw(text);


            sf::Vector2f invPos(300,200);
            int x = 0, y = 0;
            for(auto &item : npc->inventory)
            {
                sf::Vector2f drawPos(invPos.x,invPos.y+(60*y));
                sf::Texture itemTex = *item.img.getTexture();
                //createImageButton(drawPos,itemTex,"",0,window.getDefaultView());
                createImageButton(drawPos,itemTex,"",0);
                /*

                sf::Sprite itemSpr;
                itemSpr.setTexture(itemTex);
                itemSpr.setPosition(drawPos);

                sf::Text itemName = createText(item.name);
                itemName.setPosition(drawPos);

                window.draw(itemSpr);
                window.draw(itemName);
                */
                y++;
            }
        }
    }
    window.setView(gvars::view1);
}

void bountyTowerLoop()
{
    if(bountytower::towerLoaded == "")
        towerMenu();
    if(myTargetPtr != nullptr && inputState.key[Key::I].time == 1)
    {
        squaddieMenu(*myTargetPtr);
    }

    drawMenus();


    if(gvars::tileEdit)
    {
        textList.createText(gvars::centerScreen.x,gvars::topLeft.y,10,gvars::cycleBlue,"Debug Tilemode Engaged");
        debugTileMode();
    }

    if(bountytower::pausewaves && bountytower::towerLoaded != "")
    {
        //sf::Vector2f vPos(gvars::centerScreen.x,gvars::centerScreen.y-(RESOLUTION.y/4));
        sf::Vector2f vPos(RESOLUTION.x/2,(RESOLUTION.y/2)-(RESOLUTION.y/4));
        int startButt = createImageButton(vPos,texturemanager.getTexture("ElevatorButton.png"),"Start the swarm!",0,window.getDefaultView());
        if(imageButtonClicked(startButt))
            bountytower::pausewaves = false;
    }


    cameraControls();

    if(inputState.key[Key::X].time == 1)
    { // Print Faction Names
        for(auto & fact : uniFact)
        {
            std::cout << fact.name << std::endl;
        }
    }

    if(inputState.key[Key::C].time == 1)
    { // Print All Items
        for(auto & item : itemlist)
        {
            std::cout << item.name << std::endl;
            std::cout << "X/Y/Z: " << item.xpos << "/" << item.ypos << "/" << item.zpos << std::endl;
        }
    }

    if(inputState.key[Key::V].time == 1)
    {
        Item item = *getGlobalItem("Scrap");
        item.xpos = gvars::mousePos.x;
        item.ypos = gvars::mousePos.y;
        item.zpos = gvars::currentz*GRID_SIZE;
        worlditems.push_back(item);

    }

    if(inputState.key[Key::G].time == 1)
    { // Renew Menu Towers
        towers.clear();
        int Amt = randz(5,30);
        for(int i = 0; i != Amt; i++)
        {
            Tower tower;
            towers.push_back(tower);
        }
    }

    if (gCtrl.menuType != "NULL")
    { // Menu Code
        menuPopUp();
    }
    else
    {
        gCtrl.menuPos = math::Vec2f(-10000, -10000);
    }


    rightMouseButtonContextMenu();

    critterBrain(npclist);


    if(inputState.key[Key::Space])
        for(auto &npc : npclist)
    { // Move NPC's towards mouse
        npc.dirMove(gvars::mousePos);
    }

    if(gCtrl.phase == "Lobby")
    {

    }



    if(inputState.key[Key::LShift] && inputState.key[Key::X].time == 1)
    { // Spit out some info about some states regard to global stuffs.
        std::string outPut = "elevatoravailable: " + std::to_string(bountytower::elevatoravailable)
                            + " towerlingassault: " + std::to_string(bountytower::towerlingassault)
                            + " bountytower: " + std::to_string(bountytower::bountytower);

        chatBox.addChat(outPut,sf::Color::Red);
    }

    if(inputState.key[Key::LShift] && inputState.key[Key::C].time == 1)
    { // Toggle pausewave, Mostly for debug purposes.
        toggle(bountytower::pausewaves);
        toggle(bountytower::elevatoravailable);
        std::string outPut = "***pausewaves has been toggled! pausewaves: " + std::to_string(bountytower::pausewaves);

        chatBox.addChat(outPut,sf::Color::Red);
    }

    if( (gvars::framesPassed % 5) == 0 && !bountytower::elevatoravailable)
    { // Here we scan the floor for all switches that are set to Off, If even a single one is off, then the elevator is not functional.
        bool floorComplete = true;
        for(int x = 0; x != GRIDS; x++)
            for(int y = 0; y != GRIDS; y++)
        {
            if(tiles[x][y][gvars::currentz].id == 3500 && tiles[x][y][gvars::currentz].state == "Off")
            {
                floorComplete = false;
            }
        }

        if(floorComplete)
        {
            bountytower::elevatoravailable = true;


            std::string outPut = "*Ding!*";
            chatBox.addChat(outPut,sf::Color(255,150,150));
            outPut = "*Elevator is now functional, You may now progress* ";
            chatBox.addChat(outPut,sf::Color(255,150,150));
            soundmanager.playSound("ding.wav"); // zomfg sounds, so gud.
        }


    }

    if( (gvars::framesPassed % 300) == 0 && npclist.size() < 20 && !bountytower::pausewaves)
    { // This looks for stairs, then spawns critters from them every 300 frames until a certain cap is met.
        debug("Gettin Stairs");
        std::vector<Tile*> stairs;
        for(int x = 0; x != GRIDS; x++)
            for(int y = 0; y != GRIDS; y++)
        {
            if(tiles[x][y][gvars::currentz].id == 2031)
                stairs.push_back(&tiles[x][y][gvars::currentz]);
        }
        debug("Placin Towerlings");
        for(auto &stair : stairs)
        {
            Npc member;
            debug("V");
            member = *getGlobalCritter("BTHalfCelestial");
            debug("X");
            member.faction = "Towerlings";
            debug("Y");
            member.factionPtr = &listAt(uniFact,2);
            debug("Z");
            //member.xpos = ((GRIDS*GRID_SIZE)/2)+randz(-20,20);
            //member.ypos = ((GRIDS*GRID_SIZE)-100)+randz(-20,20);
            member.xpos = stair->pos.x*GRID_SIZE+(GRID_SIZE/2); //((GRIDS*GRID_SIZE)/2)+randz(-20,20);
            member.ypos = stair->pos.y*GRID_SIZE+(GRID_SIZE/2);
            member.zpos = (gvars::currentz*GRID_SIZE);
            member.id = gvars::globalid++;

            itemPtrVector IPV = randomEquipment(member.inventory);

            //std::cout << IPV.ptrs.size() << ", IPV. \n";
            //fSleep(2);
            for (auto &i : IPV.ptrs)
            {
                if(i->type == 2)
                {
                    Item bullet;
                    bullet = *getGlobalItem("5.56mm");
                    bullet.amount = 30;

                    i->internalitems.push_back(bullet);
                }
            }

            printItems(member.inventory);

            npclist.push_back(member);
        }
        debug("Done placin Towerlings");
    }

    if(bountytower::towerLoaded != "")
        textList.createText(sf::Vector2f(RESOLUTION.x/2,15),15,sf::Color::White,"Floor: " + std::to_string(gvars::currentz),window.getDefaultView());
        //textList.createText(gvars::centerScreen.x,gvars::topLeft.y+15,15,sf::Color::White,"Floor: " + std::to_string(gvars::currentz));

    if(bountytower::elevatoravailable && bountytower::towerLoaded != "")
    { // Prints Elevator HUD and other such things


        textList.createText(sf::Vector2f(RESOLUTION.x/2,50),20,sf::Color::Green,"Elevator is Ready!",window.getDefaultView());

        int AmountRaised = 0;
        for(auto &npc : npclist)
        {
            if(tiles[abs_to_index(npc.xpos/GRID_SIZE)][abs_to_index(npc.ypos/GRID_SIZE)][gvars::currentz].id == 3202)
            {
                AmountRaised++;
            }
        }
        //textList.createText(gvars::centerScreen.x,gvars::topLeft.y+70,20,sf::Color::White,"On Elevator: " + std::to_string(AmountRaised));
        textList.createText(sf::Vector2f(RESOLUTION.x/2,70),20,sf::Color::White,"On Elevator: " + std::to_string(AmountRaised),window.getDefaultView());

        //sf::Vector2f vPos(gvars::centerScreen.x-25,gvars::topLeft.y+75);
        sf::Vector2f vPos((RESOLUTION.x/2)-25,75);
        int butt = createImageButton(vPos,texturemanager.getTexture("ElevatorButton.png"),"",0,window.getDefaultView());
        if(imageButtonClicked(butt))
        {
            if (gvars::currentz < CHUNK_SIZE - 1) {
                chatBox.addChat("You progress to the next floor!", sf::Color::Blue);
                bountytower::elevatoravailable = false;
                bountytower::pausewaves = true;
                gvars::currentz++;
                elevateElevatorInhabitants();
                soundmanager.playSound("ding.wav"); // zomfg sounds, so gud.
            } else {
                chatBox.addChat("You cannot go further up.", sf::Color::Red);
            }
        }
    }

    //lmbPress();

}

void buildTower(std::string towerName)
{
    bountytower::elevatoravailable = false;
    bountytower::pausewaves = true;
    if(towerName == "FantasyModern")
    {
        for (int x = 0; x != GRIDS; x++)
            for (int y = 0; y != GRIDS; y++)
                for (int z = 0; z != CHUNK_SIZE; z++)
        {
            if(z == 0 || z == 1)
                tiles[x][y][z].BTgrass();
            else
                tiles[x][y][z].BTsky();
            if(aabb(x,y,31,64,31,64))
            {
                tiles[x][y][z].BTwall();
            }
            if(aabb(x,y,32,63,32,63))
            {
                tiles[x][y][z].BTstone();
            }

            if(y == 64)
            {
                tiles[48][64][1].BTdoor();
                tiles[47][64][1].BTdoor();
            }


            if( (x == 48 || x == 47) && y > 64 && z == 1)
                tiles[x][y][z].BTstone();

            if(y == 32)
            {
                tiles[48][32][z].BTstairs();
                tiles[47][32][z].BTstairs();
            }

            if(x == 32)
            {
                tiles[32][48][z].BTstairs();
                tiles[32][47][z].BTstairs();
            }

            if(x == 63)
            {
                tiles[63][48][z].BTstairs();
                tiles[63][47][z].BTstairs();
            }



            if(aabb(x,y,47,50,47,50))
            {
                tiles[x][y][z].BTwall();
            }
            if(aabb(x,y,48,49,48,49))
            {
                tiles[x][y][z].BTelevator();
            }

            if(x == 48 || x == 49)
            {
                if(y == 47)
                {
                    tiles[48][47][z].BTelevatordoor();
                    tiles[49][47][z].BTelevatordoor();
                }
            }

            if(y == 32)
            {
                if(x == 51)
                    tiles[51][32][z].BTswitch();
                if(x == 44)
                    tiles[44][32][z].BTswitch();

            }
        }
    }

}

Tower::Tower()
{
    tex = &texturemanager.getTexture("FantasyModern.png");
    name = "The Tower mk" + std::to_string(randz(1,10));
    difficulty = randz(10,100);
    minioncount = difficulty * randz(3,10);
    bountyTarget = "Dudeman mk" + std::to_string(randz(1,300));
    bountyPay = (difficulty/10) * (minioncount / 10);
}

std::vector<Tower> towers;

void bountyBrain(Npc &npc, std::list<Npc> &container)
{

}

namespace bountytower
{
    bool elevatoravailable = false;
    bool towerlingassault = true;
    bool bountytower = true;
    bool pausewaves = true;
    std::string towerLoaded = "";
}
