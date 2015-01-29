#include "menus.h"

#include "Effects.h"
#include "Text.h"
#include "keys.h"
#include "Galaxy.h"
#include "FactionJobSecurity.h"
#include "Faction.h"

#include <iostream>

std::vector<button> vButtonList;
std::vector<squarebutton> vSquareButtonList;
MenuPointerContainer MenuPtrCon;

using std::abs;

void button::DrawButton()
{
    if (BeenPressed == true)
    {
        //Effectz.CreateCircle(vPos.x,vPos.y,iSize,sf::Color(Color.r/2,Color.g/2,Color.b/2),2,White);
        Effectz.CreateSquare(vPos.x - iSize, vPos.y - (iSize / 1.5),
                             vPos.x + iSize, vPos.y + (iSize / 1.5),
                             sf::Color(Color.r / 2, Color.g / 2, Color.b / 2),
                             2, White);
    }
    else if (Math::Closeish(MousePos.x, MousePos.y, vPos.x, vPos.y) < iSize)
    {
        //Effectz.CreateCircle(vPos.x,vPos.y,iSize,Color,2,White);
        Effectz.CreateSquare(vPos.x - iSize, vPos.y - (iSize / 1.5),
                             vPos.x + iSize, vPos.y + (iSize / 1.5), Color, 2,
                             White);
    }
    else
    {
        //Effectz.CreateCircle(vPos.x,vPos.y,iSize,Color,1,Black);
        Effectz.CreateSquare(vPos.x - iSize, vPos.y - (iSize / 1.5),
                             vPos.x + iSize, vPos.y + (iSize / 1.5), Color, 2,
                             Black);
    }
    cText.CreateText(vPos.x + 10, vPos.y - (TextSize / 2), TextSize, TextColor,
                     sForwardText);
    Effectz.DrawEffects();
}

button::button() : iSize{}, TextSize{}
{
    BeenPressed = false;
    id = Globals.glbbtn++;
}

void squarebutton::DrawButton()
{
    if (BeenPressed == true)
    {
        //Effectz.CreateCircle(vPos.x,vPos.y,iSize,sf::Color(Color.r/2,Color.g/2,Color.b/2),2,White);
        Effectz.CreateSquare(
            vPos.x - iSizex, vPos.y - iSizey, vPos.x + iSizex, vPos.y + iSizey,
            sf::Color(Color.r / 2, Color.g / 2, Color.b / 2), 2, White);
    }
    else if (AABB(MousePos, vPos.x - iSizex, vPos.x + iSizex, vPos.y - iSizey,
                  vPos.y + iSizey))
    {
        //Effectz.CreateCircle(vPos.x,vPos.y,iSize,Color,2,White);
        Effectz.CreateSquare(vPos.x - iSizex, vPos.y - iSizey, vPos.x + iSizex,
                             vPos.y + iSizey, Color, 2, White);
        if (MouseStagnation > 10 && sButtonText.length() != 0)
        {
            Effectz.CreateSquare(MousePos.x + 10, MousePos.y - 6,
                                 MousePos.x + (sButtonText.length() * 7),
                                 MousePos.y + 6, sf::Color::Black, 1,
                                 sf::Color(175, 175, 0));
            cText.CreateText(MousePos.x + 12, MousePos.y - 6, 11, TextColor,
                             sButtonText);
        }
    }
    else
    {
        //Effectz.CreateCircle(vPos.x,vPos.y,iSize,Color,1,Black);
        Effectz.CreateSquare(vPos.x - iSizex, vPos.y - iSizey, vPos.x + iSizex,
                             vPos.y + iSizey, Color, 2, Black);
    }
    cText.CreateText(vPos.x + 10, vPos.y - (TextSize / 2), TextSize, TextColor,
                     sForwardText);
    Effectz.DrawEffects();
}

squarebutton::squarebutton() : iSizex{}, iSizey{}, TextSize{}
{
    BeenPressed = false;
    TextColor = sf::Color(175, 175, 0);
    id = Globals.glbbtn++;
}

void Buttons::ButtonPressed()
{
    for (auto const &button : vButtonList)
    {
        if (Math::Closeish(MousePos.x, MousePos.y, button.vPos.x,
                           button.vPos.y) < button.iSize)
        {
            std::cout << "Button Click!\n";
        }
    }
}

void SquareButtons::ButtonPressed()
{
    for (auto const &button : vSquareButtonList)
    {
        if (AABB(MousePos, button.vPos.x - button.iSizex,
                 button.vPos.x + button.iSizex, button.vPos.y - button.iSizey,
                 button.vPos.y + button.iSizey))
        {
            std::cout << "Button Click!\n";
            // Is this never called? Is this class useless?
        }
    }
}

int CreateButton(sf::Vector2f vPos, int iSize, sf::Color Color,
                 std::string Text)
{
    button var;
    var.vPos = vPos;
    var.iSize = iSize;
    var.Color = Color;
    var.sButtonText = Text;
    vButtonList.push_back(var);
    return var.id;
}

int CreateSquareButton(sf::Vector2f vPos, int iSizex, int iSizey,
                       sf::Color Color, std::string Text)
{
    squarebutton var;
    var.vPos = vPos;
    var.iSizex = iSizex;
    var.iSizey = iSizey;
    var.Color = Color;
    var.sButtonText = Text;
    vSquareButtonList.push_back(var);
    return var.id;
}

bool ButtonClicked(int id)
{
    for (auto &button : vButtonList)
    {
        if (button.id == id)
        {
            if (Math::Closeish(MousePos.x, MousePos.y, button.vPos.x,
                               button.vPos.y) < button.iSize &&
                (Key.LMBTime == 1 || Key.LMBTime > 20))
            {
                button.BeenPressed = true;
                std::cout << "Pressed! \n";
                Globals.ButtonClicked = true;
                return true;
            }
        }
    }
    return false;
}

bool SquareButtonClicked(int id)
{
    for (auto &button : vSquareButtonList)
    {
        if (button.id == id)
        {
            if (AABB(MousePos, button.vPos.x - button.iSizex,
                     button.vPos.x + button.iSizex,
                     button.vPos.y - button.iSizey,
                     button.vPos.y + button.iSizey) &&
                (Key.LMBTime == 1 || Key.LMBTime > 20))
            {
                button.BeenPressed = true;
                std::cout << "Pressed! \n";
                Globals.ButtonClicked = true;
                Globals.ButtonClickedTime = 3;
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

void
MenuPopUp() // TODO: Add functionality to allow you to press 1-9 to activate the menu buttons.
{

    if (GC.MenuPos.x == -10000)
    {
        GC.MenuPos = MousePos;
    }

    sf::Vector2f Tiled(abs(GC.MenuPos.x / 20) * 20,
                       abs(GC.MenuPos.y / 20) * 20);

    Effectz.CreateSquare(Tiled.x, Tiled.y, Tiled.x + 20, Tiled.y + 20,
                         sf::Color::Transparent, 1, sf::Color::Cyan);

    if (GC.MenuType == "Orbital Drop")
    {

        int Options = 8;
        GC.MenuEndPos = sf::Vector2f(GC.MenuPos.x + 150,
                                     (GC.MenuPos.y + (Options * 13)) + 5);
        Effectz.CreateSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, sf::Color::Black, 2,
                             sf::Color::Cyan);
        int iY = 0;
        int BRD = 140;           // ButtonRightDisplacement.
        int BS = 7;              // ButtonSize;
        int BSY = 5;             // ButtonSize;
        int MBD = 8;             // MoveButtonDown
        sf::Color ButCol = Cyan; // ButtonColor.

        for (int i = 0; i != Options; i++)
        {

            if (i == 0)
            {
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 White, "Orbital Drop - Missle");
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Strikes the location!");
                if (SquareButtonClicked(Butt) || Key.num1Time == 1)
                {
                    Effectz.CreateSquare(GC.MenuPos.x - 10, GC.MenuPos.y - 2,
                                         GC.MenuPos.x + 10, GC.MenuPos.y + 2,
                                         sf::Color::Black);
                    Effectz.CreateSquare(GC.MenuPos.x - 2, GC.MenuPos.y - 10,
                                         GC.MenuPos.x + 2, GC.MenuPos.y + 10,
                                         sf::Color::Black);
                    item Var;
                    Var = *GetGlobalItem("Missile");
                    Var.TargetPos = sf::Vector2f(GC.MenuPos);
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
        Effectz.CreateSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, sf::Color::Black, 2,
                             sf::Color::Cyan);
        int iY = 0;
        int BRD = 140;           // ButtonRightDisplacement.
        int BS = 7;              // ButtonSize;
        int BSY = 5;             // ButtonSize;
        int MBD = 8;             // MoveButtonDown
        sf::Color ButCol = Cyan; // ButtonColor.

        for (int i = 0; i != Options; i++)
        {

            if (i == 0)
            {
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 White, "Build - Wall");
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Makes a Wall!");
                if (SquareButtonClicked(Butt) || Key.num1Time == 1)
                {
                    RMBMenuTile(GC.MenuPos);
                }
            }

            if (i == 1)
            {
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 White, "Dig - Natural Wall");
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Digs out a natural wall.");
                if (SquareButtonClicked(Butt) || Key.num1Time == 1)
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
        Effectz.CreateSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, sf::Color::Black, 2,
                             sf::Color::Cyan);
        int iY = 0;
        int BRD = 140;           // ButtonRightDisplacement.
        int BS = 7;              // ButtonSize;
        int BSY = 5;             // ButtonSize;
        int MBD = 8;             // MoveButtonDown
        sf::Color ButCol = Cyan; // ButtonColor.

        for (int i = 0; i != Options; i++)
        {

            if (i == 0)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 White, "Build");
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol,
                    "This opens the menu to build various structures!");
                if (SquareButtonClicked(Butt) || Key.num1Time == 1)
                {
                    GC.MenuType = "BuildStructure";
                    //fSleep(0.2);
                    return;
                }
            }
            if (i == 1)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 White, "Orbital Drop");
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Causes an orbital ship to release "
                                     "specific cargo at target location.");
                if (SquareButtonClicked(Butt) || Key.num2Time == 1)
                {

                    GC.MenuType = "Orbital Drop";
                    //fSleep(0.2);
                    return;
                }
            }

            if (i == -5)
            {

                //Effectz.CreateLine(GC.MenuPos.x,(GC.MenuPos.y+(iY*13))+8,GC.MenuPos.x+90,(GC.MenuPos.y+(iY*13))+8,3,Black,1,Yellow);
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 White, "Dig");
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Dig out the current wall tile");
                if (SquareButtonClicked(Butt) || Key.num2Time == 1)
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
        Effectz.CreateSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, sf::Color::Black, 2,
                             sf::Color::Cyan);
        int iY = 0;
        int BRD = 140;           // ButtonRightDisplacement.
        int BS = 7;              // ButtonSize;
        int BSY = 5;             // ButtonSize;
        int MBD = 8;             // MoveButtonDown
        sf::Color ButCol = Cyan; // ButtonColor.

        for (int i = 0; i != Options; i++)
        {

            if (i == 0)
            {
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                if (GC.MenuPtrCon.pItem->Pickupable)
                    cText.CreateText(
                        GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12, White,
                        AddString(GC.MenuPtrCon.pItem->name, " - PickUp"));
                else
                    cText.CreateText(
                        GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12, White,
                        AddString(GC.MenuPtrCon.pItem->name,
                                  " - xPickUpx, Cannot be picked up."));
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Makes someone pickup this item.");
                if ((SquareButtonClicked(Butt) &&
                     GC.MenuPtrCon.pItem->Pickupable) ||
                    (Key.num1Time == 1 && GC.MenuPtrCon.pItem->Pickupable))
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
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                if (!GC.MenuPtrCon.pItem->Pickupable)
                    cText.CreateText(
                        GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12, White,
                        AddString(GC.MenuPtrCon.pItem->name, " - ChopDown"));
                else
                    cText.CreateText(
                        GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12, White,
                        AddString(GC.MenuPtrCon.pItem->name,
                                  " - xChopDownx, Cannot be chopped up."));
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol,
                    "Makes someone seek and destroy this item.");
                if ((SquareButtonClicked(Butt) &&
                     !GC.MenuPtrCon.pItem->Pickupable) ||
                    (Key.num1Time == 1 && !GC.MenuPtrCon.pItem->Pickupable))
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
        Effectz.CreateSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, sf::Color::Black, 2,
                             sf::Color::Cyan);
        int iY = 0;
        int BRD = 140;           // ButtonRightDisplacement.
        int BS = 7;              // ButtonSize;
        int BSY = 5;             // ButtonSize;
        int MBD = 8;             // MoveButtonDown
        sf::Color ButCol = Cyan; // ButtonColor.

        for (size_t i = 0; i != GC.MenuPtrCon.pVecItem.size(); i++)
        {

            Effectz.CreateLine(GC.MenuPos.x, (GC.MenuPos.y + (iY * 13)) + 13,
                               GC.MenuPos.x + 90,
                               (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
            cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                             White, GC.MenuPtrCon.pVecItem[i]->name);
            int Butt = CreateSquareButton(
                Math::Vec(GC.MenuPos.x + BRD, (GC.MenuPos.y + (iY * 13)) + MBD),
                BS, BSY, ButCol, "Strikes the location!");
            if (SquareButtonClicked(Butt))
            {

                GC.MenuType = "ItemContext";
                GC.MenuPtrCon.pItem = GC.MenuPtrCon.pVecItem[i];
                GC.MenuPtrCon.pVecItem.clear();
                return;
            }

            if (/*i == -1*/ false) // `i` can't be -1
            {
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 White, GC.MenuPtrCon.pVecItem[0]->name);
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Strikes the location!");
                if (SquareButtonClicked(Butt) || Key.num1Time == 1)
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
        Effectz.CreateSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, sf::Color::Black, 2,
                             sf::Color::Cyan);
        int iY = 0;
        int BRD = 140;           // ButtonRightDisplacement.
        int BS = 7;              // ButtonSize;
        int BSY = 5;             // ButtonSize;
        int MBD = 8;             // MoveButtonDown
        sf::Color ButCol = Cyan; // ButtonColor.

        for (int i = 0; i != Options; i++)
        {

            if (i == 0)
            {
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 White, "Close Menu");
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Close Menu");
                if (SquareButtonClicked(Butt) || Key.num1Time == 1)
                {
                    GC.MenuPos = sf::Vector2f(-10000, -10000);
                    GC.MenuType = "NULL";

                    fSleep(0.2);
                    break;
                }
            }
            if (i == 1)
            {
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 White, "Missile Strike");
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol, "Missile Strike");
                if (SquareButtonClicked(Butt) || Key.num2Time == 1)
                {

                    Effectz.CreateSquare(GC.MenuPos.x - 10, GC.MenuPos.y - 2,
                                         GC.MenuPos.x + 10, GC.MenuPos.y + 2,
                                         sf::Color::Black);
                    Effectz.CreateSquare(GC.MenuPos.x - 2, GC.MenuPos.y - 10,
                                         GC.MenuPos.x + 2, GC.MenuPos.y + 10,
                                         sf::Color::Black);
                    item Var;
                    Var = *GetGlobalItem("Missile");
                    Var.TargetPos = sf::Vector2f(GC.MenuPos);
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
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 White, "Shift Wall/Stone");
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (SquareButtonClicked(Butt) || Key.num3Time == 1)
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
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                cText.CreateText(GC.MenuPos.x + 2, GC.MenuPos.y + (iY * 13), 12,
                                 White, "Spawn Zombie Horde");
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (SquareButtonClicked(Butt) || Key.num4Time == 1)
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
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 13), 12,
                                 White, "Print NPC's ConsoleInfo");
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (SquareButtonClicked(Butt) || Key.num5Time == 1)
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
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 13), 12,
                                 White, "Give Everyone Zombification");
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol,
                    "You horrible monster, Why would you do this?!");
                if (SquareButtonClicked(Butt) || Key.num6Time == 1)
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
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 13), 12,
                                 White, "Delete all critters");
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (SquareButtonClicked(Butt) || Key.num7Time == 1)
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
                Effectz.CreateLine(GC.MenuPos.x,
                                   (GC.MenuPos.y + (iY * 13)) + 13,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 13)) + 13, 1, Cyan);
                cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 13), 12,
                                 White, "Starve all critters");
                int Butt = CreateSquareButton(
                    Math::Vec(GC.MenuPos.x + BRD,
                              (GC.MenuPos.y + (iY * 13)) + MBD),
                    BS, BSY, ButCol);
                if (SquareButtonClicked(Butt) || Key.num8Time == 1)
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
            (GC.MenuPos.y + (itemmanager.GlobalItem.size() * 10)) + 5);
        Effectz.CreateSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, White);
        int iY = 0;
        for (auto &elem : itemmanager.GlobalItem)
        {
            Effectz.CreateLine(
                GC.MenuPos.x, (GC.MenuPos.y + (iY * 11)) + 8, GC.MenuPos.x + 90,
                (GC.MenuPos.y + (iY * 11)) + 8, 3, Black, 1, Yellow);
            cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 11), 11, Cyan,
                             elem.name);
            int Butt = CreateButton(
                Math::Vec(GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 11)) + 5), 5,
                Red);
            if (ButtonClicked(Butt))
            {
                SpawnItem(elem.name, GC.MenuPos.x, GC.MenuPos.y);
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
        Effectz.CreateSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, White);
        int iY = 0;
        for (auto &elem : npcmanager.GlobalCritter)
        {
            Effectz.CreateLine(
                GC.MenuPos.x, (GC.MenuPos.y + (iY * 11)) + 8, GC.MenuPos.x + 90,
                (GC.MenuPos.y + (iY * 11)) + 8, 3, Black, 1, Yellow);
            cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 11), 11, Cyan,
                             elem.name);
            int Butt = CreateButton(
                Math::Vec(GC.MenuPos.x + 90, (GC.MenuPos.y + (iY * 11)) + 5), 5,
                Red);
            if (ButtonClicked(Butt))
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
        Effectz.CreateSquare(GC.MenuPos.x, GC.MenuPos.y, GC.MenuEndPos.x,
                             GC.MenuEndPos.y, White);
        int iY = 0;
        for (int i = 0; i != Options; i++)
        {

            if (i == 0)
            {

                Effectz.CreateLine(GC.MenuPos.x, (GC.MenuPos.y + (iY * 11)) + 8,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 11)) + 8, 3, Black, 1,
                                   Yellow);
                cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 11), 11,
                                 Cyan, "Close Menu");
                int Butt =
                    CreateButton(Math::Vec(GC.MenuPos.x + 90,
                                           (GC.MenuPos.y + (iY * 11)) + 5),
                                 5, Red);
                if (ButtonClicked(Butt))
                {

                    std::list<item>::iterator Item;
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

                Effectz.CreateLine(GC.MenuPos.x, (GC.MenuPos.y + (iY * 11)) + 8,
                                   GC.MenuPos.x + 90,
                                   (GC.MenuPos.y + (iY * 11)) + 8, 3, Black, 1,
                                   Yellow);
                cText.CreateText(GC.MenuPos.x, GC.MenuPos.y + (iY * 11), 11,
                                 Cyan, "PickUp");
                int Butt =
                    CreateButton(Math::Vec(GC.MenuPos.x + 90,
                                           (GC.MenuPos.y + (iY * 11)) + 5),
                                 5, Red);
                if (ButtonClicked(Butt))
                {

                    std::list<item>::iterator Item;
                    for (Item = worlditems.begin(); Item != worlditems.end();
                         Item++)
                    {
                        if (Math::Closeish(GC.MenuPos.x, GC.MenuPos.y,
                                           Item->xpos, Item->ypos) <= 10)
                        {
                            if (Item->Pickupable == true)
                            {
                                npclist.at(GetNpcVectorId(MyTargetid)).action =
                                    "Pickup";
                                npclist.at(GetNpcVectorId(MyTargetid)).target =
                                    Item->name;
                                npclist.at(GetNpcVectorId(MyTargetid))
                                    .TargetId = Item->id;
                                npclist.at(GetNpcVectorId(MyTargetid))
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

        if (MyTarget == -1 && GC.MenuType == "CritterContext")
        {
            GC.MenuPos = sf::Vector2f(-10000, -10000);
            GC.MenuType = "NULL";
            Con("Closing Menu due to No Target && CritterContext");
        }
    }
}

void RightMouseButtonContextMenu()
{
    if (Key.RMBTime ==
        1) // This is the RightMouseButton's context menu for Tiles/Critters/Items, This will probably be rather robust
    {

        for (size_t i = 0; i != npclist.size(); i++)
        {
            if (Math::Closeish(MousePos.x, MousePos.y, npclist[i].xpos,
                               npclist[i].ypos) <= npclist[i].size)
            {
                GC.MenuType = "CritterRMB";
                MenuPopUp();
                return;
            }
        }

        for (auto &worlditem : worlditems)
        {
            if (Math::Closeish(MousePos.x, MousePos.y, (worlditem).xpos,
                               (worlditem).ypos) <= 10)
            {
                GC.MenuType = "ItemRMB";
                GC.MenuPtrCon.pVecItem.push_back(&(worlditem));
            }
        }
        if (GC.MenuPtrCon.pVecItem.size() != 0)
        {
            MenuPopUp();
            return;
        }

        GC.MenuType = "BlankRMB";
        MenuPopUp();
    }
}
