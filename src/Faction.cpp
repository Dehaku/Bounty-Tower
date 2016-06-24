#include "Faction.h"
#include "filesystemUtils.hpp"
#include "astar.h"
#include "Textures.h"
#include "math.h"
#include "util.h"
#include "globalvars.h"

#include <sstream>
#include <fstream>
#include <cstring>
#include <iostream>


Faction *conFact;
std::list<Npc> npclist;
MakeSquad squady;
std::vector<Npc> worldCritters;
std::list<Faction> uniFact;
NpcManager npcmanager;
extern sf::RenderWindow window;

std::vector<Npc*> selectedNPCs;



skillKeepInfo::skillKeepInfo()
{
    user = nullptr;
    toDelete = false;
    legal = false;
    age = 0;
}

std::vector<skillKeepInfo> skillKeeps;

void skillKeepLoop()
{
    for(auto &sKI : skillKeeps)
    {
        if(inputState.rmb)
            sKI.toDelete = true;

        sKI.age++;
        sKI.usePos = gvars::mousePos;

        shapes.createCircle(sKI.usePos.x,sKI.usePos.y,10,sf::Color::Cyan);
        textList.createText(sKI.usePos,15,sf::Color::White,sKI.skillName);
    }
}

skillKeepInfo * getSkillKeep()
{
    for(auto &sKI : skillKeeps)
    {
        return &sKI;
    }
    return nullptr;
}

bool canSeeNpc(Npc &ori, Npc &target)
{
    bool foundTarget = false;
    bool foundOri = false;
    float x1 = ori.getPos().x, y1 = ori.getPos().y, x2 = target.getPos().x, y2 = target.getPos().y;

    // Bresenham's line algorithm
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if(steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if(x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    const int maxX = (int)x2;

    for(int x=(int)x1; x<maxX; x++)
    {
        if(steep)
        {
            if(tiles[abs_to_index(y/GRID_SIZE)][abs_to_index(x/GRID_SIZE)][gvars::currentz].walkable)
            {
                if(math::distance(Vec3f(y,x,gvars::currentz*GRID_SIZE),target.getPos()) <= target.size/2)
                {
                    foundTarget = true;
                }
                if(math::distance(Vec3f(y,x,gvars::currentz*GRID_SIZE),ori.getPos()) <= ori.size/2) // Set target.size to ori critter in final version.
                {
                    foundOri = true;
                }
            }
            else
            {
                //Ran into a wall, Cannot see further.
                return false;
            }
        }
        else
        {
            if(tiles[abs_to_index(x/GRID_SIZE)][abs_to_index(y/GRID_SIZE)][gvars::currentz].walkable)
            {
                if(math::distance(Vec3f(x,y,gvars::currentz*GRID_SIZE),target.getPos()) <= target.size/2)
                {
                    foundTarget = true;
                }
                if(math::distance(Vec3f(x,y,gvars::currentz*GRID_SIZE),ori.getPos()) <= ori.size/2) // Set target.size to ori critter in final version.
                {
                    foundOri = true;
                }
            }
            else
            {
                //Ran into a wall, Cannot see further.
                return false;
            }
        }
        error -= dy;
        if(error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
    if(foundOri && foundTarget) // This is done due to Bresenham's line automatically using the topleft most coordinate. (Half cases start trace on target.)
        return true;

    return false;
}


bool canSeeBetweenTiled(Vec3f ori, Vec3f target)
{
    bool foundTarget = false;
    bool foundOri = false;
    float x1 = abs_to_index(ori.x/GRID_SIZE), y1 = abs_to_index(ori.y/GRID_SIZE), x2 = abs_to_index(target.x/GRID_SIZE), y2 = abs_to_index(target.y/GRID_SIZE);
    int zFloor = ori.z/GRID_SIZE;
    // Bresenham's line algorithm
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if(steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if(x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    const int maxX = (int)x2;

    for(int x=(int)x1; x<maxX; x++)
    {
        if(steep)
        {

            if(tiles[abs_to_index(y)][abs_to_index(x)][zFloor].walkable)
            {

            }
            else
            {
                // Hit a wall!
                return false;
            }
        }
        else
        {
            if(tiles[abs_to_index(x)][abs_to_index(y)][zFloor].walkable)
            {

            }
            else
            {
                // Hit a wall!
                return false;
            }
        }

        error -= dy;
        if(error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
    if(foundOri && foundTarget) // This is done due to Bresenham's line automatically using the topleft most coordinate. (Half cases start trace on target.)
    {
            std::cout << "Returning true. \n";
            return true;
    }

    //This can only run if it didn't hit any walls.
    return true;
}




bool canSeeNpcv2(Npc &ori, Npc &target)
{
    bool foundTarget = false;
    bool foundOri = false;
    float x1 = abs_to_index(ori.getPos().x/GRID_SIZE), y1 = abs_to_index(ori.getPos().y/GRID_SIZE), x2 = abs_to_index(target.getPos().x/GRID_SIZE), y2 = abs_to_index(target.getPos().y/GRID_SIZE);
    int zFloor = ori.zpos/GRID_SIZE;
    // Bresenham's line algorithm
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if(steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if(x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    const int maxX = (int)x2;

    for(int x=(int)x1; x<maxX; x++)
    {
        if(steep)
        {

            if(tiles[abs_to_index(y)][abs_to_index(x)][zFloor].walkable)
            {

            }
            else
            {
                // Hit a wall!
                return false;
            }
        }
        else
        {
            if(tiles[abs_to_index(x)][abs_to_index(y)][zFloor].walkable)
            {

            }
            else
            {
                // Hit a wall!
                return false;
            }
        }

        error -= dy;
        if(error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
    if(foundOri && foundTarget) // This is done due to Bresenham's line automatically using the topleft most coordinate. (Half cases start trace on target.)
    {
            std::cout << "Returning true. \n";
            return true;
    }

    //This can only run if it didn't hit any walls.
    return true;
}


void assignItemsUser(Npc &npc, std::list<Npc> &container)
{
    for(auto &item : npc.inventory)
        item.user = &npc;
}

void critterEquip(Npc &npc, std::list<Npc> &container)
{
    for (int i = 0; i != 20; i++)
    { // Slotting unslotted items, and assigning their user as current NPC.
        if(npc.invSlots[i] == nullptr)
        {
            for (auto &item : npc.inventory)
            {
                if(item.slotted == false)
                {
                    std::cout << "Putting" << item.name << " in mem: " << npc.invSlots[i] << "/";
                    npc.invSlots[i] = &item;
                    npc.invSlots[i]->slotted = true;
                    npc.invSlots[i]->currentSlot = &npc.invSlots[i];
                    npc.invSlots[i]->user = &npc;
                    std::cout << npc.invSlots[i] << std::endl;
                    break;
                }
            }
        }
    }


    if(inputState.key[Key::I].time == 1 && inputState.key[Key::LShift])
    { // Printing all critters inventories into console in their respective slots.
        std::cout << npc.name << "'s inventory; \n";
        for (int i = 0; i != 20; i++)
        {
            if(npc.invSlots[i] != nullptr)
                std::cout << i << ": " << npc.invSlots[i]->name << std::endl;
        }
    }

    //Making sure there's a weapon in our hands.
    bool weaponEquipped = false;
    if(npc.getItemTypeInHands(1) != nullptr || npc.getItemTypeInHands(2) != nullptr)
        weaponEquipped = true;



    if(!weaponEquipped && inputState.key[Key::E].time == 1)
    {//No weapon in our hands? Let's see if we have one in our inventory.
        Item * weapon;
        for(int i = 0; i != 20; i++)
        {

            if(npc.invSlots[i] != nullptr && (npc.invSlots[i]->type == 1 || npc.invSlots[i]->type == 2))
            {// Found one, Let's put it in our hands.
                weapon = npc.invSlots[i];
                Item * leftHandItem = npc.getLeftHandItem();
                Item * rightHandItem = npc.getRightHandItem();

                if(leftHandItem != nullptr)
                { // Is there something in our hand? Let's move it to the back of our inventory.
                    for(int t = 1; t != 20; t++)
                    {//Opinions are split on whether the item should go in the off hand, or the inventory.
                        if(npc.invSlots[t] == nullptr)
                        {// Found our empty slot! If no empty slot is found, do nothing.
                            if(t == 1 && leftHandItem->size == 1)
                            {//If it's our offhand, and the item is small enough, it can go in.
                                npc.invSlots[t] = npc.invSlots[0];
                                npc.invSlots[0] = nullptr;
                            }
                            else if(t != 1)
                            {// If it's not our offhand, the size doesn't matter, for now. TODO: Make items take up their size in slots.
                                npc.invSlots[t] = npc.invSlots[0];
                                npc.invSlots[0] = nullptr;
                            }
                        }
                    }
                }

                if(weapon->size == 2)
                {
                    if(rightHandItem != nullptr)
                    {//Gotta make room for the large weapon.
                        for(int t = 2; t != 20; t++)
                        {//Starting late, No point in checking hands.
                            if(npc.invSlots[t] == nullptr)
                            {// Found our empty slot! If no empty slot is found, do nothing.
                                npc.invSlots[t] = npc.invSlots[1];
                                npc.invSlots[1] = nullptr;
                            }
                        }
                    }
                    if(npc.getLeftHandItem() == nullptr && npc.getRightHandItem() == nullptr)
                    {//Both empty? We're in business!

                        if(i != 19 && npc.invSlots[i+1] != nullptr && npc.invSlots[i+1]->id == npc.invSlots[i]->id)
                            npc.invSlots[i+1] = nullptr;

                        npc.invSlots[i] = nullptr;

                        npc.invSlots[0] = weapon;
                        npc.invSlots[1] = weapon;
                    }
                }

                if(weapon->size == 1)
                {//One handed weapon.
                    if(npc.getLeftHandItem() == nullptr)
                    {//This hand should be empty now, If not, do nothing.
                        npc.invSlots[i] = nullptr;
                        npc.invSlots[0] = weapon;
                    }
                }
            }
        }
    }


}

void critterPush(Npc &npc, std::list<Npc> &container)
{
    for(auto &critters : container)
    {
        int dist = math::distance(npc.getPos(),critters.getPos());
        if(dist <= npc.size && npc.id != critters.id && critters.canmove)
        {

            if(npc.draggedBy != nullptr && critters.dragging != nullptr)
            {
                if(npc.draggedBy == &critters && critters.dragging == &npc)
                    continue;
            }
            critters.momentum += sf::Vector2f( -(  (npc.getPos().x-critters.getPos().x)*0.25  ),
                                               -(  (npc.getPos().y-critters.getPos().y)*0.25  ) );
        }
    }
}

void critterVision(Npc &npc, std::list<Npc> &container)
{
    const sf::Vector2f npcPos(npc.xpos, npc.ypos);

    /* Running through the critters pointers to see which one is valid, Then setting it to aim at it. */
    sf::Vector2f * targetPos = &npc.desiredViewAngle;

    if (npc.targetInfo.item != nullptr)
        *targetPos = sf::Vector2f(npc.targetInfo.item->xpos,npc.targetInfo.item->ypos);
    else if (npc.targetInfo.npc != nullptr)
        *targetPos = sf::Vector2f(npc.targetInfo.npc->xpos,npc.targetInfo.npc->ypos);
    else if (npc.targetInfo.tile != nullptr)
    {
        Vec3 tilePos(npc.targetInfo.tile->getPos());
        *targetPos = sf::Vector2f(tilePos.x,tilePos.y);
    }
    else if (randz(1,100) == 100)/* No target? Occasionally choose a nearby direction to look at. */
        *targetPos = sf::Vector2f(npc.xpos+randz(-15,15),npc.ypos+randz(-15,15));

    debug("debug 2", false);

    /*  Do the angle math to figure out which direction we need to turn */
    const float npcAngle = math::constrainAngle(npc.angle);
    const float targetAngle = math::constrainAngle(math::angleBetweenVectors(npcPos, *targetPos) - 90);

    /*  Turn towards it's target!    */
    if (math::angleDiff(npcAngle, targetAngle) > npc.turnSpeed)
        npc.angle += npc.turnSpeed;
    else if (math::angleDiff(npcAngle, targetAngle) < -npc.turnSpeed)
        npc.angle -= npc.turnSpeed;
    else
        npc.angle = targetAngle;

    const int endAngle = -(npc.angle - (npc.viewangle / 2));
    const int startAngle = -(npc.angle - (-npc.viewangle / 2));
    debug("debug 3", false);

    /*Drawing Vision*/
    sf::ConvexShape shape;
    int pointCounter = 1;
    shape.setPointCount(pointCounter);
    shape.setPoint(0, sf::Vector2f(0, 0));
    if(true == false) // TODO: Reenable this once BT is done, It has no purpose in BT, No stealth mechanics.
        for (int rot = startAngle; rot != endAngle; rot++)
    {
        float xPos = npc.xpos + sinf(rot * PI / 180) * npc.viewrange;
        float yPos = npc.ypos + cosf(rot * PI / 180) * npc.viewrange;

        entityvectorpointercontainer EVPC;

        //EVPC = entityTrace(Vec3(npcPos.x,npcPos.y,npc.zpos), Vec3(xPos, yPos, npc.zpos));

        //std::cout << npc.name << "'s EVPC: " << EVPC.items.size() << "/" << EVPC.npcs.size() << "/" << EVPC.tiles.size() << std::endl;

        for (auto &i : EVPC.items)
        {
            shapes.createCircle(i->xpos,i->ypos,5,sf::Color::Black);
        }
        for (auto &i : EVPC.tiles)
        {
            Vec3 Pos = i->getPos();
            shapes.createCircle(Pos.x*20,Pos.y*20,5,sf::Color::Black);
        }

        /* Placing the points to make the visible vision visibility visible */
        if (rot == startAngle)
        {
            pointCounter++;
            shape.setPointCount(pointCounter);
            shape.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - npc.xpos, yPos - npc.ypos));
        }

        int CenterPoint = (startAngle + endAngle) / 2;
        if (rot == CenterPoint)
        {
            pointCounter++;
            shape.setPointCount(pointCounter);
            shape.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - npc.xpos, yPos - npc.ypos));

        }
        if ((rot % 10) == 0 && rot != 0)
        {
            pointCounter++;
            shape.setPointCount(pointCounter);
            shape.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - npc.xpos, yPos - npc.ypos));
        }
        if (rot == endAngle - 1)
        {
            pointCounter++;
            shape.setPointCount(pointCounter);
            shape.setPoint(pointCounter - 1,
                           sf::Vector2f(xPos - npc.xpos, yPos - npc.ypos));
        }
    }

    debug("debug 4", false);
    if(!selectedNPCs.empty() && selectedNPCs[0]->id == npc.id)
    {
        sf::Color filling(sf::Color::Yellow);
        filling.a = (50);
        shape.setFillColor(filling);
        shape.setOutlineColor(sf::Color::Yellow);
        shape.setOutlineThickness(1);
        shape.setPosition(npcPos);
        shapes.polygons.push_back(shape);
    }
    debug("debug 5", false);
    }

void critterSkillRefresh(Npc &npc, std::list<Npc> &container)
{
    for(auto &skill : npc.skills.list)
    {
        if(skill.active)
        {
            skill.cooldown--;
            if(skill.cooldown <= 0)
                skill.cooldown = 0;
        }
    }
}

void critterPickUp()
{
    if(!selectedNPCs.empty())
    {//Making sure we have someone selected.
        for(auto &item : worlditems)
        {//Running through the worlds items.

            int critterDist = math::distance(selectedNPCs[0]->getPos(),Vec3f(item.xpos,item.ypos,item.zpos));
            int mouseDist = math::closeish(gvars::mousePos,sf::Vector2f(item.xpos,item.ypos));
            if(critterDist <= 120 && mouseDist <= 10)
            {//Arbitrary number, but good enough distance for picking up.
                shapes.createCircle(item.xpos,item.ypos,10,sf::Color(0,255,255,100));
                textList.createText(item.xpos,item.ypos+15,15,sf::Color::White,"RMB: Pickup " + item.name);
                if(selectedNPCs[0]->inventory.size() < 20 && inputState.rmbTime == 1)
                {//Making sure we have room.

                    if(item.name == "Scrap" && item.firstPickup)
                        continue;

                    selectedNPCs[0]->addItem(item);
                    AnyDeletes(worlditems);
                }
                return;
            }
        }
    }
}



std::string useHealItem()
{
    if(mouseItem->type != 42)
        return "";
    if(mouseItem->user == nullptr)
        return "";

    int useRange = 120;

    Npc &npc = *mouseItem->user;

    shapes.createCircle(npc.xpos,npc.ypos,useRange,sf::Color(0,255,0,100));

    for(auto &squaddie : Squaddies)
    {

        bool squadClose = (math::distance(npc.getPos(),squaddie->getPos()) <= useRange);
        bool mouseClose = (math::closeish(gvars::mousePos,squaddie->getPos2d()) <= 20);

        if(squadClose && mouseClose)
        {
            sf::Vector2f vPos(gvars::mousePos.x,gvars::mousePos.y+15);
            textList.createText(vPos,15,sf::Color::White,"RMB: Use Potion On " + squaddie->name,gvars::view1);

            if(inputState.rmbTime == 1)
            {
                {
                    for(auto &status : mouseItem->statusEffectsInflict)
                        squaddie->statusEffects.push_back(status);

                    chatBox.addChat(npc.name + " used " + mouseItem->name + " on " + squaddie->name, sf::Color::Green);
                    mouseItem->remove();
                    AnyDeletes(mouseItem->user->inventory);
                    mouseItem = nullptr;
                    return "Used";
                }
            }
            return "Hovering";
        }
    }
    return "";
}

std::string dropItem()
{
    if(mouseItem != nullptr)
    {
        sf::Color canDropColor = sf::Color::Red;
        sf::Vector2f drawPos = mouseItem->user->getPos2d();
        if(math::closeish(gvars::mousePos,mouseItem->user->getPos2d()) <= 60)
        {
            canDropColor = sf::Color::Green;
            sf::Vector2f vPos(gvars::mousePos.x,gvars::mousePos.y+15);
            textList.createText(vPos,15,sf::Color::White,"RMB: drop " + mouseItem->name,gvars::view1);

            if(inputState.rmbTime == 1)
            {
                mouseItem->xpos = gvars::mousePos.x;
                mouseItem->ypos = gvars::mousePos.y;
                mouseItem->zpos = mouseItem->user->zpos;
                mouseItem->slotted = false;
                mouseItem->currentSlot = nullptr;

                worlditems.push_back(*mouseItem);
                mouseItem->remove();
                AnyDeletes(mouseItem->user->inventory);
                mouseItem = nullptr;

            }
        }
        shapes.createCircle(drawPos.x,drawPos.y,60,sf::Color::Transparent,1,canDropColor);
    }
    return "";
}

void mouseItemFunctions()
{
    if(mouseItem == nullptr)
        return;

    if(useHealItem() != "")
    {

    }
    else
        dropItem();
}

void critterWallCheck(Npc &npc, std::list<Npc> &container)
{
    if(tiles[abs_to_index(npc.xpos/GRID_SIZE)][abs_to_index(npc.ypos/GRID_SIZE)][abs_to_index(npc.zpos/GRID_SIZE)].walkable == false)
    {
        sf::Vector2f position = gridEject(sf::Vector2f(npc.xpos,npc.ypos));

        sf::Vector2f Alter = position - npc.getPos2d();
        npc.xpos += Alter.x;
        npc.ypos += Alter.y;

        npc.momentum = sf::Vector2f();
    }
}

void critterLevelUp(Npc &npc, std::list<Npc> &container)
{
    //if(inputState.key[Key::Q].time == 1)
    //    npc.xp += 50;

    if(inputState.key[Key::Q].time == 1 && inputState.key[Key::LShift])
        npc.xp += 5000;

    if(npc.xp >= nextLevelXpRequired(npc.level))
    {
        npc.xp = npc.xp-nextLevelXpRequired(npc.level);
        npc.level++;
        npc.skillpoints += 5;
        chatBox.addChat(npc.name + " has leveled up to " + std::to_string(npc.level) + "!", sf::Color::White);
    }
}




void scrapPickup(Npc &npc, std::list<Npc> &container)
{

    for(auto &scraps : worlditems)
    {
        if(scraps.name == "Scrap" && math::distance(npc.getPos(),scraps.getPos()) <= 60 && scraps.firstPickup)
        {
            scraps.firstPickup = false;
            std::string invStatus = npc.addItem(scraps);

            if(invStatus == "Done")
            {
                if(npc.skills.getRanks("Lucky Scavenger") > 0)
                {
                    int scavRandom = random(1,100);
                    if(scavRandom <= npc.skills.getRanks("Lucky Scavenger")*20)
                    {
                        int coinFlip = random(1,2);
                        if(coinFlip == 1)
                        {
                            Item Cash = *getGlobalItem("Cash");
                            Cash.name = "Cash";
                            Cash.amount = random(10,100);
                            Cash.xpos = npc.xpos;
                            Cash.ypos = npc.ypos;
                            Cash.zpos = npc.zpos;
                            worlditems.push_back(Cash);
                        }
                        if(coinFlip == 2)
                        {
                            Item Ammo = *getGlobalItem("Bullet Standard");
                            Ammo.amount = random(1,5);
                            Ammo.xpos = npc.xpos;
                            Ammo.ypos = npc.ypos;
                            Ammo.zpos = npc.zpos;
                            worlditems.push_back(Ammo);
                            npc.addItem(worlditems.back());
                            //npc.addItem(Ammo);
                        }
                    }
                }

                int soundRan = random(1,4);
                if(soundRan == 1)
                    soundmanager.playSound("ScrapPickup1.ogg");
                if(soundRan == 2)
                    soundmanager.playSound("ScrapPickup2.ogg");
                if(soundRan == 3)
                    soundmanager.playSound("ScrapPickup3.ogg");
                if(soundRan == 4)
                    soundmanager.playSound("ScrapPickup4.ogg");
            }
            else
            {
                scraps.firstPickup = true;
            }
        }
    }
}

void cashPickup(Npc &npc, std::list<Npc> &container)
{
    for(auto &moneys : worlditems)
    {
        if(math::distance(npc.getPos(),moneys.getPos()) <= 60 && moneys.name == "Cash")
        {
            npc.factionPtr->credits += moneys.amount;
            moneys.toDelete = true;

            int soundRan = random(1,3);
            if(soundRan == 1)
                soundmanager.playSound("CashPickup1.ogg");
            if(soundRan == 2)
                soundmanager.playSound("CashPickup2.ogg");
            if(soundRan == 3)
                soundmanager.playSound("CashPickup3.ogg");

        }
    }
}


void checkAmmo(Npc &npc, std::list<Npc> &container, Item * rangewep, bool forceReload)
{
    bool weaponEmpty = false;
    bool needsReload = false;
    bool isFull = false;

    bool hasSpareAmmo = false;

    //Checking if there's any ammo at all.
    Item * currentAmmo = getItemType(rangewep->internalitems,rangewep->ammotype);
    if(currentAmmo == nullptr)
        weaponEmpty = true;

    //If there's no ammo, Definitely needs to be reloaded, This extra step is for when a manual reload order is issued with ammo still in the gun.
    if(weaponEmpty)
        needsReload = true;

    if(inputState.key[Key::R].time == 1)
    {
        unloadAmmo(rangewep,&npc.inventory);
        return;
    }

    if(forceReload)
        needsReload = true;

    //Check if we have some ammo in our inventory to reload with!
    itemPtrVector ammoVector;
    for(auto &item : npc.inventory)
        if(item.type == rangewep->ammotype)
            ammoVector.ptrs.push_back(&item);

    if(!ammoVector.ptrs.empty())
        hasSpareAmmo = true;

    //Making sure the weapon isn't full, No point in reloading then.
    if(!weaponEmpty)
        if(currentAmmo->amount >= rangewep->getAmmoCapacity())
            isFull = true;

    for(auto &item : rangewep->internalitems)
    { // Doing some cleaning to make sure nothing bad ever happens.
        item.currentSlot = nullptr;
        item.slotted = false;
        item.user = nullptr;
    }

    if(needsReload && hasSpareAmmo && !isFull)
    {//Conditions are right, Let's reload!
        bool timeForReload = false;
        if(rangewep->reloadTimeCounter <= 0)
            timeForReload = true;
        else
        {
            rangewep->reloadTimeCounter--;
            float reloadPercent = rangewep->reloadTimeCounter / rangewep->getReloadTime();
            float reloadRadius = 60 * reloadPercent;
            shapes.createCircle(npc.xpos,npc.ypos,reloadRadius,sf::Color::Transparent,1,sf::Color::Yellow);
        }

        if(timeForReload == false) // Cannot continue, Stuff isn't ready.
            return;
        else
            rangewep->reloadTimeCounter = rangewep->getReloadTime();

        for(auto &newAmmo : ammoVector.ptrs)
        {
            if(currentAmmo != nullptr)
            {//We already have ammo! Let's deal with this first.

                //First we check if the already existing ammo in the gun is the same as the new one...
                bool sameAmmo = (currentAmmo->name == newAmmo->name);
                if(!sameAmmo)
                    continue;

                //Defining how much ammo needs to be put into the weapon.
                int ammoMissingFromClip = rangewep->getAmmoCapacity();
                ammoMissingFromClip -= currentAmmo->amount;
                int loadAmount = currentAmmo->amount;

                if(newAmmo->amount > ammoMissingFromClip)
                {//We have too much ammo, So we simply set loadAmount to max, reduce newAmount by ammoMissing.
                    newAmmo->amount -= ammoMissingFromClip;
                    loadAmount = rangewep->getAmmoCapacity();
                }
                else
                {//Not enough ammo to go over the cap, We'll add it up, then delete the empty ammo.
                    loadAmount += newAmmo->amount;
                    newAmmo->remove();
                }
                currentAmmo->amount = loadAmount;
                soundmanager.playSound("Reload.ogg");
                return;

            }
            else
            {//Gun's empty! This should be easy.
                int ammoMissingFromClip = rangewep->getAmmoCapacity();

                Item loadingAmmo = *newAmmo;
                int loadAmount = 0;

                if(newAmmo->amount > ammoMissingFromClip)
                {//Too much ammo? fill loadAmount, drain from newAmmo!
                    loadAmount = ammoMissingFromClip;
                    newAmmo->amount -= ammoMissingFromClip;
                }
                else
                {// Less than too much? loadAmount is newAmmo, then delete newAmmo! It's empty now!
                    loadAmount = newAmmo->amount;
                    newAmmo->remove();
                }

                loadingAmmo.amount = loadAmount;
                rangewep->internalitems.push_back(loadingAmmo);
                soundmanager.playSound("Reload.ogg");
                return;

            }
        }
    }


}




void buildTurret(Npc &npc, std::list<Npc> &container)
{
    skillKeepInfo * sKI;
    sKI = getSkillKeep();
    if(sKI != nullptr && sKI->user->id == npc.id && sKI->skillName == "Turret Construction")
    {
        Skill * turretSkill = npc.skills.getSkill("Turret Construction");
        if(turretSkill != nullptr && turretSkill->ranks > 0 && turretSkill->cooldown <= 0)
        {
            int sX = (abs_to_index(sKI->usePos.x/GRID_SIZE)*GRID_SIZE);
            int sY = (abs_to_index(sKI->usePos.y/GRID_SIZE)*GRID_SIZE);
            int eX = (abs_to_index(sKI->usePos.x/GRID_SIZE)*GRID_SIZE)+(GRID_SIZE);
            int eY = (abs_to_index(sKI->usePos.y/GRID_SIZE)*GRID_SIZE)+(GRID_SIZE);



            int scrapCost = 30;
            int buildDist = math::closeish(npc.getPos2d(),sKI->usePos);
            bool canBuild = false;
            bool tileBuildable = false;
            bool enoughScrap = false;
            Item * scrapPtr = nullptr;

            for(auto &item : npc.inventory)
            {
                if(item.name == "Scrap" && item.amount >= scrapCost)
                {
                    enoughScrap = true;
                    scrapPtr = &item;
                }
            }

            if(enoughScrap)
                textList.createText(sf::Vector2f(sKI->usePos.x,sKI->usePos.y+10),15,sf::Color::Cyan,std::to_string(scrapCost) + " Scrap Required");
            else
                textList.createText(sf::Vector2f(sKI->usePos.x,sKI->usePos.y+10),15,sf::Color::Red,std::to_string(scrapCost) + " Scrap Required");



            if(isInBounds(sKI->usePos) && tiles[abs_to_index(sKI->usePos.x/GRID_SIZE)][abs_to_index(sKI->usePos.y/GRID_SIZE)][gvars::currentz].walkable)
                tileBuildable = true;

            if(buildDist <= 120 && tileBuildable && enoughScrap)
            {
                canBuild = true;
                shapes.createSquare(sX,sY,eX,eY,sf::Color::Transparent,1,sf::Color::Cyan);
                shapes.createCircle(npc.xpos,npc.ypos,120,sf::Color::Transparent,1,sf::Color::Cyan);
            }
            else
            {
                shapes.createSquare(sX,sY,eX,eY,sf::Color::Transparent,1,sf::Color::Red);
                shapes.createCircle(npc.xpos,npc.ypos,120,sf::Color::Transparent,1,sf::Color::Red);
            }


            if(inputState.lmbTime == 1 && canBuild)
            {
                turretSkill->cooldown = turretSkill->cooldownint;

                if(scrapPtr != nullptr)
                {
                    scrapPtr->amount -= scrapCost;
                    if(scrapPtr->amount <= 0)
                        scrapPtr->remove();
                }


                Npc turret = *getGlobalCritter("Turret");
                turret.xpos = sX+(GRID_SIZE/2);
                turret.ypos = sY+(GRID_SIZE/2);
                turret.zpos = npc.zpos;
                turret.faction = npc.faction;
                turret.factionPtr = npc.factionPtr;





                Item ammo = *getGlobalItem("Bullet Standard");
                ammo.amount = 100;

                Item gun = *getGlobalItem("Gun");
                gun.maxdam = 25*turretSkill->ranks;
                gun.mindam = gun.maxdam/2;
                gun.recoilReduction = 10000;
                gun.fireDelay = 10;

                gun.range += ammo.range;
                ammo.range = 0;
                ammo.richochets = 0;

                { // Quality Built Skill
                    Skill * qualityBuilt = npc.skills.getSkill("Quality Built");
                    if(qualityBuilt != nullptr && qualityBuilt->ranks > 0)
                    {
                        gun.maxdam += gun.maxdam*(qualityBuilt->ranks*0.2);
                        gun.mindam = gun.maxdam/2;
                        gun.range += gun.range*(qualityBuilt->ranks*0.2);
                    }
                }


                gun.internalitems.push_back(ammo);
                turret.inventory.push_back(gun);

                // Tossing on another gun if you've got Practical Problems.
                if(npc.skills.getSkill("Practical Problems")->ranks > 0)
                    turret.inventory.push_back(gun);

                if(npc.skills.getSkill("Explosive Deconstruction")->ranks > 0)
                    turret.skills.getSkill("Explosive Deconstruction")->ranks = -1;

                turret.tags.append("[DeleteOnDeath:1]");

                npclist.push_back(turret);
            }
        }
    }
}


void craftAmmo(Npc &npc, std::list<Npc> &container)
{
    skillKeepInfo * sKI;
    sKI = getSkillKeep();
    if(sKI != nullptr && sKI->user->id == npc.id && sKI->skillName == "Ammo Construction")
    {
        Skill * ammoSkill = npc.skills.getSkill("Ammo Construction");
        if(ammoSkill != nullptr && ammoSkill->ranks > 0 && ammoSkill->cooldown <= 0)
        {

            int scrapCost = 30;
            bool enoughScrap = false;
            Item * scrapPtr = nullptr;

            for(auto &item : npc.inventory)
            {
                if(item.name == "Scrap" && item.amount >= scrapCost)
                {
                    enoughScrap = true;
                    scrapPtr = &item;
                }
            }

            if(enoughScrap)
                textList.createText(sf::Vector2f(sKI->usePos.x,sKI->usePos.y+10),15,sf::Color::Cyan,std::to_string(scrapCost) + " Scrap Required");
            else
                textList.createText(sf::Vector2f(sKI->usePos.x,sKI->usePos.y+10),15,sf::Color::Red,std::to_string(scrapCost) + " Scrap Required");

            if(enoughScrap)
            {
                ammoSkill->cooldown = ammoSkill->cooldownint;

                if(scrapPtr != nullptr)
                {
                    scrapPtr->amount -= scrapCost;
                    if(scrapPtr->amount <= 0)
                        scrapPtr->remove();
                }

                //Get the default generic bullet.
                Item ammo = *getGlobalItem("Bullet Standard");

                //Acquire hands items.
                Item * leftItem = npc.getLeftHandItem();
                Item * rightItem = npc.getRightHandItem();

                if(leftItem != nullptr && leftItem->type == 2)
                {
                    if(leftItem->ammotype != 3)
                    {
                        for(auto &globalammo : itemmanager.globalItems)
                        {
                            if(globalammo.type == leftItem->ammotype)
                            {
                                ammo = globalammo;
                                break;
                            }
                        }
                    }
                }
                else if(rightItem != nullptr && rightItem->type == 2)
                {
                    if(rightItem->ammotype != 3)
                    {
                        for(auto &globalammo : itemmanager.globalItems)
                        {
                            if(globalammo.type == rightItem->ammotype)
                            {
                                ammo = globalammo;
                                break;
                            }
                        }
                    }
                }

                ammo.amount = 5*ammoSkill->ranks;
                if(ammo.amount > ammo.stackSize)
                    ammo.amount = ammo.stackSize;
                npc.inventory.push_back(ammo);
                sKI->toDelete = true;
            }
        }
    }
}

void placeExplosive(Npc &npc, std::list<Npc> &container)
{
    skillKeepInfo * sKI;
    sKI = getSkillKeep();
    if(sKI != nullptr && sKI->user->id == npc.id && sKI->skillName == "Explosives Construction")
    {
        Skill * turretSkill = npc.skills.getSkill("Explosives Construction");
        if(turretSkill != nullptr && turretSkill->ranks > 0 && turretSkill->cooldown <= 0)
        {
            int sX = (abs_to_index(sKI->usePos.x/GRID_SIZE)*GRID_SIZE);
            int sY = (abs_to_index(sKI->usePos.y/GRID_SIZE)*GRID_SIZE);
            int eX = (abs_to_index(sKI->usePos.x/GRID_SIZE)*GRID_SIZE)+(GRID_SIZE);
            int eY = (abs_to_index(sKI->usePos.y/GRID_SIZE)*GRID_SIZE)+(GRID_SIZE);



            int scrapCost = 30;
            int buildDist = math::closeish(npc.getPos2d(),sKI->usePos);
            bool canBuild = false;
            bool tileBuildable = false;
            bool enoughScrap = false;
            Item * scrapPtr = nullptr;

            for(auto &item : npc.inventory)
            {
                if(item.name == "Scrap" && item.amount >= scrapCost)
                {
                    enoughScrap = true;
                    scrapPtr = &item;
                }
            }

            if(enoughScrap)
                textList.createText(sf::Vector2f(sKI->usePos.x,sKI->usePos.y+10),15,sf::Color::Cyan,std::to_string(scrapCost) + " Scrap Required");
            else
                textList.createText(sf::Vector2f(sKI->usePos.x,sKI->usePos.y+10),15,sf::Color::Red,std::to_string(scrapCost) + " Scrap Required");



            if(isInBounds(sKI->usePos) && tiles[abs_to_index(sKI->usePos.x/GRID_SIZE)][abs_to_index(sKI->usePos.y/GRID_SIZE)][gvars::currentz].walkable)
                tileBuildable = true;

            if(buildDist <= 120 && tileBuildable && enoughScrap)
            {
                canBuild = true;
                shapes.createSquare(sX,sY,eX,eY,sf::Color::Transparent,1,sf::Color::Cyan);
                shapes.createCircle(npc.xpos,npc.ypos,120,sf::Color::Transparent,1,sf::Color::Cyan);
            }
            else
            {
                shapes.createSquare(sX,sY,eX,eY,sf::Color::Transparent,1,sf::Color::Red);
                shapes.createCircle(npc.xpos,npc.ypos,120,sf::Color::Transparent,1,sf::Color::Red);
            }


            if(inputState.lmbTime == 1 && canBuild)
            {
                turretSkill->cooldown = turretSkill->cooldownint;

                if(scrapPtr != nullptr)
                {
                    scrapPtr->amount -= scrapCost;
                    if(scrapPtr->amount <= 0)
                        scrapPtr->remove();
                }


                Npc turret = *getGlobalCritter("Turret");
                turret.xpos = sX+(GRID_SIZE/2);
                turret.ypos = sY+(GRID_SIZE/2);
                turret.zpos = npc.zpos;
                turret.name = "Da Bomb";
                turret.maxhealth = 1;
                turret.health = 1;
                turret.tags.append("[DeleteOnDeath:1]");

                turret.statusEffects.push_back(globalStatusEffects.getStatusEffect("Bomb Skill Explosion"));

                npclist.push_back(turret);
            }
        }
    }
}
void activeSkills(Npc &npc, std::list<Npc> &container)
{
    buildTurret(npc,container);
    craftAmmo(npc,container);
    placeExplosive(npc,container);
}

void workDesire(Npc &npc, std::list<Npc> &container, Vec3 &endPos, bool &hasPath, bool &inComplete, Desire * highestDesire )
{
    if(npc.jobPtr == nullptr)
    {
        for(auto &jobs : npc.factionPtr->jobList)
        {
            if(jobs.pWorker != nullptr || jobs.toDelete)
                continue;

            std::cout << npc.name << ", workin " << jobs.name << "/" << jobs.type << std::endl;
            jobs.pWorker = &npc;
            npc.jobPtr = &jobs;
            break;
        }
    }
    else
    {
            debug("I have a job, Now if only my programmer did.");
            Vec3 wPos(npc.jobPtr->workPos);
            Vec3 myPos(npc.xpos,npc.ypos,npc.zpos);
            if(npc.jobPtr != nullptr && npc.jobPtr->type == "Build")
            {
                debug("I have build job.");
                std::string desiredItem = "Wood";
                debug("Checking for material");
                Item * material = npc.hasItem(desiredItem);

                debug("Material checked");
                if(material == nullptr)
                {
                    for (auto &item : worlditems)
                    {
                        if(item.name == desiredItem)
                        {
                            if(npc.targetInfo.item == nullptr && item.user == nullptr)
                            {
                                npc.targetInfo.item = &item;
                                item.user = &npc;
                            }

                            else if(npc.targetInfo.item != nullptr)
                            {
                                sf::Vector2f myItemPos(npc.targetInfo.item->xpos,npc.targetInfo.item->ypos);
                                if(math::closeish(npc.xpos,npc.ypos,item.xpos,item.ypos) <= math::closeish(npc.xpos,npc.ypos,myItemPos.x,myItemPos.y)
                                   && item.user == nullptr)
                                {
                                    npc.targetInfo.item->user = nullptr;
                                    npc.targetInfo.item = &item;
                                    item.user = &npc;
                                }

                            }
                        }
                    }
                    Item * item = npc.targetInfo.item;
                    if(item != nullptr)
                    {
                        endPos = Vec3(npc.targetInfo.item->xpos/GRID_SIZE, npc.targetInfo.item->ypos/GRID_SIZE, npc.targetInfo.item->zpos/GRID_SIZE);
                        hasPath = true;
                        if(math::closeish(npc.xpos,npc.ypos,item->xpos,item->ypos) <= npc.size*2)
                        {
                            item->user = nullptr;
                            npc.inventory.push_back(*item);
                            item->toDelete = true;
                            npc.targetInfo.item = nullptr;
                        }
                    }

                }
                debug("Checking if any desiredItem exists");
                if(material == nullptr && npc.targetInfo.item == nullptr)
                {
                    debug("It doesn't");
                    inComplete = true;
                    npc.jobPtr->errorReason = "No " + desiredItem + " located. \n";
                    (*highestDesire).potency = 0;
                }
                else
                    npc.jobPtr->errorReason = "";

                if(material != nullptr)
                {
                    endPos = Vec3(npc.jobPtr->workPos.x/GRID_SIZE,npc.jobPtr->workPos.y/GRID_SIZE, npc.jobPtr->workPos.z/GRID_SIZE);
                    hasPath = true;
                }

                if(math::distance(myPos,wPos) <= npc.size*3 && myPos.z/GRID_SIZE == wPos.z/GRID_SIZE && material != nullptr)
                {
                    debug("Close to workPos and has material.");
                    endPos = Vec3(myPos.x/GRID_SIZE,myPos.y/GRID_SIZE,myPos.z/GRID_SIZE);
                    hasPath = false;

                    npc.jobPtr->completionProgress += npc.attributes.intelligence / 2;

                    for (float rot = 1; rot < 361 * (percentIs( npc.jobPtr->completionTimer,npc.jobPtr->completionProgress) / 100); rot++)
                    {

                        float xPos = wPos.x + sin(rot * PI / 180) * 10;
                        float yPos = wPos.y + cos(rot * PI / 180) * 10;

                        shapes.createLine( wPos.x, wPos.y, xPos, yPos, 1, sf::Color(150, 150, 150, 150));
                    }

                    if (npc.jobPtr->completionProgress >= npc.jobPtr->completionTimer)
                    {
                        tiles[abs_to_index(wPos.x / GRID_SIZE)][abs_to_index(wPos.y / GRID_SIZE)][abs_to_index(wPos.z / GRID_SIZE)].wall();
                        material->toDelete = true;
                        npc.jobPtr->toDelete = true;
                        npc.jobPtr->pWorker->hasJob = false;/* Oops... this amuses me. */
                        npc.jobPtr = nullptr;

                        if(network::connectedServer != "")
                        {
                            sf::Packet pack;
                            pack << ident.tilesUpdate;
                            pack << gvars::currentregionx << gvars::currentregiony;
                            pack << std::abs(wPos.x / GRID_SIZE) << std::abs(wPos.y / GRID_SIZE) << std::abs(wPos.z / GRID_SIZE);
                            pack << tiles[abs_to_index(wPos.x / GRID_SIZE)][abs_to_index(wPos.y / GRID_SIZE)][abs_to_index(wPos.z / GRID_SIZE)].id;
                            cliSocket.send(pack);
                        }

                    }

                }
            }
            else if(npc.jobPtr != nullptr && npc.jobPtr->type == "Chop" && npc.jobPtr->pItem != nullptr)
            {
                Item * itemPtr = npc.jobPtr->pItem;
                endPos = Vec3(abs_to_index(itemPtr->xpos/GRID_SIZE),abs_to_index(itemPtr->ypos/GRID_SIZE),abs_to_index(itemPtr->zpos/GRID_SIZE));
                hasPath = true;

                if(math::closeish(npc.xpos,npc.ypos,endPos.x*GRID_SIZE,endPos.y*GRID_SIZE) <= npc.size*3)
                {

                    npc.jobPtr->completionProgress += npc.attributes.intelligence / 2;

                    for (float rot = 1; rot < 361 * (percentIs( npc.jobPtr->completionTimer,npc.jobPtr->completionProgress) / 100); rot++)
                    {

                        float xPos = itemPtr->xpos + sin(rot * PI / 180) * 10;
                        float yPos = itemPtr->ypos + cos(rot * PI / 180) * 10;

                        shapes.createLine( itemPtr->xpos, itemPtr->ypos, xPos, yPos, 1, sf::Color(150, 150, 150, 150));
                    }

                    if (npc.jobPtr->completionProgress >= npc.jobPtr->completionTimer)
                    {

                        itemPtr->toDelete = true;
                        for(int Woody = 0; Woody != 5; Woody++)
                            spawnItem("Wood",itemPtr->xpos+randz(-3,3),itemPtr->ypos+randz(-3,3),itemPtr->zpos);
                        npc.jobPtr->toDelete = true;
                        npc.jobPtr = nullptr;
                    }
                }

            }
            else if(npc.jobPtr != nullptr && npc.jobPtr->type == "PickUp" && npc.jobPtr->pItem != nullptr)
            {
                Item * itemPtr = npc.jobPtr->pItem;
                endPos = Vec3(abs_to_index(itemPtr->xpos/GRID_SIZE),abs_to_index(itemPtr->ypos/GRID_SIZE),abs_to_index(itemPtr->zpos/GRID_SIZE));
                hasPath = true;

                //if(math::closeish(npc.xpos,npc.ypos,endPos.x*20,endPos.y*20) <= npc.size*3)
                if(math::distance(myPos,wPos) <= npc.size*3 && myPos.z/GRID_SIZE == wPos.z/GRID_SIZE)
                {
                    itemPtr->user = nullptr;
                    npc.inventory.push_back(*itemPtr);
                    itemPtr->toDelete = true;
                    npc.jobPtr->toDelete = true;
                    npc.jobPtr = nullptr;
                }
            }
            else if(npc.jobPtr != nullptr && npc.jobPtr->type == "Dig")
            {
                endPos = Vec3(npc.jobPtr->workPos.x/GRID_SIZE,npc.jobPtr->workPos.y/GRID_SIZE,npc.jobPtr->workPos.z/GRID_SIZE);
                hasPath = true;

                if(math::distance(myPos,wPos) <= npc.size*3 && myPos.z/GRID_SIZE == wPos.z/GRID_SIZE)
                {
                    debug("Close to workPos");
                    endPos = Vec3(myPos.x/GRID_SIZE,myPos.y/GRID_SIZE,myPos.z/GRID_SIZE);
                    hasPath = false;

                    npc.jobPtr->completionProgress += npc.attributes.intelligence / 2;
                    debug("post job completetion progress");

                    for (float rot = 1; rot < 361 * (percentIs( npc.jobPtr->completionTimer,npc.jobPtr->completionProgress) / 100); rot++)
                    {

                        float xPos = wPos.x + sin(rot * PI / 180) * 10;
                        float yPos = wPos.y + cos(rot * PI / 180) * 10;

                        shapes.createLine( wPos.x, wPos.y, xPos, yPos, 1, sf::Color(150, 150, 150, 150));
                    }
                    debug("post line drawing");

                    if (npc.jobPtr->completionProgress >= npc.jobPtr->completionTimer)
                    {
                        debug("Job complete!");

                        tiles[abs_to_index(wPos.x / GRID_SIZE)][abs_to_index(wPos.y / GRID_SIZE)][abs_to_index(wPos.z / GRID_SIZE)].stone();
                        for(int Stoney = 0; Stoney != 5; Stoney++)
                            spawnItem("Rock",wPos.x+randz(-3,3),wPos.y+randz(-3,3),wPos.z);

                        npc.jobPtr->toDelete = true;
                        npc.jobPtr = nullptr;
                    }
                }
            }
            else if(npc.jobPtr != nullptr && npc.jobPtr->type == "FlipSwitch")
            {
                endPos = Vec3(npc.jobPtr->workPos.x/GRID_SIZE,npc.jobPtr->workPos.y/GRID_SIZE,npc.jobPtr->workPos.z/GRID_SIZE);
                hasPath = true;
                Tile *workTile = &tiles[abs_to_index(wPos.x / GRID_SIZE)][abs_to_index(wPos.y / GRID_SIZE)][abs_to_index(wPos.z / GRID_SIZE)];

                if(math::distance(myPos,wPos) <= npc.size*3 && myPos.z/GRID_SIZE == wPos.z/GRID_SIZE)
                {
                    debug("Close to workPos");
                    endPos = Vec3(myPos.x/GRID_SIZE,myPos.y/GRID_SIZE,myPos.z/GRID_SIZE);
                    hasPath = false;

                    //npc.jobPtr->completionProgress += npc.attributes.intelligence;
                    workTile->workProgress += npc.attributes.intelligence;
                    debug("post job completetion progress");
                    int percentage = percentIs( npc.jobPtr->completionTimer,workTile->workProgress);
                    textList.createText(wPos.x-10,wPos.y-20,15,sf::Color::Yellow,"%" + std::to_string(percentage));

                    if (workTile->workProgress >= npc.jobPtr->completionTimer)
                    {
                        debug("Job complete!");

                        workTile->state = "On";

                        npc.jobPtr->toDelete = true;
                        npc.jobPtr = nullptr;
                    }
                }
            }
            else if(npc.jobPtr != nullptr && npc.jobPtr->type == "Move")
            {
                endPos = Vec3(wPos.x/GRID_SIZE,wPos.y/GRID_SIZE,wPos.z/GRID_SIZE);
                hasPath = true;

                if(math::distance(myPos,wPos) <= npc.size*2 && myPos.z/GRID_SIZE == wPos.z/GRID_SIZE
                   && npc.storedPath.empty())
                {

                    npc.dirMove(sf::Vector2f(wPos.x,wPos.y));
                }

                if(math::distance(myPos,wPos) <= npc.moverate && myPos.z/GRID_SIZE == wPos.z/GRID_SIZE)
                {
                    hasPath = false;

                    npc.xpos = wPos.x;
                    npc.ypos = wPos.y;
                    npc.zpos = wPos.z;

                    npc.jobPtr->toDelete = true;
                    npc.jobPtr = nullptr;
                }
            }

        }

}

void workSwitch(Npc &npc, std::list<Npc> &container)
{
    if(npc.faction != "The Titanium Grip")
        return;

    if(!isInBounds(npc.getPos2d()))
        return;
    int xTile = abs_to_index(npc.xpos/GRID_SIZE);
    int yTile = abs_to_index(npc.ypos/GRID_SIZE);
    int zTile = abs_to_index(npc.zpos/GRID_SIZE);
    Tile *standTile = &tiles[xTile][yTile][zTile];
    if(standTile->state == "Off")
    {
        float workAmount = npc.attributes.intelligence;


        Skill * logExpert = npc.skills.getSkill("Logistics Expert");
        if(logExpert != nullptr && logExpert->ranks > 0)
            workAmount = workAmount * logExpert->ranks;

        standTile->workProgress += workAmount;

        if(standTile->workProgress >= standTile->workGoal)
        {
            standTile->state = "On";
        }

        if(inputState.key[Key::L] && inputState.key[Key::LShift])
            standTile->state = "On";

        int percentage = percentIs(standTile->workGoal,standTile->workProgress);
        textList.createText(xTile*GRID_SIZE+10,yTile*GRID_SIZE+10,15,sf::Color::Yellow,"%" + std::to_string(percentage));
    }



}



void Npc::BodyDefinition::bodyPartFind(std::string part, int amount)
{

    size_t linePosOne;
    linePosOne = bodyParts.find(part);
    if (linePosOne != std::string::npos)
    {

        size_t linePosTwo;
        std::string lineValue;
        linePosTwo = bodyParts.find("]", linePosOne + 1);
        if (linePosTwo != std::string::npos)
        {
            int len = part.length();
            lineValue.assign(bodyParts, linePosOne + len,
                             linePosTwo - (linePosOne + len));
            amount += atoi(lineValue.c_str());

            std::stringstream out;
            out << amount;
            bodyParts.replace(linePosOne + len, linePosTwo - (linePosOne + len),
                              out.str());

            if (out.str() == "0")
            {
                bodyParts.replace(linePosOne, linePosTwo, "");
            }
        }
    }
    else if (amount == 0)
    {
        //cout<<"No "<<Part<<" Here\n";
    }
    else if (amount > 0)
    {
        bodyParts.assign(part);
        std::stringstream out;
        out << amount;
        bodyParts.append(out.str());
        bodyParts.append("]");
    }
}



void Npc::Attribute::Train(std::string skill, int amount, int skillgain)
{
    if (skill == "agility")
    {
        agilityxp += amount;
        if (agilityxp >= 100)
        {
            agility += skillgain;
            agilityxp = 0;
        }
    }
    if (skill == "charisma")
    {
        charismaxp += amount;
        if (charismaxp >= 100)
        {
            charisma += skillgain;
            charismaxp = 0;
        }
    }
    if (skill == "intelligence")
    {
        intelligencexp += amount;
        if (intelligencexp >= 100)
        {
            intelligence += skillgain;
            intelligencexp = 0;
        }
    }
    if (skill == "dexterity")
    {
        dexterityxp += amount;
        if (dexterityxp >= 100)
        {
            dexterity += skillgain;
            dexterityxp = 0;
        }
    }
    if (skill == "strength")
    {
        strengthxp += amount;
        if (strengthxp >= 100)
        {
            strength += skillgain;
            strengthxp = 0;
        }
    }
    if (skill == "perception")
    {
        perceptionxp += amount;
        if (perceptionxp >= 100)
        {
            perception += skillgain;
            perceptionxp = 0;
        }
    }
    if (skill == "endurance")
    {
        endurancexp += amount;
        if (endurancexp >= 100)
        {
            endurance += skillgain;
            endurancexp = 0;
        }
    }
    if (skill == "wisdom")
    {
        wisdomxp += amount;
        if (wisdomxp >= 100)
        {
            wisdom += skillgain;
            wisdomxp = 0;
        }
    }
}

Vec3f Npc::getPos()
{
    Vec3f myPos(xpos,ypos,zpos);
    return myPos;
}

sf::Vector2f Npc::getPos2d()
{
    sf::Vector2f myPos(xpos,ypos);
    return myPos;
}

Npc::Npc()
    : body{},
      isPlayer{}, hasSpawned{}, grappling{}, cbaseid{},
      alive{}, ticksalive{}, canmove{},
      age{},
      gypos{}, gxpos{}, planet{}, ypos{}, xpos{},
      rypos{}, rxpos{}, mana{}, credits{},
      size{}
{
    hasJob = false;
    jobPtr = nullptr;

    draggedBy = nullptr;
    dragging = nullptr;

    isSquaddie = false;
    isBoss = false;

    disabled = false;

    factionPtr = nullptr;

    framesSinceLastDamage = 0;

    level = 0;
    xp = 0;
    skillpoints = 5;

    chasePriority = "Assault"; // Assault, Defend, Hold Position
    chaseRange = 100;

    recruitable = false;
    value = 0;
    rarity = -1;

    additionalSlots = 0;

    deadFrames = 0;
    framesSinceLastMove = 0;

    hasPath = false;
    needsPath = false;

    for (int i = 0; i != 22; i++)
    {
        invSlots[i] = nullptr;
    }

    toDelete = false;
    viewangle = 180;
    viewrange = 200;
    stench = 0;
    killcount = 0;
    id = gvars::globalid++;
    needsFood = true;
    needsWater = true;

    angle = 1;
    turnSpeed = 1;
    allowedDrink = true;
    allowedFood = true;
    allowedMove = true;

    moveSpeed = 1;
    moverateint = 1;
    moverate = moverateint;
    movetimerint = 1000;
    movetimer = movetimerint;
    movetimerrate = 500;
    attacktimer = 0;
    attacktimerint = 25;
    maxhunger = 600;
    hungertimerint = 10;
    hungertimer = hungertimerint;
    hungerrate = 1;
    hunger = 600;
    maxthirst = 600;
    thirsttimerint = 5;
    thirsttimer = thirsttimerint;
    thirstrate = 1;
    thirst = 600;
    reach = 10;

    attributes.strength = randz(5, 10);
    attributes.perception = randz(5, 10);
    attributes.intelligence = randz(5, 10);
    attributes.charisma = randz(5, 10);
    attributes.endurance = randz(5, 10);
    attributes.dexterity = randz(5, 10);

    attributes.wisdom = randz(5, 10);
    attributes.agility = randz(5, 10);

    attributes.strengthxp = 0;
    attributes.perceptionxp = 0;
    attributes.intelligencexp = 0;
    attributes.charismaxp = 0;
    attributes.endurancexp = 0;
    attributes.dexterityxp = 0;

    attributes.wisdomxp = 0;
    attributes.agilityxp = 0;

    targetInfo.item = nullptr;
    targetInfo.npc = nullptr;
    targetInfo.tile = nullptr;

    consumeFlesh = false;
    consumeVeggy = false;
    consumeWater = false;

    blockTime = 0;

    zpos = 30*20;

    maxhealth = attributes.endurance * 0.8;
    regentimerint = 100;
    regentimer = regentimerint;
    regenrate = attributes.endurance / 10;
    health = maxhealth;
    stench = 50;
    race = "Human";
    int numz = randz(0, 1);
    if (numz == 0)
    {
        gender = "Male";
    }
    else
    {
        gender = "Female";
    }
    int numzb = randz(0, 1);
    if (numzb == 0)
    {
        gender = "Male";
    }
    else
    {
        gender = "Female";
    }
    if (gender == "Male")
    {
        int numzb = randz(0, 6);
        if (numzb == 0)
        {
            name = "John";
        }
        else if (numzb == 1)
        {
            name = "Jack";
        }
        else if (numzb == 2)
        {
            name = "Jonathan";
        }
        else if (numzb == 3)
        {
            name = "James";
        }
        else if (numzb == 4)
        {
            name = "Joseph";
        }
        else if (numzb == 5)
        {
            name = "Joe";
        }
        else if (numzb == 6)
        {
            name = "Justin";
        }
    };
    if (gender == "Female")
    {
        int numzb = randz(0, 5);
        if (numzb == 0)
        {
            name = "Jennifer";
        }
        else if (numzb == 1)
        {
            name = "Jill";
        }
        else if (numzb == 2)
        {
            name = "Jamie";
        }
        else if (numzb == 3)
        {
            name = "Jackie";
        }
        else if (numzb == 4)
        {
            name = "Joan";
        }
        else if (numzb == 5)
        {
            name = "Jessica";
        }
    };
    //img.SetImage(Images);
    isNPC = true;
    needsFood = true;
    allowedFood = true;
    needsWater = true;
    allowedDrink = true;
    needsPath = false;
}

float Npc::getMaxHealth()
{
    float modifiedMaxHealth = maxhealth;

    Skill * perfectHealth = skills.getSkill("Perfect Health");
    if(perfectHealth != nullptr)
        modifiedMaxHealth += modifiedMaxHealth*(perfectHealth->ranks*0.1);



    float endPercent = getEnd() * 0.01;
    modifiedMaxHealth += modifiedMaxHealth * endPercent;



    return modifiedMaxHealth;
}

void Npc::reCreateSkills()
{

    attributes.endurance = randz(5, 10);
    attributes.strength = randz(5, 10);
    attributes.dexterity = randz(5, 10);
    attributes.intelligence = randz(5, 10);
    attributes.wisdom = randz(5, 10);
    attributes.charisma = randz(5, 10);
    attributes.perception = randz(5, 10);
    attributes.agility = randz(5, 10);
    attributes.endurancexp = 0;
    attributes.strengthxp = 0;
    attributes.dexterityxp = 0;
    attributes.intelligencexp = 0;
    attributes.wisdomxp = 0;
    attributes.charismaxp = 0;
    attributes.perceptionxp = 0;
    attributes.agilityxp = 0;
    maxhealth = attributes.endurance * 0.8;
    regentimerint = 100;
    regentimer = regentimerint;
    regenrate = attributes.endurance / 10;
    health = maxhealth;
}

void Npc::blankSkills()
{

    attributes.endurance = 20;
    attributes.strength = 20;
    attributes.dexterity = 20;
    attributes.intelligence = 20;
    attributes.wisdom = 20;
    attributes.charisma = 20;
    attributes.perception = 20;
    attributes.agility = 20;
    attributes.endurancexp = 0;
    attributes.strengthxp = 0;
    attributes.dexterityxp = 0;
    attributes.intelligencexp = 0;
    attributes.wisdomxp = 0;
    attributes.charismaxp = 0;
    attributes.perceptionxp = 0;
    attributes.agilityxp = 0;
    maxhealth = attributes.endurance * 0.8;
    regentimerint = 100;
    regentimer = regentimerint;
    regenrate = attributes.endurance / 10;
    health = maxhealth;
}

void Npc::Attribute::randomizeAttributes()
{
    strength = random(strengthMin,strengthMax);
    perception = random(perceptionMin,perceptionMax);
    intelligence = random(intelligenceMin,intelligenceMax);
    charisma = random(charismaMin,charismaMax);
    endurance = random(enduranceMin,enduranceMax);
    dexterity = random(dexterityMin,dexterityMax);
}

npcPtrVector Npc::getEnemies()
{
    npcPtrVector enemyPtrs;
    for (auto &enemys : *container)
    {
        if(enemys.faction != faction)
        {
            for (auto &i : factionPtr->factRelations)
            {
                if(enemys.faction == i.faction && i.appeal < 1000)
                {
                    enemyPtrs.ptrs.push_back(&enemys);
                }
            }
        }
    }
    return enemyPtrs;
}

bool Npc::hasWeapon(std::string weapon)
{
    bool specific = false;
    bool hasSpecifiedWeapon = false;
    bool foundWeapon = false;
    if (weapon != "")
        specific = true;
    for (auto const &item : inventory)
    {
        if (item.isWeapon)
            foundWeapon = true;
        if (weapon != "")
        {
            if (item.name == weapon)
                hasSpecifiedWeapon = true;
        }
    }
    if (hasSpecifiedWeapon)
    {
        return true;
    }
    if (foundWeapon && specific == false)
    {
        return true;
    }
    return false;
}

Item *Npc::hasItem(std::string name)
{
    for (auto &item : inventory)
    {
        if(item.name == name)
            return &item;
    }
    return nullptr;
}

Item *Npc::getItemType(int type)
{
    //for(int i = 0; i < inventory.size(); i++)
    for (auto &elem : inventory)
    {
        if ((elem).type == type)
        {
            if (gvars::debug)
            {
                std::cout << "Returning inventory class \n";
            }
            return &(elem);
        }
    }
    if (gvars::debug)
    {
        std::cout << "Returning inventory nothing. \n";
    }
    return nullptr;
}

bool Npc::hasItemType(int type)
{
    //for(int i=0; i<inventory.size(); i++)
    for (auto &elem : inventory)
    {
        if ((elem).type == type)
        {
            if (gvars::debug)
            {
                std::cout << "returning inventory bool true \n";
            }
            return true;
        }
    }
    if (gvars::debug)
    {
        std::cout << "returning inventory bool false \n";
    }
    return false;
}

void Npc::drawImg()
{
    img.setRotation(angle);


    float offsetX = 0;
    float offsetY = 0;
    bool walking = (framesSinceLastMove < 3);
    if(walking && functional())
    {
        float cosWave = cos(fpsKeeper.startTime.getElapsedTime().asSeconds()*5 );

        sf::Vector2f leftPos = math::angleCalc(getPos2d(),angle,3*cosWave);
        offsetX = leftPos.x-xpos;
        offsetY = leftPos.y-ypos;
    }


    img.setPosition(xpos+offsetX, ypos+offsetY);



    if(framesSinceLastDamage < 12)
    {
        sf::Color oldColor = img.getColor();
        if(framesSinceLastDamage < 3)
        {
            img.setColor(sf::Color(255,0,0));
        }
        else if(framesSinceLastDamage < 9)
        {
            img.setColor(sf::Color(255,0,0));
        }
        window.draw(img);
        img.setColor(oldColor);
    }
    else
        window.draw(img);




}


/*

void dirMove(sf::Vector2f tar)
{
    bool atTargetx = false;
    bool atTargety = false;
    bool closeX = false;
    bool closeY = false;
    if (tar.x < xpos)
    {
    }
    else if (tar.x == xpos)
    {
        atTargetx = true;
    }
    else
    {
    }

    if (tar.y < ypos)
    {
    }
    else if (tar.y == ypos)
    {
        atTargety = true;
    }
    else
    {
    }

    if (atTargetx == true && atTargety == true)
    {
        atTarget = true;
    }
    else
    {
        if (atTargetx == false && atTargety == false)
        {
        }
        int numz0 = math::closeishS(tar.y, ypos);
        int numz1 = math::closeishS(tar.x, xpos);
        if (numz1 < moverate && numz0 < moverate)
        {
            xpos = tar.x;
            ypos = tar.y;
            closeX = true;
            closeY = true;
        }
        else
        {
            atTarget = false;
        }
    }

    if (closeY == false && closeX == false)
    {
        int xx = 0;
        int yy = 0;
        int tempangle =
            90 -
            (180 / PI) *
                (atan2f(
                    xpos - tar.x,
                    ypos -
                        tar.y)); //To be honest, I spent alot of time with trial and error to get this part to work.
        xx = cosf((tempangle)*PI / 180) * moverate;
        yy = sinf((tempangle)*PI / 180) * moverate;
        xpos -= xx;
        ypos -= yy;
    }
}

*/

void Npc::dirMove(sf::Vector2f tar)
{
    if(!canmove)
        return;


    sf::Vector2f oldPos(xpos,ypos);

    sf::Vector2f pos(xpos,ypos);
    if(math::closeish(pos.x,pos.y,tar.x,tar.y) <= getMoveSpeed())
    {
        xpos = tar.x;
        ypos = tar.y;
    }
    else
    {
        int xx = 0;
        int yy = 0;
        int tempangle =
            90 -
            (180 / PI) *
                (atan2f(
                    xpos - tar.x,
                    ypos -
                        tar.y)); //To be honest, I spent alot of time with trial and error to get this part to work.
        xx = cosf((tempangle)*PI / 180) * getMoveSpeed();
        yy = sinf((tempangle)*PI / 180) * getMoveSpeed();
        xpos -= xx;
        ypos -= yy;
    }

    if(!isInBounds(getPos2d()))
    {
        xpos = oldPos.x;
        ypos = oldPos.y;
    }
    if(!tiles[abs_to_index(xpos/GRID_SIZE)][abs_to_index(ypos/GRID_SIZE)][abs_to_index(zpos/GRID_SIZE)].walkable)
    {
        xpos = oldPos.x;
        ypos = oldPos.y;
    }


}


void Npc::angMove(float ang)
{
    float moveX = cosf(ang) * moverate;
    float moveY = sinf(ang) * moverate;
    xpos += moveX;
    ypos += moveY;
}

void Npc::momMove()
{
    if(momentum.x == 0 && momentum.y == 0)
        return;

    sf::Vector2f oldPos(xpos,ypos);

    xpos += (momentum.x / size);
    ypos += (momentum.y / size);


    if(!isInBounds(getPos2d()))
    {
        xpos = oldPos.x;
        ypos = oldPos.y;
    }
    if(!tiles[abs_to_index(xpos/GRID_SIZE)][abs_to_index(ypos/GRID_SIZE)][abs_to_index(zpos/GRID_SIZE)].walkable)
    {
        xpos = oldPos.x;
        ypos = oldPos.y;

        int momSpeed = 0;
        if(abs_to_index(momentum.x) > momSpeed)
            momSpeed = abs_to_index(momentum.x);
        if(abs_to_index(momentum.y) > momSpeed)
            momSpeed = abs_to_index(momentum.y);

        momentum.x = 0;
        momentum.y = 0;
        if(alive && momSpeed > 50)
        {
            if(racialAbility == "Rock Steady")
                chatBox.addChat(name + " slammed into the wall, but is immune!",sf::Color::White);
            else
            {
                takeDamage(nullptr,nullptr,(momSpeed/2),DamageTypes::Blunt);
                chatBox.addChat(name + " slammed into the wall with " + str(momSpeed) + " force!",sf::Color::White);
            }

        }



    }

    //I have no idea how I figured this out, but it worked first try and seems perfect, Woohoo! I'm free!

    float xairPressure = gvars::airPressure;
    float yairPressure = gvars::airPressure;

    float xMom = momentum.x;
    if(xMom < 0)
        xMom = -xMom;

    float yMom = momentum.y;
    if(yMom < 0)
        yMom = -yMom;

    float average = (yMom+xMom)/2;

    float xPercentage = xMom/average;
    float yPercentage = yMom/average;

    if(momentum.x > gvars::airPressure*xPercentage)
        momentum.x -= gvars::airPressure*xPercentage;
    else if(momentum.x < -gvars::airPressure*xPercentage)
        momentum.x += gvars::airPressure*xPercentage;
    else
        momentum.x = 0;

    if(momentum.y > gvars::airPressure*yPercentage)
        momentum.y -= gvars::airPressure*yPercentage;
    else if(momentum.y < -gvars::airPressure*yPercentage)
        momentum.y += gvars::airPressure*yPercentage;
    else
        momentum.y = 0;
}


bool Npc::isHungry()
{
    if (allowedFood == false)
    {
        return false;
    }
    if (hunger < (maxhunger * 0.40))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Npc::isThirsty()
{
    if (allowedDrink == false)
    {
        return false;
    }
    if (thirst < (maxthirst * 0.40))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Npc::isHurt()
{
    if (health < (maxhealth * 0.60))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Npc::breath()
{
}

void Npc::sethealth(float amount)
{
    health = amount;
}

bool Npc::modhealth(float amount)
{
    health += amount;
    if(health > getMaxHealth())
        health = getMaxHealth();

    if (health < 1)
    {
        health = 0;
        //alive = false;
        img.setColor(sf::Color(255,0,0,100));
        return false;
    }
    else
    {
        return true;
    }
}

void Npc::fillhunger(float amount)
{
    hunger += amount;
    if (hunger > maxhunger)
    {
        hunger = maxhunger;
    }
}

void Npc::fillthirst(int amount)
{
    thirst += amount;
    if (thirst > maxthirst)
    {
        thirst = maxthirst;
    }
}

std::string Npc::getname()
{
    return name;
}

void Npc::spawn(int /*gposx*/, int /*gposy*/, int /*rposx*/, int /*rposy*/,
                int posx, int posy, int posz)
{
    gxpos = 100;
    gypos = 100;
    rxpos = 100;
    rypos = 100;
    xpos = posx;
    ypos = posy;
    ypos = 30*20; //posz;
    hasSpawned = true;
}

bool Npc::boolblood(std::string ailment)
{
    size_t found;
    found = bloodcontent.find(ailment);
    if (found != std::string::npos)
    {
        //cout <<aliment<<" found at: " << int(found) << endl;
        return true;
    }
    else
        return false;
}

bool Npc::hasTag(std::string tag)
{
    size_t found;
    found = tags.find(tag);
    if (found != std::string::npos)
    {
        return true;
    }
    else
        return false;
}

void Npc::tag(std::string tag, int amount)
{
    using namespace std;
    size_t found;
    found = tags.find(tag);
    if (found != string::npos)
    {
        //cout <<tag<<" found at: " << int(found) << endl;
        size_t foundz;
        std::string funz;
        foundz = tags.find("]", found + 1);
        if (foundz != string::npos)
        {
            int len = tag.length();
            funz.assign(tags, found + len, foundz - (found + len));
            amount += atoi(funz.c_str());
            std::stringstream out;
            out << amount;
            tags.replace(found + len, foundz - (found + len), out.str());
            //cout<<"Out is:"<<out.str()<<endl;
            if (out.str() == "0")
            {
                //cout<<"out is 0, It at least it's supposed to be\n";
                tags.replace(found, foundz, "");
            }
        }
    }
    else if (amount == 0)
    {
        //cout<<"No "<<tag<<" Here\n";
    }
    else if (amount > 0)
    {
        tags.assign(tag);
        std::stringstream out;
        out << amount;
        tags.append(out.str());
        tags.append("]");
    }

    /*
        found=tags.find(Zom);if (found!=string::npos){cout << "Zombification found at: " << int(found) << endl;}else{cout<<"No Zombification Here\n";}
        }*/
}

float Npc::bloodwork(std::string aliment, float amount)
{ // Returns the final altered value of the altered 'aliment'

    size_t tStart;
    aliment = "[" + aliment +
              ":"; // Doing this so the rest of the code is much, much simplier.
    tStart = bloodcontent.find(aliment);
    float returns = amount;

    if (tStart != std::string::npos)
    {
        //cout <<aliment<<" tStart at: " << int(tStart) << endl;
        size_t tEnd;
        std::string StrNum;
        tEnd = bloodcontent.find("]", tStart + 1);

        if (tEnd != std::string::npos)
        {
            int len = aliment.length();

            StrNum.append(bloodcontent, tStart + len, tEnd - (tStart + len));
            //std::cout << "StrNum: " << StrNum;
            amount += atoi(StrNum.c_str());
            //std::cout << ", became: " << amount << std::endl;
            returns = amount;

            std::stringstream out;
            //Hargendy!
            out << amount;
            bloodcontent.replace(tStart + len, tEnd - (tStart + len),
                                 out.str());
            //cout<<"Out is:"<<out.str()<<endl;

            if (amount <= 0) //if(out.str() == "0")
            {
                //cout<<"out is 0, It at least it's supposed to be\n";
                bloodcontent.replace(tStart, tEnd + 1, "");
            }
        }
    }
    else if (amount == 0)
    {
        //cout<<"No "<<aliment<<" Here\n";
    }
    else // if(amount > 0)
    {
        bloodcontent.append(aliment);
        std::stringstream out;
        returns = amount;
        out << amount;
        bloodcontent.append(out.str());
        bloodcontent.append("]");
    }
    /*

        found=bloodcontent.find(Zom);if (found!=string::npos){cout << "Zombification found at: " << int(found) << endl;}else{cout<<"No Zombification Here\n";}

        }*/
    return returns;
}

float Npc::bloodworkXX(std::string aliment, int amount)
{ // Returns the final altered value of the altered 'aliment'

    size_t tStart;
    tStart = bloodcontent.find(aliment);
    float returns = amount;

    if (tStart != std::string::npos)
    {
        //cout <<aliment<<" tStart at: " << int(tStart) << endl;
        size_t tEnd;
        std::string StrNum;
        tEnd = bloodcontent.find("]", tStart + 1);

        if (tEnd != std::string::npos)
        {
            int len = aliment.length();

            StrNum.append(bloodcontent, tStart + len, tEnd - (tStart + len));
            amount += atoi(StrNum.c_str());
            returns = amount;

            std::stringstream out;
            //Hargendy!
            out << amount;
            bloodcontent.replace(tStart + len, tEnd - (tStart + len),
                                 out.str());
            //cout<<"Out is:"<<out.str()<<endl;

            if (out.str() == "0")
            {
                //cout<<"out is 0, It at least it's supposed to be\n";
                bloodcontent.replace(tStart, tEnd, "");
            }
        }
    }
    else if (amount == 0)
    {
        //cout<<"No "<<aliment<<" Here\n";
    }
    else if (amount > 0)
    {
        bloodcontent.append(aliment);
        std::stringstream out;
        returns = amount;
        out << amount;
        bloodcontent.append(out.str());
        bloodcontent.append("]");
    }
    /*

        found=bloodcontent.find(Zom);if (found!=string::npos){cout << "Zombification found at: " << int(found) << endl;}else{cout<<"No Zombification Here\n";}

        }*/
    return returns;
}

bool Npc::closeToTarget(int distance, sf::Vector2f Tar)
{
    int numbz = 0;
    numbz = math::closeish(xpos, ypos, Tar.x, Tar.y);
    if (numbz <= distance)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Npc::addItem(const std::string &itemname, int amount)
{ // TODO: Set this to optionally receive an item class instead, Will be useful for modded weapons and ect.
    if (gvars::debug)
    {
        std::cout << "Pre var in AddItem"
                  << " \n";
    }
    Item var = *getGlobalItem(itemname);
    if (gvars::debug)
    {
        std::cout << "Post var in AddItem"
                  << " \n";
    }
    for (int Times = 0; Times != amount; Times++)
    {
        var.id = gvars::globalid++;
        inventory.push_back(var);
    }
}

std::string Npc::addItem(Item &item)
{
    for(auto &inv : inventory)
    {
        if(inv.name == item.name)
        {
            if((inv.amount+item.amount) <= inv.stackSize)
            {
                inv.amount += item.amount;
                item.toDelete = true;
                return "Done";
            }
            else if(inv.amount != inv.stackSize)
            {
                int stackDiff = inv.stackSize-inv.amount;
                inv.amount += stackDiff;
                item.amount -= stackDiff;
            }
        }
    }

    if(inventory.size() >= getInventoryMax())
        return "Full";

    while(item.amount > item.stackSize)
    {
        Item newStack = item;
        newStack.id = gvars::globalid++;
        newStack.amount = newStack.stackSize;
        inventory.push_back(newStack);
        item.amount -= item.stackSize;
    }
    if(item.amount > 0)
    {
        inventory.push_back(item);
        item.amount = 0;
        item.toDelete = true;
    }

    return "Done";
}

int Npc::getInventoryMax()
{
    /*
    if(race == "Noirves")
        return 10;
    */

    int defaultSlots = 6; // This seems wasteful.
    int totalSlots = defaultSlots;

    totalSlots += additionalSlots;

    if(totalSlots > 22)
        totalSlots = 22;

    return totalSlots;
    //return 22;
}

void Npc::printConsoleInfo()
{
    using namespace std;
    cout << "Name: " << name << endl;
    cout << "cBaseId: " << cbaseid << endl;
    cout << "Id: " << id << endl;
    cout << "Xpos: " << xpos << endl;
    cout << "Ypos: " << ypos << endl;
    if (alive == true)
    {
        cout << " Alive" << endl;
    }
    if (alive == false)
    {
        cout << " Dead" << endl;
    }
    cout << "Health: " << health << endl;
    cout << "Hunger: " << hunger << endl;
    cout << "Thirst: " << thirst << endl;
    cout << "Kill Count: " << killcount << endl;
}

void Npc::printBloodContent()
{
    using namespace std;
    cout << "Name: " << name << endl;
    cout << bloodcontent << endl;
}


int Npc::getStr()
{
    int returnAttribute = attributes.strength;
    returnAttribute += mods.strMod;

    return returnAttribute;
}
int Npc::getPer()
{
    int returnAttribute = attributes.perception;
    returnAttribute += mods.perMod;

    return returnAttribute;
}
int Npc::getInt()
{
    int returnAttribute = attributes.intelligence;
    returnAttribute += mods.intMod;

    return returnAttribute;
}
int Npc::getCha()
{
    int returnAttribute = attributes.charisma;
    returnAttribute += mods.chaMod;

    return returnAttribute;
}
int Npc::getEnd()
{
    int returnAttribute = attributes.endurance;
    returnAttribute += mods.endMod;

    return returnAttribute;
}
int Npc::getDex()
{
    int returnAttribute = attributes.dexterity;
    returnAttribute += mods.dexMod;

    return returnAttribute;
}
int Npc::getAgi()
{
    int returnAttribute = attributes.agility;

    return returnAttribute;
}
int Npc::getWis()
{
    int returnAttribute = attributes.wisdom;

    return returnAttribute;
}

std::string Npc::getRace()
{


    return race;
}

std::string Npc::getRacialAbility()
{



    return racialAbility;
}

Modifiers::Modifiers()
{
    clearAllMods();
    clearAllPostConditions();
}

void Modifiers::clearAllMods()
{
    attackSpeedMod = 0;
    castSpeedMod = 0;
    reloadSpeedMod = 0;
    switchWorkSpeedMod = 0;
    moveSpeedMod = 0;
    freezeMod = 0;
    sleepMod = 0;
    stunMod = 0;
    affectDamageMod.clear();
    armorMod.clear();
    thornsMod.clear();
    manaRegenMod = 0;
    strMod = 0;
    perMod = 0;
    intMod = 0;
    chaMod = 0;
    endMod = 0;
    dexMod = 0;
    applyMomentumMod = 0;
    momentumSensitivityMod = 0;
    itemFindMod = 0;
    itemDropRateMod = 0;
    xpModifierMod = 0;
    deathXpModifierMod = 0;
    revivesOnDeathMod = 0;
    disableDeathMod = 0;

    immunityMod.clear(); // The damage string should be turned into a damage enum eventually.
    causeExplosionOnItemUseMod.clear(); // should be turned into a conditional somehow.
    causeDamageOnItemUseMod.clear(); // should be turned into a conditional somehow.
    spawnCreatureOnDeathMod.clear();
    spawnItemOnDeathMod.clear();
    autoDodgeMod.clear();
}

void Modifiers::clearAllPostConditions()
{
    onDeath = false;
    onItemUse = false;
    onItemUseType = -1;
    onHit = false;
    onHitType = 0;
}

bool Npc::operator<(const Npc &other) const
{
    return other.id > id;
}

double critDamage(float damage, critScore crit)
{
    throw std::runtime_error("Unimplemented.");
}

std::string Npc::onDeath(Npc *attacker, Item *weapon, float amount, int damageType, critScore *crit)
{
    if(!alive)
        return "";



    mods.onDeath = true;

    if(faction == "The Titanium Grip")
        chatBox.addChat(name + " has died!",sf::Color::White);
    alive = false;

    if(attacker != nullptr && attacker->skills.getSkill("Demonic Bounty")->ranks > 0)
    {
        StatusEffect demonBounty;
        demonBounty.name = "Demonic Bounty Buff(";

        int randomAspect = random(1,3);
        if(randomAspect == 1)
        { // Attribute
            RandomWeightList butes;
            butes.addEntry("Strength",100);
            butes.addEntry("Perception",100);
            butes.addEntry("Intelligence",100);
            butes.addEntry("Charisma",100);
            butes.addEntry("Endurance",100);
            butes.addEntry("Dexterity",100);
            std::string chosenBute = butes.getRandomName();
            demonBounty.addAspect(StatusAspect::Attribute,random(1,5),chosenBute);
            demonBounty.name.append("Attribute:"+chosenBute+")");
        }
        else if(randomAspect == 2)
        { // Damage
            RandomWeightList butes;
            butes.addEntry("Pierce",100);
            butes.addEntry("Slash",100);
            butes.addEntry("Blunt",100);
            std::string chosenBute = butes.getRandomName();
            demonBounty.addAspect(StatusAspect::AffectDamage,random(10,20),chosenBute);
            demonBounty.name.append("Damage:"+chosenBute+")");
        }
        else if (randomAspect == 3)
        { // Armor
            RandomWeightList butes;
            butes.addEntry("Pierce",100);
            butes.addEntry("Slash",100);
            butes.addEntry("Blunt",100);
            std::string chosenBute = butes.getRandomName();
            demonBounty.addAspect(StatusAspect::Armor,random(10,20),chosenBute);
            demonBounty.name.append("Armor:"+chosenBute+")");
        }

        demonBounty.duration = 600;
        attacker->statusEffects.push_back(demonBounty);
    }

    /*
    if(attacker != nullptr)
    { // Experience.
        float extraExperience = 0;

        for(auto &npc : *attacker->container)
            if(npc.faction == attacker->faction)
                if(npc.racialAbility == "Ideal Command")
                    extraExperience += 0.1;

        for(auto &npc : *attacker->container)
        {
            if(!npc.alive)
                continue;
            if(npc.faction != attacker->faction)
                continue;

            float expGain = level*10;
            expGain += expGain*extraExperience;

            expGain += expGain*(mods.deathXpModifierMod*0.01);

            expGain += expGain*(attacker->mods.xpModifierMod*0.01);


            npc.xp += expGain;
        }
    }

    */


    if(faction == "Towerlings")
    { // Experience.
        float extraExperience = 0;

        for(auto &npc : npclist)
            if(npc.faction == conFact->name)
                if(npc.racialAbility == "Ideal Command")
                    extraExperience += 0.1;

        for(auto &npc : npclist)
        {
            if(!npc.functional())
                continue;
            if(npc.faction != conFact->name)
                continue;

            float expGain = level*10;
            expGain += expGain*extraExperience;

            expGain += expGain*(mods.deathXpModifierMod*0.01);

            if(attacker != nullptr)
                expGain += expGain*(attacker->mods.xpModifierMod*0.01);


            npc.xp += expGain;
        }
    }


    if(faction == "Towerlings")
    { // Item Drops


        int dropRate = random(0,100);

        dropRate += dropRate*(mods.itemDropRateMod*0.01);

        if(attacker != nullptr)
            dropRate += dropRate*(attacker->mods.itemFindMod*0.01);

        std::cout << " Droprate: " << dropRate << std::endl;
        if(dropRate > 50)
        {
            int scrapAmount = random(1,5);
            for(int i = 0; i != scrapAmount; i++)
            {
                Item scrap = *getGlobalItem("Scrap");
                scrap.xpos = xpos+random(0,30);
                scrap.ypos = ypos+random(0,30);
                scrap.zpos = zpos;

                scrap.amount = random(1,3);
                worlditems.push_back(scrap);
            }
        }
        if(dropRate > 90 || isBoss)
        {
            RandomWeightList equipmentSet;
            equipmentSet.addEntry("Melee",100);
            equipmentSet.addEntry("Ranged",50);
            equipmentSet.addEntry("Charm",10);

            Item lootItem = generateRandomItem(equipmentSet);

            RandomWeightList rankList;
            if(isBoss)
            {
                rankList.addEntry("Alpha",0);
                rankList.addEntry("Beta",0);
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
                if(!isBoss)
                {
                    rankList.addEntry("Alpha",1000000);
                    rankList.addEntry("Beta",100000);
                    rankList.addEntry("Gamma",10000);
                    rankList.addEntry("Delta",1000);
                    rankList.addEntry("Epsilon",100);
                    rankList.addEntry("Zeta",10);
                    rankList.addEntry("Eta",1);
                }
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

            lootItem.xpos = xpos+random(0,30);
            lootItem.ypos = ypos+random(0,30);
            lootItem.zpos = zpos;

            worlditems.push_back(lootItem);
        }
    }

    if(skills.getSkill("Explosive Deconstruction")->ranks == -1)
    {
        int scrapCost = 30;
        //Damage is equal to 10% total health, multiplied by 10% of scrapCost
        int exploDamage = (getMaxHealth()*0.1)*(scrapCost*0.1);

        std::vector<Npc*> victims;
        for(auto &npc: npclist)
            if(npc.faction != conFact->name)
                victims.push_back(&npc);

        explosion(getPos(),200,exploDamage,this,&victims);
    }


    if(race != "Turret")
    { // Blood stuffs

        /*
        Shape splatter;
        splatter.startPos = sf::Vector2f(xpos-30,ypos-30);
        splatter.endPos = sf::Vector2f(xpos+30,ypos+30);

        splatter.shape = splatter.Square;
        splatter.duration = 2000000000;
        splatter.maincolor = sf::Color(255,0,0,60);

        int bloodNum = random(1,8);
        splatter.texture = &texturemanager.getTexture("Blood"+str(bloodNum)+".png");
        shapes.shapes.push_back(splatter);
        shapes.shapes.back().layer += random(1,100);
        */


        img.setColor(sf::Color(255,255,255,255));
        //img.setRotation(0);
        //angle = 0;
        //img.setOrigin(30,30);

        //img.setTexture(texturemanager.getTexture("MeatCorpsev2.png"));
    }


    if(tags.find("[DeleteOnDeath:1]") != std::string::npos)
        toDelete = true;
    return "";
}

std::string Npc::takeDamage(Npc *attacker, Item *weapon, float amount, int damageType, critScore *crit)
{

    // Immunity Status Effect
    if(blockTime > 0)
    {
        {// Dodge Popup Code
            Shape text;
            text.shape = text.Text;
            text.duration = 60;
            text.fades = true;
            text.maincolor = sf::Color::White;
            text.size = 20;
            sf::Vector2f textPos(xpos-15,(ypos-50)-random(0,50));
            text.startPos = textPos;
            text.text = "Blocked!";
            shapes.shapes.push_back(text);

            soundmanager.playSound("Block.ogg");
        }
        return "userBlock";
    }

    // Immunity Status Effect
    for(auto &immunity : mods.immunityMod)
    {
        if(immunity.str == damageTypes.TypeStrings[damageType])
        {
            {// Dodge Popup Code
                Shape text;
                text.shape = text.Text;
                text.duration = 60;
                text.fades = true;
                text.maincolor = sf::Color::White;
                text.size = 20;
                sf::Vector2f textPos(xpos-15,(ypos-50)-random(0,50));
                text.startPos = textPos;
                text.text = immunity.str + " Immunity";
                shapes.shapes.push_back(text);
            }
            return "Immune";
        }
    }



    for(auto &status : statusEffects)
        if(status.auraRadius == 0)
            for(auto &aspect : status.aspects)
                if(aspect.name == StatusAspect::AutoDodge && aspect.potency > 0)
    { // AutoDodge Status Effects. (Can't use mods for this, since AutoDodge has a limited amount of dodges.

        if(amount > 0 && damageType == damageTypes.getNum("Energy"))
        { // This checks if it's healing energy, No one wants to dodge being healed! ((Except maybe undead))

        }
        else
        {
            if(aspect.type == "")
            {
                aspect.potency--;
                if(lastHitFrames > 30)
                {// Dodge Popup Code
                    Shape text;
                    text.shape = text.Text;
                    text.duration = 60;
                    text.fades = true;
                    text.maincolor = sf::Color::White;
                    text.size = 20;
                    sf::Vector2f textPos(xpos-15,(ypos-50)-random(0,50));
                    text.startPos = textPos;
                    text.text = "*Auto Dodged*";
                    shapes.shapes.push_back(text);
                }
                return "Dodged";
            }
            else if(aspect.type == damageTypes.TypeStrings[damageType])
            {
                aspect.potency--;
                if(lastHitFrames > 30)
                {// Dodge Popup Code
                    Shape text;
                    text.shape = text.Text;
                    text.duration = 60;
                    text.fades = true;
                    text.maincolor = sf::Color::White;
                    text.size = 20;
                    sf::Vector2f textPos(xpos-15,(ypos-50)-random(0,50));
                    text.startPos = textPos;
                    text.text = "*Auto Dodged*";
                    shapes.shapes.push_back(text);
                }
                return "Dodged";
            }

        }

    }



    float dodgeChance = (attributes.dexterity/2);
    int dodgeRoll = random(0,100);
    if(dodgeRoll <= dodgeChance && amount > 0)
    {
        if(attacker != nullptr)
            std::cout << name << " dodged " << attacker->name << "'s attack! (" << dodgeRoll << "/" << dodgeChance << ")" << std::endl;
        if(lastHitFrames > 30)
        {// Dodge Popup Code
            Shape text;
            text.shape = text.Text;
            text.duration = 60;
            text.fades = true;
            text.maincolor = sf::Color::White;
            text.size = 20;
            sf::Vector2f textPos(xpos-15,(ypos-50)-random(0,50));
            text.startPos = textPos;
            text.text = "Dodged";
            shapes.shapes.push_back(text);
        }

        if(lastHitFrames > 30)
        {// Dodge Sound Code
            int soundRan = random(1,6);
            soundmanager.playSound("Woosh"+str(soundRan)+".ogg");
        }

        return "Dodged";
    }

    // All dodge modifications must be done above this line.

    mods.onHit = true;
    mods.onHitType = damageType;

    if(attacker != nullptr && weapon != nullptr && weapon->type == 1)
    { // Thorns Status Effect
        for(auto &thorns : mods.thornsMod)
            attacker->takeDamage(this,nullptr,thorns.num,damageTypes.getNum(thorns.str) );
    }

    if(racialAbility == "Rock Steady" && amount > 0)
    {
        amount -= amount*0.2;
    }

    if(skills.getRanks("Feral Body") > 0 && amount > 0)
    {
        int reduction = amount*(skills.getRanks("Feral Body")*0.1);
        amount = amount-reduction;
    }


    // Armor Status Effect
    for(auto &armor : mods.armorMod)
        if(armor.str == damageTypes.TypeStrings[damageType])
            amount -= amount*(armor.num*0.01);



    // All damage modifications must be done above this line.

    for(auto &status : statusEffects)
        if(status.auraRadius == 0)
            for(auto &aspect : status.aspects)
                if(aspect.name == StatusAspect::Freeze)
                    aspect.potency = math::clamp(aspect.potency-amount,0,9999999);

    for(auto &status : statusEffects)
        if(status.auraRadius == 0)
            for(auto &aspect : status.aspects)
                if(aspect.name == StatusAspect::Sleep)
                    aspect.potency = 0;



    {// Knockback Code
        int knockbackAmount = 0;
        sf::Vector2f AtkerPos;

        if(weapon != nullptr)
        {
            knockbackAmount += weapon->getKnockback();
        }
        if(attacker != nullptr)
        {
            AtkerPos = attacker->getPos2d();
            if(weapon != nullptr && weapon->type == 1 && attacker->skills.getRanks("Batter Up") > 0)
            {
                int ranks = attacker->skills.getRanks("Batter Up");
                knockbackAmount += amount*ranks;
            }
        }


        sf::Vector2f VictPos = getPos2d();
        float attackAngle = math::angleBetweenVectors(AtkerPos,VictPos);
        sf::Vector2f finalPos = math::angleCalc(VictPos,attackAngle,knockbackAmount);

        sf::Vector2f compared(finalPos.x-xpos,finalPos.y-ypos);
        if(AtkerPos != sf::Vector2f())
            momentum += compared;
    }

    if(amount > 0 && lastHitFrames > 30)
    {// Sound code
        if(race.find("Human") != race.npos)
        {
            int soundRan = random(1,9);
            soundmanager.playSound("InjuryHumanMale"+str(soundRan)+".ogg");
        }
        else if(race.find("Rockkid") != race.npos)
        {
            int soundRan = random(1,4);
            soundmanager.playSound("InjuryRockkid"+str(soundRan)+".ogg");
        }
        else if(race.find("Noirves") != race.npos)
        {
            int soundRan = random(1,9);
            soundmanager.playSound("InjuryHumanMale"+str(soundRan)+".ogg");
        }

        if(amount > getMaxHealth()/2)
        {
            int soundRan = random(1,6);
            soundmanager.playSound("vlatkoblazek_bones-breaking"+str(soundRan)+".ogg");
        }

    }

    if(attacker != nullptr && amount > 0)
    { // Vampirism Skill
        Skill * vamp = attacker->skills.getSkill("Vampirism");
        if(vamp != nullptr && vamp->ranks > 0)
        {
            float hpRegen = (amount*0.01)*(vamp->ranks);
            attacker->modhealth(hpRegen);
        }
    }

    if(amount > 0 && lastHitFrames > 30)
    {// Damage Popup Code
        Shape text;
        text.shape = text.Text;
        text.duration = 60;
        text.fades = true;
        text.maincolor = sf::Color::Red;
        text.size = 20;
        sf::Vector2f textPos(xpos-15,(ypos-50)-random(0,50));
        text.startPos = textPos;
        text.text = "-"+str(static_cast<int>(amount))+" "+damageTypes.TypeStrings[damageType];
        shapes.shapes.push_back(text);
    }

    if(modhealth(-amount) == false) // modhealth returns false on death.
    {
        int knockoutChance = 5;
        if(isSquaddie)
            disabled = true;
        else if(faction == "Towerlings" && random(1,100) < knockoutChance)
            disabled = true;
        else
            onDeath(attacker, weapon, amount, damageType, crit);
    }


    if(lastHitFrames > 30)
        lastHitFrames = 0;
    framesSinceLastDamage = 0;
    return "Hit";
}

bool Npc::functional()
{
    if(!alive)
        return false;
    if(disabled)
        return false;
    return true;
}

std::string Npc::dealDamage(Npc *victim, Item *weapon, float amount, int damageType)
{
    critScore pass;
    std::string outPut;



    int totalDamage = amount;
    if(weapon != nullptr)
    {
        totalDamage = random(weapon->getMinDamage(),weapon->getMaxDamage());

        if(weapon->type == 1)
        { // Each point of strength makes melee weapons deal 10% more damage.
            totalDamage += totalDamage * (getStr()*0.1);
        }
    }


    { // undeadDread Skill
        Skill * undeadDread = skills.getSkill("Undead Dread");
        if(undeadDread != nullptr && undeadDread->ranks > 0)
        {
            float removeAmount = health/getMaxHealth();
            float missingHealth = 1 - removeAmount;
            missingHealth = missingHealth / 2.5;
            totalDamage += totalDamage*(missingHealth*undeadDread->ranks);
        }
    }

    // AffectDamage Status Effect.
    for(auto &damage : mods.affectDamageMod)
        if(damage.str == damageTypes.TypeStrings[damageType])
            totalDamage += totalDamage*(damage.num*0.01);


    outPut = victim->takeDamage(this,weapon,totalDamage, damageType);

    //outPut = victim->takeDamage(this,weapon,amount);


    if(racialAbility == "Pocket Picker")
        if(weapon != nullptr && weapon->type == 1)
    {
        if(randz(0,100) <= 5)
        {
            Item loot;
            int ranNum = randz(1,5);
            if(ranNum == 1)
                loot = *getGlobalItem("Cash");
            if(ranNum == 2)
                loot = *getGlobalItem("Scrap");
            if(ranNum == 3)
                loot = *getGlobalItem("Bullet Standard");
            if(ranNum == 4)
                loot = *getGlobalItem("Shell Slug");
            if(ranNum == 5)
                loot = *getGlobalItem("Healing Juices");


            if(loot.stackSize == 1)
                loot.amount = 1;
            else
                loot.amount = random(5,loot.stackSize);

            loot.xpos = xpos;
            loot.ypos = ypos;
            loot.zpos = zpos;

            worlditems.push_back(loot);

        }
    }

    if(weapon != nullptr && weapon->type == 1 && outPut == "Hit" && victim->isBoss == false)
    { // Sunder Skill
        Skill * sunderSkill = skills.getSkill("Sunder");
        if(sunderSkill != nullptr && sunderSkill->ranks > 0)
        {
            int sunderOdds = random(0,100);
            if(sunderOdds < (sunderSkill->ranks * 20))
            {
                Item * victimWeapon = victim->getLeftHandItem();
                if(victimWeapon == nullptr)
                    victimWeapon = victim->getRightHandItem();
                if(victimWeapon == nullptr)
                {

                }
                else
                {
                    chatBox.addChat("* " + name + " sundered " + victim->name + "'s " + victimWeapon->name + "!",sf::Color::White);
                    victimWeapon->remove();
                }
            }
        }
    }

    if(outPut == "Hit")
    {
        return outPut;
    }

    else
        return "Miss";
}


Item * Npc::getLeftHandItem()
{
    return invSlots[0];
}

Item * Npc::getRightHandItem()
{
    return invSlots[1];
}

Item * Npc::getItemTypeInHands(int type)
{
    if(getLeftHandItem() != nullptr && getLeftHandItem()->type == type)
        return getLeftHandItem();
    else if(getRightHandItem() != nullptr && getRightHandItem()->type == type)
        return getRightHandItem();

    return nullptr;
}

float Npc::getMoveSpeed()
{
    float totalMoveSpeed = moveSpeed;
    totalMoveSpeed += mods.moveSpeedMod;
    // Don't want critters walking backwards.
    if(totalMoveSpeed < 0)
        totalMoveSpeed = 0;
    return totalMoveSpeed;
}

float Npc::getMomentumMagnitude()
{ // I doubt this is a proper magnitude function, but I needed something to roughly gauge how strong the momentum was.
    float returns;
    int xMom = momentum.x;
    int yMom = momentum.y;
    if(xMom < 0)
        xMom = -xMom;
    if(yMom < 0)
        yMom = -yMom;

    returns = xMom;

    if(xMom < yMom)
        returns = yMom;


    return returns;
}

void timeTest()
{

    time_t timer;
    time_t timer2;
    //struct tm y2k;
    double seconds;

    //y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0; y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

    time(&timer); // get current time; same as: timer = time(NULL)

    for (unsigned int i = 0; i != 100; i++)
    {
        //int Cake = randz(5,10);
        //std::cout << "Cake is: " << Cake << std::endl;
        //std::string TextLine = "Cake is: " + std::to_string(Cake); I need to upgrade to C++ 11
        //PrintToFile("Cake.txt",AddString("Cake Is: ",NumbertoString(Cake)), true);
    }
    time(&timer2);

    seconds = difftime(timer, timer2);

    //

    clock_t start = clock();

    for (unsigned int i = 0; i != 100; i++)
    {
        //int Cake = randz(5,10);
        //std::cout << "Cake is: " << Cake << std::endl;
        //PrintToFile("Cake.txt",AddString("Cake Is: ",NumbertoString(Cake)), true);
    }

    std::cout << "Time Difference: " << clock() - start << std::endl;
    printf("%.f second difference", seconds);

    clock_t vInital = clock();
    std::vector<Npc> testVector;
    for (int n = 0; n != 500; n++)
    {
        Npc npc;
        testVector.push_back(npc);
    }
    clock_t vCreation = clock();
    int vJohns = 0;
    for (size_t n = 0; n != testVector.size(); n++)
    {
        if (testVector[n].name == "John")
        {
            vJohns++;
        }
        if (testVector[n].id == 10)
        {
            //vJohns++;
        }
        if (testVector[n].health == 10)
        {
            //vJohns++;
        }
        if (testVector[n].id == 10)
        {
            //vJohns++;
        }
        if (testVector[n].health == 10)
        {
            //vJohns++;
        }
        if (testVector[n].id == 10)
        {
            //vJohns++;
        }
        if (testVector[n].health == 10)
        {
            //vJohns++;
        }
        //else Con(TestVector[n].name);
    }
    clock_t vFinished = clock();

    clock_t sInital = clock();
    std::list<Npc> testSet;
    for (int n = 0; n != 500; n++)
    {
        Npc npc;
        testSet.push_back(npc);
    }
    clock_t sCreation = clock();
    int sJohns = 0;
    for (auto const &elem : testSet)
    {
        if (elem.name == "John")
        {
            sJohns++;
        }
        if (elem.id == 10)
        {
            //sJohns++;
        }
        if (elem.health == 10)
        {
            //sJohns++;
        }
        if (elem.id == 10)
        {
            //sJohns++;
        }
        if (elem.health == 10)
        {
            //sJohns++;
        }
        if (elem.id == 10)
        {
            //sJohns++;
        }
        if (elem.health == 10)
        {
            //sJohns++;
        }
        //else Con(it->name);
    }
    clock_t sFinished = clock();

    con(testSet.size());
    con(sizeof(Npc));
    std::cout << "Vector Time, Creation: " << vCreation - vInital
              << ", John Counter: " << vFinished - vCreation << ", Total("
              << vFinished - vInital << "), Johns: " << vJohns << std::endl;
    std::cout << "Set Time, Creation: " << sCreation - sInital
              << ", John Counter: " << sFinished - sCreation << ", Total("
              << sFinished - sInital << "), Johns: " << sJohns << std::endl;
}

void NpcManager::addCritters()
{
    sf::Lock lock(mutex::npcList);
    for (auto const &c : addedCritters)
    {
        npclist.push_back(c);
    }
    addedCritters.clear();
}

void initializeGraspers(Npc &critter)
{
    partGrasp pG;
    pG.part = "Left Hand";
    critter.graspers.push_back(pG);
    pG.part = "Right Hand";
    critter.graspers.push_back(pG);
}

void NpcManager::initializeCritters()
{

    /*using namespace std;  I'll be using this soon enough, It'll allow the program to surf through the folder, and create all the stuff it needs.
        string dir = string(".//data//items");
        vector<string> files = vector<string>();
        getdir(dir,files);
        for(unsigned int i = 0; i < files.size(); i++)
        {
            string line("saves/maps/");
            string ending(files[i]);
            line.append(ending);
            if( remove(line.c_str()) != 0 ){}
        } */

    std::ifstream input(
        "data/npcs.txt"); // TODO: Have this read from an Items folder, and read from all .txt files in it, Allowing greater compability between mods.
    if (input.is_open())
    {
        while (input.good())
        {
            std::string line;
            getline(input, line);
            Npc critter;
            critter.faction = "";
            critter.factionPtr = nullptr;
            critter.name = "Debuggery";

            critter.racialAbility = stringFindString(line, "[RacialAbility:");
            critter.originalRacialAbility = critter.racialAbility;

            //hungerrate = 1; // TODO: Should these be modded? Or only effected by Diseases/Bionics ect.
            //thirstrate = 1;

            critter.name = stringFindString(line, "[Name:");
            std::cout << "Added: " << critter.name << std::endl;
            critter.race = critter.name;
            critter.originalRace = critter.race;
            if (critter.name == "Zombie")
                critter.race = "Zombie";
            if (gvars::debug)
            {
                std::cout << "Working on " << critter.name << "\n";
            }
            critter.cbaseid = stringFindNumber(line, "[BaseId:");
            critter.id = gvars::globalid++;

            //critter.target = stringFindString(line, "[Target:");
            critter.needsFood =
                booleanize(stringFindNumber(line, "[NeedsFood:"));
            critter.allowedFood =
                booleanize(stringFindNumber(line, "[AllowedFood:"));
            critter.needsWater =
                booleanize(stringFindNumber(line, "[NeedsWater:"));
            critter.allowedDrink =
                booleanize(stringFindNumber(line, "[AllowedDrink:"));
            critter.canmove = booleanize(stringFindNumber(line, "[CanMove:"));
            if (gvars::debug)
            {
                std::cout << "1 \n";
            }
            critter.maxhealth = stringFindNumber(line, "[MaxHealth:");
            critter.maxhunger = stringFindNumber(line, "[MaxHunger:");
            critter.maxthirst = stringFindNumber(line, "[MaxThirst:");
            critter.regentimerint = stringFindNumber(line, "[RegenRate:");

            //critter.breathtimerint = stringFindNumber(line, "[BreathTimerInt:");
            critter.hungertimerint = stringFindNumber(line, "[HungerTimerInt:");
            critter.thirsttimerint = stringFindNumber(line, "[ThirstTimerInt:");

            critter.recruitable = stringFindNumber(line, "[Recruitable:");
            critter.value = stringFindNumber(line, "[Value:");
            critter.rarity = stringFindNumber(line, "[Rarity:");

            critter.moveSpeed = stringFindNumber(line, "[MoveSpeed:");
            critter.turnSpeed = stringFindNumber(line, "[TurnSpeed:");
            critter.movetimerrate = stringFindNumber(line, "[MoveRate:");
            critter.viewangle = stringFindNumber(line, "[ViewAngle:");
            critter.viewrange = stringFindNumber(line, "[ViewRange:");
            critter.size = stringFindNumber(line, "[Size:");
            critter.reach = stringFindNumber(line, "[Reach:");
            critter.stench = stringFindNumber(line, "[Stench:");
            if (gvars::debug)
            {
                std::cout << "2 \n";
            }
            critter.attributes.strengthMin = stringFindNumber(line, "[MinStr:");
            critter.attributes.strengthMax = stringFindNumber(line, "[MaxStr:");
            critter.attributes.strength = random(critter.attributes.strengthMin,critter.attributes.strengthMax);

            critter.attributes.perceptionMin = stringFindNumber(line, "[MinPer:");
            critter.attributes.perceptionMax = stringFindNumber(line, "[MaxPer:");
            critter.attributes.perception = random(critter.attributes.perceptionMin,critter.attributes.perceptionMax);

            critter.attributes.intelligenceMin = stringFindNumber(line, "[MinInt:");
            critter.attributes.intelligenceMax = stringFindNumber(line, "[MaxInt:");
            critter.attributes.intelligence = random(critter.attributes.intelligenceMin,critter.attributes.intelligenceMax);

            critter.attributes.charismaMin = stringFindNumber(line, "[MinCha:");
            critter.attributes.charismaMax = stringFindNumber(line, "[MaxCha:");
            critter.attributes.charisma = random(critter.attributes.charismaMin,critter.attributes.charismaMax);

            critter.attributes.enduranceMin = stringFindNumber(line, "[MinEnd:");
            critter.attributes.enduranceMax = stringFindNumber(line, "[MaxEnd:");
            critter.attributes.endurance = random(critter.attributes.enduranceMin,critter.attributes.enduranceMax);

            critter.attributes.dexterityMin = stringFindNumber(line, "[MinDex:");
            critter.attributes.dexterityMax = stringFindNumber(line, "[MaxDex:");
            critter.attributes.dexterity = random(critter.attributes.dexterityMin,critter.attributes.dexterityMax);

            critter.attributes.agilityMin = stringFindNumber(line, "[MinAgi:");
            critter.attributes.agilityMax = stringFindNumber(line, "[MaxAgi:");
            critter.attributes.agility = random(critter.attributes.agilityMin,critter.attributes.agilityMax);

            critter.attributes.wisdomMin = stringFindNumber(line, "[MinWis:");
            critter.attributes.wisdomMax = stringFindNumber(line, "[MaxWis:");
            critter.attributes.wisdom = random(critter.attributes.wisdomMin,critter.attributes.wisdomMax);
            /*
            critter.attributes.enduranceMin = stringFindNumber(line, "[MinEnd:");
                randz(stringFindNumber(line, "[MinEnd:"),
                      stringFindNumber(line, "[MaxEnd:"));
            critter.attributes.strength = randz(stringFindNumber(line, "[MinStr:"),
                                            stringFindNumber(line, "[MaxStr:"));
            critter.attributes.dexterity =
                randz(stringFindNumber(line, "[MinDex:"),
                      stringFindNumber(line, "[MaxDex:"));
            critter.attributes.intelligence =
                randz(stringFindNumber(line, "[MinInt:"),
                      stringFindNumber(line, "[MaxInt:"));
            critter.attributes.wisdom = randz(stringFindNumber(line, "[MinWis:"),
                                          stringFindNumber(line, "[MaxWis:"));
            critter.attributes.charisma = randz(stringFindNumber(line, "[MinCha:"),
                                            stringFindNumber(line, "[MaxCha:"));
            critter.attributes.perception =
                randz(stringFindNumber(line, "[MinPer:"),
                      stringFindNumber(line, "[MaxPer:"));
            critter.attributes.agility = randz(stringFindNumber(line, "[MinAgi:"),
                                           stringFindNumber(line, "[MaxAgi:"));


                                           */
            if (gvars::debug)
            {
                std::cout << "3 \n";
            }
            if (critter.maxhealth == -1)
            {
                critter.maxhealth = critter.attributes.endurance * 0.8;
            }
            debug("v-Adding Tags-v");
            debug(stringFindChaos(line, "{Tags:", "}"));
            debug("^-Added Tags-^");
            critter.tags.append(stringFindChaos(line, "{Tags:", "}"));
            std::vector<std::string> items;
            std::string list;
            list.append(stringFindChaos(line, "{Items:", "}"));
            items = stringFindVectorChaos(list, "[", "]");
            for (auto item : items)
            {

                auto a = new char[item.size() + 1];
                a[item.size()] = 0;
                memcpy(a, item.c_str(), item.size());
                if (gvars::debug)
                {
                    std::cout << "Pre Critter.AddItem \n";
                }
                critter.addItem(a, 1);
                if (gvars::debug)
                {
                    std::cout << "*Post Critter.AddItem* \n";
                }
            }

            /*===================================================*/

            //critter.action = "Act";
            critter.alive = true;
            critter.hasSpawned = true;
            critter.movetimerint = 1000;
            critter.moverate = critter.moverateint;

            critter.health = critter.maxhealth;
            critter.hunger = critter.maxhunger;
            critter.thirst = critter.maxthirst;
            if (critter.hasTag("[EatsFlesh:"))
            {
                critter.hunger = 10;
                critter.thirst = 0;
            }
            critter.regentimer = critter.regentimerint;
            critter.regenrate =
                critter.attributes.endurance /
                10; // TODO: Have Skill based values update in the Train() function, So that stuff like Regen doesn't fall behind.

            //critter.breathtimer = critter.breathtimerint;
            critter.hungertimer = critter.hungertimerint;
            critter.thirsttimer = critter.thirsttimerint;
            critter.needsPath = false;

            // Critter Bodies
            {
                if(critter.race == "Human" || critter.race == "BlankBody")
                {

                    critter.body.bodyParts =
                        "{[Name:UpperTorso][BloodPumpRate:100][AirCapacity:"
                        "200][AirAbsorbtion:100][ObjectCapacity:1]["
                        "MassFlesh:15:1000]}";
                    critter.body.bodyParts.append(
                        "{[Name:Head][Mind:true][MassFlesh:5:"
                        "1000][Dependant:UpperTorso]}");
                    critter.body.bodyParts.append(
                        "{[Name:LowerTorso][ObjectCapacity:10]["
                        "DigestionRate:125][NutritionExtraction:10]["
                        "DigestsFlesh:60][DigestsVeggy:60][DigestsWater:"
                        "100][MassFlesh:15:1000][Dependant:UpperTorso]}");

                    critter.body.bodyParts.append("{[Name:Left "
                                                  "Leg][Walk:3][MassFlesh:"
                                                  "15:1000][Dependant:"
                                                  "LowerTorso]}");
                    critter.body.bodyParts.append("{[Name:Right "
                                                  "Leg][Walk:3][MassFlesh:"
                                                  "15:1000][Dependant:"
                                                  "LowerTorso]}");

                    critter.body.bodyParts.append("{[Name:Left "
                                                  "Arm][Grasp:2][MassFlesh:"
                                                  "10:1000][Dependant:"
                                                  "UpperTorso]}");
                    critter.body.bodyParts.append("{[Name:Right "
                                                  "Arm][Grasp:2][MassFlesh:"
                                                  "10:1000][Dependant:"
                                                  "UpperTorso]}");

                }
                if (critter.race == "Human" || critter.race == "Zombie")
                {
                    critter.body.bodyParts =
                        "{[Name:UpperTorso][BloodPumpRate:100][AirCapacity:"
                        "200][AirAbsorbtion:100][ObjectCapacity:1]["
                        "MassFlesh:15:1000]}";
                    critter.body.bodyParts.append(
                        "{[Name:Head][Mind:true][Orafice:1][MassFlesh:5:"
                        "1000][Dependant:UpperTorso]}");
                    critter.body.bodyParts.append(
                        "{[Name:LowerTorso][ObjectCapacity:10]["
                        "DigestionRate:125][NutritionExtraction:10]["
                        "DigestsFlesh:60][DigestsVeggy:60][DigestsWater:"
                        "100][MassFlesh:15:1000][Dependant:UpperTorso]}");

                    critter.body.bodyParts.append("{[Name:Left "
                                                  "Leg][Walk:3][MassFlesh:"
                                                  "15:1000][Dependant:"
                                                  "LowerTorso]}");
                    critter.body.bodyParts.append("{[Name:Right "
                                                  "Leg][Walk:3][MassFlesh:"
                                                  "15:1000][Dependant:"
                                                  "LowerTorso]}");

                    critter.body.bodyParts.append("{[Name:Left "
                                                  "Arm][Grasp:2][MassFlesh:"
                                                  "10:1000][Dependant:"
                                                  "UpperTorso]}");
                    critter.body.bodyParts.append("{[Name:Right "
                                                  "Arm][Grasp:2][MassFlesh:"
                                                  "10:1000][Dependant:"
                                                  "UpperTorso]}");
                }
                if (critter.race == "Azabul")
                {
                    critter.body.bodyParts =
                        "{[Name:UpperTorso][BloodPumpRate:100][AirCapacity:200]"
                        "["
                        "AirAbsorbtion:100][ObjectCapacity:1]["
                        "NutritionExtraction:"
                        "25][MassFlesh:15:1000]}";
                    critter.body.bodyParts.append(
                        "\n{[Name:Head][Mind:true]["
                        "Orafice:1][MassFlesh:5:1000]["
                        "Dependant:UpperTorso]}");
                    critter.body.bodyParts.append(
                        "\n{[Name:LowerTorso][ObjectCapacity:10][DigestionRate:"
                        "125]"
                        "[NutritionExtraction:50][PoisonFilter:Zombification:"
                        "10]["
                        "DigestsBlood:100][MassFlesh:15:1000][Dependant:"
                        "UpperTorso]"
                        "}");

                    critter.body.bodyParts.append(
                        "\n{[Name:Left "
                        "Leg][Walk:8][MassFlesh:15:1000]["
                        "Dependant:LowerTorso]}");
                    critter.body.bodyParts.append(
                        "\n{[Name:Right "
                        "Leg][Walk:8][MassFlesh:15:1000]["
                        "Dependant:LowerTorso]}");

                    critter.body.bodyParts.append(
                        "\n{[Name:Left "
                        "Arm][Grasp:2][MassFlesh:10:1000]"
                        "[Dependant:UpperTorso]}");
                    critter.body.bodyParts.append(
                        "\n{[Name:Right "
                        "Arm][Grasp:2][MassFlesh:10:1000]"
                        "[Dependant:UpperTorso]}");
                }
            }

            std::string imagery = stringFindString(line, "[Image:");
            if (gvars::debug)
            {
                std::cout << "Pre Imagery \n";
            }
            for (auto const &image : texturemanager.textures)
            {
                if (image.name == imagery)
                {
                    critter.img.setTexture(image.texture);
                    //Critter.img.SetSubRect(sf::IntRect(0, 0, i->Image.GetWidth(),i->Image.GetHeight()));
                    //Critter.img.SetCenter(i->Image.GetWidth()/2,i->Image.GetHeight()/2);
                }
            }

            debug("Performing Grasper Initialization");
            initializeGraspers(critter);

            if (critter.name != "Debuggery")
            {
                globalCritter.push_back(critter);
            }
            if (gvars::debug)
            {
                std::cout << "Post Imagery \n";
            }
        }
    }
}



MakeSquad::MakeSquad()
{
    makeSquadPoints = 1000;
    squadMates = 3;
    aim = 0;
}

std::set<int> npcTrace(int xa, int ya, int xb, int yb, int id,
                       std::set<int> /*exceptions*/)
{ //.at(0) = Item/NPC(23/69) .at(1) = id

    int dx = xb - xa, dy = yb - ya, steps;
    float xIncrement, yIncrement, x = xa, y = ya;
    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);
    xIncrement = dx / (float)steps;
    yIncrement = dy / (float)steps;
    std::set<int> setID;

    sf::Lock lock(mutex::npcList);

    for (int k = 0; k < steps; k++)
    {

        x += xIncrement;
        y += yIncrement;
        if (inputState.key[Key::G])
        {
            shapes.createCircle(x, y, 1, sf::Color::Blue);
        }

        // Merely doing this so I can reuse the same code, but for items, Hehe.
        {
            //std::vector<Npc>::iterator me;

            //for (me = npclist.begin(); me != npclist.end(); ++me)
            for (auto &me : npclist)
            {
                if (math::closeish(x, y, me.xpos, me.ypos) <= me.size &&
                    me.id != id)
                {

                    try
                    {
                        setID.insert(getNpcVectorId(me.id));
                    }
                    catch (std::exception &e)
                    {
                        std::cout << "SetError\n";
                    }

                } //23 = Item, 69 = NPC
            }
        }
    }
    if (setID.empty())
    {
        setID.insert(-1);
    }
    return setID;
}

void saveNPC(int planet, sf::Vector2i region, Npc &critter)
{
    using namespace std;
    string line("data/maps/Planet");
    stringstream convert;
    convert << planet;
    line.append(convert.str());
    convert.clear();
    galaxy_mkdir(line);
    line.append("/npc");
    convert << "x";
    convert << region.x;
    convert << "y";
    convert << region.y;
    line.append(convert.str());
    string ending(".crit");
    line.append(ending);

    ofstream file;
    file.open(line.c_str(), fstream::in | fstream::ate);
    debug("looking for file...");

    if (file.is_open())
    {
        //File << std::endl;
        file << std::endl;
        con("Problem? 3");
        file << "[name:" << critter.name << "]"
             << "[race:" << critter.race << "]"
             << "[xpos:" << critter.xpos << "]"
             << "[ypos:" << critter.ypos << "]"
             << "[strength:" << critter.attributes.strength << "]"
             << "[perception:" << critter.attributes.perception << "]"
             << "[intelligence:" << critter.attributes.intelligence << "]"
             << "[charisma:" << critter.attributes.charisma << "]"
             << "[endurance:" << critter.attributes.endurance << "]"
             << "[dexterity:" << critter.attributes.dexterity << "]"
             << "[agility:" << critter.attributes.agility << "]"
             << "[health:" << critter.health << "]"
             //<< "[action:" << critter.action << "]"
             << "[angle:" << critter.angle << "]"
             << "[thirst:" << critter.thirst << "]"
             << "[hunger:" << critter.hunger << "]"
             << "[strengthxp:" << critter.attributes.strengthxp << "]"
             << "[perceptionxp:" << critter.attributes.perceptionxp << "]"
             << "[intelligencexp:" << critter.attributes.intelligencexp << "]"
             << "[charismaxp:" << critter.attributes.charismaxp << "]"
             << "[endurancexp:" << critter.attributes.endurancexp << "]"
             << "[dexterityxp:" << critter.attributes.dexterityxp << "]"
             << "[agilityxp:" << critter.attributes.agilityxp << "]"
             << "[cbaseid:" << critter.cbaseid << "]"
             << "[maxhealth:" << critter.maxhealth << "]"
             << "{Tags:" << critter.tags << "}"
             << "{Blood:" << critter.bloodcontent << "}"
             << "{Items:";
        con("Problem? 4");

        //for(int i = 0; i != Critter.inventory.size(); i++)
        for (auto i = critter.inventory.begin(); i != critter.inventory.begin();
             i++)
        {
            file << "[" << (*i).name << "]";
        }
        file << "}";
        con("Problem? 5");
        //Con("Added", false);
        //Con(object.name);
        file.close();
    } // End to Load Map

    /*debug("v-Adding Tags-v");
                debug(StringFindChaos(line,"{Tags:","}"));
                debug("^-Added Tags-^");
                Critter.tags.append(StringFindChaos(line,"{Tags:","}"));
                std::set<std::string> Items;
                std::string List;
                List.append(StringFindChaos(line,"{Items:","}"));
                Items = StringFindSetChaos(List,"[","]");
                for(std::set<std::string>::iterator i = Items.begin(); i != Items.end(); i++)
                {
                    std::string OogaBooga = *i;
                    char *a=new char[OogaBooga.size()+1];a[OogaBooga.size()]=0;memcpy(a,OogaBooga.c_str(),OogaBooga.size());
                    if(Debug){ std::cout << "Pre Critter.AddItem \n";}
                    Critter.AddItem(a,1);
                    if(Debug){ std::cout << "*Post Critter.AddItem* \n";}
                }*/

    else
    {
        ofstream outputFile(line.c_str());

        outputFile << "[name:" << critter.name << "]"
                   << "[race:" << critter.race << "]"
                   << "[xpos:" << critter.xpos << "]"
                   << "[ypos:" << critter.ypos << "]"
                   << "[strength:" << critter.attributes.strength << "]"
                   << "[perception:" << critter.attributes.perception << "]"
                   << "[intelligence:" << critter.attributes.intelligence << "]"
                   << "[charisma:" << critter.attributes.charisma << "]"
                   << "[endurance:" << critter.attributes.endurance << "]"
                   << "[dexterity:" << critter.attributes.dexterity << "]"
                   << "[agility:" << critter.attributes.agility << "]"
                   << "[health:" << critter.health << "]"
                   //<< "[action:" << critter.action << "]"
                   << "[angle:" << critter.angle << "]"
                   << "[thirst:" << critter.thirst << "]"
                   << "[hunger:" << critter.hunger << "]"
                   << "[strengthxp:" << critter.attributes.strengthxp << "]"
                   << "[perceptionxp:" << critter.attributes.perceptionxp << "]"
                   << "[intelligencexp:" << critter.attributes.intelligencexp << "]"
                   << "[charismaxp:" << critter.attributes.charismaxp << "]"
                   << "[endurancexp:" << critter.attributes.endurancexp << "]"
                   << "[dexterityxp:" << critter.attributes.dexterityxp << "]"
                   << "[agilityxp:" << critter.attributes.agilityxp << "]"
                   << "[cbaseid:" << critter.cbaseid << "]"
                   << "[maxhealth:" << critter.maxhealth << "]"
                   << "{Tags:" << critter.tags << "}"
                   << "{Blood:" << critter.bloodcontent << "}"
                   << "{Items:";
        for (auto i = critter.inventory.begin(); i != critter.inventory.begin();
             i++)
        {
            outputFile << "[" << (*i).name << "]";
        }
        outputFile << "}";
    }
}

Territory::Territory()
{
    toDelete = false;
}

void cleanTerritories(std::vector<Territory> &territories)
{
    bool done = false;
    while (done == false)
    {
        done = true;
        for (auto it = territories.begin(); it != territories.end(); ++it)
        {
            if (it->toDelete)
            {
                done = false;
                territories.erase(it);
                debug("Territory deleted.");
                break;
            }
        }
    }
}

Faction::Faction()
    : creativity{}, aggressiveness{}, techAgriculture{}, techArchitecture{},
      techEnergyProduction{}, techWeaponryMass{}, techWeaponryEnergy{},
      techWeaponryExplosive{}, techWeaponrySharp{}, techWeaponryBlunt{},
      techDiplomacy{}, techMedical{}, techRobotics{}
{
    playerControlled = false;
    initialized = false;

    members = 0;
    credits = 0;

    primaryColor = sf::Color(randz(0, 255), randz(0, 255), randz(0, 255));
    secondaryColor = sf::Color(randz(0, 255), randz(0, 255), randz(0, 255));
}

int Faction::getFactionRelations(std::string factionName)
{
    for(auto &faction : factRelations)
    {
        if(factionName == faction.faction)
            return faction.appeal;
    }

    return 0;
}

int factionMembers(std::string factionName)
{
    for (auto &i : uniFact)
    {
        if (i.name == factionName)
        {
            return i.members;
        }
    }
    return 0;
}

float factionAggression(std::string factionName)
{
    for (auto &i : uniFact)
    {
        if (i.name == factionName)
        {
            return i.aggressiveness;
        }
    }
    return 0;
}

int factionTerritories(std::string factionName)
{
    for (auto &i : uniFact)
    {
        if (i.name == factionName)
        {
            return i.territories.size();
        }
    }
    return 0;
}

float factionPower(std::string factionName)
{
    for (auto &i : uniFact)
    {
        if (i.name == factionName)
        {
            float tenantTech = 0;

            tenantTech += i.techMedical;
            tenantTech += i.techWeaponryBlunt;
            tenantTech += i.techWeaponryEnergy;
            tenantTech += i.techWeaponryExplosive;
            tenantTech += i.techWeaponryMass;
            tenantTech += i.techWeaponrySharp;

            float tenantTechnique =
                tenantTech +
                (tenantTech * (percentIs(i.creativity, 35) * 0.01));

            float tenantPower =
                tenantTechnique * (percentIs(i.members, 60) * 0.01);

            tenantPower = tenantPower * 8;

            return tenantPower;
        }
    }
    return 0;
}

int factionPopulation()
{
    int pop = 0;
    for (auto &i : uniFact)
    {
        pop += i.members;
    }
    return pop;
}

void initializeFactions(int generateMax)
{
    for (int i = 0; i != generateMax; i++)
    {
        Faction genFact;

        genFact.name = generateName();
        genFact.playerControlled = false;

        genFact.members = randz(1, 20);

        genFact.primaryColor =
            sf::Color(randz(0, 255), randz(0, 255), randz(0, 255));
        genFact.secondaryColor =
            sf::Color(randz(0, 255), randz(0, 255), randz(0, 255));

        genFact.aggressiveness = randz(0, 100);
        genFact.creativity = randz(0, 100);

        genFact.techAgriculture = randz(0, 100);
        genFact.techArchitecture = randz(0, 100);
        genFact.techDiplomacy = randz(0, 100);
        genFact.techEnergyProduction = randz(0, 100);
        genFact.techMedical = randz(0, 100);
        genFact.techRobotics = randz(0, 100);
        genFact.techWeaponryBlunt = randz(0, 100);
        genFact.techWeaponryEnergy = randz(0, 100);
        genFact.techWeaponryExplosive = randz(0, 100);
        genFact.techWeaponryMass = randz(0, 100);
        genFact.techWeaponrySharp = randz(0, 100);

        uniFact.push_back(genFact);
    }
}

Npc *getGlobalCritter(std::string strtype)
{
    for (auto &elem : npcmanager.globalCritter)
    {
        if (elem.name == strtype)
        {
            elem.id = gvars::globalid++;
            return &elem;
        }
    }
    debug("Returning nullptr from getGlobalCritter");
    return nullptr;
}



void spawnCritter(std::string object, int xpos, int ypos, int zpos)
{
    if (gvars::debug)
    {
        std::cout << "Spawning" << object << " \n";
    }
    Npc var;
    var = *getGlobalCritter(object);
    // var.ReCreate();
    var.id = gvars::globalid++;
    var.xpos = xpos;
    var.ypos = ypos;
    var.zpos = zpos;
    var.reCreateSkills();

    npcmanager.addedCritters.push_back(var);
    if (gvars::debug)
    {
        std::cout << "Done Spawning. \n";
    }

    /*bool searching = true;
    while( searching )
    {
        if( itemmanager.GlobalItem[i].name == Object && t != itemmanager.GlobalItem.end())
        {
            doThings();
            searching=false;
        }
    i++;
    t++;
    }*/

    /*for(i = itemmanager.GlobalItem.begin(); i != itemmanager.GlobalItem.end(); i++)
    {
        if(i->name == Object)
        {
            cItem var;
            var = *i;
            var.id = globalid++;
            var.xpos = xpos;
            var.ypos = ypos;
            worlditems.push_back(var);
            break;
        }
    }*/
}

void buildStartingCritters(int zedAmount)
{
    {
        if (gvars::debug)
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
            for (size_t count = 0; count != squady.squad.size(); count++)
            {
                sf::Vector2f vPos = math::circleRandz(1000, 1000, 180);
                //SpawnCritter("Human",vPos.x,vPos.y);
                squady.squad.at(count).xpos = vPos.x;
                squady.squad.at(count).ypos = vPos.y;

                squady.squad[count].faction = listAt(uniFact,0).name;
                squady.squad[count].factionPtr = &listAt(uniFact,0);

                npcmanager.addedCritters.push_back(squady.squad.at(count));
            }

            for (int zeds = 0; zeds != zedAmount; zeds++)
            {
                con("Starting Zed");
                sf::Vector2f vPos = math::circleRandz(1000, 1000, 580);
                spawnCritter("Zombie", vPos.x, vPos.y, 30*20);
                con("Ending Zed");
            }
        }
        catch (std::exception &e)
        {
            std::cout << "Problem during initalization of NPC Spawns. \n";
        }
        if (gvars::debug)
        {
            std::cout << "PostInt\n";
        }
    }
}

std::string loadCritters(sf::Vector2i worldPos, std::string direction,
                         int planet)
{

    std::string line("data/maps/Planet");
    std::stringstream convert;
    convert << planet;
    line.append(convert.str());
    convert.clear();
    galaxy_mkdir(line);
    line.append("/npc");
    convert << "x";
    convert << worldPos.x;
    convert << "y";
    convert << worldPos.y;
    line.append(convert.str());
    std::string ending(".crit");
    line.append(ending);

    std::ifstream input(line.c_str());
    if (input.is_open())
    {
        while (input.good())
        {
            std::string line;
            getline(input, line);
            Npc critter;

            critter.name = "Debuggery";

            critter.name = stringFindString(line, "[name:");
            critter.race = stringFindString(line, "[race:");
            if (critter.name != "Debuggery" && critter.name != "Zombie")
                critter = *getGlobalCritter(critter.race);
            if (critter.name == "Zombie")
                critter = *getGlobalCritter("Zombie");
            critter.id = gvars::globalid++;
            critter.name = stringFindString(line, "[name:");
            critter.race = stringFindString(line, "[race:");
            critter.xpos = stringFindNumber(line, "[xpos:");
            critter.cbaseid = stringFindNumber(line, "[cbaseid:");
            critter.ypos = stringFindNumber(line, "[ypos:");
            critter.health = stringFindNumber(line, "[health:");
            critter.maxhealth = stringFindNumber(line, "[maxhealth:");
            //critter.action = stringFindString(line, "[action:");
            critter.angle = stringFindNumber(line, "[angle:");
            critter.thirst = stringFindNumber(line, "[thirst:");
            critter.hunger = stringFindNumber(line, "[hunger:");
            con("Problem?");
            critter.tags.clear();
            critter.tags.append(stringFindChaos(line, "{Tags:", "}"));
            critter.bloodcontent.append(stringFindChaos(line, "{Blood:", "}"));
            con("Problem? 2");

            /*debug("v-Adding Tags-v");
                debug(StringFindChaos(line,"{Tags:","}"));
                debug("^-Added Tags-^");
                Critter.tags.append(StringFindChaos(line,"{Tags:","}"));
                std::set<std::string> Items;
                std::string List;
                List.append(StringFindChaos(line,"{Items:","}"));
                Items = StringFindSetChaos(List,"[","]");
                for(std::set<std::string>::iterator i = Items.begin(); i != Items.end(); i++)
                {
                    std::string OogaBooga = *i;
                    char *a=new char[OogaBooga.size()+1];a[OogaBooga.size()]=0;memcpy(a,OogaBooga.c_str(),OogaBooga.size());
                    if(Debug){ std::cout << "Pre Critter.AddItem \n";}
                    Critter.AddItem(a,1);
                    if(Debug){ std::cout << "*Post Critter.AddItem* \n";}
                }*/

            std::cout << "Xpos: " << critter.xpos << "Ypos: " << critter.ypos
                      << std::endl;

            if (direction == "TopLeft")
            {
            }
            if (direction == "Top")
            {
                critter.xpos += 640;
            }
            if (direction == "TopRight")
            {
                critter.xpos += 640;
                critter.xpos += 640;
            }
            if (direction == "Right")
            {
                critter.xpos += 640;
                critter.xpos += 640;
                critter.ypos += 640;
            }
            if (direction == "BottomRight")
            {
                critter.xpos += 640;
                critter.xpos += 640;
                critter.ypos += 640;
                critter.ypos += 640;
            }
            if (direction == "Bottom")
            {
                critter.xpos += 640;
                critter.ypos += 640;
                critter.ypos += 640;
            }
            if (direction == "BottomLeft")
            {
                critter.ypos += 640;
                critter.ypos += 640;
            }
            if (direction == "Left")
            {
                critter.ypos += 640;
            }
            std::cout << "Xpos: " << critter.xpos << "Ypos: " << critter.ypos
                      << std::endl;

            critter.attributes.strength = stringFindNumber(line, "[strength:");
            critter.attributes.perception = stringFindNumber(line, "[perception:");
            critter.attributes.intelligence =
                stringFindNumber(line, "[intelligence:");
            critter.attributes.charisma = stringFindNumber(line, "[charisma:");
            critter.attributes.endurance = stringFindNumber(line, "[endurance:");
            critter.attributes.dexterity = stringFindNumber(line, "[dexterity:");
            critter.attributes.agility = stringFindNumber(line, "[agility:");

            critter.attributes.strengthxp = stringFindNumber(line, "[strengthxp:");
            critter.attributes.perceptionxp =
                stringFindNumber(line, "[perceptionxp:");
            critter.attributes.intelligencexp =
                stringFindNumber(line, "[intelligencexp:");
            critter.attributes.charismaxp = stringFindNumber(line, "[charismaxp:");
            critter.attributes.endurancexp =
                stringFindNumber(line, "[endurancexp:");
            critter.attributes.dexterityxp =
                stringFindNumber(line, "[dexterityxp:");
            critter.attributes.agilityxp = stringFindNumber(line, "[agilityxp:");

            /*std::string Imagery = StringFindString(line,"[Image:");
                std::vector<cImageHolder>::iterator i;
                if(Debug){ std::cout << "Pre Imagery \n";}
                for(i = imagemanager.GlobalImage.begin(); i != imagemanager.GlobalImage.end(); i++)
                {
                    if(i->name == Imagery)
                    {
                        Critter.img.setTexture(i->Image);
                    }
                }*/

            if (critter.name != "Debuggery")
            {
                sf::Lock lock(mutex::npcList);
                npclist.push_back(critter);
            }
        }
    }
    return line;
}

void boom(int xpos, int ypos, int damage, int size)
{
    shapes.createCircle(xpos, ypos, size, sf::Color(255, 0, 0, 150), 0,
                         sf::Color(0, 0, 0));
    std::vector<Npc>::iterator me;
    sf::Lock lock(mutex::npcList);
    //for (me = npclist.begin(); me != npclist.end(); ++me)
    for (auto &me : npclist)
    {
        if (math::closeish(xpos, ypos, me.xpos, me.ypos) < size)
        {
            me.modhealth(-damage);
        }
    }
}

void squadHud()
{ // This prints that "pretty" little Squad Unit display in the top left.
    try
    {
        sf::Lock lock(mutex::npcList);
        //for (size_t i = 0; i != npclist.size(); i++)
        int iterNum = 0;
        for (auto &i : npclist)
        {
            iterNum++;
            if (i.name != "debug")
            {

                shapes.createSquare(
                    gvars::topLeft.x + (20), gvars::topLeft.y + (20 * iterNum),
                    gvars::topLeft.x + 20 + (20),
                    gvars::topLeft.y + 20 + (20 * iterNum), sf::Color::Black);

                std::string output =
                    i.name;
                textList.createText(gvars::topLeft.x + 20 + (20),
                                    gvars::topLeft.y + (20 * iterNum), 12,
                                    sf::Color::White, output);
            }
        }
    }
    catch (std::exception &e)
    {
        std::cout << "\n \n Something went wrong in SquadHud \n \n";
    }
}

void removeNPCs(std::list<Npc> &NPCLIST, sf::Mutex &npcmutex)
{
    sf::Lock lock(npcmutex);
    bool done = false;
    while (done == false)
    {
        bool yet = false;
        for (auto it = NPCLIST.begin(); it != NPCLIST.end(); ++it)
        {
            if (it->toDelete)
            {
                std::cout << it->name << " to be deleted. \n";
                NPCLIST.erase(it);
                yet = true;
                break;
            }
        }
        if (yet == false)
        {
            done = true;
        }
    }
}

void addInitialFaction()
{
    Faction g_pf;

    g_pf.name = "The Alphas";
    g_pf.playerControlled = true;
    g_pf.initialized = true;
    uniFact.push_back(g_pf);
    conFact = &listAt(uniFact,0);
}

Faction * addFaction(std::string name)
{
    Faction g_pf;

    g_pf.name = name;
    g_pf.playerControlled = true;
    g_pf.initialized = true;
    uniFact.push_back(g_pf);
    //return &listAt(uniFact,uniFact.size()-1);
    return &(uniFact.back());
}

void addMembers(int amount, std::string faction)
{
    for (auto &fact : uniFact)
    {
        if(fact.name == faction)
        {
            for(int i = 0; i != amount; i++)
            {
                Npc member;
                member = *getGlobalCritter("Human");
                member.faction = faction;
                member.factionPtr = &fact;
                member.xpos = ((GRIDS*GRID_SIZE)/2)+randz(-80,80);
                member.ypos = ((GRIDS*GRID_SIZE)-200)+randz(-20,20);
                member.zpos = (1*GRID_SIZE);
                member.id = gvars::globalid++;
                npclist.push_back(member);
            }
        }
    }
}

void offloadNpcs()
{
    for (auto &i : npclist)
                {

                    if (i.xpos > 1920 && i.ypos < 640)
                    {
                        i.xpos =
                            i.xpos - 640 - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx + 2,
                                                  gvars::currentregiony - 1),
                                i);
                        i.toDelete = true;
                    }
                    else if (i.xpos > 1920 &&
                             i.ypos > 1280)
                    {
                        i.xpos =
                            i.xpos - 640 - 640 - 640;
                        i.ypos = i.ypos - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx + 2,
                                                  gvars::currentregiony + 1),
                                i);
                        i.toDelete = true;
                    }

                    else if (i.xpos < 0 &&
                             i.ypos > 1280)
                    {
                        i.xpos = i.xpos + 640;
                        i.ypos = i.ypos - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx - 2,
                                                  gvars::currentregiony + 1),
                                i);
                        i.toDelete = true;
                    }
                    else if (i.xpos < 0 && i.ypos < 640)
                    {
                        i.xpos = i.xpos + 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx - 2,
                                                  gvars::currentregiony - 1),
                                i);
                        i.toDelete = true;
                    }

                    else if (i.ypos < 0 &&
                             i.xpos > 1280)
                    {
                        i.xpos = i.xpos - 640 - 640;
                        i.ypos = i.ypos + 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx + 1,
                                                  gvars::currentregiony - 2),
                                i);
                        i.toDelete = true;
                    }
                    else if (i.ypos < 0 && i.xpos < 640)
                    {
                        i.ypos = i.ypos + 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx - 1,
                                                  gvars::currentregiony - 2),
                                i);
                        i.toDelete = true;
                    }

                    else if (i.ypos > 1920 &&
                             i.xpos > 1280)
                    {
                        i.xpos = i.xpos - 640 - 640;
                        i.ypos =
                            i.ypos - 640 - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx + 1,
                                                  gvars::currentregiony + 2),
                                i);
                        i.toDelete = true;
                    }
                    else if (i.ypos > 1920 &&
                             i.xpos < 640)
                    {
                        i.ypos =
                            i.ypos - 640 - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx - 1,
                                                  gvars::currentregiony + 2),
                                i);
                        i.toDelete = true;
                    }

                    //HAGGINABAGGINA  Some reason, When a critter is saved, It'll have more than 640 for it's position, This is unacceptable.

                    else if (i.xpos > 1920)
                    {
                        i.xpos =
                            i.xpos - 640 - 640 - 640;
                        i.ypos = i.ypos - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx + 2,
                                                  gvars::currentregiony),
                                i);
                        i.toDelete = true;
                    }
                    else if (i.ypos > 1920)
                    {
                        i.xpos = i.xpos - 640;
                        i.ypos =
                            i.ypos - 640 - 640 - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx,
                                                  gvars::currentregiony + 2),
                                i);
                        i.toDelete = true;
                    }
                    else if (i.xpos < 0)
                    {
                        i.xpos = i.xpos + 640;
                        i.ypos = i.ypos - 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx - 2,
                                                  gvars::currentregiony),
                                i);
                        i.toDelete = true;
                    }
                    else if (i.ypos < 0)
                    {
                        i.xpos = i.xpos - 640;
                        i.ypos = i.ypos + 640;
                        saveNPC(500, sf::Vector2i(gvars::currentregionx,
                                                  gvars::currentregiony - 2),
                                i);

                        i.toDelete = true;
                    }
                }
}

void attractNPCs(sf::Vector2f position)
{
    if(inputState.key[Key::B].time == 1)
    {
        sf::Lock lock(mutex::npcList);
        for(auto &npc : npclist)
        {
            sf::Vector2f npcPos(npc.xpos,npc.ypos);
            sf::Vector2f Alter = npcPos - position;
            npc.momentum += Alter;
        }
    }
    if(inputState.key[Key::N].time == 1)
    {
        sf::Lock lock(mutex::npcList);
        for(auto &npc : npclist)
        {
            sf::Vector2f npcPos(npc.xpos,npc.ypos);
            sf::Vector2f Alter = position - npcPos;
            npc.momentum += Alter;
        }
    }
}

Npc *getCritter(int id)
{
    if (gvars::debug)
    {
        std::cout << "Getting critter(" << id << ") \n";
    }
    sf::Lock lock(mutex::npcList);
    for (auto &elem : npclist)
    {
        if (elem.id == id)
        {
            if (gvars::debug)
            {
                std::cout << "Found critter(" << id << ") \n";
            }
            return &elem;
        }
    }
    if (gvars::debug)
    {
        std::cout << "Didn't Find critter(" << id << ") \n";
    }
    return nullptr;
}

void selectedNPCprocess()
{

    for(auto &selected : selectedNPCs)
    {
        sf::Vector2f Pos = selected->getPos2d();
        shapes.createCircle(Pos.x, Pos.y, 5,
                                sf::Color(0, 255, 255, 100));

        shapes.createImageButton(Pos,texturemanager.getTexture("SelectionCircle.png"),"",gvars::constantRotation);

    }

}

void drawInventory(sf::Vector2f vPos, std::list<Item> &inventory)
{
    shapes.createSquare(vPos.x,vPos.y,vPos.x+(20*inventory.size()),vPos.y-20,sf::Color(0,0,0),2,sf::Color::Cyan);
        int itemCount = 0;
        for(auto &i : inventory)
        {
            //i.img.setPosition(vPos.x+(20*itemCount)+5,vPos.y);
            //shapes.createCircle(vPos.x+(20*itemCount),vPos.y,20,gvars::cycleGreen);
            //i.drawImg();
            int butt = shapes.createImageButton(sf::Vector2f(vPos.x+(20*itemCount)+10,vPos.y-10),
                              texturemanager.getTexture(i.name + ".png")  //(i.img.getTexture())
                              ,i.name);
            textList.createText(vPos.x+(20*itemCount)+5,vPos.y-5,10,sf::Color::Yellow,std::to_string(i.amount));
            if(shapes.shapeHovered(butt))
            {
                textList.createText(vPos.x+(20*itemCount),vPos.y-40,15,sf::Color::Cyan,i.name);
                if(!i.internalitems.empty())
                {
                    drawInventory(sf::Vector2f(vPos.x,vPos.y-40),i.internalitems);
                }
            }
            itemCount++;
        }
}





void drawSelectedCritterHUD()
{
    if(!selectedNPCs.empty() && bountytower::bountytower)
    {
        // restore the default view
        window.setView(gvars::hudView);

        sf::Sprite CIH;
        sf::Texture Tex;
        sf::Vector2u TexSize = texturemanager.getTexture("CritterInventoryHud.png").getSize();
        CIH.setTexture(texturemanager.getTexture("CritterInventoryHud.png"));
        CIH.setPosition(0,0);
        sf::Color curColor = CIH.getColor();
        //curColor.a = 200;
        CIH.setColor(curColor);

        window.draw(CIH);

        sf::Texture * slotTex = &texturemanager.getTexture("BeltSlot.png");
        sf::Sprite slotSprite;
        slotSprite.setTexture(*slotTex);

        for (int i = 0; i != selectedNPCs[0]->getInventoryMax(); i++)
        {
            sf::Vector2f vPos = gvars::slotPos[i];
            if(i > 1)
            {
                slotSprite.setPosition(vPos.x-(slotTex->getSize().x/2),vPos.y-(slotTex->getSize().y/2) );
                window.draw(slotSprite);
            }




            Item * slotItem = selectedNPCs[0]->invSlots[i];
            if(slotItem != nullptr)
            {
                if(slotItem->img.getTexture() == nullptr)
                {
                    std::cout << "Breaking on Item GetTexture() of slot:" << i << std::endl;
                    break;
                }

                sf::Sprite SP;
                SP.setTexture(*slotItem->img.getTexture());
                SP.setPosition(vPos);
                SP.setOrigin(SP.getTexture()->getSize().x/2,SP.getTexture()->getSize().y/2);

                //Don't draw the item in the slot while we're holding it.
                if(mouseItem == nullptr || mouseItem != nullptr && mouseItem->id != slotItem->id)
                    window.draw(SP);

                sf::Vector2f uPos(vPos.x-20,vPos.y+5);
                int amount = slotItem->amount;
                sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), gvars::hudView);

                if(math::closeish(worldPos,vPos) <= 15)
                {
                    std::string additionalInfo;
                    if(slotItem->type == 2)
                    {
                        int dur = slotItem->durabilityCounter;
                        int totalDur = slotItem->getDurability();

                        additionalInfo.append("\nDur:"+str(dur)+"/"+str(totalDur));
                    }

                    shapes.createText(gvars::mousePos,10,sf::Color::White,"      " + slotItem->name + additionalInfo);
                }



                std::string outPut;// = slotItem->name;
                if(amount > 1)
                    outPut.append("\n" + std::to_string(amount));

                if(slotItem->type == 2)
                { // Checking and display ammo count/max.
                    int curAmmo = 0;
                    int maxAmmo = slotItem->getAmmoCapacity();
                    Item * itemAmmo = getItemType(slotItem->internalitems,slotItem->ammotype);
                    if(itemAmmo != nullptr)
                        curAmmo = itemAmmo->amount;

                    outPut.append("\n("+str(curAmmo)+"/"+str(maxAmmo)+")");

                    sf::Vector2f buttPos(uPos);
                    buttPos.y -= 25;

                }

                window.draw(drawText(uPos,outPut));

            }
        }

        int invButt = shapes.createImageButton(sf::Vector2f(600,590),texturemanager.getTexture("InventorySlotUpgrade.png"),"",0,&gvars::hudView);
        int skillButt = shapes.createImageButton(sf::Vector2f(680,590),texturemanager.getTexture("SkillsMonster.png"),"",0,&gvars::hudView);



        if(shapes.shapeClicked(invButt))
            squaddieMenu(*selectedNPCs[0]);
        if(shapes.shapeHovered(invButt))
            shapes.createText(gvars::mousePos.x+10,gvars::mousePos.y,10,sf::Color::White,"Overview (i)");
        if(shapes.shapeClicked(skillButt))
            skillMenu(*selectedNPCs[0]);
        if(shapes.shapeHovered(skillButt))
            shapes.createText(gvars::mousePos.x+10,gvars::mousePos.y,10,sf::Color::White,"Skills (u)");

        window.setView(gvars::view1);
    }
}

void runCritterBody(Npc &npc)
{
    /*  BodyPart Loop
        First, Run through the bodyparts finding the 'global' tags, like
            Nutrient Extraction and such.
        Second, Run through each individual part running through all
            the local tags.
    */

    short int parts = 0;
    size_t searchPos = 0;
    size_t endPos = 0;

    debug("Debug: Beginning Part Loop for" + npc.name);

    //  Global Part Tag Variables

    float partsWalkSpeed = 0;
    float globalNutritionPercentage = 100;

    //  *   Global Part Tag Variables   *

    while (searchPos != npc.body.bodyParts.npos) // Global Part Tags
    {
        searchPos = npc.body.bodyParts.find("{", searchPos);

        if (searchPos != npc.body.bodyParts.npos)
        {
            endPos = npc.body.bodyParts.find("}", searchPos);

            std::string workingLine;

            workingLine.append(npc.body.bodyParts, searchPos,
                               endPos - searchPos);
            float partNumber = 0;

            partNumber = stringFindNumber(workingLine, "[Walk:");
            if (partNumber != 0)
            {

                if (partsWalkSpeed != 0)
                {
                    partsWalkSpeed = partNumber;
                }
                else
                {
                    partsWalkSpeed += (partNumber * 0.5);
                }
            }

            partNumber = stringFindNumber(workingLine, "[NutritionExtraction:");
            if (partNumber != 0)
            {
                globalNutritionPercentage += partNumber;
            }

            partNumber = stringFindNumber(workingLine, "[DigestsBlood:");
            if (partNumber != 0)
            {
            }
            partNumber = stringFindNumber(workingLine, "[DigestsFlesh:");
            if (partNumber != 0)
            {
                npc.consumeFlesh = true;
            }
            partNumber = stringFindNumber(workingLine, "[DigestsVeggy:");
            if (partNumber != 0)
            {
                npc.consumeVeggy = true;
            }
            partNumber = stringFindNumber(workingLine, "[DigestsWater:");
            if (partNumber != 0)
            {
                npc.consumeWater = true;
            }

            searchPos = endPos;
        }
    }

    searchPos = 0;

    for (auto &elem : npc.inventory)
    {
        if ((elem).insidePart != "")
        {
            (elem).hasInternalUse--;
        }
    }

    while (searchPos != npc.body.bodyParts.npos) // Individual Part Tags
    {

        searchPos = npc.body.bodyParts.find("{", searchPos);

        if (searchPos != npc.body.bodyParts.npos)
        {
            endPos = npc.body.bodyParts.find("}", searchPos);
            parts++;

            std::string workingLine;

            workingLine.append(npc.body.bodyParts, searchPos,
                               endPos - searchPos);

            float partNumber = 0;
            std::string partString = "";
            Item *partItem;

            std::string currentPart = stringFindString(workingLine, "[Name:");

            partNumber = stringFindNumber(workingLine, "[DigestsBlood:");
            partItem = getItemPtrFromVector(npc.inventory, "Blood");
            if (partNumber != 0 && partItem != nullptr)
            {

                float workAmount = partItem->amount;
                float diff = workAmount - (partNumber / 1000);

                if (diff > 0)
                {
                    partItem->amount = diff;
                    float Nutr = (workAmount - diff) *
                                 100; // TODO: Figure this out better.
                    npc.bloodwork(
                        "Nutrients",
                        Nutr * percentageBuff(globalNutritionPercentage));
                }
                else
                {
                    //*GetItemPtrfromVector(npc.inventory,"Blood").amount = 0;
                    getItemPtrFromVector(npc.inventory, "Blood")->toDelete =
                        true;
                    float nutr =
                        workAmount * 100; // TODO: Figure this out better.
                    npc.bloodwork(
                        "Nutrients",
                        nutr * percentageBuff(globalNutritionPercentage));
                }
            }

            partNumber = stringFindNumber(workingLine, "[DigestsFlesh:");
            partItem =
                getItemPtrfromVectorVarSearch(npc.inventory, "MassFlesh");
            //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
            if (partNumber != 0 && partItem != nullptr &&
                partItem->massFlesh >
                    0) // TODO: Make sure the item is in THIS PART before digesting it!
            {
                //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                float workAmount = partItem->massFlesh;
                float diff = workAmount - (partNumber / 1000);

                if (diff > 0)
                {
                    partItem->massFlesh = diff;
                    partItem->hasInternalUse = 0;
                    float Nutr = (workAmount - diff) *
                                 100; // TODO: Figure this out better.
                    npc.bloodwork(
                        "Nutrients",
                        Nutr * percentageBuff(globalNutritionPercentage));
                }
                if (partItem->massFlesh <= 0)
                {
                    //*GetItemPtrfromVector(npc.inventory,"Blood").amount = 0;
                    //partItem->toDelete = true;
                    //Add Food to everyone, Make sure they go hungry to eat it, Figure out a way to Eject the empty item, Or do water! Everyone starts with water.
                    //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                }
            }

            partNumber = stringFindNumber(workingLine, "[DigestsVeggy:");
            partItem =
                getItemPtrfromVectorVarSearch(npc.inventory, "MassVeggy");
            //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
            if (partNumber != 0 && partItem != nullptr &&
                partItem->massVeggy >
                    0) // TODO: Make sure the item is in THIS PART before digesting it!
            {
                //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                float workAmount = partItem->massVeggy;
                float diff = workAmount - (partNumber / 1000);

                if (diff > 0)
                {
                    partItem->massVeggy = diff;
                    partItem->hasInternalUse = 0;
                    float nutr = (workAmount - diff) *
                                 100; // TODO: Figure this out better.
                    npc.bloodwork(
                        "Nutrients",
                        nutr * percentageBuff(globalNutritionPercentage));
                }
                if (partItem->massVeggy <= 0)
                {
                    //partItem->toDelete = true;
                    //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                }
            }

            partNumber = stringFindNumber(workingLine, "[DigestsWater:");
            partItem =
                getItemPtrfromVectorVarSearch(npc.inventory, "MassWater");
            //if(PartItem != NULL) PartItem->HasInternalUse++; // This is designed to keep items from being ejected until they are completely useless to a critter, I.E. Items with multiple Food Mass's.
            if (partNumber != 0 && partItem != nullptr &&
                partItem->massWater >
                    0) // TODO: Make sure the item is in THIS PART before digesting it!
            {
                //std::cout << "HasInternalUse: " << PartItem->HasInternalUse << std::endl;
                float workAmount = partItem->massWater;
                float diff = workAmount - (partNumber / 1000);

                if (diff > 0)
                {
                    partItem->massWater = diff;
                    partItem->hasInternalUse = 0;
                    float nutr = (workAmount - diff) *
                                 100; // TODO: Figure this out better.
                    npc.bloodwork(
                        "Hydration",
                        nutr * percentageBuff(globalNutritionPercentage));
                }
                if (partItem->massWater <= 0)
                {
                    //partItem->toDelete = true;
                    //npc.bloodwork("Nutrients",Nutr*PercentageBuff(GlobalNutritionPercentage));
                }
            }

            partString = stringFindString(workingLine, "[PoisonFilter:");
            if (partString != "")
            {

                std::vector<std::string> strVec =
                    stringFindElements(partString, ":");
                if (gvars::debug)
                    std::cout << "StrVec[0]: " << strVec[0] << std::endl;
                float leftover =
                    npc.bloodwork(strVec[0], -atof(strVec[1].c_str()));
                if (gvars::debug)
                    std::cout << "Bloodwork leftover is: " << leftover
                              << std::endl;
                //NPC Critter;

                for (size_t i = 0; i != strVec.size(); i++)
                {
                    if (gvars::debug)
                        std::cout << strVec[i] << std::endl;
                }
            }

            partNumber = stringFindNumber(workingLine, "[Orafice:");
            if (partNumber > 0)
            {
                //std::vector<item> * Inv = &npc.inventory;

                //for(int i = 0; i != npc.inventory.size(); i++)
                for (auto i = npc.inventory.begin(); i != npc.inventory.end();
                     i++)
                {
                    bool foundIt = false;
                    if ((*i).insidePart == "" && (*i).massFlesh > 0 &&
                        npc.consumeFlesh) // Awww yeeessss, We gonna eat some flesh with our Orafice!
                    {
                        (*i).insidePart = currentPart;
                        foundIt = true;
                    }

                    if ((*i).insidePart == "" && (*i).massVeggy > 0 &&
                        npc.consumeVeggy) // Awww yeeessss, We gonna eat something with our Orafice!
                    {
                        (*i).insidePart = currentPart;
                        foundIt = true;
                    }

                    if ((*i).insidePart == "" && (*i).massWater > 0 &&
                        npc.consumeWater) // Awww yeeessss, We gonna eat something with our Orafice!
                    {
                        (*i).insidePart = currentPart;
                        foundIt = true;
                    }

                    if (foundIt)
                    {
                        std::string chtStr;
                        chtStr.append("* ");
                        chtStr.append(npc.name);
                        chtStr.append("(" + std::to_string(npc.id) + ")");
                        chtStr.append(" has inserted ");
                        chtStr.append((*i).name);
                        chtStr.append(" into their ");
                        chtStr.append(currentPart);
                        chtStr.append("'s Orafice(");
                        chtStr.append(std::to_string(partNumber));
                        chtStr.append(").");

                        chatBox.addChat(chtStr, sf::Color(150, 150, 0));
                    }
                }
            }

            partNumber = stringFindNumber(
                workingLine, "[BloodPumpRate:"); // TODO: Do this right.
            if (partNumber != 0)
            {
                float blood = stringFindNumber(npc.bloodcontent, "[Nutrients:");
                if (blood > partNumber)
                {
                    if ((npc.maxhunger - npc.hunger) > partNumber)
                    {
                        npc.hunger += partNumber;
                        npc.bloodwork("Nutrients", -partNumber);
                    }
                }
                blood = stringFindNumber(npc.bloodcontent, "[Hydration:");
                if (blood > partNumber)
                {
                    if ((npc.maxthirst - npc.thirst) > partNumber)
                    {
                        npc.thirst += partNumber;
                        npc.bloodwork("Hydration", -partNumber);
                    }
                }
            }
            searchPos = endPos;
        }
    }
}

void drawEquippedItems(Npc &npc)
{
    if(npc.invSlots[0] != nullptr)
    {
        sf::Vector2f drawPos = math::angleCalc(npc.getPos2d(),npc.angle,35);


        sf::Sprite itemSprite = npc.invSlots[0]->img;
        itemSprite.setPosition(drawPos);
        itemSprite.setRotation(npc.angle+180);

        window.draw(itemSprite);
    }

    if(npc.invSlots[1] != nullptr)
    {
        sf::Vector2f drawPos = math::angleCalc(npc.getPos2d(),npc.angle+180,35);


        sf::Sprite itemSprite = npc.invSlots[1]->img;
        itemSprite.setPosition(drawPos);
        itemSprite.setRotation(npc.angle+180);

        window.draw(itemSprite);
    }
}

void Npc::setupAnimations()
{
    sf::Texture * spriteSheet = &texturemanager.getTexture("SpriteSheet"+race+".png");
    bodyAnimation.aniLeftWalk.setSpriteSheet(*spriteSheet);
    bodyAnimation.aniDownWalk.setSpriteSheet(*spriteSheet);
    bodyAnimation.aniRightWalk.setSpriteSheet(*spriteSheet);
    bodyAnimation.aniUpWalk.setSpriteSheet(*spriteSheet);

    bodyAnimation.aniDownWalk.addFrame(sf::IntRect( 0, 0, 60, 60));
    bodyAnimation.aniDownWalk.addFrame(sf::IntRect(60, 0, 60, 60));
    bodyAnimation.aniDownWalk.addFrame(sf::IntRect( 0, 0, 60, 60));
    bodyAnimation.aniDownWalk.addFrame(sf::IntRect(120, 0, 60, 60));

    bodyAnimation.spriteDownWalk.setAnimation(bodyAnimation.aniDownWalk);
    bodyAnimation.spriteDownWalk.setOrigin(30,30);

    bodyAnimation.aniRightWalk.addFrame(sf::IntRect( 0, 60, 60, 60));
    bodyAnimation.aniRightWalk.addFrame(sf::IntRect(60, 60, 60, 60));
    bodyAnimation.aniRightWalk.addFrame(sf::IntRect( 0, 60, 60, 60));
    bodyAnimation.aniRightWalk.addFrame(sf::IntRect(120, 60, 60, 60));
    bodyAnimation.spriteRightWalk.setAnimation(bodyAnimation.aniRightWalk);
    bodyAnimation.spriteRightWalk.setOrigin(30,30);

    bodyAnimation.aniUpWalk.addFrame(sf::IntRect( 0, 120, 60, 60));
    bodyAnimation.aniUpWalk.addFrame(sf::IntRect(60, 120, 60, 60));
    bodyAnimation.aniUpWalk.addFrame(sf::IntRect( 0, 120, 60, 60));
    bodyAnimation.aniUpWalk.addFrame(sf::IntRect(120, 120, 60, 60));
    bodyAnimation.spriteUpWalk.setAnimation(bodyAnimation.aniUpWalk);
    bodyAnimation.spriteUpWalk.setOrigin(30,30);

    bodyAnimation.aniLeftWalk.addFrame(sf::IntRect( 0, 60, 60, 60));
    bodyAnimation.aniLeftWalk.addFrame(sf::IntRect(60, 60, 60, 60));
    bodyAnimation.aniLeftWalk.addFrame(sf::IntRect( 0, 60, 60, 60));
    bodyAnimation.aniLeftWalk.addFrame(sf::IntRect(120, 60, 60, 60));
    bodyAnimation.spriteLeftWalk.setAnimation(bodyAnimation.aniLeftWalk);
    bodyAnimation.spriteLeftWalk.setOrigin(30,30);
    bodyAnimation.spriteLeftWalk.scale(-1,1);

    if(race.find("Noirves") != race.npos)
    {
        bodyAnimation.spriteDownWalk.scale(0.75,0.75);
        bodyAnimation.spriteLeftWalk.scale(0.75,0.75);
        bodyAnimation.spriteRightWalk.scale(0.75,0.75);
        bodyAnimation.spriteUpWalk.scale(0.75,0.75);
    }
}

void Npc::handleStatusEffects()
{

    { // This is done to simplify conditions of returning to normal form.
        race = originalRace;
        racialAbility = originalRacialAbility;
    }

    Item * heldItem;
    for(int i = 0; i != 2; i++)
    { // Applying held item status effects to the carrier.
        if(deadFrames > 5)
            continue;

        if(i == 0)
            heldItem = getLeftHandItem();
        else if(i == 1)
            heldItem = getRightHandItem();

        if(heldItem == nullptr)
            continue;
        if(heldItem->statusEffects.empty())
            continue;

        for(auto &status : heldItem->statusEffects)
            statusEffects.push_back(status);
    }

    Item * carriedItem;
    for(int i = 0; i != getInventoryMax(); i++)
    { // Applying held item status effects to the carrier.
        if(deadFrames > 5)
            continue;

        carriedItem = invSlots[i];

        if(carriedItem == nullptr)
            continue;
        if(carriedItem->statusEffectsCarried.empty())
            continue;

        for(auto &status : carriedItem->statusEffectsCarried)
            statusEffects.push_back(status);
    }


    if(!selectedNPCs.empty() && id == selectedNPCs[0]->id)
    { // Drawing a display of inventory status effects.
        for(int i = 0; i != getInventoryMax(); i++)
        {
            if(invSlots[i] == nullptr)
                continue;


            if(invSlots[i]->statusEffects.empty() && invSlots[i]->statusEffectsInflict.empty() && invSlots[i]->statusEffectsCarried.empty())
                continue;

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), gvars::hudView);
            bool mouseHovering = (math::closeish(gvars::slotPos[i],mousePos) <= 20);

            if(!mouseHovering)
                continue;

            sf::Vector2f mainDrawPos(percentPos(40,0,screen.x()),percentPos(80,0,screen.y()));

            sf::Vector2f drawPos = mainDrawPos;
            drawPos.x += 15;
            shapes.createSquare(drawPos.x,drawPos.y,drawPos.x+500,drawPos.y+(invSlots[i]->statusEffects.size()*20)+(invSlots[i]->statusEffectsInflict.size()*20)+(invSlots[i]->statusEffectsCarried.size()*20),sf::Color::Black,1,sf::Color::Cyan,&gvars::hudView);
            shapes.shapes.back().layer = 9000;
            int total_elements = 0;

            for(auto &status : invSlots[i]->statusEffects)
            {
                std::string outPut;
                outPut.append("Equip:[" + status.name + ", Duration: " + str(status.duration/60) + "]");
                if(inputState.key[Key::LShift])
                    for(auto &aspect : status.aspects)
                {
                    outPut.append("[" + aspectNum[aspect.name] + ", Potency: " + str(static_cast<int>(aspect.potency)) + ", Type: " + aspect.type + "]");
                }

                shapes.createText(sf::Vector2f(drawPos.x,drawPos.y+(20*total_elements)),15,sf::Color::Cyan,outPut,&gvars::hudView);
                shapes.shapes.back().layer = 9001; // IT'S OVER NINE THO-

                total_elements++;
            }
            for(auto &status : invSlots[i]->statusEffectsInflict)
            {
                std::string outPut;
                outPut.append("Inflict:[" + status.name + ", Duration: " + str(status.duration/60) + "]");
                if(inputState.key[Key::LShift])
                    for(auto &aspect : status.aspects)
                {
                    outPut.append("[" + aspectNum[aspect.name] + ", Potency: " + str(static_cast<int>(aspect.potency)) + ", Type: " + aspect.type + "]");
                }

                shapes.createText(sf::Vector2f(drawPos.x,drawPos.y+(20*total_elements)),15,sf::Color::Cyan,outPut,&gvars::hudView);
                shapes.shapes.back().layer = 9001; // IT'S OVER NINE THO-

                total_elements++;
            }
            for(auto &status : invSlots[i]->statusEffectsCarried)
            {
                std::string outPut;
                outPut.append("Carry:[" + status.name + ", Duration: " + str(status.duration/60) + "]");
                if(inputState.key[Key::LShift])
                    for(auto &aspect : status.aspects)
                {
                    outPut.append("[" + aspectNum[aspect.name] + ", Potency: " + str(static_cast<int>(aspect.potency)) + ", Type: " + aspect.type + "]");
                }

                shapes.createText(sf::Vector2f(drawPos.x,drawPos.y+(20*total_elements)),15,sf::Color::Cyan,outPut,&gvars::hudView);
                shapes.shapes.back().layer = 9001; // IT'S OVER NINE THO-

                total_elements++;
            }




        }

    }

    mods.clearAllMods(); // TODO: Whenever performance becomes an issue, Put a single bool on Npc to check if we've gone one frame without any status effects, If so, then call this once.

    if(statusEffects.empty())
    {
        mods.clearAllPostConditions();
        return;
    }




    bool mouseHovering = (math::closeish(getPos2d(),gvars::mousePos) <= 20);
    if(mouseHovering)
    { // Drawing a display of all status effects.
        sf::Vector2f drawPos = gvars::mousePos;
        drawPos.x += 15;
        shapes.createSquare(drawPos.x,drawPos.y,drawPos.x+500,drawPos.y+(statusEffects.size()*20),sf::Color::Black,1,sf::Color::Cyan);
        shapes.shapes.back().layer = 9000;
        int total_elements = 0;

        for(auto &status : statusEffects)
        {
            std::string outPut;
            outPut.append("[" + status.name + ", Duration: " + str(status.duration/60) + "]");
            if(inputState.key[Key::LShift])
                for(auto &aspect : status.aspects)
            {
                outPut.append("[" + aspectNum[aspect.name] + ", Potency: " + str(static_cast<int>(aspect.potency)) + ", Type: " + aspect.type + "]");
            }

            shapes.createText(sf::Vector2f(drawPos.x,drawPos.y+(20*total_elements)),15,sf::Color::Cyan,outPut);
            shapes.shapes.back().layer = 9001; // IT'S OVER NINE THO-

            total_elements++;
        }



    }



    for(auto &status : statusEffects)
    {

        if(status.critterEffect != nullptr)
        { // Drawing status overlay on critter.
            shapes.createImageButton(getPos2d(),*status.critterEffect,"",0);
            shapes.shapes.back().layer = 10;
        }

        if(status.auraRadius > 0)
        {
            shapes.createCircle(xpos,ypos,status.auraRadius,sf::Color(255,255,255,100));
            shapes.shapes.back().layer = 15;
            for(auto &npc : npclist)
            {
                if(!npc.functional())
                    continue;



                int distance = math::distance(getPos(),npc.getPos());
                if(distance > status.auraRadius)
                    continue;




                StatusEffect deliverStatus = status;
                deliverStatus.auraRadius = 0;
                deliverStatus.duration = 1;

                if(status.auraAffectsAllies && npc.faction == faction)
                { // Deliver Aura Allies
                    npc.statusEffects.push_back(deliverStatus);
                    continue;
                }
                bool isEnemy = (factionPtr->getFactionRelations(npc.faction) <= -1000);
                if(status.auraAffectsEnemies && isEnemy)
                { // Deliver Aura Enemies
                    npc.statusEffects.push_back(deliverStatus);
                    continue;
                }

                if(status.auraAffectsNeutrals && npc.faction != faction && !isEnemy)
                { // Deliver Aura Neutrals.
                    npc.statusEffects.push_back(deliverStatus);
                    continue;
                }
            }
        }

        if(status.auraRadius == 0)
        {
            for(auto &aspect : status.aspects)
            {
                if(aspect.name == StatusAspect::ConditionHealth)
                {
                    float aspectPercent = aspect.potency*0.01;
                    float healthCheck = getMaxHealth()*aspectPercent;
                    bool aboveHealth = (health >= healthCheck);
                    bool belowHealth = (health <= healthCheck);

                    if(aspect.type == "Above" && belowHealth)
                        break;
                    if(aspect.type == "Below" && aboveHealth)
                        break;
                }
                if(aspect.name == StatusAspect::ConditionLife)
                {
                    if(aspect.type == "Alive" && !alive)
                        break;
                    if(aspect.type == "Dead" && alive)
                        break;
                }
                if(aspect.name == StatusAspect::ConditionOnItemUse)
                {
                    if(!mods.onItemUse)
                        break;

                    int ItemType;
                    if(aspect.type != "")
                        ItemType = std::stoi(aspect.type);
                    else
                        ItemType = 0;

                    if(ItemType != 0)
                        if(mods.onItemUseType != ItemType)
                            break;
                }
                if(aspect.name == StatusAspect::ConditionOnDeath)
                {
                    if(!mods.onDeath)
                        break;
                }
                if(aspect.name == StatusAspect::ConditionOnHit)
                {
                    if(!mods.onHit)
                        break;
                }
                if(aspect.name == StatusAspect::ConditionOnHitByType)
                {
                    if(!mods.onHit)
                        break;
                    if(mods.onHitType != damageTypes.getNum(aspect.type))
                        break;
                }
                if(aspect.name == StatusAspect::ConditionTimeDelay)
                {
                    if(aspect.potency > 0)
                    {
                        aspect.potency--;
                        break;
                    }
                }


                //std::cout << "Aspect:" << aspectNum[aspect.name] << ", " << StatusAspect::AffectHealth << "/" << aspect.name << " \n";
                if(aspect.name == StatusAspect::AffectHealth) // TODO: Add a pointer to Npc Source of status effect, with a check on floor change to set it to null, so XP can be credited for kills.
                    takeDamage(nullptr,nullptr,-aspect.potency,damageTypes.getNum(aspect.type));





                if(aspect.name == StatusAspect::AffectDamage)
                    mods.affectDamageMod.push_back(StringFloat(aspect.type,aspect.potency));

                if(aspect.name == StatusAspect::Armor)
                    mods.armorMod.push_back(StringFloat(aspect.type,aspect.potency));

                if(aspect.name == StatusAspect::Attribute)
                {
                    if(aspect.type == "Strength")
                        mods.strMod += aspect.potency;
                    if(aspect.type == "Perception")
                        mods.perMod += aspect.potency;
                    if(aspect.type == "Intelligence")
                        mods.intMod += aspect.potency;
                    if(aspect.type == "Charisma")
                        mods.chaMod += aspect.potency;
                    if(aspect.type == "Endurance")
                        mods.endMod += aspect.potency;
                    if(aspect.type == "Dexterity")
                        mods.dexMod += aspect.potency;
                }

                if(aspect.name == StatusAspect::MoveSpeed)
                    mods.moveSpeedMod += aspect.potency;

                if(aspect.name == StatusAspect::Freeze)
                    mods.freezeMod += aspect.potency;

                if(aspect.name == StatusAspect::Sleep)
                    mods.sleepMod += aspect.potency;

                if(aspect.name == StatusAspect::Stun)
                    mods.stunMod += aspect.potency;

                if(aspect.name == StatusAspect::Thorns)
                    mods.thornsMod.push_back(StringFloat(aspect.type,aspect.potency));

                if(aspect.name == StatusAspect::Immunity)
                    mods.immunityMod.push_back(StringFloat(aspect.type,aspect.potency));

                if(aspect.name == StatusAspect::CauseExplosion)
                    explosion(getPos(),aspect.potency*2,aspect.potency);

                if(aspect.name == StatusAspect::ItemFind)
                    mods.itemFindMod += aspect.potency;

                if(aspect.name == StatusAspect::ItemDropRate)
                    mods.itemDropRateMod += aspect.potency;

                if(aspect.name == StatusAspect::XPModifier)
                    mods.xpModifierMod += aspect.potency;

                if(aspect.name == StatusAspect::DeathXPModifier)
                    mods.deathXpModifierMod += aspect.potency;

                if(aspect.name == StatusAspect::SpawnCreature)
                {
                    Npc spawn;
                    spawn = *getGlobalCritter(aspect.type);
                    spawn.xpos = xpos;
                    spawn.ypos = ypos;
                    spawn.zpos = zpos;
                    spawn.faction = "Wild";
                    for(auto &faction : uniFact)
                        if(faction.name == spawn.faction)
                            spawn.factionPtr = &faction;
                    Item nautralWeapon = *getGlobalItem("Baton");
                    for(auto &globalStatus : globalStatusEffects.statusEffects)
                        if(spawn.tags.find(globalStatus.name) != std::string::npos)
                            nautralWeapon.statusEffectsInflict.push_back(globalStatusEffects.getStatusEffect(globalStatus.name));
                    // if(spawn.race == BTInfectionRace), nautralWeapon.statusEffectsInflict.push_back(infection status effect)
                    spawn.inventory.push_back(nautralWeapon);

                    npclist.push_back(spawn);
                }

                if(aspect.name == StatusAspect::SpawnItem)
                {
                    Item spawn;
                    spawn = *getGlobalItem(aspect.type);
                    spawn.xpos = xpos;
                    spawn.ypos = ypos;
                    spawn.zpos = zpos;
                    spawn.amount = aspect.potency;
                    if(spawn.amount > spawn.stackSize)
                        spawn.amount = spawn.stackSize;
                    worlditems.push_back(spawn);
                }

                if(aspect.name == StatusAspect::Revive && !functional() && aspect.potency > 0)
                {
                    aspect.potency--;
                    alive = true;
                    disabled = false;
                    health = getMaxHealth();
                    img = getGlobalCritter(race)->img;
                }

                if(aspect.name == StatusAspect::AutoDodge)
                {
                    //mods.autoDodgeMod.push_back(StringFloat(aspect.type,aspect.potency));
                }

                if(aspect.name == StatusAspect::ChangeRace)
                {
                    if(aspect.potency == 0)
                    {
                        //race = originalRace;
                        //racialAbility = originalRacialAbility;
                        continue;
                    }


                    Npc * changed = getGlobalCritter(aspect.type);
                    if(changed != nullptr)
                    {
                        race = changed->race;
                        racialAbility = changed->racialAbility;
                    }

                    if(aspect.potency > 0)
                        aspect.potency--;
                }
                if(aspect.name == StatusAspect::GrantXP)
                {
                    if(aspect.type == "Raw")
                    {
                        xp += aspect.potency;
                    }
                    if(aspect.type == "Level")
                    {
                        int xpAmount = nextLevelXpRequired(level);
                        int grantedXP = xpAmount * (aspect.potency*0.01);
                        xp += grantedXP;
                    }
                }
                if(aspect.name == StatusAspect::GrantLevel)
                {
                    level += aspect.potency;
                    // Insert xp formula here.
                }

            }
        }


        status.duration--;
        if(status.duration <= 0)
        {
            status.toDelete = true;
            /*
            for(auto &status : statusEffects)
                if(status.auraRadius == 0)
                    for(auto &aspect : status.aspects)
                        if(aspect.name == StatusAspect::ChangeRace)
            { // This is done to simplify conditions of returning to normal form.
                //race = originalRace;
                //racialAbility = originalRacialAbility;
            }

            */
        }


    }

    AnyDeletes(statusEffects);
    mods.clearAllPostConditions();
}

AnimationHolder * aniHold = nullptr;

void drawNPCs(std::list<Npc> &container)
{



    //for(auto &ani : animationmanager.animations)
    //    ani.animation.update(sf::milliseconds(10));

    int lowcapX = (gvars::view1.getCenter().x - (gvars::view1.getSize().x/2));
    int lowcapY = (gvars::view1.getCenter().y - (gvars::view1.getSize().y/2));
    int highcapX = ((gvars::view1.getCenter().x + (gvars::view1.getSize().x/2)));
    int highcapY = ((gvars::view1.getCenter().y + (gvars::view1.getSize().y/2)));

    for (auto &npc : container)
    {
        {
            int CritterZ = npc.zpos/GRID_SIZE;
            //bool withinField = aabb(npc.xpos,npc.ypos,gvars::topLeft.x,gvars::topRight.x,gvars::topLeft.y,gvars::bottomRight.y);


            bool withinField = aabb(npc.xpos,npc.ypos,lowcapX,highcapX,lowcapY,highcapY);




            if(bountytower::towerVictory || withinField && CritterZ == gvars::currentz)
            {
                if(npc.functional())
                    drawEquippedItems(npc);


                if (npc.name == "Azabul")
                {
                    sf::Vector2f ta1 = npc.tentArm1;
                    sf::Vector2f ta2 = npc.tentArm2;
                    sf::Vector2f te1 = npc.tentEnd1;
                    sf::Vector2f te2 = npc.tentEnd2;

                    shapes.createLine(npc.xpos, npc.ypos, ta1.x + npc.xpos,
                                       ta1.y + npc.ypos, 2, sf::Color(0, 200, 200),
                                       1, sf::Color(0, 255, 255));
                    shapes.createLine(npc.xpos, npc.ypos, ta2.x + npc.xpos,
                                       ta2.y + npc.ypos, 2, sf::Color(0, 200, 200),
                                       1, sf::Color(0, 255, 255));
                    shapes.createLine(
                        ta1.x + npc.xpos, ta1.y + npc.ypos,
                        te1.x + ta1.x + npc.xpos, te1.y + ta1.y + npc.ypos, 2,
                        sf::Color(0, 200, 200), 1, sf::Color(0, 255, 255));
                    shapes.createLine(
                        ta2.x + npc.xpos, ta2.y + npc.ypos,
                        te2.x + ta2.x + npc.xpos, te2.y + ta2.y + npc.ypos, 2,
                        sf::Color(0, 200, 200), 1, sf::Color(0, 255, 255));
                    //shapes.drawEffects();
                }

                int aniAngle = npc.angle;
                int angMod = 45-90; // To help with the odd directionals.

                std::string Direction;
                if(inbetween(-1,91,math::constrainAngle(aniAngle+angMod)))
                    Direction = "LeftWalk";
                if(inbetween(89,181,math::constrainAngle(aniAngle+angMod)))
                    Direction = "UpWalk";
                if(inbetween(-181,-89,math::constrainAngle(aniAngle+angMod)))
                    Direction = "RightWalk";
                if(inbetween(-91,1,math::constrainAngle(aniAngle+angMod)))
                    Direction = "DownWalk";

                bool performanceLowDueToAnimationIteration = false;

                if(performanceLowDueToAnimationIteration && npc.isSquaddie)
                { // TODO: This is for when adding in more clothing and races bloats the animiation loop too much.

                    /*
                    if(inputState.key[Key::H])
                    {
                        npc.bodyAnimation.aniDownWalk.setSpriteSheet(texturemanager.getTexture("SpriteSheetBTNoirves.png"));
                        npc.bodyAnimation.spriteDownWalk.setAnimation(npc.bodyAnimation.aniDownWalk);
                    }

                    else if(inputState.key[Key::LShift])
                    {
                        npc.bodyAnimation.aniDownWalk.setSpriteSheet(texturemanager.getTexture("SpriteSheetBTHuman.png"));
                        npc.bodyAnimation.spriteDownWalk.setAnimation(npc.bodyAnimation.aniDownWalk);
                    }

                    AnimatedSprite * spriteWalk = nullptr;

                    if(Direction == "LeftWalk")
                        spriteWalk = &npc.bodyAnimation.spriteLeftWalk;
                    else if(Direction == "DownWalk")
                        spriteWalk = &npc.bodyAnimation.spriteDownWalk;
                    else if(Direction == "RightWalk")
                        spriteWalk = &npc.bodyAnimation.spriteRightWalk;
                    else if(Direction == "UpWalk")
                        spriteWalk = &npc.bodyAnimation.spriteUpWalk;


                    spriteWalk->update(sf::milliseconds(10));
                    spriteWalk->setPosition(npc.getPos2d());
                    window.draw(*spriteWalk, &shadermanager.shockwaveShader);
                    */
                }
                /*

                bool hasAnimations = false;
                if(true == false && npc.alive)
                {
                    for(auto &ani : animationmanager.animations)
                    {
                        bool drawMe = false;
                        bool allowedHat = false;
                        if(npc.isSquaddie)
                            allowedHat = true;
                        else if(npc.tags.find("[WearsHat:1]") != npc.tags.npos)
                            allowedHat = true;

                        if(ani.name.find(Direction) != ani.name.npos)
                        {
                            if(ani.name.find(npc.race) != ani.name.npos)
                            {
                                drawMe = true;
                                hasAnimations = true;
                            }

                            if(ani.name.find("HatBasic") != ani.name.npos)
                                if(npc.isSquaddie || npc.tags.find("[WearsHat:1]") != npc.tags.npos)
                                    drawMe = true;
                            if(ani.name.find("BootsLeather") != ani.name.npos)
                                if(npc.tags.find("[WearsBoots:1]") != npc.tags.npos)
                                    drawMe = true;
                            if(ani.name.find("GlovesLeather") != ani.name.npos)
                                if(npc.tags.find("[WearsGloves:1]") != npc.tags.npos)
                                    drawMe = true;

                        }
                        if(drawMe)
                        {


                            if(npc.race == "Noirves")
                            {
                                ani.animation.setScale(0.75,0.75);

                                if(ani.name.find("LeftWalk") != ani.name.npos)
                                    ani.animation.scale(-1,1);
                            }
                            else if(npc.name == "The Hardened Criminal")
                            {
                                ani.animation.setScale(1.25,1.25);

                                if(ani.name.find("LeftWalk") != ani.name.npos)
                                    ani.animation.scale(-1,1);
                            }
                            else
                            {
                                ani.animation.setScale(1,1);

                                if(ani.name.find("LeftWalk") != ani.name.npos)
                                    ani.animation.scale(-1,1);
                            }


                            ani.animation.setPosition(npc.xpos,npc.ypos);
                            window.draw(ani.animation, &shadermanager.shockwaveShader);
                        }
                    }
                }
                else if(!npc.alive)
                {
                    npc.drawImg();
                }
                if(!hasAnimations)
                    npc.drawImg();
                */



                npc.drawImg();


                /*
                sf::Color shadow(255,50,50,50);


                if(!npc.alive) // To simulate blood.
                    shapes.createCircle(npc.xpos, npc.ypos, npc.size, shadow);
                */
            }
        }
    }
    debug("Done drawing NPCs");
}
