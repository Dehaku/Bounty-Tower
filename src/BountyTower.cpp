#include "BountyTower.h"




std::vector<Npc*> Squaddies;
std::list<Npc> leftBehind;

int gridIt(int num)
{
    return num/GRID_SIZE;
}

void setupSquadHotKeySelection()
{
    Squaddies.clear();
    for(auto &npc : npclist)
        if(npc.faction == "The Titanium Grip")
            Squaddies.push_back(&npc);
}

void equipStarters()
{
    int currentCritter = 0;
    for(auto &member : npclist)
    {
        member.name = generateName();
        member.isSquaddie = true;

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

    conFact->credits = 750;

    addMembers(1,"The Titanium Grip");
    equipStarters();


    FactPtr = addFaction("Towerlings");
    std::cout << FactPtr->name << " added. \n";
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

    int bossrugButt = createImageButton(sf::Vector2f(menuEdgeL+30+60+60+60,menuEdgeU+30+60+60+60),texturemanager.getTexture("BTRug.png"));
    if(imageButtonClicked(bossrugButt))
        debugTileKeeper.useTile.BTbossrug();




}

void towerTransition()
{
    worlditems.clear();
    leftBehind.clear();
    clearBullets();


    for(auto &npc : npclist)
    {
        npc.storedPath.clear();
        npc.endPos = Vec3();
        npc.hasPath = false;
        npc.needsPath = false;
        npc.momentum = sf::Vector2f(0,0);

        if(npc.faction != conFact->name)
            npc.toDelete = true;
    }

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
    int fanTowButt = createImageButton(towerPos[0],*towers[1].tex);
    if(imageButtonClicked(fanTowButt))
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
        textList.createText(towerPos[0].x+50,towerPos[0].y-25,15,sf::Color::Red,textOut);
    }

    shapes.createSquare(-60,-60,RESOLUTION.x+60,RESOLUTION.y+60,sf::Color::Black);

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


    int returnButt = createImageButton(sf::Vector2f(RESOLUTION.x-50,50),texturemanager.getTexture("returnButton.png"),"",0,gvars::hudView);
    if(imageButtonClicked(returnButt))
    {
        gCtrl.phase = "Lobby";
        int xview = (96*60)/20;
        gvars::currentx = xview/2;
        gvars::currenty = xview/1.4;
    }


}




baseMenu::baseMenu()
{
    age = 0;
    toDelete = false;
}

std::list<baseMenu> menus;

void squaddieMenu(Npc &npc)
{
    for(auto menu : menus)
        if(menu.name == "Squaddie Menu")
            return;
    std::cout << "This squaddie has, Health: " << npc.health << ", items: " << npc.inventory.size() << std::endl;
    baseMenu sMenu;
    sMenu.name = "Squaddie Menu";
    sMenu.Pos = sf::Vector2f(RESOLUTION.x/2,RESOLUTION.y/2);
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
    sMenu.Pos = sf::Vector2f(RESOLUTION.x/2,RESOLUTION.y/2);
    sMenu.makePos = creationPos;
    menus.push_back(sMenu);
}

void recruiterMenu(Vec3f creationPos)
{
    for(auto menu : menus)
        if(menu.name == "Recruitment Menu")
            return;
    baseMenu sMenu;
    sMenu.name = "Recruitment Menu";
    sMenu.Pos = sf::Vector2f(RESOLUTION.x/2,RESOLUTION.y/2);
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
    sMenu.Pos = sf::Vector2f(RESOLUTION.x/2,RESOLUTION.y/2);
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
    sMenu.Pos = sf::Vector2f(RESOLUTION.x/2,RESOLUTION.y/2);
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

void renderSquaddieMenu(baseMenu &menu)
{

    if(inputState.key[Key::Comma].time == 1)
    {
        for(int i = 0; i != 101; i++)
        {
            std::cout << "New Level " << i << ": " << nextLevelXpRequired(i) << "xp \n";
        }
        std::cout << "Level 15: " << nextLevelXpRequired(15);
    }



    shapes.createSquare(100,100,RESOLUTION.x-100,RESOLUTION.y-100,sf::Color(sf::Color(150,150,0)),5,sf::Color::White,&gvars::hudView);
    //Close Button
    int exitButt = createImageButton(sf::Vector2f(RESOLUTION.x-100,100),texturemanager.getTexture("ExitButton.png"),"",0,window.getDefaultView());
    if(imageButtonClicked(exitButt))
        menu.toDelete = true;

    Npc *npc = menu.npc;
    textList.createText(sf::Vector2f(105,100),20,sf::Color(100,100,100),"Name: " + npc->name,window.getView());

    //XP and Level!
    std::string LevelXpLine = "Level: " + std::to_string(npc->level) + "\n";
    LevelXpLine.append("XP: " + std::to_string(npc->xp));
    LevelXpLine.append(", (Next Level: " + std::to_string(nextLevelXpRequired(npc->level)) + ")");
    textList.createText(sf::Vector2f(105,120),20,sf::Color::White,LevelXpLine,window.getView());

    //Attributes! SPICED
    std::string AttributeLine;
    AttributeLine.append("S.P.I.C.E.D. \n");
    AttributeLine.append("Strength: " + std::to_string(npc->attributes.strength) + "\n");
    AttributeLine.append("Perception: " + std::to_string(npc->attributes.perception) + "\n");
    AttributeLine.append("Intelligence: " + std::to_string(npc->attributes.intelligence) + "\n");
    AttributeLine.append("Charisma: " + std::to_string(npc->attributes.charisma) + "\n");
    AttributeLine.append("Endurance: " + std::to_string(npc->attributes.endurance) + "\n");
    AttributeLine.append("Dexterity: " + std::to_string(npc->attributes.dexterity) + "\n");
    textList.createText(sf::Vector2f(105,170),20,sf::Color::White,AttributeLine,window.getView());




    //Inventory!
    sf::Vector2f invPos(RESOLUTION.x/2,130);
    textList.createText(sf::Vector2f(636,102),15,sf::Color::White,"Inventory",window.getView());
    shapes.createSquare(invPos.x-40,invPos.y-10,invPos.x+100,invPos.y+(RESOLUTION.y/2)+120,sf::Color::Transparent,2,sf::Color::Black,&gvars::hudView);

    int x = 0, y = 0;
    bool offSet = false;
    for(auto &item : npc->inventory)
    {
        sf::Vector2f drawPos(invPos.x,invPos.y+(60*y)+30);
        if(offSet)
            drawPos.x += 62;
        shapes.createSquare(drawPos.x-30,drawPos.y-30,drawPos.x+30,drawPos.y+30,sf::Color::Black,2,sf::Color::White,&gvars::hudView);
        createImageButton(drawPos,*item.img.getTexture(),"",0,window.getDefaultView());
        if(offSet)
        {
            drawPos.x -= 62;
            y++;
        }

        toggle(offSet);
    }

    //Skills!
    sf::Vector2f skillPos(800,105);

    if(npc->skillpoints > 0)
        textList.createText(skillPos,15,sf::Color::White,"Skills   SP Remaining: " + std::to_string(npc->skillpoints),window.getView());
    else
        textList.createText(skillPos,15,sf::Color::White,"Skills",window.getView());

    y = 0;
    std::string lastTree = "";
    for(auto &skill : npc->skills.list)
    {
        std::string outPut = "(" + std::to_string(skill.ranks) + "/" + std::to_string(skill.ranksmax) + ")" + skill.name;
        sf::Vector2f drawPos(skillPos.x,skillPos.y+(y*15)+15);
        if(skill.tree != lastTree)
        {
            lastTree = skill.tree;
            textList.createText(drawPos,20,sf::Color::Red,skill.tree,window.getView());
            y++;
        }
        drawPos = sf::Vector2f(skillPos.x,skillPos.y+(y*15)+15);
        //else
        {
            //textList.createText(drawPos,20,sf::Color::White,outPut,window.getView());
            sf::Vector2f buttonPos(drawPos.x-40,drawPos.y+10);


            int decreaseSkillButton = createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",-90,window.getView());
            buttonPos.x += 20;
            int increaseSkillButton = createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",90,window.getView());

            if(imageButtonHovered(increaseSkillButton) || imageButtonHovered(decreaseSkillButton))
            {
                textList.createText(drawPos,20,sf::Color::Cyan,outPut,window.getView());
                sf::Vector2f textPos(RESOLUTION.x/8,70);
                shapes.createSquare(0,textPos.y,RESOLUTION.x,textPos.y+13,sf::Color::Black,1,sf::Color::White,&gvars::hudView);
                textList.createText(textPos,10,sf::Color::Cyan,skill.desc,window.getView());
            }
            else
                textList.createText(drawPos,20,sf::Color::White,outPut,window.getView());

            if(imageButtonClicked(decreaseSkillButton) && skill.ranks > 0 && menu.age > 30)
            {
                skill.ranks--;
                npc->skillpoints++;
            }

            if(imageButtonClicked(increaseSkillButton) && skill.ranks < skill.ranksmax && npc->skillpoints > 0 && menu.age > 30)
            {
                skill.ranks++;
                npc->skillpoints--;
            }

        }



        y++;
    }

}

void renderMerchantMenu(baseMenu &menu)
{
    shapes.createSquare(100,100,RESOLUTION.x-100,RESOLUTION.y-100,sf::Color(sf::Color(150,150,0)),5,sf::Color::White,&gvars::hudView);
    //Close Button
    int exitButt = createImageButton(sf::Vector2f(RESOLUTION.x-100,100),texturemanager.getTexture("ExitButton.png"),"",0,window.getDefaultView());
    if(imageButtonClicked(exitButt))
        menu.toDelete = true;

    int xOffset = 0;
    int yOffset = 0;
    for(auto item : itemmanager.globalItems)
    {
        if(item.value == -1)
            continue;
        int posX = 150+(xOffset*300);
        int posY = 150+(yOffset*60);
        shapes.createSquare(posX-30,posY-30,posX+30,posY+30,sf::Color::Black,0,sf::Color::Cyan, &gvars::hudView);

        sf::Vector2f vPos(posX,posY);
        int itemButt = createImageButton(vPos,*item.img.getTexture(),"",0,gvars::hudView);

        sf::Color highlightColor = sf::Color::White;
        if(imageButtonHovered(itemButt))
            highlightColor = sf::Color::Cyan;

        vPos.y -= 30;
        vPos.x += 30;
        std::string outPut = item.name;


        textList.createText(vPos,15,highlightColor,outPut,gvars::hudView);
        vPos.y += 15;
        textList.createText(vPos,10,highlightColor,"$" + str(item.value),gvars::hudView);
        vPos.y += 15;
        std::string stats = "Dam: " + str(item.mindam) + "/" + str(item.maxdam);
        if(item.type == 1 || item.type == 2 || item.type == 23)
            stats.append(", AtkDelay: " + str(static_cast<int>(item.activateratemax / item.activaterategrowth)));
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

        if(item.stackSize > 1)
            stats.append("\nStack: " + str(item.stackSize));
        textList.createText(vPos,10,highlightColor,stats,gvars::hudView);



        if(imageButtonClicked(itemButt) && menu.age > 30)
        {
            if(conFact->credits < item.value)
                chatBox.addChat("You do not have enough cash for "+item.name+"!", sf::Color::White);
            else
            {
                conFact->credits -= item.value;
                Item soldItem = item;
                soldItem.id = gvars::globalid++;
                soldItem.xpos = menu.makePos.x+(randz(-90,90));
                soldItem.ypos = menu.makePos.y+90;
                soldItem.zpos = menu.makePos.z;
                soldItem.amount = soldItem.stackSize;

                worlditems.push_back(soldItem);
                int soundRan = random(1,3);
                if(soundRan == 1)
                    soundmanager.playSound("CashPickup1.ogg");
                if(soundRan == 2)
                    soundmanager.playSound("CashPickup2.ogg");
                if(soundRan == 3)
                    soundmanager.playSound("CashPickup3.ogg");

                chatBox.addChat("You purchased a "+item.name+" for "+str(item.value)+"!", sf::Color::White);
            }
        }



        yOffset++;
        if(yOffset > 7)
        {
            yOffset = 0;
            xOffset++;
        }
    }
}

void renderRecruiterMenu(baseMenu &menu)
{
    shapes.createSquare(100,100,RESOLUTION.x-100,RESOLUTION.y-100,sf::Color(sf::Color(150,150,0)),5,sf::Color::White,&gvars::hudView);
    //Close Button
    int exitButt = createImageButton(sf::Vector2f(RESOLUTION.x-100,100),texturemanager.getTexture("ExitButton.png"),"",0,window.getDefaultView());
    if(imageButtonClicked(exitButt))
        menu.toDelete = true;

    int xOffset = 0;
    int yOffset = 0;

    std::string SPICED = "SPICED = Strength Perception Intelligence Charisma Endurance Dexterity";
    textList.createText(sf::Vector2f(105,100),10,sf::Color::White,SPICED,gvars::hudView);

    for(auto npc : npcmanager.globalCritter)
    {
        if(!npc.recruitable)
            continue;
        int critterCost = 100;
        int posX = 150+(xOffset*300);
        int posY = 150+(yOffset*60);;
        shapes.createSquare(posX-30,posY-30,posX+30,posY+30,sf::Color::Black,0,sf::Color::Cyan, &gvars::hudView);



        sf::Vector2f vPos(posX,posY);

        int npcButt = createImageButton(vPos,texturemanager.getTexture("SpriteSheet"+npc.race+"Frame.png"),"",0,gvars::hudView);
        // hehe... npc butt.
        sf::Color highlightColor = sf::Color::White;
        if(imageButtonHovered(npcButt))
            highlightColor = sf::Color::Cyan;

        vPos.y -= 30;
        vPos.x += 30;
        textList.createText(vPos,15,highlightColor,npc.name,gvars::hudView);
        vPos.y += 15;
        textList.createText(vPos,10,highlightColor,"$" + str(critterCost),gvars::hudView);
        vPos.y += 10;
        std::string outPut = "Speed: " + str(static_cast<int>(npc.moverate) );
        outPut.append("\nS.P.I.C.E.D. : ");
        outPut.append(str(npc.attributes.strength) + ",");
        outPut.append(str(npc.attributes.perception) + ",");
        outPut.append(str(npc.attributes.intelligence) + ",");
        outPut.append(str(npc.attributes.charisma) + ",");
        outPut.append(str(npc.attributes.endurance) + ",");
        outPut.append(str(npc.attributes.dexterity));
        textList.createText(vPos,10,highlightColor,outPut,gvars::hudView);



        if(imageButtonClicked(npcButt) && menu.age > 30)
        {
            if(Squaddies.size() >= 4)
                chatBox.addChat("You already have a full squad!", sf::Color::White);
            else if(conFact->credits < critterCost)
                chatBox.addChat("You do not have enough cash for "+npc.name+"!", sf::Color::White);
            else
            {
                conFact->credits -= critterCost;
                Npc soldNpc = npc;
                soldNpc.id = gvars::globalid++;
                soldNpc.xpos = menu.makePos.x+(randz(-30,30));
                soldNpc.ypos = menu.makePos.y+30;
                soldNpc.zpos = menu.makePos.z;
                soldNpc.factionPtr = conFact;
                soldNpc.faction = conFact->name;
                soldNpc.name = generateName();
                soldNpc.tags.append("[WearsHat:1]");
                soldNpc.isSquaddie = true;

                npclist.push_back(soldNpc);
                int soundRan = random(1,3);
                if(soundRan == 1)
                    soundmanager.playSound("CashPickup1.ogg");
                if(soundRan == 2)
                    soundmanager.playSound("CashPickup2.ogg");
                if(soundRan == 3)
                    soundmanager.playSound("CashPickup3.ogg");

                chatBox.addChat("You purchased a "+npc.name+" for "+str(critterCost)+"!", sf::Color::White);
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
}

void renderTowerMenu(baseMenu &menu)
{
    shapes.createSquare(100,100,RESOLUTION.x-100,RESOLUTION.y-100,sf::Color(sf::Color(150,150,0)),5,sf::Color::White,&gvars::hudView);
    //Close Button
    int exitButt = createImageButton(sf::Vector2f(RESOLUTION.x-100,100),texturemanager.getTexture("ExitButton.png"),"",0,window.getDefaultView());
    if(imageButtonClicked(exitButt))
        menu.toDelete = true;

        //Screen position defaults
    int xPart = RESOLUTION.x/5;
    int yPart = RESOLUTION.y/3;
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

    int fanTowButt = createImageButton(towerPos[0],*towers[1].tex,"",0,gvars::hudView);
    if(imageButtonClicked(fanTowButt))
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

    }

    //Draw some info about the thing.
    {
        std::string textOut = " Tower: " + towers[1].name +
        //"\n Minions: " + std::to_string(towers[1].minioncount) +
        "\n Target: " + "Rockkid, The Hardened Criminal" +
        "\n Bounty: $" + str(towers[1].bountyPay) +
        "\n Difficulty: " + std::to_string(towers[1].difficulty);
        sf::Vector2f textPos(towerPos[0].x+50,towerPos[0].y-25);
        textList.createText(textPos,15,sf::Color::Red,textOut,gvars::hudView);
    }



}

void renderEscapeMenu(baseMenu &menu)
{
    shapes.createSquare(100,100,RESOLUTION.x-100,RESOLUTION.y-100,sf::Color(sf::Color(150,150,0)),5,sf::Color::White,&gvars::hudView);
    //Close Button
    int exitButt = createImageButton(sf::Vector2f(RESOLUTION.x-100,100),texturemanager.getTexture("ExitButton.png"),"",0,window.getDefaultView());
    if(imageButtonClicked(exitButt))
        menu.toDelete = true;

    sf::Vector2f textPos( (RESOLUTION.x/2) - 50,110);
    sf::Vector2f buttonPos(textPos);
    textList.createText(textPos,10,sf::Color::White,"Sound: " + str(gvars::soundVolume),gvars::hudView);
    buttonPos.x += 90;
    buttonPos.y += 15;
    int decreaseSoundButt = createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",-90,gvars::hudView);
    buttonPos.x += 30;
    int IncreaseSoundButt = createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",90,gvars::hudView);



    textPos.y += RESOLUTION.y/8;
    buttonPos = textPos;
    textList.createText(textPos,10,sf::Color::White,"Music: " + str(gvars::musicVolume),gvars::hudView);
    buttonPos.x += 90;
    buttonPos.y += 15;
    int decreaseMusicButt = createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",-90,gvars::hudView);
    buttonPos.x += 30;
    int IncreaseMusicButt = createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",90,gvars::hudView);


    sf::Vector2f exitPos((RESOLUTION.x/2), RESOLUTION.y-150);
    int exitGameButt = createImageButton(exitPos,texturemanager.getTexture("blankButton.png"),"",0,gvars::hudView);
    exitPos.x -= 31;
    textList.createText(exitPos,9,sf::Color::White,"Exit Game",gvars::hudView);


    if(menu.age > 30 && imageButtonHovered(decreaseSoundButt) && (inputState.lmbTime == 1 || inputState.lmbTime > 20))
        gvars::soundVolume = math::clamp(gvars::soundVolume-1,0,100);

    if(menu.age > 30 && imageButtonHovered(IncreaseSoundButt) && (inputState.lmbTime == 1 || inputState.lmbTime > 20))
        gvars::soundVolume = math::clamp(gvars::soundVolume+1,0,100);

    if(menu.age > 30 && imageButtonHovered(decreaseMusicButt) && (inputState.lmbTime == 1 || inputState.lmbTime > 20))
        gvars::musicVolume = math::clamp(gvars::musicVolume-1,0,100);

    if(menu.age > 30 && imageButtonHovered(IncreaseMusicButt) && (inputState.lmbTime == 1 || inputState.lmbTime > 20))
        gvars::musicVolume = math::clamp(gvars::musicVolume+1,0,100);



    if(menu.age > 30 && imageButtonClicked(exitGameButt))
        window.close();

}

void drawMenus()
{
    int xOffset = gvars::mousePos.x - gvars::topLeft.x;
    int yOffset = gvars::mousePos.y - gvars::topLeft.y;
    if(inputState.key[Key::Home])
        textList.createText(gvars::mousePos,10,sf::Color::White,"defaultPos:" + std::to_string(xOffset) + "/" + std::to_string(yOffset));
    window.setView(window.getDefaultView());

    for(auto &menu : menus)
    {
        menu.age++;
        if(menu.name == "Squaddie Menu")
            renderSquaddieMenu(menu);
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

    Item weaponry = *getGlobalItem("Gun");
    Item ammo = *getGlobalItem("Bullet: Armor Piercing");
    ammo.amount = 10000;
    weaponry.internalitems.push_back(ammo);

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
        //!npc.alive
        if(gvars::currentz != abs_to_index(npc.zpos/GRID_SIZE))
        {
            clearSlots(npc);
            leftBehind.push_back(npc);
            npc.toDelete = true;
        }
    }

    clearBullets();

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
    //Preemptively acquiring item lists here, instead of doing it once per critter.
    std::vector<Item> ammoBullet = itemmanager.getAllofType(3);
    std::vector<Item> ammoShell = itemmanager.getAllofType(4);
    std::vector<Item> ammoMissile = itemmanager.getAllofType(5);
    std::vector<Item> rangedweps = itemmanager.getAllofType(2);
    std::vector<Item> meleeweps = itemmanager.getAllofType(1);

    debug("Gettin Stairs");
    std::vector<Tile*> stairs;
    for(int x = 0; x != GRIDS; x++)
        for(int y = 0; y != GRIDS; y++)
    {
        if(tiles[x][y][gvars::currentz].id == 2031)
            stairs.push_back(&tiles[x][y][gvars::currentz]);
    }
    debug("Placin Towerlings");

    //I'm still not sure why I nabbed all stairs, then did another for loop just for them... but I kinda like it.
    for(auto &stair : stairs)
    {
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

        if(random(1,5) == 1)
            member.tags.append("[WearsBoots:1]");
        if(random(1,5) == 1)
            member.tags.append("[WearsGloves:1]");

        //Melee Me!
        int ranMelee = random(0,meleeweps.size()-1);
        Item melee = meleeweps[ranMelee];
        melee.id = gvars::globalid++;
        member.inventory.push_back(melee);

        std::cout << "Member: " << member.id;
        printItems(member.inventory);
        std::cout << "\n \n";

        npclist.push_back(member);
    }
    debug("Done placin Towerlings");
}



void towerVictory()
{
    shapes.createSquare(0,0,RESOLUTION.x,RESOLUTION.y,sf::Color(0,0,0,100),0,sf::Color::Transparent,&gvars::hudView);
    sf::Vector2f textPos(RESOLUTION.x/2,50);
    textList.createText(textPos,10,sf::Color::Yellow,"$ Bounty Killed! $ \n $ Time to collect! $",window.getDefaultView());
    textPos.y += 30;
    textList.createText(textPos,10,sf::Color::White,"The people who got in your way.",window.getDefaultView());

    textPos.x -= 100;

    int returnButt = createImageButton(textPos,texturemanager.getTexture("returnButton.png"),"",0,gvars::hudView);
    if(imageButtonClicked(returnButt))
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
    }

}

void bountyTowerMainMenu()
{
    int startBut = createImageButton(sf::Vector2f(RESOLUTION.x/2,RESOLUTION.y/2),texturemanager.getTexture("Fortune Fortress Title.png"));
    textList.createText(gvars::mousePos,10,sf::Color::White,"Click to begin!");

    if(imageButtonClicked(startBut))
        loadTavern();
}

void tavernButtons()
{
    /*
    int towerButt = createImageButton(sf::Vector2f(43*GRID_SIZE,67*GRID_SIZE),texturemanager.getTexture("yellowBook.png"));
    int towerButt2 = createImageButton(sf::Vector2f(53*GRID_SIZE,67*GRID_SIZE),texturemanager.getTexture("yellowBook.png"));

    if(imageButtonHovered(towerButt) || imageButtonHovered(towerButt2))
        textList.createText(gvars::mousePos,15,sf::Color::Yellow,"Click to progress to bounty menus!");

    if(imageButtonClicked(towerButt) || imageButtonClicked(towerButt2))
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
            int bountyButt = createImageButton(npc.getPos2d(),texturemanager.getTexture("SelectionCircle.png"));
            if(imageButtonHovered(bountyButt))
                textList.createText(gvars::mousePos,15,sf::Color::Yellow,"Ready to go bounty hunting? \n(Left Mouse Button)");
            if(imageButtonClicked(bountyButt))
                towerMenu(Vec3());
        }
        else if(npc.tags.find("[WeaponDealer:1]") != npc.tags.npos)
        {
            int dealerButt = createImageButton(npc.getPos2d(),texturemanager.getTexture("SelectionCircle.png"));
            if(imageButtonHovered(dealerButt))
                textList.createText(gvars::mousePos,15,sf::Color::Yellow,"Wanna see my gear? \n(Left Mouse Button)");
            if(imageButtonClicked(dealerButt))
                merchantMenu(npc.getPos());

        }
        else if(npc.tags.find("[Recruiter:1]") != npc.tags.npos)
        {
            int dealerButt = createImageButton(npc.getPos2d(),texturemanager.getTexture("SelectionCircle.png"));
            if(imageButtonHovered(dealerButt))
                textList.createText(gvars::mousePos,15,sf::Color::Yellow,"Looking for some fresh meat? \n(Left Mouse Button)");
            if(imageButtonClicked(dealerButt))
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
    sf::Vector2f vPos( (RESOLUTION.x-offSet)-20,0);

    textList.createText(vPos,20,sf::Color::Yellow,cashLine,gvars::hudView);

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
    if(bountytower::currentTower != nullptr)
    {
        if(bountytower::currentTower->name == "FantasyModern")
        {
            if(gvars::currentz == 1)
            {
                textList.createText(2940,3870,15,gvars::cycleRed,"Passing through doors will alert enemies to your presence on the floor! \n"
                            " They will flood from the stairs until they're sufficiently scared!");
                textList.createText(3000,2790,15,gvars::cycleRed,"Stand squaddies on switches to activate them, enabling the elevator. \n"
                            "Be sure to get everyone on the elevator before you try to leave, Or you'll abandon them!");

                textList.createText(2520,3900,15,gvars::cycleRed,"Your goal? Fix the elevator! \n"
                            "(White Tiles in the Middle) \nFind Switches, then head up!");
            }
            else if(gvars::currentz == 4)
            {
                textList.createText(2940,2940,15,gvars::cycleRed,"Your bounty is waiting for you. \n"
                                                                "Be prepared for a fight upstairs!");
            }
            else if(gvars::currentz == 5)
            {
                textList.createText(3000,2760,15,gvars::cycleRed,"Using the elevator from here will return you to the tavern. \n"
                                                                "Are you up to the challenge, or are you a blank body coward?!");
            }
        }
        if(bountytower::currentTower->name == "The Tavern")
        {
            textList.createText(2520,4200,15,gvars::cycleRed,"Camera Control: WASD / Arrow Keys \n"
                            "Select/Order: Left Mouse Button to select your squad, Right Mouse Button to order them around. \n"
                            "You can buy equipment or recruit more squaddies from the venders up north. \n"
                            "You start with a simple magitech PDA, though you will likely want better weapons. \n"
                            "The bartender will provide you with the means to go after your first bounty."
                            );
            if(menus.empty())
            {
                textList.createText(1920,3120,15,gvars::cycleRed,"Inventory: Press I with a squaddie selected to see their stats, skills, and inventory. \n"
                                "Range Check: Press and hold Alt with squaddies selected to see their attack ranges. \nRed for guns,  Green for magic, Blue for melee.\n"
                                "Squaddies have two hand slots, and can dual wield any two items! \n"
                                "You can manage their hand slots with a squaddie selected at the bottom of the screen.");
                textList.createText(2220,2940,15,gvars::cycleRed,"Right Click to pickup items with a selected squaddie(Must be near item.) \n"
                                "You can also drop items by right clicking near the squaddie with an item from their hotbar.");
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
    bountytower::pausewaves = true;
    gvars::currentz++;
    elevateElevatorInhabitants();
    soundmanager.playSound("ding.wav");
    nextFloorTransition();
    AnyDeletes(npclist);
    setupSquadHotKeySelection();
}

void bountyTowerLoop()
{ // Game Loop
    hotkeySquaddieSelect();
    bossLoop();
    checkDoors();
    NPCbuttons();
    layHints();

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
        textList.createText(gvars::mousePos,10,sf::Color::White,outPut);
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
        //TODO: Add a check of isSquaddie to prevent turrets from being modified/skilled.
        squaddieMenu(*selectedNPCs[0]);
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
        textList.createText(gvars::centerScreen.x,gvars::topLeft.y,10,gvars::cycleBlue,"Debug Tilemode Engaged");
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


    if(inputState.key[Key::Space])
        for(auto &npc : npclist)
    { // Move NPC's towards mouse
        npc.dirMove(gvars::mousePos);
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
        sf::Vector2f vPos(RESOLUTION.x/1.5,30);
        textList.createText(vPos,15,sf::Color::White,"Remaining Elevator Components: " + std::to_string(bountytower::switchesRemain),window.getDefaultView());
    }

    if( (gvars::framesPassed % 300) == 0 && getLivingFactionMemberCount("Towerlings") < 20 && getFloorFactionMemberCount(gvars::currentz,"Towerlings") < 50 && !bountytower::pausewaves)
    { // This looks for stairs, then spawns critters from them every 300 frames until a certain cap is met.
        spawnEnemies();
    }

    if(bountytower::towerLoaded != "")
        textList.createText(sf::Vector2f(RESOLUTION.x/2,15),15,sf::Color::White,"Floor: " + std::to_string(gvars::currentz),window.getDefaultView());

    displayCash();

    if(bountytower::elevatoravailable && bountytower::towerLoaded != "" && !bountytower::towerVictory)
    { // Prints Elevator HUD and other such things


        textList.createText(sf::Vector2f(RESOLUTION.x/2,50),20,sf::Color::Green,"Elevator is Ready!",window.getDefaultView());

        int AmountRaised = 0;
        for(auto &npc : npclist)
            if(tiles[abs_to_index(npc.xpos/GRID_SIZE)][abs_to_index(npc.ypos/GRID_SIZE)][gvars::currentz].id == 3202)
                if(npc.faction == conFact->name)
                    AmountRaised++;

        textList.createText(sf::Vector2f(RESOLUTION.x/2,70),20,sf::Color::White,"On Elevator: " + std::to_string(AmountRaised),window.getDefaultView());

        sf::Vector2f vPos((RESOLUTION.x/2)-25,75);
        int butt = createImageButton(vPos,texturemanager.getTexture("ElevatorButton.png"),"",0,window.getDefaultView());
        if(imageButtonClicked(butt))
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
}
