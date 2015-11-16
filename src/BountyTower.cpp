#include "BountyTower.h"




std::vector<Npc*> Squaddies;
std::list<Npc> leftBehind;
std::list<Npc> recruitables;

bool onScreen(sf::Vector2f vPos)
{
    if(aabb(vPos,gvars::centerScreen.x-(screen.x()/2),gvars::centerScreen.x+(screen.x()/2),gvars::centerScreen.y-(screen.y()/2),gvars::centerScreen.y+(screen.y()/2)))
        return true;

    return false;
}

void screenShake(float intensity)
{
    gvars::screenShake += intensity;
}

void renderScreenShake()
{
    sf::Vector2f screenPos = gvars::view1.getCenter();


    if(randz(0,1) == 0)
        screenPos.x += randz(gvars::screenShake/2,gvars::screenShake);
    else
        screenPos.x -= randz(gvars::screenShake/2,gvars::screenShake);

    if(randz(0,1) == 0)
        screenPos.y += randz(gvars::screenShake/2,gvars::screenShake);
    else
        screenPos.y -= randz(gvars::screenShake/2,gvars::screenShake);

    gvars::view1.setCenter(screenPos);

    gvars::screenShake = math::clamp(gvars::screenShake - 0.5,0,100);
}

int gridIt(int num)
{
    return num/GRID_SIZE;
}

void setupSquadHotKeySelection()
{
    Squaddies.clear();
    for(auto &npc : npclist)
        if(npc.faction == "The Titanium Grip" && npc.isSquaddie)
            Squaddies.push_back(&npc);
}

void equipStarters()
{
    int currentCritter = 0;
    for(auto &member : npclist)
    {
        member.name = generateName();
        member.isSquaddie = true;

        member.setupAnimations();

        member.chasePriority = "Hold Position";

        member.health = member.getMaxHealth();

        member.ypos = 4040;

        if(random(1,2) == 1)
            member.tags.append("[WearsBoots:1]");
        if(random(1,2) == 1)
            member.tags.append("[WearsGloves:1]");

        if(currentCritter == 0)
        {
            Item item = *getGlobalItem("PDA");
            member.inventory.push_back(item);
            item = *getGlobalItem("Shotgun");
            //member.inventory.push_back(item);

            item = *getGlobalItem("Shell: Spread");
            item.amount = 30;
            //member.inventory.push_back(item);
        }
        if(currentCritter == 1)
        {
            Item item = *getGlobalItem("Sword");
            member.inventory.push_back(item);

            item = *getGlobalItem("RocketLauncher");
            member.inventory.push_back(item);

            item = *getGlobalItem("Missile");
            item.amount = 5;
            member.inventory.push_back(item);
            member.inventory.push_back(item);

        }
        if(currentCritter == 2)
        {
            Item item = *getGlobalItem("Gun");
            Item bullet;
            bullet = *getGlobalItem("Bullet: Standard");
            bullet.amount = 30;

            member.inventory.push_back(item);
            member.inventory.push_back(bullet);
        }

        if(currentCritter == 3)
        {
            Item item = *getGlobalItem("Sniper");
            Item bullet;
            bullet = *getGlobalItem("Bullet: Armor Piercing");
            bullet.amount = 30;

            member.inventory.push_back(item);
            member.inventory.push_back(bullet);
        }
        currentCritter++;
    }

    setupSquadHotKeySelection();
}

void setupTowers()
{


    //Build the Tavern.
    Tower theTavern;
    {
        theTavern.mapID = 23;
        theTavern.bountyPay = 14628;
        theTavern.bountyTarget = "The Keep";
        theTavern.floors = 1;
        theTavern.difficulty = 636;
        theTavern.minioncount = 23;
        theTavern.name = "The Tavern";
        theTavern.tex = &texturemanager.getTexture("FantasyModern.png");
    }
    towers.push_back(theTavern);

    //Build the modern fantasy tower.
    Tower fantasyTower;
    {
        fantasyTower.mapID = 636;
        fantasyTower.bountyPay = 2000;
        fantasyTower.bountyTarget = "Trifecta";
        fantasyTower.floors = 5;
        fantasyTower.difficulty = 5;
        fantasyTower.minioncount = 200;
        fantasyTower.name = "FantasyModern";
        //fantasyTower.tex = &texturemanager.getTexture("FantasyModern.png");
        fantasyTower.tex = &texturemanager.getTexture("Scroll.png");
    }
    towers.push_back(fantasyTower);
}

void positionSquaddies()
{
    for(auto &npc : npclist)
    {
        if(npc.faction == conFact->name)
        {
            npc.ypos = 4040;
            npc.xpos = (96*GRID_SIZE/2) + random(-50,50);
            npc.zpos = 60;
        }
    }
}

void bountyTowerSetup()
{
    assignSlotPos();
    gCtrl.phase = "Lobby";
    gCtrl.phase = "BTMain Menu";
    window.setFramerateLimit(60); // 0 is unlimited
    //UnyTiles.makeTest();
    window.setView(gvars::view1);
    gvars::currentx = 32;
    gvars::currenty = 18;
    gvars::currentz = 1;
    /*
    {
        int Amt = randz(5,30);
        for(int i = 0; i != Amt; i++)
        {
            Tower tower;
            towers.push_back(tower);
        }
    }
    */

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
    factR.faction = "Wild";
    conFact->factRelations.push_back(factR);

    conFact->credits = 750*2;

    addMembers(1,"The Titanium Grip");
    equipStarters();


    FactPtr = addFaction("Towerlings");
    std::cout << FactPtr->name << " added. \n";

    FactPtr->factRelations.push_back(factR); // Still on "Wild" faction.
    factR.faction = "The Titanium Grip";
    FactPtr->factRelations.push_back(factR);

    FactPtr = addFaction("Wild");
    std::cout << FactPtr->name << " added. \n";

    factR.faction = "Towerlings";
    FactPtr->factRelations.push_back(factR); // Still on "Wild" faction.
    factR.faction = "The Titanium Grip";
    FactPtr->factRelations.push_back(factR);

    setupTowers();

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
    shapes.createSquare(menuEdgeL,gvars::topLeft.y,gvars::topRight.x,gvars::bottomLeft.y,sf::Color::Black);
    int mouseX = gvars::mousePos.x/GRID_SIZE;
    int mouseY = gvars::mousePos.y/GRID_SIZE;


    if(debugTileKeeper.useTile.id != -1 && aabb(mouseX,mouseY,0,GRIDS-1,0,GRIDS-1))
    {
        debugTileKeeper.tex = *debugTileKeeper.useTile.img.getTexture();
        sf::Vector2f vPos((gvars::mousePos.x/GRID_SIZE)*GRID_SIZE,(gvars::mousePos.y/GRID_SIZE)*GRID_SIZE);
        shapes.createImageButton(vPos,debugTileKeeper.tex);

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
            shapes.createSquare(x*GRID_SIZE,y*GRID_SIZE,(x+1)*GRID_SIZE,(y+1)*GRID_SIZE,sf::Color(0,255,255,100));
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
            std::string outPut = "(23)Hold for another ten seconds to save the current map layout, This cannot be undone. \n";
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


    /*
    if(inputState.key[Key::G].time == 10)
    {
        tileWipe();
    }
    */

    int wallButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30,menuEdgeU+30),texturemanager.getTexture("FMTwallcheat2.png"));
    if(shapes.shapeClicked(wallButt))
        debugTileKeeper.useTile.BTwall();

    int floorButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+90,menuEdgeU+30),texturemanager.getTexture("FMTtile1.png"));
    if(shapes.shapeClicked(floorButt))
        debugTileKeeper.useTile.BTstone();

    int fakewallButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30+60+60,menuEdgeU+30),texturemanager.getTexture("FMTwallcheat.png"));
    if(shapes.shapeClicked(fakewallButt))
        debugTileKeeper.useTile.BTwallFake();

    //
    int grassButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30,menuEdgeU+30+60),texturemanager.getTexture("BTGrass.png"));
    if(shapes.shapeClicked(grassButt))
        debugTileKeeper.useTile.BTgrass();
    int waterButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30+60,menuEdgeU+30+60),texturemanager.getTexture("BTWater.png"));
    if(shapes.shapeClicked(waterButt))
        debugTileKeeper.useTile.BTwater();
    int skyButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30+60+60,menuEdgeU+30+60),texturemanager.getTexture("BTSky.png"));
    if(shapes.shapeClicked(skyButt))
        debugTileKeeper.useTile.BTsky();

    int switchButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30,menuEdgeU+30+60+60),texturemanager.getTexture("Switchboard.png"));
    if(shapes.shapeClicked(switchButt))
        debugTileKeeper.useTile.BTswitch();

    int stairButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30+60,menuEdgeU+30+60+60),texturemanager.getTexture("Stairs.png"));
    if(shapes.shapeClicked(stairButt))
        debugTileKeeper.useTile.BTstairs();

    int doorButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30+60+60,menuEdgeU+30+60+60),texturemanager.getTexture("BTDoorTile.png"));
    if(shapes.shapeClicked(doorButt))
        debugTileKeeper.useTile.BTdoor();

    int elevatorButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30+60,menuEdgeU+30+60+60+60),texturemanager.getTexture("BTElevator.png"));
    if(shapes.shapeClicked(elevatorButt))
        debugTileKeeper.useTile.BTelevator();

    int elevatordoorButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30+60+60,menuEdgeU+30+60+60+60),texturemanager.getTexture("BTElevatorDoor.png"));
    if(shapes.shapeClicked(elevatordoorButt))
        debugTileKeeper.useTile.BTelevatordoor();

    int bossrugButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30+60+60+60,menuEdgeU+30+60+60+60),texturemanager.getTexture("BTRug.png"));
    if(shapes.shapeClicked(bossrugButt))
        debugTileKeeper.useTile.BTbossrug();




}

void towerTransition()
{
    shapes.shapes.clear();
    worlditems.clear();
    leftBehind.clear();
    clearBullets();
    resetPathingController();


    for(auto &npc : npclist)
    {
        npc.storedPath.clear();
        npc.endPos = Vec3();
        npc.hasPath = false;
        npc.needsPath = false;
        npc.momentum = sf::Vector2f(0,0);
        npc.chaseDefendPos = Vec3(npc.xpos,npc.ypos,npc.zpos);

        if(!npc.isSquaddie)
            npc.toDelete = true;
    }

}

void towerMenu()
{
    //Lock camera in a reasonable position.
    gvars::currentx = 32;
    gvars::currenty = 18;
    //Screen position defaults
    int xPart = screen.x()/5;
    int yPart = screen.y()/3;
    int xMinus = xPart/2;
    int yMinus = yPart/2;

    std::vector<sf::Vector2f> towerPos;
    {// Inserting all the positions
        sf::Vector2f vInsert;
        vInsert = sf::Vector2f((-xMinus)+xPart,(-yMinus)+yPart);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*2,(-yMinus)+yPart);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*3,(-yMinus)+yPart);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*4,(-yMinus)+yPart);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*5,(-yMinus)+yPart);
        towerPos.push_back(vInsert);

        vInsert = sf::Vector2f((-xMinus)+xPart,(-yMinus)+yPart*2);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*2,(-yMinus)+yPart*2);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*3,(-yMinus)+yPart*2);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*4,(-yMinus)+yPart*2);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*5,(-yMinus)+yPart*2);
        towerPos.push_back(vInsert);

        vInsert = sf::Vector2f((-xMinus)+xPart,(-yMinus)+yPart*3);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*2,(-yMinus)+yPart*3);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*3,(-yMinus)+yPart*3);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*4,(-yMinus)+yPart*3);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*5,(-yMinus)+yPart*3);
        towerPos.push_back(vInsert);

    }


    //Place a button for the tower.
    //sf::Vector2f vPos((-xMinus)+xPart,(-yMinus)+yPart);
    int fanTowButt = shapes.createImageButton(towerPos[0],*towers[1].tex);
    if(shapes.shapeClicked(fanTowButt))
    {

        bountytower::pausewaves = true;
        bountytower::towerLoaded = towers[1].name;
        bountytower::currentTower = &towers[1];
        gCtrl.phase = "Lobby";
        //buildTower(towers[1].name);

        loadMap(towers[1].mapID,0,0,50,50);

        int xview = (96*60)/20;
        gvars::currentx = xview/2;
        gvars::currenty = xview/1.4;

        towerTransition();


        positionSquaddies();

    }

    //Draw some info about the thing.
    {
        std::string textOut = " Tower: " + towers[1].name +
        "\n Minions: " + std::to_string(towers[1].minioncount) +
        "\n Difficulty: " + std::to_string(towers[1].difficulty);
        shapes.createText(towerPos[0].x+50,towerPos[0].y-25,15,sf::Color::Red,textOut);
    }

    shapes.createSquare(-60,-60,screen.x()+60,screen.y()+60,sf::Color::Black);

    shapes.createCircle((-xMinus)+xPart,(-yMinus)+yPart,50,gvars::cycleBlue);
    shapes.createCircle((-xMinus)+xPart*2,(-yMinus)+yPart,50,gvars::cycleBlue);
    shapes.createCircle((-xMinus)+xPart*3,(-yMinus)+yPart,50,gvars::cycleBlue);
    shapes.createCircle((-xMinus)+xPart*4,(-yMinus)+yPart,50,gvars::cycleBlue);
    shapes.createCircle((-xMinus)+xPart*5,(-yMinus)+yPart,50,gvars::cycleRed);

    shapes.createCircle((-xMinus)+xPart,(-yMinus)+yPart*2,50,gvars::cycleBlue);
    shapes.createCircle((-xMinus)+xPart*2,(-yMinus)+yPart*2,50,gvars::cycleBlue);
    shapes.createCircle((-xMinus)+xPart*3,(-yMinus)+yPart*2,50,gvars::cycleBlue);
    shapes.createCircle((-xMinus)+xPart*4,(-yMinus)+yPart*2,50,gvars::cycleBlue);
    shapes.createCircle((-xMinus)+xPart*5,(-yMinus)+yPart*2,50,gvars::cycleRed);

    shapes.createCircle((-xMinus)+xPart,(-yMinus)+yPart*3,50,gvars::cycleBlue);
    shapes.createCircle((-xMinus)+xPart*2,(-yMinus)+yPart*3,50,gvars::cycleBlue);
    shapes.createCircle((-xMinus)+xPart*3,(-yMinus)+yPart*3,50,gvars::cycleBlue);
    shapes.createCircle((-xMinus)+xPart*4,(-yMinus)+yPart*3,50,gvars::cycleBlue);
    shapes.createCircle((-xMinus)+xPart*5,(-yMinus)+yPart*3,50,gvars::cycleRed);


    int returnButt = shapes.createImageButton(sf::Vector2f(screen.x()-50,50),texturemanager.getTexture("returnButton.png"),"",0,&gvars::hudView);
    if(shapes.shapeClicked(returnButt))
    {
        gCtrl.phase = "Lobby";
        int xview = (96*60)/20;
        gvars::currentx = xview/2;
        gvars::currenty = xview/1.4;
    }


}




baseMenu::baseMenu()
{
    scrollOne = 0;
    scrollTwo = 0;
    age = 0;
    toDelete = false;
}

std::list<baseMenu> menus;

void skillMenu(Npc &npc)
{
    for(auto menu : menus)
        if(menu.name == "Skill Menu")
            return;
    baseMenu sMenu;
    sMenu.name = "Skill Menu";
    sMenu.Pos = sf::Vector2f(screen.x()/2,screen.y()/2);
    sMenu.npc = &npc;
    menus.push_back(sMenu);
}

void squaddieMenu(Npc &npc)
{
    for(auto menu : menus)
        if(menu.name == "Squaddie Menu")
            return;
    std::cout << "This squaddie has, Health: " << npc.health << ", items: " << npc.inventory.size() << std::endl;
    baseMenu sMenu;
    sMenu.name = "Squaddie Menu";
    sMenu.Pos = sf::Vector2f(screen.x()/2,screen.y()/2);
    sMenu.npc = &npc;
    menus.push_back(sMenu);
}

void merchantMenu(Vec3f creationPos)
{
    for(auto menu : menus)
        if(menu.name == "Merchant Menu")
            return;
    baseMenu sMenu;
    sMenu.name = "Merchant Menu";
    sMenu.Pos = sf::Vector2f(screen.x()/2,screen.y()/2);
    sMenu.makePos = creationPos;
    menus.push_back(sMenu);
}

void recruiterMenu(Vec3f creationPos)
{
    for(auto menu : menus)
        if(menu.name == "Recruitment Menu")
            return;

    if(recruitables.empty())
    {
        RandomWeightList recruitList;
        for(auto &npc : npcmanager.globalCritter)
            if(npc.recruitable)
        {
            recruitList.addEntry(npc.getRace(),npc.rarity);
        }

        for(int i = 0; i != 8; i++)
        {
            Npc recruit = *getGlobalCritter(recruitList.getRandomName());


            recruit.attributes.randomizeAttributes();

            int sum_of_attributes = 0;
            sum_of_attributes += recruit.attributes.strength;
            sum_of_attributes += recruit.attributes.perception;
            sum_of_attributes += recruit.attributes.intelligence;
            sum_of_attributes += recruit.attributes.charisma;
            sum_of_attributes += recruit.attributes.endurance;
            sum_of_attributes += recruit.attributes.dexterity;

            recruit.value += sum_of_attributes*3;

            recruit.name = generateName();

            recruitables.push_back(recruit);
        }
    }

    baseMenu sMenu;
    sMenu.name = "Recruitment Menu";
    sMenu.Pos = sf::Vector2f(screen.x()/2,screen.y()/2);
    sMenu.makePos = creationPos;
    menus.push_back(sMenu);
}

void towerMenu(Vec3f creationPos)
{
    for(auto menu : menus)
        if(menu.name == "Tower Menu")
            return;
    baseMenu sMenu;
    sMenu.name = "Tower Menu";
    sMenu.Pos = sf::Vector2f(screen.x()/2,screen.y()/2);
    sMenu.makePos = creationPos;
    menus.push_back(sMenu);
}

void escapeMenu(Vec3f creationPos)
{
    for(auto menu : menus)
        if(menu.name == "Escape Menu")
            return;
    baseMenu sMenu;
    sMenu.name = "Escape Menu";
    sMenu.Pos = sf::Vector2f(screen.x()/2,screen.y()/2);
    sMenu.makePos = creationPos;
    menus.push_back(sMenu);
}

int nextLevelXpRequired(int level)
{
    int baseXpPerLevel = 100;
    float xpPerLevelMultiplier = 0.4;
    int returns = 100;
    for(int i = 0; i != level; i++)
    {
        //returns += baseXpPerLevel*(i*xpPerLevelMultiplier);
        returns += returns*(xpPerLevelMultiplier);

    }

    return returns;
}

int totalLevelXp(int level)
{
    int returns = 0;
    for(int i = 0; i != level; i++)
    {
        returns += nextLevelXpRequired(i);
    }
    return returns;
}

int getFloorDifficulty(float currentFloor, float towerFloors, float towerDifficulty)
{
    return (towerDifficulty/2)+((currentFloor/towerFloors*towerDifficulty)/2);
}

bool baseMenu::hasVar(std::string varName)
{
    for(auto &var : vars)
    {
        if(var.name == varName)
            return true;
    }

    return false;
}

dynamicVariable * baseMenu::getVar(std::string varName)
{
    for(auto &var : vars)
    {
        if(var.name == varName)
            return &var;
    }

    return nullptr;
}

void renderSkillMenu(baseMenu &menu)
{ // Layer 17000
    selectedNPCs.clear();
    int layer = 17000;

    shapes.createSquare(100,100,screen.x()-100,screen.y()-100,sf::Color(sf::Color(150,150,0)),5,sf::Color::White,&gvars::hudView);
    shapes.shapes.back().layer = layer+0;
    //shapes.shapes.back().layer = 10;
    //Close Button
    int exitButt = shapes.createImageButton(sf::Vector2f(screen.x()-100,100),texturemanager.getTexture("ExitButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+50;

    if(shapes.shapeClicked(exitButt))
        menu.toDelete = true;

    if(menu.age > 30 && inputState.key[Key::U])
        menu.toDelete = true;

    Npc *npc = menu.npc;


    shapes.createText(sf::Vector2f(105,100),20,sf::Color(100,100,100),"Name: " + npc->name,&gvars::hudView);
    shapes.shapes.back().layer = layer+75;



    sf::Vector2f invPos(screen.x()/3,130);

    if(npc->skillpoints == 0)
        shapes.createText(sf::Vector2f(636,102),15,sf::Color::White,"Skills",&gvars::hudView);
    else
        shapes.createText(sf::Vector2f(636,102),15,sf::Color::White,"Skills   SP Remaining: " + str(npc->skillpoints),&gvars::hudView);

    shapes.shapes.back().layer = layer+75;



    shapes.createSquare(300,invPos.y-10,1150,invPos.y+(screen.y()/2),sf::Color::Transparent,2,sf::Color::Black,&gvars::hudView);
    shapes.shapes.back().layer = layer+25;
    shapes.createSquare(300,invPos.y+(screen.y()/2),1150,invPos.y+(screen.y()/2)+110,sf::Color::Black,0,sf::Color::Black,&gvars::hudView);
    shapes.shapes.back().layer = layer+25;

    dynamicVariable * currentSkill = menu.getVar("currentSkill");

    { // Current Skill Tree Selection
        if(!menu.hasVar("currentSkill"))
        {
            dynamicVariable var;
            var.name = "currentSkill";
            var.varString = "Melee";

            menu.vars.push_back(var);
        }
        std::cout << "Menu: " << menu.name << " has " << menu.vars.size() << " dynamic vars. \n";
        int yOffset = 0;



        std::string lastSkillTree;
        for(auto &skill : npc->skills.list)
        {
            if(skill.tree == lastSkillTree)
                continue;
            lastSkillTree = skill.tree;

            sf::Vector2f drawPos(120,150+(yOffset*60));
            int skillTreeButt = shapes.createImageButton(drawPos,texturemanager.getTexture("Skills"+skill.tree+".png"),"",0,&gvars::hudView);
            shapes.shapes.back().layer = layer+75;
            sf::Vector2f textPos(drawPos.x+40, drawPos.y);
            shapes.createText(textPos,10,sf::Color::White,skill.tree,&gvars::hudView);
            shapes.shapes.back().layer = layer+75;

            bool isParagon = (skill.tree == "Paragon");


            if(isParagon && !bountytower::gameBeaten && shapes.shapeHovered(skillTreeButt))
            {
                shapes.createText(gvars::mousePos,10,sf::Color::White,"   You must beat the game to unlock the Paragon tree! (Demo: Beat the tower)");
                shapes.shapes.back().layer = layer+75;
            }
            if(shapes.shapeClicked(skillTreeButt))
            {
                if((bountytower::gameBeaten || !isParagon) || inputState.key[Key::LShift] && inputState.key[Key::LControl])
                    if(currentSkill != nullptr)
                        currentSkill->varString = skill.tree;
            }

            yOffset++;
        }


    }


    //Pooling up all skill ranks of each level from current tree, to use with skill level locks.
    int levels[20];
    int totalRanksPerLevel[20];
    {
       for(int i = 0; i != 20; i++)
        {
            levels[i] = 0;
            totalRanksPerLevel[i] = 0;
        }

        for(auto &skill : npc->skills.list)
        {
            if(currentSkill == nullptr)
                return;
            if(currentSkill->varString != skill.tree)
                continue;

            levels[skill.level]++;
            totalRanksPerLevel[skill.level] += skill.ranks;
        }
    }




    float x = 0, y = 0;
    bool offSet = false;
    for(auto &skill : npc->skills.list)
    {
        if(currentSkill == nullptr)
            return;
        if(currentSkill->varString != skill.tree)
            continue;

        if(y != skill.level)
        {
            y++;
            x = 0;
        }

        sf::Vector2f drawPos(300+30+5+(x*(120)),invPos.y + (80*y)+30);
        x++;

        bool skillLocked = (skill.level != 0 && totalRanksPerLevel[skill.level-1] < 5);

        if(skillLocked)
            shapes.createSquare(drawPos.x-30,drawPos.y-30,drawPos.x+30,drawPos.y+30,sf::Color::Red,0,sf::Color::White,&gvars::hudView);
        else
            shapes.createSquare(drawPos.x-30,drawPos.y-30,drawPos.x+30,drawPos.y+30,sf::Color::Black,0,sf::Color::White,&gvars::hudView);

        shapes.shapes.back().layer = layer+25;

        //shapes.createSquare(drawPos.x-30,drawPos.y-30,drawPos.x+30,drawPos.y+30,sf::Color::Black,0,sf::Color::White,&&gvars::hudView);
        int skillButt = shapes.createImageButton(drawPos,texturemanager.getTexture("Skills"+skill.tree+".png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+50;

        sf::Vector2f skillNamePos(drawPos.x-30,drawPos.y-40);
        shapes.createText(skillNamePos,8,sf::Color::White,skill.name,&gvars::hudView);
        shapes.shapes.back().layer = layer+75;

        sf::Vector2f skillRankPos(drawPos.x-30,drawPos.y-30);
        shapes.createText(skillRankPos,8,sf::Color::White,str(skill.ranks),&gvars::hudView);
        shapes.shapes.back().layer = layer+75;

        if(shapes.shapeHovered(skillButt))
        {
            sf::Vector2f textPos(305,invPos.y+(screen.y()/2));
            sf::Vector2f mouseConvPos(gvars::mousePos.x+10,gvars::mousePos.y);
            shapes.createText(textPos,15,sf::Color::White,skill.name,&gvars::hudView);
            shapes.shapes.back().layer = layer+75;

            textPos.y += 15;
            std::string outPut = "Ranks: (" + str(skill.ranks) + "/" + str(skill.ranksmax) + ")";
            shapes.createText(textPos,15,sf::Color::White,outPut,&gvars::hudView);
            shapes.shapes.back().layer = layer+75;
            textPos.y += 15;
            if(skill.active)
            { // Cooldown
                outPut = "Cooldown: " + str(skill.cooldownint/60) + " sec."; // Dividing by 60 to give seconds.
                shapes.createText(textPos,15,sf::Color::White,outPut,&gvars::hudView);
                shapes.shapes.back().layer = layer+75;
            }

            textPos.y += 15;
            if(skill.durationint > 0)
            { // Duration
                outPut = "Duration: " + str(skill.durationint);
                shapes.createText(textPos,15,sf::Color::White,outPut,&gvars::hudView);
                shapes.shapes.back().layer = layer+75;
            }



            textPos.y += 15;
            outPut = skill.desc;
            shapes.createText(textPos,15,sf::Color::White,outPut,&gvars::hudView);
            shapes.shapes.back().layer = layer+75;


            if(inputState.rmbTime == 1 && skill.ranks > 0 && menu.age > 30)
            {
                skill.ranks--;
                npc->skillpoints++;
            }



            if(!skillLocked && inputState.lmbTime == 1 && skill.ranks < skill.ranksmax && npc->skillpoints > 0 && menu.age > 30)
            {
                skill.ranks++;
                npc->skillpoints--;
            }
            else if(inputState.key[Key::LShift] && inputState.key[Key::LControl] && inputState.lmbTime == 1 && skill.ranks < skill.ranksmax)
            {
                skill.ranks++;
                npc->skillpoints--;
            }
        }

    }

    int squaddieNSkillButt = shapes.createImageButton(sf::Vector2f(110+30,490+30),texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+50;
    textList.createText(sf::Vector2f(110+5,490+25),9,sf::Color::White,"Overview",gvars::hudView);
    shapes.shapes.back().layer = layer+75;
    if(menu.age > 15 && shapes.shapeClicked(squaddieNSkillButt))
    {
        menu.toDelete = true;
        squaddieMenu(*npc);
    }




}

void renderSquaddieMenu(baseMenu &menu)
{ // Layer 18000
    int layer = 18000;
    enum
    {
        BackPanel,
        Shapes,
        Button,
        Text
    };
    if(inputState.key[Key::Comma].time == 1)
    {
        for(int i = 0; i != 101; i++)
        {
            std::cout << "New Level " << i << ": " << nextLevelXpRequired(i) << "xp \n";
        }
        std::cout << "Level 15: " << nextLevelXpRequired(15);
    }



    shapes.createSquare(100,100,screen.x()-100,screen.y()-100,sf::Color(sf::Color(150,150,0)),5,sf::Color::White,&gvars::hudView);
    shapes.shapes.back().layer = layer+BackPanel;
    //Close Button
    int exitButt = shapes.createImageButton(sf::Vector2f(screen.x()-100,100),texturemanager.getTexture("ExitButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+Button;
    if(shapes.shapeClicked(exitButt))
        menu.toDelete = true;

    if(menu.age > 30 && inputState.key[Key::I])
        menu.toDelete = true;

    Npc *npc = menu.npc;
    shapes.createText(sf::Vector2f(105,100),20,sf::Color(100,100,100),"Name: " + npc->name,&gvars::hudView);
    shapes.shapes.back().layer = layer+Text;

    //XP and Level!
    std::string LevelXpLine = "Level: " + std::to_string(npc->level) + "\n";
    LevelXpLine.append("XP: " + std::to_string(npc->xp));
    LevelXpLine.append(", (Next Level: " + std::to_string(nextLevelXpRequired(npc->level)) + ")");
    shapes.createText(sf::Vector2f(105,120),20,sf::Color::White,LevelXpLine,&gvars::hudView);
    shapes.shapes.back().layer = layer+Text;

    //Attributes! SPICED
    std::string AttributeLine;
    AttributeLine.append("S.P.I.C.E.D. \n");
    AttributeLine.append("Strength: " + str(npc->attributes.strength)+"("+str(npc->getStr())+")\n");
    AttributeLine.append("Perception: " + str(npc->attributes.perception)+"("+str(npc->getPer())+")\n");
    AttributeLine.append("Intelligence: " + str(npc->attributes.intelligence)+"("+str(npc->getInt())+")\n");
    AttributeLine.append("Charisma: " + str(npc->attributes.charisma)+"("+str(npc->getCha())+")\n");
    AttributeLine.append("Endurance: " + str(npc->attributes.endurance)+"("+str(npc->getEnd())+")\n");
    AttributeLine.append("Dexterity: " + str(npc->attributes.dexterity)+"("+str(npc->getDex())+")\n");
    shapes.createText(sf::Vector2f(105,170),20,sf::Color::White,AttributeLine,&gvars::hudView);
    shapes.shapes.back().layer = layer+Text;




    //Inventory!
    sf::Vector2f invPos(screen.x()/2,130);
    shapes.createText(sf::Vector2f(636,102),15,sf::Color::White,"Inventory",&gvars::hudView);
    shapes.shapes.back().layer = layer+Text;
    shapes.createSquare(invPos.x-40,invPos.y-10,invPos.x+100,invPos.y+(screen.y()/2)+120,sf::Color::Transparent,2,sf::Color::Black,&gvars::hudView);
    shapes.shapes.back().layer = layer+Shapes;

    float x = 0, y = 0;
    bool offSet = false;
    for(auto &item : npc->inventory)
    {
        sf::Vector2f drawPos(invPos.x,invPos.y+(60*y)+30);
        if(offSet)
            drawPos.x += 62;
        shapes.createSquare(drawPos.x-30,drawPos.y-30,drawPos.x+30,drawPos.y+30,sf::Color::Black,2,sf::Color::White,&gvars::hudView);
        shapes.shapes.back().layer = layer+Shapes;
        shapes.createImageButton(drawPos,*item.img.getTexture(),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+Button;
        if(offSet)
        {
            drawPos.x -= 62;
            y++;
        }

        toggle(offSet);
    }

    /*

    //Skills!
    sf::Vector2f skillPos(800,105);

    if(npc->skillpoints > 0)
        shapes.createText(skillPos,15,sf::Color::White,"Skills   SP Remaining: " + std::to_string(npc->skillpoints),&gvars::hudView);
    else
        shapes.createText(skillPos,15,sf::Color::White,"Skills",&gvars::hudView);

    y = 0;
    std::string lastTree = "";
    for(auto &skill : npc->skills.list)
    {
        std::string outPut = "(" + std::to_string(skill.ranks) + "/" + std::to_string(skill.ranksmax) + ")" + skill.name;
        sf::Vector2f drawPos(skillPos.x,skillPos.y+(y*15)+15);
        if(skill.tree != lastTree)
        {
            lastTree = skill.tree;
            shapes.createText(drawPos,20,sf::Color::Red,skill.tree,&gvars::hudView);
            y++;
        }
        drawPos = sf::Vector2f(skillPos.x,skillPos.y+(y*15)+15);
        //else
        {
            sf::Vector2f buttonPos(drawPos.x-40,drawPos.y+10);


            int decreaseSkillButton = shapes.createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",-90,&gvars::hudView);
            buttonPos.x += 20;
            int increaseSkillButton = shapes.createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",90,&gvars::hudView);

            if(shapes.shapeHovered(increaseSkillButton) || shapes.shapeHovered(decreaseSkillButton))
            {
                shapes.createText(drawPos,20,sf::Color::Cyan,outPut,&gvars::hudView);
                sf::Vector2f textPos(screen.x()/8,70);
                shapes.createSquare(0,textPos.y,screen.x(),textPos.y+13,sf::Color::Black,1,sf::Color::White,&&gvars::hudView);
                shapes.createText(textPos,10,sf::Color::Cyan,skill.desc,&gvars::hudView);
            }
            else
                shapes.createText(drawPos,20,sf::Color::White,outPut,&gvars::hudView);

            if(shapes.shapeClicked(decreaseSkillButton) && skill.ranks > 0 && menu.age > 30)
            {
                skill.ranks--;
                npc->skillpoints++;
            }

            if(shapes.shapeClicked(increaseSkillButton) && skill.ranks < skill.ranksmax && npc->skillpoints > 0 && menu.age > 30)
            {
                skill.ranks++;
                npc->skillpoints--;
            }

        }



        y += 1.3;

    }


    */
    int squaddieNSkillButt = shapes.createImageButton(sf::Vector2f(110+30,490+30),texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+Button;
    textList.createText(sf::Vector2f(110+5,490+25),10,sf::Color::White,"Skills",gvars::hudView);
    shapes.shapes.back().layer = layer+Text;

    if(menu.age > 15 && shapes.shapeClicked(squaddieNSkillButt))
    {
        menu.toDelete = true;
        skillMenu(*npc);
    }

}

int getSquadDiscount(int originalValue)
{
    float totalCharisma = 0;
    for(auto &squaddie : Squaddies)
    {
        totalCharisma += squaddie->getCha();
    }

    int newValue = originalValue - (originalValue * (totalCharisma*0.01));

    return newValue;
}

void renderMerchantMenu(baseMenu &menu)
{ // Layer 19000
    int layer = 19000;
    enum
    {
        BackPanel,
        Shapes,
        Button,
        Text,
        FrontPanel
    };

    sf::Vector2f menuStartPos(100,100);
    sf::Vector2f menuEndPos(screen.x()-100,screen.y()-100);
    sf::Color menuColor(150,150,0);

    shapes.createSquare(menuStartPos.x,menuStartPos.y,menuEndPos.x,menuEndPos.y,menuColor,5,sf::Color::White,&gvars::hudView);
    shapes.shapes.back().layer = layer+BackPanel;

    shapes.createSquare(menuStartPos.x,menuStartPos.y,menuEndPos.x,menuStartPos.y+60,menuColor,0,sf::Color::White,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel;

    shapes.createSquare(menuStartPos.x,menuEndPos.y-60,menuEndPos.x,menuEndPos.y,menuColor,0,sf::Color::White,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel;
    //Close Button
    int exitButt = shapes.createImageButton(sf::Vector2f(screen.x()-100,100),texturemanager.getTexture("ExitButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel+1;
    if(shapes.shapeClicked(exitButt))
        menu.toDelete = true;


    sf::Event event;
    while (window.pollEvent(event))
        if (event.type == sf::Event::MouseWheelMoved)
    {
        if (event.mouseWheel.delta > 0)
        { // down
            menu.scrollOne += 1;
        }
        if (event.mouseWheel.delta < 0)
        { // up

            menu.scrollOne -= 1;
            if(menu.scrollOne < 0)
                menu.scrollOne = 0;
        }
    }

    sf::Vector2f upScrollButtPos(percentPos(1,menuStartPos.x,menuEndPos.x),percentPos(15,menuStartPos.y,menuEndPos.y));
    sf::Vector2f downScrollButtPos(percentPos(1,menuStartPos.x,menuEndPos.x),percentPos(25,menuStartPos.y,menuEndPos.y));
    int upScrollButt = shapes.createImageButton(upScrollButtPos,texturemanager.getTexture("ArrowButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+Button;
    int downScrollButt = shapes.createImageButton(downScrollButtPos,texturemanager.getTexture("ArrowButton.png"),"",180,&gvars::hudView);
    shapes.shapes.back().layer = layer+Button;

    if(shapes.shapeClicked(upScrollButt))
    {
        menu.scrollOne++;
        if(menu.scrollOne > 0)
            menu.scrollOne = 0;
    }

    if(shapes.shapeClicked(downScrollButt))
        menu.scrollOne--;

    int xOffset = 0;
    int yOffset = 0;
    for(auto item : itemmanager.globalItems)
    {
        if(item.value == -1)
            continue;
        int posX = 150+(xOffset*300);
        int posY = 150+(yOffset*60);

        posY += (menu.scrollOne+3)*15;

        if(posY < menuStartPos.y+30 || posY > menuEndPos.y-30 )
        {
            xOffset++;
            if(xOffset > 1)
            {
                xOffset = 0;
                yOffset++;
            }
            continue;
        }




        shapes.createSquare(posX-30,posY-30,posX+30,posY+30,sf::Color::Black,0,sf::Color::Cyan, &gvars::hudView);
        shapes.shapes.back().layer = layer+Shapes;

        sf::Vector2f vPos(posX,posY);
        int itemButt = shapes.createImageButton(vPos,*item.img.getTexture(),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+Button;

        sf::Color highlightColor = sf::Color::White;
        if(shapes.shapeHovered(itemButt))
            highlightColor = sf::Color::Cyan;

        vPos.y -= 30;
        vPos.x += 30;
        std::string outPut = item.name;


        shapes.createText(vPos,15,highlightColor,outPut,&gvars::hudView);
        shapes.shapes.back().layer = layer+Text;
        vPos.y += 15;
        shapes.createText(vPos,10,highlightColor,"$" + str(getSquadDiscount(item.value)),&gvars::hudView);
        shapes.shapes.back().layer = layer+Text;
        vPos.y += 15;
        std::string stats = "Dam: " + str(item.mindam) + "/" + str(item.maxdam);
        if(item.type == 1 || item.type == 2 || item.type == 23)
            stats.append(", AtkDelay: " + str(static_cast<int>(item.activateratemax / item.activaterategrowth)));
        if(item.type == 3 || item.type == 4 || item.type == 5)
            stats.append(", Knockback: " + str(static_cast<int>(item.getKnockback())));

        stats.append("\nRange:" + str(item.range));


        if(item.type == 2)
        {
            stats.append(", Mag: " + str(item.maxclip));
            if(item.ammotype == 3)
                stats.append(", Bullet");
            if(item.ammotype == 4)
                stats.append(", Shell");
            if(item.ammotype == 5)
                stats.append(", Missile");
        }
        else if(item.type == 3)
        {
            stats.append(", Pierce:" + str(item.penetration));
            stats.append(", Richochets:" + str(item.richochets));
        }
        else if(item.type == 42)
        {
            stats.append(", Heal Amount: " + str(item.healAmount));
        }

        if(item.stackSize > 1)
            stats.append("\nStack: " + str(item.stackSize));
        shapes.createText(vPos,10,highlightColor,stats,&gvars::hudView);
        shapes.shapes.back().layer = layer+Text;



        if(shapes.shapeClicked(itemButt) && menu.age > 30)
        {
            if(conFact->credits < getSquadDiscount(item.value))
                chatBox.addChat("You do not have enough cash for "+item.name+"!", sf::Color::White);
            else
            {
                conFact->credits -= getSquadDiscount(item.value);
                Item soldItem = item;
                soldItem.id = gvars::globalid++;
                soldItem.xpos = menu.makePos.x+(randz(-90,90));
                soldItem.ypos = menu.makePos.y+90;
                soldItem.zpos = menu.makePos.z;
                soldItem.amount = soldItem.stackSize;

                worlditems.push_back(soldItem);

                int soundRan = random(1,3);
                soundmanager.playSound("CashPickup"+str(soundRan)+".ogg");

                chatBox.addChat("You purchased a "+item.name+" for "+str(getSquadDiscount(item.value))+"!", sf::Color::White);
            }
        }



        xOffset++;
        if(xOffset > 1)
        {
            xOffset = 0;
            yOffset++;
        }
    }
}

void renderRecruiterMenu(baseMenu &menu)
{// Layer 20000
    int layer = 20000;
    enum
    {
        BackPanel,
        Shapes,
        Button,
        Text
    };
    shapes.createSquare(100,100,screen.x()-100,screen.y()-100,sf::Color(sf::Color(150,150,0)),5,sf::Color::White,&gvars::hudView);
    shapes.shapes.back().layer = layer+BackPanel;
    //Close Button
    int exitButt = shapes.createImageButton(sf::Vector2f(screen.x()-100,100),texturemanager.getTexture("ExitButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+Button;
    if(shapes.shapeClicked(exitButt))
        menu.toDelete = true;

    int xOffset = 0;
    int yOffset = 0;

    std::string SPICED = "SPICED = Strength Perception Intelligence Charisma Endurance Dexterity";
    shapes.createText(sf::Vector2f(105,100),10,sf::Color::White,SPICED,&gvars::hudView);
    shapes.shapes.back().layer = layer+Text;

    //for(auto npc : npcmanager.globalCritter)
    for(auto &npc : recruitables)
    {
        if(!npc.recruitable)
            continue;
        int critterCost = 100;

        if(npc.getRace() == "BTRockkid")
            critterCost = 1000;

        critterCost = npc.value;

        int posX = 150+(xOffset*300);
        int posY = 150+(yOffset*60);;
        shapes.createSquare(posX-30,posY-30,posX+30,posY+30,sf::Color::Black,0,sf::Color::Cyan, &gvars::hudView);
        shapes.shapes.back().layer = layer+Shapes;



        sf::Vector2f vPos(posX,posY);

        int npcButt = shapes.createImageButton(vPos,texturemanager.getTexture("SpriteSheet"+npc.getRace()+"Frame.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+Button;
        // hehe... npc butt.
        sf::Color highlightColor = sf::Color::White;
        if(shapes.shapeHovered(npcButt))
            highlightColor = sf::Color::Cyan;

        vPos.y -= 30;
        vPos.x += 30;
        shapes.createText(vPos,15,highlightColor,npc.getRace()+": "+npc.name,&gvars::hudView);
        shapes.shapes.back().layer = layer+Text;
        vPos.y += 15;
        shapes.createText(vPos,10,highlightColor,"$" + str(getSquadDiscount(critterCost) ),&gvars::hudView);
        shapes.shapes.back().layer = layer+Text;
        vPos.y += 10;
        std::string outPut = "Speed: " + str(static_cast<int>(npc.moverate) );
        outPut.append("\nS.P.I.C.E.D. : ");
        outPut.append(str(npc.attributes.strength) + ",");
        outPut.append(str(npc.attributes.perception) + ",");
        outPut.append(str(npc.attributes.intelligence) + ",");
        outPut.append(str(npc.attributes.charisma) + ",");
        outPut.append(str(npc.attributes.endurance) + ",");
        outPut.append(str(npc.attributes.dexterity));

        if(npc.racialAbility != "")
        {
            outPut.append("\nRacial: " + npc.racialAbility);
            if(npc.racialAbility == "Ideal Command")
                outPut.append(", +10% EXP for the entire squad.");
            if(npc.racialAbility == "Rock Steady")
                outPut.append(", Immune to Knockback Damage, Additional 20% Damage Reduction.");
            if(npc.racialAbility == "Pocket Picker")
                outPut.append(", 5% Chance to gain some small items when using Melee.");
            if(npc.racialAbility == "Telekinetic Field")
                outPut.append(", Constantly pushes back nearby enemies and slows enemy bullets.");
        }

        shapes.createText(vPos,10,highlightColor,outPut,&gvars::hudView);
        shapes.shapes.back().layer = layer+Text;





        if(shapes.shapeClicked(npcButt) && menu.age > 30)
        {
            if(Squaddies.size() >= 4)
                chatBox.addChat("You already have a full squad!", sf::Color::White);
            else if(conFact->credits < getSquadDiscount(critterCost))
                chatBox.addChat("You do not have enough cash for "+npc.name+"!", sf::Color::White);
            else
            {
                conFact->credits -= getSquadDiscount(critterCost);
                Npc soldNpc = npc;
                soldNpc.id = gvars::globalid++;
                soldNpc.xpos = menu.makePos.x+(randz(-30,30));
                soldNpc.ypos = menu.makePos.y+30;
                soldNpc.zpos = menu.makePos.z;

                soldNpc.health = soldNpc.getMaxHealth();

                soldNpc.factionPtr = conFact;
                soldNpc.faction = conFact->name;
                //soldNpc.name = generateName();
                soldNpc.tags.append("[WearsHat:1]");
                soldNpc.isSquaddie = true;
                soldNpc.chasePriority = "Hold Ground";

                npclist.push_back(soldNpc);
                npc.toDelete = true;

                for(auto &dudes : recruitables)
                {
                    if(dudes.toDelete)
                        std::cout << dudes.name << " should be deleted! \n";
                }

                int soundRan = random(1,3);
                soundmanager.playSound("CashPickup"+str(soundRan)+".ogg");


                chatBox.addChat("You purchased a "+npc.name+" for "+str(getSquadDiscount(critterCost))+"!", sf::Color::White);
                setupSquadHotKeySelection();
            }
        }



        yOffset++;
        if(yOffset > 7)
        {
            yOffset = 0;
            xOffset++;
        }
    }


    AnyDeletes(recruitables);

    for(auto &dudes : recruitables)
    {
        if(dudes.toDelete)
            std::cout << dudes.name << " should be deleted! \n";
    }
}

void renderTowerMenu(baseMenu &menu)
{// Layer 21000
    int layer = 21000;
    enum
    {
        BackPanel,
        Shapes,
        Button,
        Text
    };
    shapes.createSquare(100,100,screen.x()-100,screen.y()-100,sf::Color(sf::Color(150,150,0)),5,sf::Color::White,&gvars::hudView);
    shapes.shapes.back().layer = layer+BackPanel;
    //shapes.shapes.back().layer = 10000000;
    //Close Button
    int exitButt = shapes.createImageButton(sf::Vector2f(screen.x()-100,100),texturemanager.getTexture("ExitButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+Button;
    if(shapes.shapeClicked(exitButt))
        menu.toDelete = true;

        //Screen position defaults
    int xPart = screen.x()/5;
    int yPart = screen.y()/3;
    int xMinus = xPart/2;
    int yMinus = yPart/2;

    std::vector<sf::Vector2f> towerPos;
    {// Inserting all the positions
        sf::Vector2f vInsert;
        vInsert = sf::Vector2f(150,150);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*2,(-yMinus)+yPart);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*3,(-yMinus)+yPart);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*4,(-yMinus)+yPart);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*5,(-yMinus)+yPart);
        towerPos.push_back(vInsert);

        vInsert = sf::Vector2f((-xMinus)+xPart,(-yMinus)+yPart*2);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*2,(-yMinus)+yPart*2);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*3,(-yMinus)+yPart*2);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*4,(-yMinus)+yPart*2);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*5,(-yMinus)+yPart*2);
        towerPos.push_back(vInsert);

        vInsert = sf::Vector2f((-xMinus)+xPart,(-yMinus)+yPart*3);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*2,(-yMinus)+yPart*3);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*3,(-yMinus)+yPart*3);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*4,(-yMinus)+yPart*3);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f((-xMinus)+xPart*5,(-yMinus)+yPart*3);
        towerPos.push_back(vInsert);

    }

    int fanTowButt = shapes.createImageButton(towerPos[0],*towers[1].tex,"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+Button;
    if(shapes.shapeClicked(fanTowButt))
    {
        menu.toDelete = true;
        bountytower::pausewaves = true;
        bountytower::towerLoaded = towers[1].name;
        bountytower::currentTower = &towers[1];
        gCtrl.phase = "Lobby";
        //buildTower(towers[1].name);

        loadMap(towers[1].mapID,0,0,50,50);

        int xview = (96*60)/20;
        gvars::currentx = xview/2;
        gvars::currenty = xview/1.4;

        towerTransition();


        positionSquaddies();

        playMusic("KerriOverblowEcho.ogg");

    }

    //Draw some info about the thing.
    {
        std::string textOut = " Tower: " + towers[1].name +
        //"\n Minions: " + std::to_string(towers[1].minioncount) +
        "\n Target: " + "Rockkid, The Hardened Criminal" +
        "\n Bounty: $" + str(towers[1].bountyPay) +
        "\n Difficulty: " + std::to_string(towers[1].difficulty);
        sf::Vector2f textPos(towerPos[0].x+50,towerPos[0].y-25);
        shapes.createText(textPos,15,sf::Color::White,textOut,&gvars::hudView);
        shapes.shapes.back().layer = layer+Text;
    }



}

void renderEscapeMenu(baseMenu &menu)
{ // Layer 15000
    // Backpanel 15000, Buttons 15050, shapes 15025, text 15075;
    shapes.createSquare(100,100,screen.x()-100,screen.y()-100,sf::Color(sf::Color(150,150,0)),5,sf::Color::White,&gvars::hudView);
    shapes.shapes.back().layer = 15000;
    //Close Button
    int exitButt = shapes.createImageButton(sf::Vector2f(screen.x()-100,100),texturemanager.getTexture("ExitButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = 15050;
    if(shapes.shapeClicked(exitButt))
        menu.toDelete = true;

    sf::Vector2f textPos( (screen.x()/2) - 50,110);
    sf::Vector2f buttonPos(textPos);
    shapes.createText(textPos,10,sf::Color::White,"Sound: " + str(gvars::soundVolume),&gvars::hudView);
    shapes.shapes.back().layer = 15075;
    buttonPos.x += 90;
    buttonPos.y += 15;
    int decreaseSoundButt = shapes.createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",-90,&gvars::hudView);
    shapes.shapes.back().layer = 15050;
    buttonPos.x += 30;
    int increaseSoundButt = shapes.createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",90,&gvars::hudView);
    shapes.shapes.back().layer = 15050;



    textPos.y += screen.y()/8;
    buttonPos = textPos;
    shapes.createText(textPos,10,sf::Color::White,"Music: " + str(gvars::musicVolume),&gvars::hudView);
    shapes.shapes.back().layer = 15075;
    buttonPos.x += 90;
    buttonPos.y += 15;
    int decreaseMusicButt = shapes.createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",-90,&gvars::hudView);
    shapes.shapes.back().layer = 15050;
    buttonPos.x += 30;
    int increaseMusicButt = shapes.createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",90,&gvars::hudView);
    shapes.shapes.back().layer = 15050;

    textPos.y += screen.y()/8;
    buttonPos = textPos;
    shapes.createText(textPos,10,sf::Color::White,"Resolution: "
                        + std::to_string(resolution.resolutions[resolution.currentRes].width)
                        +"/"+std::to_string(resolution.resolutions[resolution.currentRes].height)
                        +": "+std::to_string(resolution.resolutions[resolution.currentRes].bitsPerPixel)
                        +"\n \n \nFullscreen: " + str(resolution.fullscreen)
                        ,&gvars::hudView);
    shapes.shapes.back().layer = 15075;

    buttonPos.x += 90;
    buttonPos.y += 20;
    int decreaseResolution = shapes.createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",-90,&gvars::hudView);
    shapes.shapes.back().layer = 15050;
    buttonPos.x += 30;
    int increaseResolution = shapes.createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",90,&gvars::hudView);
    shapes.shapes.back().layer = 15050;
    buttonPos.x += 30;
    int applyResolution = shapes.createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",180,&gvars::hudView);
    shapes.shapes.back().layer = 15050;

    buttonPos.y += 20;
    int toggleFullscreen = shapes.createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",180,&gvars::hudView);
    shapes.shapes.back().layer = 15050;


    sf::Vector2f exitPos((screen.x()/2), screen.y()-150);
    int exitGameButt = shapes.createImageButton(exitPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = 15050;
    exitPos.x -= 31;
    textList.createText(exitPos,9,sf::Color::White,"Exit Game",gvars::hudView);
    shapes.shapes.back().layer = 15075;

    exitPos.x += 31;
    exitPos.y -= 80;

    int saveGameButt = shapes.createImageButton(exitPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = 15050;
    exitPos.x -= 31;
    textList.createText(exitPos,9,sf::Color::White,"Save Game",gvars::hudView);
    shapes.shapes.back().layer = 15075;



    //Warning, the increase and decrease resolution buttons are backwards, since the videomode put the highest res in first, and the smallest last.
    if(menu.age > 30 && shapes.shapeHovered(increaseResolution) && (inputState.lmbTime == 1 || inputState.lmbTime > 20))
        if(resolution.currentRes > 0)
            resolution.currentRes--;
    if(menu.age > 30 && shapes.shapeHovered(decreaseResolution) && (inputState.lmbTime == 1 || inputState.lmbTime > 20))
        if(resolution.currentRes < resolution.resolutions.size()-1)
            resolution.currentRes++;

    if(menu.age > 30 && shapes.shapeHovered(toggleFullscreen))
    {
        shapes.createText(gvars::mousePos,9,sf::Color::White,"Toggle fullscreen");
        shapes.shapes.back().layer = 15075;
        if((inputState.lmbTime == 1))
            toggle(resolution.fullscreen);
    }


    if(menu.age > 30 && shapes.shapeHovered(applyResolution))
    {
        shapes.createText(gvars::mousePos,9,sf::Color::White,"Apply the Resolution!");
        shapes.shapes.back().layer = 15075;

        if(inputState.lmbTime == 1)
        {
            if(resolution.fullscreen)
                window.create(resolution.resolutions[resolution.currentRes], randomWindowName(),sf::Style::Fullscreen);
            else
                window.create(resolution.resolutions[resolution.currentRes], randomWindowName());
            //,sf::Style::Fullscreen
            window.setFramerateLimit(60);
        }

    }

    if(menu.age > 30 && shapes.shapeHovered(decreaseSoundButt) && (inputState.lmbTime == 1 || inputState.lmbTime > 20))
        gvars::soundVolume = math::clamp(gvars::soundVolume-1,0,100);

    if(menu.age > 30 && shapes.shapeHovered(increaseSoundButt) && (inputState.lmbTime == 1 || inputState.lmbTime > 20))
        gvars::soundVolume = math::clamp(gvars::soundVolume+1,0,100);

    if(menu.age > 30 && shapes.shapeHovered(decreaseMusicButt) && (inputState.lmbTime == 1 || inputState.lmbTime > 20))
    {
        gvars::musicVolume = math::clamp(gvars::musicVolume-1,0,100);
        setMusicVolume();
    }


    if(menu.age > 30 && shapes.shapeHovered(increaseMusicButt) && (inputState.lmbTime == 1 || inputState.lmbTime > 20))
    {
        gvars::musicVolume = math::clamp(gvars::musicVolume+1,0,100);
        setMusicVolume();
    }


    if(menu.age > 30 && shapes.shapeClicked(saveGameButt))
    {
        saveGame("Profile1");
        soundmanager.playSound("Startup.wav");
    }


    if(menu.age > 30 && shapes.shapeClicked(exitGameButt))
        window.close();

}

void drawMenus()
{
    int xOffset = gvars::mousePos.x - gvars::topLeft.x;
    int yOffset = gvars::mousePos.y - gvars::topLeft.y;
    if(inputState.key[Key::Home])
        shapes.createText(gvars::mousePos,10,sf::Color::White,"defaultPos:" + std::to_string(xOffset) + "/" + std::to_string(yOffset));
    window.setView(window.getDefaultView());

    for(auto &menu : menus)
    {
        menu.age++;
        if(menu.name == "Squaddie Menu")
            renderSquaddieMenu(menu);
        if(menu.name == "Skill Menu")
            renderSkillMenu(menu);
        if(menu.name == "Merchant Menu")
            renderMerchantMenu(menu);
        if(menu.name == "Recruitment Menu")
            renderRecruiterMenu(menu);
        if(menu.name == "Tower Menu")
            renderTowerMenu(menu);
        if(menu.name == "Escape Menu")
            renderEscapeMenu(menu);

        if(inputState.key[Key::Escape] && menu.age > 30)
            menu.toDelete = true;

    }
    window.setView(gvars::view1);
}

int getLivingFactionMemberCount(std::string faction)
{
    int livingCount = 0;
    for(auto &npc : npclist)
        if(npc.faction == faction && npc.alive)
            livingCount++;
    return livingCount;
}

int getFactionMemberCount(std::string faction)
{
    int Count = 0;
    for(auto &npc : npclist)
        if(npc.faction == faction)
            Count++;
    return Count;
}

int getFloorFactionMemberCount(int floor, std::string faction)
{
    int Count = 0;
    for(auto &npc : npclist)
        if(abs_to_index(npc.zpos/GRID_SIZE) == floor && npc.faction == faction)
            Count++;
    return Count;
}



void spawnBoss()
{
    Npc boss = *getGlobalCritter("BTRockkid");

    boss.isBoss = true;

    bool posSet = false;
    for(int x = 0; x != GRIDS; x++)
        for(int y = 0; y != GRIDS; y++)
            if(tiles[x][y][gvars::currentz].id == 3250 && !posSet)
    {
        posSet = true;

        boss.xpos = x*GRID_SIZE;
        boss.ypos = y*GRID_SIZE;
        boss.zpos = gvars::currentz*GRID_SIZE;

        if(tiles[x+1][y][gvars::currentz].id == 3250)
            boss.xpos += GRID_SIZE/2;
        if(tiles[x][y+1][gvars::currentz].id == 3250)
            boss.ypos += GRID_SIZE/2;
    }

    boss.id = gvars::globalid++;
    boss.level = 10;
    boss.maxhealth = 10000;
    boss.health = 10000;
    boss.name = "The Hardened Criminal";


    boss.faction = "Towerlings";
    boss.factionPtr = &listAt(uniFact,2);

    Item weaponry = *getGlobalItem("Shotgun");
    Item ammo = *getGlobalItem("Shell: Spread");
    ammo.amount = 10000;
    weaponry.internalitems.push_back(ammo);

    boss.inventory.push_back(weaponry);
    boss.inventory.push_back(weaponry);

    boss.skills.getSkill("Feral Body")->ranks = 2;

    npclist.push_back(boss);
}

void clearSlots(Npc &npc)
{
    for(int i = 0; i != 20; i++)
        npc.invSlots[i] = nullptr;
}



void loadTavern()
{
    recruitables.clear();
    towerTransition();

    gvars::currentz = 1;


    positionSquaddies();


    bountytower::towerLoaded = towers[0].name;
    bountytower::currentTower = &towers[0];
    bountytower::elevatoravailable = false;
    bountytower::pausewaves = true;
    //buildTower(towers[1].name);

    loadMap(towers[0].mapID,0,0,50,50);

    int xview = (96*60)/20;
    gvars::currentx = xview/2;
    gvars::currenty = xview/1.4;

    for(auto &npc : npclist)
        npc.momentum = sf::Vector2f(0,0);

    Npc barPatron = *getGlobalCritter("BTHuman");
    barPatron.maxhealth = 100000;
    barPatron.health = barPatron.getMaxHealth();
    barPatron.canmove = false;
    barPatron.moverate = 0;
    barPatron.name = "The Tender";
    barPatron.id = gvars::globalid++;
    barPatron.xpos = 2790;
    barPatron.ypos = 3090;
    barPatron.zpos = 60;
    barPatron.tags.append("[WearsHat:1]");

    //Saving the tags so each patron can provide different services.
    std::string backupTags = barPatron.tags;
    barPatron.tags.append("[BountyProvider:1]");

    npclist.push_back(barPatron);
    barPatron.name = "The Dealer";
    barPatron.xpos = 2160;
    barPatron.ypos = 2970;
    barPatron.tags = backupTags;
    barPatron.tags.append("[WeaponDealer:1]");
    npclist.push_back(barPatron);
    barPatron.name = "The Recruiter";
    barPatron.xpos = 3600;
    barPatron.ypos = 2970;
    barPatron.tags = backupTags;
    barPatron.tags.append("[Recruiter:1]");
    npclist.push_back(barPatron);

    gCtrl.phase = "Lobby";
}



void nextFloorTransition()
{

    for(auto &npc : npclist)
    {
        npc.storedPath.clear();
        npc.endPos = Vec3();
        npc.hasPath = false;
        npc.needsPath = false;
        npc.chaseDefendPos = Vec3(npc.xpos,npc.ypos,npc.zpos);
        //!npc.alive
        if(gvars::currentz != abs_to_index(npc.zpos/GRID_SIZE))
        {
            clearSlots(npc);
            leftBehind.push_back(npc);
            npc.toDelete = true;
        }
    }

    clearBullets();
    shapes.shapes.clear();

    if(gvars::currentz > bountytower::currentTower->floors)
        loadTavern();


}



void hotkeySquaddieSelect()
{
    if(inputState.key[Key::Num1] && Squaddies.size() > 0)
    {
        selectedNPCs.clear();
        selectedNPCs.push_back(Squaddies[0]);
    }
    if(inputState.key[Key::Num2] && Squaddies.size() > 1)
    {
        selectedNPCs.clear();
        selectedNPCs.push_back(Squaddies[1]);
    }
    if(inputState.key[Key::Num3] && Squaddies.size() > 2)
    {
        selectedNPCs.clear();
        selectedNPCs.push_back(Squaddies[2]);
    }
    if(inputState.key[Key::Num4] && Squaddies.size() > 3)
    {
        selectedNPCs.clear();
        selectedNPCs.push_back(Squaddies[3]);
    }
    if(inputState.key[Key::Tilde])
    {
        for(auto &squaddie : Squaddies)
            selectedNPCs.push_back(squaddie);
    }
}

void spawnEnemies()
{
    // This looks for stairs, then spawns critters from them every 60 frames until a certain cap is met.
    if((gvars::framesPassed % 60) != 0)
        return;
    if( !(getLivingFactionMemberCount("Towerlings") < 20) )
        return;
    if( !(getFloorFactionMemberCount(gvars::currentz,"Towerlings") < 50) )
        return;
    if(bountytower::pausewaves)
        return;




    //Preemptively acquiring item lists here, instead of doing it once per critter.
    std::vector<Item> ammoBullet = itemmanager.getAllofType(3);
    std::vector<Item> ammoShell = itemmanager.getAllofType(4);
    std::vector<Item> ammoMissile = itemmanager.getAllofType(5);
    std::vector<Item> rangedweps = itemmanager.getAllofType(2);
    std::vector<Item> meleeweps = itemmanager.getAllofType(1);

    RandomWeightList equipmentSet;
    equipmentSet.addEntry("Melee",100*bountytower::currentTower->floors);
    equipmentSet.addEntry("Ranged",50*(gvars::currentz*4));

    RandomWeightList meleeList;
    RandomWeightList rangedList;
    RandomWeightList bulletList;
    RandomWeightList shellList;
    RandomWeightList missileList;
    for(auto &item : itemmanager.globalItems)
    {
        if(item.type == 1)
            meleeList.addEntry(item.name,item.rarity);
        if(item.type == 2)
            rangedList.addEntry(item.name,item.rarity);
        if(item.type == 3)
            bulletList.addEntry(item.name,item.rarity);
        if(item.type == 4)
            shellList.addEntry(item.name,item.rarity);
        if(item.type == 5)
            missileList.addEntry(item.name,item.rarity);
    }



    debug("Gettin Stairs");
    std::vector<Tile*> stairs;
    std::vector<Tile*> offSwitches;
    for(int x = 0; x != GRIDS; x++)
        for(int y = 0; y != GRIDS; y++)
    {
        if(tiles[x][y][gvars::currentz].id == 2031)
            stairs.push_back(&tiles[x][y][gvars::currentz]);
        if(tiles[x][y][gvars::currentz].id == 3500 && tiles[x][y][gvars::currentz].state == "Off")
            offSwitches.push_back(&tiles[x][y][gvars::currentz]);
    }
    debug("Placin Towerlings");

    //I'm still not sure why I nabbed all stairs, then did another for loop just for them... but I kinda like it.
    //There made it also get Offswitches for me, Efficiency!
    for(auto &stair : stairs)
    {
        //Add a 25% chance for a critter to spawn.
        if(randz(1,4) != 1)
            continue;


        Npc member;
        debug("V");
        member = *getGlobalCritter("BTBlankBody");
        debug("X");
        member.faction = "Towerlings";
        debug("Y");
        member.factionPtr = &listAt(uniFact,2);
        debug("Z");
        member.xpos = stair->pos.x*GRID_SIZE+(GRID_SIZE/2);
        member.ypos = stair->pos.y*GRID_SIZE+(GRID_SIZE/2);
        member.zpos = (gvars::currentz*GRID_SIZE);
        member.id = gvars::globalid++;
        member.level = getFloorDifficulty(gvars::currentz,bountytower::currentTower->floors,bountytower::currentTower->difficulty);

        member.attributes.randomizeAttributes();

        member.health = member.getMaxHealth();


        std::string equipmentResult = equipmentSet.getRandomName();

        Item meleeWeapon = *getGlobalItem(meleeList.getRandomName());
        member.inventory.push_back(meleeWeapon);

        if(equipmentResult == "Ranged")
        {
            Item rangedWeapon = *getGlobalItem(rangedList.getRandomName());
            member.inventory.push_back(rangedWeapon);

            Item ammo;
            if(rangedWeapon.ammotype == 3)
                ammo = *getGlobalItem(bulletList.getRandomName());
            if(rangedWeapon.ammotype == 4)
                ammo = *getGlobalItem(shellList.getRandomName());
            if(rangedWeapon.ammotype == 5)
                ammo = *getGlobalItem(missileList.getRandomName());

            ammo.amount = random(1,ammo.stackSize);
            member.inventory.push_back(ammo);

            //Gun wielders wear hats!
            member.tags.append("[WearsHat:1]");
        }


        /*
        int randomEquipRoll = random(1,100);
        int gunChance = 30;
        if(randomEquipRoll <= gunChance)
        {//Gun me!
            int ranGun = random(0,rangedweps.size()-1);
            Item gun = rangedweps[ranGun];
            gun.id = gvars::globalid++;
            member.inventory.push_back(gun);

            if(gun.ammotype == 3)
            {
                int ranAmmo = random(0,ammoBullet.size()-1);
                Item ammo = ammoBullet[ranAmmo];
                ammo.id = gvars::globalid++;
                ammo.amount = random(5,30);
                member.inventory.push_back(ammo);
            }
            if(gun.ammotype == 4)
            {
                int ranAmmo = random(0,ammoShell.size()-1);
                Item ammo = ammoShell[ranAmmo];
                ammo.id = gvars::globalid++;
                ammo.amount = random(5,30);
                member.inventory.push_back(ammo);
            }
            if(gun.ammotype == 5)
            {
                int ranAmmo = random(0,ammoMissile.size()-1);
                Item ammo = ammoMissile[ranAmmo];
                ammo.id = gvars::globalid++;
                ammo.amount = random(1,5);
                member.inventory.push_back(ammo);
            }

            member.tags.append("[WearsHat:1]");

        }

        //Melee Me!
        int ranMelee = random(0,meleeweps.size()-1);
        Item melee = meleeweps[ranMelee];
        melee.id = gvars::globalid++;
        member.inventory.push_back(melee);

        */

        if(random(1,5) == 1)
            member.tags.append("[WearsBoots:1]");
        if(random(1,5) == 1)
            member.tags.append("[WearsGloves:1]");

        std::cout << "Member: " << member.id;
        printItems(member.inventory);
        std::cout << "\n \n";

        if(!offSwitches.empty() && random(1,10) == 1)
        { // Having some enemies go and guard switches instead of assaulting the squaddies.
            int randomSwitch = random(0,offSwitches.size()-1);

            member.chasePriority = "Defend";
            member.chaseDefendPos.x = offSwitches[randomSwitch]->pos.x*GRID_SIZE+(GRID_SIZE/2);
            member.chaseDefendPos.y = offSwitches[randomSwitch]->pos.y*GRID_SIZE+(GRID_SIZE/2);
            member.chaseDefendPos.z = (gvars::currentz*GRID_SIZE);
            member.chaseRange = 400;
        }


        npclist.push_back(member);


        if(onScreen(member.getPos2d()))
        {
            int soundRan = random(1,2);
            soundmanager.playSound("Thump"+str(soundRan)+".ogg");
        }

    }
    debug("Done placin Towerlings");







}



void towerVictory()
{
    shapes.createSquare(0,0,screen.x(),screen.y(),sf::Color(0,0,0,100),0,sf::Color::Transparent,&gvars::hudView);
    sf::Vector2f textPos(screen.x()/2,50);
    shapes.createText(textPos,10,sf::Color::Yellow,"$ Bounty Killed! $ \n $ Time to collect! $",&gvars::hudView);
    textPos.y += 30;
    shapes.createText(textPos,10,sf::Color::White,"The people who got in your way.",&gvars::hudView);

    textPos.x -= 100;

    int returnButt = shapes.createImageButton(textPos,texturemanager.getTexture("returnButton.png"),"",0,&gvars::hudView);
    if(shapes.shapeClicked(returnButt))
    {
        bountytower::towerVictory = false;
        loadTavern();
    }

    int xSlot = 0;
    int ySlot = 0;
    int xCapacity = (gvars::centerScreen.x - gvars::topLeft.x);
    for(auto &npc : leftBehind)
    {
        if(xSlot > (((xCapacity * 2) - 120) / (GRID_SIZE)*2) )
        {
            xSlot = 0;
            ySlot++;
        }

        npc.xpos = (gvars::centerScreen.x - xCapacity) + 60 + (30*xSlot);

        int ySet = gvars::topLeft.y + 120;
        npc.ypos = ySet + (60*ySlot);

        xSlot++;
    }
    drawNPCs(leftBehind);
}

void deadDrawTest()
{
    int xSlot = 0;
    int ySlot = 0;

    int xCapacity = (gvars::centerScreen.x - gvars::topLeft.x);

    for(int i = 0; i != 50; i++)
    {
        if(xSlot > (((xCapacity * 2) - 120) / GRID_SIZE) )
        {
            xSlot = 0;
            ySlot++;
        }
        int xposition = (gvars::centerScreen.x - xCapacity) + 60 + (60*xSlot);
        //npc.xpos = gvars::topLeft.x + (60*xSlot);

        int ySet = (gvars::topLeft.y + gvars::bottomLeft.y) / 2;
        int yposition = ySet + (60*ySlot);
        //npc.ypos = ySet + (60*ySlot);

        //npc.drawImg();
        shapes.createCircle(xposition,yposition,30,sf::Color::White);

        xSlot++;
    }
}

void bossLoop()
{
    if(bountytower::towerVictory)
        return;


    if(inputState.key[Key::F] && true == false)
    {
        for(int i = 0; i != 10; i++)
        {
            Npc corpse = *getGlobalCritter("BTBlankBody");
            corpse.alive = random(0,1);

            if(random(1,5) == 1)
                corpse.tags.append("[WearsBoots:1]");
            if(random(1,5) == 1)
                corpse.tags.append("[WearsGloves:1]");


            leftBehind.push_back(corpse);
        }

        bountytower::towerVictory = true; //towerVictory();//deadDrawTest();
    }



    int bossesDead = 0;
    int bosses = 0;
    for(auto &npc : npclist)
    {
        if(npc.isBoss)
        {
            bosses++;
            if(!npc.alive)
                bossesDead++;
        }
    }
    if(bossesDead > 0 && bossesDead == bosses)
    {
        conFact->credits += bountytower::currentTower->bountyPay;
        bountytower::towerVictory = true;
        bountytower::gameBeaten = true;
    }

}

void bountyTowerMainMenu()
{
    int startBut = shapes.createImageButton(sf::Vector2f(screen.x()/2,screen.y()/2),texturemanager.getTexture("Fortune Fortress Title.png"));
    shapes.createText(gvars::mousePos,10,sf::Color::White,"Click to begin!");

    shapes.createText(10,screen.y()-80,10,sf::Color(255,255,255),"Made by Johnny Fojtik(Dehaku Zedon)");
    shapes.createText(10,screen.y()-60,10,sf::Color(255,255,255),"Contact: dehakuzedon@gmail.com  or reddit.com/r/bountytower");
    shapes.createText(10,screen.y()-40,10,sf::Color(255,255,255),"Can't stand some of the art in this game? \nCan you do better? Contact me! I'm looking for an artist!");

    if(shapes.shapeClicked(startBut))
        loadTavern();

    sf::Vector2f exitPos(screen.x()/2,screen.y()/2+30);
    int loadButt = shapes.createImageButton(exitPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = 15050;
    exitPos.x -= 31;
    textList.createText(exitPos,9,sf::Color::White,"Load Game",gvars::hudView);
    shapes.shapes.back().layer = 15075;

    if(shapes.shapeClicked(loadButt))
        loadGame("Profile1");
}

void tavernButtons()
{
    /*
    int towerButt = shapes.createImageButton(sf::Vector2f(43*GRID_SIZE,67*GRID_SIZE),texturemanager.getTexture("yellowBook.png"));
    int towerButt2 = shapes.createImageButton(sf::Vector2f(53*GRID_SIZE,67*GRID_SIZE),texturemanager.getTexture("yellowBook.png"));

    if(shapes.shapeHovered(towerButt) || shapes.shapeHovered(towerButt2))
        shapes.createText(gvars::mousePos,15,sf::Color::Yellow,"Click to progress to bounty menus!");

    if(shapes.shapeClicked(towerButt) || shapes.shapeClicked(towerButt2))
    {
        gCtrl.phase = "Tower Selection";
    }

    */
}

void NPCbuttons()
{
    if(menus.empty())
        for(auto &npc : npclist)
    {
        if(npc.tags.find("[BountyProvider:1]") != npc.tags.npos)
        {
            int bountyButt = shapes.createImageButton(npc.getPos2d(),texturemanager.getTexture("SelectionCircle.png"));
            if(shapes.shapeHovered(bountyButt))
                shapes.createText(gvars::mousePos,15,sf::Color::Yellow,"Ready to go bounty hunting? \n(Left Mouse Button)");
            if(shapes.shapeClicked(bountyButt))
                towerMenu(Vec3());
        }
        else if(npc.tags.find("[WeaponDealer:1]") != npc.tags.npos)
        {
            int dealerButt = shapes.createImageButton(npc.getPos2d(),texturemanager.getTexture("SelectionCircle.png"));
            if(shapes.shapeHovered(dealerButt))
                shapes.createText(gvars::mousePos,15,sf::Color::Yellow,"Wanna see my gear? \n(Left Mouse Button)");
            if(shapes.shapeClicked(dealerButt))
                merchantMenu(npc.getPos());

        }
        else if(npc.tags.find("[Recruiter:1]") != npc.tags.npos)
        {
            int dealerButt = shapes.createImageButton(npc.getPos2d(),texturemanager.getTexture("SelectionCircle.png"));
            if(shapes.shapeHovered(dealerButt))
                shapes.createText(gvars::mousePos,15,sf::Color::Yellow,"Looking for some fresh meat? \n(Left Mouse Button)");
            if(shapes.shapeClicked(dealerButt))
                recruiterMenu(npc.getPos());

        }
    }
}

void displayCash()
{
    std::string cashLine = "$";
    int cashAmount = conFact->credits;

    cashLine.append( str(cashAmount) );
    int offSet = cashLine.size()*17;
    //sf::Vector2f vPos( (screen.x()-offSet)-20,0);
    sf::Vector2f vPos( 300,0);

    shapes.createText(vPos,20,sf::Color::Yellow,cashLine,&gvars::hudView);

    if(inputState.key[Key::End].time == 1 || inputState.key[Key::End].time > 60)
        conFact->credits += 50;
}

void checkDoors()
{// This starts the assault the moment a squaddie touchs a door tile.
    if(bountytower::pausewaves == false)
        return;
    if(bountytower::currentTower == nullptr)
        return;
    if(bountytower::currentTower->name == "The Tavern")
        return;


    for(auto &npc : Squaddies)
    {
        if(!isInBounds(npc->getPos2d()))
            continue;
        if(!bountytower::pausewaves)
            return;

        Tile * tile;
        tile = &tiles[gridIt(npc->xpos)][gridIt(npc->ypos)][gridIt(npc->zpos)];
        if(tile->id == 2100)
        {
            bountytower::pausewaves = false;
            chatBox.addChat("Here they come!",sf::Color::White);
            soundmanager.playSound("AngryWallabee.ogg");

            if(gvars::currentz == bountytower::currentTower->floors)
                spawnBoss();
        }

    }
}

void layHints()
{
    if(!menus.empty())
        return;

    if(bountytower::currentTower != nullptr)
    {
        if(bountytower::currentTower->name == "FantasyModern")
        {
            if(gvars::currentz == 1)
            {

                float timeHover = cos(fpsKeeper.startTime.getElapsedTime().asMilliseconds()/250)*2;
                sf::Vector2f textPos;
                textPos = sf::Vector2f(2970,3920);
                int tutBook1 = shapes.createImageButton(sf::Vector2f(textPos.x,textPos.y+timeHover),texturemanager.getTexture("tutBook.png"));
                if(shapes.shapeHovered(tutBook1))
                {
                    textPos = sf::Vector2f(textPos.x+20,textPos.y-10);
                    shapes.createSquare(textPos.x-5,textPos.y,textPos.x+900,textPos.y+40,sf::Color::Black,3,sf::Color::Cyan);
                    shapes.createText(textPos,15,sf::Color::Red,"Passing through doors will alert enemies to your presence on the floor! \n"
                            " They will flood from the stairs until they're sufficiently scared!");
                }

                textPos = sf::Vector2f(3030,2790);
                int tutBook2 = shapes.createImageButton(sf::Vector2f(textPos.x,textPos.y+timeHover),texturemanager.getTexture("tutBook.png"));
                if(shapes.shapeHovered(tutBook2))
                {
                    textPos = sf::Vector2f(textPos.x+20,textPos.y-10);
                    shapes.createSquare(textPos.x-5,textPos.y,textPos.x+900,textPos.y+60,sf::Color::Black,3,sf::Color::Cyan);
                    shapes.createText(textPos,15,sf::Color::Red,"Stand squaddies on switches to activate them, enabling the elevator. \n"
                            "Intelligence is the main factor for activating switches! Use Smart People! \n"
                            "Be sure to get everyone on the elevator before you try to leave, Or you'll abandon them!");
                }

                textPos = sf::Vector2f(2790,3920);
                int tutBook3 = shapes.createImageButton(sf::Vector2f(textPos.x,textPos.y+timeHover),texturemanager.getTexture("tutBook.png"));
                if(shapes.shapeHovered(tutBook3))
                {
                    textPos = sf::Vector2f(textPos.x+20,textPos.y-10);
                    shapes.createSquare(textPos.x-5,textPos.y,textPos.x+400,textPos.y+60,sf::Color::Black,3,sf::Color::Cyan);
                    shapes.createText(textPos,15,sf::Color::Red,"Your goal? Fix the elevator! \n"
                            "(White Tiles in the Middle) \nFind Switches, then head up!");
                }
            }

            else if(gvars::currentz == 2)
            {
                float timeHover = cos(fpsKeeper.startTime.getElapsedTime().asMilliseconds()/250)*2;
                sf::Vector2f textPos;
                textPos = sf::Vector2f(2970,3090);
                int tutBook1 = shapes.createImageButton(sf::Vector2f(textPos.x,textPos.y+timeHover),texturemanager.getTexture("tutBook.png"));
                if(shapes.shapeHovered(tutBook1))
                {
                    textPos = sf::Vector2f(textPos.x+20,textPos.y-10);
                    shapes.createSquare(textPos.x-5,textPos.y,textPos.x+1040,textPos.y+100,sf::Color::Black,3,sf::Color::Cyan);
                    shapes.createText(textPos,15,sf::Color::Red,"Advanced Tactics: You can hold Left Alt to see 'passive orders' that you can assign, floating above your units. \n"
                                                                "Assault: Automatically run off and assault enemies \n"
                                                                "Defend: Guards the position you tell them to move, attacking anyone who enters the area \n"
                                                                "You can change the radius they protect with the two buttons below the main three passive orders. \n"
                                                                "Hold Position: This unit will not willingly move unless you tell them to, They will still attack enemies they see."
                                        );
                }
            }

            else if(gvars::currentz == 4)
            {

                float timeHover = cos(fpsKeeper.startTime.getElapsedTime().asMilliseconds()/250)*2;
                sf::Vector2f textPos;
                textPos = sf::Vector2f(2940,2940);
                int tutBook1 = shapes.createImageButton(sf::Vector2f(textPos.x,textPos.y+timeHover),texturemanager.getTexture("tutBook.png"));
                if(shapes.shapeHovered(tutBook1))
                {
                    textPos = sf::Vector2f(textPos.x+20,textPos.y-10);
                    shapes.createSquare(textPos.x-5,textPos.y,textPos.x+450,textPos.y+40,sf::Color::Black,3,sf::Color::Cyan);
                    shapes.createText(textPos,15,sf::Color::Red,"Your bounty is waiting for you upstairs! \n"
                                                                "Be prepared for a fight.");
                }
            }
            else if(gvars::currentz == 5)
            {
                float timeHover = cos(fpsKeeper.startTime.getElapsedTime().asMilliseconds()/250)*2;
                sf::Vector2f textPos;
                textPos = sf::Vector2f(2970,2790);
                int tutBook1 = shapes.createImageButton(sf::Vector2f(textPos.x,textPos.y+timeHover),texturemanager.getTexture("tutBook.png"));
                if(shapes.shapeHovered(tutBook1))
                {
                    textPos = sf::Vector2f(textPos.x+20,textPos.y-10);
                    textPos = sf::Vector2f(textPos.x+10,textPos.y+10);
                    shapes.createSquare(textPos.x-5,textPos.y,textPos.x+800,textPos.y+40,sf::Color::Black,3,sf::Color::Cyan);
                    shapes.createText(textPos,15,sf::Color::Red,"Using the elevator from here will return you to the tavern. \n"
                                                                "Are you up to the challenge, or are you a blank body coward?!");
                }
            }
        }
        if(bountytower::currentTower->name == "The Tavern")
        {
            shapes.createText(2520,4170,15,sf::Color::Red,"Hover your mouse over books to see tutorial text!");

            float timeHover = cos(fpsKeeper.startTime.getElapsedTime().asMilliseconds()/250)*2;
            sf::Vector2f textPos;
            textPos = sf::Vector2f(2550,4230);
            int tutBook1 = shapes.createImageButton(sf::Vector2f(textPos.x,textPos.y+timeHover),texturemanager.getTexture("tutBook.png"));
            if(shapes.shapeHovered(tutBook1))
            {
                textPos = sf::Vector2f(textPos.x+20,textPos.y-10);
                shapes.createSquare(textPos.x-5,textPos.y,textPos.x+900,textPos.y+100,sf::Color::Black,3,sf::Color::Cyan);
                shapes.createText(textPos,15,sf::Color::Red,"Camera Control: WASD / Arrow Keys \n"
                            "Select/Order: Left Mouse Button to select your squad, Right Mouse Button to order them around. \n"
                            "You can buy equipment or recruit more squaddies from the venders up north. \n"
                            "You start with a simple magitech PDA, though you will likely want better weapons. \n"
                            "The bartender will provide you with the means to go after your first bounty."
                            );
            }

            textPos = sf::Vector2f(1950,3210);
            int tutBook2 = shapes.createImageButton(sf::Vector2f(textPos.x,textPos.y+timeHover),texturemanager.getTexture("tutBook.png"));
            if(shapes.shapeHovered(tutBook2))
            {
                textPos = sf::Vector2f(textPos.x+20,textPos.y-10);
                shapes.createSquare(textPos.x-5,textPos.y,textPos.x+900,textPos.y+100,sf::Color::Black,3,sf::Color::Cyan);
                shapes.createText(textPos,15,sf::Color::Red,"Inventory: Press I with a squaddie selected to see their stats, skills, and inventory. \n"
                                "Range Check: Press and hold Alt with squaddies selected to see their held item's attack ranges. \nRed for guns,  Green for magic, Blue for melee.\n"
                                "Squaddies have two hand slots, and can dual wield any two items! \n"
                                "You can manage their hand slots with a squaddie selected at the bottom of the screen.");
            }

            textPos = sf::Vector2f(2250,2970);
            int tutBook3 = shapes.createImageButton(sf::Vector2f(textPos.x,textPos.y+timeHover),texturemanager.getTexture("tutBook.png"));
            if(shapes.shapeHovered(tutBook3))
            {
                textPos = sf::Vector2f(textPos.x+20,textPos.y-10);
                shapes.createSquare(textPos.x-5,textPos.y,textPos.x+850,textPos.y+50,sf::Color::Black,3,sf::Color::Cyan);
                shapes.createText(textPos,15,sf::Color::Red,"Right Click to pickup items with a selected squaddie(Must be near item.) \n"
                                "You can also drop items by right clicking near the squaddie with an item from their hotbar.");
            }

            textPos = sf::Vector2f(3390,3270);
            int tutBook4 = shapes.createImageButton(sf::Vector2f(textPos.x,textPos.y+timeHover),texturemanager.getTexture("tutBook.png"));
            if(shapes.shapeHovered(tutBook4))
            {
                textPos = sf::Vector2f(textPos.x+20,textPos.y-10);
                shapes.createSquare(textPos.x-5,textPos.y,textPos.x+850,textPos.y+180,sf::Color::Black,3,sf::Color::Cyan);
                shapes.createText(textPos,15,sf::Color::Red,"You can have up to a total of 4 Squad members at once.\n"
                                "A critters stats is represented as S.P.I.C.E.D., \n"
                                "Strength, Perception, Intelligence, Charisma, Endurance, Dexterity\n"
                                "Strength, gives 10% more damage with melee attacks, per point.\n"
                                "Perception reduces ranged weapon spread. \n"
                                "Intelligence affects switch working speed. \n"
                                "Charisma, gives a 1% discount on prices, per point. (All active squad members stack) \n"
                                "Endurance, gives 1% more health, per point. \n"
                                "Dexterity, gives 1% dodge chance, per point.\n"
                                );
            }

        }
    }

}

void debugTeleportSquaddies(Vec3 telePos)
{
    for(auto &npc : npclist)
        if(npc.isSquaddie)
    {
        npc.xpos = telePos.x;
        npc.ypos = telePos.y;
        npc.zpos = telePos.z;
    }
}

void useElevator()
{
    chatBox.addChat("You progress to the next floor!", sf::Color::Blue);
    bountytower::elevatoravailable = false;
    bountytower::floorCleared = false;
    bountytower::pausewaves = true;

    gvars::currentz++;
    elevateElevatorInhabitants();
    soundmanager.playSound("ding.wav");

    nextFloorTransition();
    AnyDeletes(npclist);
    setupSquadHotKeySelection();
}

void checkFloorCleared()
{
    int enemies = 0;
    int deadEnemies = 0;
    for(auto &npc : npclist)
    {
        if(npc.faction == "Towerlings")
        {
            enemies++;
            if(!npc.alive)
                deadEnemies++;
        }
    }

    if(enemies > 0 && deadEnemies == enemies)
        if(!bountytower::floorCleared)
    {
        bountytower::floorCleared = true;
        playMusic("KerriHauntedCanyonFlute.ogg");
    }

}

void printTile()
{
    if(!isInBounds(gvars::mousePos))
        return;

    int x = gvars::mousePos.x/GRID_SIZE;
    int y = gvars::mousePos.y/GRID_SIZE;

    Tile * printTile = &tiles[x][y][gvars::currentz];

    std::cout << "====Tile Printout==== \n";
    std::cout << "printTile->deathID: " << printTile->deathID << std::endl;
    std::cout << "printTile->goesDown: " << printTile->goesDown << std::endl;
    std::cout << "printTile->goesUp: " << printTile->goesUp << std::endl;
    std::cout << "printTile->health: " << printTile->health << std::endl;

    std::cout << "\nprintTile->pos: " << printTile->pos.x << "/" << printTile->pos.y << "/" << printTile->pos.z << std::endl;
    std::cout << "printTile->state: " << printTile->state << std::endl;
    std::cout << "printTile->teleporter: " << printTile->teleporter << std::endl;
    std::cout << "printTile->telePos: " << printTile->telePos.x << "/" << printTile->telePos.y << "/" << printTile->telePos.z << std::endl;

    std::cout << "\nprintTile->id: " << printTile->id << std::endl;
    std::cout << "printTile->walkable: " << printTile->walkable << std::endl;
    std::cout << "printTile->transparent: " << printTile->transparent << std::endl;
    std::cout << "printTile->workGoal: " << printTile->workGoal << std::endl;
    std::cout << "printTile->workProgress: " << printTile->workProgress << std::endl;
    std::cout << "===================== \n";
}

void makeBlood()
{
    if(!inputState.key[Key::LShift])
        return;

    if(inputState.lmbTime != 1)
        return;

    Shape splatter;
    splatter.startPos = sf::Vector2f(gvars::mousePos.x-30,gvars::mousePos.y-30);
    splatter.endPos = sf::Vector2f(gvars::mousePos.x+30,gvars::mousePos.y+30);

    splatter.shape = splatter.Square;
    splatter.duration = 2000000000;
    splatter.maincolor = sf::Color(255,255,255,255);

    int bloodNum = random(1,8);

    //splatter.texture = &texturemanager.getTexture("Blood"+str(bloodNum)+".png");
    splatter.texture = &texturemanager.getTexture("MeatCorpse.png");

    //splatter.rotation = random(1,360);

    shapes.shapes.push_back(splatter);
    shapes.shapes.back().layer += random(1,100);
}

void corpsesBleed()
{ // Such a friendly title.
    for(auto &npc : npclist)
    {
        //Alive? Get outta here!
        if(npc.alive)
            continue;

        npc.deadFrames++;
        //Dead for too long? Get outta here!
        if(npc.deadFrames > 15)
            continue;

        //Robot? Get outta here!
        if(npc.getRace() == "BTTurret")
            continue;
        //Too soon? Get outta here!
        if((gvars::framesPassed % 5) != 0)
            continue;

        //Not moving? Get outta here!
        if(npc.momentum == sf::Vector2f())
            continue;

        Shape splatter;
        splatter.startPos = sf::Vector2f(npc.xpos-30,npc.ypos-30);
        splatter.endPos = sf::Vector2f(npc.xpos+30,npc.ypos+30);

        splatter.shape = splatter.Square;
        splatter.duration = 2000000000;
        splatter.maincolor = sf::Color(255,0,0,127);

        int bloodNum = random(1,8);
        splatter.texture = &texturemanager.getTexture("Blood"+str(bloodNum)+".png");
        shapes.shapes.push_back(splatter);
        bloodNum = random(1,8);
        splatter.texture = &texturemanager.getTexture("Blood"+str(bloodNum)+".png");
        shapes.shapes.push_back(splatter);
    }

}

void chasePriorityFunction()
{
    if(!inputState.key[Key::LAlt])
        return;

    for(auto &npc : Squaddies)
    {
        sf::Texture * arrowButt = &texturemanager.getTexture("ArrowButton.png");

        //Multiplying by Boolean, BOOYAH!
        int assaultRotate = 180*(npc->chasePriority == "Assault");
        int defendRotate = 180*(npc->chasePriority == "Defend");
        int holdRotate = 180*(npc->chasePriority == "Hold Position");

        //I love shortening button to butt.
        int assaultButt = shapes.createImageButton(sf::Vector2f(npc->xpos-30,npc->ypos-90),*arrowButt,"",assaultRotate);
        int defendButt = shapes.createImageButton(sf::Vector2f(npc->xpos,npc->ypos-90),*arrowButt,"",defendRotate);
        int holdButt = shapes.createImageButton(sf::Vector2f(npc->xpos+30,npc->ypos-90),*arrowButt,"",holdRotate);
        sf::Vector2f mouseConv(gvars::mousePos.x+20,gvars::mousePos.y);

        if(shapes.shapeHovered(assaultButt))
        {
            shapes.createText(mouseConv,10,sf::Color::White,"Assault Order, Constantly hunts down enemies.");
            if(inputState.lmb)
                npc->chasePriority = "Assault";
        }
        if(shapes.shapeHovered(defendButt))
        {
            shapes.createText(mouseConv,10,sf::Color::White,"Defend Order, Guards an area you define, centered on their lost move order.");
            if(inputState.lmb)
                npc->chasePriority = "Defend";

        }
        if(shapes.shapeHovered(holdButt))
        {
            shapes.createText(mouseConv,10,sf::Color::White,"Hold Position Order, Will not move automatically.");
            if(inputState.lmb)
                npc->chasePriority = "Hold Position";
        }

        int decreaseRadiusButt = shapes.createImageButton(sf::Vector2f(npc->xpos-15,npc->ypos-70),*arrowButt,"",-90);
        int increaseRadiusButt = shapes.createImageButton(sf::Vector2f(npc->xpos+15,npc->ypos-70),*arrowButt,"",90);

        if(shapes.shapeHovered(decreaseRadiusButt))
        {
            shapes.createText(mouseConv,10,sf::Color::White,"Decrease Defend Radius");
            if(inputState.lmbTime == 1 || inputState.lmbTime > 15)
                npc->chaseRange--;
        }

        if(shapes.shapeHovered(increaseRadiusButt))
        {
            shapes.createText(mouseConv,10,sf::Color::White,"Increase Defend Radius");
            if(inputState.lmbTime == 1 || inputState.lmbTime > 15)
                npc->chaseRange++;
        }


        if(npc->chasePriority == "Defend")
            shapes.createCircle(npc->chaseDefendPos.x,npc->chaseDefendPos.y,npc->chaseRange,sf::Color::Transparent,2,sf::Color::White);

    }
}

void showItemProgressCone()
{
    for(auto &npc : npclist)
    {
        if(!npc.alive)
            continue;

        for(int i = 0; i != 2; i++)
        {
            Item * curHand = npc.invSlots[i];
            if(curHand == nullptr)
                continue;

            if(!curHand->isReady())
            {
                sf::Color coneColor;
                if(curHand->type == 1)
                    coneColor = sf::Color::Blue;
                if(curHand->type == 2)
                    coneColor = sf::Color::Red;
                if(curHand->type == 3)
                    coneColor = sf::Color::Green;

                float remainingTime = curHand->activaterate / curHand->activateratemax;
                remainingTime = 1 - remainingTime;

                shapes.createCone(npc.getPos2d(),npc.angle,180*(remainingTime),20,coneColor);
            }
        }
    }
}

void newSlotWorkMethod()
{
    if(selectedNPCs.empty())
        return;

    int slotCount;
    slotCount = 0;
    if(inputState.key[Key::Z])
        slotCount = 8;
    if(inputState.key[Key::X])
        slotCount = 9;
    if(inputState.key[Key::C])
        slotCount = 12;
    if(inputState.key[Key::V])
        slotCount = 22;

    sf::Texture * slotTex = &texturemanager.getTexture("BeltSlot.png");


    for(int i = 0; i != slotCount; i++)
    {
        if(i < 2)
            continue;

        float xDrawPos;
        float yDrawPos;

        xDrawPos = gvars::slotPos[i].x-30;
        yDrawPos = gvars::slotPos[i].y-30;

        //sf::Color primary(0,0,0,100);
        //sf::Color secondary(150,0,0);

        //shapes.createSquare(xDrawPos+5,yDrawPos+5,xDrawPos+60-5,yDrawPos+60-5,primary,1,secondary,&gvars::hudView);
        {

            Shape evar;
            evar.shape = Shape::Button;

            evar.maincolor = sf::Color(255,255,255,255);
            evar.startPos = gvars::slotPos[i];
            evar.endPos = sf::Vector2f(gvars::slotPos[i].x+slotTex->getSize().x, gvars::slotPos[i].y+slotTex->getSize().y);
            evar.texture = slotTex;
            evar.drawView = &gvars::hudView;
            shapes.shapes.push_back(evar);
        }

        //shapes.createImageButton(gvars::slotPos[i],*slotTex,"",0,&gvars::hudView);
    }

    /*
    float xCenter = screen.x()/2;
    float yPos = 600;
    bool xFlip = false;
    bool yFlip = false;
    int xOffset = 0;

    for(int i = 0; i != slotCount; i++)
    {
        float xDrawPos;
        float yDrawPos;
        if(xFlip)
            xDrawPos = xCenter + (xOffset*30);
        else
            xDrawPos = xCenter + -(xOffset*30);

        if(yFlip)
            yDrawPos = yPos + 60;
        else
            yDrawPos = yPos;

        shapes.createSquare(xDrawPos+5,yDrawPos,xDrawPos+60-5,yDrawPos+60,sf::Color::Cyan,0,sf::Color::Cyan,&&gvars::hudView);

        shapes.createText(sf::Vector2f(xDrawPos,yDrawPos),10,sf::Color::White,"Slot: " + str(i),&gvars::hudView);

        /*
        if(i < 2)
            shapes.createSquare(xDrawPos+5,yPos,xDrawPos+60-5,yPos+120,sf::Color::Cyan,0,sf::Color::Cyan,&&gvars::hudView);
        else
            shapes.createSquare(xDrawPos+5,yPos,xDrawPos+60-5,yPos+60,sf::Color::Cyan,0,sf::Color::Cyan,&&gvars::hudView);


        if(xFlip)
            xOffset++;

        toggle(xFlip);
        if((i % 4) == 0)
            toggle(yFlip);
    }

    */
}

void layerTest(int newThings)
{
    for(int i = 0; i != newThings; i++)
    {
        int randomShape = random(0,3);

        Shape shape;
        shape.startPos = sf::Vector2f(random(0,screen.x()),random(0,screen.y()));
        shape.endPos = sf::Vector2f(random(0,screen.x()),random(0,screen.y()));
        shape.duration = 600;
        shape.size = random(5,20);
        shape.outline = 1;
        shape.seccolor = sf::Color::Black;


        if(randomShape == 0) // Square
        {
            shape.shape = Shape::Square;
            shape.maincolor = sf::Color::Red;
            shape.layer = 50;
        }

        if(randomShape == 1) // Circle
        {
            shape.shape = Shape::Circle;
            shape.maincolor = sf::Color::Green;
            shape.layer = 5000;
        }

        if(randomShape == 2) // Line
        {
            shape.shape = Shape::Line;
            shape.maincolor = sf::Color::Blue;
            shape.layer = 500000;
        }

        if(randomShape == 3) // Text
        {
            shape.shape = Shape::Text;
            shape.maincolor = sf::Color::White;
            shape.text = randomWindowName();
            shape.layer = 50000000;
        }
        shapes.shapes.push_back(shape);
        shapes.layerSortAlpha();
    }
}



void testStatusEffects()
{
    if(inputState.key[Key::LShift] && inputState.key[Key::G].time == 1)
    {
        StatusEffect testStatus;
        for(auto &status : globalStatusEffects.statusEffects)
            if(status.name == "Test One")
                testStatus = status;

        for(auto &npc : Squaddies)
            npc->statusEffects.push_back(testStatus);

        /*

        std::string testString = "[Name:On Fire][Duration: 180][OverlayImage:LilFire.png][Aspect:AffectHealth:-1][Aspect:Immunity:1:Cold][Aspect:Mark:1:Monkey][Aspect:AmmoCost:500][Aspect:SpawnCreatureOnDeath:2:BTRockkid]";

        sf::Clock clock;
        sf::Time timer = clock.restart();
        std::vector<std::string> aspectContainer = stringFindVectorChaos(testString,"[Aspect:","]");
        timer = clock.restart();
        std::cout << "Newstring test: " << timer.asMicroseconds() << std::endl;

        for(auto &strings : aspectContainer)
        {
            std::cout << "New Elements: " << strings << std::endl;
        }

        */

    }

    if(inputState.key[Key::LShift] && inputState.key[Key::F].time == 1)
    {
        StatusEffect testStatus;
        for(auto &status : globalStatusEffects.statusEffects)
            if(status.name == "Test Two")
                testStatus = status;

        for(auto &npc : Squaddies)
            npc->statusEffects.push_back(testStatus);




    }

    if(inputState.key[Key::LShift] && inputState.key[Key::H].time == 1)
    {
        StatusEffect testStatus;
        for(auto &status : globalStatusEffects.statusEffects)
            if(status.name == "On Fire")
                testStatus = status;

        Item statusItem = *getGlobalItem("Bullet: Incinderary");
        statusItem.xpos = gvars::mousePos.x;
        statusItem.ypos = gvars::mousePos.y;
        statusItem.zpos = gvars::currentz*GRID_SIZE;

        //testStatus.duration = 1; // Doing this here so it's a one time process, rather than every time it's tossed on someone.
        statusItem.statusEffectsInflict.push_back(testStatus);

        //statusItem.name = "Fire Gun";
        statusItem.amount = 30;

        worlditems.push_back(statusItem);
    }

    if(inputState.key[Key::LShift] && inputState.key[Key::J].time == 1)
    {
        StatusEffect testStatus;
        for(auto &status : globalStatusEffects.statusEffects)
            if(status.name == "Infider Infection")
                testStatus = status;

        Item statusItem = *getGlobalItem("Gun");
        statusItem.xpos = gvars::mousePos.x;
        statusItem.ypos = gvars::mousePos.y;
        statusItem.zpos = gvars::currentz*GRID_SIZE;

        //testStatus.duration = 1; // Doing this here so it's a one time process, rather than every time it's tossed on someone.
        statusItem.statusEffectsInflict.push_back(testStatus);

        statusItem.name = "Horror Gun";

        worlditems.push_back(statusItem);
    }



}

void testNewMenu()
{
    /*

    sf::Vector2f menuStart(900,100);
    sf::Vector2f menuEnd(800,600);
    shapes.createCircle(menuStart.x,menuStart.y,10,sf::Color::Red);
    shapes.createCircle(menuEnd.x,menuEnd.y,10,sf::Color::Blue);

    float timePass = cos(fpsKeeper.startTime.getElapsedTime().asSeconds());
    timePass += 1;
    timePass = timePass/2;

    sf::Vector2f objectPos(percentPos(100*timePass,menuStart.x,menuEnd.x),percentPos(100,menuStart.y,menuEnd.y));

    shapes.createCircle(objectPos.x,objectPos.y,10,sf::Color::Green);

    */

}

void bountyTowerLoop()
{ // Game Loop
    hotkeySquaddieSelect();
    bossLoop();
    checkDoors();
    NPCbuttons();
    layHints();
    corpsesBleed();
    chasePriorityFunction();
    showItemProgressCone();

    //newSlotWorkMethod();

    testStatusEffects();
    testNewMenu();

    if(inputState.key[Key::F9].time == 1)
    {
        soundmanager.playSound("Startup.wav");
        loadGame("Profile1");
    }





    /*

    if(inputState.key[Key::Space].time == 1)
        layerTest(30);
    if(inputState.key[Key::V].time == 1)
        layerTest(1);



    if(inputState.key[Key::Z].time == 1)
        shapes.layerSortAlpha();
    if(inputState.key[Key::X].time == 1)
        shapes.layerSortBeta();
    if(inputState.key[Key::C].time == 1)
        shapes.layerSortGamma();

        */

    makeBlood();
    if(inputState.key[Key::J])
        for(auto &squaddie : Squaddies)
    {
        for(auto &npc : npclist)
        {
            if(npc.id != squaddie->id)
            {
                bool canSee = canSeeNpcv2(*squaddie, npc);
                if(canSee)
                {
                    shapes.createLine(squaddie->xpos,squaddie->ypos,npc.xpos,npc.ypos,2,sf::Color::Green);
                }
            }
        }
    }

    if(inputState.key[Key::LShift] && inputState.lmbTime == 1)
    {
        Shape text;
        text.shape = text.Text;
        text.duration = 60;
        text.fades = true;
        text.maincolor = sf::Color::Red;
        text.size = 20;
        text.startPos = gvars::mousePos;
        text.text = "-"+str(random(0,30));
        shapes.shapes.push_back(text);
    }

    if(inputState.key[Key::End].time == 1)
        printTile();

    Vec3 mV3(gvars::mousePos.x,gvars::mousePos.y,gvars::currentz*GRID_SIZE);
    std::vector<Npc*> victims;
    for(auto &npc: npclist)
        if(npc.faction != conFact->name)
            victims.push_back(&npc);

    if(inputState.key[Key::LShift] && inputState.key[Key::M].time == 1)
        explosion(mV3,180,60);


    if(inputState.key[Key::Escape].time == 1 && menus.empty())
        escapeMenu(Vec3());

    if(inputState.key[Key::T])
        towerMenu(Vec3f());

    if(inputState.key[Key::LControl].time > 10)
        if(inputState.key[Key::LShift].time > 10)
            if(inputState.lmbTime > 10)
                debugTeleportSquaddies(Vec3(gvars::mousePos.x,gvars::mousePos.y,gvars::currentz*GRID_SIZE));


    if(inputState.key[Key::LControl].time > 10)
    { // Display current mouse position.
        std::string outPut = "MousePos: " + std::to_string(gvars::mousePos.x) + "/" + std::to_string(gvars::mousePos.y) + "/" + std::to_string(gvars::currentz*GRID_SIZE);
        outPut.append("\n" + str(gvars::mousePos.x/GRID_SIZE) + "/" + str(gvars::mousePos.y/GRID_SIZE) + "/" + str(gvars::currentz));
        shapes.createText(gvars::mousePos,10,sf::Color::White,outPut);
    }

    if(inputState.key[Key::N] && inputState.key[Key::LShift])
        bountytower::towerVictory = true;

    if(bountytower::currentTower != nullptr && bountytower::currentTower->name == "The Tavern")
        tavernButtons();

    AnyDeletes(menus);
    if(gCtrl.phase == "BTMain Menu")
        bountyTowerMainMenu();

    if(gCtrl.phase == "Tower Selection")
        towerMenu();
    if(!selectedNPCs.empty() && inputState.key[Key::I].time == 1)
    {
        if(selectedNPCs[0]->isSquaddie)
            squaddieMenu(*selectedNPCs[0]);
    }
    if(!selectedNPCs.empty() && inputState.key[Key::U].time == 1)
    {
        if(selectedNPCs[0]->isSquaddie)
            skillMenu(*selectedNPCs[0]);
    }


    if(inputState.key[Key::T].time == 1 && inputState.key[Key::LShift])
    {
        Npc boss = *getGlobalCritter("BTRockkid");
        boss.xpos = gvars::mousePos.x;
        boss.ypos = gvars::mousePos.y;
        boss.zpos = gvars::currentz*GRID_SIZE;

        boss.id = gvars::globalid++;
        boss.level = 10;
        boss.health = 10000;

        boss.faction = "Towerlings";
        boss.factionPtr = &listAt(uniFact,2);

        Item weaponry = *getGlobalItem("Minigun");
        Item ammo = *getGlobalItem("Bullet: Standard");
        ammo.amount = 10000;
        weaponry.internalitems.push_back(ammo);

        boss.inventory.push_back(weaponry);

        boss.skills.getSkill("Feral Body")->ranks = 2;

        npclist.push_back(boss);
    }


    drawMenus();


    if(gvars::tileEdit)
    {
        shapes.createText(gvars::centerScreen.x,gvars::topLeft.y,10,gvars::cycleBlue,"Debug Tilemode Engaged");
        debugTileMode();
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

    if(inputState.key[Key::V].time == 1 && inputState.key[Key::LShift])
    { // Spawn Scrap
        Item item = *getGlobalItem("Scrap");
        item.xpos = gvars::mousePos.x;
        item.ypos = gvars::mousePos.y;
        item.zpos = gvars::currentz*GRID_SIZE;
        item.amount = random(3,8);
        worlditems.push_back(item);
    }

    if(inputState.key[Key::B].time == 1 && inputState.key[Key::LShift])
    { // Spawn Cash
        Item item = *getGlobalItem("Cash");
        item.amount = random(10,100);
        item.xpos = gvars::mousePos.x;
        item.ypos = gvars::mousePos.y;
        item.zpos = gvars::currentz*GRID_SIZE;
        worlditems.push_back(item);

    }

    if(inputState.key[Key::N].time == 1 && inputState.key[Key::LShift])
    { // Spawn Ammo
        Item item = *getGlobalItem("Bullet: Standard");
        item.amount = random(5,10);
        item.xpos = gvars::mousePos.x;
        item.ypos = gvars::mousePos.y;
        item.zpos = gvars::currentz*GRID_SIZE;
        worlditems.push_back(item);
    }


    /*

    if(inputState.key[Key::G].time == 1 && inputState.key[Key::LShift])
    { // Renew Menu Towers
        towers.clear();
        int Amt = randz(5,30);
        for(int i = 0; i != Amt; i++)
        {
            Tower tower;
            towers.push_back(tower);
        }
    }

    */

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

    /*
    for(auto &npc : npclist)
    {
        //if(npc.skills.getRanks("Knockback") > 0)
        {
            int amount = 30;
            int ranks = 5;//npc.skills.getRanks("Knockback");
            sf::Vector2f AtkerPos = gvars::mousePos;
            sf::Vector2f VictPos = npc.getPos2d();
            float attackAngle = math::angleBetweenVectors(AtkerPos,VictPos);
            sf::Vector2f finalPos = math::angleCalc(VictPos,attackAngle,(amount*ranks));

            shapes.createCircle(finalPos.x,finalPos.y,10,sf::Color::Red);

            if(inputState.lmbTime == 3)
            {
                sf::Vector2f compared(finalPos.x-npc.xpos,finalPos.y-npc.ypos);
                npc.momentum += compared;
            }
        }
    }
    */


    /*
    if(inputState.key[Key::Space])
        for(auto &npc : npclist)
    { // Move NPC's towards mouse
        npc.dirMove(gvars::mousePos);
    }
    */




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
        bool switchesExist = false;
        int remainingSwitches = 0;
        for(int x = 0; x != GRIDS; x++)
            for(int y = 0; y != GRIDS; y++)
        {
            if(tiles[x][y][gvars::currentz].id == 3500 && tiles[x][y][gvars::currentz].state == "Off")
            {
                floorComplete = false;
                remainingSwitches++;
            }
            if(tiles[x][y][gvars::currentz].id == 3500)
                switchesExist = true;
        }
        bountytower::switchesRemain = remainingSwitches;

        if(floorComplete && switchesExist)
        {
            bountytower::elevatoravailable = true;


            std::string outPut = "*Ding!*";
            chatBox.addChat(outPut,sf::Color(255,150,150));
            outPut = "*Elevator is now functional, You may now progress* ";
            chatBox.addChat(outPut,sf::Color(255,150,150));
            soundmanager.playSound("ding.wav"); // zomfg sounds, so gud.
        }




    }

    if(bountytower::switchesRemain > 0)
    {
        sf::Vector2f vPos(screen.x()/1.5,130);
        shapes.createText(vPos,15,sf::Color::White,"Remaining Elevator Components: " + std::to_string(bountytower::switchesRemain),&gvars::hudView);
    }


    spawnEnemies();


    if(bountytower::towerLoaded != "")
        shapes.createText(sf::Vector2f(screen.x()/2,15),15,sf::Color::White,"Floor: " + std::to_string(gvars::currentz),&gvars::hudView);

    displayCash();

    checkFloorCleared();

    if(bountytower::elevatoravailable && bountytower::towerLoaded != "" && !bountytower::towerVictory)
    { // Prints Elevator HUD and other such things


        shapes.createText(sf::Vector2f(screen.x()/2-110,50),20,sf::Color::Green,"Elevator is Ready!",&gvars::hudView);

        int AmountRaised = 0;
        for(auto &npc : npclist)
            if(tiles[abs_to_index(npc.xpos/GRID_SIZE)][abs_to_index(npc.ypos/GRID_SIZE)][gvars::currentz].id == 3202)
                if(npc.faction == conFact->name)
                    AmountRaised++;

        shapes.createText(sf::Vector2f(screen.x()/2-110,70),20,sf::Color::White,"On Elevator: " + std::to_string(AmountRaised),&gvars::hudView);

        sf::Vector2f vPos((screen.x()/2)-25-110,75);
        int butt = shapes.createImageButton(vPos,texturemanager.getTexture("ElevatorButton.png"),"",0,&gvars::hudView);
        if(shapes.shapeClicked(butt))
        {
            selectedNPCs.clear();
            if(AmountRaised == 0)
                chatBox.addChat("No one's on the elevator!", sf::Color::Red);
            else if (gvars::currentz < CHUNK_SIZE - 1)
                useElevator();
            else
                chatBox.addChat("You cannot go further up.", sf::Color::Red);
        }
    }

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
    Tower * currentTower = nullptr;
    int switchesRemain = 0;
    bool towerVictory = false;
    bool floorCleared = false;
    bool gameBeaten = false;
}
