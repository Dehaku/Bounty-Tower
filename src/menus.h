#ifndef __MENUS_H_INCLUDED__
#define __MENUS_H_INCLUDED__

#include "Items.h"
#include "Tiles.h"
#include "BountyTower.h"

#include <SFML/Graphics.hpp>

// TODO: Add a tooltip to the buttons, When you hover over it,
// it will display some text
extern sf::RenderWindow window;

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

class ImageButton
{
public:
    bool beenPressed;
    sf::Sprite sprite;
    //sf::Vector2f vPos;
    sf::Color color;
    sf::View view;
    std::string sFunction;
    std::string sButtonText;
    std::string sForwardText;
    int textSize;
    sf::Color textColor;
    bool hovering;
    int id;

    void draw();

    ImageButton();
};

extern std::vector<Button> vButtonList;
extern std::vector<SquareButton> vSquareButtonList;
extern std::vector<ImageButton> vImageButtonList;

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

int createButton(sf::Vector2f vPos, int iSize, sf::Color color,
                 std::string text = "");

int createSquareButton(sf::Vector2f vPos, int iSizex, int iSizey,
                       sf::Color color, std::string text = "");
int createImageButton(sf::Vector2f vPos, sf::Texture &Tex, std::string text = "", int rotation = 0);
int createImageButton(sf::Vector2f vPos, const sf::Texture &Tex, std::string text = "", int rotation = 0);
int createImageButton(sf::Vector2f vPos, sf::Texture &Tex, std::string text, int rotation, sf::View viewTarget);


bool buttonClicked(int id);

bool squareButtonClicked(int id);
bool imageButtonClicked(int id);

bool imageButtonHovered(int id);

class MenuPointerContainer
{
public:
    Item *pItem;
    Npc *pNPC;
    Tile *pTile;
    MenuPointerContainer();
};
extern MenuPointerContainer menuPtrCon;

void menuPopUp();

void rightMouseButtonContextMenu();

#endif // MENUS_H_INCLUDED
