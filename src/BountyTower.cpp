#include "BountyTower.h"

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

    gCtrl.menuType = "BTTowers";
    menuPopUp();

}

void bountyTowerLoop()
{
    cameraControls();
    int mouseX = gvars::mousePos.x, mouseY = gvars::mousePos.y;
    std::string stringy = std::to_string(mouseX) + "/" + std::to_string(mouseY) + "(" + std::to_string(gvars::currentz) + ")";
    textList.createText(gvars::mousePos.x,gvars::mousePos.y,15,sf::Color::Cyan,stringy);
    //UnyTiles.drawTiles();
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

    if(inputState.key[Key::Space].time == 1)
    {
        gCtrl.menuType = "BTTowers";
        menuPopUp();
    }

    if (gCtrl.menuType != "NULL")
    {
        menuPopUp();
    }
    else
    {
        gCtrl.menuPos = math::Vec2f(-10000, -10000);
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
                tiles[x][y][z].lava();
            else
                tiles[x][y][z].sky();
            if(aabb(x,y,31,64,31,64))
            {
                tiles[x][y][z].wall();
            }
            if(aabb(x,y,32,63,32,63))
            {
                tiles[x][y][z].stone();
            }
            tiles[48][64][1].door();
            tiles[47][64][1].door();
            if( (x == 48 || x == 47) && y > 64 && z == 1)
                tiles[x][y][z].stone();

        }
    }
}

Tower::Tower()
{
    tex = &texturemanager.getTexture("TowerTile.png");
    name = "The Tower mk" + std::to_string(randz(1,10));
    difficulty = randz(10,100);
    minioncount = difficulty * randz(3,10);
    bountyTarget = "Dudeman mk" + std::to_string(randz(1,300));
    bountyPay = (difficulty/10) * (minioncount / 10);
}

std::vector<Tower> towers;
