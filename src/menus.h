#ifndef __MENUS_H_INCLUDED__
#define __MENUS_H_INCLUDED__

#include "Items.h"
#include "Tiles.h"

#include <SFML/Graphics.hpp>

class button
{
public:
    bool
        BeenPressed; // TODO: Add a tooltip to the buttons, When you hover over it, it will display some text
    int iSize;
    sf::Vector2f vPos;
    sf::Color Color;
    std::string sFunction;
    std::string sButtonText;
    std::string sForwardText;
    int TextSize;
    sf::Color TextColor;
    int id;

    void DrawButton();

    button();
};

class squarebutton
{
public:
    bool
        BeenPressed; // TODO: Add a tooltip to the buttons, When you hover over it, it will display some text
    int iSizex;
    int iSizey;
    sf::Vector2f vPos;
    sf::Color Color;
    std::string sFunction;
    std::string sButtonText;
    std::string sForwardText;
    int TextSize;
    sf::Color TextColor;
    int id;

    void DrawButton();

    squarebutton();
};

extern std::vector<button> vButtonList;
extern std::vector<squarebutton> vSquareButtonList;

class Buttons
{
public:
    void ButtonPressed();
};

class SquareButtons
{
public:
    void ButtonPressed();
};

int CreateButton(sf::Vector2f vPos, int iSize, sf::Color Color,
                 std::string Text = "");

int CreateSquareButton(sf::Vector2f vPos, int iSizex, int iSizey,
                       sf::Color Color, std::string Text = "");

bool ButtonClicked(int id);

bool SquareButtonClicked(int id);

class MenuPointerContainer
{
public:
    item *pItem;
    NPC *pNPC;
    Tile *pTile;
    MenuPointerContainer();
};
extern MenuPointerContainer MenuPtrCon;

void MenuPopUp();

void RightMouseButtonContextMenu();

#endif // MENUS_H_INCLUDED
