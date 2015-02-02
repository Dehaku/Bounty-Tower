#ifndef __MENUS_H_INCLUDED__
#define __MENUS_H_INCLUDED__

#include "Items.h"
#include "Tiles.h"

#include <SFML/Graphics.hpp>

// TODO: Add a tooltip to the buttons, When you hover over it,
// it will display some text

class Button
{
public:
    bool beenPressed;
    int iSize;
    sf::Vector2f vPos;
    sf::Color color;
    std::string sFunction;
    std::string sButtonText;
    std::string sForwardText;
    int textSize;
    sf::Color textColor;
    int id;

    void draw();

    Button();
};

class SquareButton
{
public:
    bool beenPressed;
    int iSizex;
    int iSizey;
    sf::Vector2f vPos;
    sf::Color color;
    std::string sFunction;
    std::string sButtonText;
    std::string sForwardText;
    int textSize;
    sf::Color textColor;
    int id;

    void draw();

    SquareButton();
};

extern std::vector<Button> vButtonList;
extern std::vector<SquareButton> vSquareButtonList;

class Buttons
{
public:
    void pressed();
};

class SquareButtons
{
public:
    void pressed();
};

int createButton(sf::Vector2f vPos, int iSize, sf::Color Color,
                 std::string Text = "");

int createSquareButton(sf::Vector2f vPos, int iSizex, int iSizey,
                       sf::Color Color, std::string Text = "");

bool buttonClicked(int id);

bool squareButtonClicked(int id);

class MenuPointerContainer
{
public:
    item *pItem;
    NPC *pNPC;
    Tile *pTile;
    MenuPointerContainer();
};
extern MenuPointerContainer menuPtrCon;

void menuPopUp();

void rightMouseButtonContextMenu();

#endif // MENUS_H_INCLUDED
