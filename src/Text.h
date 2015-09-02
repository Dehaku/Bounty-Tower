#ifndef __TEXT_H_INCLUDED__
#define __TEXT_H_INCLUDED__

#include <SFML/Graphics.hpp>
#include "util.h"
#include "globalvars.h"

sf::Text createText(std::string Text);

class TextList
{
public:
    int xpos;
    int ypos;
    sf::Color color;
    sf::Text sString;
    sf::Font font;
    int size;
    bool shadow;
    bool bold;
    sf::View drawView;

    void setup();
    void drawMe();
    std::vector<TextList> textlist;

    void createText(int xpos, int ypos, int sizeMe, sf::Color color,
                    std::string stringvalue1, std::string string1 = "",
                    float int1 = -6698, std::string stringvalue2 = "",
                    std::string string2 = "", float int2 = -6698,
                    std::string stringvalue3 = "", std::string string3 = "",
                    float int3 = -6698, int shadow = 1, int null3 = 0);

    void createText(sf::Vector2f vPos, int sizeMe, sf::Color color,
                    std::string stringvalue, sf::View drawView = gvars::view1);

    void drawTextz();

    void loadFont();

    TextList();
};
extern TextList textList;

sf::Text drawText(sf::Vector2f vPos, std::string text, sf::Color color = sf::Color::White, int size = 10);

class ChatBox
{
public:
    class ChatLine
    {
    public:
        std::string line;
        sf::Color color;
    };

    std::vector<ChatLine> chatStorage;

    void addChat(std::string text, sf::Color color);

    void displayChat(sf::Vector2f position);
};
extern ChatBox chatBox;

std::string randomWindowName();

#endif // TEXT_H_INCLUDED
