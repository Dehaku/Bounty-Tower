#include "BountyTower.h"

#include "RichText.hpp"

#include <iomanip>
#include <cmath>

/*

static int tempOffset = 0;
        if(inputState.key[Key::Right].time == 1)
            tempOffset++;
        if(inputState.key[Key::Left].time == 1)
            tempOffset--;
        if(inputState.key[Key::Up].time == 1)
            std::cout << "temptOffset: " << tempOffset << std::endl;



        static int tempOffset = 0;
        static int tempOffset2 = 0;
        if(inputState.key[Key::Right].time == 1 || inputState.key[Key::Right].time > 30)
            tempOffset++;
        if(inputState.key[Key::Left].time == 1 || inputState.key[Key::Left].time > 30)
            tempOffset--;
        if(inputState.key[Key::Down].time == 1 || inputState.key[Key::Down].time > 30)
            tempOffset2++;
        if(inputState.key[Key::Up].time == 1 || inputState.key[Key::Up].time > 30)
            tempOffset2--;
        if(inputState.key[Key::LShift].time == 1)
            std::cout << "temptOffset: " << tempOffset << std::endl << "temptOffset2: " << tempOffset2 << std::endl;

*/

std::vector<Npc*> Squaddies;
std::list<Npc> leftBehind;
std::list<Npc> recruitables;
std::list<Item> enchantedItems;
std::list<Item> itemStorage;
std::list<Item> gunModMerchantInventory;

std::string itemDescription(Item item)
{
    std::string returns = "";
    returns.append(item.name + "\n");
    returns.append("Damage: " + str(item.getMaxDamage()) + "\n");
    returns.append("Ammo Capacity: " + str(item.getAmmoCapacity()) + "\n");


    return returns;
}

bool onScreen(sf::Vector2f vPos)
{
    int lowcapX = (gvars::view1.getCenter().x - (gvars::view1.getSize().x/2));
    int lowcapY = (gvars::view1.getCenter().y - (gvars::view1.getSize().y/2));
    int highcapX = ((gvars::view1.getCenter().x + (gvars::view1.getSize().x/2)));
    int highcapY = ((gvars::view1.getCenter().y + (gvars::view1.getSize().y/2)));
    // TODO: Set these variables only once per frame.


    if(aabb(vPos,lowcapX,highcapX,lowcapY,highcapY))
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

void setupItemStorage()
{

    /*
    for(auto gItem: itemmanager.globalItems)
        if(gItem.cbaseid < 10000)
            itemStorage.push_back(gItem);
    */

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

            item = *getGlobalItem("Shell Spread");
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
            bullet = *getGlobalItem("Bullet Standard");
            bullet.amount = 30;

            member.inventory.push_back(item);
            member.inventory.push_back(bullet);
        }

        if(currentCritter == 3)
        {
            Item item = *getGlobalItem("Sniper");
            Item bullet;
            bullet = *getGlobalItem("Bullet Armor Piercing");
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

    //Build the necromancer tower.
    Tower necroTower;
    {
        necroTower.mapID = 637;
        necroTower.bountyPay = 5000;
        necroTower.bountyTarget = "Necromancer, Dabbles in Swordsmanship";
        necroTower.floors = 5;
        necroTower.difficulty = 10;
        necroTower.minioncount = 400;
        necroTower.name = "Necromancer Tower";
        //fantasyTower.tex = &texturemanager.getTexture("FantasyModern.png");
        necroTower.tex = &texturemanager.getTexture("Scroll.png");
    }
    towers.push_back(necroTower);

    Tower powerTower;
    {
        powerTower.mapID = 638;
        powerTower.bountyPay = 5000;
        powerTower.bountyTarget = "Cyborg Psuedo Diety, Mega Dron";
        powerTower.floors = 5;
        powerTower.difficulty = 20;
        powerTower.minioncount = 600;
        powerTower.name = "Powerhouse Tower";
        //fantasyTower.tex = &texturemanager.getTexture("FantasyModern.png");
        powerTower.tex = &texturemanager.getTexture("Scroll.png");
    }
    towers.push_back(powerTower);
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

    setupItemStorage();

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
    for(auto &item : worlditems)
    {
        if(tiles[abs_to_index(item.xpos/GRID_SIZE)][abs_to_index(item.ypos/GRID_SIZE)][gvars::currentz].id == 3202)
        {
            item.zpos += GRID_SIZE;
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

void generateHiddenGoodies()
{
    for(int x = 0; x != GRIDS; x++)
        for(int y = 0; y != GRIDS; y++)
            if(tiles[x][y][gvars::currentz].id == 3750)
                if(random(1,2) == 1)
    {
        //tiles[x][y][gvars::currentz].BTstone();
        int xPos = (x * GRID_SIZE) + GRID_SIZE/2;
        int yPos = (y * GRID_SIZE) + GRID_SIZE/2;
        int zPos = gvars::currentz * GRID_SIZE;

        int difficulty = bountytower::currentTower->difficulty;


        RandomWeightList goodieType;
        goodieType.addEntry("Cash",1000);
        goodieType.addEntry("Equipment",1000);
        goodieType.addEntry("Orb",100);
        goodieType.addEntry("Event",10);

        std::string goodie = goodieType.getRandomName();

        if(goodie == "Cash")
        {
            int cashAmount = random(0,5);
            for(int i = 0; i != cashAmount; i++)
            {
                Item cash = *getGlobalItem("Cash");
                cash.xpos = xPos+random(0,30);
                cash.ypos = yPos+random(0,30);
                cash.zpos = zPos;

                cash.amount = random(10*difficulty,100*difficulty);
                worlditems.push_back(cash);
            }
        }
        else if(goodie == "Equipment")
        {

            RandomWeightList equipmentSet;
            equipmentSet.addEntry("Melee",100);
            equipmentSet.addEntry("Ranged",50);
            equipmentSet.addEntry("Charm",10);

            Item lootItem = generateRandomItem(equipmentSet);

            RandomWeightList rankList;
            {
                rankList.addEntry("Alpha",0);
                rankList.addEntry("Beta",100000);
                rankList.addEntry("Gamma",10000);
                rankList.addEntry("Delta",1000);
                rankList.addEntry("Epsilon",100);
                rankList.addEntry("Zeta",10);
                rankList.addEntry("Eta",1);
            }

            StatusEffect status = generateRandomStatusEffect(rankList);
            status.duration = 1;
            lootItem.statusEffects.push_back(status);
            status = generateRandomStatusEffect(rankList);
            status.duration = 180;
            lootItem.statusEffectsInflict.push_back(status);

            if(lootItem.name == "Charm")
            {
                lootItem.statusEffects.clear();
                lootItem.statusEffectsInflict.clear();

                status = generateRandomStatusEffect(rankList);
                StatusEffect charmStatus;

                int randomSlot = rankList.getRandomSlot();
                con("Rolled: " + str(randomSlot) );
                con("Slots: " + std::to_string(rankList.entries.size()));
                charmStatus.rank = rankList.entries[randomSlot].name;
                charmStatus.aspects.push_back(generateRandomStatusAspectConstant(randomSlot));

                charmStatus.name = charmStatus.rank + " Chaos " + generateName(2,3);
                charmStatus.duration = 1;

                status.duration = 1;
                lootItem.statusEffectsCarried.push_back(charmStatus);
            }

            lootItem.xpos = xPos+random(0,30);
            lootItem.ypos = yPos+random(0,30);
            lootItem.zpos = zPos;

            worlditems.push_back(lootItem);
        }
        else if(goodie == "Orb")
        {
            RandomWeightList orbType;
            orbType.addEntry("Level",1);
            orbType.addEntry("Quarter",10);
            orbType.addEntry("Bank",100);
            std::string orbby = orbType.getRandomName();

            if(orbby == "Level")
            {
                Item genOrb = *getGlobalItem("Level Egg");
                genOrb.xpos = xPos+random(0,30);
                genOrb.ypos = yPos+random(0,30);
                genOrb.zpos = zPos;
                worlditems.push_back(genOrb);
            }

            if(orbby == "Quarter")
            {
                Item genOrb = *getGlobalItem("Quarter Level Egg");
                genOrb.xpos = xPos+random(0,30);
                genOrb.ypos = yPos+random(0,30);
                genOrb.zpos = zPos;
                worlditems.push_back(genOrb);
            }

            if(orbby == "Bank")
            {
                Item genOrb = *getGlobalItem("Experience Essence Egg");
                genOrb.xpos = xPos+random(0,30);
                genOrb.ypos = yPos+random(0,30);
                genOrb.zpos = zPos;
                worlditems.push_back(genOrb);
            }


        }
        else if(goodie == "Event")
        {
            Npc spawn;
            spawn = *getGlobalCritter("Infider");
            spawn.xpos = xPos;
            spawn.ypos = yPos;
            spawn.zpos = zPos;

            spawn.faction = "Wild";
            for(auto &faction : uniFact)
                if(faction.name == spawn.faction)
                    spawn.factionPtr = &faction;
            Item naturalWeapon = *getGlobalItem("Baton");
            for(auto &globalStatus : globalStatusEffects.statusEffects)
                if(spawn.tags.find(globalStatus.name) != std::string::npos)
                    naturalWeapon.statusEffectsInflict.push_back(globalStatusEffects.getStatusEffect(globalStatus.name));
            spawn.inventory.push_back(naturalWeapon);

            npclist.push_back(spawn);
        }

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

    static int currentMap = 0;
    if(currentMap < 0)
        currentMap = 0;
    if(currentMap > 15)
        currentMap = 15;

    if(inputState.key[Key::LShift] && inputState.key[Key::LControl] && inputState.key[Key::Right].time == 1)
        currentMap++;
    if(inputState.key[Key::LShift] && inputState.key[Key::LControl] && inputState.key[Key::Left].time == 1)
        currentMap--;
    std::cout << "Current Map: " << currentMap+636 << std::endl;



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
            saveMap(636+currentMap,0,0,50,50);
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
            loadMap(636+currentMap,0,0,50,50);
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

    int wallButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30,menuEdgeU+30),texturemanager.getTexture("FantasyModernWall.png"));
    if(shapes.shapeClicked(wallButt))
        debugTileKeeper.useTile.BTwall();

    int floorButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+90,menuEdgeU+30),texturemanager.getTexture("FantasyModernFloor1.png"));
    if(shapes.shapeClicked(floorButt))
        debugTileKeeper.useTile.BTstone();

    int fakewallButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30+60+60,menuEdgeU+30),texturemanager.getTexture("FantasyModernWallBreakable.png"));
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

    int stairButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30+60,menuEdgeU+30+60+60),texturemanager.getTexture("CautionTile.png"));
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

    int goodietileButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30+60+60+60,menuEdgeU+30+60+60+60+60),texturemanager.getTexture("BTGoodieTile.png"));
    if(shapes.shapeClicked(goodietileButt))
        debugTileKeeper.useTile.BTgoodietile();

    int airdroptileButt = shapes.createImageButton(sf::Vector2f(menuEdgeL+30+60+60+60,menuEdgeU+30+60+60+60+60+60),texturemanager.getTexture("BTAirdropTile.png"));
    if(shapes.shapeClicked(airdroptileButt))
        debugTileKeeper.useTile.BTairdroptile();

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
        npc.dragging = nullptr;
        npc.draggedBy = nullptr;
        npc.chaseDefendPos = Vec3(npc.xpos,npc.ypos,npc.zpos);

        if(!npc.isSquaddie)
            npc.toDelete = true;
        if(!npc.alive)//if(!npc.functional())
            npc.toDelete = true;

    }

    for(int i = 0; i != 30; i++)
    {
        shapes.createSquare(0,0,1400,1400,sf::Color(0,0,0,25),0,sf::Color::Transparent,&gvars::hudView);
        shapes.shapes.back().duration = 60-i;
        shapes.shapes.back().layer = 10000000;

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

void randomizeGunModderInventory()
{
    gunModMerchantInventory.clear();
    //gunModMerchantInventory.push_back(itemmanager.globalItems.at(random(0,itemmanager.globalItems.size()-1)));


    int genAmount = random(4,10);
    for(int i = 0; i != genAmount; i++)
    {
        Item weaponMod;
        { // Hack
            weaponMod = *getGlobalItem("Simple Grip");
            weaponMod.recoilReduction = 0;
        }



        RandomWeightList modType;
        modType.addEntry("PartBody",100);
        modType.addEntry("PartBarrel",100);
        modType.addEntry("PartMagazine",100);
        modType.addEntry("PartGrip",100);
        modType.addEntry("PartStock",100);
        modType.addEntry("PartSight",100);
        modType.addEntry("PartAccessory",100);

        std::string gennedType = modType.getRandomName();
        weaponMod.type = itemTypes.getTypeID(gennedType).num;
        std::cout << "Genned: " << gennedType << ": " << weaponMod.type << std::endl;

        weaponMod.name = gennedType;
        weaponMod.value = random(100,10000);

        if(gennedType == "PartBody")
        {
            // Type 1, Tactical Body, Small Burst, Tiny Fire Delay, No Auto, Low recoil
            // Type 2, Automatic Body, Large Burst, Small Fire Delay, Automatic, Medium Damage, Decent Recoil.
            // Type 3, Sturdy Body, Semi Only, Large Durability
            // Type 4, Assault Body, All Fire Modes, Medium Fire Delay, Medium Damage, Low Durability.


            int ranType = random(1,3);
            if(ranType == 1)
            { // Tactical
                weaponMod.aimTime = 60;
                weaponMod.durability = 10000;
                weaponMod.durabilityCost = random(10,20);
                weaponMod.fireDelay = (3,7);
                weaponMod.recoil = random(3,7);

                weaponMod.canFireSemi = true;
                weaponMod.canFireBurst = true;

                weaponMod.burstCount = random(2,5);

                weaponMod.damageMultiplier = random(10,30)*0.1;

            }
            if(ranType == 2)
            { // Automatic
                weaponMod.aimTime = 60;
                weaponMod.durability = 10000;
                weaponMod.durabilityCost = random(15,30);
                weaponMod.fireDelay = (3,7);
                weaponMod.recoil = random(5,10);

                weaponMod.canFireSemi = true;
                //weaponMod.canFireBurst = true;
                weaponMod.canFireAuto = true;

                //weaponMod.burstCount = random(3,6);

                weaponMod.damageMultiplier = random(10,20)*0.1;

            }
            if(ranType == 3)
            { // Sturdy
                weaponMod.aimTime = 60;
                weaponMod.durability = random(2,5) * 5000;
                weaponMod.durabilityCost = random(5,10);
                weaponMod.fireDelay = (15,25);
                weaponMod.recoil = random(5,15);

                weaponMod.canFireSemi = true;

                weaponMod.damageMultiplier = random(10,20)*0.1;

            }

        }

        else if(gennedType == "PartBarrel")
        {

            // Type 1, High Recoil, High Damage
            // Type 2, Multiple Barrels, Low Damage.
            // Type 3, High Durability Cost, Very High Damage, Slow Bullet.
            // All variables should be caluclated BEFORE 'Final' Damage, for Damage will scale.


            int ranType = random(1,3);
            if(ranType == 1)
            {
                weaponMod.barrelCount = 1;
                weaponMod.damageMultiplier = random(10,50)*0.1;
                weaponMod.recoil = random(10,30);
                weaponMod.dispersion = random(1,5);
                weaponMod.durabilityCost = weaponMod.damageMultiplier * random(1,5);
                weaponMod.bulletSpeedMultiplier = random(1,3);
            }
            if(ranType == 2)
            {
                RandomWeightList barrelCount;
                {
                    barrelCount.addEntry("2",1000);
                    barrelCount.addEntry("3",100);
                    barrelCount.addEntry("4",50);
                    barrelCount.addEntry("5",25);
                    barrelCount.addEntry("6",10);
                    barrelCount.addEntry("20",1);
                }


                weaponMod.barrelCount = std::stoi(barrelCount.getRandomName());
                weaponMod.damageMultiplier = random(2,10)*0.1;
                weaponMod.recoil = random(10,30);
                weaponMod.dispersion = weaponMod.barrelCount*5;
                weaponMod.durabilityCost = weaponMod.barrelCount * random(1,5);
                weaponMod.bulletSpeedMultiplier = random(1,3);
            }
            if(ranType == 3)
            {
                weaponMod.barrelCount = 1;
                weaponMod.damageMultiplier = random(50,200)*0.1;
                weaponMod.recoil = random(10,30);
                weaponMod.dispersion = random(1,3);
                weaponMod.durabilityCost = weaponMod.damageMultiplier * random(20,50);
                weaponMod.bulletSpeedMultiplier = (random(3,6)*0.1);
            }




        }

        else if(gennedType == "PartMagazine")
        {
            RandomWeightList partQuality;
            partQuality.addEntry("Magazine", 1000);
            partQuality.addEntry("Rotary Magazine", 250);
            partQuality.addEntry("PA Loader", 250);
            partQuality.addEntry("Breech Loader", 250);

            std::string partRoll = partQuality.getRandomName();

            if(partRoll == "Magazine")
            {
                weaponMod.ammoCapacity = random(1,4)*5;
                weaponMod.reloadTime = random(3,9)*30;
                weaponMod.reloadAmount = weaponMod.ammoCapacity;
                weaponMod.recoil = random(1,2)*5;

                weaponMod.value = random(1,4)*50;
                weaponMod.name = "Simple Mag";
            }

            else if(partRoll == "Rotary Magazine")
            {
                weaponMod.ammoCapacity = random(1,4)*25;
                weaponMod.reloadTime = random(7,14)*30;
                weaponMod.reloadAmount = weaponMod.ammoCapacity;
                weaponMod.recoil = random(1,4)*5;
                weaponMod.durabilityCost = random(5,10)*2;

                weaponMod.value = random(1,6)*100;
                weaponMod.name = "Rotary Mag";
            }
            else if(partRoll == "PA Loader")
            {
                weaponMod.ammoCapacity = random(1,6)*2;
                weaponMod.reloadTime = random(1,4)*15;
                weaponMod.reloadAmount = random(1,2);
                weaponMod.durabilityCost = random(5,10)*2;

                weaponMod.value = random(1,6)*100;
                weaponMod.name = "Pump Loader";
            }
            else if(partRoll == "Breech Loader")
            {
                weaponMod.ammoCapacity = 1;
                weaponMod.reloadTime = 30+(random(1,4)*15);
                weaponMod.reloadAmount = 1;
                weaponMod.durability = random(1,8)*2500;

                weaponMod.value = random(1,6)*1000;
                weaponMod.name = "Breech Loader";
            }
        }




        else if(gennedType == "PartGrip")
        {
            RandomWeightList gripQuality;
            gripQuality.addEntry("Poor", 1000);
            gripQuality.addEntry("Decent", 1000);
            gripQuality.addEntry("Good", 500);
            gripQuality.addEntry("Great", 100);
            gripQuality.addEntry("Legendary", 10);

            std::string gripRoll = gripQuality.getRandomName();

            if(gripRoll == "Poor")
            {
                weaponMod.recoilReduction = random(1,5);
                weaponMod.value = 100*1;
                weaponMod.name = weaponMod.name + " " + gripRoll;
            }

            else if(gripRoll == "Decent")
            {
                weaponMod.recoilReduction = random(5,10);
                weaponMod.value = 100*3;
                weaponMod.name = weaponMod.name + " " + gripRoll;
            }
            else if(gripRoll == "Good")
            {
                weaponMod.recoilReduction = random(10,15);
                weaponMod.value = 100*5;
                weaponMod.name = weaponMod.name + " " + gripRoll;
            }
            else if(gripRoll == "Great")
            {
                weaponMod.recoilReduction = random(15,20);
                weaponMod.value = 100*10;
                weaponMod.name = weaponMod.name + " " + gripRoll;
            }
            else if(gripRoll == "Legendary")
            {
                weaponMod.recoilReduction = random(30,50);
                weaponMod.value = 100*50;
                weaponMod.name = weaponMod.name + " " + gripRoll;
            }
        }

        else if(gennedType == "PartStock")
        {
            RandomWeightList partQuality;
            partQuality.addEntry("Poor", 1000);
            partQuality.addEntry("Decent", 1000);
            partQuality.addEntry("Good", 500);
            partQuality.addEntry("Great", 100);
            partQuality.addEntry("Legendary", 10);

            std::string partRoll = partQuality.getRandomName();

            if(partRoll == "Poor")
            {
                weaponMod.recoilReduction = random(1,5);
                weaponMod.recoil = -random(1,5);
                weaponMod.value = 100*1;
                weaponMod.name = weaponMod.name + " " + partRoll;
            }

            else if(partRoll == "Decent")
            {
                weaponMod.recoilReduction = random(5,10);
                weaponMod.recoil = -random(5,10);
                weaponMod.value = 100*3;
                weaponMod.name = weaponMod.name + " " + partRoll;
            }
            else if(partRoll == "Good")
            {
                weaponMod.recoilReduction = random(10,15);
                weaponMod.recoil = -random(10,15);
                weaponMod.value = 100*5;
                weaponMod.name = weaponMod.name + " " + partRoll;
            }
            else if(partRoll == "Great")
            {
                weaponMod.recoilReduction = random(15,20);
                weaponMod.recoil = -random(15,20);
                weaponMod.value = 100*10;
                weaponMod.name = weaponMod.name + " " + partRoll;
            }
            else if(partRoll == "Legendary")
            {
                weaponMod.recoilReduction = random(30,50);
                weaponMod.recoil = -random(30,50);
                weaponMod.value = 100*50;
                weaponMod.name = weaponMod.name + " " + partRoll;
            }
        }

        else if(gennedType == "PartSight")
        {
            RandomWeightList partQuality;
            partQuality.addEntry("Iron Sight", 1000);
            partQuality.addEntry("Holo Sight", 250);
            partQuality.addEntry("Scope", 250);

            std::string partRoll = partQuality.getRandomName();

            if(partRoll == "Iron Sight")
            {
                weaponMod.aimTime = -random(5,15);
                weaponMod.value = 100*1;
                weaponMod.name = weaponMod.name + " Iron";
            }

            else if(partRoll == "Holo Sight")
            {
                weaponMod.aimTime = -random(10,20);
                weaponMod.durabilityCost = random(5,10);
                weaponMod.value = 100*3;
                weaponMod.name = weaponMod.name + " Holo";
            }
            else if(partRoll == "Scope")
            {
                weaponMod.aimTime = random(90,360);
                weaponMod.durabilityCost = random(20,40);
                weaponMod.damageMultiplier = random(10,60)*0.1;
                weaponMod.value = 100*5;
                weaponMod.name = std::to_string(random(2,10)) + "x " + partRoll;
            }
        }

        else if (gennedType == "PartAccessory")
        {
            RandomWeightList partQuality;
            partQuality.addEntry("Laser Attachment", 1000);
            partQuality.addEntry("Gyrostablizer", 250);
            partQuality.addEntry("Barrel Rifling", 250);

            std::string partRoll = partQuality.getRandomName();

            if(partRoll == "Laser Attachment")
            {
                weaponMod.aimTime = -random(5,15);
                weaponMod.dispersion = random(1,3);
                weaponMod.value = 100*1;
                weaponMod.name = partRoll;
            }

            if(partRoll == "Gyrostablizer")
            {
                weaponMod.recoil = -random(5,15);
                weaponMod.value = 100*1;
                weaponMod.name = partRoll;
            }

            if(partRoll == "Barrel Rifling")
            {
                float randomRoll = random(10,30);
                weaponMod.bulletSpeedMultiplier = randomRoll*0.1;
                weaponMod.damageMultiplier = randomRoll*0.1;
                weaponMod.durabilityCost = random(1,30);
                weaponMod.value = 100*1;
                weaponMod.name = partRoll;
            }
        }



        gunModMerchantInventory.push_back(weaponMod);

    }


}

void merchantModderMenu(Vec3f creationPos)
{
    for(auto menu : menus)
        if(menu.name == "Merchant Modder Menu")
            return;
    baseMenu sMenu;
    sMenu.name = "Merchant Modder Menu";
    sMenu.Pos = sf::Vector2f(screen.x()/2,screen.y()/2);
    sMenu.makePos = creationPos;

    randomizeGunModderInventory();

    menus.push_back(sMenu);
}

void storageMenu(Vec3f creationPos)
{
    for(auto menu : menus)
        if(menu.name == "Storage Menu")
            return;
    baseMenu sMenu;
    sMenu.name = "Storage Menu";
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

StatusAspect generateRandomStatusAspectConditionOnce(int rankNum)
{
    int multipliers[10] = {1,2,4,8,12,16,20,24,28,32};
    RandomWeightList damageList;
    {
        for(auto damString : damageTypes.TypeStrings)
            if(damString != "None")
                damageList.addEntry(damString,10000);
    }


    StatusAspect aspect;
    RandomWeightList aspectList;
    { // Get Appropriote and Implemented aspects.
        std::string aspectName;

        aspectName = "ConditionOnItemUse";
        aspectList.addEntry(aspectName,10000);
        aspectName = "ConditionOnDeath";
        aspectList.addEntry(aspectName,10000);
        aspectName = "ConditionOnHit";
        aspectList.addEntry(aspectName,10000);
        aspectName = "ConditionOnHitByType";
        aspectList.addEntry(aspectName,10000);

    }
    std::string aspectString = aspectList.getRandomName();
    std::cout << "Building Aspect: " << aspectString;

    aspect.potency = 0;
    aspect.name = getAspectNum(aspectString);
    int minRandom = 1;
    int maxRandom = 5;
    bool ableFlipped = true;



    if(aspectString == "ConditionOnItemUse")
    {
        aspect.potency = 1;
        RandomWeightList itemTypes;
        itemTypes.addEntry(str(1),10);
        itemTypes.addEntry(str(2),10);
        itemTypes.addEntry(str(23),10);
        itemTypes.addEntry(str(42),10);
        itemTypes.addEntry(str(69),10);
        aspect.type = itemTypes.getRandomName();
    }
    if(aspectString == "ConditionOnDeath")
        aspect.potency = 1;
    if(aspectString == "ConditionOnHit")
        aspect.potency = 1;
    if(aspectString == "ConditionOnHitByType")
    {
        aspect.potency = 1;
        aspect.type = damageList.getRandomName();
    }

    return aspect;
}

StatusAspect generateRandomStatusAspectConditionConstant(int rankNum)
{
    int multipliers[10] = {1,2,4,8,12,16,20,24,28,32};
    RandomWeightList damageList;
    {
        for(auto damString : damageTypes.TypeStrings)
            if(damString != "None")
                damageList.addEntry(damString,10000);
    }


    StatusAspect aspect;
    RandomWeightList aspectList;
    { // Get Appropriote and Implemented aspects.
        std::string aspectName;

        aspectName = "ConditionHealth";
        aspectList.addEntry(aspectName,10000);
        aspectName = "ConditionLife";
        aspectList.addEntry(aspectName,10000);
        aspectName = "ConditionTimeDelay";
        aspectList.addEntry(aspectName,10000);

    }
    std::string aspectString = aspectList.getRandomName();
    std::cout << "Building Aspect: " << aspectString;

    aspect.potency = 0;
    aspect.name = getAspectNum(aspectString);
    int minRandom = 1;
    int maxRandom = 5;
    bool ableFlipped = true;


    if(aspectString == "ConditionHealth")
    {
        if(random(0,1) == 0)
        {
            aspect.type = "Below";
            for(int i = 0; i != multipliers[rankNum]; i++)
                aspect.potency += 10;
            if(aspect.potency > 100)
                aspect.potency = 100;
        }
        else
        {
            aspect.type = "Above";
            aspect.potency = 100;
            for(int i = 0; i != multipliers[rankNum]; i++)
                aspect.potency -= 10;
            if(aspect.potency < 0)
                aspect.potency = 0;
        }
    }

    if(aspectString == "ConditionLife")
    {
        if(random(0,1) == 0)
            aspect.type = "Dead";
        else
            aspect.type = "Alive";
        aspect.potency = 1;
    }

    if(aspectString == "ConditionTimeDelay")
        aspect.potency = random(60,180);

    return aspect;
}

StatusAspect generateRandomStatusAspectConstant(int rankNum)
{
    int multipliers[10] = {1,2,4,8,12,16,20,24,28,32};
    RandomWeightList damageList;
    {
        for(auto damString : damageTypes.TypeStrings)
            if(damString != "None")
                damageList.addEntry(damString,10000);
    }


    StatusAspect aspect;
    RandomWeightList aspectList;
    { // Get Appropriote and Implemented aspects.
        std::string aspectName;

        aspectName = "AffectDamage";
        aspectList.addEntry(aspectName,10000);
        aspectName = "AffectHealth"; // Small amount.
        aspectList.addEntry(aspectName,10000);
        aspectName = "Armor";
        aspectList.addEntry(aspectName,10000);
        aspectName = "Freeze";
        aspectList.addEntry(aspectName,10000);
        aspectName = "MoveSpeed";
        aspectList.addEntry(aspectName,10000);
        aspectName = "Sleep";
        aspectList.addEntry(aspectName,10000);
        aspectName = "Stun";
        aspectList.addEntry(aspectName,10000);
        aspectName = "Attribute";
        aspectList.addEntry(aspectName,10000);
        aspectName = "Thorns";
        aspectList.addEntry(aspectName,10000);
        aspectName = "Immunity";
        aspectList.addEntry(aspectName,10000);
        //aspectName = "StatusEffectImmunity";
        //aspectList.addEntry(aspectName,10000);
        aspectName = "ItemFind";
        aspectList.addEntry(aspectName,10000);
        aspectName = "ItemDropRate";
        aspectList.addEntry(aspectName,10000);
        aspectName = "XPModifier";
        aspectList.addEntry(aspectName,10000);
        aspectName = "DeathXPModifier";
        aspectList.addEntry(aspectName,10000);
        aspectName = "AutoDodge";
        aspectList.addEntry(aspectName,10000);
        aspectName = "ChangeRace";
        aspectList.addEntry(aspectName,10000);
    }
    std::string aspectString = aspectList.getRandomName();
    std::cout << "Building Aspect: " << aspectString;

    aspect.potency = 0;
    aspect.name = getAspectNum(aspectString);
    int minRandom = 1;
    int maxRandom = 5;
    bool ableFlipped = true;


    if(aspectString == "AffectDamage")
    {
        minRandom = 5;
        maxRandom = 20;
        aspect.type = damageList.getRandomName();
    }
    if(aspectString == "Armor")
    {
        minRandom = 5;
        maxRandom = 20;
        aspect.type = damageList.getRandomName();
    }
    if(aspectString == "Freeze")
    {
        minRandom = 100;
        maxRandom = 500;
        ableFlipped = false;
    }
    if(aspectString == "Sleep")
    {
        minRandom = 100;
        maxRandom = 500;
        ableFlipped = false;
    }
    if(aspectString == "Stun")
    {
        minRandom = 1;
        maxRandom = 1;
        ableFlipped = false;
    }
    if(aspectString == "Thorns")
    {
        minRandom = 10;
        maxRandom = 50;
        ableFlipped = false;
        aspect.type = damageList.getRandomName();
    }
    if(aspectString == "Immunity")
    {
        minRandom = 1;
        maxRandom = 1;
        ableFlipped = false;
        aspect.type = damageList.getRandomName();
    }
    if(aspectString == "AutoDodge")
    {
        minRandom = 1;
        maxRandom = 1;
        ableFlipped = false;
        aspect.type = damageList.getRandomName();
    }
    if(aspectString == "ChangeRace")
    {
        minRandom = 100;
        maxRandom = 500;
        ableFlipped = false;
        RandomWeightList raceList;
        {
            for(auto npc : npcmanager.globalCritter)
                if(npc.race != "Turret")
                    raceList.addEntry(npc.race,10000);
        }
        aspect.type = raceList.getRandomName();
    }
    if(aspectString == "Attribute")
    {
        RandomWeightList attributeList;
        {
            attributeList.addEntry("Strength",10000);
            attributeList.addEntry("Perception",10000);
            attributeList.addEntry("Intelligence",10000);
            attributeList.addEntry("Charisma",10000);
            attributeList.addEntry("Endurance",10000);
            attributeList.addEntry("Dexterity",10000);
        }
        aspect.type = attributeList.getRandomName();
    }



    for(int i = 0; i != multipliers[rankNum]; i++)
    {
        int randomPotency = random(minRandom,maxRandom);
        std::cout << "Adding Potency: " << randomPotency << std::endl;
        aspect.potency += randomPotency;
    }
    if(ableFlipped)
    {
        if(random(0,1) == 0)
            aspect.potency = -aspect.potency;
    }

    if(aspectString == "AffectHealth")
    {
        if(aspect.potency > 0)
            aspect.type = "Energy";
        else
            aspect.type = damageList.getRandomName();
    }



    return aspect;
}

StatusAspect generateRandomStatusAspectOnce(int rankNum)
{
    int multipliers[10] = {1,2,4,8,12,16,20,24,28,32};
    RandomWeightList damageList;
    {
        for(auto damString : damageTypes.TypeStrings)
            if(damString != "None")
                damageList.addEntry(damString,10000);
    }


    StatusAspect aspect;
    RandomWeightList aspectList;
    {
        std::string aspectName;


        aspectName = "AffectHealth"; // Massive amount.
        aspectList.addEntry(aspectName,10000);
        aspectName = "CauseExplosion";
        aspectList.addEntry(aspectName,10000);
        aspectName = "Revive";
        aspectList.addEntry(aspectName,10000);
        //aspectName = "SpawnCreature";
        //aspectList.addEntry(aspectName,10000);
        aspectName = "SpawnItem";
        aspectList.addEntry(aspectName,10000);
    }


    std::string aspectString = aspectList.getRandomName();
    std::cout << "Building Aspect: " << aspectString;

    aspect.potency = 0;
    aspect.name = getAspectNum(aspectString);
    int minRandom = 1;
    int maxRandom = 5;
    bool ableFlipped = true;




    if(aspectString == "AffectHealth")
    {
        minRandom = 100;
        maxRandom = 500;
        aspect.type = damageList.getRandomName();
    }
    if(aspectString == "SpawnCreature")
    {
        minRandom = 1;
        maxRandom = 2;
        ableFlipped = false;
        RandomWeightList raceList;
        {
            for(auto npc : npcmanager.globalCritter)
                if(npc.race != "Turret")
                    raceList.addEntry(npc.race,10000);
        }
        aspect.type = raceList.getRandomName();
    }
    if(aspectString == "SpawnItem")
    {
        minRandom = 1;
        maxRandom = 2;
        ableFlipped = false;
        RandomWeightList itemList;
        {
            for(auto item : itemmanager.globalItems)
                itemList.addEntry(item.name,10000);
        }
        aspect.type = itemList.getRandomName();
    }
    if(aspectString == "Revive")
    {
        minRandom = 1;
        maxRandom = 1;
        ableFlipped = false;
    }




    for(int i = 0; i != multipliers[rankNum]; i++)
    {
        int randomPotency = random(minRandom,maxRandom);
        std::cout << "Adding Potency: " << randomPotency << std::endl;
        aspect.potency += randomPotency;
    }
    if(ableFlipped)
    {
        if(random(0,1) == 0)
            aspect.potency = -aspect.potency;
    }

    if(aspectString == "AffectHealth")
    {
        if(aspect.potency > 0)
            aspect.type = "Energy";
    }





    return aspect;

}


StatusEffect generateRandomStatusEffectOld(RandomWeightList rankList)
{
/*
Weapon Status Effect Generation
Order of Logic
Common, Uncommon, Rare, Legendary weighted list. This will govern the potency of Aspects. (Have an option to pay coin to force a generation of one of these)
1-10 Status Effects, on a Weighted List.
Each Status Effect will be...
1-2 Aspects
1 Conditional (If a ConditionHealth, Have a Weighted List to determin whether to have multiple conditions to keep amping up further aspects)
1-2 Aspects.
1 Conditional
1 Potent Aspect(Since it's quite unlikely for two random conditionals to ever be met, this could be wonderful.)
*/
    StatusEffect status;

    //RandomWeightList rankList;
    if(rankList.entries.empty())
    { // Fill Rank List
        rankList.addEntry("Alpha",1000000);
        rankList.addEntry("Beta",100000);
        rankList.addEntry("Gamma",10000);
        rankList.addEntry("Delta",1000);
        rankList.addEntry("Epsilon",100);
        rankList.addEntry("Zeta",10);
        rankList.addEntry("Eta",1);
    }
    RandomWeightList aspectList;
    { // Get Appropriote and Implemented aspects.
        for(auto &aspect : aspectNum)
        {
            if(aspect.find("Condition") != std::string::npos)
                continue;
            else if(aspect.find("Mana") != std::string::npos)
                continue;
            else if(aspect == "ConditionNearbyUnit")
                continue;
            else if(aspect == "ConditionUnitCount")
                continue;
            else if(aspect == "ConditionReceivedDamage")
                continue;
            else if(aspect == "ActionSpeed")
                continue;
            else if(aspect == "AmmoCost")
                continue;
            else if(aspect == "Accuracy")
                continue;
            else if(aspect == "StatusEffectImmunity")
                continue;
            else if(aspect == "ApplyMomentum")
                continue;
            else if(aspect == "MomentumSensitivity")
                continue;
            else if(aspect == "AffectVisionRange")
                continue;
            else if(aspect == "Mark")
                continue;
            else if(aspect == "DisableDeath")
                continue;

            aspectList.addEntry(aspect,10000);

        }
    }
    aspectList.printEntries();
    RandomWeightList conditionList;
    { // Get implemented conditions.
        for(auto &aspect : aspectNum)
        {
            if( !(aspect.find("Condition") != std::string::npos))
                continue;
            else if(aspect.find("Mana") != std::string::npos)
                continue;
            else if(aspect == "ConditionNearbyUnit")
                continue;
            else if(aspect == "ConditionUnitCount")
                continue;
            else if(aspect == "ConditionReceivedDamage")
                continue;

            conditionList.addEntry(aspect,10000);

        }
    }
    conditionList.printEntries();
    RandomWeightList damageList;
    {
        for(auto damString : damageTypes.TypeStrings)
            if(damString != "None")
                damageList.addEntry(damString,10000);
    }
    damageList.printEntries();

    int rankNum = rankList.getRandomSlot();
    status.rank = rankList.entries[rankNum].name;
    std::cout << "Generated Rank: " << status.rank << std::endl;

    std::vector<std::string> aspects;
    { // We generate the aspects and condition's to be put into the status effect...
        int firstSet = random(1,2);
        for(int i = 0; i != firstSet; i++)
        {
            aspects.push_back(aspectList.getRandomName());
            std::cout << "Aspect: " << aspects.back() << std::endl;
        }
        aspects.push_back(conditionList.getRandomName());
        std::cout << "Condition: " << aspects.back() << std::endl;
        int secondSet = random(1,2);
        for(int i = 0; i != secondSet; i++)
        {
            aspects.push_back(aspectList.getRandomName());
            std::cout << "Aspect: " << aspects.back() << std::endl;
        }
        aspects.push_back(conditionList.getRandomName());
        std::cout << "Condition: " << aspects.back() << std::endl;
        aspects.push_back(aspectList.getRandomName());
        std::cout << "Final Aspect: " << aspects.back() << std::endl;
    }

    int multipliers[10] = {1,2,4,8,12,16,20,24,28,32};
    for(auto aspectString : aspects)
    {
        std::cout << "Building Aspect: " << aspectString;
        StatusAspect aspect;
        aspect.potency = 0;
        aspect.name = getAspectNum(aspectString);
        int minRandom = 1;
        int maxRandom = 5;
        bool ableFlipped = true;
        if(aspectString == "AffectHealth")
            aspect.type = damageList.getRandomName();
        if(aspectString == "AffectDamage")
        {
            minRandom = 5;
            maxRandom = 20;
            aspect.type = damageList.getRandomName();
        }
        if(aspectString == "Armor")
        {
            minRandom = 5;
            maxRandom = 20;
            aspect.type = damageList.getRandomName();
        }
        if(aspectString == "Freeze")
        {
            minRandom = 100;
            maxRandom = 500;
            ableFlipped = false;
        }
        if(aspectString == "Sleep")
        {
            minRandom = 100;
            maxRandom = 500;
            ableFlipped = false;
        }
        if(aspectString == "Stun")
        {
            minRandom = 1;
            maxRandom = 1;
            ableFlipped = false;
        }
        if(aspectString == "Thorns")
        {
            minRandom = 10;
            maxRandom = 50;
            ableFlipped = false;
            aspect.type = damageList.getRandomName();
        }
        if(aspectString == "Immunity")
        {
            minRandom = 1;
            maxRandom = 1;
            ableFlipped = false;
            aspect.type = damageList.getRandomName();
        }
        if(aspectString == "SpawnCreature")
        {
            minRandom = 1;
            maxRandom = 2;
            ableFlipped = false;
            RandomWeightList raceList;
            {
                for(auto npc : npcmanager.globalCritter)
                    if(npc.race != "Turret")
                        raceList.addEntry(npc.race,10000);
            }
            aspect.type = raceList.getRandomName();
        }
        if(aspectString == "SpawnItem")
        {
            minRandom = 1;
            maxRandom = 2;
            ableFlipped = false;
            aspect.type = damageList.getRandomName();
            RandomWeightList itemList;
            {
                for(auto item : itemmanager.globalItems)
                    itemList.addEntry(item.name,10000);
            }
            aspect.type = itemList.getRandomName();
        }
        if(aspectString == "Revive")
        {
            minRandom = 1;
            maxRandom = 1;
            ableFlipped = false;
        }
        if(aspectString == "AutoDodge")
        {
            minRandom = 1;
            maxRandom = 1;
            ableFlipped = false;
            aspect.type = damageList.getRandomName();
        }
        if(aspectString == "ChangeRace")
        {
            minRandom = 100;
            maxRandom = 500;
            ableFlipped = false;
            RandomWeightList raceList;
            {
                for(auto npc : npcmanager.globalCritter)
                    if(npc.race != "Turret")
                        raceList.addEntry(npc.race,10000);
            }
            aspect.type = raceList.getRandomName();
        }
        if(aspectString == "Attribute")
        {
            RandomWeightList attributeList;
            {
                attributeList.addEntry("Strength",10000);
                attributeList.addEntry("Perception",10000);
                attributeList.addEntry("Intelligence",10000);
                attributeList.addEntry("Charisma",10000);
                attributeList.addEntry("Endurance",10000);
                attributeList.addEntry("Dexterity",10000);
            }
            aspect.type = attributeList.getRandomName();
        }






        for(int i = 0; i != multipliers[rankNum]; i++)
        {
            int randomPotency = random(minRandom,maxRandom);
            std::cout << "Adding Potency: " << randomPotency << std::endl;
            aspect.potency += randomPotency;
        }




        if(ableFlipped)
        {
            if(random(0,1) == 0)
                aspect.potency = -aspect.potency;
        }






        if(aspectString == "ConditionHealth")
        {
            if(random(0,1) == 0)
            {
                aspect.type = "Below";
                for(int i = 0; i != multipliers[rankNum]; i++)
                    aspect.potency += 10;
                if(aspect.potency > 100)
                    aspect.potency = 100;
            }
            else
            {
                aspect.type = "Above";
                aspect.potency = 100;
                for(int i = 0; i != multipliers[rankNum]; i++)
                    aspect.potency -= 10;
                if(aspect.potency < 0)
                    aspect.potency = 0;
            }
        }

        if(aspectString == "ConditionLife")
        {
            if(random(0,1) == 0)
                aspect.type = "Dead";
            else
                aspect.type = "Alive";
            aspect.potency = 1;
        }

        if(aspectString == "ConditionOnItemUse")
        {
            aspect.potency = 1;
            RandomWeightList itemTypes;
            itemTypes.addEntry(str(1),10);
            itemTypes.addEntry(str(2),10);
            itemTypes.addEntry(str(23),10);
            itemTypes.addEntry(str(42),10);
            itemTypes.addEntry(str(69),10);
            aspect.type = itemTypes.getRandomName();
        }
        if(aspectString == "ConditionTimeDelay")
            aspect.potency = random(60,180);
        if(aspectString == "ConditionOnDeath")
            aspect.potency = 1;
        if(aspectString == "ConditionOnHit")
            aspect.potency = 1;
        if(aspectString == "ConditionOnHitByType")
        {
            aspect.potency = 1;
            aspect.type = damageList.getRandomName();
        }




        std::cout << ", Final Potency: " << aspect.potency << std::endl;
        status.aspects.push_back(aspect);
    }


    status.name = status.rank + " Chaos " + generateName(2,3);

    return status;
}

StatusEffect generateRandomStatusEffect(RandomWeightList rankList)
{
/*
Weapon Status Effect Generation
Order of Logic
Common, Uncommon, Rare, Legendary weighted list. This will govern the potency of Aspects. (Have an option to pay coin to force a generation of one of these)
1-10 Status Effects, on a Weighted List.
Each Status Effect will be...
1-2 Aspects
1 Conditional (If a ConditionHealth, Have a Weighted List to determin whether to have multiple conditions to keep amping up further aspects)
1-2 Aspects.
1 Conditional
1 Potent Aspect(Since it's quite unlikely for two random conditionals to ever be met, this could be wonderful.)
*/
    StatusEffect status;

    //RandomWeightList rankList;
    if(rankList.entries.empty())
    { // Fill Rank List
        rankList.addEntry("Alpha",1000000);
        rankList.addEntry("Beta",100000);
        rankList.addEntry("Gamma",10000);
        rankList.addEntry("Delta",1000);
        rankList.addEntry("Epsilon",100);
        rankList.addEntry("Zeta",10);
        rankList.addEntry("Eta",1);
    }
    RandomWeightList aspectList;
    { // Get Appropriote and Implemented aspects.
        for(auto &aspect : aspectNum)
        {
            if(aspect.find("Condition") != std::string::npos)
                continue;
            else if(aspect.find("Mana") != std::string::npos)
                continue;
            else if(aspect == "ConditionNearbyUnit")
                continue;
            else if(aspect == "ConditionUnitCount")
                continue;
            else if(aspect == "ConditionReceivedDamage")
                continue;
            else if(aspect == "ActionSpeed")
                continue;
            else if(aspect == "AmmoCost")
                continue;
            else if(aspect == "Accuracy")
                continue;
            else if(aspect == "StatusEffectImmunity")
                continue;
            else if(aspect == "ApplyMomentum")
                continue;
            else if(aspect == "MomentumSensitivity")
                continue;
            else if(aspect == "AffectVisionRange")
                continue;
            else if(aspect == "Mark")
                continue;
            else if(aspect == "DisableDeath")
                continue;

            aspectList.addEntry(aspect,10000);

        }
    }
    aspectList.printEntries();
    RandomWeightList conditionList;
    { // Get implemented conditions.
        for(auto &aspect : aspectNum)
        {
            if( !(aspect.find("Condition") != std::string::npos))
                continue;
            else if(aspect.find("Mana") != std::string::npos)
                continue;
            else if(aspect == "ConditionNearbyUnit")
                continue;
            else if(aspect == "ConditionUnitCount")
                continue;
            else if(aspect == "ConditionReceivedDamage")
                continue;

            conditionList.addEntry(aspect,10000);

        }
    }
    conditionList.printEntries();
    RandomWeightList damageList;
    {
        for(auto damString : damageTypes.TypeStrings)
            if(damString != "None")
                damageList.addEntry(damString,10000);
    }
    damageList.printEntries();

    int rankNum = rankList.getRandomSlot();
    status.rank = rankList.entries[rankNum].name;
    std::cout << "Generated Rank: " << status.rank << std::endl;

    std::vector<std::string> aspects;
    { // We generate the aspects and condition's to be put into the status effect...
        int firstSet = random(1,2);
        for(int i = 0; i != firstSet; i++)
        {
            aspects.push_back(aspectList.getRandomName());
            std::cout << "Aspect: " << aspects.back() << std::endl;
        }
        aspects.push_back(conditionList.getRandomName());
        std::cout << "Condition: " << aspects.back() << std::endl;
        int secondSet = random(1,2);
        for(int i = 0; i != secondSet; i++)
        {
            aspects.push_back(aspectList.getRandomName());
            std::cout << "Aspect: " << aspects.back() << std::endl;
        }
        aspects.push_back(conditionList.getRandomName());
        std::cout << "Condition: " << aspects.back() << std::endl;
        aspects.push_back(aspectList.getRandomName());
        std::cout << "Final Aspect: " << aspects.back() << std::endl;
    }

    { // Generation of status's aspects and conditions.
        int firstSet = random(1,2);
        for(int i = 0; i != firstSet; i++)
            status.aspects.push_back(generateRandomStatusAspectConstant(rankNum));
        status.aspects.push_back(generateRandomStatusAspectConditionConstant(rankNum));
        int secondSet = random(1,2);
        for(int i = 0; i != secondSet; i++)
            status.aspects.push_back(generateRandomStatusAspectConstant(rankNum));
        status.aspects.push_back(generateRandomStatusAspectConditionOnce(rankNum));
        status.aspects.push_back(generateRandomStatusAspectOnce(rankNum));

    }

    status.name = status.rank + " Chaos " + generateName(2,3);

    return status;
}

Item generateRandomItem(RandomWeightList equipmentSet)
{


    std::vector<Item> ammoBullet = itemmanager.getAllofType(3);
    std::vector<Item> ammoShell = itemmanager.getAllofType(4);
    std::vector<Item> ammoMissile = itemmanager.getAllofType(5);
    std::vector<Item> rangedweps = itemmanager.getAllofType(2);
    std::vector<Item> meleeweps = itemmanager.getAllofType(1);

    //RandomWeightList equipmentSet;
    //equipmentSet.addEntry("Melee",100);
    //equipmentSet.addEntry("Ranged",100);

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

    std::string equipmentResult = equipmentSet.getRandomName();

    //Item meleeWeapon = *getGlobalItem(meleeList.getRandomName());
    //member.inventory.push_back(meleeWeapon);

    Item returnItem;


    if(equipmentResult == "Ranged")
        returnItem = *getGlobalItem(rangedList.getRandomName());
    if(equipmentResult == "Melee")
        returnItem = *getGlobalItem(meleeList.getRandomName());
    if(equipmentResult == "Bullet")
        returnItem = *getGlobalItem(bulletList.getRandomName());
    if(equipmentResult == "Shell")
        returnItem = *getGlobalItem(shellList.getRandomName());
    if(equipmentResult == "Missile")
        returnItem = *getGlobalItem(missileList.getRandomName());
    if(equipmentResult == "Charm")
        returnItem = *getGlobalItem("Charm");

    return returnItem;
}

void generateEnchantedItems()
{
    int randomAmount = random(3,20);
    for(int i = 0; i != randomAmount; i++)
    {

        RandomWeightList equipmentSet;
        equipmentSet.addEntry("Melee",100);
        equipmentSet.addEntry("Ranged",100);
        equipmentSet.addEntry("Bullet",100);
        equipmentSet.addEntry("Shell",25);
        equipmentSet.addEntry("Missile",10);

        Item enchantItem = generateRandomItem(equipmentSet);

        enchantedItems.push_back(enchantItem);
    }

}

void enchantMenu(Vec3f creationPos)
{
    for(auto menu : menus)
        if(menu.name == "Enchant Menu")
            return;

    if(enchantedItems.empty())
        generateEnchantedItems();

    baseMenu sMenu;
    sMenu.name = "Enchant Menu";
    sMenu.Pos = sf::Vector2f(screen.x()/2,screen.y()/2);
    sMenu.makePos = creationPos;
    menus.push_back(sMenu);
}

void gunModMenu(Item * item)
{
    for(auto menu : menus)
        if(menu.name == "Gun Mod Menu")
            return;

    baseMenu sMenu;
    sMenu.name = "Gun Mod Menu";
    sMenu.Pos = sf::Vector2f(screen.x()/2,screen.y()/2);

    dynamicVariable itemCarry;
    itemCarry.name = "Item";
    itemCarry.varItemPtr = item;
    sMenu.vars.push_back(itemCarry);

    itemCarry.name = "Original Item";
    sMenu.vars.push_back(itemCarry);


    menus.push_back(sMenu);
}

void itemHotBarRMBMenu(Item* item)
{
    for(auto menu : menus)
        if(menu.name == "Item Hotbar RMB Menu")
            return;

    baseMenu sMenu;
    sMenu.name = "Item Hotbar RMB Menu";
    sMenu.Pos = sf::Vector2f(screen.x()/2,screen.y()/2);

    dynamicVariable itemCarry;
    itemCarry.name = "Item";
    itemCarry.varItemPtr = item;
    sMenu.vars.push_back(itemCarry);

    itemCarry.name = "Original Item";
    sMenu.vars.push_back(itemCarry);


    menus.push_back(sMenu);
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
        if(inputState.key[Key::LShift])
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
                shapes.createText(gvars::mousePos,10,sf::Color::White,"   You must beat the game to unlock the Paragon tree! (Demo: Beat a tower)");
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

    int offTavernMultipler = 1;
    if(bountytower::currentTower->name != towers[0].name)
        offTavernMultipler = 4;

    sf::Vector2f menuStartPos(100,100);
    sf::Vector2f menuEndPos(screen.x()-100,screen.y()-100);
    sf::Color menuColor(sf::Color::Black);

    shapes.createSquare(menuStartPos.x,menuStartPos.y,menuEndPos.x,menuEndPos.y,menuColor,5,sf::Color::Cyan,&gvars::hudView);
    shapes.shapes.back().layer = layer+BackPanel;

    shapes.createSquare(menuStartPos.x,menuStartPos.y,menuEndPos.x,menuStartPos.y+60,menuColor,0,sf::Color::Cyan,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel;



    shapes.createSquare(percentPos(60,menuStartPos.x,menuEndPos.x),percentPos(0,menuStartPos.y,menuEndPos.y),menuEndPos.x,menuEndPos.y,sf::Color::Black,1,sf::Color::White,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel+1;
    //Close Button
    int exitButt = shapes.createImageButton(sf::Vector2f(screen.x()-100,100),texturemanager.getTexture("ExitButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel+2;
    if(shapes.shapeClicked(exitButt))
        menu.toDelete = true;

    /*
    int renewButt = shapes.createImageButton(sf::Vector2f(screen.x()-200,150),texturemanager.getTexture("ExitButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel+1;
    if(shapes.shapeClicked(renewButt))
    {
        enchantedItems.clear();
        generateEnchantedItems();
    }
    */



    shapes.createText(menuStartPos,10,sf::Color::White,"Squad Charisma Discount: %" + str(100-getSquadDiscount(100)),&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel+1;

    /* Old buttons, just don't fit.
    sf::Vector2f upScrollButtPos(percentPos(xMod,menuStartPos.x,menuEndPos.x),percentPos(yMod,menuStartPos.y,menuEndPos.y));
    sf::Vector2f downScrollButtPos(percentPos(10,menuStartPos.x,menuEndPos.x),percentPos(85,menuStartPos.y,menuEndPos.y));

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

    */


    dynamicVariable * currentType = menu.getVar("currentType");
    { // Current Item Type Selection
        if(!menu.hasVar("currentType"))
        {
            dynamicVariable var;
            var.name = "currentType";
            var.varString = "All";

            menu.vars.push_back(var);
        }
        if(inputState.key[Key::LShift])
            std::cout << "Menu: " << menu.name << " has " << menu.vars.size() << " dynamic vars. \n";
        int yOffset = 0;


        // Types: All, Weapons, Ammo, Aid, Misc
        const int totalTypes = 7;
        std::string types[totalTypes] = {"All", "Weapons", "Ammo", "Magic", "Engineer", "Aid", "Misc"};

        for(int i = 0; i != totalTypes; i++)
        {
            sf::Vector2f drawPos(152,150+(yOffset*31));
            int skillTreeButt = shapes.createImageButton(drawPos,texturemanager.getTexture("blankLargeButton.png"),"",0,&gvars::hudView);
            shapes.shapes.back().layer = layer+75;


            if(shapes.shapeClicked(skillTreeButt))
            {
                menu.scrollOne = 0;
                if(currentType != nullptr)
                    currentType->varString = types[i];
            }
            sf::Color highlightColor = sf::Color::White;
            if(shapes.shapeHovered(skillTreeButt))
                highlightColor = sf::Color::Cyan;

            sf::Vector2f textPos(drawPos.x-40, drawPos.y-5);
            shapes.createText(textPos,10,highlightColor,types[i],&gvars::hudView);
            shapes.shapes.back().layer = layer+76;

            yOffset++;
        }
    }

    dynamicVariable * selectedItem = menu.getVar("selectedItem");

    int xOffset = 0;
    int yOffset = 0;

    for(auto &item : itemmanager.globalItems)
    {
        if(item.value == -1)
            continue;
        if(currentType == nullptr)
            continue;

        if(currentType->varString != "All")
        {
            if(item.type == 1 || item.type == 2)
            {
                if(currentType->varString != "Weapons")
                    continue;
            }
            else if(item.type == 3 || item.type == 4 || item.type == 5)
            {
                if(currentType->varString != "Ammo")
                    continue;
            }
            else if(item.type == 23)
            {
                if(currentType->varString != "Magic")
                    continue;
            }
            else if(item.type == 12)
            {
                if(currentType->varString != "Engineer")
                    continue;
            }
            else if(item.type == 42)
            {
                if(currentType->varString != "Aid")
                    continue;
            }
            else if(item.type == 69)
            {
                if(currentType->varString != "Misc")
                    continue;
            }
            else
                continue;

        }


        int posX = 250+(xOffset*300);
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


        shapes.createText(vPos,12,highlightColor,outPut,&gvars::hudView);
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
            stats.append(", Mag: " + str(item.getAmmoCapacity()));
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
        //shapes.createText(vPos,10,highlightColor,stats,&gvars::hudView);
        //shapes.shapes.back().layer = layer+Text;



        if(shapes.shapeClicked(itemButt) && menu.age > 30 && gvars::hovering == 0)
        {
            if(selectedItem == nullptr)
            {
                dynamicVariable var;
                var.name = "selectedItem";
                var.varItemPtr = &item;
                menu.vars.push_back(var);
                selectedItem = &menu.vars.back();
            }
            else
            {
                selectedItem->varItemPtr = &item;
            }



            /*

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

            */
        }





        xOffset++;
        if(xOffset > 1)
        {
            xOffset = 0;
            yOffset++;
        }
    }

    if(selectedItem == nullptr)
        return;

    Item * item = selectedItem->varItemPtr;

    sf::Vector2f statusPos(percentPos(61,menuStartPos.x,menuEndPos.x),percentPos(5,menuStartPos.y,menuEndPos.y));
    std::string itemDesc = itemDescription(*item);
    //shapes.createText(statusPos,10,sf::Color::White,item->name + ", Total Status Effects: " + std::to_string(item->statusEffects.size()+item->statusEffectsInflict.size()+item->statusEffectsCarried.size()),&gvars::hudView);
    shapes.createText(statusPos,10,sf::Color::White,itemDesc,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel+4;



    { // Purchase Item

        sf::Vector2f randomEnchantPos(percentPos(63,menuStartPos.x,menuEndPos.x), percentPos(97,menuStartPos.y,menuEndPos.y));
        int randomEnchantButt = shapes.createImageButton(randomEnchantPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Button;
        randomEnchantPos.x -= 28;
        randomEnchantPos.y -= 10;
        textList.createText(randomEnchantPos,9,sf::Color::White,"Purchase \nItem",gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Text;

        if(shapes.shapeHovered(randomEnchantButt))
        {
            int randomEnchantCost = getSquadDiscount(item->value*offTavernMultipler);
            textList.createText(gvars::mousePos,10,sf::Color::White,"   $"+str(randomEnchantCost)+", Pay for the item, Receive it instantly, Capitalism!");
            shapes.shapes.back().layer = 99999999;
            gvars::hovering = 3;


            if(inputState.lmbTime == 1)
            {
                if(conFact->credits < getSquadDiscount(item->value*offTavernMultipler))
                    chatBox.addChat("You do not have enough cash for "+item->name+"!", sf::Color::White);
                else
                {
                    conFact->credits -= getSquadDiscount(item->value*offTavernMultipler);
                    Item soldItem = *item;
                    soldItem.id = gvars::globalid++;
                    soldItem.xpos = menu.makePos.x+(randz(-90,90));
                    soldItem.ypos = menu.makePos.y+45;
                    soldItem.zpos = menu.makePos.z;
                    soldItem.amount = soldItem.stackSize;

                    worlditems.push_back(soldItem);

                    int soundRan = random(1,3);
                    soundmanager.playSound("CashPickup"+str(soundRan)+".ogg");

                    chatBox.addChat("You purchased a "+item->name+" for "+str(getSquadDiscount(item->value*offTavernMultipler))+"!", sf::Color::White);
                }



            }
        }
    }

    /*

    {   // Remove Last Status Effect
        sf::Vector2f randomEnchantPos(percentPos(21,menuStartPos.x,menuEndPos.x), percentPos(62,menuStartPos.y,menuEndPos.y));
        int randomEnchantButt = shapes.createImageButton(randomEnchantPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Button;
        randomEnchantPos.x -= 28;
        randomEnchantPos.y -= 10;
        textList.createText(randomEnchantPos,9,sf::Color::White,"Remove \nSelf",gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Text;

        if(shapes.shapeHovered(randomEnchantButt))
        {
            int removeEnchantCost = getSquadDiscount(250);
            textList.createText(gvars::mousePos,10,sf::Color::White,"   $"+str(removeEnchantCost)+", Removes the last Self Enchant on the item.");
            shapes.shapes.back().layer = 99999999;

            if(inputState.lmbTime == 1)
            {
                if(item->statusEffects.empty())
                    chatBox.addChat("There's no Self Enchants to remove!",sf::Color::White);
                else if(conFact->credits < removeEnchantCost)
                    chatBox.addChat("You do not have enough money!",sf::Color::White);
                else
                {
                    soundmanager.playSound("enchantSound.ogg");
                    conFact->credits -= removeEnchantCost;
                    item->statusEffects.back().toDelete = true;
                    AnyDeletes(item->statusEffects);


                }
            }
        }
    }
    {   // Remove Last Status Effect Inflict
        sf::Vector2f randomEnchantPos(percentPos(27,menuStartPos.x,menuEndPos.x), percentPos(62,menuStartPos.y,menuEndPos.y));
        int randomEnchantButt = shapes.createImageButton(randomEnchantPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Button;
        randomEnchantPos.x -= 28;
        randomEnchantPos.y -= 10;
        textList.createText(randomEnchantPos,9,sf::Color::White,"Remove \nInflict",gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Text;

        if(shapes.shapeHovered(randomEnchantButt))
        {
            int removeEnchantCost = getSquadDiscount(250);
            textList.createText(gvars::mousePos,10,sf::Color::White,"   $"+str(removeEnchantCost)+", Removes the last Inflict Enchant on the item.");
            shapes.shapes.back().layer = 99999999;

            if(inputState.lmbTime == 1)
            {
                if(item->statusEffectsInflict.empty())
                    chatBox.addChat("There's no Inflict Enchants to remove!",sf::Color::White);
                else if(conFact->credits < removeEnchantCost)
                    chatBox.addChat("You do not have enough money!",sf::Color::White);
                else
                {
                    soundmanager.playSound("enchantSound.ogg");
                    conFact->credits -= removeEnchantCost;
                    item->statusEffectsInflict.back().toDelete = true;
                    AnyDeletes(item->statusEffectsInflict);

                }
            }
        }
    }
    {   // Swap Last Self Enchant
        sf::Vector2f randomEnchantPos(percentPos(33,menuStartPos.x,menuEndPos.x), percentPos(62,menuStartPos.y,menuEndPos.y));
        int randomEnchantButt = shapes.createImageButton(randomEnchantPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Button;
        randomEnchantPos.x -= 28;
        randomEnchantPos.y -= 10;
        textList.createText(randomEnchantPos,9,sf::Color::White,"Swap \nSelf",gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Text;

        if(shapes.shapeHovered(randomEnchantButt))
        {
            int removeEnchantCost = getSquadDiscount(250);
            textList.createText(gvars::mousePos,10,sf::Color::White,"   $"+str(removeEnchantCost)+", Turns last Self enchant into an Inflict enchant.");
            shapes.shapes.back().layer = 99999999;

            if(inputState.lmbTime == 1)
            {
                if(item->statusEffects.empty())
                    chatBox.addChat("There's no Self Enchants to move!",sf::Color::White);
                else if(conFact->credits < removeEnchantCost)
                    chatBox.addChat("You do not have enough money!",sf::Color::White);
                else
                {

                    conFact->credits -= removeEnchantCost;
                    item->statusEffectsInflict.push_back(item->statusEffects.back());
                    item->statusEffectsInflict.back().duration = 180;
                    item->statusEffects.back().toDelete = true;
                    AnyDeletes(item->statusEffects);

                }
            }
        }
    }
    {   // Swap Last Inflict Enchant
        sf::Vector2f randomEnchantPos(percentPos(39,menuStartPos.x,menuEndPos.x), percentPos(62,menuStartPos.y,menuEndPos.y));
        int randomEnchantButt = shapes.createImageButton(randomEnchantPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Button;
        randomEnchantPos.x -= 28;
        randomEnchantPos.y -= 10;
        textList.createText(randomEnchantPos,9,sf::Color::White,"Swap \nInflict",gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Text;

        if(shapes.shapeHovered(randomEnchantButt))
        {
            int removeEnchantCost = getSquadDiscount(250);
            textList.createText(gvars::mousePos,10,sf::Color::White,"   $"+str(removeEnchantCost)+", Turns last Inflict enchant into a self enchant.");
            shapes.shapes.back().layer = 99999999;

            if(inputState.lmbTime == 1)
            {
                if(item->statusEffectsInflict.empty())
                    chatBox.addChat("There's no Inflict Enchants to remove!",sf::Color::White);
                else if(conFact->credits < removeEnchantCost)
                    chatBox.addChat("You do not have enough money!",sf::Color::White);
                else
                {
                    conFact->credits -= removeEnchantCost;
                    item->statusEffects.push_back(item->statusEffectsInflict.back());
                    item->statusEffects.back().duration = 1;
                    item->statusEffectsInflict.back().toDelete = true;
                    AnyDeletes(item->statusEffectsInflict);

                }
            }
        }
    }

    */

    yOffset = 1;
    for(auto &status : item->statusEffects)
    {
        std::string outPut = "Equip:";
        outPut.append("[" + status.name + ", Duration: " + str(status.duration) + "f]");
        for(auto &aspect : status.aspects)
        {
            if(aspectNum[aspect.name].find("Condition") != std::string::npos)
                outPut.append("\n   ");
            outPut.append("[" + aspectNum[aspect.name] + ",Potency:" + str(static_cast<int>(aspect.potency)));
            if(aspect.type != "")
                outPut.append(",Type:" + aspect.type);
            outPut.append("]");
        }

        sf::Vector2f vPos = statusPos;
        vPos.y += 10*yOffset;
        shapes.createText(vPos,8,sf::Color::Cyan,outPut,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+4;

        yOffset++;
        yOffset++;
        yOffset++;
    }
    for(auto &status : item->statusEffectsInflict)
    {
        std::string outPut = "Inflict:";

        outPut.append("[" + status.name + ", Duration: " + str(status.duration) + "f]");
        for(auto &aspect : status.aspects)
        {
            if(aspectNum[aspect.name].find("Condition") != std::string::npos)
                outPut.append("\n   ");
            outPut.append("[" + aspectNum[aspect.name] + ",Potency:" + str(static_cast<int>(aspect.potency)));
            if(aspect.type != "")
                outPut.append(",Type:" + aspect.type);
            outPut.append("]");
        }

        sf::Vector2f vPos = statusPos;
        vPos.y += 10*yOffset;
        shapes.createText(vPos,8,sf::Color(255,150,150),outPut,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+4;

        yOffset++;
        yOffset++;
        yOffset++;
    }
    for(auto &status : item->statusEffectsCarried)
    {
        std::string outPut = "Carry:";

        outPut.append("[" + status.name + ", Duration: " + str(status.duration) + "f]");
        for(auto &aspect : status.aspects)
        {
            if(aspectNum[aspect.name].find("Condition") != std::string::npos)
                outPut.append("\n   ");
            outPut.append("[" + aspectNum[aspect.name] + ",Potency:" + str(static_cast<int>(aspect.potency)));
            if(aspect.type != "")
                outPut.append(",Type:" + aspect.type);
            outPut.append("]");
        }

        sf::Vector2f vPos = statusPos;
        vPos.y += 10*yOffset;
        shapes.createText(vPos,8,sf::Color::Yellow,outPut,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+4;

        yOffset++;
        yOffset++;
        yOffset++;
    }

}

void renderMerchantModderMenu(baseMenu &menu)
{ // Layer 19500
    int layer = 19500;
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


    shapes.createText(menuStartPos,10,sf::Color::White,"Squad Charisma Discount: %" + str(100-getSquadDiscount(100)),&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel+1;

    sf::Vector2f upScrollButtPos(percentPos(10,menuStartPos.x,menuEndPos.x),percentPos(75,menuStartPos.y,menuEndPos.y));
    sf::Vector2f downScrollButtPos(percentPos(10,menuStartPos.x,menuEndPos.x),percentPos(85,menuStartPos.y,menuEndPos.y));
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


    dynamicVariable * currentType = menu.getVar("currentType");
    { // Current Item Type Selection
        if(!menu.hasVar("currentType"))
        {
            dynamicVariable var;
            var.name = "currentType";
            var.varString = "All";

            menu.vars.push_back(var);
        }
        if(inputState.key[Key::LShift])
            std::cout << "Menu: " << menu.name << " has " << menu.vars.size() << " dynamic vars. \n";
        int yOffset = 0;


        // Types: All, Weapons, Ammo, Aid, Misc
        const int totalTypes = 7;
        std::string types[totalTypes] = {"All", "Weapons", "Ammo", "Magic", "Engineer", "Aid", "Misc"};


        for(int i = 0; i != totalTypes; i++)
        {
            sf::Vector2f drawPos(152,150+(yOffset*60));
            int skillTreeButt = shapes.createImageButton(drawPos,texturemanager.getTexture("blankLargeButton.png"),"",0,&gvars::hudView);
            shapes.shapes.back().layer = layer+75;


            if(shapes.shapeClicked(skillTreeButt))
            {
                menu.scrollOne = 0;
                if(currentType != nullptr)
                    currentType->varString = types[i];
            }
            sf::Color highlightColor = sf::Color::White;
            if(shapes.shapeHovered(skillTreeButt))
                highlightColor = sf::Color::Cyan;

            sf::Vector2f textPos(drawPos.x-45, drawPos.y);
            shapes.createText(textPos,10,highlightColor,types[i],&gvars::hudView);
            shapes.shapes.back().layer = layer+76;

            yOffset++;
        }
    }


    int xOffset = 0;
    int yOffset = 0;
    for(auto item : gunModMerchantInventory)
    {
        if(item.value == -1)
            continue;
        if(currentType == nullptr)
            continue;

        if(currentType->varString != "All")
        {
            if(item.type == 1 || item.type == 2)
            {
                if(currentType->varString != "Weapons")
                    continue;
            }
            else if(item.type == 3 || item.type == 4 || item.type == 5)
            {
                if(currentType->varString != "Ammo")
                    continue;
            }
            else if(item.type == 23)
            {
                if(currentType->varString != "Magic")
                    continue;
            }
            else if(item.type == 12)
            {
                if(currentType->varString != "Engineer")
                    continue;
            }
            else if(item.type == 42)
            {
                if(currentType->varString != "Aid")
                    continue;
            }
            else if(item.type == 69)
            {
                if(currentType->varString != "Misc")
                    continue;
            }
            else
                continue;

        }


        int posX = 250+(xOffset*300);
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
            stats.append(", Mag: " + str(item.getAmmoCapacity()));
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
        if(shapes.shapeHovered(itemButt) && menu.age > 30)
        {
            sf::Vector2f mouseDis = gvars::mousePos;
            mouseDis.y += 10;
            shapes.createText(mouseDis,10,sf::Color::White,item.desc);
            shapes.shapes.back().layer = layer+Text;
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

    sf::Vector2f rerollPos(screen.x()-200,200);
    int rerollButt = shapes.createImageButton(rerollPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+Button;
    rerollPos.x -= 31;
    textList.createText(rerollPos,9,sf::Color::White,"Restock",gvars::hudView);
    shapes.shapes.back().layer = layer+Text;

    if(shapes.shapeClicked(rerollButt))
    {
        menu.toDelete = true;
        recruitables.clear();
        recruiterMenu(menu.makePos);
    }



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

        critterCost = npc.value;

        int posX = 150+(xOffset*300);
        int posY = 150+(yOffset*60);;
        shapes.createSquare(posX-30,posY-30,posX+30,posY+30,sf::Color::Black,0,sf::Color::Cyan, &gvars::hudView);
        shapes.shapes.back().layer = layer+Shapes;



        sf::Vector2f vPos(posX,posY);

        int npcButt = shapes.createImageButton(vPos,texturemanager.getTexture(npc.getRace()+".png"),"",0,&gvars::hudView);
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
        std::string outPut = "Speed: " + str(static_cast<int>(npc.moveSpeed) );
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
        vInsert = sf::Vector2f(150,300);
        towerPos.push_back(vInsert);
        vInsert = sf::Vector2f(150,450);
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
        generateHiddenGoodies();
        setTileImages(towers[1].name);


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

    int necroTowButt = shapes.createImageButton(towerPos[1],*towers[2].tex,"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+Button;
    if(shapes.shapeClicked(necroTowButt))
    {
        menu.toDelete = true;
        bountytower::pausewaves = true;
        bountytower::towerLoaded = towers[2].name;
        bountytower::currentTower = &towers[2];
        gCtrl.phase = "Lobby";
        //buildTower(towers[1].name);

        loadMap(towers[2].mapID,0,0,50,50);

        int xview = (96*60)/20;
        gvars::currentx = xview/2;
        gvars::currenty = xview/1.4;

        towerTransition();
        generateHiddenGoodies();
        setTileImages(towers[2].name);


        positionSquaddies();

        playMusic("KerriOverblowEcho.ogg");

    }

    //Draw some info about the thing.
    {
        std::string textOut = " Tower: " + towers[2].name +
        //"\n Minions: " + std::to_string(towers[1].minioncount) +
        "\n Target: " + towers[2].bountyTarget +
        "\n Bounty: $" + str(towers[2].bountyPay) +
        "\n Difficulty: " + std::to_string(towers[2].difficulty);
        sf::Vector2f textPos(towerPos[1].x+50,towerPos[1].y-25);
        shapes.createText(textPos,15,sf::Color::White,textOut,&gvars::hudView);
        shapes.shapes.back().layer = layer+Text;
    }

    int powerTowButt = shapes.createImageButton(towerPos[2],*towers[3].tex,"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+Button;
    if(shapes.shapeClicked(powerTowButt))
    {
        menu.toDelete = true;
        bountytower::pausewaves = true;
        bountytower::towerLoaded = towers[3].name;
        bountytower::currentTower = &towers[3];
        gCtrl.phase = "Lobby";
        //buildTower(towers[1].name);

        loadMap(towers[3].mapID,0,0,50,50);

        int xview = (96*60)/20;
        gvars::currentx = xview/2;
        gvars::currenty = xview/1.4;

        towerTransition();
        generateHiddenGoodies();
        setTileImages(towers[3].name);


        positionSquaddies();



        playMusic("KerriOverblowEcho.ogg");

    }

    //Draw some info about the thing.
    {
        std::string textOut = " Tower: " + towers[3].name +
        //"\n Minions: " + std::to_string(towers[1].minioncount) +
        "\n Target: " + towers[3].bountyTarget +
        "\n Bounty: $" + str(towers[3].bountyPay) +
        "\n Difficulty: " + std::to_string(towers[3].difficulty);
        sf::Vector2f textPos(towerPos[2].x+50,towerPos[2].y-25);
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

    exitPos.x += 31;
    exitPos.y -= 40;

    int homeGameButt = shapes.createImageButton(exitPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = 15050;
    exitPos.x -= 31;
    textList.createText(exitPos,9,sf::Color::White,"Return Home",gvars::hudView);
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

    if(menu.age > 30 && shapes.shapeClicked(homeGameButt))
    {
        chatBox.addChat("Weren't up to the task, I see.");
        chatBox.addChat("Your next performance better be interesting!");
        saveGame("Profile1");
        soundmanager.playSound("Startup.wav");
        loadTavern();
        menu.toDelete = true;
    }


    if(menu.age > 30 && shapes.shapeClicked(exitGameButt))
    {
        saveGame("Profile1");
        window.close();
    }


}

void renderEnchantMenu(baseMenu &menu)
{ // Layer 22000
    int layer = 22000;
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
    sf::Color menuColor(sf::Color::Black);

    shapes.createSquare(menuStartPos.x,menuStartPos.y,menuEndPos.x,menuEndPos.y,menuColor,5,sf::Color::Cyan,&gvars::hudView);
    shapes.shapes.back().layer = layer+BackPanel;

    shapes.createSquare(menuStartPos.x,menuStartPos.y,menuEndPos.x,menuStartPos.y+60,menuColor,0,sf::Color::Cyan,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel;

    shapes.createSquare(percentPos(12,menuStartPos.x,menuEndPos.x),percentPos(60,menuStartPos.y,menuEndPos.y),menuEndPos.x,menuEndPos.y,sf::Color::Black,0,sf::Color::White,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel;
    //Close Button
    int exitButt = shapes.createImageButton(sf::Vector2f(screen.x()-100,100),texturemanager.getTexture("ExitButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel+1;
    if(shapes.shapeClicked(exitButt))
        menu.toDelete = true;

    /*
    int renewButt = shapes.createImageButton(sf::Vector2f(screen.x()-200,150),texturemanager.getTexture("ExitButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel+1;
    if(shapes.shapeClicked(renewButt))
    {
        enchantedItems.clear();
        generateEnchantedItems();
    }
    */



    shapes.createText(menuStartPos,10,sf::Color::White,"Squad Charisma Discount: %" + str(100-getSquadDiscount(100)),&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel+1;

    sf::Vector2f upScrollButtPos(percentPos(10,menuStartPos.x,menuEndPos.x),percentPos(75,menuStartPos.y,menuEndPos.y));
    sf::Vector2f downScrollButtPos(percentPos(10,menuStartPos.x,menuEndPos.x),percentPos(85,menuStartPos.y,menuEndPos.y));
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


    dynamicVariable * currentType = menu.getVar("currentType");
    { // Current Item Type Selection
        if(!menu.hasVar("currentType"))
        {
            dynamicVariable var;
            var.name = "currentType";
            var.varString = "All";

            menu.vars.push_back(var);
        }
        if(inputState.key[Key::LShift])
            std::cout << "Menu: " << menu.name << " has " << menu.vars.size() << " dynamic vars. \n";
        int yOffset = 0;


        // Types: All, Weapons, Ammo, Aid, Misc
        const int totalTypes = 7;
        std::string types[totalTypes] = {"All", "Weapons", "Ammo", "Magic", "Engineer", "Aid", "Misc"};

        for(int i = 0; i != totalTypes; i++)
        {
            sf::Vector2f drawPos(152,150+(yOffset*31));
            int skillTreeButt = shapes.createImageButton(drawPos,texturemanager.getTexture("blankLargeButton.png"),"",0,&gvars::hudView);
            shapes.shapes.back().layer = layer+75;


            if(shapes.shapeClicked(skillTreeButt))
            {
                menu.scrollOne = 0;
                if(currentType != nullptr)
                    currentType->varString = types[i];
            }
            sf::Color highlightColor = sf::Color::White;
            if(shapes.shapeHovered(skillTreeButt))
                highlightColor = sf::Color::Cyan;

            sf::Vector2f textPos(drawPos.x-40, drawPos.y-5);
            shapes.createText(textPos,10,highlightColor,types[i],&gvars::hudView);
            shapes.shapes.back().layer = layer+76;

            yOffset++;
        }
    }

    dynamicVariable * selectedItem = menu.getVar("selectedItem");

    int xOffset = 0;
    int yOffset = 0;

    int distance = 999999;
    Npc * closeSquaddy;
    for(auto &squaddy : Squaddies)
    {
        if(closeSquaddy == nullptr)
        {
            closeSquaddy = squaddy;
            distance = math::distance(squaddy->getPos(),menu.makePos);
        }

        else
        {
            int newDistance = math::distance(squaddy->getPos(),menu.makePos);
            if(newDistance < distance)
            {
                distance = newDistance;
                closeSquaddy = squaddy;
            }
        }
    }

    if(closeSquaddy == nullptr)
        return;

    for(auto &item : closeSquaddy->inventory)
    {
        if(item.value == -1)
            continue;
        if(currentType == nullptr)
            continue;

        if(currentType->varString != "All")
        {
            if(item.type == 1 || item.type == 2)
            {
                if(currentType->varString != "Weapons")
                    continue;
            }
            else if(item.type == 3 || item.type == 4 || item.type == 5)
            {
                if(currentType->varString != "Ammo")
                    continue;
            }
            else if(item.type == 23)
            {
                if(currentType->varString != "Magic")
                    continue;
            }
            else if(item.type == 12)
            {
                if(currentType->varString != "Engineer")
                    continue;
            }
            else if(item.type == 42)
            {
                if(currentType->varString != "Aid")
                    continue;
            }
            else if(item.type == 69)
            {
                if(currentType->varString != "Misc")
                    continue;
            }
            else
                continue;

        }


        int posX = 250+(xOffset*300);
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
            stats.append(", Mag: " + str(item.getAmmoCapacity()));
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
        shapes.createText(vPos,10,highlightColor,stats,&gvars::hudView);
        shapes.shapes.back().layer = layer+Text;



        if(shapes.shapeClicked(itemButt) && menu.age > 30)
        {
            if(selectedItem == nullptr)
            {
                dynamicVariable var;
                var.name = "selectedItem";
                var.varItemPtr = &item;
                menu.vars.push_back(var);
                selectedItem = &menu.vars.back();
            }
            else
            {
                selectedItem->varItemPtr = &item;
            }



            /*

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

            */
        }





        xOffset++;
        if(xOffset > 1)
        {
            xOffset = 0;
            yOffset++;
        }
    }

    if(selectedItem == nullptr)
        return;

    Item * item = selectedItem->varItemPtr;

    sf::Vector2f statusPos(percentPos(12,menuStartPos.x,menuEndPos.x),percentPos(65,menuStartPos.y,menuEndPos.y));
    shapes.createText(statusPos,10,sf::Color::White,item->name + ", Total Status Effects: " + std::to_string(item->statusEffects.size()+item->statusEffectsInflict.size()+item->statusEffectsCarried.size()),&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel+1;



    { // Random Enchant Button
        sf::Vector2f randomEnchantPos(percentPos(15,menuStartPos.x,menuEndPos.x), percentPos(62,menuStartPos.y,menuEndPos.y));
        int randomEnchantButt = shapes.createImageButton(randomEnchantPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Button;
        randomEnchantPos.x -= 28;
        randomEnchantPos.y -= 10;
        textList.createText(randomEnchantPos,9,sf::Color::White,"Random \nEnchant",gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Text;

        if(shapes.shapeHovered(randomEnchantButt))
        {
            int randomEnchantCost = getSquadDiscount(500);
            textList.createText(gvars::mousePos,10,sf::Color::White,"   $"+str(randomEnchantCost)+", Adds a randomized status effect to the item.");
            shapes.shapes.back().layer = 99999999;

            if(inputState.lmbTime == 1)
            {
                if(conFact->credits < randomEnchantCost)
                    chatBox.addChat("You do not have enough money!",sf::Color::White);
                else
                {
                    soundmanager.playSound("enchantSound.ogg");
                    conFact->credits -= randomEnchantCost;
                    StatusEffect status = generateRandomStatusEffect();
                    if(random(0,1) == 0)
                    {
                        status.duration = 1;
                        item->statusEffects.push_back(status);
                    }

                    else
                    {
                        status.duration = 180;
                        item->statusEffectsInflict.push_back(status);
                    }

                }
            }
        }
    }
    {   // Remove Last Status Effect
        sf::Vector2f randomEnchantPos(percentPos(21,menuStartPos.x,menuEndPos.x), percentPos(62,menuStartPos.y,menuEndPos.y));
        int randomEnchantButt = shapes.createImageButton(randomEnchantPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Button;
        randomEnchantPos.x -= 28;
        randomEnchantPos.y -= 10;
        textList.createText(randomEnchantPos,9,sf::Color::White,"Remove \nSelf",gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Text;

        if(shapes.shapeHovered(randomEnchantButt))
        {
            int removeEnchantCost = getSquadDiscount(250);
            textList.createText(gvars::mousePos,10,sf::Color::White,"   $"+str(removeEnchantCost)+", Removes the last Self Enchant on the item.");
            shapes.shapes.back().layer = 99999999;

            if(inputState.lmbTime == 1)
            {
                if(item->statusEffects.empty())
                    chatBox.addChat("There's no Self Enchants to remove!",sf::Color::White);
                else if(conFact->credits < removeEnchantCost)
                    chatBox.addChat("You do not have enough money!",sf::Color::White);
                else
                {
                    soundmanager.playSound("enchantSound.ogg");
                    conFact->credits -= removeEnchantCost;
                    item->statusEffects.back().toDelete = true;
                    AnyDeletes(item->statusEffects);


                }
            }
        }
    }
    {   // Remove Last Status Effect Inflict
        sf::Vector2f randomEnchantPos(percentPos(27,menuStartPos.x,menuEndPos.x), percentPos(62,menuStartPos.y,menuEndPos.y));
        int randomEnchantButt = shapes.createImageButton(randomEnchantPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Button;
        randomEnchantPos.x -= 28;
        randomEnchantPos.y -= 10;
        textList.createText(randomEnchantPos,9,sf::Color::White,"Remove \nInflict",gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Text;

        if(shapes.shapeHovered(randomEnchantButt))
        {
            int removeEnchantCost = getSquadDiscount(250);
            textList.createText(gvars::mousePos,10,sf::Color::White,"   $"+str(removeEnchantCost)+", Removes the last Inflict Enchant on the item.");
            shapes.shapes.back().layer = 99999999;

            if(inputState.lmbTime == 1)
            {
                if(item->statusEffectsInflict.empty())
                    chatBox.addChat("There's no Inflict Enchants to remove!",sf::Color::White);
                else if(conFact->credits < removeEnchantCost)
                    chatBox.addChat("You do not have enough money!",sf::Color::White);
                else
                {
                    soundmanager.playSound("enchantSound.ogg");
                    conFact->credits -= removeEnchantCost;
                    item->statusEffectsInflict.back().toDelete = true;
                    AnyDeletes(item->statusEffectsInflict);

                }
            }
        }
    }
    {   // Swap Last Self Enchant
        sf::Vector2f randomEnchantPos(percentPos(33,menuStartPos.x,menuEndPos.x), percentPos(62,menuStartPos.y,menuEndPos.y));
        int randomEnchantButt = shapes.createImageButton(randomEnchantPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Button;
        randomEnchantPos.x -= 28;
        randomEnchantPos.y -= 10;
        textList.createText(randomEnchantPos,9,sf::Color::White,"Swap \nSelf",gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Text;

        if(shapes.shapeHovered(randomEnchantButt))
        {
            int removeEnchantCost = getSquadDiscount(250);
            textList.createText(gvars::mousePos,10,sf::Color::White,"   $"+str(removeEnchantCost)+", Turns last Self enchant into an Inflict enchant.");
            shapes.shapes.back().layer = 99999999;

            if(inputState.lmbTime == 1)
            {
                if(item->statusEffects.empty())
                    chatBox.addChat("There's no Self Enchants to move!",sf::Color::White);
                else if(conFact->credits < removeEnchantCost)
                    chatBox.addChat("You do not have enough money!",sf::Color::White);
                else
                {

                    conFact->credits -= removeEnchantCost;
                    item->statusEffectsInflict.push_back(item->statusEffects.back());
                    item->statusEffectsInflict.back().duration = 180;
                    item->statusEffects.back().toDelete = true;
                    AnyDeletes(item->statusEffects);

                }
            }
        }
    }
    {   // Swap Last Inflict Enchant
        sf::Vector2f randomEnchantPos(percentPos(39,menuStartPos.x,menuEndPos.x), percentPos(62,menuStartPos.y,menuEndPos.y));
        int randomEnchantButt = shapes.createImageButton(randomEnchantPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Button;
        randomEnchantPos.x -= 28;
        randomEnchantPos.y -= 10;
        textList.createText(randomEnchantPos,9,sf::Color::White,"Swap \nInflict",gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Text;

        if(shapes.shapeHovered(randomEnchantButt))
        {
            int removeEnchantCost = getSquadDiscount(250);
            textList.createText(gvars::mousePos,10,sf::Color::White,"   $"+str(removeEnchantCost)+", Turns last Inflict enchant into a self enchant.");
            shapes.shapes.back().layer = 99999999;

            if(inputState.lmbTime == 1)
            {
                if(item->statusEffectsInflict.empty())
                    chatBox.addChat("There's no Inflict Enchants to remove!",sf::Color::White);
                else if(conFact->credits < removeEnchantCost)
                    chatBox.addChat("You do not have enough money!",sf::Color::White);
                else
                {
                    conFact->credits -= removeEnchantCost;
                    item->statusEffects.push_back(item->statusEffectsInflict.back());
                    item->statusEffects.back().duration = 1;
                    item->statusEffectsInflict.back().toDelete = true;
                    AnyDeletes(item->statusEffectsInflict);

                }
            }
        }
    }



    yOffset = 1;
    for(auto &status : item->statusEffects)
    {
        std::string outPut = "Equip:";
        outPut.append("[" + status.name + ", Duration: " + str(status.duration) + "f]");
        for(auto &aspect : status.aspects)
        {
            if(aspectNum[aspect.name].find("Condition") != std::string::npos)
                outPut.append("\n   ");
            outPut.append("[" + aspectNum[aspect.name] + ",Potency:" + str(static_cast<int>(aspect.potency)));
            if(aspect.type != "")
                outPut.append(",Type:" + aspect.type);
            outPut.append("]");
        }

        sf::Vector2f vPos = statusPos;
        vPos.y += 10*yOffset;
        shapes.createText(vPos,8,sf::Color::Cyan,outPut,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+1;

        yOffset++;
        yOffset++;
        yOffset++;
    }
    for(auto &status : item->statusEffectsInflict)
    {
        std::string outPut = "Inflict:";

        outPut.append("[" + status.name + ", Duration: " + str(status.duration) + "f]");
        for(auto &aspect : status.aspects)
        {
            if(aspectNum[aspect.name].find("Condition") != std::string::npos)
                outPut.append("\n   ");
            outPut.append("[" + aspectNum[aspect.name] + ",Potency:" + str(static_cast<int>(aspect.potency)));
            if(aspect.type != "")
                outPut.append(",Type:" + aspect.type);
            outPut.append("]");
        }

        sf::Vector2f vPos = statusPos;
        vPos.y += 10*yOffset;
        shapes.createText(vPos,8,sf::Color(255,150,150),outPut,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+1;

        yOffset++;
        yOffset++;
        yOffset++;
    }
    for(auto &status : item->statusEffectsCarried)
    {
        std::string outPut = "Carry:";

        outPut.append("[" + status.name + ", Duration: " + str(status.duration) + "f]");
        for(auto &aspect : status.aspects)
        {
            if(aspectNum[aspect.name].find("Condition") != std::string::npos)
                outPut.append("\n   ");
            outPut.append("[" + aspectNum[aspect.name] + ",Potency:" + str(static_cast<int>(aspect.potency)));
            if(aspect.type != "")
                outPut.append(",Type:" + aspect.type);
            outPut.append("]");
        }

        sf::Vector2f vPos = statusPos;
        vPos.y += 10*yOffset;
        shapes.createText(vPos,8,sf::Color::Yellow,outPut,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+1;

        yOffset++;
        yOffset++;
        yOffset++;
    }
}

void renderGunModMenu(baseMenu &menu)
{ // Layer 23000
    int layer = 23000;
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

    shapes.createSquare(percentPos(12,menuStartPos.x,menuEndPos.x),percentPos(60,menuStartPos.y,menuEndPos.y),menuEndPos.x,menuEndPos.y,sf::Color::Black,0,sf::Color::White,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel;
    //Close Button
    int exitButt = shapes.createImageButton(sf::Vector2f(screen.x()-100,100),texturemanager.getTexture("ExitButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel+1;
    if(shapes.shapeClicked(exitButt))
        menu.toDelete = true;



    shapes.createText(menuStartPos,10,sf::Color::White,"Squad Charisma Discount: %" + str(100-getSquadDiscount(100)),&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel+1;

    sf::Vector2f upScrollButtPos(percentPos(10,menuStartPos.x,menuEndPos.x),percentPos(75,menuStartPos.y,menuEndPos.y));
    sf::Vector2f downScrollButtPos(percentPos(10,menuStartPos.x,menuEndPos.x),percentPos(85,menuStartPos.y,menuEndPos.y));
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

    /*
    dynamicVariable * currentType = menu.getVar("currentType");
    { // Current Item Type Selection
        if(!menu.hasVar("currentType"))
        {
            dynamicVariable var;
            var.name = "currentType";
            var.varString = "All";

            menu.vars.push_back(var);
        }
        if(inputState.key[Key::LShift])
            std::cout << "Menu: " << menu.name << " has " << menu.vars.size() << " dynamic vars. \n";
        int yOffset = 0;


        // Types: All, Weapons, Ammo, Aid, Misc
        int totalTypes = 7;
        std::string types[totalTypes] = {"All", "Weapons", "Ammo", "Magic", "Engineer", "Aid", "Misc"};

        for(int i = 0; i != totalTypes; i++)
        {
            sf::Vector2f drawPos(152,150+(yOffset*60));
            int skillTreeButt = shapes.createImageButton(drawPos,texturemanager.getTexture("blankLargeButton.png"),"",0,&gvars::hudView);
            shapes.shapes.back().layer = layer+75;


            if(shapes.shapeClicked(skillTreeButt))
            {
                menu.scrollOne = 0;
                if(currentType != nullptr)
                    currentType->varString = types[i];
            }
            sf::Color highlightColor = sf::Color::White;
            if(shapes.shapeHovered(skillTreeButt))
                highlightColor = sf::Color::Cyan;

            sf::Vector2f textPos(drawPos.x-45, drawPos.y);
            shapes.createText(textPos,10,highlightColor,types[i],&gvars::hudView);
            shapes.shapes.back().layer = layer+76;

            yOffset++;
        }
    }


    */
    dynamicVariable * selectedItem = menu.getVar("Item");

    int xOffset = 0;
    int yOffset = 0;

    Item * weapon = selectedItem->varItemPtr;


    { // Current Item Display


        std::string outPut;
        sf::Color textColor = sf::Color::Cyan;
        bool hasAllParts = true;
        bool hasBody = false;
        bool hasBarrel = false;
        bool hasGrip = false;
        bool hasMagazine = false;
        for(auto insidePart : weapon->internalitems)
        {
            if(insidePart.type == itemTypes.getTypeID("PartBody").num)
                hasBody = true;
            if(insidePart.type == itemTypes.getTypeID("PartBarrel").num)
                hasBarrel = true;
            if(insidePart.type == itemTypes.getTypeID("PartGrip").num)
                hasGrip = true;
            if(insidePart.type == itemTypes.getTypeID("PartMagazine").num)
                hasMagazine = true;
        }
        if(hasBody && hasBarrel && hasGrip && hasMagazine)
        {
            hasAllParts = true;
            textColor = sf::Color::Cyan;
        }
        else if(weapon->type == itemTypes.getTypeID("Gun").num)
        {
            textColor = sf::Color::Red;

            outPut.append("**You are missing these:");
            if(!hasBody)
                outPut.append("|Body|");
            if(!hasBarrel)
                outPut.append("|Barrel|");
            if(!hasGrip)
                outPut.append("|Grip|");
            if(!hasMagazine)
                outPut.append("|Magazine|");

            outPut.append("** \n");
        }


        outPut.append(itemTypes.getTypeID(weapon->type).str + ": " + weapon->name + "\n");

        std::ostringstream num;
        num << std::setprecision(5);
        num << weapon->getBarrelCount();
        outPut.append("Barrel Count: " + num.str() + "\n");
        num.str(std::string());
        num.clear();

        num << weapon->getDamageMultiplier();
        outPut.append("Damage Multiplier: " + num.str() + "\n");
        num.str(std::string());
        num.clear();

        num << weapon->getDispersion();
        outPut.append("Dispersion: " + num.str() + "\n");
        num.str(std::string());
        num.clear();

        num << weapon->getAimTime()/60;
        outPut.append("Aim Time: " + num.str() + "s\n");
        num.str(std::string());
        num.clear();

        num << weapon->getRecoil();
        outPut.append("Recoil: " + num.str() + "\n");
        num.str(std::string());
        num.clear();

        num << weapon->getRecoilReduction();
        outPut.append("Recoil Reduction: " + num.str() + "\n");
        num.str(std::string());
        num.clear();

        num << weapon->getFireDelay();
        outPut.append("Fire Delay: " + num.str() + "\n");
        num.str(std::string());
        num.clear();

        num << weapon->getBurstCount();
        outPut.append("Burst Count: " + num.str() + "\n");
        num.str(std::string());
        num.clear();

        num << weapon->getAmmoCapacity();
        outPut.append("Ammo Capacity: " + num.str() + "\n");
        num.str(std::string());
        num.clear();

        num << weapon->getReloadTime()/60;
        outPut.append("Reload Time: " + num.str() + "s\n");
        num.str(std::string());
        num.clear();

        /*   Maybe one day
        num << weapon->getReloadAmount();
        outPut.append("Reload Amount: " + num.str() + "\n");
        num.str(std::string());
        num.clear();
        */

        num << weapon->getBulletSpeedMultiplier();
        outPut.append("Bullet Speed Multiplier: " + num.str() + "\n");
        num.str(std::string());
        num.clear();

        num << weapon->getDurability();
        outPut.append("Durability: " + num.str() + "\n");
        num.str(std::string());
        num.clear();

        num << weapon->getDurabilityCost();
        outPut.append("Durability Cost: " + num.str() + "\n");
        num.str(std::string());
        num.clear();





        sf::Vector2f vPos(250,420);
        shapes.createText(vPos,10,textColor,outPut,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+1;

    }

    { // Gun's Internal Parts.
        sf::Vector2f vDrawPos(102,126);

        std::string mainoutPut = "     " + menu.getVar("Original Item")->varItemPtr->name;
        shapes.createText(vDrawPos,8,sf::Color::Red,mainoutPut,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+1;

        int mainitemButt = shapes.createImageButton(vDrawPos,texturemanager.getTexture("ArrowButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+2;

        if(shapes.shapeClicked(mainitemButt))
            selectedItem->varItemPtr = menu.getVar("Original Item")->varItemPtr;

        if(shapes.shapeHovered(mainitemButt))
        {
            shapes.createText(sf::Vector2f(gvars::mousePos.x,gvars::mousePos.y-20),10,sf::Color::White,"Examine Weapon");
            shapes.shapes.back().layer = layer+FrontPanel+3;
        }



        yOffset = 2;

        std::string outPutInternal = "        Internal Items";
        sf::Vector2f vPosX = vDrawPos;
        vPosX.y += 20*yOffset;
        shapes.createText(vPosX,8,sf::Color::Cyan,outPutInternal,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+1;
        yOffset++;

        for(auto & modPart : menu.getVar("Original Item")->varItemPtr->internalitems)
        {
            sf::Vector2f vPos = vDrawPos;

            std::string outPut = "";
            outPut.append("        " +itemTypes.getTypeID(modPart.type).str + ": "+ modPart.name + "\n");
            vPos.y += 20*yOffset;
            shapes.createText(vPos,8,sf::Color::Cyan,outPut,&gvars::hudView);
            shapes.shapes.back().layer = layer+FrontPanel+1;

            int itemButt = shapes.createImageButton(vPos,texturemanager.getTexture("ArrowButton.png"),"",0,&gvars::hudView);
            shapes.shapes.back().layer = layer+FrontPanel+2;

            if(shapes.shapeClicked(itemButt))
            {
                menu.getVar("Item")->varItemPtr = &modPart;
            }
            if(shapes.shapeHovered(itemButt))
            {
                shapes.createText(sf::Vector2f(gvars::mousePos.x,gvars::mousePos.y-20),10,sf::Color::White,"Examine Part");
                shapes.shapes.back().layer = layer+FrontPanel+3;
            }



            vPos.x += 20;
            int equipSwapButt = shapes.createImageButton(vPos,texturemanager.getTexture("ArrowButton.png"),"",0,&gvars::hudView);
            vPos.x -= 20;
            shapes.shapes.back().layer = layer+FrontPanel+2;

            if(shapes.shapeClicked(equipSwapButt))
            {
                //menu.getVar("Item")->varItemPtr = &modPart;
                itemStorage.push_back(modPart);
                modPart.toDelete = true;
                AnyDeletes(menu.getVar("Original Item")->varItemPtr->internalitems);
                selectedItem->varItemPtr = menu.getVar("Original Item")->varItemPtr;
                break;
            }
            if(shapes.shapeHovered(equipSwapButt))
            {
                shapes.createText(sf::Vector2f(gvars::mousePos.x,gvars::mousePos.y-20),10,sf::Color::White,"Remove Part");
                shapes.shapes.back().layer = layer+FrontPanel+3;
            }


            yOffset++;
        }


        yOffset++;
        std::string outPutStorage = "        Items In Storage";
        sf::Vector2f vPosStorage = vDrawPos;
        vPosStorage.y += 20*yOffset;
        shapes.createText(vPosStorage,8,sf::Color::White,outPutStorage,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+1;
        yOffset++;

        for(auto & modPart : itemStorage)
        {
            sf::Vector2f vPos = vDrawPos;

            std::string outPut = "";
            outPut.append("        " + itemTypes.getTypeID(modPart.type).str + ": " + modPart.name + "\n");
            vPos.y += 20*yOffset;
            shapes.createText(vPos,8,sf::Color::White,outPut,&gvars::hudView);
            shapes.shapes.back().layer = layer+FrontPanel+1;

            int itemButt = shapes.createImageButton(vPos,texturemanager.getTexture("ArrowButton.png"),"",0,&gvars::hudView);
            shapes.shapes.back().layer = layer+FrontPanel+2;

            if(shapes.shapeClicked(itemButt))
            {
                menu.getVar("Item")->varItemPtr = &modPart;
            }
            if(shapes.shapeHovered(itemButt))
            {
                shapes.createText(sf::Vector2f(gvars::mousePos.x,gvars::mousePos.y-20),10,sf::Color::White,"Examine Part");
                shapes.shapes.back().layer = layer+FrontPanel+3;
            }



            vPos.x += 20;
            int equipSwapButt = shapes.createImageButton(vPos,texturemanager.getTexture("ArrowButton.png"),"",0,&gvars::hudView);
            vPos.x -= 20;
            shapes.shapes.back().layer = layer+FrontPanel+2;

            if(shapes.shapeClicked(equipSwapButt))
            {
                bool partAllowed = true;
                int accessoryCount = 0;
                for(auto compare : menu.getVar("Original Item")->varItemPtr->internalitems)
                {
                    if(compare.type == itemTypes.getTypeID("PartAccessory").num)
                        accessoryCount++;

                    if(modPart.type != itemTypes.getTypeID("PartAccessory").num && compare.type == modPart.type)
                    {
                        partAllowed = false;
                        chatBox.addChat("This item already has a part in this slot!");
                    }
                }

                if(accessoryCount >= 4 && modPart.type == itemTypes.getTypeID("PartAccessory").num)
                {
                    partAllowed = false;
                    chatBox.addChat("This item has too many accessories!");
                    if(random(1,4) == 1)
                        chatBox.addChat("Such fabulousness.", sf::Color::Green);
                }



                if(partAllowed)
                {
                    menu.getVar("Original Item")->varItemPtr->internalitems.push_back(modPart);
                    modPart.toDelete = true;
                    AnyDeletes(itemStorage);
                    selectedItem->varItemPtr = menu.getVar("Original Item")->varItemPtr;
                    break;
                }
            }
            if(shapes.shapeHovered(equipSwapButt))
            {
                shapes.createText(sf::Vector2f(gvars::mousePos.x,gvars::mousePos.y-20),10,sf::Color::White,"Install Part");
                shapes.shapes.back().layer = layer+FrontPanel+3;
            }


            yOffset++;
        }

    }



}

void renderItemHotBarRMBMenu(baseMenu &menu)
{ // Layer 24000
    int layer = 24000;
    enum
    {
        BackPanel,
        Shapes,
        Button,
        Text,
        FrontPanel
    };

    sf::Vector2f menuStartPos(577,500);
    sf::Vector2f menuEndPos(702,550);
    sf::Color menuColor(100,100,100,100);

    bool rmbCheck = (inputState.rmbTime == 1);
    bool lmbCheck = (inputState.lmbTime == 1);
    if( (menu.age > 10 && rmbCheck) || (menu.age > 10 && lmbCheck))
    { // Closing Menu Check

        bool mouseOutsideMenu = true;


        sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), gvars::hudView);

        if (
            aabb(worldPos, menuStartPos.x,
                 menuStartPos.x + menuEndPos.x - menuStartPos.x,
                 menuStartPos.y,
                 menuStartPos.y + menuEndPos.y - menuStartPos.y)
            )
            {
                mouseOutsideMenu = false;
            }


        if(mouseOutsideMenu)
        {
            menu.toDelete = true;
            return;
        }
    }


    shapes.createSquare(menuStartPos.x,menuStartPos.y,menuEndPos.x,menuEndPos.y,menuColor,1,sf::Color::Black,&gvars::hudView);
    shapes.shapes.back().layer = layer+BackPanel;

    /*
    sf::Vector2f upScrollButtPos(percentPos(10,menuStartPos.x,menuEndPos.x),percentPos(75,menuStartPos.y,menuEndPos.y));
    sf::Vector2f downScrollButtPos(percentPos(10,menuStartPos.x,menuEndPos.x),percentPos(85,menuStartPos.y,menuEndPos.y));
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

    */

    dynamicVariable * selectedItem = menu.getVar("Item");

    int xOffset = 0;
    int yOffset = 0;

    Item * weapon = selectedItem->varItemPtr;




    { // Put Item In Storage
        sf::Vector2f storeButtPos(menuStartPos.x+115,menuStartPos.y+10);
        int storeButt = shapes.createImageButton(storeButtPos,texturemanager.getTexture("ArrowButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+Button;

        bool canStore = false;
        if(bountytower::currentTower->name == towers[0].name) // TODO: || Npc On Balcony for Air Drops (Maybe) || Carrying Storage Portal
        {
            canStore = true;
        }

        if(weapon->user->isSquaddie && shapes.shapeHovered(storeButt))
        {
            if(canStore)
                textList.createText(gvars::mousePos.x+10,gvars::mousePos.y,10,sf::Color::White,"Put In Storage");
            else
                textList.createText(gvars::mousePos.x+10,gvars::mousePos.y,10,sf::Color::Red,"Put In Storage \n Can only be done in the tavern.");
            gvars::hovering += 3;
            if(inputState.lmbTime == 1 && canStore)
            {
                itemStorage.push_back(*weapon);
                weapon->remove();
                menu.toDelete = true;
                return;
            }

        }
    }




    if(weapon->type == itemTypes.getTypeID("Gun").num)
    {


        sf::Vector2f reloadButtPos(menuStartPos.x+10,menuStartPos.y+10);
        int reloadButt = shapes.createImageButton(reloadButtPos,texturemanager.getTexture("ArrowButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+Button;

        sf::Vector2f modButtPos(menuStartPos.x+31,menuStartPos.y+10);
        int modButt = shapes.createImageButton(modButtPos,texturemanager.getTexture("ArrowButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+Button;


        // Next Row


        sf::Vector2f unloadButtPos(menuStartPos.x+10,menuStartPos.y+40);
        int unloadButt = shapes.createImageButton(unloadButtPos,texturemanager.getTexture("ArrowButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+Button;

        // --

        sf::Vector2f cycleButtPos(menuStartPos.x+52,menuStartPos.y+40);
        int cycleButt = shapes.createImageButton(cycleButtPos,texturemanager.getTexture("ArrowButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+Button;

        sf::Vector2f repairButtPos(menuStartPos.x+73,menuStartPos.y+40);
        int repairButt = shapes.createImageButton(repairButtPos,texturemanager.getTexture("ArrowButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+Button;



        if(weapon->user->isSquaddie && shapes.shapeHovered(reloadButt))
        {
            textList.createText(gvars::mousePos.x+10,gvars::mousePos.y,10,sf::Color::White,"Reload");
            gvars::hovering += 3;
            if(inputState.lmbTime)
                checkAmmo(*weapon->user, *weapon->user->container, weapon, true);
        }

        if(weapon->user->isSquaddie && shapes.shapeHovered(unloadButt))
        {
            textList.createText(gvars::mousePos.x+10,gvars::mousePos.y,10,sf::Color::White,"Unload");
            gvars::hovering += 3;
            if(inputState.lmbTime == 1)
            {
                unloadAmmo(weapon, &weapon->user->inventory);
            }
        }

        if(weapon->user->isSquaddie && shapes.shapeHovered(modButt))
        {
            textList.createText(gvars::mousePos.x+10,gvars::mousePos.y,10,sf::Color::White,"Mod Weapon");
            gvars::hovering += 3;
            if(inputState.lmbTime == 1)
            {
                gunModMenu(weapon);
                menu.toDelete = true;
                return;
            }
        }

        if(weapon->user->isSquaddie && shapes.shapeHovered(cycleButt))
        {
            std::string modeText;
            if(weapon->fireMode == 0)
                modeText = "Semi";
            if(weapon->fireMode == 1)
                modeText = "Burst";
            if(weapon->fireMode == 2)
                modeText = "Auto";


            textList.createText(gvars::mousePos.x+10,gvars::mousePos.y,10,sf::Color::White,"("+modeText+") Cycle Fire Mode");
            gvars::hovering += 3;
            if(inputState.lmbTime == 1)
            {

                cycleFireMode(weapon);
            }
        }

        if(weapon->user->isSquaddie && shapes.shapeHovered(repairButt))
        {
            std::string modeText;



            int scrapCost = 30; // TODO: Make a repair skill that cheapens this cost.
            bool enoughScrap = false;
            Item * scrapPtr = nullptr;

            for(auto &item : weapon->user->inventory)
            {
                if(item.name == "Scrap" && item.amount >= scrapCost)
                {
                    enoughScrap = true;
                    scrapPtr = &item;
                }
            }







            textList.createText(gvars::mousePos.x+10,gvars::mousePos.y,10,sf::Color::White,"Repair Weapon: " + str(scrapCost) + " Scrap");
            gvars::hovering += 3;
            if(inputState.lmbTime == 1)
            {

                if(weapon->durabilityCounter >= weapon->getDurability())
                {
                    chatBox.addChat(weapon->name + " is fully repaired!");
                }
                else if(enoughScrap)
                {

                    if(scrapPtr != nullptr)
                    {
                        scrapPtr->amount -= scrapCost;
                        if(scrapPtr->amount <= 0)
                            scrapPtr->remove();
                    }

                    weapon->durabilityCounter = weapon->getDurability();
                    chatBox.addChat(weapon->name + " has been fully repaired!");
                }
                else
                    chatBox.addChat(weapon->user->name + " needs more scrap to fix " + weapon->name);

            }
        }





    }

    if(weapon->type == itemTypes.getTypeID("Consumable").num)
    {
        sf::Vector2f healButtPos(menuStartPos.x+10,menuStartPos.y+10);
        int healButt = shapes.createImageButton(healButtPos,texturemanager.getTexture("ArrowButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+Button;

        if(shapes.shapeHovered(healButt))
        {
            textList.createText(gvars::mousePos.x+10,gvars::mousePos.y,10,sf::Color::White,"Drink Juices");
            gvars::hovering += 3;
            if(inputState.lmbTime == 1)
            {
                if(weapon->user->functional() == false)
                    chatBox.addChat("He's dead, Jim.", sf::Color::Green);
                else
                {
                    weapon->activate(weapon->user->getPos());
                    chatBox.addChat(weapon->user->name + " used " + weapon->name + "!", sf::Color::Green);
                    weapon->remove();
                    menu.toDelete = true;
                    return;
                }
            }
        }


    }

    if(weapon->type == itemTypes.getTypeID("InventorySlotUnlocker").num)
    {

        sf::Vector2f invButtPos(menuStartPos.x+10,menuStartPos.y+10);
        int invButt = shapes.createImageButton(invButtPos,texturemanager.getTexture("ArrowButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+Button;

        if(shapes.shapeHovered(invButt))
        {
            textList.createText(gvars::mousePos.x+10,gvars::mousePos.y,10,sf::Color::White,"Gain 2 Slots(22 Maximum)");
            gvars::hovering += 3;
            if(inputState.lmbTime == 1)
            {
                if(weapon->user->functional() == false)
                    chatBox.addChat("He's dead, Jim.", sf::Color::Green);
                else if(weapon->user->getInventoryMax() >= 22)
                {
                    chatBox.addChat(weapon->user->name + " is cannot have anymore slots!", sf::Color::Green);
                    chatBox.addChat("Get another workhorse! Maybe a Noirve?", sf::Color::Green);
                }
                else
                {
                    chatBox.addChat(weapon->user->name + " used " + weapon->name + "!", sf::Color::Green);
                    weapon->user->additionalSlots += 2;
                    chatBox.addChat(weapon->user->name + " now has " + str(weapon->user->getInventoryMax()) + " slots!", sf::Color::Green);
                    weapon->remove();
                    menu.toDelete = true;
                    return;
                }
            }
        }
    }









}

void renderStorageMenu(baseMenu &menu)
{ // Layer 25000
    int layer = 25000;
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

    shapes.createSquare(percentPos(12,menuStartPos.x,menuEndPos.x),percentPos(60,menuStartPos.y,menuEndPos.y),menuEndPos.x,menuEndPos.y,sf::Color::Black,0,sf::Color::White,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel;
    //Close Button
    int exitButt = shapes.createImageButton(sf::Vector2f(screen.x()-100,100),texturemanager.getTexture("ExitButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel+1;
    if(shapes.shapeClicked(exitButt))
        menu.toDelete = true;


    sf::Vector2f upScrollButtPos(percentPos(10,menuStartPos.x,menuEndPos.x),percentPos(75,menuStartPos.y,menuEndPos.y));
    sf::Vector2f downScrollButtPos(percentPos(10,menuStartPos.x,menuEndPos.x),percentPos(85,menuStartPos.y,menuEndPos.y));
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


    dynamicVariable * currentType = menu.getVar("currentType");
    { // Current Item Type Selection
        if(!menu.hasVar("currentType"))
        {
            dynamicVariable var;
            var.name = "currentType";
            var.varString = "All";

            menu.vars.push_back(var);
        }
        if(inputState.key[Key::LShift])
            std::cout << "Menu: " << menu.name << " has " << menu.vars.size() << " dynamic vars. \n";
        int yOffset = 0;


        // Types: All, Weapons, Ammo, Aid, Misc
        const int totalTypes = 7;
        std::string types[totalTypes] = {"All", "Weapons", "Ammo", "Magic", "Engineer", "Aid", "Misc"};

        for(int i = 0; i != totalTypes; i++)
        {
            sf::Vector2f drawPos(152,150+(yOffset*60));
            int skillTreeButt = shapes.createImageButton(drawPos,texturemanager.getTexture("blankLargeButton.png"),"",0,&gvars::hudView);
            shapes.shapes.back().layer = layer+75;


            if(shapes.shapeClicked(skillTreeButt))
            {
                menu.scrollOne = 0;
                if(currentType != nullptr)
                    currentType->varString = types[i];
            }
            sf::Color highlightColor = sf::Color::White;
            if(shapes.shapeHovered(skillTreeButt))
                highlightColor = sf::Color::Cyan;

            sf::Vector2f textPos(drawPos.x-45, drawPos.y);
            shapes.createText(textPos,10,highlightColor,types[i],&gvars::hudView);
            shapes.shapes.back().layer = layer+76;

            yOffset++;
        }
    }

    dynamicVariable * selectedItem = menu.getVar("selectedItem");

    int xOffset = 0;
    int yOffset = 0;


    for(auto &item : itemStorage)
    {
        if(currentType == nullptr)
            continue;

        if(currentType->varString != "All")
        {
            if(item.type == 1 || item.type == 2)
            {
                if(currentType->varString != "Weapons")
                    continue;
            }
            else if(item.type == 3 || item.type == 4 || item.type == 5)
            {
                if(currentType->varString != "Ammo")
                    continue;
            }
            else if(item.type == 23)
            {
                if(currentType->varString != "Magic")
                    continue;
            }
            else if(item.type == 12)
            {
                if(currentType->varString != "Engineer")
                    continue;
            }
            else if(item.type == 42)
            {
                if(currentType->varString != "Aid")
                    continue;
            }
            else if(item.type == 69)
            {
                if(currentType->varString != "Misc")
                    continue;
            }
            else
                continue;

        }


        int posX = 250+(xOffset*300);
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
            stats.append(", Mag: " + str(item.getAmmoCapacity()));
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
        shapes.createText(vPos,10,highlightColor,stats,&gvars::hudView);
        shapes.shapes.back().layer = layer+Text;



        if(shapes.shapeClicked(itemButt) && menu.age > 30)
        {
            if(selectedItem == nullptr)
            {
                dynamicVariable var;
                var.name = "selectedItem";
                var.varItemPtr = &item;
                menu.vars.push_back(var);
                selectedItem = &menu.vars.back();
            }
            else
            {
                selectedItem->varItemPtr = &item;
            }



            /*

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

            */
        }





        xOffset++;
        if(xOffset > 1)
        {
            xOffset = 0;
            yOffset++;
        }
    }

    if(selectedItem == nullptr)
        return;

    Item * item = selectedItem->varItemPtr;

    sf::Vector2f statusPos(percentPos(12,menuStartPos.x,menuEndPos.x),percentPos(65,menuStartPos.y,menuEndPos.y));
    shapes.createText(statusPos,10,sf::Color::White,item->name + ", Total Status Effects: " + std::to_string(item->statusEffects.size()+item->statusEffectsInflict.size()+item->statusEffectsCarried.size()),&gvars::hudView);
    shapes.shapes.back().layer = layer+FrontPanel+1;



    { // Withdraw Item Button
        sf::Vector2f withdrawItemPos(percentPos(15,menuStartPos.x,menuEndPos.x), percentPos(62,menuStartPos.y,menuEndPos.y));
        int withdrawItemButt = shapes.createImageButton(withdrawItemPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Button;
        withdrawItemPos.x -= 28;
        withdrawItemPos.y -= 10;
        textList.createText(withdrawItemPos,9,sf::Color::White,"Withdraw \nItem",gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Text;

        if(shapes.shapeHovered(withdrawItemButt))
        {
            textList.createText(gvars::mousePos,10,sf::Color::White,"   Withdraws item from storage.");
            shapes.shapes.back().layer = 99999999;

            if(inputState.lmbTime == 1)
            {
                Item withdrawn = *item;
                withdrawn.xpos = menu.makePos.x;
                withdrawn.ypos = menu.makePos.y;
                withdrawn.zpos = menu.makePos.z;

                withdrawn.slotted = false;


                worlditems.push_back(withdrawn);
                item->toDelete = true;
                selectedItem->toDelete = true;
                selectedItem = nullptr;
                AnyDeletes(menu.vars);
                AnyDeletes(itemStorage);
                //menu.toDelete = true;
                return;

            }
        }
    }

    /*
    {   // Remove Last Status Effect
        sf::Vector2f randomEnchantPos(percentPos(21,menuStartPos.x,menuEndPos.x), percentPos(62,menuStartPos.y,menuEndPos.y));
        int randomEnchantButt = shapes.createImageButton(randomEnchantPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Button;
        randomEnchantPos.x -= 28;
        randomEnchantPos.y -= 10;
        textList.createText(randomEnchantPos,9,sf::Color::White,"Remove \nSelf",gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Text;

        if(shapes.shapeHovered(randomEnchantButt))
        {
            int removeEnchantCost = getSquadDiscount(250);
            textList.createText(gvars::mousePos,10,sf::Color::White,"   $"+str(removeEnchantCost)+", Removes the last Self Enchant on the item.");
            shapes.shapes.back().layer = 99999999;

            if(inputState.lmbTime == 1)
            {
                if(item->statusEffects.empty())
                    chatBox.addChat("There's no Self Enchants to remove!",sf::Color::White);
                else if(conFact->credits < removeEnchantCost)
                    chatBox.addChat("You do not have enough money!",sf::Color::White);
                else
                {
                    soundmanager.playSound("enchantSound.ogg");
                    conFact->credits -= removeEnchantCost;
                    item->statusEffects.back().toDelete = true;
                    AnyDeletes(item->statusEffects);


                }
            }
        }
    }
    {   // Remove Last Status Effect Inflict
        sf::Vector2f randomEnchantPos(percentPos(27,menuStartPos.x,menuEndPos.x), percentPos(62,menuStartPos.y,menuEndPos.y));
        int randomEnchantButt = shapes.createImageButton(randomEnchantPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Button;
        randomEnchantPos.x -= 28;
        randomEnchantPos.y -= 10;
        textList.createText(randomEnchantPos,9,sf::Color::White,"Remove \nInflict",gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Text;

        if(shapes.shapeHovered(randomEnchantButt))
        {
            int removeEnchantCost = getSquadDiscount(250);
            textList.createText(gvars::mousePos,10,sf::Color::White,"   $"+str(removeEnchantCost)+", Removes the last Inflict Enchant on the item.");
            shapes.shapes.back().layer = 99999999;

            if(inputState.lmbTime == 1)
            {
                if(item->statusEffectsInflict.empty())
                    chatBox.addChat("There's no Inflict Enchants to remove!",sf::Color::White);
                else if(conFact->credits < removeEnchantCost)
                    chatBox.addChat("You do not have enough money!",sf::Color::White);
                else
                {
                    soundmanager.playSound("enchantSound.ogg");
                    conFact->credits -= removeEnchantCost;
                    item->statusEffectsInflict.back().toDelete = true;
                    AnyDeletes(item->statusEffectsInflict);

                }
            }
        }
    }
    {   // Swap Last Self Enchant
        sf::Vector2f randomEnchantPos(percentPos(33,menuStartPos.x,menuEndPos.x), percentPos(62,menuStartPos.y,menuEndPos.y));
        int randomEnchantButt = shapes.createImageButton(randomEnchantPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Button;
        randomEnchantPos.x -= 28;
        randomEnchantPos.y -= 10;
        textList.createText(randomEnchantPos,9,sf::Color::White,"Swap \nSelf",gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Text;

        if(shapes.shapeHovered(randomEnchantButt))
        {
            int removeEnchantCost = getSquadDiscount(250);
            textList.createText(gvars::mousePos,10,sf::Color::White,"   $"+str(removeEnchantCost)+", Turns last Self enchant into an Inflict enchant.");
            shapes.shapes.back().layer = 99999999;

            if(inputState.lmbTime == 1)
            {
                if(item->statusEffects.empty())
                    chatBox.addChat("There's no Self Enchants to move!",sf::Color::White);
                else if(conFact->credits < removeEnchantCost)
                    chatBox.addChat("You do not have enough money!",sf::Color::White);
                else
                {

                    conFact->credits -= removeEnchantCost;
                    item->statusEffectsInflict.push_back(item->statusEffects.back());
                    item->statusEffectsInflict.back().duration = 180;
                    item->statusEffects.back().toDelete = true;
                    AnyDeletes(item->statusEffects);

                }
            }
        }
    }
    {   // Swap Last Inflict Enchant
        sf::Vector2f randomEnchantPos(percentPos(39,menuStartPos.x,menuEndPos.x), percentPos(62,menuStartPos.y,menuEndPos.y));
        int randomEnchantButt = shapes.createImageButton(randomEnchantPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Button;
        randomEnchantPos.x -= 28;
        randomEnchantPos.y -= 10;
        textList.createText(randomEnchantPos,9,sf::Color::White,"Swap \nInflict",gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+Text;

        if(shapes.shapeHovered(randomEnchantButt))
        {
            int removeEnchantCost = getSquadDiscount(250);
            textList.createText(gvars::mousePos,10,sf::Color::White,"   $"+str(removeEnchantCost)+", Turns last Inflict enchant into a self enchant.");
            shapes.shapes.back().layer = 99999999;

            if(inputState.lmbTime == 1)
            {
                if(item->statusEffectsInflict.empty())
                    chatBox.addChat("There's no Inflict Enchants to remove!",sf::Color::White);
                else if(conFact->credits < removeEnchantCost)
                    chatBox.addChat("You do not have enough money!",sf::Color::White);
                else
                {
                    conFact->credits -= removeEnchantCost;
                    item->statusEffects.push_back(item->statusEffectsInflict.back());
                    item->statusEffects.back().duration = 1;
                    item->statusEffectsInflict.back().toDelete = true;
                    AnyDeletes(item->statusEffectsInflict);

                }
            }
        }
    }

    */


    yOffset = 1;
    for(auto &status : item->statusEffects)
    {
        std::string outPut = "Equip:";
        outPut.append("[" + status.name + ", Duration: " + str(status.duration) + "f]");
        for(auto &aspect : status.aspects)
        {
            if(aspectNum[aspect.name].find("Condition") != std::string::npos)
                outPut.append("\n   ");
            outPut.append("[" + aspectNum[aspect.name] + ",Potency:" + str(static_cast<int>(aspect.potency)));
            if(aspect.type != "")
                outPut.append(",Type:" + aspect.type);
            outPut.append("]");
        }

        sf::Vector2f vPos = statusPos;
        vPos.y += 10*yOffset;
        shapes.createText(vPos,8,sf::Color::Cyan,outPut,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+1;

        yOffset++;
        yOffset++;
        yOffset++;
    }
    for(auto &status : item->statusEffectsInflict)
    {
        std::string outPut = "Inflict:";

        outPut.append("[" + status.name + ", Duration: " + str(status.duration) + "f]");
        for(auto &aspect : status.aspects)
        {
            if(aspectNum[aspect.name].find("Condition") != std::string::npos)
                outPut.append("\n   ");
            outPut.append("[" + aspectNum[aspect.name] + ",Potency:" + str(static_cast<int>(aspect.potency)));
            if(aspect.type != "")
                outPut.append(",Type:" + aspect.type);
            outPut.append("]");
        }

        sf::Vector2f vPos = statusPos;
        vPos.y += 10*yOffset;
        shapes.createText(vPos,8,sf::Color(255,150,150),outPut,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+1;

        yOffset++;
        yOffset++;
        yOffset++;
    }
    for(auto &status : item->statusEffectsCarried)
    {
        std::string outPut = "Carry:";

        outPut.append("[" + status.name + ", Duration: " + str(status.duration) + "f]");
        for(auto &aspect : status.aspects)
        {
            if(aspectNum[aspect.name].find("Condition") != std::string::npos)
                outPut.append("\n   ");
            outPut.append("[" + aspectNum[aspect.name] + ",Potency:" + str(static_cast<int>(aspect.potency)));
            if(aspect.type != "")
                outPut.append(",Type:" + aspect.type);
            outPut.append("]");
        }

        sf::Vector2f vPos = statusPos;
        vPos.y += 10*yOffset;
        shapes.createText(vPos,8,sf::Color::Yellow,outPut,&gvars::hudView);
        shapes.shapes.back().layer = layer+FrontPanel+1;

        yOffset++;
        yOffset++;
        yOffset++;
    }
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
        if(menu.name == "Merchant Modder Menu")
            renderMerchantModderMenu(menu);
        if(menu.name == "Recruitment Menu")
            renderRecruiterMenu(menu);
        if(menu.name == "Tower Menu")
            renderTowerMenu(menu);
        if(menu.name == "Escape Menu")
            renderEscapeMenu(menu);
        if(menu.name == "Enchant Menu")
            renderEnchantMenu(menu);
        if(menu.name == "Gun Mod Menu")
            renderGunModMenu(menu);
        if(menu.name == "Item Hotbar RMB Menu")
            renderItemHotBarRMBMenu(menu);
        if(menu.name == "Storage Menu")
            renderStorageMenu(menu);

        if(inputState.key[Key::Escape] && menu.age > 30)
            menu.toDelete = true;

    }
    window.setView(gvars::view1);
}

int getLivingFactionMemberCount(std::string faction)
{
    int livingCount = 0;
    for(auto &npc : npclist)
        if(npc.faction == faction && npc.functional())
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
    Npc boss;

    if(bountytower::currentTower->name == "FantasyModern")
    {
        boss = *getGlobalCritter("Rockkid");
        //Stats
        boss.level = 10;
        boss.maxhealth = 10000;
        boss.health = 10000;
        boss.name = "The Hardened Criminal";
        //Equipment
        Item weaponry = *getGlobalItem("Shotgun");
        Item ammo = *getGlobalItem("Shell Spread");
        ammo.amount = 10000;
        weaponry.internalitems.push_back(ammo);

        boss.inventory.push_back(weaponry);
        boss.inventory.push_back(weaponry);

        //Skills
        boss.skills.getSkill("Feral Body")->ranks = 2;
    }

    if(bountytower::currentTower->name == "Necromancer Tower")
    {
        boss = *getGlobalCritter("Samurai");
        //Stats
        boss.level = 10;
        boss.maxhealth = 10000;
        boss.health = 10000;
        boss.name = "Necromancer Samurai";
        //Equipment
        Item weaponry = *getGlobalItem("Sword");

        boss.inventory.push_back(weaponry);
        boss.inventory.push_back(weaponry);

        //Skills
        boss.skills.getSkill("Vampirism")->ranks = 5;
        boss.skills.getSkill("Undead Dread")->ranks = 5;
        boss.skills.getSkill("Cleave")->ranks = 5;
    }

    if(bountytower::currentTower->name == "Powerhouse Tower")
    {
        boss = *getGlobalCritter("Psylin");
        //Stats
        boss.level = 10;
        boss.maxhealth = 10000;
        boss.health = 10000;
        boss.name = "Cyborg Mega Dron";
        //Equipment
        Item weaponry = *getGlobalItem("Sniper");
        Item ammo = *getGlobalItem("Bullet Armor Piercing");
        ammo.amount = 10000;
        weaponry.internalitems.push_back(ammo);

        boss.inventory.push_back(weaponry);

        //Skills
        boss.skills.getSkill("Snipe Shot")->ranks = 2;
        boss.skills.getSkill("Feral Regeneration")->ranks = 5;
    }


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



    boss.faction = "Towerlings";
    boss.factionPtr = &listAt(uniFact,2);



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
    worlditems.clear();
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
    {
        npc.momentum = sf::Vector2f(0,0);
        npc.draggedBy = nullptr;
        npc.dragging = nullptr;
    }


    Npc barPatron = *getGlobalCritter("Human");
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

    barPatron.name = "The Enchanter";
    barPatron.xpos = 2010;
    barPatron.ypos = 3690;
    barPatron.tags = backupTags;
    barPatron.tags.append("[Enchanter:1]");
    npclist.push_back(barPatron);

    barPatron.name = "The Modder";
    barPatron.xpos = 2330;
    barPatron.ypos = 2970;
    barPatron.tags = backupTags;
    barPatron.tags.append("[Modder:1]");
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

        npc.draggedBy = nullptr;
        npc.dragging = nullptr;

        npc.chaseDefendPos = Vec3(npc.xpos,npc.ypos,npc.zpos);
        //!npc.alive
        if(gvars::currentz != abs_to_index(npc.zpos/GRID_SIZE))
        {
            clearSlots(npc);
            leftBehind.push_back(npc);
            npc.toDelete = true;
        }
    }
    for(auto &item : worlditems)
        if(gvars::currentz != abs_to_index(item.zpos/GRID_SIZE))
            item.toDelete = true;



    clearBullets();
    shapes.shapes.clear();

    if(gvars::currentz > bountytower::currentTower->floors)
        loadTavern();

    for(int i = 0; i != 30; i++)
    {
        shapes.createSquare(0,0,1400,1400,sf::Color(0,0,0,25),0,sf::Color::Transparent,&gvars::hudView);
        shapes.shapes.back().duration = 60-i;
        shapes.shapes.back().layer = 10000000;

    }


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

    if(!menus.empty())
        for(auto menu : menus)
            if(menu.name == "Escape Menu")
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
        if(bountytower::currentTower->name == "FantasyModern")
        { //Add a 12.5% chance for a critter to spawn on the first tower.
            if(randz(1,16) != 1)
                continue;
        }
        else if(randz(1,4) != 1)
            continue; //Add a 25% chance for a critter to spawn.



        Npc member;
        debug("V");

        RandomWeightList RandomEnemy;
        RandomEnemy.addEntry("BlankBody",10000);
        if(bountytower::currentTower->name == "FantasyModern")
        {
            RandomEnemy.addEntry("Ogre",2500);
            RandomEnemy.addEntry("ArmoredOgre",1000);
            RandomEnemy.addEntry("Goblin",10000);
        }
        if(bountytower::currentTower->name == "Necromancer Tower")
        {
            RandomEnemy.addEntry("Skeleton",2500);
            RandomEnemy.addEntry("SkeletonArmored",1000);
            RandomEnemy.addEntry("Rat",5000);
        }
        if(bountytower::currentTower->name == "Powerhouse Tower")
        {
            RandomEnemy.addEntry("SimpleRobot",2500);
            RandomEnemy.addEntry("DroneHover",1000);
            RandomEnemy.addEntry("DroneSpider",2500);
            RandomEnemy.addEntry("DroneTank",100);
        }

        std::string chosenRace = RandomEnemy.getRandomName();
        member = *getGlobalCritter(chosenRace);

        { // Status Effects/Templates
            StatusEffect status;
            status.duration = 1000000000;
            bool giveTemplate = false;
            if(chosenRace == "Skeleton" || chosenRace == "SkeletonArmored")
            {
                giveTemplate = true;
                status.name = "Skeleton Template";
                status.addAspect(StatusAspect::Armor,75,"Pierce");
                status.addAspect(StatusAspect::Armor,200,"Energy");
                status.addAspect(StatusAspect::Armor,100,"Poison");
                status.addAspect(StatusAspect::Armor,100,"Cold");
            }
            if(chosenRace == "SkeletonArmored" || chosenRace == "ArmoredOgre")
            {
                giveTemplate = true;
                status.addAspect(StatusAspect::Armor,50,"Blunt");
            }

            if(chosenRace == "DroneHover" || chosenRace == "DroneSpider" || chosenRace == "DroneTank" || chosenRace == "SimpleRobot")
            {
                giveTemplate = true;
                status.name = "Drone Template";
                status.addAspect(StatusAspect::Armor,-100,"Electricity");
                status.addAspect(StatusAspect::Armor,50,"Pierce");
                status.addAspect(StatusAspect::ConditionOnDeath,1,"");
                status.addAspect(StatusAspect::CauseExplosion,100,"");

            }
            if(giveTemplate)
                member.statusEffects.push_back(status);
        }

        //member = *getGlobalCritter("BlankBody");
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

        { // Skills.
            int totalCount = member.level; // We copy the number seperately so we can keep iterating later on in case of a max skill.
            for(int i = 0; i != totalCount; i++)
            {
                int randomSkill = randz(0,member.skills.list.size()-1);
                Skill & npcSkill = listAt(member.skills.list,randomSkill);
                if(npcSkill.ranks >= npcSkill.ranksmax)
                { // Max skill detected, Looping once more instead.
                    totalCount++;
                    continue;
                }
                if(npcSkill.name == "Sunder")
                { // Disallow NPC's to have Sunder.
                    totalCount++;
                    continue;
                }
                if(npcSkill.name == "Hamstring")
                { // Disallow NPC's to have Hamstring.
                    totalCount++;
                    continue;
                }
                npcSkill.ranks++;
            }
        }

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

        if(member.tags.find("Humanoid:1") != std::string::npos)
        {
            if(random(1,5) == 1)
                member.tags.append("[WearsBoots:1]");
            if(random(1,5) == 1)
                member.tags.append("[WearsGloves:1]");
        }


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
{ // Layer 13000
    int layer = 13000;
    enum
    {
        BackPanel,
        Shapes,
        Button,
        Text,
        FrontPanel
    };
    shapes.createSquare(0,0,screen.x(),screen.y(),sf::Color(0,0,0,100),0,sf::Color::Transparent,&gvars::hudView);
    shapes.shapes.back().layer = layer+BackPanel;
    sf::Vector2f textPos(screen.x()/2,50);
    shapes.createText(textPos,10,sf::Color::Yellow,"$ Bounty Killed! $ \n $ Time to collect! $",&gvars::hudView);
    shapes.shapes.back().layer = layer+Text;
    textPos.y += 30;
    shapes.createText(textPos,10,sf::Color::White,"The people who got in your way.",&gvars::hudView);
    shapes.shapes.back().layer = layer+Text;

    textPos.x -= 100;

    int returnButt = shapes.createImageButton(textPos,texturemanager.getTexture("returnButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = layer+Button;
    if(shapes.shapeClicked(returnButt))
    {
        saveGame("Profile1");
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
            Npc corpse = *getGlobalCritter("BlankBody");
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
            if(!npc.functional())
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
    //shapes.createText(gvars::mousePos,10,sf::Color::White,"Click to begin!");

    shapes.createText(10,screen.y()-80,10,sf::Color(255,255,255),"Made by Johnny Fojtik(Dehaku Zedon)");
    shapes.createText(10,screen.y()-60,10,sf::Color(255,255,255),"Contact: dehakuzedon@gmail.com  or reddit.com/r/bountytower");
    shapes.createText(10,screen.y()-40,10,sf::Color(255,255,255),"Can't stand some of the art in this game? \nCan you do better? Contact me! I'm looking for an artist!");

    //if(shapes.shapeClicked(startBut))
    //    loadTavern();


    sf::Vector2f newPos(screen.x()/2,screen.y()/2);
    int newButt = shapes.createImageButton(newPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = 15050;
    newPos.x -= 31;
    textList.createText(newPos,9,sf::Color::White,"New Game",gvars::hudView);
    shapes.shapes.back().layer = 15075;

    sf::Vector2f loadPos(screen.x()/2,screen.y()/2+90);
    int loadButt = shapes.createImageButton(loadPos,texturemanager.getTexture("blankButton.png"),"",0,&gvars::hudView);
    shapes.shapes.back().layer = 15050;
    loadPos.x -= 31;
    textList.createText(loadPos,9,sf::Color::White,"Load Game",gvars::hudView);
    shapes.shapes.back().layer = 15075;

    if(shapes.shapeClicked(newButt))
    {
        soundmanager.playSound("Startup.wav");
        loadTavern();
    }


    if(shapes.shapeClicked(loadButt))
    {
        soundmanager.playSound("Startup.wav");
        loadGame("Profile1");
        loadTavern();
    }
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
                shapes.createText(gvars::mousePos,15,sf::Color::Yellow,"    Ready to go bounty hunting? \n(Left Mouse Button)");
            if(shapes.shapeClicked(bountyButt))
                towerMenu(Vec3());
        }
        else if(npc.tags.find("[WeaponDealer:1]") != npc.tags.npos)
        {
            int dealerButt = shapes.createImageButton(npc.getPos2d(),texturemanager.getTexture("SelectionCircle.png"));
            if(shapes.shapeHovered(dealerButt))
                shapes.createText(gvars::mousePos,15,sf::Color::Yellow,"    Wanna see my gear? \n(Left Mouse Button)");
            if(shapes.shapeClicked(dealerButt))
                merchantMenu(npc.getPos());

        }
        else if(npc.tags.find("[Recruiter:1]") != npc.tags.npos)
        {
            int dealerButt = shapes.createImageButton(npc.getPos2d(),texturemanager.getTexture("SelectionCircle.png"));
            if(shapes.shapeHovered(dealerButt))
                shapes.createText(gvars::mousePos,15,sf::Color::Yellow,"    Looking for some fresh meat? \n(Left Mouse Button)");
            if(shapes.shapeClicked(dealerButt))
                recruiterMenu(npc.getPos());

        }
        else if(npc.tags.find("[Enchanter:1]") != npc.tags.npos)
        {
            int dealerButt = shapes.createImageButton(npc.getPos2d(),texturemanager.getTexture("SelectionCircle.png"));
            if(shapes.shapeHovered(dealerButt))
                shapes.createText(gvars::mousePos,15,sf::Color::Yellow,"    You want something that'll make you a legend? \n(Left Mouse Button)");
            if(shapes.shapeClicked(dealerButt))
                enchantMenu(npc.getPos());

        }
        else if(npc.tags.find("[Modder:1]") != npc.tags.npos)
        {
            int dealerButt = shapes.createImageButton(npc.getPos2d(),texturemanager.getTexture("SelectionCircle.png"));
            if(shapes.shapeHovered(dealerButt))
                shapes.createText(gvars::mousePos,15,sf::Color::Yellow,"    Nothing better than a fresh change. \n(Left Mouse Button)");
            if(shapes.shapeClicked(dealerButt))
                merchantModderMenu(npc.getPos());

        }


        if(bountytower::currentTower != nullptr && bountytower::currentTower->name == towers[0].name)
        {
            if(gvars::currentz == 1)
            {
                sf::Vector2f storagePos(1950,3090);
                int storageButt = shapes.createImageButton(storagePos,texturemanager.getTexture("StorageChest.png"));
                if(shapes.shapeHovered(storageButt))
                    shapes.createText(gvars::mousePos,15,sf::Color::Yellow,"    Item Storage \n(Left Mouse Button)");

                Vec3f storageDropPos(storagePos.x+GRID_SIZE,storagePos.y+random(-30,30),gvars::currentz*GRID_SIZE);
                if(shapes.shapeClicked(storageButt))
                    storageMenu(storageDropPos);
            }
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
                            " They will flood from the dropchutes until they're sufficiently scared!");
                    shapes.shapes.back().layer += 1;
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
                    shapes.shapes.back().layer += 1;
                }

                textPos = sf::Vector2f(2790,3920);
                int tutBook3 = shapes.createImageButton(sf::Vector2f(textPos.x,textPos.y+timeHover),texturemanager.getTexture("tutBook.png"));
                if(shapes.shapeHovered(tutBook3))
                {
                    textPos = sf::Vector2f(textPos.x+20,textPos.y-10);
                    shapes.createSquare(textPos.x-5,textPos.y,textPos.x+400,textPos.y+60,sf::Color::Black,3,sf::Color::Cyan);
                    shapes.createText(textPos,15,sf::Color::Red,"Your goal? Fix the elevator! \n"
                            "(White Tiles in the Middle) \nFind Switches, then head up!");
                    shapes.shapes.back().layer += 1;
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
                    shapes.shapes.back().layer += 1;
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
                    shapes.shapes.back().layer += 1;
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
                    shapes.createText(textPos,15,sf::Color::Red,"At any time you can use the escape menu to return to the tavern. \n"
                                                                "The boss is right there though, You wouldn't disappoint me, Right?");
                    shapes.shapes.back().layer += 1;
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
                shapes.shapes.back().layer += 1;
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
                shapes.shapes.back().layer += 1;
            }

            textPos = sf::Vector2f(2250,2970);
            int tutBook3 = shapes.createImageButton(sf::Vector2f(textPos.x,textPos.y+timeHover),texturemanager.getTexture("tutBook.png"));
            if(shapes.shapeHovered(tutBook3))
            {
                textPos = sf::Vector2f(textPos.x+20,textPos.y-10);
                shapes.createSquare(textPos.x-5,textPos.y,textPos.x+850,textPos.y+50,sf::Color::Black,3,sf::Color::Cyan);
                shapes.createText(textPos,15,sf::Color::Red,"Right Click to pickup items with a selected squaddie(Must be near item.) \n"
                                "You can also drop items by right clicking near the squaddie with an item from their hotbar.");
                shapes.shapes.back().layer += 1;
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
                                "Dexterity, gives 0.5% dodge chance, per point.\n"
                                );
                shapes.shapes.back().layer += 1;
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
    AnyDeletes(worlditems);
    setupSquadHotKeySelection();
    saveGame("Profile1");
    generateHiddenGoodies();
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
            if(!npc.functional())
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
        if(npc.getRace() == "Turret")
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
        con("Squaddies: " + std::to_string(Squaddies.size()));
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
        if(!npc.functional())
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
            if(status.name == "Haste")
                testStatus = status;

        Item statusItem = *getGlobalItem("Bullet Incinderary");
        statusItem.xpos = gvars::mousePos.x;
        statusItem.ypos = gvars::mousePos.y;
        statusItem.zpos = gvars::currentz*GRID_SIZE;

        testStatus.duration = 1; // Doing this here so it's a one time process, rather than every time it's tossed on someone.
        statusItem.statusEffectsCarried.push_back(testStatus);

        //statusItem.name = "Fire Gun";
        statusItem.amount = 1;

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

void enchantGlow()
{
    float cosWave = cos(fpsKeeper.startTime.getElapsedTime().asSeconds());
    cosWave = 1 - cosWave;
    for(auto item : worlditems)
    {
        if(item.statusEffects.empty() && item.statusEffectsInflict.empty())
            continue;
        for(auto status : item.statusEffects)
        {
            int aspects = 0;
            sf::Color circleColor;
            for(auto aspect : status.aspects)
                aspects++;
            if(status.rank == "Alpha")
                circleColor = sf::Color::Blue;
            else if(status.rank == "Beta")
                circleColor = sf::Color::Green;
            else if(status.rank == "Gamma")
                circleColor = sf::Color::Red;
            else
                circleColor = sf::Color::Yellow;

            shapes.createCircle(item.xpos,item.ypos,(10*aspects)*cosWave,sf::Color::Transparent,1,circleColor);
            shapes.shapes.back().layer = 11;
        }
        for(auto status : item.statusEffectsInflict)
        {
            int aspects = 0;
            sf::Color circleColor;
            for(auto aspect : status.aspects)
                aspects++;
            if(status.rank == "Alpha")
                circleColor = sf::Color::Blue;
            else if(status.rank == "Beta")
                circleColor = sf::Color::Green;
            else if(status.rank == "Gamma")
                circleColor = sf::Color::Red;
            else
                circleColor = sf::Color::Yellow;

            shapes.createCircle(item.xpos,item.ypos,(10*aspects)*cosWave,sf::Color::Transparent,1,circleColor);
            shapes.shapes.back().layer = 11;
        }
    }
}

void setTileImages(std::string towerName)
{
    std::cout << "Looking for " + towerName + " tiles. \n";
    for(int x = 0; x != GRIDS; x++)
        for(int y = 0; y != GRIDS; y++)
            for(int z = 0; z != CHUNK_SIZE; z++)
    {
        if(tiles[x][y][z].id == 3007)
            tiles[x][y][z].img.setTexture(texturemanager.getTexture(towerName+"Floor"+str(randz(1,3))+".png"));
        else if(tiles[x][y][z].id == 2010)
            tiles[x][y][z].img.setTexture(texturemanager.getTexture(towerName+"Wall.png"));
        else if(tiles[x][y][z].id == 2012)
            tiles[x][y][z].img.setTexture(texturemanager.getTexture(towerName+"WallBreakable.png"));


    }


}

void castSpell()
{
    Npc spell;
    spell.xpos = gvars::mousePos.x;
    spell.ypos = gvars::mousePos.y;
    spell.zpos = gvars::currentz*GRID_SIZE;
    //spell.tags.append("[DeleteOnDeath:1]");
    spell.alive = true;
    spell.health = 10000000;
    spell.maxhealth = 10000000;
    spell.hasSpawned = true;
    spell.toDelete = true;



    spell.statusEffects.push_back(globalStatusEffects.getStatusEffect("Fireball"));



    npclist.push_back(spell);
}

void spawnModWeapon()
{
    Item weapon;

    weapon.xpos = gvars::mousePos.x;
    weapon.ypos = gvars::mousePos.y;
    weapon.zpos = gvars::currentz*GRID_SIZE;
    weapon.img.setTexture(texturemanager.getTexture("ChainMagnum.png"));
    weapon.type = 2;
    weapon.name = "Mod Weapon";
    weapon.ammotype = itemTypes.getTypeID("Bullet").num;
    weapon.size = 1;

    {
        Item mod;
        mod.name = "Double Barrel";
        mod.type = itemTypes.getTypeID("PartBarrel").num;
        mod.barrelCount = 2;
        mod.damageMultiplier = 3;
        mod.dispersion = 5;
        mod.recoil = 5;
        mod.bulletSpeedMultiplier = 2.3;
        weapon.internalitems.push_back(mod);
    }

    {
        Item mod;
        mod.name = "Rapid Fire Body";
        mod.type = itemTypes.getTypeID("PartBody").num;

        mod.damageMultiplier = -0.25;
        mod.fireMode = mod.Burst;
        mod.burstCount = 3;
        mod.fireDelay = 30;
        mod.recoil = 15;
        mod.recoilReduction = 1;
        mod.dispersion = 5;
        mod.aimTime = 3*60;

        mod.durability = 5000;
        mod.durabilityCost = 50;

        weapon.internalitems.push_back(mod);
    }

    {
        Item mod;
        mod.name = "Simple Iron Sights";
        mod.type = itemTypes.getTypeID("PartSight").num;

        mod.aimTime = -30;
        mod.dispersion = -5;

        weapon.internalitems.push_back(mod);
    }

    {
        Item mod;
        mod.name = "Stock Magazine";
        mod.type = itemTypes.getTypeID("PartMagazine").num;

        mod.ammoCapacity = 8;
        mod.reloadAmount = 2;
        mod.reloadTime = 15;

        weapon.internalitems.push_back(mod);
    }

    {
        Item mod;
        mod.name = "Rifle Stock";
        mod.type = itemTypes.getTypeID("PartStock").num;

        mod.recoil = -10;
        mod.recoilReduction = 5;
        mod.reloadTime = 5;

        weapon.internalitems.push_back(mod);
    }

    {
        Item mod;
        mod.name = "Stock Grip";
        mod.type = itemTypes.getTypeID("PartGrip").num;

        mod.recoilReduction = 5;

        weapon.internalitems.push_back(mod);
    }

    worlditems.push_back(weapon);
}

void colorTest()
{

    if(inputState.key[Key::LControl] && inputState.key[Key::RControl])
    {

    }
    else
        return;

    sfe::RichText text(gvars::defaultFont);
    text << sf::Text::Bold       << sf::Color::Cyan  << "This "
         << sf::Text::Italic     << sf::Color::White << "is\ncool\n"
         << sf::Text::Regular    << sf::Color::Green << "mate"
         << sf::Color::White     << ".\n"
         << sf::Text::Underlined << "I wish I could lick it!";

    if(inputState.key[Key::Space])
        text << sf::Color::Yellow << "(I've Gone Super!)";

    text.setCharacterSize(25);
    text.setPosition(gvars::mousePos);
    text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);

    window.draw(text);
}

void airdropMenuChecker()
{
    for(auto squaddie : Squaddies)
    {
        if(tiles[squaddie->xpos/60][squaddie->ypos/60][squaddie->zpos/60].id == 3850)
        {
            sf::Vector2f buttPos(screen.x()/2,100);
            int shopButt = shapes.createImageButton(buttPos,texturemanager.getTexture("ShopButton.png"),"",0,&gvars::hudView);
            shapes.shapes.back().layer = 150000;
            if(shapes.shapeClicked(shopButt))
                merchantMenu(squaddie->getPos());
            if(shapes.shapeHovered(shopButt))
            {
                shapes.createText(gvars::mousePos.x,gvars::mousePos.y,12,sf::Color::White,"\n Call in Airdrop Resources");
                shapes.shapes.back().layer = 150001;
            }
        }
    }
}

void userBlock()
{ // The player can press Q at any time to attempt to intercept all strikes for a short amount of time.
    static int lastBlock = 0;
    int userBlockRate = 60;
    lastBlock = math::clamp(lastBlock+1,0,99999999);


    for(auto Squad : Squaddies)
        Squad->blockTime = math::clamp(Squad->blockTime-1,0,99999999);



    int blockTime = 30;
    if(inputState.key[Key::Q].time == 1 && lastBlock > userBlockRate)
    {

        lastBlock = 0;
        for(auto Squad : Squaddies)
        {
            shapes.createImageButton(Squad->getPos2d(),texturemanager.getTexture("userBlock.png"));
            shapes.shapes.back().duration = blockTime;
            Squad->blockTime = blockTime;
        }
    }
}

void dragging()
{
    for(auto &npc : npclist)
    {
        if(npc.dragging != nullptr)
        {
            shapes.createLine(npc.xpos,npc.ypos,npc.endPos.x,npc.endPos.y,1,sf::Color::Cyan,1,sf::Color::Blue);
            shapes.createCircle(npc.endPos.x,npc.endPos.y,10,sf::Color::Cyan,1,sf::Color::Blue);
        }

        Npc * draggy = npc.draggedBy;
        if(draggy != nullptr)
        {
            if(draggy->dragging == nullptr)
            {
                npc.draggedBy = nullptr;
                std::cout << "No longer being dragged! \n";
                continue;
            }
            sf::Color ropeColor(170,170,0);

            shapes.createLine(npc.xpos,npc.ypos,draggy->xpos,draggy->ypos,3,ropeColor);
            sf::Vector2f momPos( -math::clamp(npc.xpos-draggy->xpos,-5,5), -math::clamp(npc.ypos-draggy->ypos,-5,5) );
            npc.momentum += momPos;
        }
    }

    for(auto &squad : Squaddies) // Sever rope button
    {
        if(squad->dragging == nullptr)
            continue;

        sf::Vector2f buttPos = squad->getPos2d();
        buttPos.y -= 30;
        int ropeButt = shapes.createImageButton(buttPos,texturemanager.getTexture("ExitButton.png"));

        if(shapes.shapeHovered(ropeButt))
        {
            shapes.createText(gvars::mousePos.x,gvars::mousePos.y,10,sf::Color::White,"    Sever Rope!");
            if(inputState.lmbTime == 1)
            {
                squad->dragging->draggedBy = nullptr;
                squad->dragging = nullptr;
            }
        }
    }
}

void bountyTowerLoop()
{ // Game Loop

    hotkeySquaddieSelect();
    bossLoop();
    checkDoors();
    NPCbuttons();
    airdropMenuChecker();
    layHints();
    //corpsesBleed();
    chasePriorityFunction();
    showItemProgressCone();

    userBlock();

    enchantGlow();
    dragging();

    if(inputState.key[Key::L].time == 1)
    {
        Squaddies[0]->dragging = Squaddies[1];
        Squaddies[1]->draggedBy = Squaddies[0];
    }

    if(inputState.key[Key::L].time == 1 && inputState.key[Key::LShift])
    {
        Squaddies[0]->dragging = nullptr;
        Squaddies[1]->draggedBy = nullptr;
    }

    if(inputState.key[Key::O].time == 1)
    {
        Squaddies[0]->dragging = nullptr;
        //Squaddies[1]->draggedBy = Squaddies[0];
    }
    if(inputState.key[Key::M].time == 1)
    {
        for(auto &npc : npclist)
        {
            //npc.skills
            npc.skills.getSkill("Rope")->ranks = 1;
        }
    }


    colorTest();

    if(inputState.key[Key::RShift] && inputState.lmbTime == 1)
        castSpell();

    if(inputState.key[Key::Y].time == 1)
        spawnModWeapon();

    testStatusEffects();
    testNewMenu();

    if(inputState.key[Key::F9].time == 1)
    {
        soundmanager.playSound("Startup.wav");
        loadGame("Profile1");
    }


    if(inputState.key[Key::F3])
    {
        //setTileImages("Necromancer");
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


    if(inputState.key[Key::T].time == 10 && inputState.key[Key::LShift])
    {
        Npc boss = *getGlobalCritter("Rockkid");
        boss.xpos = gvars::mousePos.x;
        boss.ypos = gvars::mousePos.y;
        boss.zpos = gvars::currentz*GRID_SIZE;

        boss.id = gvars::globalid++;
        boss.level = 10;
        boss.health = 10000;

        boss.faction = "Towerlings";
        boss.factionPtr = &listAt(uniFact,2);

        Item weaponry = *getGlobalItem("Minigun");
        Item ammo = *getGlobalItem("Bullet Standard");
        ammo.amount = 10000;
        weaponry.internalitems.push_back(ammo);

        boss.inventory.push_back(weaponry);

        boss.skills.getSkill("Feral Body")->ranks = 2;

        npclist.push_back(boss);
    }


    drawMenus();

    if(inputState.key[Key::LShift] && inputState.key[Key::LControl] && inputState.key[Key::T].time == 1)
        toggle(gvars::tileEdit);

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

    if(inputState.key[Key::A].time == 1 && inputState.key[Key::LShift])
    { // Spawn XP Eggs
        Item item = *getGlobalItem("Level Egg");
        item.xpos = gvars::mousePos.x;
        item.ypos = gvars::mousePos.y;
        item.zpos = gvars::currentz*GRID_SIZE;
        item.amount = 1;

        worlditems.push_back(item);

        item = *getGlobalItem("Quarter Level Egg");
        item.xpos = gvars::mousePos.x - 60;
        item.ypos = gvars::mousePos.y;
        item.zpos = gvars::currentz*GRID_SIZE;
        worlditems.push_back(item);

        item = *getGlobalItem("Experience Essence Egg");
        item.xpos = gvars::mousePos.x + 60;
        item.ypos = gvars::mousePos.y;
        item.zpos = gvars::currentz*GRID_SIZE;
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
        Item item = *getGlobalItem("Bullet Standard");
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
