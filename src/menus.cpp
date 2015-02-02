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
    else if (math::closeish(gvars::MousePos.x, gvars::MousePos.y, vPos.x,
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
    else if (AABB(gvars::MousePos, vPos.x - iSizex, vPos.x + iSizex,
                  vPos.y - iSizey, vPos.y + iSizey))
    {
        //Effectz.CreateCircle(vPos.x,vPos.y,iSize,Color,2,White);
        effects.createSquare(vPos.x - iSizex, vPos.y - iSizey, vPos.x + iSizex,
                             vPos.y + iSizey, color, 2, sf::Color::White);
        if (gvars::MouseStagnation > 10 && sButtonText.length() != 0)
        {
            effects.createSquare(gvars::MousePos.x + 10, gvars::MousePos.y - 6,
                                 gvars::MousePos.x + (sButtonText.length() * 7),
                                 gvars::MousePos.y + 6, sf::Color::Black, 1,
                                 sf::Color(175, 175, 0));
            cText.CreateText(gvars::MousePos.x + 12, gvars::MousePos.y - 6, 11,
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
        if (math::closeish(gvars::MousePos.x, gvars::MousePos.y, button.vPos.x,
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
        if (AABB(gvars::MousePos, button.vPos.x - button.iSizex,
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
            if (math::closeish(gvars::MousePos.x, gvars::MousePos.y,
                               button.vPos.x, button.vPos.y) < button.iSize &&
                (Key.LMBTime == 1 || Key.LMBTime > 20))
            {
                button.beenPressed = true;
                std::cout << "Pressed! \n";
                gvars::ButtonClicked = true;
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
            if (AABB(gvars::MousePos, button.vPos.x - button.iSizex,
                     button.vPos.x + button.iSizex,
                     button.vPos.y - button.iSizey,
                     button.vPos.y + button.iSizey) &&
                (Key.LMBTime == 1 || Key.LMBTime > 20))
            {
                button.beenPressed = true;
                std::cout << "Pressed! \n";
                gvars::ButtonClicked = true;
                gvars::ButtonClickedTime = 3;
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

    if (GC.MenuPos.x == -10000)
    {
        GC.MenuPos = gvars::MousePos;
    }

    sf::Vector2f Tiled(abs(GC.MenuPos.x / 20) * 20,
                       abs(GC.MenuPos.y / 20) * 20);

    effects.createSquare(Tiled.x, Tiled.y, Tiled.x + 20, Tiled.y + 20,
                         sf::Color::Transparent, 1, sf::Color::Cyan);

    if (GC.MenuType == "Orbital Drop")
    {

        int Options = 8;
        GC.MenuEndPos = sf::Vector2f(GC.MenuPos.x + 150,
                                     (GC.MenuPos.y + (Options * 13)) + 5);
        effects.createSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, sf::Color::Black, 2,
                             sf::Color::Cyan);
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
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Orbital Drop - Missle");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Strikes the location!");
                if (squareButtonClicked(Butt) || Key.num1Time == 1)
                {
                    effects.createSquare(GC.MenuPos.x - 10, GC.MenuPos.y - 2,
                                         GC.MenuPos.x + 10, GC.MenuPos.y + 2,
                                         sf::Color::Black);
                    effects.createSquare(GC.MenuPos.x - 2, GC.MenuPos.y - 10,
                                         GC.MenuPos.x + 2, GC.MenuPos.y + 10,
                                         sf::Color::Black);
                    Item Var;
                    Var = *getGlobalItem("Missile");
                    Var.targetPos = sf::Vector2f(GC.MenuPos);
                    Var.xpos = GC.MenuPos.x - 200;
                    Var.ypos = GC.MenuPos.y - 200;
                    Var.zpos = 100;
                    worlditems.push_back(Var);

                    GC.MenuPos = sf::Vector2f(-10000, -10000);
                    GC.MenuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }
        }
    }

    if (GC.MenuType == "BuildStructure")
    {

        int Options = 8;
        GC.MenuEndPos = sf::Vector2f(GC.MenuPos.x + 150,
                                     (GC.MenuPos.y + (Options * 13)) + 5);
        effects.createSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, sf::Color::Black, 2,
                             sf::Color::Cyan);
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
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Build - Wall");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Makes a Wall!");
                if (squareButtonClicked(Butt) || Key.num1Time == 1)
                {
                    RMBMenuTile(GC.MenuPos);
                }
            }

            if (i == 1)
            {
                effects.createLine(
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Dig - Natural Wall");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Digs out a natural wall.");
                if (squareButtonClicked(Butt) || Key.num1Time == 1)
                {
                    DigWall(GC.MenuPos);
                }
            }

            iY++;
        }
    }

    if (GC.MenuType == "BlankRMB")
    {

        int Options = 8;
        GC.MenuEndPos = sf::Vector2f(GC.MenuPos.x + 150,
                                     (GC.MenuPos.y + (Options * 13)) + 5);
        effects.createSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, sf::Color::Black, 2,
                             sf::Color::Cyan);
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
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Build");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol,
                    "This opens the menu to build various structures!");
                if (squareButtonClicked(Butt) || Key.num1Time == 1)
                {
                    GC.MenuType = "BuildStructure";
                    //fSleep(0.2);
                    return;
                }
            }
            if (i == 1)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Orbital Drop");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Causes an orbital ship to release "
                                     "specific cargo at target location.");
                if (squareButtonClicked(Butt) || Key.num2Time == 1)
                {

                    GC.MenuType = "Orbital Drop";
                    //fSleep(0.2);
                    return;
                }
            }

            if (i == -5)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Dig");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Dig out the current wall tile");
                if (squareButtonClicked(Butt) || Key.num2Time == 1)
                {

                    GC.MenuType = "Dig";
                    //fSleep(0.2);
                    return;
                }
            }

            iY++;
        }
    }

    if (GC.MenuType == "CritterRMB")
    {
        GC.MenuPos = sf::Vector2f(-10000, -10000);
        GC.MenuEndPos = sf::Vector2f(-10000, -10000);
        GC.MenuType = "NULL";
    }

    if (GC.MenuType == "ItemContext")
    {

        int Options = 8;
        GC.MenuEndPos = sf::Vector2f(GC.MenuPos.x + 150,
                                     (GC.MenuPos.y + (Options * 13)) + 5);
        effects.createSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, sf::Color::Black, 2,
                             sf::Color::Cyan);
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
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                if (GC.MenuPtrCon.pItem->pickupable)
                    cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13),
                                     12, sf::Color::White,
                                     GC.MenuPtrCon.pItem->name + " - PickUp");
                else
                    cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13),
                                     12, sf::Color::White,
                                     GC.MenuPtrCon.pItem->name +
                                         " - xPickUpx, Cannot be picked up.");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Makes someone pickup this item.");
                if ((squareButtonClicked(Butt) &&
                     GC.MenuPtrCon.pItem->pickupable) ||
                    (Key.num1Time == 1 && GC.MenuPtrCon.pItem->pickupable))
                {

                    for (size_t i = 0; i != UniFact.size(); i++)
                    {
                        if (UniFact[i].PlayerControlled)
                        {
                            Job job;
                            job.pItem = GC.MenuPtrCon.pItem;

                            job.Name = "PickUpItem";
                            job.Type = "PickUp";

                            UniFact[i].JobList.push_back(job);
                        }
                    }
                }
            }

            if (i == 1)
            {
                effects.createLine(
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                if (!GC.MenuPtrCon.pItem->pickupable)
                    cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13),
                                     12, sf::Color::White,
                                     GC.MenuPtrCon.pItem->name + " - ChopDown");
                else
                    cText.CreateText(
                        GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                        sf::Color::White,
                        GC.MenuPtrCon.pItem->name +
                            " - xChopDownx, Cannot be chopped up.");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol,
                    "Makes someone seek and destroy this item.");
                if ((squareButtonClicked(Butt) &&
                     !GC.MenuPtrCon.pItem->pickupable) ||
                    (Key.num1Time == 1 && !GC.MenuPtrCon.pItem->pickupable))
                {

                    for (size_t i = 0; i != UniFact.size(); i++)
                    {
                        if (UniFact[i].PlayerControlled)
                        {
                            Job job;
                            job.pItem = GC.MenuPtrCon.pItem;

                            job.Name = "ChopDownTree";
                            job.Type = "Chop";

                            UniFact[i].JobList.push_back(job);
                        }
                    }
                }
            }

            iY++;
        }
    }

    if (GC.MenuType == "ItemRMB")
    {
        int Options = 8;
        GC.MenuEndPos = sf::Vector2f(GC.MenuPos.x + 150,
                                     (GC.MenuPos.y + (Options * 13)) + 5);
        effects.createSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, sf::Color::Black, 2,
                             sf::Color::Cyan);
        int iY = 0;
        int BRD = 140;                      // ButtonRightDisplacement.
        int BS = 7;                         // ButtonSize;
        int BSY = 5;                        // ButtonSize;
        int MBD = 8;                        // MoveButtonDown
        sf::Color ButCol = sf::Color::Cyan; // ButtonColor.

        for (size_t i = 0; i != GC.MenuPtrCon.pVecItem.size(); i++)
        {

            effects.createLine(GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                               GC.MenuPos.x + 90,
                               (GC.MenuPos.y + (iY * 13)) + 13, 1,
                               sf::Color::Cyan);
            cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                             sf::Color::White, GC.MenuPtrCon.pVecItem[i]->name);
            int Butt = createSquareButton(
                math::Vec2f(GC.MenuPos.x + BRD,
                            (GC.MenuPos.y + (iY * 13)) + MBD),
                BS, BSY, ButCol, "Strikes the location!");
            if (squareButtonClicked(Butt))
            {

                GC.MenuType = "ItemContext";
                GC.MenuPtrCon.pItem = GC.MenuPtrCon.pVecItem[i];
                GC.MenuPtrCon.pVecItem.clear();
                return;
            }

            if (/*i == -1*/ false) // `i` can't be -1
            {
                effects.createLine(
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 sf::Color::White,
                                 GC.MenuPtrCon.pVecItem[0]->name);
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Strikes the location!");
                if (squareButtonClicked(Butt) || Key.num1Time == 1)
                {

                    GC.MenuPos = sf::Vector2f(-10000, -10000);
                    GC.MenuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }

            iY++;
        }
    }

    if (GC.MenuType == "DebugFunctions")
    {

        int Options = 8;
        GC.MenuEndPos = sf::Vector2f(GC.MenuPos.x + 150,
                                     (GC.MenuPos.y + (Options * 13)) + 5);
        effects.createSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, sf::Color::Black, 2,
                             sf::Color::Cyan);
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
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Close Menu");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Close Menu");
                if (squareButtonClicked(Butt) || Key.num1Time == 1)
                {
                    GC.MenuPos = sf::Vector2f(-10000, -10000);
                    GC.MenuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }
            if (i == 1)
            {
                effects.createLine(
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Missile Strike");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Missile Strike");
                if (squareButtonClicked(Butt) || Key.num2Time == 1)
                {

                    effects.createSquare(GC.MenuPos.x - 10, GC.MenuPos.y - 2,
                                         GC.MenuPos.x + 10, GC.MenuPos.y + 2,
                                         sf::Color::Black);
                    effects.createSquare(GC.MenuPos.x - 2, GC.MenuPos.y - 10,
                                         GC.MenuPos.x + 2, GC.MenuPos.y + 10,
                                         sf::Color::Black);
                    Item Var;
                    Var = *getGlobalItem("Missile");
                    Var.targetPos = sf::Vector2f(GC.MenuPos);
                    Var.xpos = GC.MenuPos.x - 200;
                    Var.ypos = GC.MenuPos.y - 200;
                    Var.zpos = 100;
                    worlditems.push_back(Var);

                    GC.MenuPos = sf::Vector2f(-10000, -10000);
                    GC.MenuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }

            if (i == 2)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Shift Wall/Stone");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (squareButtonClicked(Butt) || Key.num3Time == 1)
                {

                    if (Tiles[abs_to_index(GC.MenuPos.x / GridSize)]
                             [abs_to_index(GC.MenuPos.y / GridSize)][30].ID !=
                        1010)
                    {
                        Tiles[abs_to_index(GC.MenuPos.x / GridSize)]
                             [abs_to_index(GC.MenuPos.y / GridSize)][30].Wall();
                        fSleep(0.2);
                    }
                    else
                    {
                        Tiles[abs_to_index(GC.MenuPos.x / GridSize)]
                             [abs_to_index(GC.MenuPos.y / GridSize)][30]
                                 .Stone();
                        fSleep(0.2);
                    }

                    Con("TODO: Make this a Toggle Mode");

                    GC.MenuPos = sf::Vector2f(-10000, -10000);
                    GC.MenuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }

            if (i == 3)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Spawn Zombie Horde");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (squareButtonClicked(Butt) || Key.num4Time == 1)
                {

                    GC.ZombieSwarmLocal();

                    GC.MenuPos = sf::Vector2f(-10000, -10000);
                    GC.MenuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }

            if (i == 4)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Print NPC's ConsoleInfo");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (squareButtonClicked(Butt) || Key.num5Time == 1)
                {

                    std::vector<NPC>::iterator zit;
                    for (zit = npclist.begin(); zit != npclist.end(); ++zit)
                    {
                        if (zit->alive == true)
                        {
                            zit->PrintConsoleInfo();
                        }
                    }

                    GC.MenuPos = sf::Vector2f(-10000, -10000);
                    GC.MenuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }

            if (i == 5)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 13), 12,
                                 sf::Color::White,
                                 "Give Everyone Zombification");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol,
                    "You horrible monster, Why would you do this?!");
                if (squareButtonClicked(Butt) || Key.num6Time == 1)
                {

                    std::vector<NPC>::iterator zit;
                    for (zit = npclist.begin(); zit != npclist.end(); ++zit)
                    {
                        zit->bloodwork("Zombification", 100);
                    }

                    GC.MenuPos = sf::Vector2f(-10000, -10000);
                    GC.MenuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }

            if (i == 6)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Delete all critters");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (squareButtonClicked(Butt) || Key.num7Time == 1)
                {
                    for (size_t i = 0; i != npclist.size(); i++)
                    {
                        npclist.at(i).ToDelete = true;
                    }
                }
            }

            if (i == 7)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                effects.createLine(
                    GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                    GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 13)) + 13, 1,
                    sf::Color::Cyan);
                cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 13), 12,
                                 sf::Color::White, "Starve all critters");
                int Butt = createSquareButton(
                    math::Vec2f(GC.MenuPos.x + BRD,
                                (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (squareButtonClicked(Butt) || Key.num8Time == 1)
                {
                    for (size_t i = 0; i != npclist.size(); i++)
                    {
                        npclist.at(i).hunger = 50;
                    }
                }
            }

            iY++;
        }
    }

    if (GC.MenuType == "DebugItemSpawn")
    {
        GC.MenuEndPos = sf::Vector2f(
            GC.MenuPos.x + 100,
            (GC.MenuPos.y + (itemmanager.globalItems.size() * 10)) + 5);
        effects.createSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, sf::Color::White);
        int iY = 0;
        for (auto &elem : itemmanager.globalItems)
        {
            effects.createLine(GC.MenuPos.x, (GC.MenuPos.y + (iY * 11)) + 8,
                               GC.MenuPos.x + 90,
                               (GC.MenuPos.y + (iY * 11)) + 8, 3,
                               sf::Color::Black, 1, sf::Color::Yellow);
            cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 11), 11,
                             sf::Color::Cyan, elem.name);
            int Butt = createButton(
                math::Vec2f(GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 11)) + 5),
                5, sf::Color::Red);
            if (buttonClicked(Butt))
            {
                spawnItem(elem.name, GC.MenuPos.x, GC.MenuPos.y);
                fSleep(0.2);
            }
            iY++;
        }
    }

    if (GC.MenuType == "DebugCritterSpawn")
    {
        GC.MenuEndPos = sf::Vector2f(
            GC.MenuPos.x + 100,
            (GC.MenuPos.y + (npcmanager.GlobalCritter.size() * 10)) + 5);
        effects.createSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, sf::Color::White);
        int iY = 0;
        for (auto &elem : npcmanager.GlobalCritter)
        {
            effects.createLine(GC.MenuPos.x, (GC.MenuPos.y + (iY * 11)) + 8,
                               GC.MenuPos.x + 90,
                               (GC.MenuPos.y + (iY * 11)) + 8, 3,
                               sf::Color::Black, 1, sf::Color::Yellow);
            cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 11), 11,
                             sf::Color::Cyan, elem.name);
            int Butt = createButton(
                math::Vec2f(GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 11)) + 5),
                5, sf::Color::Red);
            if (buttonClicked(Butt))
            {
                SpawnCritter(elem.name, GC.MenuPos.x, GC.MenuPos.y);
                fSleep(0.2);
                break;
            }
            iY++;
        }
    }

    if (GC.MenuType == "CritterContext" && true == false)
    {
        int Options = 2;
        GC.MenuEndPos = sf::Vector2f(GC.MenuPos.x + 100,
                                     (GC.MenuPos.y + (Options * 10)) + 5);
        effects.createSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, sf::Color::White);
        int iY = 0;
        for (int i = 0; i != Options; i++)
        {

            if (i == 0)
            {

                effects.createLine(GC.MenuPos.x, (GC.MenuPos.y + (iY * 11)) + 8,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 11)) + 8, 3,
                                   sf::Color::Black, 1, sf::Color::Yellow);
                cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 11), 11,
                                 sf::Color::Cyan, "Close Menu");
                int Butt =
                    createButton(math::Vec2f(GC.MenuPos.x + 90,
                                             (GC.MenuPos.y + (iY * 11)) + 5),
                                 5, sf::Color::Red);
                if (buttonClicked(Butt))
                {

                    std::list<Item>::iterator Item;
                    for (Item = worlditems.begin(); Item != worlditems.end();
                         Item++)
                    {
                        GC.MenuPos = sf::Vector2f(-10000, -10000);
                        GC.MenuType = "NULL";
                    }

                    fSleep(0.2);
                    break;
                }
            }
            if (i == 1)
            {

                effects.createLine(GC.MenuPos.x, (GC.MenuPos.y + (iY * 11)) + 8,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 11)) + 8, 3,
                                   sf::Color::Black, 1, sf::Color::Yellow);
                cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 11), 11,
                                 sf::Color::Cyan, "PickUp");
                int Butt =
                    createButton(math::Vec2f(GC.MenuPos.x + 90,
                                             (GC.MenuPos.y + (iY * 11)) + 5),
                                 5, sf::Color::Red);
                if (buttonClicked(Butt))
                {

                    std::list<Item>::iterator Item;
                    for (Item = worlditems.begin(); Item != worlditems.end();
                         Item++)
                    {
                        if (math::closeish(GC.MenuPos.x, GC.MenuPos.y,
                                           Item->xpos, Item->ypos) <= 10)
                        {
                            if (Item->pickupable == true)
                            {
                                npclist.at(GetNpcVectorId(gvars::MyTargetid))
                                    .action = "Pickup";
                                npclist.at(GetNpcVectorId(gvars::MyTargetid))
                                    .target = Item->name;
                                npclist.at(GetNpcVectorId(gvars::MyTargetid))
                                    .TargetId = Item->id;
                                npclist.at(GetNpcVectorId(gvars::MyTargetid))
                                    .TargetPos =
                                    sf::Vector2f(Item->xpos, Item->ypos);
                                GC.MenuPos = sf::Vector2f(-10000, -10000);
                                GC.MenuType = "NULL";
                            }
                        }
                    }

                    fSleep(0.2);
                    break;
                }
            }

            iY++;
        }

        if (gvars::MyTarget == -1 && GC.MenuType == "CritterContext")
        {
            GC.MenuPos = sf::Vector2f(-10000, -10000);
            GC.MenuType = "NULL";
            Con("Closing Menu due to No Target && CritterContext");
        }
    }
}

void rightMouseButtonContextMenu()
{
    if (Key.RMBTime ==
        1) // This is the RightMouseButton's context menu for Tiles/Critters/Items, This will probably be rather robust
    {

        for (size_t i = 0; i != npclist.size(); i++)
        {
            if (math::closeish(gvars::MousePos.x, gvars::MousePos.y,
                               npclist[i].xpos,
                               npclist[i].ypos) <= npclist[i].size)
            {
                GC.MenuType = "CritterRMB";
                menuPopUp();
                return;
            }
        }

        for (auto &worlditem : worlditems)
        {
            if (math::closeish(gvars::MousePos.x, gvars::MousePos.y,
                               (worlditem).xpos, (worlditem).ypos) <= 10)
            {
                GC.MenuType = "ItemRMB";
                GC.MenuPtrCon.pVecItem.push_back(&(worlditem));
            }
        }
        if (GC.MenuPtrCon.pVecItem.size() != 0)
        {
            menuPopUp();
            return;
        }

        GC.MenuType = "BlankRMB";
        menuPopUp();
    }
}
