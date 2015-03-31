#include "BountyTower.h"

void bountyTowerSetup()
{
    gCtrl.phase = "Lobby";
    window.setFramerateLimit(30); // 0 is unlimited
    //UnyTiles.makeTest();
    window.setView(gvars::view1);
    gvars::currentx = 32;
    gvars::currenty = 18;
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
    std::string stringy = std::to_string(mouseX) + "/" + std::to_string(mouseY);
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
            int holder = randz(1,3);
            if(holder == 1)
                tiles[x][y][z].stoneWall();
            else if(holder == 2)
                tiles[x][y][z].stone();
            else if(holder == 3)
                tiles[x][y][z].lava();
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
