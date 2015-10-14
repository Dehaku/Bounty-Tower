#include "BountyTower.h"




std::vector<Npc*> Squaddies;

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

            item = *getGlobalItem("Shotgun - Spread");
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
            bullet = *getGlobalItem("Bullet - Standard");
            bullet.amount = 30;

            member.inventory.push_back(item);
            member.inventory.push_back(bullet);
        }

        if(currentCritter == 3)
        {
            Item item = *getGlobalItem("Sniper");
            Item bullet;
            bullet = *getGlobalItem("Bullet - Armor Piercing");
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
        fantasyTower.minioncount = 100;
        fantasyTower.name = "FantasyModern";
        fantasyTower.tex = &texturemanager.getTexture("FantasyModern.png");
    }
    towers.push_back(fantasyTower);
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

    addMembers(4,"The Titanium Grip");
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
            saveMap(23,0,0,50,50);
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


        bountytower::towerLoaded = towers[1].name;
        bountytower::currentTower = &towers[1];
        gCtrl.phase = "Lobby";
        //buildTower(towers[1].name);

        loadMap(towers[1].mapID,0,0,50,50);

        int xview = (96*60)/20;
        gvars::currentx = xview/2;
        gvars::currenty = xview/1.4;

        for(auto &npc : npclist)
            npc.momentum = sf::Vector2f(0,0);
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
    textList.createText(sf::Vector2f(105,115),20,sf::Color::White,LevelXpLine,window.getView());

    //Attributes! SPICED
    std::string AttributeLine;
    AttributeLine.append("S.P.I.C.E.D. \n");
    AttributeLine.append("Strength: " + std::to_string(npc->attributes.strength) + "\n");
    AttributeLine.append("Perception: " + std::to_string(npc->attributes.perception) + "\n");
    AttributeLine.append("Intelligence: " + std::to_string(npc->attributes.intelligence) + "\n");
    AttributeLine.append("Charisma: " + std::to_string(npc->attributes.charisma) + "\n");
    AttributeLine.append("Endurance: " + std::to_string(npc->attributes.endurance) + "\n");
    AttributeLine.append("Dexterity: " + std::to_string(npc->attributes.dexterity) + "\n");
    textList.createText(sf::Vector2f(105,150),20,sf::Color::White,AttributeLine,window.getView());




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
                sf::Vector2f textPos(drawPos);
                textPos.y -= 10;
                textPos.x -= 100;
                textList.createText(textPos,20,sf::Color::Cyan,skill.desc,window.getView());
            }
            else
                textList.createText(drawPos,20,sf::Color::White,outPut,window.getView());

            if(imageButtonClicked(decreaseSkillButton) && skill.ranks > 0)
            {
                skill.ranks--;
                npc->skillpoints++;
            }

            if(imageButtonClicked(increaseSkillButton) && skill.ranks < skill.ranksmax && npc->skillpoints > 0)
            {
                skill.ranks++;
                npc->skillpoints--;
            }

        }



        y++;
    }

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

        if(menu.name == "Squaddie Menu")
        {
            renderSquaddieMenu(menu);
        }
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

std::list<Npc> leftBehind;

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
    boss.health = 10000;

    boss.faction = "Towerlings";
    boss.factionPtr = &listAt(uniFact,2);

    Item weaponry = *getGlobalItem("Gun");
    Item ammo = *getGlobalItem("Bullet - Standard");
    ammo.amount = 10000;
    weaponry.internalitems.push_back(ammo);

    boss.inventory.push_back(weaponry);

    boss.skills.getSkill("Feral Body")->ranks = 2;

    npclist.push_back(boss);
}

void nextFloorTransition()
{
    for(auto &npc : npclist)
    {
        //!npc.alive
        if(gvars::currentz != abs_to_index(npc.zpos/GRID_SIZE))
        {
            leftBehind.push_back(npc);
            npc.toDelete = true;
        }
    }

    if(gvars::currentz == bountytower::currentTower->floors)
        spawnBoss();
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
    std::vector<Item> ammoV = itemmanager.getAllofType(3);
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
        member = *getGlobalCritter("BTHalfCelestial");
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

            int ranAmmo = random(0,ammoV.size()-1);
            Item ammo = ammoV[ranAmmo];
            ammo.id = gvars::globalid++;
            ammo.amount = random(5,30);
            member.inventory.push_back(ammo);
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
    textList.createText(textPos,30,sf::Color::Yellow,"$ Bounty Killed! $ \n $ Time to collect! $",window.getDefaultView());
    textPos.y += 30;
    textList.createText(textPos,30,sf::Color::White,"The people who got in your way.",window.getDefaultView());

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

    if(inputState.key[Key::F] && true == false)
    {
        for(int i = 0; i != 10; i++)
        {
            Npc corpse = *getGlobalCritter("BTHalfCelestial");
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
        bountytower::towerVictory = true;
}

void bountyTowerMainMenu()
{
    int startBut = createImageButton(sf::Vector2f(RESOLUTION.x/2,RESOLUTION.y/2),texturemanager.getTexture("Fortune Fortress Title.png"));
    textList.createText(gvars::mousePos,10,sf::Color::White,"Click to begin!");

    if(imageButtonClicked(startBut))
    {
        bountytower::towerLoaded = towers[0].name;
        bountytower::currentTower = &towers[0];
        //buildTower(towers[1].name);

        loadMap(towers[0].mapID,0,0,50,50);

        int xview = (96*60)/20;
        gvars::currentx = xview/2;
        gvars::currenty = xview/1.4;

        for(auto &npc : npclist)
            npc.momentum = sf::Vector2f(0,0);

        Npc barPatron = *getGlobalCritter("BTHuman");
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
        //gCtrl.phase = "Tower Selection";

}

void tavernButtons()
{
    int towerButt = createImageButton(sf::Vector2f(43*GRID_SIZE,67*GRID_SIZE),texturemanager.getTexture("yellowBook.png"));
    int towerButt2 = createImageButton(sf::Vector2f(53*GRID_SIZE,67*GRID_SIZE),texturemanager.getTexture("yellowBook.png"));

    if(imageButtonHovered(towerButt) || imageButtonHovered(towerButt2))
        textList.createText(gvars::mousePos,15,sf::Color::Yellow,"Click to progress to bounty menus!");

    if(imageButtonClicked(towerButt) || imageButtonClicked(towerButt2))
    {
        gCtrl.phase = "Tower Selection";
    }
}

void NPCbuttons()
{
    for(auto &npc : npclist)
    {
        if(npc.tags.find("[BountyProvider:1]") != npc.tags.npos)
        {
            int bountyButt = createImageButton(npc.getPos2d(),texturemanager.getTexture("SelectionCircle.png"));
            if(imageButtonHovered(bountyButt))
                textList.createText(gvars::mousePos,15,sf::Color::Yellow,"Ready to go bounty hunting? \n(Left Mouse Button)");
            if(imageButtonClicked(bountyButt))
                gCtrl.phase = "Tower Selection";
        }
        else if(npc.tags.find("[WeaponDealer:1]") != npc.tags.npos)
        {
            int dealerButt = createImageButton(npc.getPos2d(),texturemanager.getTexture("SelectionCircle.png"));
            if(imageButtonHovered(dealerButt))
                textList.createText(gvars::mousePos,15,sf::Color::Yellow,"Wanna see my gear? \n(Left Mouse Button) \n*Not Implimented*");
            /*if(imageButtonClicked(dealerButt))
                gCtrl.phase = "Tower Selection";
                */
        }
        else if(npc.tags.find("[Recruiter:1]") != npc.tags.npos)
        {
            int dealerButt = createImageButton(npc.getPos2d(),texturemanager.getTexture("SelectionCircle.png"));
            if(imageButtonHovered(dealerButt))
                textList.createText(gvars::mousePos,15,sf::Color::Yellow,"Looking for some fresh meat? \n(Left Mouse Button) \n*Not Implimented*");
            /*if(imageButtonClicked(dealerButt))
                gCtrl.phase = "Tower Selection";
                */
        }
    }
}

void bountyTowerLoop()
{
    hotkeySquaddieSelect();
    bossLoop();
    NPCbuttons();

    if(inputState.key[Key::LControl].time > 10)
    { // Display current mouse position.
        std::string outPut = "MousePos: " + std::to_string(gvars::mousePos.x) + "/" + std::to_string(gvars::mousePos.y) + "/" + std::to_string(gvars::currentz*GRID_SIZE);
        textList.createText(gvars::mousePos,10,sf::Color::White,outPut);
    }


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


    if(inputState.key[Key::T].time == 1)
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
        Item ammo = *getGlobalItem("Bullet - Standard");
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

    if(bountytower::pausewaves && bountytower::towerLoaded != "")
    {
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
    { // Spawn Scrap
        Item item = *getGlobalItem("Scrap");
        item.xpos = gvars::mousePos.x;
        item.ypos = gvars::mousePos.y;
        item.zpos = gvars::currentz*GRID_SIZE;
        item.amount = random(3,8);
        worlditems.push_back(item);
    }

    if(inputState.key[Key::B].time == 1)
    { // Spawn Cash
        Item item = *getGlobalItem("Cash");
        item.amount = random(10,100);
        item.xpos = gvars::mousePos.x;
        item.ypos = gvars::mousePos.y;
        item.zpos = gvars::currentz*GRID_SIZE;
        worlditems.push_back(item);

    }

    if(inputState.key[Key::N].time == 1)
    { // Spawn Ammo
        Item item = *getGlobalItem("Bullet - Standard");
        item.amount = random(5,10);
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
        textList.createText(sf::Vector2f(RESOLUTION.x/2,70),20,sf::Color::White,"On Elevator: " + std::to_string(AmountRaised),window.getDefaultView());

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
                soundmanager.playSound("ding.wav");
                nextFloorTransition();
                AnyDeletes(npclist);
                setupSquadHotKeySelection();
            } else {
                chatBox.addChat("You cannot go further up.", sf::Color::Red);
            }
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
