#include "BountyTower.h"

template <typename T> T &listAt(std::list<T> &list, size_t index)
{
    auto it = list.begin();
    std::advance(it, index);
    return *it;
}


void bountyTowerSetup()
{
    gCtrl.phase = "Lobby";
    window.setFramerateLimit(30); // 0 is unlimited
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



    addFaction("The Titanium Grip");
    conFact = &listAt(uniFact,0);
    conFact->playerControlled = false;
    conFact = &listAt(uniFact,1);
    addMembers(4,"The Titanium Grip");

    addFaction("Towerlings");


    gCtrl.menuType = "BTTowers";
    menuPopUp();

    //gvars::debug = true;
}

void bountyTowerLoop()
{
    cameraControls();
    int mouseX = gvars::mousePos.x, mouseY = gvars::mousePos.y;
    std::string stringy = std::to_string(mouseX) + "/" + std::to_string(mouseY) + "(" + std::to_string(gvars::currentz) + ")";
    textList.createText(gvars::mousePos.x,gvars::mousePos.y,15,sf::Color::Cyan,stringy);

    if(inputState.key[Key::X].time == 1)
    {
        for(auto & fact : uniFact)
        {
            std::cout << fact.name << std::endl;
        }
    }

    if(inputState.key[Key::G].time == 1)
    {
        towers.clear();
        int Amt = randz(5,30);
        for(int i = 0; i != Amt; i++)
        {
            Tower tower;
            towers.push_back(tower);
        }
    }

    if (gCtrl.menuType != "NULL")
    {
        menuPopUp();
    }
    else
    {
        gCtrl.menuPos = math::Vec2f(-10000, -10000);
    }

    lmbPress();
    rightMouseButtonContextMenu();

    critterBrain(npclist);

    if(inputState.key[Key::Space])
        for(auto &npc : npclist)
    {
        npc.dirMove(gvars::mousePos);
    }

    if(gCtrl.phase == "Lobby")
    {

    }




    if( (gvars::framesPassed % 300) == 0 )
    {
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
            member.xpos = stair->pos.x*GRID_SIZE; //((GRIDS*GRID_SIZE)/2)+randz(-20,20);
            member.ypos = stair->pos.y*GRID_SIZE;
            member.zpos = (gvars::currentz*GRID_SIZE);
            member.id = gvars::globalid++;
            npclist.push_back(member);
        }
        debug("Done placin Towerlings");
    }


}

void buildTower(std::string towerName)
{
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
