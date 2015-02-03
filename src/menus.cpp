#include "menus.h"

#include "Effects.h"
#include "Text.h"
#include "keys.h"
#include "Galaxy.h"
#include "FactionJobSecurity.h"
#include "Faction.h"
#include "math.h"
#include "util.h"
#include "globalvars.h"

#include <iostream>

std::vector<Button> vButtonList;
std::vector<SquareButton> vSquareButtonList;
MenuPointerContainer menuPtrCon;

using std::abs;

void Button::draw()
{
    if (beenPressed == true)
    {
        //Effectz.CreateCircle(vPos.x,vPos.y,iSize,sf::Color(Color.r/2,Color.g/2,Color.b/2),2,White);
        effects.createSquare(vPos.x - iSize, vPos.y - (iSize / 1.5),
                             vPos.x + iSize, vPos.y + (iSize / 1.5),
                             sf::Color(color.r / 2, color.g / 2, color.b / 2),
                             2, sf::Color::White);
    }
    else if (math::closeish(gvars::mousePos.x, gvars::mousePos.y, vPos.x,
                            vPos.y) < iSize)
    {
        //Effectz.CreateCircle(vPos.x,vPos.y,iSize,Color,2,White);
        effects.createSquare(vPos.x - iSize, vPos.y - (iSize / 1.5),
                             vPos.x + iSize, vPos.y + (iSize / 1.5), color, 2,
                             sf::Color::White);
    }
    else
    {
        //Effectz.CreateCircle(vPos.x,vPos.y,iSize,Color,1,Black);
        effects.createSquare(vPos.x - iSize, vPos.y - (iSize / 1.5),
                             vPos.x + iSize, vPos.y + (iSize / 1.5), color, 2,
                             sf::Color::Black);
    }
    cText.CreateText(vPos.x + 10, vPos.y - (textSize / 2), textSize, textColor,
                     sForwardText);
    effects.drawEffects();
}

Button::Button() : iSize{}, textSize{}
{
    beenPressed = false;
    id = gvars::glbbtn++;
}

void SquareButton::draw()
{
    if (beenPressed == true)
    {
        //Effectz.CreateCircle(vPos.x,vPos.y,iSize,sf::Color(Color.r/2,Color.g/2,Color.b/2),2,White);
        effects.createSquare(vPos.x - iSizex, vPos.y - iSizey, vPos.x + iSizex,
                             vPos.y + iSizey,
                             sf::Color(color.r / 2, color.g / 2, color.b / 2),
                             2, sf::Color::White);
    }
    else if (AABB(gvars::mousePos, vPos.x - iSizex, vPos.x + iSizex,
                  vPos.y - iSizey, vPos.y + iSizey))
    {
        //Effectz.CreateCircle(vPos.x,vPos.y,iSize,Color,2,White);
        effects.createSquare(vPos.x - iSizex, vPos.y - iSizey, vPos.x + iSizex,
                             vPos.y + iSizey, color, 2, sf::Color::White);
        if (gvars::mouseStagnation > 10 && sButtonText.length() != 0)
        {
            effects.createSquare(gvars::mousePos.x + 10, gvars::mousePos.y - 6,
                                 gvars::mousePos.x + (sButtonText.length() * 7),
                                 gvars::mousePos.y + 6, sf::Color::Black, 1,
                                 sf::Color(175, 175, 0));
            cText.CreateText(gvars::mousePos.x + 12, gvars::mousePos.y - 6, 11,
                             textColor, sButtonText);
        }
    }
    else
    {
        //Effectz.CreateCircle(vPos.x,vPos.y,iSize,Color,1,Black);
        effects.createSquare(vPos.x - iSizex, vPos.y - iSizey, vPos.x + iSizex,
                             vPos.y + iSizey, color, 2, sf::Color::Black);
    }
    cText.CreateText(vPos.x + 10, vPos.y - (textSize / 2), textSize, textColor,
                     sForwardText);
    effects.drawEffects();
}

SquareButton::SquareButton() : iSizex{}, iSizey{}, textSize{}
{
    beenPressed = false;
    textColor = sf::Color(175, 175, 0);
    id = gvars::glbbtn++;
}

void Buttons::pressed()
{
    for (auto const &button : vButtonList)
    {
        if (math::closeish(gvars::mousePos.x, gvars::mousePos.y, button.vPos.x,
                           button.vPos.y) < button.iSize)
        {
            std::cout << "Button Click!\n";
        }
    }
}

void SquareButtons::pressed()
{
    for (auto const &button : vSquareButtonList)
    {
        if (AABB(gvars::mousePos, button.vPos.x - button.iSizex,
                 button.vPos.x + button.iSizex, button.vPos.y - button.iSizey,
                 button.vPos.y + button.iSizey))
        {
            std::cout << "Button Click!\n";
            // Is this never called? Is this class useless?
        }
    }
}

int createButton(sf::Vector2f vPos, int iSize, sf::Color Color,
                 std::string Text)
{
    Button var;
    var.vPos = vPos;
    var.iSize = iSize;
    var.color = Color;
    var.sButtonText = Text;
    vButtonList.push_back(var);
    return var.id;
}

int createSquareButton(sf::Vector2f vPos, int iSizex, int iSizey,
                       sf::Color Color, std::string Text)
{
    SquareButton var;
    var.vPos = vPos;
    var.iSizex = iSizex;
    var.iSizey = iSizey;
    var.color = Color;
    var.sButtonText = Text;
    vSquareButtonList.push_back(var);
    return var.id;
}

bool buttonClicked(int id)
{
    for (auto &button : vButtonList)
    {
        if (button.id == id)
        {
            if (math::closeish(gvars::mousePos.x, gvars::mousePos.y,
                               button.vPos.x, button.vPos.y) < button.iSize &&
                (key.lmbTime == 1 || key.lmbTime > 20))
            {
                button.beenPressed = true;
                std::cout << "Pressed! \n";
                gvars::buttonClicked = true;
                return true;
            }
        }
    }
    return false;
}

bool squareButtonClicked(int id)
{
    for (auto &button : vSquareButtonList)
    {
        if (button.id == id)
        {
            if (AABB(gvars::mousePos, button.vPos.x - button.iSizex,
                     button.vPos.x + button.iSizex,
                     button.vPos.y - button.iSizey,
                     button.vPos.y + button.iSizey) &&
                (key.lmbTime == 1 || key.lmbTime > 20))
            {
                button.beenPressed = true;
                std::cout << "Pressed! \n";
                gvars::buttonClicked = true;
                gvars::buttonClickedTime = 3;
                return true;
            }
        }
    }
    return false;
}

MenuPointerContainer::MenuPointerContainer()
{
    pItem = nullptr;
    pNPC = nullptr;
    pTile = nullptr;
}

// TODO: Add functionality to allow you to press 1-9
// to activate the menu buttons.
void menuPopUp()
{

    if (gCtrl.menuPos.x == -10000)
    {
        gCtrl.menuPos = gvars::mousePos;
    }

    sf::Vector2f Tiled(abs(gCtrl.menuPos.x / 20) * 20,
                       abs(gCtrl.menuPos.y / 20) * 20);

    effects.createSquare(Tiled.x, Tiled.y, Tiled.x + 20, Tiled.y + 20,
                         sf::Color::Transparent, 1, sf::Color::Cyan);

    if (gCtrl.menuType == "Orbital Drop")
    {

        int Options = 8;
        gCtrl.menuEndPos = sf::Vector2f(gCtrl.menuPos.x + 150,
                                        (gCtrl.menuPos.y + (Options * 13)) + 5);
        effects.createSquare(gCtrl.menuPos.x, gCtrl.menuPos.y,
                             gCtrl.menuEndPos.x, gCtrl.menuEndPos.y,
                             sf::Color::Black, 2, sf::Color::Cyan);
        int iY = 0;
        int BRD = 140;                      // ButtonRightDisplacement.
        int BS = 7;                         // ButtonSize;
        int BSY = 5;                        // ButtonSize;
        int MBD = 8;                        // MoveButtonDown
        sf::Color ButCol = sf::Color::Cyan; // ButtonColor.

        for (int i = 0; i != Options; i++)
        {

            if (i == 0)
            {
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x + 2,
                                 gCtrl.menuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Orbital Drop - Missle");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Strikes the location!");
                if (squareButtonClicked(Butt) || key.num1Time == 1)
                {
                    effects.createSquare(gCtrl.menuPos.x - 10,
                                         gCtrl.menuPos.y - 2,
                                         gCtrl.menuPos.x + 10,
                                         gCtrl.menuPos.y + 2, sf::Color::Black);
                    effects.createSquare(
                        gCtrl.menuPos.x - 2, gCtrl.menuPos.y - 10,
                        gCtrl.menuPos.x + 2, gCtrl.menuPos.y + 10,
                        sf::Color::Black);
                    Item Var;
                    Var = *getGlobalItem("Missile");
                    Var.targetPos = sf::Vector2f(gCtrl.menuPos);
                    Var.xpos = gCtrl.menuPos.x - 200;
                    Var.ypos = gCtrl.menuPos.y - 200;
                    Var.zpos = 100;
                    worlditems.push_back(Var);

                    gCtrl.menuPos = sf::Vector2f(-10000, -10000);
                    gCtrl.menuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }
        }
    }

    if (gCtrl.menuType == "BuildStructure")
    {

        int Options = 8;
        gCtrl.menuEndPos = sf::Vector2f(gCtrl.menuPos.x + 150,
                                        (gCtrl.menuPos.y + (Options * 13)) + 5);
        effects.createSquare(gCtrl.menuPos.x, gCtrl.menuPos.y,
                             gCtrl.menuEndPos.x, gCtrl.menuEndPos.y,
                             sf::Color::Black, 2, sf::Color::Cyan);
        int iY = 0;
        int BRD = 140;                      // ButtonRightDisplacement.
        int BS = 7;                         // ButtonSize;
        int BSY = 5;                        // ButtonSize;
        int MBD = 8;                        // MoveButtonDown
        sf::Color ButCol = sf::Color::Cyan; // ButtonColor.

        for (int i = 0; i != Options; i++)
        {

            if (i == 0)
            {
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x + 2,
                                 gCtrl.menuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Build - Wall");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Makes a Wall!");
                if (squareButtonClicked(Butt) || key.num1Time == 1)
                {
                    rmbMenuTile(gCtrl.menuPos);
                }
            }

            if (i == 1)
            {
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x + 2,
                                 gCtrl.menuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Dig - Natural Wall");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Digs out a natural wall.");
                if (squareButtonClicked(Butt) || key.num1Time == 1)
                {
                    digWall(gCtrl.menuPos);
                }
            }

            iY++;
        }
    }

    if (gCtrl.menuType == "BlankRMB")
    {

        int Options = 8;
        gCtrl.menuEndPos = sf::Vector2f(gCtrl.menuPos.x + 150,
                                        (gCtrl.menuPos.y + (Options * 13)) + 5);
        effects.createSquare(gCtrl.menuPos.x, gCtrl.menuPos.y,
                             gCtrl.menuEndPos.x, gCtrl.menuEndPos.y,
                             sf::Color::Black, 2, sf::Color::Cyan);
        int iY = 0;
        int BRD = 140;                      // ButtonRightDisplacement.
        int BS = 7;                         // ButtonSize;
        int BSY = 5;                        // ButtonSize;
        int MBD = 8;                        // MoveButtonDown
        sf::Color ButCol = sf::Color::Cyan; // ButtonColor.

        for (int i = 0; i != Options; i++)
        {

            if (i == 0)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x + 2,
                                 gCtrl.menuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Build");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol,
                    "This opens the menu to build various structures!");
                if (squareButtonClicked(Butt) || key.num1Time == 1)
                {
                    gCtrl.menuType = "BuildStructure";
                    //fSleep(0.2);
                    return;
                }
            }
            if (i == 1)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x + 2,
                                 gCtrl.menuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Orbital Drop");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Causes an orbital ship to release "
                                     "specific cargo at target location.");
                if (squareButtonClicked(Butt) || key.num2Time == 1)
                {

                    gCtrl.menuType = "Orbital Drop";
                    //fSleep(0.2);
                    return;
                }
            }

            if (i == -5)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x + 2,
                                 gCtrl.menuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Dig");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Dig out the current wall tile");
                if (squareButtonClicked(Butt) || key.num2Time == 1)
                {

                    gCtrl.menuType = "Dig";
                    //fSleep(0.2);
                    return;
                }
            }

            iY++;
        }
    }

    if (gCtrl.menuType == "CritterRMB")
    {
        gCtrl.menuPos = sf::Vector2f(-10000, -10000);
        gCtrl.menuEndPos = sf::Vector2f(-10000, -10000);
        gCtrl.menuType = "NULL";
    }

    if (gCtrl.menuType == "ItemContext")
    {

        int Options = 8;
        gCtrl.menuEndPos = sf::Vector2f(gCtrl.menuPos.x + 150,
                                        (gCtrl.menuPos.y + (Options * 13)) + 5);
        effects.createSquare(gCtrl.menuPos.x, gCtrl.menuPos.y,
                             gCtrl.menuEndPos.x, gCtrl.menuEndPos.y,
                             sf::Color::Black, 2, sf::Color::Cyan);
        int iY = 0;
        int BRD = 140;                      // ButtonRightDisplacement.
        int BS = 7;                         // ButtonSize;
        int BSY = 5;                        // ButtonSize;
        int MBD = 8;                        // MoveButtonDown
        sf::Color ButCol = sf::Color::Cyan; // ButtonColor.

        for (int i = 0; i != Options; i++)
        {

            if (i == 0)
            {
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                if (gCtrl.menuPtrCon.pItem->pickupable)
                    cText.CreateText(
                        gCtrl.menuPos.x + 2, gCtrl.menuPos.y + (iY * 13), 12,
                        sf::Color::White,
                        gCtrl.menuPtrCon.pItem->name + " - PickUp");
                else
                    cText.CreateText(gCtrl.menuPos.x + 2,
                                     gCtrl.menuPos.y + (iY * 13), 12,
                                     sf::Color::White,
                                     gCtrl.menuPtrCon.pItem->name +
                                         " - xPickUpx, Cannot be picked up.");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Makes someone pickup this item.");
                if ((squareButtonClicked(Butt) &&
                     gCtrl.menuPtrCon.pItem->pickupable) ||
                    (key.num1Time == 1 && gCtrl.menuPtrCon.pItem->pickupable))
                {

                    for (size_t i = 0; i != uniFact.size(); i++)
                    {
                        if (uniFact[i].playerControlled)
                        {
                            Job job;
                            job.pItem = gCtrl.menuPtrCon.pItem;

                            job.name = "PickUpItem";
                            job.type = "PickUp";

                            uniFact[i].jobList.push_back(job);
                        }
                    }
                }
            }

            if (i == 1)
            {
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                if (!gCtrl.menuPtrCon.pItem->pickupable)
                    cText.CreateText(
                        gCtrl.menuPos.x + 2, gCtrl.menuPos.y + (iY * 13), 12,
                        sf::Color::White,
                        gCtrl.menuPtrCon.pItem->name + " - ChopDown");
                else
                    cText.CreateText(
                        gCtrl.menuPos.x + 2, gCtrl.menuPos.y + (iY * 13), 12,
                        sf::Color::White,
                        gCtrl.menuPtrCon.pItem->name +
                            " - xChopDownx, Cannot be chopped up.");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol,
                    "Makes someone seek and destroy this item.");
                if ((squareButtonClicked(Butt) &&
                     !gCtrl.menuPtrCon.pItem->pickupable) ||
                    (key.num1Time == 1 && !gCtrl.menuPtrCon.pItem->pickupable))
                {

                    for (size_t i = 0; i != uniFact.size(); i++)
                    {
                        if (uniFact[i].playerControlled)
                        {
                            Job job;
                            job.pItem = gCtrl.menuPtrCon.pItem;

                            job.name = "ChopDownTree";
                            job.type = "Chop";

                            uniFact[i].jobList.push_back(job);
                        }
                    }
                }
            }

            iY++;
        }
    }

    if (gCtrl.menuType == "ItemRMB")
    {
        int Options = 8;
        gCtrl.menuEndPos = sf::Vector2f(gCtrl.menuPos.x + 150,
                                        (gCtrl.menuPos.y + (Options * 13)) + 5);
        effects.createSquare(gCtrl.menuPos.x, gCtrl.menuPos.y,
                             gCtrl.menuEndPos.x, gCtrl.menuEndPos.y,
                             sf::Color::Black, 2, sf::Color::Cyan);
        int iY = 0;
        int BRD = 140;                      // ButtonRightDisplacement.
        int BS = 7;                         // ButtonSize;
        int BSY = 5;                        // ButtonSize;
        int MBD = 8;                        // MoveButtonDown
        sf::Color ButCol = sf::Color::Cyan; // ButtonColor.

        for (size_t i = 0; i != gCtrl.menuPtrCon.pVecItem.size(); i++)
        {

            effects.createLine(
                gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                sf::Color::Cyan);
            cText.CreateText(gCtrl.menuPos.x + 2, gCtrl.menuPos.y + (iY * 13),
                             12, sf::Color::White,
                             gCtrl.menuPtrCon.pVecItem[i]->name);
            int Butt = createSquareButton(
                math::Vec2f(gCtrl.menuPos.x + BRD,
                            (gCtrl.menuPos.y + (iY * 13)) + MBD),
                BS, BSY, ButCol, "Strikes the location!");
            if (squareButtonClicked(Butt))
            {

                gCtrl.menuType = "ItemContext";
                gCtrl.menuPtrCon.pItem = gCtrl.menuPtrCon.pVecItem[i];
                gCtrl.menuPtrCon.pVecItem.clear();
                return;
            }

            if (/*i == -1*/ false) // `i` can't be -1
            {
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(
                    gCtrl.menuPos.x + 2, gCtrl.menuPos.y + (iY * 13), 12,
                    sf::Color::White, gCtrl.menuPtrCon.pVecItem[0]->name);
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Strikes the location!");
                if (squareButtonClicked(Butt) || key.num1Time == 1)
                {

                    gCtrl.menuPos = sf::Vector2f(-10000, -10000);
                    gCtrl.menuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }

            iY++;
        }
    }

    if (gCtrl.menuType == "WorldRMB")
    {
        int Options = 1;
        gCtrl.menuEndPos = sf::Vector2f(gCtrl.menuPos.x + 150,
                                        (gCtrl.menuPos.y + (Options * 13)) + 5);
        effects.createSquare(gCtrl.menuPos.x, gCtrl.menuPos.y,
                             gCtrl.menuEndPos.x, gCtrl.menuEndPos.y,
                             sf::Color::Black, 2, sf::Color::Cyan);
        int iY = 0;
        int BRD = 140;                      // ButtonRightDisplacement.
        int BS = 7;                         // ButtonSize;
        int BSY = 5;                        // ButtonSize;
        int MBD = 8;                        // MoveButtonDown
        sf::Color ButCol = sf::Color::Cyan; // ButtonColor.

        for (int i = 0; i != Options; i++)
        {

            if (i == 0)
            {

                if (WorldMap[abs_to_index(gCtrl.menuPos.x / 20)]
                            [abs_to_index(gCtrl.menuPos.y / 20)].ID == 0)
                    effects.createLine(
                        gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                        gCtrl.menuPos.x + 90,
                        (gCtrl.menuPos.y + (iY * 13)) + 13, 1, sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x, gCtrl.menuPos.y + (iY * 13),
                                 12, sf::Color::White, "Enter city");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Welcome to the jungle baby.");
                if (squareButtonClicked(Butt) || key.num1Time == 1)
                {
                    GenerateChunk("Building", 500,
                                  sf::Vector2i(abs(gCtrl.menuPos.x / 20),
                                               abs(gCtrl.menuPos.y / 20)));
                    gvars::currentregionx = abs(gCtrl.menuPos.x / 20);
                    gvars::currentregiony = abs(gCtrl.menuPos.y / 20);
                    gCtrl.phase = "Local";

                    //GC.BuildLocal("City", WorldMap[abs_to_index(GC.MenuPos.x/20)][abs_to_index(GC.MenuPos.y/20)].Infected);
                    BuildLocalfromWorld(sf::Vector2i(
                        abs(gCtrl.menuPos.x / 20), abs(gCtrl.menuPos.y / 20)));
                    buildStartingCritters(WorldMap[abs_to_index(
                        gCtrl.menuPos.x / 20)][abs_to_index(gCtrl.menuPos.y /
                                                            20)].Infected);
                    spawnItem("Sword", 990, 1020);

                    spawnItem("Gun", 1010, 1020);

                    Con("Hunting for Trees 4");
                    for (int i = 0; i != 8; i++)
                    {
                        int Tx = randz(50, 1950);
                        int Ty = randz(50, 1950);
                        Con(Tx, false);
                        Con(":", false);
                        Con(Ty, false);
                        if (Tiles[abs_to_index(Tx / 20)][abs_to_index(
                                Ty / 20)][30].ID == 1001 ||
                            Tiles[abs_to_index(Tx / 20)][abs_to_index(
                                Ty / 20)][30].ID == 1003)
                        {
                            Con(", Is Plantable.");
                            spawnItem("Fruit Tree", Tx, Ty);
                        }
                    }

                    spawnItem("Broken Vending Machine", 1030, 1030);

                    spawnCritter("Mini Turret", 1000, 1070);

                    gCtrl.menuPos = sf::Vector2f(-10000, -10000);
                    gCtrl.menuType = "NULL";

                    fSleep(0.2);
                    break;
                }
                iY++;
            }
        }
    }

    if (gCtrl.menuType == "DebugFunctions")
    {

        int Options = 10;
        gCtrl.menuEndPos = sf::Vector2f(gCtrl.menuPos.x + 150,
                                        (gCtrl.menuPos.y + (Options * 13)) + 5);
        effects.createSquare(gCtrl.menuPos.x, gCtrl.menuPos.y,
                             gCtrl.menuEndPos.x, gCtrl.menuEndPos.y,
                             sf::Color::Black, 2, sf::Color::Cyan);
        int iY = 0;
        int BRD = 140;                      // ButtonRightDisplacement.
        int BS = 7;                         // ButtonSize;
        int BSY = 5;                        // ButtonSize;
        int MBD = 8;                        // MoveButtonDown
        sf::Color ButCol = sf::Color::Cyan; // ButtonColor.

        for (int i = 0; i != Options; i++)
        {

            if (i == 0)
            {
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x + 2,
                                 gCtrl.menuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Close Menu");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Close Menu");
                if (squareButtonClicked(Butt) || key.num1Time == 1)
                {
                    gCtrl.menuPos = sf::Vector2f(-10000, -10000);
                    gCtrl.menuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }
            if (i == 1)
            {
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x + 2,
                                 gCtrl.menuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Missile Strike");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Missile Strike");
                if (squareButtonClicked(Butt) || key.num2Time == 1)
                {

                    effects.createSquare(gCtrl.menuPos.x - 10,
                                         gCtrl.menuPos.y - 2,
                                         gCtrl.menuPos.x + 10,
                                         gCtrl.menuPos.y + 2, sf::Color::Black);
                    effects.createSquare(
                        gCtrl.menuPos.x - 2, gCtrl.menuPos.y - 10,
                        gCtrl.menuPos.x + 2, gCtrl.menuPos.y + 10,
                        sf::Color::Black);
                    Item Var;
                    Var = *getGlobalItem("Missile");
                    Var.targetPos = sf::Vector2f(gCtrl.menuPos);
                    Var.xpos = gCtrl.menuPos.x - 200;
                    Var.ypos = gCtrl.menuPos.y - 200;
                    Var.zpos = 100;
                    worlditems.push_back(Var);

                    gCtrl.menuPos = sf::Vector2f(-10000, -10000);
                    gCtrl.menuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }

            if (i == 2)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x + 2,
                                 gCtrl.menuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Shift Wall/Stone");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (squareButtonClicked(Butt) || key.num3Time == 1)
                {

                    if (Tiles[abs_to_index(gCtrl.menuPos.x / GridSize)]
                             [abs_to_index(gCtrl.menuPos.y / GridSize)][30]
                                 .ID != 1010)
                    {
                        Tiles[abs_to_index(gCtrl.menuPos.x / GridSize)]
                             [abs_to_index(gCtrl.menuPos.y / GridSize)][30]
                                 .Wall();
                        fSleep(0.2);
                    }
                    else
                    {
                        Tiles[abs_to_index(gCtrl.menuPos.x / GridSize)]
                             [abs_to_index(gCtrl.menuPos.y / GridSize)][30]
                                 .Stone();
                        fSleep(0.2);
                    }

                    Con("TODO: Make this a Toggle Mode");

                    gCtrl.menuPos = sf::Vector2f(-10000, -10000);
                    gCtrl.menuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }

            if (i == 3)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x + 2,
                                 gCtrl.menuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Spawn Zombie Horde");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (squareButtonClicked(Butt) || key.num4Time == 1)
                {

                    gCtrl.zombieSwarmLocal();

                    gCtrl.menuPos = sf::Vector2f(-10000, -10000);
                    gCtrl.menuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }

            if (i == 4)
            {
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x, gCtrl.menuPos.y + (iY * 13),
                                 12, sf::Color::White,
                                 "Print NPC's ConsoleInfo");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (squareButtonClicked(Butt) || key.num5Time == 1)
                {

                    std::vector<Npc>::iterator zit;
                    for (zit = npclist.begin(); zit != npclist.end(); ++zit)
                    {
                        if (zit->alive == true)
                        {
                            zit->printConsoleInfo();
                        }
                    }

                    gCtrl.menuPos = sf::Vector2f(-10000, -10000);
                    gCtrl.menuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }

            if (i == 5)
            {
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x, gCtrl.menuPos.y + (iY * 13),
                                 12, sf::Color::White,
                                 "Print NPC's Bloodcontents");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (squareButtonClicked(Butt) || key.num5Time == 1)
                {

                    for (auto &elem : npclist)
                    {
                        elem.printBloodContent();
                    }

                    gCtrl.menuPos = sf::Vector2f(-10000, -10000);
                    gCtrl.menuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }

            if (i == 6)
            {
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x, gCtrl.menuPos.y + (iY * 13),
                                 12, sf::Color::White,
                                 "Give Everyone Zombification");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol,
                    "You horrible monster, Why would you do this?!");
                if (squareButtonClicked(Butt) || key.num6Time == 1)
                {

                    for (auto &elem : npclist)
                    {
                        elem.bloodwork("Zombification", 100);
                    }

                    gCtrl.menuPos = sf::Vector2f(-10000, -10000);
                    gCtrl.menuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }

            if (i == 7)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x, gCtrl.menuPos.y + (iY * 13),
                                 12, sf::Color::White, "Delete all critters");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (squareButtonClicked(Butt) || key.num7Time == 1)
                {
                    for (auto &elem : npclist)
                    {
                        elem.toDelete = true;
                    }
                }
            }

            if (i == 8)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x, gCtrl.menuPos.y + (iY * 13),
                                 12, sf::Color::White, "Starve all critters");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (squareButtonClicked(Butt) || key.num8Time == 1)
                {
                    for (auto &elem : npclist)
                    {
                        elem.hunger = 50;
                    }
                }
            }
            if (i == 9)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 13)) + 13,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(gCtrl.menuPos.x, gCtrl.menuPos.y + (iY * 13),
                                 12, sf::Color::White, "Giant Enclosed Arena");
                int Butt = createSquareButton(
                    math::Vec2f(gCtrl.menuPos.x + BRD,
                                (gCtrl.menuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (squareButtonClicked(Butt) || key.num8Time == 1)
                {
                    for (int ItLength = 0; ItLength != 16; ItLength++)
                    {
                        if (ItLength != 15)
                        {
                            for (int Rot = 1; Rot != 361; Rot++)
                            {
                                int XPos = abs(gCtrl.menuPos.x / 20) +
                                           sin(Rot * PI / 180) * ItLength;
                                int YPos = abs(gCtrl.menuPos.y / 20) +
                                           cos(Rot * PI / 180) * ItLength;
                                Tiles[XPos][YPos][30].Stone();
                            }
                        }
                        else
                        {
                            for (int Rot = 1; Rot != 361; Rot++)
                            {
                                int XPos = abs(gCtrl.menuPos.x / 20) +
                                           sin(Rot * PI / 180) * ItLength;
                                int YPos = abs(gCtrl.menuPos.y / 20) +
                                           cos(Rot * PI / 180) * ItLength;
                                Tiles[XPos][YPos][30].Wall();
                            }
                        }
                    }
                }
            }

            iY++;
        }
    }

    if (gCtrl.menuType == "DebugItemSpawn")
    {
        gCtrl.menuEndPos = sf::Vector2f(
            gCtrl.menuPos.x + 100,
            (gCtrl.menuPos.y + (itemmanager.globalItems.size() * 10)) + 5);
        effects.createSquare(gCtrl.menuPos.x, gCtrl.menuPos.y,
                             gCtrl.menuEndPos.x, gCtrl.menuEndPos.y,
                             sf::Color::White);
        int iY = 0;
        for (auto &elem : itemmanager.globalItems)
        {
            effects.createLine(
                gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 11)) + 8,
                gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 11)) + 8, 3,
                sf::Color::Black, 1, sf::Color::Yellow);
            cText.CreateText(gCtrl.menuPos.x, gCtrl.menuPos.y + (iY * 11), 11,
                             sf::Color::Cyan, elem.name);
            int Butt =
                createButton(math::Vec2f(gCtrl.menuPos.x + 90,
                                         (gCtrl.menuPos.y + (iY * 11)) + 5),
                             5, sf::Color::Red);
            if (buttonClicked(Butt))
            {
                spawnItem(elem.name, gCtrl.menuPos.x, gCtrl.menuPos.y);
                fSleep(0.2);
            }
            iY++;
        }
    }

    if (gCtrl.menuType == "DebugCritterSpawn")
    {
        gCtrl.menuEndPos = sf::Vector2f(
            gCtrl.menuPos.x + 100,
            (gCtrl.menuPos.y + (npcmanager.globalCritter.size() * 10)) + 5);
        effects.createSquare(gCtrl.menuPos.x, gCtrl.menuPos.y,
                             gCtrl.menuEndPos.x, gCtrl.menuEndPos.y,
                             sf::Color::White);
        int iY = 0;
        for (auto &elem : npcmanager.globalCritter)
        {
            effects.createLine(
                gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 11)) + 8,
                gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 11)) + 8, 3,
                sf::Color::Black, 1, sf::Color::Yellow);
            cText.CreateText(gCtrl.menuPos.x, gCtrl.menuPos.y + (iY * 11), 11,
                             sf::Color::Cyan, elem.name);
            int Butt =
                createButton(math::Vec2f(gCtrl.menuPos.x + 90,
                                         (gCtrl.menuPos.y + (iY * 11)) + 5),
                             5, sf::Color::Red);
            if (buttonClicked(Butt))
            {
                spawnCritter(elem.name, gCtrl.menuPos.x, gCtrl.menuPos.y);
                fSleep(0.2);
                break;
            }
            iY++;
        }
    }

    if (gCtrl.menuType == "CritterContext" && true == false)
    {
        int Options = 2;
        gCtrl.menuEndPos = sf::Vector2f(gCtrl.menuPos.x + 100,
                                        (gCtrl.menuPos.y + (Options * 10)) + 5);
        effects.createSquare(gCtrl.menuPos.x, gCtrl.menuPos.y,
                             gCtrl.menuEndPos.x, gCtrl.menuEndPos.y,
                             sf::Color::White);
        int iY = 0;
        for (int i = 0; i != Options; i++)
        {

            if (i == 0)
            {

                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 11)) + 8,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 11)) + 8, 3,
                    sf::Color::Black, 1, sf::Color::Yellow);
                cText.CreateText(gCtrl.menuPos.x, gCtrl.menuPos.y + (iY * 11),
                                 11, sf::Color::Cyan, "Close Menu");
                int Butt =
                    createButton(math::Vec2f(gCtrl.menuPos.x + 90,
                                             (gCtrl.menuPos.y + (iY * 11)) + 5),
                                 5, sf::Color::Red);
                if (buttonClicked(Butt))
                {

                    std::list<Item>::iterator Item;
                    for (Item = worlditems.begin(); Item != worlditems.end();
                         Item++)
                    {
                        gCtrl.menuPos = sf::Vector2f(-10000, -10000);
                        gCtrl.menuType = "NULL";
                    }

                    fSleep(0.2);
                    break;
                }
            }
            if (i == 1)
            {

                effects.createLine(
                    gCtrl.menuPos.x, (gCtrl.menuPos.y + (iY * 11)) + 8,
                    gCtrl.menuPos.x + 90, (gCtrl.menuPos.y + (iY * 11)) + 8, 3,
                    sf::Color::Black, 1, sf::Color::Yellow);
                cText.CreateText(gCtrl.menuPos.x, gCtrl.menuPos.y + (iY * 11),
                                 11, sf::Color::Cyan, "PickUp");
                int Butt =
                    createButton(math::Vec2f(gCtrl.menuPos.x + 90,
                                             (gCtrl.menuPos.y + (iY * 11)) + 5),
                                 5, sf::Color::Red);
                if (buttonClicked(Butt))
                {

                    std::list<Item>::iterator Item;
                    for (Item = worlditems.begin(); Item != worlditems.end();
                         Item++)
                    {
                        if (math::closeish(gCtrl.menuPos.x, gCtrl.menuPos.y,
                                           Item->xpos, Item->ypos) <= 10)
                        {
                            if (Item->pickupable == true)
                            {
                                npclist.at(getNpcVectorId(gvars::myTargetid))
                                    .action = "Pickup";
                                npclist.at(getNpcVectorId(gvars::myTargetid))
                                    .target = Item->name;
                                npclist.at(getNpcVectorId(gvars::myTargetid))
                                    .targetId = Item->id;
                                npclist.at(getNpcVectorId(gvars::myTargetid))
                                    .targetPos =
                                    sf::Vector2f(Item->xpos, Item->ypos);
                                gCtrl.menuPos = sf::Vector2f(-10000, -10000);
                                gCtrl.menuType = "NULL";
                            }
                        }
                    }

                    fSleep(0.2);
                    break;
                }
            }

            iY++;
        }

        if (gvars::myTarget == -1 && gCtrl.menuType == "CritterContext")
        {
            gCtrl.menuPos = sf::Vector2f(-10000, -10000);
            gCtrl.menuType = "NULL";
            Con("Closing Menu due to No Target && CritterContext");
        }
    }
}

void rightMouseButtonContextMenu()
{
    if (key.rmbTime ==
        1) // This is the RightMouseButton's context menu for Tiles/Critters/Items, This will probably be rather robust
    {

        for (size_t i = 0; i != npclist.size(); i++)
        {
            if (math::closeish(gvars::mousePos.x, gvars::mousePos.y,
                               npclist[i].xpos,
                               npclist[i].ypos) <= npclist[i].size)
            {
                gCtrl.menuType = "CritterRMB";
                menuPopUp();
                return;
            }
        }

        for (auto &worlditem : worlditems)
        {
            if (math::closeish(gvars::mousePos.x, gvars::mousePos.y,
                               (worlditem).xpos, (worlditem).ypos) <= 10)
            {
                gCtrl.menuType = "ItemRMB";
                gCtrl.menuPtrCon.pVecItem.push_back(&(worlditem));
            }
        }
        if (gCtrl.menuPtrCon.pVecItem.size() != 0)
        {
            menuPopUp();
            return;
        }

        gCtrl.menuType = "BlankRMB";
        menuPopUp();
    }
}
